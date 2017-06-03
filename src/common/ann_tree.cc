//
// Filename: ann_tree.cc
//
// Description:  GTS ANN 3d-tree class
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
#include <math.h>

using namespace std;

// library includes

#include "gts_common.h"

// local includes

#include "general.h"
#include "datatypes.h"
#include "ann_tree.h"

//------------------------------------------------------------------------------

// N.B. a class can have multiple overloaded constructors so just define
// a new GtsPointAnnTree( ) for whichever input type/structure is 
// required.

GtsVertexAnnTree::GtsVertexAnnTree(GtsVertexList &list, 
								   GtsVertexAnnTreeFunc func){
	
	GtsVertexList::iterator currentVertex;
	GtsPoint * p;
	
	int pointCounter = 0;
	
	// allocate ANN data point array with max point = list size 
	// and dimension = 3
	
	thePointArray = annAllocPts(((int) list.size()), 3);		
	
	// insert each point into the ANN point array
	
	for (currentVertex = list.begin(); currentVertex != list.end(); 
			 currentVertex++){
		
		p = func(*currentVertex);
		thePointArray[pointCounter][0] = p->x;
		thePointArray[pointCounter][1] = p->y;
		thePointArray[pointCounter][2] = p->z;
		vertices.push_back(*currentVertex);
		pointCounter++;
	}
	
	// create the ANN tree using the point array
	
	theTree = new ANNkd_tree(thePointArray,((int) list.size()), 3);
	
	// pre-allocate memory for singular queries
	
	theQueryPoint =  annAllocPt(3);
	returnIndexArray = new ANNidx[1];
	returnDistArray = new ANNdist[1];
	
}

//------------------------------------------------------------------------------

GtsVertexAnnTree::~GtsVertexAnnTree(){
	
	// clear all memory
	
	vertices.clear();
	delete theTree;
	annDeallocPts(thePointArray);
	
	annDeallocPt(theQueryPoint);
	delete returnIndexArray;
	delete returnDistArray;

}
//------------------------------------------------------------------------------

// return the nearest GtsPoint to query GtsPoint p

GtsVertex * GtsVertexAnnTree::nearest(GtsPoint * p){
	
	theQueryPoint[0] = p->x;
	theQueryPoint[1] = p->y;
	theQueryPoint[2] = p->z;
	
	theTree->annkSearch(theQueryPoint, 1, returnIndexArray, 
											returnDistArray);
	
	return vertices[returnIndexArray[0]];
}
//------------------------------------------------------------------------------

// populate list with the k closest vertices to GtsPoint p
// within the list of vetices specified as the constructor list 
	
void GtsVertexAnnTree::nearestK(GtsPoint * p, int k, GtsVertexList &list){
	
	ANNidxArray	returnIndexArrayK = new ANNidx[k];
	ANNdistArray returnDistArrayK = new ANNdist[k];
	int i;
	
	theQueryPoint[0] = p->x;
	theQueryPoint[1] = p->y;
	theQueryPoint[2] = p->z;
	
	theTree->annkSearch(theQueryPoint, k, returnIndexArrayK, 
											returnDistArrayK);
	
	for(i=0; i < k; i++){
		list.push_back(vertices[returnIndexArrayK[i]]);
	}
	
	delete returnIndexArrayK;
	delete returnDistArrayK;
	
  return;
	
};

//------------------------------------------------------------------------------

// re-populate an existing tree of size N with a new set of 
// vertices size N
	 
void GtsVertexAnnTree::repopulate(GtsVertexList &list, 
								  GtsVertexAnnTreeFunc func){

	GtsVertexList::iterator currentVertex;
	GtsPoint * p;
	
	int pointCounter = 0;
	
	vertices.clear();
	for (currentVertex = list.begin(); currentVertex != list.end(); 
			 currentVertex++){
		
		p = func(*currentVertex);
		thePointArray[pointCounter][0] = p->x;
		thePointArray[pointCounter][1] = p->y;
		thePointArray[pointCounter][2] = p->z;
		vertices.push_back(*currentVertex);
		pointCounter++;
	}
	
	// re-create the ANN tree using the point array
	
	delete theTree;
	theTree = new ANNkd_tree(thePointArray,((int) list.size()), 3);
	
}

//------------------------------------------------------------------------------

// replace the point position at vertex v on the asscoiated surface
// with p and rebuild tree
		
void GtsVertexAnnTree::replace(GtsVertex * v, GtsPoint p){

	// find v in the current tree
	
	theQueryPoint[0] = GTS_POINT(v)->x;
	theQueryPoint[1] = GTS_POINT(v)->y;
	theQueryPoint[2] = GTS_POINT(v)->z;
	
	theTree->annkSearch(theQueryPoint, 1, returnIndexArray, 
											returnDistArray);
	
	// replace position value of v with p - both on the surface
	// and in the tree point array
	
	(vertices[returnIndexArray[0]])->p = p;
	thePointArray[returnIndexArray[0]][0] = p.x;
	thePointArray[returnIndexArray[0]][1] = p.y;
	thePointArray[returnIndexArray[0]][2] = p.z;
	
	// rebuild the tree
	
	delete theTree;
	theTree = new ANNkd_tree(thePointArray,((int) vertices.size()), 3);
	
}

//------------------------------------------------------------------------------

void GtsVertexAnnTree::stats(){
	
	ANNkdStats stats;
	
	theTree->getStats(stats);
	
	cout << "GtsVertexAnnTree : points " << stats.n_pts << " leaves " 
			 << stats.n_lf << "\n";
	cout << "GtsVertexAnnTree : depth " << stats.depth << "\n";
	
}

//------------------------------------------------------------------------------
