//
// Filename: gtstricutter.cc
//
// Description: remove specified triangles from GTS surface
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

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;

// Library includes

#include "gts_common.h" 

// local includes

#include "general.h"
#include "gts_input.h"
#include "output.h"

//------------------------------------------------------------------------------

// global variables for command line options

static double minLimit;
static double maxLimit;
static double neighbourLimit;
static double angle;

static GtsVector orientation;

// Cutting functions

// cut on area

static int area_tri_cutter(gpointer face, gpointer none){
	
	double area = gts_triangle_area(GTS_TRIANGLE((GtsFace *) face));
	
	if ((area < minLimit) || (area > maxLimit)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

//cut on edge length

static int edge_tri_cutter(gpointer face, gpointer none){

	GtsVertex *v1, *v2, *v3;
	GtsEdge *e1, *e2, *e3;
	
	gts_triangle_vertices_edges(GTS_TRIANGLE((GtsFace *) face),
                              NULL, &v1, &v2, &v3, &e1, &e2, &e3);
	
	double length1 = gts_point_distance(GTS_POINT(v1), GTS_POINT(v2));
	double length2 = gts_point_distance(GTS_POINT(v1), GTS_POINT(v3));
	double length3 = gts_point_distance(GTS_POINT(v2), GTS_POINT(v3));
	
	if  ((length1 < minLimit) || (length1 > maxLimit)){
		return TRUE;
	} else if ((length2 < minLimit) || (length2 > maxLimit)){
		return  TRUE;
	} else if ((length3 < minLimit) || (length3 > maxLimit)){
		return TRUE;
	} else {
		return FALSE;
	}
}

// cut if ioslated

static int isolated_tri_cutter(GtsFace * face, GtsSurface * s){

	// if a face has no nieghbours in the surface it is isolated
	
	if (gts_face_neighbor_number(face, s) <= neighbourLimit){
		return TRUE;
	} else {
		return FALSE;
	}
	
}

// cut if orientation is within angle of face normal


static int orientation_tri_cutter(GtsFace * face, gpointer none){

	// if a face surface normal is within an angle of the specified
	// orientation (or the inverse of it) 
	
	GtsVector normal;
	GtsVector inverseNormal;
	
	gts_triangle_normal(GTS_TRIANGLE(face), &normal[0], &normal[1], &normal[2]);
	
	gts_vector_invert(normal, inverseNormal);
	
	if ((gts_vector_angle(normal, orientation) < angle) || 
			(gts_vector_angle(inverseNormal, orientation) < angle))
	{
		return TRUE;
	} else {
		return FALSE;
	}
	
}

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	GtsSurface* theSurface; // pointer to surface

	int initial_faces;		// starting number of tri
	int initial_vertices;	// starting number of vertices

	int i;

	bool edgeRemoval = FALSE;
	bool areaRemoval = FALSE;
	bool isolatedRemoval = FALSE;
	bool orientationRemoval = FALSE;

	GtsVertexList vertices;
	
	// parse command line
	
	if (argc < 5) { 
		fprintf(stderr, "Usage: %s {-a min max | -e min max | -i min | -o angle x y z} <input.gts> <output.gts>\n",argv[0]);
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "\t -e remove triangles with edges with length within min->max\n"); 
		fprintf(stderr, "\t -a remove triangles with area within min->max\n"); 
		fprintf(stderr, "\t -i remove triangles with upto (and inc.) min neighbours\n");
		fprintf(stderr, "\t -o remove triangles angle(normal, vector x y z) >= angle\n");
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(0);
	} else {
		for (i=1; i < (argc - 2); i++){
		if (strcmp("-a", argv[i]) == 0){
			minLimit = atof(argv[++i]);
			maxLimit = atof(argv[++i]);
			areaRemoval = TRUE;
		} else if (strcmp("-e", argv[i]) == 0){
			minLimit = atof(argv[++i]);
			maxLimit = atof(argv[++i]);
			edgeRemoval = TRUE;
		} else if (strcmp("-i", argv[i]) == 0){
			neighbourLimit = atof(argv[++i]);
			isolatedRemoval = TRUE;
		} else if (strcmp("-o", argv[i]) == 0){
			angle = atof(argv[++i]);
			angle = ((angle / 360) * 2 * PI);
			orientation[0] = atof(argv[++i]);
			orientation[1] = atof(argv[++i]);
			orientation[2] = atof(argv[++i]);
			orientationRemoval = TRUE;			
		} else {
			cerr << "Invalid option " << argv[1] << "\n";
			cerr << "Must specify -a, -e -i -o" << "\n";
			abort();
		}
	}	
	}
	
	// define new surface and read from input file
	
	theSurface = gtsInput(argv[argc -2]);
	
	// for edge face - remove if not within limits
	
	initial_faces = gts_surface_face_number(theSurface);
	initial_vertices = gts_surface_vertex_number(theSurface);
	
	if (areaRemoval == TRUE){
	
		gts_surface_foreach_face_remove(theSurface, 
										(GtsFunc) area_tri_cutter, NULL);
	}
	if (edgeRemoval == TRUE){
	
		gts_surface_foreach_face_remove(theSurface, 
										(GtsFunc) edge_tri_cutter, NULL);
	}
	if (isolatedRemoval == TRUE){
		gts_surface_foreach_face_remove(theSurface, 
									(GtsFunc) isolated_tri_cutter, theSurface);
	}
	if (orientationRemoval == TRUE){
		gts_surface_foreach_face_remove(theSurface, 
										(GtsFunc) orientation_tri_cutter, NULL);
	}
	
	// report removal and output
	
	cout << "--> " << (initial_faces - gts_surface_face_number(theSurface));
	cout << " faces removed\n";
	cout << "--> " << (initial_vertices - gts_surface_vertex_number(theSurface));
	cout << " vertices removed\n";
	
	gts_write_surface_to_file(theSurface, argv[argc - 1], NULL);

}
//------------------------------------------------------------------------------
