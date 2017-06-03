//
// Filename: gtssubtract.cc
//
// Description: subtract one surface from another based on vertex position
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

// Tolerance for matching points to handle rounding errors in input / output

#define POINT_MATCHING_TOLERANCE 0.005

//------------------------------------------------------------------------------

// system includes

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>

using namespace std;

// Library includes

#include "gts_common.h"

// local includes

#include "general.h"
#include "gts_input.h"
#include "output.h"

#include "ann_tree.h"

#ifdef USE_COLOUR_OOGL

#include "coff.h"
#include "oogl_output.h"

#endif
//------------------------------------------------------------------------------

// face removal worker function

static bool face_remover(GtsFace * f, GtsVertexAnnTree * theTree){
	
	GtsVertex * tv1, * tv2, * tv3;

	gts_triangle_vertices(GTS_TRIANGLE(f), &tv1, &tv2, &tv3);

	if ((	
			gts_point_distance(GTS_POINT(theTree->nearest(GTS_POINT(tv1))), 
																GTS_POINT(tv1))
			< POINT_MATCHING_TOLERANCE
		 ) 
		 && 
		 (	
			gts_point_distance(GTS_POINT(theTree->nearest(GTS_POINT(tv2))), 
																GTS_POINT(tv2))
			< POINT_MATCHING_TOLERANCE
		 ) 
		 &&
		 (	
			gts_point_distance(GTS_POINT(theTree->nearest(GTS_POINT(tv3))), 
																GTS_POINT(tv3))
			< POINT_MATCHING_TOLERANCE
		 ))
		{
			// face vertices present in the tree -> we return TRUE to remove it 
			return TRUE;
		} else {
			// otherwise not present so return FALSE to keep the face
			return FALSE;
		}
}

static bool reverse_op(GtsFace * f, GtsVertexAnnTree * theTree){
	
	return (!(face_remover(f,theTree)));
	
}

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	GtsVertexList surface2vertices;
	
	GtsVertexAnnTree * theTree;
	
	GtsFunc theRemovalFunction = (GtsFunc) face_remover;
	
//--------- Command Line Options
	
	if (argc < 4) { 
		fprintf(stderr, "Usage: %s [-r] <s1.gts> <s2.gts> <output.gts>\n",argv[0]);
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "\t-r : reverse operation (output intersection of s1 and s2 vertex sets)\n\n");
		fprintf(stderr, "Compute set difference (s1-s2) of vertex sets\n");
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(0);
	}
	if (strcmp("-r", argv[1]) == 0)
	{
		theRemovalFunction = (GtsFunc) reverse_op;
		argv++;
		argc--;
	}
	
//--------- Input
	#ifdef USE_COLOUR_OOGL
					GtsSurface * surface1 = coffInput(argv[1]);
					GtsSurface * surface2 = coffInput(argv[2]);
	#else
					GtsSurface * surface1 = gtsInput(argv[1]);
					GtsSurface * surface2 = gtsInput(argv[2]);
  	#endif

	
//--------- Process

	vertex_list_of(surface2, &surface2vertices);
	theTree = new GtsVertexAnnTree(surface2vertices, GtsVertexAnnTreeStandard);
	
	gts_surface_foreach_face_remove(surface1, theRemovalFunction, 
															(gpointer) theTree);
	
//--------- Output
	
	#ifdef USE_COLOUR_OOGL
		oogl_write_surface_to_file(surface1, argv[3]);
	#else
		gts_write_surface_to_file(surface1, argv[3], NULL);
	#endif

	// clear any memory
	
	delete theTree;
	gts_object_destroy(GTS_OBJECT(surface1));
	gts_object_destroy(GTS_OBJECT(surface2));
	
};

//-----------------------------------------------------------------------
