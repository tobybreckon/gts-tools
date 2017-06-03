//
// Filename: gtsnormalise.cc
//
// Description: normalise GTS surface centroid to origin (or specified point)
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

// library includes

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

int main(int argc, char * argv[]){

	GtsSurface* theSurface; // pointer to surface
	
	GtsVertexList vertices;	// list of vertices
	GtsVertexList::iterator current;
	
	GtsPoint * centroid = NULL;
	
//--------- Command Line Options
		
	if (argc < 3) { 
			fprintf(stderr, "Usage: %s [-p x y z] <input.gts> <output.gts>\n",
					  argv[0]);
			fprintf(stderr, "Options:\n");
			fprintf(stderr, "\t -p x y z : normalise to position x y z\n");
			PRINT_COPYRIGHT
			PRINT_BUGS
			PRINT_BUILD
		exit(1);
	} else if (argc == 7) {
		if (strcmp("-p", argv[1]) == 0){
			argv++;
			centroid = gts_point_new(gts_point_class(), 
								atof(argv[1]), atof(argv[2]), atof(argv[3]));
			argv+=3;
		} else {
			fprintf(stderr, "Usage: %s [-p x y z] <input.gts> <output.gts>\n",
					  argv[0]);
			fprintf(stderr, "\t -p x y z : normalise to position x y z\n");
			PRINT_COPYRIGHT
			PRINT_BUGS
			PRINT_BUILD
			exit(1);
		}
	} else if (argc != 3) {
			fprintf(stderr, "Usage: %s [-p x y z] <input.gts> <output.gts>\n",
					  argv[0]);
			fprintf(stderr, "\t -p x y z : normalise to position x y z\n");
			PRINT_COPYRIGHT
			PRINT_BUGS
			PRINT_BUILD
		exit(1);
	}
	
//--------- Input

	#ifdef USE_COLOUR_OOGL
			theSurface = coffInput(argv[1]);
	#else
			theSurface = gtsInput(argv[1]);
	#endif
	
//--------- Process

	if (centroid == NULL){
		centroid = gts_surface_centroid(theSurface);
	}
	
	vertex_list_of(theSurface, &vertices);
	
	for(current = vertices.begin(); current != vertices.end(); current++){
		GTS_POINT(*current)->x -= centroid->x;
		GTS_POINT(*current)->y -= centroid->y;
		GTS_POINT(*current)->z -= centroid->z;
	}
	
//--------- Output

	#ifdef USE_COLOUR_OOGL
		oogl_write_surface_to_file(theSurface, argv[2]);
	#else
		gts_write_surface_to_file(theSurface, argv[2], NULL);
	#endif

};

//------------------------------------------------------------------------------
