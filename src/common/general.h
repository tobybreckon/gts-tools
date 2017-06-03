//
// Filename: general.h
//
// Description: general useful defs.
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

#ifndef GENERAL_HEADERS

#define GENERAL_HEADERS

//----------------------------------------------------------------------------

#define YEAR   "2006"
#define COPYRIGHT "\nGNU General Public License : Copyright (C) Toby Breckon"
#define BUGS "Report bugs to <gtstools@breckon.me.uk>."

#define PRINT_COPYRIGHT {fprintf(stderr, "%s %s\n", COPYRIGHT, YEAR);}
#define PRINT_BUGS {fprintf(stderr, "%s\n", BUGS);}
#define PRINT_BUILD {fprintf(stderr, "BUILD : %s %s GCC: %i.%i.%i\n", \
							 __DATE__, __TIME__, __GNUC__, 									 \
							 __GNUC_MINOR__, __GNUC_PATCHLEVEL__);}
//----------------------------------------------------------------------------								
							 
// Definiton of PI

#define PI 3.14159265

// mod operator
							 
#define mod(A,B) fmod((double) A, (double) B)
					 
//----------------------------------------------------------------------------

#endif
