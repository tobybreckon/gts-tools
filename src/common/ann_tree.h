//
// Filename: ann_tree.h
//
// Description:  GTS ANN 3d-tree class header
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

#ifndef GTS_ANN_TREE_HEADERS

#define GTS_ANN_TREE_HEADERS

#include <ANN/ANN.h>
#include <ANN/ANNperf.h>

// exemplar class of usage:

// GtsVertexAnnTree * theTree;
// theTree = new GtsVertexAnnTree(MyGtsVertexList, GtsVertexAnnTreeStandard);
// pointerToNearestGtsVertex = theTree->nearest(GTS_POINT(queryVertex));

//------------------------------------------------------------------------------

// typedef for a function type that given a vetex pointer (GtsVertex *)
// returns a pointer to a point (GtsPoint *)

typedef GtsPoint* (*GtsVertexAnnTreeFunc)  (GtsVertex * v);

// typically defined as follows for standard GtsVertex class 
// (given by example):

static GtsPoint * GtsVertexAnnTreeStandard(GtsVertex * v)
									{ return GTS_POINT(v);}

//------------------------------------------------------------------------------

class GtsVertexAnnTree{
	protected:
	
	// variable for tree
	
	ANNkd_tree		*theTree;
	ANNpointArray	thePointArray;
	vector<GtsVertex *> vertices;
	
	// variables for singular queries
	
	ANNpoint theQueryPoint;
	ANNidxArray	returnIndexArray;
	ANNdistArray returnDistArray;
	
	
	public:
		GtsVertexAnnTree(GtsVertexList &list, GtsVertexAnnTreeFunc func);
		~GtsVertexAnnTree();
	  
		// return a pointer to the vertex closest to GtsPoint p
		// within the list of vetices specified as the constructor list 
	
		GtsVertex * nearest(GtsPoint * p);
		
		// populate list with the k closest vertices to GtsPoint p
		// within the list of vetices specified as the constructor list 
	
	  void nearestK(GtsPoint * p, int k, GtsVertexList &list);
	 
	  // re-populate an existing tree of size N with a new set of 
		// vertices size N (does not check sizes are equal!)
	 
	  void repopulate(GtsVertexList &list, GtsVertexAnnTreeFunc func );
	 
		// replace the point position at vertex v on the asscoiated surface
		// with p and rebuild tree (N.B. associated GtsSurface IS altered).
		
		void replace(GtsVertex * v, GtsPoint p);
	 
		void stats();
};

//------------------------------------------------------------------------------

#endif
