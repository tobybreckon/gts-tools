//
// Filename: output.cc
//
// Description: data output routines
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
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

// Library includes

#include "gts_common.h"

// local includes

#include "output.h"
#include "general.h"

//------------------------------------------------------------------------------

// setup default output settings on specified stream

static void setOutputFlags(ofstream& out){
	
	out << setprecision(OUTPUT_PRECISION);
	out << setiosflags(ios::fixed);
	return;
	
}

//------------------------------------------------------------------------------

// output GTS vertex to open file as ASCII

void GtsVertexToFile(GtsVertex v, ofstream& out){
	
	out << v.p.x << " " << v.p.y << " " << v.p.z << " ";
	
}

//------------------------------------------------------------------------------

// output GtsVertexList vertices to file specified by filename as 
// ASCII XYZ points

void vertexListToFile(GtsVertexList vertices, char * filename){
	
	ofstream out; // the output file
	
	GtsVertexList::iterator current_vertex;
	
	int no_of_vertices = 0;
	
	// open file and check OK
	
	out.open(filename, ofstream::out);
	
	if (out.bad()){
		cerr << "ERROR: Failed to open file: " 
		     << filename 
		     << " for writing curvature data.\n"; 
		abort();
	}
	setOutputFlags(out);
	
	current_vertex = vertices.begin();
	
	// iterate through  vertices and output those to file
	
	while (current_vertex != vertices.end()){

			GtsVertexToFile(**current_vertex, out);
		
			out << "\n"; // newline for every vertex
			
			no_of_vertices++;

		current_vertex++;
		
	}	
	
	// all finished => close file

	out.close();
	
	
}

//------------------------------------------------------------------------------

// output GTS surface to file specified by 'filename' in GTS format with user
// comment if not NULL

void gts_write_surface_to_file(GtsSurface* theSurface, 
							  	char * filename, char * comment){
	
	FILE *fp;
	
	fp = fopen(filename, "wb");
	if (fp == NULL){
		cerr << "ERROR: Failed to open file: " 
			 << filename << " for writing GTS surface.\n"; 
		abort();
	}

	gts_surface_write(theSurface, fp);

	fclose(fp);
}

//------------------------------------------------------------------------------

void vertexVectorListToFile(GtsVertexVectorList list, char * filename){
	
	ofstream out; // the output file
	
	GtsVertexVectorList::iterator current_item;
	
	int no_of_items = 0;
	
	// open file and check OK
	
	out.open(filename, ofstream::out);
	
	if (out.bad()){
		cerr << "OUTPUT: Failed to open file: " << filename 
				 << " for writing vertex / vector data.\n"; 
		abort();
	}
	setOutputFlags(out);
	
	current_item = list.begin();
	
	// iterate through and output to file
	
	while (current_item != list.end()){

			GtsVertexToFile(*((*current_item).v), out);
		
			// if an item is an integer (like a colour may be!)
			// then output it as such.
		
			if (floor((*current_item).vec[0]) == (*current_item).vec[0]){
				out << " " << (int) (*current_item).vec[0];
			} else {
				out << " " << (*current_item).vec[0];
			}
			
			if (floor((*current_item).vec[1]) == (*current_item).vec[1]){
				out << " " << (int) (*current_item).vec[1];
			} else {
				out << " " << (*current_item).vec[1];
			}
			
			if (floor((*current_item).vec[2]) == (*current_item).vec[2]){
				out << " " << (int) (*current_item).vec[2];
			} else {
				out << " " << (*current_item).vec[2];
			}
			out << "\n"; // newline for every item
			
			no_of_items++;

		current_item++;
		
	}	
	
	// all finished => close file
	
	cerr << "OUTPUT: format of file: " << filename << " X Y Z V1 V2 V3\n";
	cerr << "OUTPUT: " << no_of_items << " items output\n";
	out.close();
	
	
}

//------------------------------------------------------------------------------
