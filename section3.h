#pragma once

#include "section2.h"
//============================================================================================================================================================
/// Structs

typedef struct _multipleSourceMovesListCell {

	SingleSourceMovesList *single_source_moves_list;
	struct _multipleSourceMovesListCell *next;

}MultipleSourceMovesListCell;

typedef struct _multipleSourceMovesList {

	MultipleSourceMovesListCell *head;
	MultipleSourceMovesListCell *tail;

}MultipleSourceMovesList;

//============================================================================================================================================================
/// Function Declarations


// finds all optimal moves for a player on a given board
MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player);

// creates and inserts list cell to list
void insertCellToList(MultipleSourceMovesList *mainList, SingleSourceMovesList *singleList);

// returns linked list length 
int listLength(SingleSourceMovesList *list);

// free root in order (LDR)
void freeTree(SingleSourceMovesTreeNode *root);

// makes an empty MultipleSourceMovesList
void makeEmptyMultiList(MultipleSourceMovesList *list);
