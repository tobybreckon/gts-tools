//
// Filename: compressed.cc
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

// system includes

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// local includes

#include "compressed.h"

//------------------------------------------------------------------------------

compressedFile::compressedFile(char * fileName){
	
	
	theFileName = (char *) malloc(strlen(fileName) + 1);
	strcpy(theFileName, fileName);
	
	theFilePointer = NULL;
	
}

//------------------------------------------------------------------------------

compressedFile::~compressedFile(){
	
	close();
	free(theFileName);
}
//------------------------------------------------------------------------------

int compressedFile::compressionUsed(){

	if (strcasecmp((theFileName + (sizeof(char) * (strlen(theFileName) - 3))), ".gz") == 0)
	{
		cerr << "COMPRESSION: gzip compressed file : " 
			 << theFileName << endl;
		compressionOn = TRUE;
		return 1;
  } else if (strcasecmp((theFileName + (sizeof(char) * (strlen(theFileName) - 4))), ".bz2") == 0) { 
		cerr << "COMPRESSION: bzip2 compressed file : " 
	  		 << theFileName << endl;
		compressionOn = TRUE;
		return 2;
	} else {
		cerr << "COMPRESSION: no compression suffix found : " 
		     << theFileName << endl;
		compressionOn = FALSE;
		return 0;
	}
}
//------------------------------------------------------------------------------

FILE* compressedFile::open(){
	
	char * command = NULL;
	int size;
	
	// test file exists
	
	if(access(theFileName, R_OK) != 0){
		cerr << "COMPRESSION: error cannot access file " << theFileName << endl;
		return NULL;
 	}
	
	switch (compressionUsed())
	{
	case 1: // GZIP
		
		size = sizeof(char) * (strlen(theFileName) + strlen("gzip -dc ") + 2);
		command = (char *) malloc(size);
		snprintf(command, size, "gzip -dc %s", theFileName);
		theFilePointer = popen(command, "r");
		;
		break;
	case 2: // BZIP2
		size = sizeof(char) * (strlen(theFileName) + strlen("bzip2 -dc ") + 2);
		command = (char *) malloc(size);
	
		snprintf(command, size, "bzip2 -dc %s", theFileName);
		theFilePointer = popen(command, "r");
		;
		break;
	case 0:
			
		// not compressed
	
		theFilePointer = fopen(theFileName, "r");
		;
		break;
	}

	// free the command string	
	
	free(command);
	
	// safety catch all
	
	return theFilePointer;
	
}

//------------------------------------------------------------------------------

FILE* compressedFile::handle(){
	
	return theFilePointer;
	
}

//------------------------------------------------------------------------------

int compressedFile::close(){

	int returnValue;
	
	if(theFilePointer == NULL){
		return 0;
	} else if (compressionOn){
		returnValue = pclose(theFilePointer);
		theFilePointer = NULL;
	} else {
		returnValue = fclose(theFilePointer);
		theFilePointer = NULL;
	}
	
	return returnValue;
}
//------------------------------------------------------------------------------
