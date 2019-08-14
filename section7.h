#pragma once

#include "section6.h"
//============================================================================================================================================================
/// Function Declarations

// function to initialize current 'start' & 'limit' values by index 'i'
void starter(int *start, int* limit, int i);

// determines winner from given board
char winning(Board board, int *Tcount, int *Bcount);

// playes the game until winner is concluded
void PlayGame(Board board, Player starting_player);

// prints board
void printBoard(Board board);
