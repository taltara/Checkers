#include"section4.h"

//==========================================================================================================================================================
/// Functions


void insertNodeToEndList(SingleSourceMovesList *lst, SingleSourceMovesListCell *newTail) 
{//function that updates list's nodes

	if (isEmptyList(*lst)) {
		//singleton list - both head and tail point at the single node
		lst->head = lst->tail = newTail;
	}
	else {
		newTail->next = NULL;
		lst->tail->next = newTail; //point last node at the new node
		lst->tail = newTail; //update the list's tail
	}
}


// makes the best possible move for given player
void Turn(Board board, Player player)
{
	if (player == PlayerT) printf("player TOP_DOWN's turn\n");
	else					printf("player BOTTOM_UP's turn\n");

	MultipleSourceMovesList *pawnMovesList = NULL;
	pawnMovesList = FindAllPossiblePlayerMoves(board, player);

	// stops the game if one of the players has no available moves
	if (pawnMovesList->head == NULL)	exit(0); // no moves is also a valid option (not an error)

	MultipleSourceMovesListCell *current = pawnMovesList->head;
	MultipleSourceMovesListCell *tempCurrent = NULL;

	int maxTemp = -1, smart;

	while (current)
	{
		if (current->single_source_moves_list->tail->captures > maxTemp) //updates the most essential move
		{
			tempCurrent = current;
			maxTemp = current->single_source_moves_list->tail->captures;
		}
		else if (current->single_source_moves_list->tail->captures == maxTemp) //randoms between the best moves
		{

			if (tempCurrent != NULL)
			{	//	Specialized Tactics (winning -> protection)
				if ((smart = Tactics(board, tempCurrent, current, player)) == 2)  tempCurrent = current;
			}
		}
		current = current->next;
	}
	printList(tempCurrent->single_source_moves_list); //prints the list

	deletePawn(board, player, tempCurrent);
	
	freeMultiList(pawnMovesList); //frees the contents in the list

	free(pawnMovesList);
}

// deletes pawn that were eaten by optimal move
void deletePawn(Board board, Player player, MultipleSourceMovesListCell *tempCurrent)
{
	// make current pawn dissappear
	board[tempCurrent->single_source_moves_list->head->position->row - 'A'][tempCurrent->single_source_moves_list->head->position->col - '1'] = SPACE;

	SingleSourceMovesList *deathNote = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkDynAllocation(deathNote);
	makeEmptyList(deathNote); //makes the list empty
	
	int death = 0;

	if (player == PlayerT)
	{
		if (tempCurrent->single_source_moves_list->tail->position->row - tempCurrent->single_source_moves_list->head->position->row > 1) death = 1;
	}
	else if (player == PlayerB)
	{
		if (tempCurrent->single_source_moves_list->tail->position->row - tempCurrent->single_source_moves_list->head->position->row < -1) death = 1;
	}

	if (death == 1)
	{
		SingleSourceMovesListCell *tempLast = NULL;
		tempLast = tempCurrent->single_source_moves_list->head;

		while (tempLast->next)
		{
			int tempRowMove = (tempLast->next->position->row - tempLast->position->row) / 2;
			int tempColMove = (tempLast->next->position->col - tempLast->position->col) / 2;
			// make current enemy pawn dissappear
			board[(tempLast->position->row - 'A') + tempRowMove][(tempLast->position->col - '1') + tempColMove] = SPACE;

			checkersPos *headShotPos = (checkersPos*)malloc(sizeof(checkersPos));
			checkDynAllocation(headShotPos);

			//updates the position
			headShotPos->row = (char)(((tempLast->position->row - 'A') + tempRowMove) + 'A');
			headShotPos->col = (char)(((tempLast->position->col - '1') + tempColMove) + '1');

			SingleSourceMovesListCell *headShot = createNewListNode(0, headShotPos, NULL);

			if (player == PlayerT) insertNodeToEndList(deathNote, headShot);
			else if (player == PlayerB)	insertNodeToHeadList(deathNote, headShot);
			free(headShotPos);
			tempLast = tempLast->next;
		}
	}
	// make current pawn reappear
	board[tempCurrent->single_source_moves_list->tail->position->row - 'A'][tempCurrent->single_source_moves_list->tail->position->col - '1'] = player;

	if (death == 1) freeList(deathNote);
	free(deathNote);
}

// player tactics central
int Tactics(Board board, MultipleSourceMovesListCell *pawn1, MultipleSourceMovesListCell *pawn2, Player player)
{
	int result = 0, end = -1;
	int random;

	if (player == PlayerT) end = (BOARD_SIZE - 1);
	else if (player == PlayerB) end = 0;

	// suicide prevention in third preference
	suicideTactics(board, pawn1, pawn2, player, &result, end);

	// if after all tactical calculation theres no verdict, randomize choice
	if (result == 0)
	{
		random = (rand() % K);

		if (random % 2 == 0) result = 1;
		else if (random % 2 == 1) result = 2;
	}
	return result;
}

// seeks potential suicides and calculates safest move
void suicideTactics(Board board, MultipleSourceMovesListCell *pawn1, MultipleSourceMovesListCell *pawn2, Player player, int *result, int end)
{
	char enemy = SPACE;
	int rowChange = 0, random;

	int inDanger1 = 0, inDanger2 = 0;

	if (player == PlayerT)
	{
		enemy = PlayerB;
		rowChange = 1;
	}
	else if (player == PlayerB)
	{
		enemy = PlayerT;
		rowChange = -1;
	}

	// pawn1 row and columns
	int targetRow1 = pawn1->single_source_moves_list->tail->position->row - 'A';
	int targetCol1 = pawn1->single_source_moves_list->tail->position->col - '1';
	// pawn2 row and columns
	int targetRow2 = pawn2->single_source_moves_list->tail->position->row - 'A';
	int targetCol2 = pawn2->single_source_moves_list->tail->position->col - '1';

	inDangerCalc(&inDanger1, board, pawn1->single_source_moves_list, player, enemy, rowChange);	// first pawn danger assessment
	inDangerCalc(&inDanger2, board, pawn2->single_source_moves_list, player, enemy, rowChange);	// second pawn danger assessment


	if (inDanger2 > inDanger1) *result = 1;
	else if (inDanger1 > inDanger2) *result = 2;
	else if (inDanger2 == inDanger1 && inDanger1 == 0)
	{// if there is no danger, randomize the next move

		random = (rand() % K);

		if (random % 2 == 0) *result = 1;
		else if (random % 2 == 1) *result = 2;

	}
}


void printList(SingleSourceMovesList *list)
{ //function that prints the list
	SingleSourceMovesListCell *curr = list->head;

	while (curr)
	{
		printf("%c%c", curr->position->row, curr->position->col);

		if (curr->next != NULL) printf("->");

		curr = curr->next;
	}
	printf("\n");
}


void freeMultiList(MultipleSourceMovesList* lst)
{ //function that frees the dynamic allocation of the list and the dynamic allocation of the data
	MultipleSourceMovesListCell *curr = lst->head, *saver;

	while (curr)
	{
		saver = curr->next;

		freeList(curr->single_source_moves_list);
		free(curr->single_source_moves_list);
		free(curr);
		curr = saver;
	}

	lst->head = lst->tail = NULL;
}