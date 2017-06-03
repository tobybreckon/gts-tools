//
// Filename: gtsinfo.cc
//
// Description: GTS surface information tool
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
#include <ctime>
#include <list>
#include <vector>
#include <cstdlib>

using namespace std;

// Library includes

// local includes

#include "general.h"
#include "gts_common.h"
#include "gts_input.h"

#ifdef USE_COLOUR_OOGL

#include "coff.h"
#include "oogl_output.h"

#endif

static const char * BOOLEAN[2] = {"FALSE", "TRUE"};

//------------------------------------------------------------------------------


static void clockwise_triangle_counter(GtsFace * f, int * counter);
static void normal_direction_counter(GtsFace * f, double * data);

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	GtsSurface* theSurface; // pointer to surface
	
	GtsPoint * centroid;	// info items to be gathered
	int clockwise_triangles = 0;
	
	double data[4];			// data to be passed to/from "foreach" function
	
	time_t timer;			// timestamp output
    timer=time(NULL); 
	
	// open file - report any errors
	
	if (argc < 2) { 
		fprintf(stderr, "Usage: %s [filename]\n",argv[0]);
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(0);
	}
		
	#ifdef USE_COLOUR_OOGL
			theSurface = coffInput(argv[1]);
	#else
			theSurface = gtsInput(argv[1]);
	#endif
	
	// output all available info on surface to stdout
	
	cout << "# File : " << argv[1] << "\n";
	
	cout << "# " << asctime(localtime(&timer));
	cout << "#\n";
	
	gts_surface_print_stats(theSurface, stdout);
	
	cout << "#   median edge length : " 
			 << gts_surface_median_edge_length(theSurface) << "\n";
	cout << "# GTS properties\n";
	cout << "#   surface manifold ? (all edges have > 2 faces) : " 
			<< BOOLEAN[gts_surface_is_manifold(theSurface)] << "\n";
	cout << "#   surface orientable ? (no incompatible faces) : " 
			<< BOOLEAN[gts_surface_is_orientable(theSurface)] << "\n";
	cout << "#   surface closed ? (all edges have exactly 2 faces) : "  
			<< BOOLEAN[gts_surface_is_closed(theSurface)] << "\n";
	cout << "#   area : " << gts_surface_area(theSurface);
	cout << " volume : " << gts_surface_volume(theSurface)<< "\n";
	
	cout << "# Geometric properties\n";
	
	centroid = gts_surface_centroid(theSurface);
	
	cout << "#   surface centroid : " << centroid->x << " " 
			 << centroid->y << " " << centroid->z << "\n";
	
	// get the numbers of each type of triangle vertex ordering
	
	gts_surface_foreach_face(theSurface, (GtsFunc) clockwise_triangle_counter, 
														 &clockwise_triangles);
	
	cout << "#   clockwise triangles : " << clockwise_triangles << "\n";
	cout << "#   anti-clockwise (or co-linear) triangles: " 
		 << (gts_surface_face_number(theSurface) - clockwise_triangles) 
		 << "\n";
	
	// get direction of triangle normals relative to centroid
	
	data[0] = centroid->x;  // array entries 0 - 2 are centroid x,y,z
	data[1] = centroid->y;	
	data[2] = centroid->z;
	data[3] = 0; 			// number of normals pointing towards centroid
	
	gts_surface_foreach_face(theSurface, (GtsFunc) normal_direction_counter, 
							 data); 
	
	cout << "#   outward triangle normals : " << data[3] << "\n";
	cout << "#   inward triangle normals : " 
			 << (gts_surface_face_number(theSurface) - data[3]) << "\n";
	
}
//------------------------------------------------------------------------------

// worker function to return a count of the number of clockwise triangles

static void clockwise_triangle_counter(GtsFace * f, int * counter){
	if (gts_triangle_orientation(GTS_TRIANGLE(f)) < 0){
		(*counter)++;
	}
}

// worker function to return a count of the number of triangle normals 
// pointing away from the centroid

static void normal_direction_counter(GtsFace * f, double * data){

  GtsVector N;
	GtsVector centroidToTriangleCentre;
	GtsPoint * centre;
	
	centre = gts_triangle_center(GTS_TRIANGLE(f), gts_point_class());
	
	// data 0 -> 3 contains centroid
	
	centroidToTriangleCentre[0] = centre->x - data[0];
	centroidToTriangleCentre[1] = centre->y - data[1];
	centroidToTriangleCentre[2] = centre->z - data[2];
	
	gts_triangle_normal(GTS_TRIANGLE(f), &N[0], &N[1], &N[2]);
	
	// if angle between vector centroid->centre and triangle normal
	// < 90 degrees they point in the same direction (roughly)
	
	if (gts_vector_angle(N, centroidToTriangleCentre) < (PI / 2)){
		data[3]++;
	}
	
	gts_object_destroy(GTS_OBJECT(centre));
}

//------------------------------------------------------------------------------
