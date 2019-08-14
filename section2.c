#include "section2.h"

//==========================================================================================================================================================
/// Functions

// makes a list empty
void makeEmptyList(SingleSourceMovesList *list)
{
	list->head = list->tail = NULL;
}

//checks if the list is empty
int isEmptyList(SingleSourceMovesList lst)
{
	return (lst.head == NULL);
}

// creates new list node
SingleSourceMovesListCell *createNewListNode(unsigned short captures, checkersPos *position, SingleSourceMovesListCell *next)
{
	//function that creates new node
	SingleSourceMovesListCell *res;

	res = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell)); //create the node
	checkDynAllocation(res);

	// inserts number of captures
	res->captures = captures;

	res->position = (checkersPos *)malloc(sizeof(checkersPos));
	checkDynAllocation(res->position);

	//updates the checkers position in the node
	res->position->row = position->row;
	res->position->col = position->col;

	res->next = next; //we point the node into the next node that comes after

	return res;
}

// inserts to head of list
void insertNodeToHeadList(SingleSourceMovesList *lst, SingleSourceMovesListCell *newHead)
{
	if (isEmptyList(*lst))
	{
		// The node we are about to insert will create a singleton list
		// so both head and tail should point at this node 
		lst->tail = newHead;
	}
	newHead->next = lst->head;	// point new node at the start of the list
	lst->head = newHead;		// update list's head
}

// shell for recursive function to find optimal pawn move
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMovesTree *moves_tree)
{
	//SingleSourceMovesList *newList;
	SingleSourceMovesList *optimalList;

	SingleSourceMovesList *pOptimalList;
	pOptimalList = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkDynAllocation(pOptimalList);
	makeEmptyList(pOptimalList); //makes the list empty

	int max = -1;

	pOptimalList->head = FindSingleSourceOptimalMoveRec(moves_tree->source, &optimalList, &max);
	pOptimalList->tail = optimalList->tail;

	// freeing unneeded 'optimalList' list
	free(optimalList);

	return pOptimalList;
}


// recursive function to find optimal pawn move
SingleSourceMovesListCell *FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode *tNode, SingleSourceMovesList **optimalList, int *max)
{
	SingleSourceMovesList *optimalListL = NULL, *optimalListR = NULL;
	SingleSourceMovesListCell *lNode, *rNode, *newNode;

	int maxL = -1, maxR = -1;

	if (tNode == NULL) return NULL;

	if (tNode->next_move[LEFT] == NULL && tNode->next_move[RIGHT] == NULL)
	{
		*optimalList = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
		checkDynAllocation(*optimalList);
		makeEmptyList(*optimalList);

		newNode = createNewListNode(tNode->total_captures_so_far, tNode->pos, NULL);
		insertNodeToHeadList(*optimalList, newNode);

		*max = *max + 1;
		return newNode;
	}

	lNode = FindSingleSourceOptimalMoveRec(tNode->next_move[LEFT], &optimalListL, &maxL);
	rNode = FindSingleSourceOptimalMoveRec(tNode->next_move[RIGHT], &optimalListR, &maxR);

	if (lNode != NULL && rNode != NULL)
	{	// deciding best path by total number of captures
		if (lNode->captures > rNode->captures)		maxL += 1;
		else if (lNode->captures < rNode->captures)		maxR += 1;
	}

	if (maxL > maxR) // if list should have the left node
	{
		newNode = createNewListNode(tNode->total_captures_so_far, tNode->pos, lNode);
		insertNodeToHeadList(optimalListL, newNode);

		return modularMovementList(optimalList, optimalListR, optimalListL, max, maxL, newNode);
	}
	else if (maxR > maxL) // if list should have the right node
	{
		newNode = createNewListNode(tNode->total_captures_so_far, tNode->pos, rNode);
		insertNodeToHeadList(optimalListR, newNode);

		return modularMovementList(optimalList, optimalListL, optimalListR, max, maxR, newNode);
	}
	else if (maxL == maxR) // determining optimal move build by finding potential dangerous moves when equal in validness
	{
		int inDanger1 = 0, inDanger2 = 0, rowChange, random;
		Player player = SPACE, enemy = SPACE;

		// checking security for potential left move
		SingleSourceMovesListCell *newNodeL = createNewListNode(tNode->total_captures_so_far, tNode->pos, lNode);
		insertNodeToHeadList(optimalListL, newNodeL);

		friendOrFoe(tNode->board, lNode->position, newNodeL->position, &player, &enemy, &rowChange);	// finds info about current player for both checks
		inDangerCalc(&inDanger1, tNode->board, optimalListL, player, enemy, rowChange);			// checks dangerous moves for possible left move

		//checking security for potential right move
		SingleSourceMovesListCell *newNodeR = createNewListNode(tNode->total_captures_so_far, tNode->pos, rNode);
		insertNodeToHeadList(optimalListR, newNodeR);

		inDangerCalc(&inDanger2, tNode->board, optimalListR, player, enemy, rowChange);			// checks dangerous moves for possible right move

		if (inDanger1 == inDanger2)
		{
			random = (rand() % K); // randomizing preference if no smart move available (chance = 1/2)
			if (random % 2 == LEFT)		        inDanger1 += 1;
			else if (random % 2 == RIGHT)				inDanger2 += 1;
		}

		if (inDanger1 < inDanger2) return modularMovementList(optimalList, optimalListR, optimalListL, max, maxL, newNodeL);
		else if (inDanger1 > inDanger2) return modularMovementList(optimalList, optimalListL, optimalListR, max, maxR, newNodeR);
	}
}

SingleSourceMovesListCell *modularMovementList(SingleSourceMovesList **optimalList, SingleSourceMovesList *unnecessaryList, SingleSourceMovesList *essentialList, int *max, int currentMax, SingleSourceMovesListCell *newNode)
{ // function that frees unnecessary lists and use an essential lists to dynamically build the optimal moves list
	
	if (unnecessaryList != NULL)
	{
		freeList(unnecessaryList);	// freeing the left list when unneeded
		free(unnecessaryList);
	}
	*optimalList = essentialList;

	*max = currentMax + 1;

	return newNode;
}

// determines status about current player building optimal moves
void friendOrFoe(Board board, checkersPos *firstPos, checkersPos *secondPos, Player *player, Player *enemy, int *rowChange)
{
	int check = firstPos->row - secondPos->row;
	if (check < 0)
	{
		*player = PlayerB;
		*enemy = PlayerT;
		*rowChange = -1;
	}
	else if (check > 0)
	{
		*player = PlayerT;
		*enemy = PlayerB;
		*rowChange = 1;
	}
}


//function that finds last captured enemy position on the board
void enemyPosition(Board board, int *enemyRow, int *enemyCol, Player player, Player enemy, int rowChange, int targetRow, int targetCol)
{
	if (player == PlayerT)
	{
		if (board[targetRow - rowChange][targetCol - rowChange] == enemy)
		{
			*enemyRow = targetRow - rowChange;
			*enemyCol = targetCol - rowChange;
		}
		else if (board[targetRow - rowChange][targetCol + rowChange] == enemy)
		{
			*enemyRow = targetRow - rowChange;
			*enemyCol = targetCol + rowChange;
		}
	}
	else if (player == PlayerB)
	{
		if (board[targetRow + rowChange][targetCol - rowChange] == enemy)
		{
			*enemyRow = targetRow + rowChange;
			*enemyCol = targetCol - rowChange;
		}
		else if (board[targetRow + rowChange][targetCol + rowChange] == enemy)
		{
			*enemyRow = targetRow + rowChange;
			*enemyCol = targetCol + rowChange;
		}
	}
}

// calculates the potential danger of a givens pawns optimal move
void inDangerCalc(int *inDanger, Board board, SingleSourceMovesList *pawn, Player player, Player enemy, int rowChange)
{
	int targetRow = pawn->tail->position->row - 'A';
	int targetCol = pawn->tail->position->col - '1';

	int enemyRow = 0, enemyCol = 0;
	int headshots = pawn->tail->captures;

	if (headshots > 0)	enemyPosition(board, &enemyRow, &enemyCol, player, enemy, rowChange, targetRow, targetCol);

	if (targetCol != (BOARD_SIZE - 1) && targetCol != 0)
	{
		// temporarily removing current questioned pawn's original position for accurate danger assessment
		board[pawn->head->position->row - 'A'][pawn->head->position->col - '1'] = SPACE;

		// if the move includes eating, remove temporarily the last enemy to be eaten for accurate danger assessment 
		if (headshots != 0) board[enemyRow][enemyCol] = SPACE;

		// checks rate of danger for pawn1
		if (board[targetRow + rowChange][targetCol + 1] == enemy && board[targetRow - rowChange][targetCol - 1] == SPACE)	*inDanger += 1;
		if (board[targetRow + rowChange][targetCol - 1] == enemy && board[targetRow - rowChange][targetCol + 1] == SPACE)	*inDanger += 1;

		// returning original position of last eaten enemy
		if (headshots != 0) board[enemyRow][enemyCol] = enemy;

		// returning original position of current pawn
		board[pawn->head->position->row - 'A'][pawn->head->position->col - '1'] = player;
	}
}

void freeList(SingleSourceMovesList* lst)
{ //function that frees the dynamic allocation of the list and the dynamic allocation of the data
	SingleSourceMovesListCell *curr = lst->head, *saver;

	while (curr != NULL)
	{
		saver = curr->next;
		free(curr->position);
		free(curr);
		curr = saver;
	}

	lst->head = lst->tail = NULL;
}