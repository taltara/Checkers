#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//===================================================================================
/// Defines

#define ROWS 8
#define COLS 8
#define BOARD_SIZE 8
#define SPACE ' '
#define PlayerT 'T'
#define PlayerB 'B'
#define LEFT 0
#define RIGHT 1
#define K 1000

//============================================================================================================================================================
/// Typedef Declarations

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;

//============================================================================================================================================================
/// Structs
typedef struct _checkersPos {
	char row, col;
}checkersPos;
