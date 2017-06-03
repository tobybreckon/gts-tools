//
// Filename: gts_common.cc
//
// Description: common GTS related functions
//
//  Copyright (C) 2006  Toby Breckon
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation,Inc.,51 Franklin Street,Fifth Floor,Boston,MA  02110-1301, USA.
//------------------------------------------------------------------------------

// system includes

#include <list>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;

// library includes

#include "gts_common.h"

// local includes

#include "datatypes.h"
#include "general.h"

//------------------------------------------------------------------------------

// build a list of surface vertices

static void build_vertex_list (gpointer data,  GtsVertexList *vertices)
{
	vertices->push_back((GtsVertex *) data);
}

void vertex_list_of(GtsSurface* theSurface, GtsVertexList *vertices)
{
	gts_surface_foreach_vertex(theSurface, (GtsFunc) build_vertex_list, &(*vertices));
}

// build a list of edge list

static void build_edge_list (gpointer data,  GtsEdgeList *edges)
{
	edges->push_back((GtsEdge *) data);
}

void edge_list_of(GtsSurface* theSurface, GtsEdgeList *edges)
{
	gts_surface_foreach_edge(theSurface, (GtsFunc) build_edge_list, &(*edges));
}

//------------------------------------------------------------------------------

// define the wieghting method used for normal calculation
// by default use Gourand (computationally cheapest).

GtsVertexWeightOptions GTS_VERTEX_NORMAL_WEIGHT = GTS_VERTEX_NORMAL_GOURAND;

//------------------------------------------------------------------------------

// calculate surface normal at a given vertex v on surface s
// return surface normal as x, y , z

// returns TRUE if possible to calculate normal -> else FALSE;

gboolean gts_vertex_normal(GtsSurface* s, GtsVertex* v, 
										double *x, double *y, double *z){

	GSList * faces; 			// list of faces
	GSList * current_face;
	
	GtsVector sum;				// weighted sum of vectors
	GtsVector n;
	
	GtsFace * f;				// pointer to face data
	GtsTriangle * t;            // pointer to triangle data
	
	double weight = 0; 					
	int i, no_of_faces;
	
	GtsPoint* centre; 		  // face centre for distance based weighting
	GtsEdge*  edge;			  // face edge for angle based wighting
	
	GtsVector edge_vector1, edge_vector2;
	
	sum[0] = 0;			
	sum[1] = 0;
	sum[2] = 0;
	no_of_faces = 0;
	
	// get a list of the faces of which the current vertex is a member
	
	faces = gts_vertex_faces(v, s, NULL);

	if(!(faces)){return false;}; // not possible to calculate a normal at a point
								 // without ajoining faces => return FALSE

	// get the surface normals of each face and calculate a wieghted mean

	current_face = faces;
	
	while (current_face){
			
			// N.B. Each face contains a triangle => we want the triangle of 
			// the pointer to the current face. And in fact we want a 
			// pointer to that triangle!

			f = GTS_FACE (current_face->data); // the face
			t = GTS_TRIANGLE (f); 			   // the triangle

			gts_triangle_normal(t, &n[0], &n[1], &n[2]);
			
			switch (GTS_VERTEX_NORMAL_WEIGHT)
			{
				case GTS_VERTEX_NORMAL_GOURAND :
				// use equal weighting between the face normals - Gourand, 1971
					weight = 1;
					break;
				case GTS_VERTEX_NORMAL_MAX :
				// use distance proportional weighting betwen the face 
				// normals - Max, 1999
					centre = gts_triangle_center(t, gts_point_class ());
					weight = (1 / gts_point_distance(centre, &(v->p)));
					break;
				case GTS_VERTEX_NORMAL_THURMER_WUTHRICH :
				// use vertex incident angle weighting 
				// - Thurmer / Wuthrich, 1998
					edge = gts_triangle_edge_opposite(t, v);
					gts_vector_init(edge_vector1, GTS_POINT(v), 
									GTS_POINT((edge->segment).v1));
					gts_vector_init(edge_vector2, GTS_POINT(v), 
									GTS_POINT((edge->segment).v2));
				
					weight = gts_vector_angle(edge_vector1, edge_vector2);
					break;
			}
			
			// maintain wieghted sum of normals
			
			for(i=0; i < 3; i++){
				sum[i] += weight * n[i];
			}
			no_of_faces++;
			
			current_face = current_face->next;
		}
		g_slist_free (faces);
	
		// calculate normal at v as mean of weighted sum
		
		for(i=0; i < 3; i++){
				sum[i] = (sum[i] / (double) no_of_faces);
		}
		
		// normalise, store and return
		
		gts_vector_normalize(sum);
		
		*x = sum[0];
		*y = sum[1];
		*z = sum[2];
		
		return true;
}

//------------------------------------------------------------------------------

// given two gts vectors return the (accute) angle between them in radians

double gts_vector_angle(GtsVector v1, GtsVector v2){
	
	GtsVector localV1;
	GtsVector localV2;
	double scalar;
	
	// make local copies and normalize these so as to leave
	// original vectors unchanged.
	
	gts_vector_copy(v1, localV1);
	gts_vector_copy(v2, localV2);
	
	gts_vector_normalize(localV1);
	gts_vector_normalize(localV2);
	
	// Also sanity check the resulting scalar
	// is in required range to acos to avoid 
	// returning "nan" due to precision issues
	
	scalar = gts_vector_scalar(localV1, localV2);
	
	if (scalar > 1.0 ){
		scalar = 1.0;
	} else if (scalar < -1.0){
		scalar = -1.0;
	}
	
	return acos(scalar);
	
}

//------------------------------------------------------------------------------

// given a pointer to a gts surface return the median edge length present
// on the surface

double gts_surface_median_edge_length(GtsSurface * s){

	GtsEdgeList edges;
	GtsEdgeList::iterator anEdge;
	
	vector<double> lengths;
	int midpoint;
	
	// get edges from surface
	
	edge_list_of(s, &edges);
	
	// get distribution of edge lengths
	
	for(anEdge = edges.begin(); anEdge != edges.end(); anEdge++){
		
		lengths.push_back(
			gts_point_distance(GTS_POINT(GTS_SEGMENT(*anEdge)->v1), 
										 GTS_POINT(GTS_SEGMENT(*anEdge)->v2)));
	}
	
	// sort distribution
	
	sort(lengths.begin(), lengths.end());
	
	midpoint = (int) floor((lengths.size() / 2.0) - 1);
	if (midpoint < 0){ midpoint = 0;}
	
	return lengths[midpoint];
	
}	
//------------------------------------------------------------------------------

// given a pointer to a gts triangle return it's centre as the mean of its 
// three vertices

GtsPoint * gts_triangle_center(GtsTriangle *t, GtsPointClass * point_class){

	GtsVertex * tv1, * tv2, * tv3;
	double px, py, pz;
	GtsPoint * p;
	
	gts_triangle_vertices(t, &tv1, &tv2, &tv3);
	
	px = (GTS_POINT(tv1)->x + GTS_POINT(tv2)->x + GTS_POINT(tv3)->x) / 3;
	py = (GTS_POINT(tv1)->y + GTS_POINT(tv2)->y + GTS_POINT(tv3)->y) / 3;
	pz = (GTS_POINT(tv1)->z + GTS_POINT(tv2)->z + GTS_POINT(tv3)->z) / 3;
	
	p = gts_point_new(point_class, px, py ,pz);
	
	return p;
	
}

//------------------------------------------------------------------------------

// given a pointer to a gts surface return a pointer to a newly created gts point 
// lying at the euclidean centroid of the surface (i.e. centre of gravity) 
// computed as the mean euclidean position of all vertex points belonging 
// to the surface

// N.B. This point (in most cases) will not lie on the surface.

static void gts_surface_centroid_worker(GtsVertex * v, GtsPoint * centroid){
	
	centroid->x += v->p.x;
  	centroid->y += v->p.y;	
	centroid->z += v->p.z;
}


GtsPoint * gts_surface_centroid(GtsSurface * s){
	
	GtsPoint * centroid = gts_point_new(gts_point_class(),0,0,0);
	
	gts_surface_foreach_vertex(s, (GtsFunc) gts_surface_centroid_worker, 
																	centroid);
	
	centroid->x = centroid->x / gts_surface_vertex_number(s);
	centroid->y = centroid->y / gts_surface_vertex_number(s);
	centroid->z = centroid->z / gts_surface_vertex_number(s);
	
	return centroid;
	
}
//------------------------------------------------------------------------------
