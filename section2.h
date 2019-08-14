#pragma once

#include "section1.h"
//============================================================================================================================================================
/// Structs

typedef struct _SingleSourceMovesListCell {

	checkersPos *position;
	unsigned short captures;
	struct _SingleSourceMovesListCell *next;

}SingleSourceMovesListCell;

typedef struct _SingleSourceMovesList {

	SingleSourceMovesListCell *head;
	SingleSourceMovesListCell *tail;

}SingleSourceMovesList;

//============================================================================================================================================================
/// Function Declarations

// checks if list is empty
int isEmptyList(SingleSourceMovesList lst);

// makes empty list
void makeEmptyList(SingleSourceMovesList *list);

// shell for recursive optimal move finder
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMovesTree *moves_tree);

// recursive optimal move finder
SingleSourceMovesListCell *FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode *tNode, SingleSourceMovesList **optimalList, int *max);

// creates new list node
SingleSourceMovesListCell *createNewListNode(unsigned short captures, checkersPos *position, SingleSourceMovesListCell *next);

// inserts to head of list
void insertNodeToHeadList(SingleSourceMovesList *lst, SingleSourceMovesListCell *newHead);

// frees a single source moves list
void freeList(SingleSourceMovesList* lst);

// function that frees unnecessary lists and use an essential lists to dynamically build the optimal moves list
SingleSourceMovesListCell *modularMovementList(SingleSourceMovesList **optimalList, SingleSourceMovesList *unnecessaryList, SingleSourceMovesList *essentialList, int *max, int currentMax, SingleSourceMovesListCell *newNode);

// determines status about current player building optimal moves
void friendOrFoe(Board board, checkersPos *firstPos, checkersPos *secondPos, Player *player, Player *enemy, int *rowChange);

// calculates the potential danger of a givens pawns optimal move
void inDangerCalc(int *inDanger, Board board, SingleSourceMovesList *pawn, Player player, Player enemy, int rowChange);

// assistive function of 'inDangerCalc' for finding last captured enemy position on the board
void enemyPosition(Board board, int *enemyRow, int *enemyCol, Player player, Player enemy, int rowChange, int targetRow, int targetCol);
