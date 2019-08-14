#include "section5.h"

//==========================================================================================================================================================
/// Functions

// stores a given board in a binary file (empty - 00 | T player - 01 | B player - 10)
void StoreBoard(Board board, char *filename)
{
	FILE * boardFile;
	// getting a pointer to the requested new file for board info storage in 'write binary' mode
	boardFile = fopen(filename, "wb");
	fileOpenCheck(boardFile); // checks if the file has opened successfully

	BYTE binText = 0x00; // all the bits initialized to zero
	BYTE mask = 0x01; //initialize mask's LSB to 1

	int i, j;

	// main loop for accessing board rows for reading
	for (i = 0; i < BOARD_SIZE; i++)
	{
		// secondary loop for accessing board columns (right to left)
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == PlayerT)	binText ^= mask;		// entering '01' if player 'T' is present
			else if (board[i][j] == PlayerB)	binText ^= (mask << 1);	// entering '10' if player 'B' is present

			// writing the saved line whole to the requested binary file
			if (j == (BOARD_SIZE - 1) / 2 || j == BOARD_SIZE - 1)
			{
				fwrite(&binText, sizeof(BYTE), 1, boardFile);
				binText = 0x00; // reset binText to zero for the next four bits input
			}
			else						binText <<= 2;
		}
	}
	// closing the file from writing
	fclose(boardFile);
}

// checks if fopen function succeded to retreive file pointer
void fileOpenCheck(FILE *file)
{
	// if pointer returned null (error)
	if (file == NULL)
	{
		fprintf(stderr, "ERROR OPENING FILE\n");
		exit(1);
	}
}