#pragma once

#include "section3.h"
//============================================================================================================================================================
/// Function Declarations

// inserts node to the end of the list
void insertNodeToEndList(SingleSourceMovesList *lst, SingleSourceMovesListCell *newTail);

// determines best move for player and acts
void Turn(Board board, Player player);

// deletes pawn that were eaten by optimal move
void deletePawn(Board board, Player player, MultipleSourceMovesListCell *tempCurrent);

// seeks potential suicides and calculates safest move
void suicideTactics(Board board, MultipleSourceMovesListCell *pawn1, MultipleSourceMovesListCell *pawn2, Player player, int *result, int end);

// player tactics central
int Tactics(Board board, MultipleSourceMovesListCell *pawn1, MultipleSourceMovesListCell *pawn2, Player player);

// prints single source list
void printList(SingleSourceMovesList *list);

// frees a multi move list
void freeMultiList(MultipleSourceMovesList* lst);