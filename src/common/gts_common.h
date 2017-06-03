//
// Filename: gts_common.h
//
// Description: common GTS related functions
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

// GTS includes

#ifndef GTS_COMMON_HEADERS
#define GTS_COMMON_HEADERS

#include <gts.h>

#include "datatypes.h"

//----------------------------------------------------------------------------

// if passed the option FAST by the compiler turn off cast checking

#ifdef FAST

#undef GTS_CHECK_CASTS

#endif

//----------------------------------------------------------------------------

// constructing lists of Gts vertices

void vertex_list_of(GtsSurface* theSurface, GtsVertexList *vertices);

//----------------------------------------------------------------------------

// invert a specified GtsVector

#define gts_vector_invert(vector, inverse) { \
											inverse[0] = vector[0] * -1; \
											inverse[1] = vector[1] * -1; \
											inverse[2] = vector[2] * -1; \
                                  } \
//----------------------------------------------------------------------------

#define gts_vector_copy(original, copy) { \
											copy[0] = original[0]; \
											copy[1] = original[1]; \
											copy[2] = original[2]; \
                                  } \
//----------------------------------------------------------------------------

#define gts_vector_zero(vector) { \
											vector[0] = 0; \
											vector[1] = 0; \
											vector[2] = 0; \
                                  } \

//----------------------------------------------------------------------------
													
#define       gts_point_norm(v)    (sqrt ((v).x*(v).x +\
                                          (v).y*(v).y +\
                                          (v).z*(v).z))
								  
//----------------------------------------------------------------------------
// calculate a vertex normal at a given GtsVertex on a given GtsSurface

gboolean gts_vertex_normal(GtsSurface* s, GtsVertex* v, 
						   double *x, double *y, double *z);

// define the wieghting method used for normal calculation
// by default use Gourand (computationally cheapest).

enum GtsVertexWeightOptions
	   { GTS_VERTEX_NORMAL_GOURAND, 
		   GTS_VERTEX_NORMAL_MAX, 
	     GTS_VERTEX_NORMAL_THURMER_WUTHRICH
		 };

extern GtsVertexWeightOptions GTS_VERTEX_NORMAL_WEIGHT;

//-----------------------------------------------------------------------------

// given two gts vectors return the (accute) angle between them in radians

double gts_vector_angle(GtsVector v1, GtsVector v2);

//-----------------------------------------------------------------------------

// given a pointer to a gts surface return the median edge length present
// on the surface (N.B. not true median but (max-min / 2) )

double gts_surface_median_edge_length(GtsSurface * s);

//-----------------------------------------------------------------------------

// given a pointer to a gts triangle return it's centre as the mean of its 
// three vertices

GtsPoint * gts_triangle_center(GtsTriangle *t, GtsPointClass * point_class);

//-----------------------------------------------------------------------------

// given a pointer to a gts surface return a pointer to a newly created gts 
// point lying at the euclidean centroid of the surface (i.e. centre of gravity) 
// computed as the mean euclidean position of all vertex points belonging to 
// the surface

// N.B. This point (in most cases) will not lie on the surface.

GtsPoint * gts_surface_centroid(GtsSurface * s);

//-----------------------------------------------------------------------------

// given a pointer to a gts vector v 
// then scale the vector by s

#define gts_vector_scale(v, s) {v[0] = v[0] * s; \
								v[1] = v[1] * s; \
								v[2] = v[2] * s;}

//-----------------------------------------------------------------------------

// given a GtsVector v return the length of that vector

#define gts_vector_length(v) gts_vector_norm(v)

//-----------------------------------------------------------------------------
					   
#endif
