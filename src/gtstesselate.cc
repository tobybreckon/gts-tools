//
// Filename: gtstesselate.cc
//
// Description: tesselate GTS surface (replace each tri. with 4)
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

using namespace std;

// library includes

#include "gts_common.h" 

// local includes

#include "general.h"
#include "datatypes.h"
#include "gts_input.h"
#include "output.h"

//------------------------------------------------------------------------------

// return the centre vertex of a specified edge e using standard GTS midvertex
// function. Worker function for gts_surface_tessellate

static GtsVertex* centre_vertex(GtsEdge * e, 
								GtsVertexClass *klass, gpointer data){
	return gts_segment_midvertex(GTS_SEGMENT(e), klass);
}

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	GtsSurface * theSurface;
	int N; // number of times to tesselate surface
	
//--------- Command Line Options
		
	if (argc < 3) { 
		fprintf(stderr, "Usage: %s {N} [input.gts] [output.gts]\n",
					  argv[0]);
		fprintf(stderr, "Options:\n");
			fprintf(stderr, "\t N : tesselate N times\n");
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(1);
	} else if (argc == 4) {
		N = atoi(argv[1]); // set N to number of tesselations
		argc--;
		argv++;
	} else {
	    N = 1;	// by default tesselate once
	}
	
//--------- Input
	
	theSurface =  gtsInput(argv[argc - 2]);
	
//--------- Process
	
	for (int i = 0; i < N; i++) {
		gts_surface_tessellate(theSurface, centre_vertex, NULL);
	}
	
//--------- Output
	
	gts_write_surface_to_file(theSurface, argv[argc -1], NULL);
	
};

//------------------------------------------------------------------------------
