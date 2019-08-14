#include "section7.h"

//==========================================================================================================================================================
/// Functions


// playes the game until winner is concluded
void PlayGame(Board board, Player starting_player)
{
	printBoard(board); //prints the board

	srand(time(NULL)); //makes the game randomaly in the relevant cases

	Player whoWon = SPACE;
	Player first = starting_player, second = SPACE, currentPlayer = SPACE;

	if (first == PlayerT)		second = PlayerB;
	else						second = PlayerT;

	int i; //index

	for (i = 0; (whoWon = winning(board, NULL, NULL)) == SPACE; i++)
	{
		if (i % 2 == 0) currentPlayer = starting_player;
		else if (i % 2 == 1) currentPlayer = second;

		Turn(board, currentPlayer);
	}

	if (whoWon == PlayerT)	printf("player BOTTOM_UP's turn\n");
	else if (whoWon == PlayerB)	printf("player TOP_DOWN's turn\n");
}


// function to initialize current 'start' & 'limit' values by index 'i'
void starter(int *start, int* limit, int i)
{
	if (i == 0) // 'start' & 'limit' values for B winner checking
	{
		*start = 0;
		*limit = BOARD_SIZE;
	}
	else if (i == 1) // 'start' & 'limit' values for T winner checking
	{
		*start = (BOARD_SIZE - 1) * (BOARD_SIZE);
		*limit = (BOARD_SIZE * BOARD_SIZE);
	}
	else if (i == 2) // 'start' & 'limit' values for pawn count checking
	{
		*start = BOARD_SIZE;
		*limit = (BOARD_SIZE * (BOARD_SIZE - 1));
	}
}


// funtion that determines winner from given board
char winning(Board board, int *Tcount, int *Bcount)
{
	// current winner nulled to space
	char winner = SPACE;
	int countT = 0, countB = 0;
	int start, limit, cell; // 'start' for initial checking position | 'limit' for max checking position | 'cell' for cell indexing
	int i;

	// main for loop for 3 stages of winner checking
	for (i = 0; (i < 3) && (winner == SPACE); i++)
	{
		cell = 0;						// initializing board cell indexer 
		starter(&start, &limit, i);	// gets initial 'start' & 'limit' values

		for (; (start < limit) && (winner == SPACE); start++)
		{
			if (i == 0)
			{
				if (board[0][start] == PlayerB) winner = PlayerB;
				else if (board[0][start] == PlayerT) countT += 1;
			}
			else if (i == 1)
			{
				if (board[(BOARD_SIZE - 1)][cell] == PlayerT) winner = PlayerT;
				else if (board[(BOARD_SIZE - 1)][cell] == PlayerB)	countB += 1;

				cell += 1;
			}
			else if (i == 2)
			{
				if (board[(start / BOARD_SIZE)][cell] == PlayerT) countT += 1;
				else if (board[(start / BOARD_SIZE)][cell] == PlayerB)	countB += 1;

				if (((start + 1) / BOARD_SIZE) != (start / BOARD_SIZE)) cell = -1;

				cell += 1;
			}
		}

		if (i == 2)
		{
			if (countB == 0 && countT > 0) winner = PlayerT;
			else if (countT == 0 && countB > 0) winner = PlayerB;
		}
	}

	return winner;
}

// function that prints the board of the game
void printBoard(Board board)
{
	int i, j;
	char counter = 'A';

	for (i = 0; i < ROWS; i++)
	{
		if (i == 0)
		{
			printf("+-+-+-+-+-+-+-+-+-+\n");
			printf("+ |1|2|3|4|5|6|7|8|\n");
		}

		printf("+-+-+-+-+-+-+-+-+-+\n");

		for (j = 0; j < COLS; j++)
		{
			if (j == 0)
			{
				printf("|%c|", counter);
				counter++;
			}
			printf("%c|", board[i][j]);
		}
		printf("\n");
	}
	printf("+-+-+-+-+-+-+-+-+-+\n");
}