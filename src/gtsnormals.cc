//
// Filename: gtsnorm.cc
//
// Description: output set of surface normals as ASCII XYZ NX NY NZ for
//              for vector field visualisation
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

int main(int argc, char * argv[]){

	GtsSurface* theSurface; 		// pointer to surface
	
	GtsVertexList vertices;			// list of vertices
	GtsVertexList::iterator current;
		
	GtsVertexVectorList normals;	// list of surface normals
	GtsVertexVector normalItem;		
	
  bool useGtsMeanCurvature = FALSE;
	
	// open file - report any errors
	
	if (argc < 4) { 
		fprintf(stderr, "Usage: %s {-gts | -e | -d | -a} <input.gts> <output_norm.xyz>\n",
					  argv[0]);
		fprintf(stderr, "Options\n");
		fprintf(stderr, "\t -gts : use gts mean curvature normal\n\n");
		fprintf(stderr, "Calculation of vertex normals from neighbouring faces: \n");
		fprintf(stderr, "\t -e : use equal weighting of face normals (Gourand)\n");
		fprintf(stderr, "\t -d : distance proportional weighting\n");
		fprintf(stderr, "\t -a : area proportional weighting\n");
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(1);
	} else {
		if (strcmp("-gts", argv[1]) == 0){
			useGtsMeanCurvature = TRUE;
		} else if (strcmp("-e", argv[1]) == 0){
			GTS_VERTEX_NORMAL_WEIGHT = GTS_VERTEX_NORMAL_GOURAND;
		} else if (strcmp("-d", argv[1]) == 0){
			GTS_VERTEX_NORMAL_WEIGHT = GTS_VERTEX_NORMAL_MAX;
		} else if (strcmp("-a", argv[1]) == 0){
			GTS_VERTEX_NORMAL_WEIGHT = GTS_VERTEX_NORMAL_THURMER_WUTHRICH;
		} else {
			cerr << "Invalid option " << argv[1] << "\n";
			cerr << "Must specify -gts, -e, -d, -a" << "\n";
			exit(1);
		}
	}
	
	theSurface = gtsInput(argv[2]);
	
	// get list of vertices
	
	vertex_list_of(theSurface, &vertices);
	
	// compute and store surface normal at each point
	
	for(current=vertices.begin(); current != vertices.end(); current++){
		
		if (useGtsMeanCurvature){
		
			// use GTS mean curvature normal
			
			if (gts_vertex_mean_curvature_normal((*current), 
				theSurface, normalItem.vec))
			{
					normalItem.v = (*current);
					normals.push_back(normalItem);
			}
			
		} else {
			
			// use specified normal calculation method
		
			if (gts_vertex_normal(theSurface, (*current), 
						&normalItem.vec[0], &normalItem.vec[1], 
						&normalItem.vec[2]))
			{
					normalItem.v = (*current);
					normals.push_back(normalItem);
			}
			
		}
	}
	
	// output vector field to file
	
	vertexVectorListToFile(normals, argv[3]);
	
}
//------------------------------------------------------------------------------
