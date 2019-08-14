#pragma once

#include "section5.h"
//============================================================================================================================================================
/// Function Declarations

// loads a board from a given file
void LoadBoard(char *filename, Board board);

// retrives size file
long int fileSize(FILE* file);
