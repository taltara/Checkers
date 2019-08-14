#include "section3.h"

//==========================================================================================================================================================
/// Functions

// makes an empty MultipleSourceMovesList
void makeEmptyMultiList(MultipleSourceMovesList *list)
{
	list->head = list->tail = NULL;
}

// checks list length
int listLength(SingleSourceMovesList *list)
{
	int size = 0; //counter
	SingleSourceMovesListCell *tempNode = list->head;

	while (tempNode)
	{
		size += 1;
		tempNode = tempNode->next;
	}
	return size;
}


// creates and inserts list cell to list
void insertCellToList(MultipleSourceMovesList *mainList, SingleSourceMovesList *singleList)
{
	MultipleSourceMovesListCell *newCell = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	checkDynAllocation(newCell);

	// inserts single pawn optimal move list to new cell
	newCell->single_source_moves_list = singleList;
	newCell->next = NULL;

	// if list is empty
	if (mainList->head == NULL && mainList->tail == NULL)
	{
		mainList->head = mainList->tail = newCell;

	}
	else
	{
		mainList->tail->next = newCell;
		mainList->tail = newCell;

	}
}

// finds all optimal moves for a player on a given board
MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player)
{
	MultipleSourceMovesList *pawnMovesList = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	checkDynAllocation(pawnMovesList);
	makeEmptyMultiList(pawnMovesList); //makes an empty multi list

	int mLimit = -1, start = 0, end = BOARD_SIZE, move = 1; //player T's limit is "-1" and player B's limit is
	int i, j; //indexes

	if (player == PlayerT) //make the moves from the end of player T's board
	{
		start = (BOARD_SIZE - 1);
		move = -1;
	}

	for (i = start; (i < end) && (i > mLimit); i += move)
	{
		for (j = start; (j < end) && (j > mLimit); j += move)
		{
			if (board[i][j] == player)
			{
				SingleSourceMovesTree *tempTree;
				SingleSourceMovesList *tempList;

				checkersPos *posi = (checkersPos*)malloc(sizeof(checkersPos));
				checkDynAllocation(posi);

				posi->row = (char)(i + 'A');
				posi->col = (char)(j + '1');

				tempTree = FindSingleSourceMoves(board, posi); //makes the tree of the moves for the current pawn
				tempList = FindSingleSourceOptimalMove(tempTree); //makes the list of the moves for the current pawn

				if (listLength(tempList) > 1)	insertCellToList(pawnMovesList, tempList);
				else
				{
					freeList(tempList);	// releasing & freeing list when it isn't needed for multiple moves
					free(tempList);
				}

				// releasing unneeded memory
				free(posi);
				freeTree(tempTree->source);	// releasing now unneeded tree memory
				free(tempTree);
			}
		}
	}
	return pawnMovesList;
}

// free root in order (LDR)
void freeTree(SingleSourceMovesTreeNode *root)
{
	if (root != NULL)
	{
		freeTree(root->next_move[LEFT]);
		freeTree(root->next_move[RIGHT]);
		free(root->pos);
		free(root);
	}
}