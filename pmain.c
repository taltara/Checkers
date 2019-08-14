#include "section7.h"
//============================================================================================================================================================
/// Main

// initializes board to start positions
void initializeBoard(Board board)
{
	//srand(time(NULL));

	int i, j;
	int mode = 0;

	for (i = 0; i < ROWS; i++)
	{
		if (i % 2 == 0) mode = 2;
		else			mode = 1;

		for (j = 0; j < COLS; j++)
		{
			//insert player token or space correctly
			if ((j % 2 == 0 && mode == 2) || i == 3 || i == 4)		board[i][j] = SPACE;
			else if ((i >= 0 && i <= 2) && ((j + i) % 2 == 1))			board[i][j] = PlayerT;
			else if ((i >= 5 && i <= 8) && ((j + i) % 2 == 1))			board[i][j] = PlayerB;
			else														board[i][j] = SPACE;
		}
	}
}

void main()
{

	Board board;
	Board fileBoard;
	initializeBoard(board);
	//dummyBoard(board);

	//printBoard(board);

	PlayGame(board, 'T');
	//printBoard(board);
	//StoreBoard(board, "SUP.bin");

	//LoadBoard("SUP.bin", fileBoard);

	//printBoard(fileBoard);
	//printf("%d\n", _CrtDumpMemoryLeaks());
}