/*********************************************************************

file_processing.h

This holds the functions that process files 
when the user does one of the following: 
- Does a Drag and Drop of a file onto the program.
- Clicks an Open button and selects a file. 
- Passes a file to the program on the command line. 
- Associates the program with a file type which opens the program and with it an image file. 

Eventually I would like to put a bunch of global stuff
into a file that included everywhere. 
Then I can start putting a bunch of stuff
into this file instead of having main.c be a giant ball of fuck. 

*********************************************************************/

#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H

// Main disk format info.
// We need this for the FORMAT_ID_TYPE enum 
// that gives us the identifiers like: 
// cbm1541_cdos_525_ssdd_0171k_d64
#include "main_format_reference.h"  

// We will probably need this as well.
#include "hardware.h" 

#endif /* FILE_PROCESSING_H */