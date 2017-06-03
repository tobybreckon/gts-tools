//
// Filename: gts2xyz.cc
//
// Description: simple tool to strip vertices from GTS surface
//              as ASCII XYZ points
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

using namespace std;

// library includes

#include "gts_common.h"

// local includes

#include "general.h"
#include "datatypes.h"
#include "gts_input.h"
#include "output.h"

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

	GtsVertexList vertices;
	GtsSurface * s;
	
//--------- Command Line Options
	
	if (argc < 3) { 
			fprintf(stderr, "Usage: %s <input.gts> <output.xyz>\n",
							argv[0]);
			PRINT_COPYRIGHT
			PRINT_BUGS
    		PRINT_BUILD
			exit(0);
	} 
	
//--------- Input
	
	s = gtsInput(argv[1]);
	
//--------- Process

	vertex_list_of(s, &vertices);
	
//--------- Output
	
	vertexListToFile(vertices, argv[2]);
	
};

//------------------------------------------------------------------------------
