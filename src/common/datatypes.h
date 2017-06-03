//
// Filename: datatypes.h
//
// Description: datatypes utilised
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

#ifndef DATATYPES_H
#define DATATYPES_H

//-------------------- File Name Type -----------------------------------

typedef char filename_t [FILENAME_MAX];

//-------------------- GtsVertexList Type -------------------------------

typedef list<GtsPoint> GtsPointList; 		// list of point structures

//-------------------- GtsVertexList Type -------------------------------

typedef list<GtsVertex *> GtsVertexList; 	// list of pointer to vertices

//-------------------- GtsEdgeList Type -------------------------------

typedef list<GtsEdge *> GtsEdgeList; 		// list of pointer to edges

//-------------------- GtsTriangleList Type -------------------------------

typedef list<GtsTriangle *> GtsTriangleList; // list of pointer to Triangles

//-------------------- GtsVertexVector Type -------------------------------

typedef vector<GtsVertex *> GtsVertexVec; 	 // vector of pointer to vertices

//----------------------------------------------------------------------------

// Store an RGB colour for a given vertex

typedef struct{
	
	GtsVertex * v;
	int r;
	int g;
	int b;
	
} GtsVertexColour;

typedef list<GtsVertexColour> GtsVertexColourList;

//----------------------------------------------------------------------------

// store a specified vector vec for each point

typedef struct{
	
	GtsPoint p;
	GtsVector vec;
	
} GtsPointVector;

typedef list<GtsPointVector> GtsPointVectorList;

//----------------------------------------------------------------------------

// store a specified value for a given vertex

typedef struct{
	
	GtsVertex * v;
	double value;
	
} GtsVertexValue;

//----------------------------------------------------------------------------

// store a specified vector vec for each vertex

typedef struct{
	
	GtsVertex * v;
	GtsVector vec;
	
} GtsVertexVector;

typedef list<GtsVertexVector> GtsVertexVectorList;

//----------------------------------------------------------------------------

#endif
