//
// Filename: gtsorientate.cc
//
// Description: consistently orientate the surface normals of a GTS surface
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

#ifdef USE_COLOUR_OOGL

#include "coff.h"
#include "oogl_output.h"

#endif

//------------------------------------------------------------------------------

// worker functions called from main routine

static void flip_orientation(gpointer face, gpointer data){

	gts_triangle_revert(GTS_TRIANGLE((GtsFace *) face));
	
}

static void check_orientation(gpointer face, gpointer data){

	bool orientateClockwise;
	double orientation;
	
	// get orientation from data
	
	orientateClockwise = *((bool *) data);

	// gts_triangle_orientation return values = gts_point_orientation
	// " a positive value if p1, p2 and p3 appear in counterclockwise order, 
	// a negative value if they appear in clockwise order and zero if they 
	// are colinear." - GTS doc. 0.7.1
	
	orientation = gts_triangle_orientation(GTS_TRIANGLE((GtsFace *) face));
	
	if ((orientation > 0) && orientateClockwise) {
		
		// if orientation anti-clockwise and clockwise desired
		// change tri orientation
		
		gts_triangle_revert(GTS_TRIANGLE((GtsFace *) face));
	
	} else if ((orientation < 0) && (!(orientateClockwise))){
		
		// if orientation clockwise and anti-clockwise desired
		// change tri orientation
		
		gts_triangle_revert(GTS_TRIANGLE((GtsFace *) face));
		
	}
	
	return;
	
	// ignore cases where triangle points are co-linear 
	// (i.e. gts_triangle_orientation = 0)
	
}

static void centroid_orientation(GtsFace * face, double * data){

	bool orientateTowardsCentriod = data[0];

	GtsVector N;
	GtsVector centroidToTriangleCentre;
	GtsPoint * centre;

	centre = gts_triangle_center(GTS_TRIANGLE(face), gts_point_class());
	
	// data 1 -> 3 contains centroid
	
	centroidToTriangleCentre[0] = centre->x - data[1];
	centroidToTriangleCentre[1] = centre->y - data[2];
	centroidToTriangleCentre[2] = centre->z - data[3];
	
	gts_triangle_normal(GTS_TRIANGLE(face), &N[0], &N[1], &N[2]);
	
	// if angle between vector centroid->centre and triangle normal
	// < 90 degrees they point in the same direction (roughly)
	
	if ((gts_vector_angle(N, centroidToTriangleCentre) < (PI / 2)) 
		&& (orientateTowardsCentriod)){
																		
		// 1st case: point in same direction away from centroid BUT towards
		// centroid desired => FLIP TRI
																		
		gts_triangle_revert(GTS_TRIANGLE(face));
																		
	} else if ((gts_vector_angle(N, centroidToTriangleCentre) > (PI / 2)) 
		&& (!(orientateTowardsCentriod))) {
		
		// 2nd case: point in different directions (> PI / 2)
		// => normal must point towards centroid
		// BUT away from centroid desired => FLIP TRI
																		
		gts_triangle_revert(GTS_TRIANGLE(face));																
	}
	
	gts_object_destroy(GTS_OBJECT(centre));
	
	return;

}

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	GtsSurface* theSurface; 		// pointer to surface
		
	bool overwriteInput = FALSE;	// overwrite input file
	bool orientateClockwise = TRUE;	// orientatation of triangles ?
	bool orientateTowardsCentriod = FALSE;
	bool centroidBasedOrientation = FALSE;
	bool flipNormals = FALSE;
	
	double data[4]; // data to pass to centroid_orientation worker function
	GtsPoint * centroid;
	
	// process command line and load surface - report any errors
	
	if (argc < 3) { 
	fprintf(stderr, "Usage: %s {-c | -ac | -tc | -fc} [-O] {input} [output]\n",
				  argv[0]);
	fprintf(stderr, "Consistently orientates a GTS surface\n");
	fprintf(stderr, "Options\n");
	fprintf(stderr, "-f  = flip normal orientation from current orientation\n");	
	fprintf(stderr, "-c  = orientate triangle vertex order clockwise\n");
	fprintf(stderr, "-ac = orientate triangle vertex order anti-clockwise\n");
	fprintf(stderr, "-tc  = orientate triangles towards surface centroid (inwards) \n");
	fprintf(stderr, "-fc = orientate triangles away from surface centroid (outwards)\n");
	fprintf(stderr, "-O  = overwrite input file with new surface\n");
	PRINT_COPYRIGHT
	PRINT_BUGS
	PRINT_BUILD
	exit(1);
	} else {
		if (strcmp("-c", argv[1]) == 0){
			cerr << "PARAMETER: triangle vertex order = Clockwise\n";
			orientateClockwise = TRUE;
		} else if (strcmp("-ac", argv[1]) == 0){
			cerr << "PARAMETER: triangle vertex order = Anti-Clockwise\n";
			orientateClockwise = FALSE;
		} else if (strcmp("-tc", argv[1]) == 0){
			cerr << "PARAMETER: triangle orientation = Towards Centroid (inwards)\n";
			orientateTowardsCentriod = TRUE;
			centroidBasedOrientation = TRUE;
		} else if (strcmp("-fc", argv[1]) == 0){
			cerr << "PARAMETER: triangle orientation = From Centroid (outwards)\n";
			orientateTowardsCentriod = FALSE;
			centroidBasedOrientation = TRUE;		
		} else if (strcmp("-f", argv[1]) == 0){
			cerr << "PARAMETER: triangle orientation = invert orientation\n";
			flipNormals = TRUE;			
		} else {
			fprintf(stderr, "Usage: %s  {-f | -c | -ac | -tc | -fc} [-O] <input.gts> [<output.gts>]\n",
				  argv[0]);
			PRINT_COPYRIGHT
			PRINT_BUGS
			PRINT_BUILD
			exit(1);
		}
		if (strcmp("-O", argv[2]) == 0){
			cerr << "PARAMETER: overwriting input file " << argv[argc -1] 
					 << " with new surface\n";
			overwriteInput = TRUE;
			#ifdef USE_COLOUR_OOGL
				theSurface = coffInput(argv[argc - 1]);
			#else
				theSurface = gtsInput(argv[argc - 1]);
			#endif
		} else {
			#ifdef USE_COLOUR_OOGL
				theSurface = coffInput(argv[argc - 2]);
			#else
				theSurface = gtsInput(argv[argc - 2]);
			#endif
		}
		if ((!(overwriteInput)) && (argc < 4)){
			cerr << "ERROR: no output file specified!\n";
			exit(1);
		}
	}
	
	// for each triangle orient the surface
	
	if (flipNormals){
	
	// flip normals 
		
	   gts_surface_foreach_face(theSurface, (GtsFunc) flip_orientation, 
								 NULL);
		
	} else if (!(centroidBasedOrientation)){
	
	// else (if not centroid based) then orientate as specified	
		
		
		gts_surface_foreach_face(theSurface, (GtsFunc) check_orientation, 
								 &orientateClockwise);
		
	} else {
		
		// else do centroid based orientation
		
		data[0] = orientateTowardsCentriod;
		
		centroid = gts_surface_centroid(theSurface);
		data[1] = centroid->x;
		data[2] = centroid->y;
		data[3] = centroid->z;
		
		gts_surface_foreach_face(theSurface, 
								(GtsFunc) centroid_orientation,data);
		
	}
	
	// output suface (always last item of arguments)
	
	#ifdef USE_COLOUR_OOGL
	oogl_write_surface_to_file(theSurface, argv[argc - 1]);
	#else
	gts_write_surface_to_file(theSurface, argv[argc - 1], NULL);
	#endif
}

//------------------------------------------------------------------------------
