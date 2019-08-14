#pragma once

#include "section4.h"
//============================================================================================================================================================
/// Typedef Declarations
typedef unsigned char BYTE;

//============================================================================================================================================================
/// Function Declarations

// saves given board to binary file
void StoreBoard(Board board, char *filename);

// checks if fopen function succeded to retreive file pointer
void fileOpenCheck(FILE *file);
