#include "section1.h"

//==========================================================================================================================================================
/// Functions

//makes an empty tree
void makeEmptyTree(SingleSourceMovesTree *tree)
{
	tree->source = NULL; //makes the root of the tree NULL termination
}

//function that checks if the dynamic allocation has been succeeded
void checkDynAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "Dynamic Allocation Has Failed.");
		exit(1);
	}
}

// creates new tree node
SingleSourceMovesTreeNode* newNode(Board board, int row, int col, Player player)
{

	SingleSourceMovesTreeNode* nody = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkDynAllocation(nody);

	nody->pos = (checkersPos*)malloc(sizeof(checkersPos));
	checkDynAllocation(nody->pos);

	// entering passed value to data of node
	copyBoard(nody->board, board); //copy the board to the board of the node

	nody->pos->row = (char)(row + 'A');
	nody->pos->col = (char)(col + '1');
	nody->total_captures_so_far = 0;

	nody->next_move[LEFT] = nody->next_move[RIGHT] = NULL;

	return nody;
}


// function that creates new binary tree for possible moves of selected pawn (if exists, returns null if it doesn't)
SingleSourceMovesTree *FindSingleSourceMoves(Board board, checkersPos *src)
{
	SingleSourceMovesTree *treeMoves;

	// differencianting 
	Player current = SPACE;

	if (board[src->row - 'A'][src->col - '1'] == PlayerT) current = PlayerT;
	else if (board[src->row - 'A'][src->col - '1'] == PlayerB) current = PlayerB;

	treeMoves = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	checkDynAllocation(treeMoves);
	makeEmptyTree(treeMoves);

	treeMoves->source = newNode(board, src->row - 'A', src->col - '1', current);

	if (current != SPACE) //if the current player isn't 'T' or 'B'
	{
		recTreeMoveBuild(treeMoves->source, src->row - 'A', src->col - '1', current, board, 0);
		return treeMoves;
	}
	free(treeMoves);

	return NULL;
}



// recursive function for building binary tree of possible pawn moves
void recTreeMoveBuild(SingleSourceMovesTreeNode *tRoot, int row, int col, Player player, Board board, int capture)
{
	// decides on ememy type by 'player'
	char enemy = ' ';
	if (player == 'T') enemy = 'B';
	else			   enemy = 'T';

	tRoot->total_captures_so_far = capture;

	//beyond the limit of the board
	if (tRoot->pos->row < 'A' || tRoot->pos->row > 'H' || tRoot->pos->col < '1' || tRoot->pos->col > '8')
	{
		free(tRoot);
		tRoot = NULL;
		return;
	}

	// if player is 'T'
	if (player == 'T')
	{
		playerTPossibleMoves(tRoot, row, col, player, enemy, board, capture);

	}// if player is 'B'
	else if (player == 'B')
	{
		playerBPossibleMoves(tRoot, row, col, player, enemy, board, capture);
	}

}


void playerTPossibleMoves(SingleSourceMovesTreeNode *tRoot, int row, int col, Player player, Player enemy, Board board, int capture) {

	if (board[row + 1][col - 1] == player && board[row + 1][col + 1] == player) return;
	if (board[row + 1][col - 1] == player && ((col + 1) >= BOARD_SIZE))			return;
	if (board[row + 1][col + 1] == player && ((col - 1) < 0))					return;

	// if left is space for player 'T'
	if (board[row + 1][col - 1] == SPACE && ((row + 1) < BOARD_SIZE) && ((col - 1) >= 0))
	{
		if (capture == 0)
		{
			tRoot->next_move[LEFT] = newNode(board, row + 1, col - 1, player);
			updateBoard(tRoot->next_move[LEFT]->board, tRoot->pos, tRoot->next_move[LEFT]->pos, player);	// updating every tree node's board with the presumed move
		}
	}
	// if right is space for player 'T'
	if (board[row + 1][col + 1] == SPACE && ((row + 1) < BOARD_SIZE) && ((col + 1) < BOARD_SIZE))
	{
		if (capture == 0)
		{
			tRoot->next_move[RIGHT] = newNode(board, row + 1, col + 1, player);
			updateBoard(tRoot->next_move[RIGHT]->board, tRoot->pos, tRoot->next_move[RIGHT]->pos, player);	// updating every tree node's board with the presumed move
		}
	}
	// if left is enemy for player 'T'
	if (board[row + 1][col - 1] == enemy)
	{
		// if theres a space on the left after enemy's location
		if (board[row + 2][col - 2] == SPACE && ((row + 2) < BOARD_SIZE) && ((col - 2) >= 0))
		{
			if (capture > 0)	tRoot->next_move[LEFT] = newNode(tRoot->board, row + 2, col - 2, player);	// if it's a series of captures, get board from last play for correct data
			else if (capture == 0)	tRoot->next_move[LEFT] = newNode(board, row + 2, col - 2, player);

			updateBoard(tRoot->next_move[LEFT]->board, tRoot->pos, tRoot->next_move[LEFT]->pos, player);	// updating every tree node's board with the presumed move
			recTreeMoveBuild(tRoot->next_move[LEFT], row + 2, col - 2, player, board, capture + 1);
		}
	}
	// if right is enemy for player 'T'
	if (board[row + 1][col + 1] == enemy)
	{
		// if theres a space on the right after enemy existance
		if (board[row + 2][col + 2] == SPACE && ((row + 2) < BOARD_SIZE) && ((col + 2) < BOARD_SIZE))
		{
			// if it's a series of captures, get board from last play for correct data
			if (capture > 0)	tRoot->next_move[RIGHT] = newNode(tRoot->board, row + 2, col + 2, player);
			else if (capture == 0)	tRoot->next_move[RIGHT] = newNode(board, row + 2, col + 2, player);

			updateBoard(tRoot->next_move[RIGHT]->board, tRoot->pos, tRoot->next_move[RIGHT]->pos, player);	// updating every tree node's board with the presumed move
			recTreeMoveBuild(tRoot->next_move[RIGHT], row + 2, col + 2, player, board, capture + 1);
		}
	}
}


//player 'B' possible moves
void playerBPossibleMoves(SingleSourceMovesTreeNode *tRoot, int row, int col, Player player, Player enemy, Board board, int capture) {

	if (board[row - 1][col - 1] == player && board[row - 1][col + 1] == player) return;
	if (board[row - 1][col - 1] == player && ((col + 1) >= BOARD_SIZE))			return;
	if (board[row - 1][col + 1] == player && ((col - 1) < 0))					return;

	// if left is space for player 'B'
	if (board[row - 1][col - 1] == SPACE && ((row - 1) >= 0) && ((col - 1) >= 0))
	{
		if (capture == 0)
		{
			tRoot->next_move[LEFT] = newNode(board, row - 1, col - 1, player);
			updateBoard(tRoot->next_move[LEFT]->board, tRoot->pos, tRoot->next_move[LEFT]->pos, player);	// updating every tree node's board with the presumed move
		}
	}
	// if right is space for player 'B'
	if (board[row - 1][col + 1] == SPACE && ((row - 1) >= 0) && ((col + 1) < BOARD_SIZE))
	{
		if (capture == 0)
		{
			tRoot->next_move[RIGHT] = newNode(board, row - 1, col + 1, player);
			updateBoard(tRoot->next_move[RIGHT]->board, tRoot->pos, tRoot->next_move[RIGHT]->pos, player);	// updating every tree node's board with the presumed move
		}
	}
	// if left is enemy for player 'B'
	if (board[row - 1][col - 1] == enemy)
	{
		// if theres a space on the left after enemy existance
		if (board[row - 2][col - 2] == SPACE && ((row - 2) >= 0) && ((col - 2) >= 0))
		{
			if (capture > 0)	tRoot->next_move[LEFT] = newNode(tRoot->board, row - 2, col - 2, player);	// if it's a series of captures, get board from last play for correct data
			else if (capture == 0)	tRoot->next_move[LEFT] = newNode(board, row - 2, col - 2, player);

			updateBoard(tRoot->next_move[LEFT]->board, tRoot->pos, tRoot->next_move[LEFT]->pos, player);	// updating every tree node's board with the presumed move
			recTreeMoveBuild(tRoot->next_move[LEFT], row - 2, col - 2, player, board, capture + 1);
		}
	}
	// if right is enemy for player 'B'
	if (board[row - 1][col + 1] == enemy)
	{
		// if theres a space on the right after enemy existance
		if (board[row - 2][col + 2] == SPACE && ((row - 2) >= 0) && ((col + 2) < BOARD_SIZE))
		{
			if (capture > 0)	tRoot->next_move[RIGHT] = newNode(tRoot->board, row - 2, col + 2, player);	// if it's a series of captures, get board from last play for correct data
			else if (capture == 0)	tRoot->next_move[RIGHT] = newNode(board, row - 2, col + 2, player);

			updateBoard(tRoot->next_move[RIGHT]->board, tRoot->pos, tRoot->next_move[RIGHT]->pos, player);	// updating every tree node's board with the presumed move
			recTreeMoveBuild(tRoot->next_move[RIGHT], row - 2, col + 2, player, board, capture + 1);
		}
	}
}

// provides an updated list for each proposed pawn move
void updateBoard(Board board, checkersPos *origin, checkersPos *destination, Player player)
{
	int eat = 0; // determining if an enemy pawn has been eaten
	if (destination->row - origin->row > 1 || destination->row - origin->row < -1) eat = 1;

	// deleting current players location
	board[origin->row - 'A'][origin->col - '1'] = SPACE;

	if (eat == 1)
	{
		int tempRowChange = (destination->row - origin->row) / 2;
		int tempColChange = (destination->col - origin->col) / 2;

		// deleting the eaten pawn by average of destination-origin change
		board[(origin->row - 'A') + tempRowChange][(origin->col - '1') + tempColChange] = SPACE;
	}

	// returning current player in end of move location
	board[destination->row - 'A'][destination->col - '1'] = player;
}


// function that copies current board
void copyBoard(Board curr, Board board)
{
	int i, j;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			curr[i][j] = board[i][j];
		}
	}
}