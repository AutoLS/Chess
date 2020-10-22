#ifndef CHESS_H
#define CHESS_H

#include <vector>

#define CHESS_BOARD_X 8
#define CHESS_BOARD_Y 8
#define MAX_BOARD_SQUARES CHESS_BOARD_X*CHESS_BOARD_Y

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

enum chess_pieces
{
	P_PAWN_1,
	P_PAWN_2,
	P_PAWN_3,
	P_PAWN_4,
	P_PAWN_5,
	P_PAWN_6,
	P_PAWN_7,
	P_PAWN_8,
	P_ROOK_1,
	P_KNIGHT_1,
	P_BISHOP_1,
	P_QUEEN,
	P_KING,
	P_BISHOP_2,
	P_KNIGHT_2,
	P_ROOK_2,
	MAX_CHESS_PIECE,
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
	bool IsChecked;
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

bool IsSquareUnderCheckmate(player* Player, player* Opponent, int Index);

#endif