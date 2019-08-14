#include "section6.h"

//==========================================================================================================================================================
/// Functions

// retrives size file
long int fileSize(FILE* file)
{
	long int res;

	fseek(file, 0, SEEK_END);
	res = ftell(file);
	fseek(file, 0, SEEK_SET);

	return res;
}

// loads a board from a given file
void LoadBoard(char *filename, Board board)
{
   #define MSB 8 // MSB (Most Significant Bit)
   #define END_OF_BYTE -2

	FILE * boardFile;
	// getting a pointer to the requested file for board info in 'read binary' mode
	boardFile = fopen(filename, "rb");
	fileOpenCheck(boardFile); // checking if fopen was successful

	long int fSize = fileSize(boardFile);

	BYTE tempLine;
	BYTE mask = 0x01; //initialize the mask's LSB to 1

	int k = MSB - 2; //indicator when the byte is over (so won't be an overflow)
	int j; //index

	while (ftell(boardFile) < fSize)
	{
		fread(&tempLine, sizeof(BYTE), 1, boardFile); // opening requested file for board info

		// secondary loop for accessing board columns (right to left)
		for (j = 0; j < BOARD_SIZE / 2; j++) //the left half of the row
		{
			if (tempLine & (mask << k))										board[ftell(boardFile) / 2][j] = PlayerT;
			else if (tempLine & (mask << (k + 1)))									board[ftell(boardFile) / 2][j] = PlayerB;
			else if (!(tempLine & (mask << k)) && !(tempLine & (mask << (k + 1))))	board[ftell(boardFile) / 2][j] = SPACE;

			k -= 2;
		}

		if (k == END_OF_BYTE) //the left half of the row is full
		{
			k = MSB - 2;

			fread(&tempLine, sizeof(BYTE), 1, boardFile);

			for (j = BOARD_SIZE / 2; j < BOARD_SIZE; j++) //second half
			{
				if (tempLine & (mask << k))										board[ftell(boardFile) / 2 - 1][j] = PlayerT;
				else if (tempLine & (mask << (k + 1)))									board[ftell(boardFile) / 2 - 1][j] = PlayerB;
				else if (!(tempLine & (mask << k)) && !(tempLine & (mask << (k + 1))))	board[ftell(boardFile) / 2 - 1][j] = SPACE;

				k -= 2;
			}
		}
		k = MSB - 2;
	}

   #undef MSB
   #undef END_OF_BYTE

	fclose(boardFile); // closing the file from reading 
}