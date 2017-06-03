//
// Filename: gts_input.cc
//
// Description: wrapper around gts_surface_read() 
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

#include <list>
#include <vector>
#include <iostream>

#include <stdlib.h>

using namespace std;

// Library includes

// local includes

#include "general.h"
#include "gts_common.h"
#include "compressed.h"

//-----------------------------------------------------------------------------

// input a GTS format file from filename.
// Return a pointer to resulting surface

GtsSurface* gtsInput(char * filename){
	
	GtsSurface* theSurface;     // pointer to surface
	FILE *fp;
	GtsFile *gts_fp;
	
	compressedFile *decompress = new compressedFile(filename);
	
	theSurface = gts_surface_new(gts_surface_class(), gts_face_class(), 
								 gts_edge_class(), gts_vertex_class());
	
	// fp = fopen(filename, "rb");
	
	fp = decompress->open();
	if (fp == NULL){
		cerr << "ERROR: Failed to open file: " 
			 << filename << " for reading GTS surface.\n"; 
		abort();
	}

	gts_fp = gts_file_new(fp);
	gts_surface_read(theSurface, gts_fp);
	
	//fclose(fp);
	
	decompress->close();
	gts_file_destroy(gts_fp);
	
	delete decompress;
	
	return theSurface;
	
}

//-------------------------------------------------------------------------------------------
