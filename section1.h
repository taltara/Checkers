#pragma once

#include "declarations.h"
//============================================================================================================================================================
/// Structs

typedef struct _SingleSourceMovesTreeNode {

	Board board;
	checkersPos *pos;
	unsigned short total_captures_so_far;			 // total captures so far
	struct _SingleSourceMovesTreeNode *next_move[2]; // moves destitnations

}SingleSourceMovesTreeNode;

typedef struct _SingleSourceMovesTree {

	SingleSourceMovesTreeNode *source;

}SingleSourceMovesTree;

//============================================================================================================================================================
/// Function Declarations


// creates new tree node
SingleSourceMovesTreeNode* newNode(Board board, int row, int col, Player player);

// checks for valid player position and generates binary tree of posible moves
SingleSourceMovesTree *FindSingleSourceMoves(Board board, checkersPos *src);

// makes an empty tree
void makeEmptyTree(SingleSourceMovesTree *tree);

// recursive function for building binary tree of possible pawn moves
void recTreeMoveBuild(SingleSourceMovesTreeNode *tRoot, int row, int col, Player player, Board board, int capture);

// checks if dynamic alloacation is successful
void checkDynAllocation(void* ptr);

// rec function for finding optimal T moves
void playerTPossibleMoves(SingleSourceMovesTreeNode *tRoot, int row, int col, Player player, Player enemy, Board board, int capture);

// rec function for finding optimal B moves
void playerBPossibleMoves(SingleSourceMovesTreeNode *tRoot, int row, int col, Player player, Player enemy, Board board, int capture);

// copies checkers board
void copyBoard(Board curr, Board board);

// provides an updated list for each proposed pawn move
void updateBoard(Board board, checkersPos *origin, checkersPos *destination, Player player);
