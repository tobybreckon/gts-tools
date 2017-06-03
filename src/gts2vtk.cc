//
// Filename: gts2vtk.cc
//
// Description: *very* simple GTS to VTK conversion
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
#include "gts_input.h"

//------------------------------------------------------------------------------

int main(int argc, char * argv[]){

 FILE *fp; // output file pointer 
	
//--------- Command Line Options
	
  if (argc < 3) { 
		fprintf(stderr, "Usage: %s [input.gts] [output.vtk]\n",
					  argv[0]);
		PRINT_COPYRIGHT
		PRINT_BUGS
		PRINT_BUILD
		exit(1);
	} 
	
//--------- Input
	
	GtsSurface * theSurface = gtsInput(argv[1]);
	
//--------- Process

	// N/A
	
//--------- Output
	
	fp = fopen(argv[2], "wb");
	if (fp == NULL){
		cerr << "ERROR: Failed to open file: " << argv[2] 
			 << " for writing VTK surface.\n"; 
		exit(1);
	}

	gts_surface_write_vtk(theSurface, fp);
	fclose(fp);

};

//------------------------------------------------------------------------------
