//
// Filename: gtscutter.cc
//
// Description: GTS surface cutting tool
//
// Copyright (C) 2006  Toby Breckon
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

#include <math.h>
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
#include "datatypes.h"
#include "gts_input.h"
#include "output.h"

#ifdef USE_COLOUR_OOGL

#include "coff.h"
#include "oogl_output.h"

#endif

//------------------------------------------------------------------------------

// static globals for command line parameters

static double minX, minY, minZ;
static double maxX, maxY, maxZ;
static double radius;
static GtsPoint centrePoint;

//------------------------------------------------------------------------------

// face cutting operations - return TRUE if a face is to be removed	
	
static int normal_removal_operation(GtsFace * f, gpointer p){
	
	GtsVertex * tv1, * tv2, * tv3;
	gts_triangle_vertices(GTS_TRIANGLE(f), &tv1, &tv2, &tv3);

	// if a vertex is outside any of the limits then return
	// 1 to cut face, else return 0 to keep it
	
	if ((GTS_POINT(tv1)->x <= minX) ||
			(GTS_POINT(tv1)->x >= maxX) ||
			(GTS_POINT(tv1)->y <= minY) ||
			(GTS_POINT(tv1)->y >= maxY) ||
			(GTS_POINT(tv1)->z <= minZ) ||
			(GTS_POINT(tv1)->z >= maxZ))
	{
		return 1;
  } else if ((GTS_POINT(tv2)->x <= minX) ||
						 (GTS_POINT(tv2)->x >= maxX) ||
						 (GTS_POINT(tv2)->y <= minY) ||
						 (GTS_POINT(tv2)->y >= maxY) ||
						 (GTS_POINT(tv2)->z <= minZ) ||
						 (GTS_POINT(tv2)->z >= maxZ))
	{
		return 1;
	} else if ((GTS_POINT(tv3)->x <= minX) ||
						 (GTS_POINT(tv3)->x >= maxX) ||
						 (GTS_POINT(tv3)->y <= minY) ||
						 (GTS_POINT(tv3)->y >= maxY) ||
						 (GTS_POINT(tv3)->z <= minZ) ||
						 (GTS_POINT(tv3)->z >= maxZ))
	{
		return 1;
	} else {
		return 0;
	}
}

static int reversed_removal_operation(GtsFace * f, gpointer p){

	return (!(normal_removal_operation(f, NULL)));
}

static int radius_removal_operation(GtsFace * f, gpointer p){
	
	GtsVertex * tv1, * tv2, * tv3;
	gts_triangle_vertices(GTS_TRIANGLE(f), &tv1, &tv2, &tv3);

	// if a vertex is inside the radius from the point then return
	// 1 to cut face, else return 0 to keep it
	
	if (gts_point_distance(GTS_POINT(tv1), &centrePoint) <= radius)
	{
		return 1;
  } else if (gts_point_distance(GTS_POINT(tv2), &centrePoint) <= radius)
	{
		return 1;
	} else if (gts_point_distance(GTS_POINT(tv3), &centrePoint) <= radius)
	{
		return 1;
	} else {
		return 0;
	}
}

static int reversed_radius_removal_operation(GtsFace * f, gpointer p){

	return (!(radius_removal_operation(f, NULL)));
}

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	bool reversed = FALSE;
	bool radiusBasedCut = FALSE;
	GtsSurface * theSurface;
	
	int i;
	
	// initialise default behaviour in X, Y and Z (i.e. cut no points)
	
	minX = -1 * HUGE;
	minY = -1 * HUGE;
	minZ = -1 * HUGE;
	maxX = HUGE;
	maxY = HUGE;
	maxZ = HUGE;
	
	// get, check and process command line options
	
	if (argc < 6) { 
		fprintf(stderr, 
			"Usage: %s [-R]  {-r radius x y z |-x min max | -y min max | -z min max} [input] [output]\n",
			argv[0]);
		fprintf(stderr, "Options\n");
		fprintf(stderr, "\t-R : for reverse operation (i.e. inside limits).\n");
		fprintf(stderr, "\t-r : remove points within specified radius of point x y z\n");
		fprintf(stderr, "\t-x -y -z : remove points within specified x y z limits\n");
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(1);
	} 
	for (i = 1; i <= (argc - 3); i++){
		if (strcmp("-x", argv[i]) == 0){
			minX = atof(argv[++i]);
			maxX = atof(argv[++i]);
			cout << "PARAMETER: X limits : min = " 
				 << minX << " max = " << maxX << "\n";
		} else if (strcmp("-y", argv[i]) == 0){
			minY = atof(argv[++i]);
			maxY = atof(argv[++i]);	
			cout << "PARAMETER: Y limits : min = " 
				 << minY << " max = " << maxY << "\n";
		} else if (strcmp("-z", argv[i]) == 0){
 	    minZ = atof(argv[++i]);
			maxZ = atof(argv[++i]);
			cout << "PARAMETER: Z limits : min = " 
				 << minZ << " max = " << maxZ << "\n";
		} else if (strcmp("-R", argv[i]) == 0){
			reversed = TRUE;
			cout << "PARAMETER: reverse operation" 
				 << " - removing points inside limits specified\n";
		} else if (strcmp("-r", argv[i]) == 0){
			radiusBasedCut = TRUE;
			radius = atof(argv[++i]);
			centrePoint.x = atof(argv[++i]);
			centrePoint.y = atof(argv[++i]);
			centrePoint.z = atof(argv[++i]);
			cout << "PARAMETER: radius based operation "
					 << "radius = " << radius << " point = "
					 << centrePoint.x << " " << centrePoint.y << " "
					 << centrePoint.z << "\n";
			
		} else {
			fprintf(stderr, "ERROS: %s: - unknown option %s\n", 
					argv[0], argv[i]);
			
			PRINT_COPYRIGHT
			PRINT_BUGS
			PRINT_BUILD
			exit(1);
		} 
	}
	
	
	// input the points from file
	#ifdef USE_COLOUR_OOGL
			theSurface = coffInput(argv[argc - 2]);
	#else
			theSurface = gtsInput(argv[argc - 2]);
	#endif
	
	// process surface
	
	if (!(radiusBasedCut)){
	
		if (!(reversed)){
			gts_surface_foreach_face_remove(theSurface, 
									(GtsFunc) normal_removal_operation, NULL);
		} else {
			gts_surface_foreach_face_remove(theSurface, 
									(GtsFunc) reversed_removal_operation, NULL);
		}
	} else {
		
		if (!(reversed)){
			gts_surface_foreach_face_remove(theSurface, 
							(GtsFunc) radius_removal_operation, NULL);
		} else {
			gts_surface_foreach_face_remove(theSurface, 
							(GtsFunc) reversed_radius_removal_operation, NULL);
		}
	}
	
	// output to file
	
	#ifdef USE_COLOUR_OOGL
	oogl_write_surface_to_file(theSurface, argv[argc - 1]);
	#else
	gts_write_surface_to_file(theSurface, argv[argc - 1], NULL);
	#endif
	
}
//-----------------------------------------------------------------------
