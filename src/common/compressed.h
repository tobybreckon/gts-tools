//
// Filename: compressed.h
//
// Description: handles file compression on input files
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

#include <iostream>

#include <glib.h>
#include <unistd.h>
#include <stdio.h>

//------------------------------------------------------------------------------

#ifndef _COMPRESSED_H
#define _COMPRESSED_H

class compressedFile{

	private:
	
		// internal variables storing file pointer and file name
	
		FILE * theFilePointer;
	
		char * theFileName;
	
	protected:
	
			// return values:
			// 0 = not compressed
			// 1 = gzip
			// 2 = bzip2
	
			int compressionUsed();
	
			// is the file compressed or not
	
			bool compressionOn;
			
	public :
	
		// boolean return value as to whether it is a compressed file or
		// not (based on suffix)
	
		bool isCompressed();
	
		// return a handle to the file
	
		FILE * handle();
	
		// open the file (return value is file handle, 
		// NULL return value indicates failure)
		
		FILE * open();
		
		// close file (return values as per fclose())
		
		int close();
		
		compressedFile(char * fileName);
		~compressedFile();
	
	
};

#endif /* _COMPRESSED_H */


//------------------------------------------------------------------------------
