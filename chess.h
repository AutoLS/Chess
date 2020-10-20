#ifndef CHESS_H
#define CHESS_H

#include <vector>

#define CHESS_BOARD_X 8
#define CHESS_BOARD_Y 8
#define MAX_BOARD_SQUARES CHESS_BOARD_X*CHESS_BOARD_Y

#define MAX_CHESS_PIECE 16

enum game_state
{
	STATE_PLAYER_BLACK,
	STATE_PLAYER_WHITE,
};

enum move_state
{
	MOVE_IDLE,
	MOVE_DRAG,
};

enum piece_value
{
	PIECE_PAWN,
	PIECE_KNIGHT,
	PIECE_BISHOP,
	PIECE_ROOK,
	PIECE_QUEEN,
	PIECE_KING,
	PIECE_EMPTY,
};

enum chess_player
{
	PLAYER_WHITE,
	PLAYER_BLACK,
};

struct piece_entry
{
	piece_value Value;
	uint8 Location;
	chess_player Color;
	bool Dead;
	bool FirstMoveExpired;
	rect32 Rect;
};

struct test_piece_movable
{
	bool Result;
	int Index;
};

struct move_data
{
	move_state State;
	test_piece_movable Movable;
};

struct player
{
	piece_entry PiecesData[16];
	move_data MoveData;
	chess_player Color;
};

struct chess_board
{
	rect32 Tiles[CHESS_BOARD_X * CHESS_BOARD_Y];
	
	rect32 Rect;
	
	AE_Texture SquareA;
	AE_Texture SquareB;
};

struct chess_game
{
	chess_board Board;
	game_state State;
	
	player White;
	player Black;
	
	AE_Texture WhitePieces[6];
	AE_Texture BlackPieces[6];
};

#endif