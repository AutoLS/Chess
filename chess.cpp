#include "chess.h"

chess_board InitChessBoard(AE_Graphics* Graphics)
{
	chess_board Board;
	
	v2 BoardDim = V2(800, 800);
	Board.Rect = {V2(0, 0), BoardDim};
	real32 AspectRatio = BoardDim.x / BoardDim.y;
		
	real32 RectWidth = 100;
	real32 RectHeight = 100;
	
	for(int y = 0; y < CHESS_BOARD_Y; ++y)
	{
		for(int x = 0; x < CHESS_BOARD_X; ++x)
		{
			v2 RectDim = V2(RectWidth, RectHeight);
			v2 RectPos = V2(RectWidth * x, RectHeight * y);
			
			Board.Tiles[y * CHESS_BOARD_X + x] = Rect32(RectPos, RectDim);
		}
	}
	
	Board.SquareA = AE_LoadTexture(Graphics, "ast/textures/squareA.png");
	Board.SquareB = AE_LoadTexture(Graphics, "ast/textures/squareB.png");
	
	return Board;
}

void InitPlayers(chess_game* Chess)
{
/*  Chess board
	R k B Q K B k R
	P P P P P P P P 
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	P P P P P P P P 
	R k B Q K B k R
*/
	
	player* White = &Chess->White;
	player* Black = &Chess->Black;

	for(int i = 0; i < 8; ++i)
	{
		Black->PiecesData[i] = {PIECE_PAWN, (uint8)(i+8), PLAYER_BLACK};
	}
	
	Black->PiecesData[8] = {PIECE_ROOK, 0, PLAYER_BLACK};
	Black->PiecesData[9] = {PIECE_KNIGHT, 1, PLAYER_BLACK};
	Black->PiecesData[10] = {PIECE_BISHOP, 2, PLAYER_BLACK};
	Black->PiecesData[11] = {PIECE_QUEEN, 3, PLAYER_BLACK};
	Black->PiecesData[12] = {PIECE_KING, 4, PLAYER_BLACK};
	Black->PiecesData[13] = {PIECE_BISHOP, 5, PLAYER_BLACK};
	Black->PiecesData[14] = {PIECE_KNIGHT, 6, PLAYER_BLACK};
	Black->PiecesData[15] = {PIECE_ROOK, 7, PLAYER_BLACK};
	
	
	Black->Color = PLAYER_BLACK;
	
	for(int i = 0; i < 8; ++i)
	{
		White->PiecesData[i] = {PIECE_PAWN, (uint8)(48 + i), PLAYER_WHITE};
	}
	
	White->PiecesData[8] = {PIECE_ROOK, 56, PLAYER_WHITE};
	White->PiecesData[9] = {PIECE_KNIGHT, 57, PLAYER_WHITE};
	White->PiecesData[10] = {PIECE_BISHOP, 58, PLAYER_WHITE};
	White->PiecesData[11] = {PIECE_QUEEN, 59, PLAYER_WHITE};
	White->PiecesData[12] = {PIECE_KING, 60, PLAYER_WHITE};
	White->PiecesData[13] = {PIECE_BISHOP, 61, PLAYER_WHITE};
	White->PiecesData[14] = {PIECE_KNIGHT, 62, PLAYER_WHITE};
	White->PiecesData[15] = {PIECE_ROOK, 63, PLAYER_WHITE};
	
	White->Color = PLAYER_WHITE;
	
	chess_board* Board = &Chess->Board;
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		int WhitePieceValue = White->PiecesData[i].Value;
		int BlackPieceValue = Black->PiecesData[i].Value;
		
		int WhiteTileIndex = White->PiecesData[i].Location;
		int BlackTileIndex = Black->PiecesData[i].Location;
		
		White->PiecesData[i].Rect = Board->Tiles[WhiteTileIndex];
		Black->PiecesData[i].Rect = Board->Tiles[BlackTileIndex];
	}
}

chess_game InitChess(AE_Core* Core)
{
	AE_Graphics* Graphics = Core->Graphics;
	
	chess_game Chess = {};
	Chess.Board = InitChessBoard(Graphics);
	InitPlayers(&Chess);
	Chess.State = STATE_PLAYER_WHITE;
	
	Chess.WhitePieces[PIECE_PAWN] = 
	AE_LoadTexture(Graphics, "ast/textures/pawn_white.png");
	Chess.WhitePieces[PIECE_KNIGHT] = 
	AE_LoadTexture(Graphics, "ast/textures/knight_white.png");
	Chess.WhitePieces[PIECE_BISHOP] = 
	AE_LoadTexture(Graphics, "ast/textures/bishop_white.png");
	Chess.WhitePieces[PIECE_ROOK] =
	AE_LoadTexture(Graphics, "ast/textures/rook_white.png");
	Chess.WhitePieces[PIECE_QUEEN] = 
	AE_LoadTexture(Graphics, "ast/textures/queen_white.png");
	Chess.WhitePieces[PIECE_KING] = 
	AE_LoadTexture(Graphics, "ast/textures/king_white.png");
	
	Chess.BlackPieces[PIECE_PAWN] = 
	AE_LoadTexture(Graphics, "ast/textures/pawn_black.png");
	Chess.BlackPieces[PIECE_KNIGHT] = 
	AE_LoadTexture(Graphics, "ast/textures/knight_black.png");
	Chess.BlackPieces[PIECE_BISHOP] = 
	AE_LoadTexture(Graphics, "ast/textures/bishop_black.png");
	Chess.BlackPieces[PIECE_ROOK] =
	AE_LoadTexture(Graphics, "ast/textures/rook_black.png");
	Chess.BlackPieces[PIECE_QUEEN] = 
	AE_LoadTexture(Graphics, "ast/textures/queen_black.png");
	Chess.BlackPieces[PIECE_KING] = 
	AE_LoadTexture(Graphics, "ast/textures/king_black.png");
	
	return Chess;
}

test_piece_movable TestPieceMovable(int TileIndex, player* Player)
{
	test_piece_movable Ret = {};
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		piece_entry* Piece = &Player->PiecesData[i];
		if(Piece->Location == TileIndex)
		{
			if(Piece->Color == Player->Color && !Piece->Dead)
			{
				Ret.Result = true;
				Ret.Index = i;
				break;
			}
		}
	}
	
	return Ret;
}

int GetCurrentMouseTileIndex(AE_Input* Input, chess_board* Board)
{
	int x = Input->MouseState.x / (int)Board->Tiles[0].Dim.x;
	int y = Input->MouseState.y / (int)Board->Tiles[0].Dim.y;
	
	return y * CHESS_BOARD_X + x;
}

void ResetPiecePosition(player* Player, chess_board* Board, int PieceIndex)
{
	int TileIndex = Player->PiecesData[PieceIndex].Location;
	Player->PiecesData[PieceIndex].Rect = Board->Tiles[TileIndex];
	Player->MoveData.State = MOVE_IDLE;
}

int GetTileNumber(int TileIndex)
{
	int ReverseIndex = (64 - TileIndex);
	if(ReverseIndex % 8 == 0)
	{
		return ReverseIndex / 8;
	}
	else
	{
		return (ReverseIndex / 8) + 1;		
	}
}

char GetTileLetter(int TileIndex)
{
	int x = TileIndex % 8;
	switch(x)
	{
		case 0: return 'a';
		case 1: return 'b';
		case 2: return 'c';
		case 3: return 'd';
		case 4: return 'e';
		case 5: return 'f';
		case 6: return 'g';
		case 7: return 'h';
	}
	return 0;
}

char GetPieceLetter(piece_value n)
{
	switch(n)
	{
		case PIECE_PAWN: return 'p';
		case PIECE_KNIGHT: return 'k';
		case PIECE_BISHOP: return 'b';
		case PIECE_ROOK: return 'r';
		case PIECE_QUEEN: return 'q';
		case PIECE_KING: return 'K';
	}
	return 0;
}

int GetPieceIndexInTile(player* Player, int TileIndex)
{
	int Index = -1;
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		if(Player->PiecesData[i].Location == TileIndex &&
		   !Player->PiecesData[i].Dead)
		{
			Index = i;
			break;
		}
	}
	return Index;
}

piece_entry* ContainPiece(player* Player, int TileIndex)
{
	piece_entry* Result = 0;
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		piece_entry* Piece = &Player->PiecesData[i];
		if(Piece->Location == TileIndex && !Piece->Dead)
		{
			Result = Piece;
			return Result;
		}
	}
	
	return Result;
}

bool CheckValid(std::vector<int> Patterns, 
				player* Player, player* Opponent, int TileIndex)
{
	bool Result = false;
	for(int i = 0; i < Patterns.size(); ++i)
	{
		if(Patterns[i] == TileIndex)
		{
			piece_entry* PlayerPiece = ContainPiece(Player, TileIndex);
			piece_entry* OpponentPiece = ContainPiece(Opponent, TileIndex);
			
			if(PlayerPiece)
			{
				Result = false;
				break;
			}
			else if(OpponentPiece)
			{
				OpponentPiece->Dead = true;
				Result = true;
				break;
			}
			else 
			{
				Result = true;
				break;
			}
		}
	}
	
	return Result;
}

void UpdatePiece(piece_entry* Piece, chess_board* Board, int NewLocation)
{
	Piece->FirstMoveExpired = true;
	Piece->Location = NewLocation;
	Piece->Rect = Board->Tiles[NewLocation];
}

bool CheckPawnMove(player* Player, player* Opponent, 
				   piece_entry* Piece, chess_board* Board, int TileIndex)
{
	int CurrentLocation =  Piece->Location;
	int DiagLeft, DiagRight, Forward, ForwardTwo;
	if(Player->Color == PLAYER_WHITE)
	{
		DiagLeft = CurrentLocation - 9;
		DiagRight = CurrentLocation - 7;
		Forward = CurrentLocation - 8;
		ForwardTwo = CurrentLocation - 16;
	}
	else
	{
		DiagLeft = CurrentLocation + 7;
		DiagRight = CurrentLocation + 9;
		Forward = CurrentLocation + 8;
		ForwardTwo = CurrentLocation + 16;
	}
	
	bool Result = false;
	
	if(TileIndex == DiagLeft)
	{
		piece_entry* OpponentPiece = ContainPiece(Opponent, TileIndex);
		if(OpponentPiece)
		{
			OpponentPiece->Dead = true;
			Result = true;
		}
	}
	else if(TileIndex == DiagRight)
	{
		piece_entry* OpponentPiece = ContainPiece(Opponent, TileIndex);
		if(OpponentPiece)
		{
			OpponentPiece->Dead = true;
			Result = true;
		}
	}
	else if(TileIndex == Forward)
	{
		piece_entry* OpponentPiece = ContainPiece(Opponent, TileIndex);
		piece_entry* PlayerPiece = ContainPiece(Player, TileIndex);
		if(!OpponentPiece && !PlayerPiece)
		{
			Result = true;
		}
	}
	else if(TileIndex == ForwardTwo)
	{
		if(!Piece->FirstMoveExpired)
		{
			piece_entry* OpponentPiece = ContainPiece(Opponent, TileIndex);
			piece_entry* PlayerPiece = ContainPiece(Player, TileIndex);
			if(!OpponentPiece && !PlayerPiece)
			{
				Result = true;
			}
			else 
				Result = false;
		}
		else
		{
			Result = false;
		}
	}
	else
	{
		Result = false;
	}
	
	if(Result)
	{
		if(TileIndex < 8 || TileIndex > 55)
		{
			Piece->Value = PIECE_QUEEN;
		}
		UpdatePiece(Piece, Board, TileIndex);
	}
	
	return Result;
}

bool CheckKnightMove(player* Player, player* Opponent, 
					 piece_entry* Piece, chess_board* Board, int TileIndex)
{
	/*  o - Movable location
		x o x o x 
		o x x x o
		x x K x x
		o x x x o
		x o x o x 
		
		NOTE: Order from left most upper left circle clockwise
	*/
	
	int CurrentLocation = Piece->Location;
	std::vector<int> Patterns;
	
	Patterns.push_back(CurrentLocation - 10);
	Patterns.push_back(CurrentLocation - 17);
	Patterns.push_back(CurrentLocation - 15);
	Patterns.push_back(CurrentLocation - 6);
	Patterns.push_back(CurrentLocation + 10);
	Patterns.push_back(CurrentLocation + 17);
	Patterns.push_back(CurrentLocation + 15);
	Patterns.push_back(CurrentLocation + 6);
	
	bool Result = CheckValid(Patterns, Player, Opponent, TileIndex);
	
	if(Result) UpdatePiece(Piece, Board, TileIndex);
	
	
	return Result;
}

std::vector<int> 
GenerateDiagonalPatterns(player* Player, player* Opponent, 
						 int CoordX, int CoordY)
{
	std::vector<int> Patterns;
	
	int x = CoordX+1;
	int y = CoordY+1;
	while(x < CHESS_BOARD_X && y < CHESS_BOARD_Y)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		Patterns.push_back(Index);
		++y;
		++x;
	}
	
	x = CoordX-1;
	y = CoordY-1;
	while(x >= 0 && y >= 0)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		Patterns.push_back(Index);
		--x;
		--y;
	}
	
	x = CoordX+1;
	y = CoordY-1;
	while(x < 8 && y >= 0)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		Patterns.push_back(Index);
		++x;
		--y;
	}
	
	x = CoordX-1;
	y = CoordY+1;
	while(x >= 0 && y < 8)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		Patterns.push_back(Index);
		--x;
		++y;
	}
	
	return Patterns;
}

std::vector<int> 
GenerateHorVerPatterns(player* Player, player* Opponent, 
					   int CoordX, int CoordY)
{
	std::vector<int> Patterns;
	//Upper
	int x = CoordX;
	int y = CoordY-1;
	while(y >= 0)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		else
		{
			Patterns.push_back(Index);
			--y;			
		}
	}
	
	//Lower
	x = CoordX;
	y = CoordY+1;
	while(y < 8)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		else
		{
			Patterns.push_back(Index);
			++y;			
		}
	}
	
	//Left
	x = CoordX-1;
	y = CoordY;
	while(x >= 0)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		else
		{
			Patterns.push_back(Index);
			--x;			
		}
	}
	
	x = CoordX+1;
	y = CoordY;
	while(x < 8)
	{
		int Index = y * CHESS_BOARD_X + x;
		if(ContainPiece(Player, Index))
		{
			break;
		}
		else if(ContainPiece(Opponent, Index))
		{
			Patterns.push_back(Index);
			break;
		}
		else
		{
			Patterns.push_back(Index);
			++x;			
		}
	}
	
	return Patterns;
}

bool CheckBishopMove(player* Player, player* Opponent, 
					 piece_entry* Piece, chess_board* Board, int TileIndex)
{
	int CurrentLocation = Piece->Location;
	int CoordX = CurrentLocation % 8;
	int CoordY = CurrentLocation / 8;
	
	std::vector<int> Patterns = 
	GenerateDiagonalPatterns(Player, Opponent, CoordX, CoordY);
	
	bool Result = CheckValid(Patterns, Player, Opponent, TileIndex);
	
	if(Result) UpdatePiece(Piece, Board, TileIndex);
	
	return Result;
}

bool CheckRookMove(player* Player, player* Opponent, 
				   piece_entry* Piece, chess_board* Board, int TileIndex)
{
	int CurrentLocation = Piece->Location;
	int CoordX = CurrentLocation % 8;
	int CoordY = CurrentLocation / 8;
	
	std::vector<int> Patterns = 
	GenerateHorVerPatterns(Player, Opponent, CoordX, CoordY);
	
	bool Result = CheckValid(Patterns, Player, Opponent, TileIndex);
	
	if(Result) UpdatePiece(Piece, Board, TileIndex);
	
	return Result;
}

bool CheckQueenMove(player* Player, player* Opponent, 
					 piece_entry* Piece, chess_board* Board, int TileIndex)
{
	int CurrentLocation = Piece->Location;
	int CoordX = CurrentLocation % 8;
	int CoordY = CurrentLocation / 8;
	
	std::vector<int> Patterns =
	GenerateHorVerPatterns(Player, Opponent, CoordX, CoordY);
	
	std::vector<int> PatternsD =
	GenerateDiagonalPatterns(Player, Opponent, CoordX, CoordY);
	
	Patterns.insert(Patterns.end(), PatternsD.begin(), PatternsD.end());
	
	bool Result = CheckValid(Patterns, Player, Opponent, TileIndex);
	
	if(Result) UpdatePiece(Piece, Board, TileIndex);
	
	return Result;
}

bool CastleRight(player* Player, player* Opponent, 
				 piece_entry* Piece, chess_board* Board,
				 int TileIndex, int CoordX, int CoordY)
{
	bool Result = false;
	int RookIndex = GetPieceIndexInTile(Player, TileIndex);
	piece_entry* Rook = &Player->PiecesData[RookIndex];
	if(!Rook->FirstMoveExpired)
	{
		int x = CoordX+1;
		bool IsBlock = false;
		while(x < CHESS_BOARD_X-1)
		{
			int Index = CoordY * CHESS_BOARD_X + x;
			if(ContainPiece(Player, Index) || 
			   ContainPiece(Opponent, Index))
			{
				IsBlock = true;
				break;
			}
			
			if(IsSquareUnderCheckmate(Player, Opponent, Index))
			{
				IsBlock = true;
				break;
			}
			
			++x;
		}
		
		if(!IsBlock)
		{
			int NewKingIndex = TileIndex-1;
			int NewRookIndex = TileIndex-2;
			
			Piece->Location = NewKingIndex;
			Rook->Location = NewRookIndex;
			Piece->FirstMoveExpired = true;
			Piece->Rect = Board->Tiles[NewKingIndex];
			Rook->Rect = Board->Tiles[NewRookIndex];
			Result = true;
		}
	}
	
	return Result;
}

bool CastleLeft(player* Player, player* Opponent, 
				piece_entry* Piece, chess_board* Board,
				int TileIndex, int CoordX, int CoordY)
{
	int RookIndex = GetPieceIndexInTile(Player, TileIndex);
	piece_entry* Rook = &Player->PiecesData[RookIndex];
	
	bool Result = false;
	
	if(!Rook->FirstMoveExpired)
	{
		int x = CoordX-1;
		bool IsBlock = false;
		while(x > 0)
		{
			int Index = CoordY * CHESS_BOARD_X + x;
			if(ContainPiece(Player, Index) || 
			   ContainPiece(Opponent, Index))
			{
				IsBlock = true;
				break;
			}
			
			if(IsSquareUnderCheckmate(Player, Opponent, Index))
			{
				IsBlock = true;
				break;
			}	
			--x;
		}
		
		if(!IsBlock)
		{
			int NewKingIndex = TileIndex+2;
			int NewRookIndex = TileIndex+3;
			
			Piece->Location = NewKingIndex;
			Rook->Location = NewRookIndex;
			Piece->FirstMoveExpired = true;
			Piece->Rect = Board->Tiles[NewKingIndex];
			Rook->Rect = Board->Tiles[NewRookIndex];
			Result = true;
		}
	}
	
	return Result;
}

bool CheckKingMove(player* Player, player* Opponent, 
				   piece_entry* Piece, chess_board* Board, int TileIndex)
{
	//NOTE: Order from upper left clockwise
	int CurrentLocation = Piece->Location;
	int CoordX = CurrentLocation % 8;
	int CoordY = CurrentLocation / 8;
	int Patterns[8];
	
	Patterns[0] = CurrentLocation - 9;
	Patterns[1] = CurrentLocation - 8;
	Patterns[2] = CurrentLocation - 7;
	Patterns[3] = CurrentLocation + 1;
	Patterns[4] = CurrentLocation + 9;
	Patterns[5] = CurrentLocation + 8;
	Patterns[6] = CurrentLocation + 7;
	Patterns[7] = CurrentLocation - 1;
	
	bool Result = false;
	
	bool IsSquare = false;
	for(int i = 0; i < ArraySize(Patterns); ++i)
	{
		if(Patterns[i] == TileIndex)
		{
			piece_entry* PlayerPiece = ContainPiece(Player, TileIndex);
			piece_entry* OpponentPiece = ContainPiece(Opponent, TileIndex);
			
			if(PlayerPiece)
			{
				Result = false;
				IsSquare = true;
				break;
			}
			else if(OpponentPiece)
			{
				OpponentPiece->Dead = true;
				Result = true;
				IsSquare = true;
				break;
			}
			else 
			{
				Result = true;
				IsSquare = true;
				break;
			}
		}
	}
	
	if(!IsSquare && !Piece->FirstMoveExpired)
	{
		if(Player->Color == PLAYER_WHITE)
		{
			if(TileIndex == 63)
			{
				Result = CastleRight(Player, Opponent, Piece, Board,
									 TileIndex, CoordX, CoordY);
			}
			else if(TileIndex == 56)
			{
				Result = CastleLeft(Player, Opponent, Piece, Board,
									TileIndex, CoordX, CoordY);
			}
		}
		else
		{
			if(TileIndex == 7)
			{
				Result = CastleRight(Player, Opponent, Piece, Board,
									 TileIndex, CoordX, CoordY);
			}
			else if(TileIndex == 0)
			{
				Result = CastleLeft(Player, Opponent, Piece, Board,
									TileIndex, CoordX, CoordY);
			}
		}
		
	}
	else
	{
		if(Result) UpdatePiece(Piece, Board, TileIndex);
	}
	
	return Result;
}

bool CheckLegalMove(chess_game* Chess, int TileIndex, int PieceIndex)
{
	player* Player = Chess->State == STATE_PLAYER_WHITE ? 
					 &Chess->White : &Chess->Black;
					 
	player* Opponent = Chess->State == STATE_PLAYER_WHITE ? 
					   &Chess->Black : &Chess->White;		 
	
	piece_entry* Piece = &Player->PiecesData[PieceIndex];
	chess_board* Board = &Chess->Board;
	switch(Piece->Value)
	{
		case PIECE_PAWN:
		{
			return CheckPawnMove(Player, Opponent, Piece, Board, TileIndex);
		} break;
		case PIECE_KNIGHT:
		{
			return CheckKnightMove(Player, Opponent, Piece, Board, TileIndex);
		} break;
		case PIECE_BISHOP:
		{
			return CheckBishopMove(Player, Opponent, Piece, Board, TileIndex);
		} break;
		case PIECE_ROOK:
		{
			return CheckRookMove(Player, Opponent, Piece, Board, TileIndex);
		} break;
		case PIECE_QUEEN:
		{
			return CheckQueenMove(Player, Opponent, Piece, Board, TileIndex);
		} break;
		case PIECE_KING:
		{
			return CheckKingMove(Player, Opponent, Piece, Board, TileIndex);
		} break;
	}
	
	return false;
}

bool IsSquareUnderCheckmate(player* Player, player* Opponent, int Index)
{
	//TODO: Make a function for the loop in each control state since it's litterally the same
	int KingP = Index;
	
	std::vector<int> PossibleCheck;
	for(int PieceValue = PIECE_PAWN; PieceValue < PIECE_EMPTY; ++PieceValue)
	{
		switch(PieceValue)
		{
			case PIECE_PAWN:
			{
				if(Player->Color == PLAYER_WHITE)
				{
					PossibleCheck.push_back(KingP - 7);
					PossibleCheck.push_back(KingP - 9);
				}
				else
				{
					PossibleCheck.push_back(KingP + 7);
					PossibleCheck.push_back(KingP + 9);
				}
				
				for(int i = 0; i < PossibleCheck.size(); ++i)
				{
					piece_entry* Piece = ContainPiece(Opponent, PossibleCheck[i]);
					if(Piece)
					{
						if(Piece->Value == PieceValue)
						{
							return true;
						}
					}
				}
			} break;
			case PIECE_KNIGHT:
			{
				if(Player->Color == PLAYER_WHITE)
				{
					PossibleCheck.push_back(KingP - 10);
					PossibleCheck.push_back(KingP - 17);
					PossibleCheck.push_back(KingP - 15);
					PossibleCheck.push_back(KingP - 6);
				}
				else
				{
					PossibleCheck.push_back(KingP + 10);
					PossibleCheck.push_back(KingP + 17);
					PossibleCheck.push_back(KingP + 15);
					PossibleCheck.push_back(KingP + 6);
				}
				
				for(int i = 0; i < PossibleCheck.size(); ++i)
				{
					piece_entry* Piece = ContainPiece(Opponent, PossibleCheck[i]);
					if(Piece)
					{
						if(Piece->Value == PieceValue)
						{
							return true;
						}
					}
				}
			} break;
			case PIECE_BISHOP:
			{
				int CoordX = KingP % 8;
				int CoordY = KingP / 8;
				PossibleCheck = 
				GenerateDiagonalPatterns(Player, Opponent, CoordX, CoordY);
				
				for(int i = 0; i < PossibleCheck.size(); ++i)
				{
					piece_entry* Piece = ContainPiece(Opponent, PossibleCheck[i]);
					if(Piece)
					{
						if(Piece->Value == PieceValue)
						{
							return true;
						}
					}
				}
			} break;
			case PIECE_ROOK:
			{
				int CoordX = KingP % 8;
				int CoordY = KingP / 8;
				
				PossibleCheck = 
				GenerateHorVerPatterns(Player, Opponent, CoordX, CoordY);
				
				for(int i = 0; i < PossibleCheck.size(); ++i)
				{
					piece_entry* Piece = ContainPiece(Opponent, PossibleCheck[i]);
					if(Piece)
					{
						if(Piece->Value == PieceValue)
						{
							return true;
						}
					}
				}
			} break;
			
			case PIECE_QUEEN:
			{
				int CoordX = KingP % 8;
				int CoordY = KingP / 8;
				
				std::vector<int> P1 =
				GenerateHorVerPatterns(Player, Opponent, CoordX, CoordY);
				
				std::vector<int> P2 =
				GenerateDiagonalPatterns(Player, Opponent, CoordX, CoordY);
				
				P1.insert(P1.end(), P2.begin(), P2.end());
				
				for(int i = 0; i < P1.size(); ++i)
				{
					piece_entry* Piece = ContainPiece(Opponent, P1[i]);
					if(Piece)
					{
						if(Piece->Value == PieceValue)
						{
							return true;
						}
					}
				}
			} break;
			
			case PIECE_KING:
			{
				if(Player->Color == PLAYER_WHITE)
				{
					PossibleCheck.push_back(KingP - 9);
					PossibleCheck.push_back(KingP - 8);
					PossibleCheck.push_back(KingP - 7);
				}
				else
				{
					PossibleCheck.push_back(KingP + 9);
					PossibleCheck.push_back(KingP + 8);
					PossibleCheck.push_back(KingP + 7);
				}
				
				for(int i = 0; i < PossibleCheck.size(); ++i)
				{
					piece_entry* Piece = ContainPiece(Opponent, PossibleCheck[i]);
					if(Piece)
					{
						if(Piece->Value == PieceValue)
						{
							return true;
						}
					}
				}
			} break;
		}
		
		PossibleCheck.clear();
	}
	
	return false;
}

bool IsCheckmate(player* Player, player* Opponent)
{
	int OpponentKingP = Opponent->PiecesData[P_KING].Location;
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		piece_entry* Piece = &Player->PiecesData[i];
		switch(Piece->Value)
		{
			case PIECE_PAWN:
			{
				std::vector<int> PossibleCheck;
				if(Opponent->Color == PLAYER_WHITE)
				{
					PossibleCheck.push_back(OpponentKingP - 7);
					PossibleCheck.push_back(OpponentKingP - 9);
				}
				else
				{
					PossibleCheck.push_back(OpponentKingP + 7);
					PossibleCheck.push_back(OpponentKingP + 9);
				}
				
				for(int i = 0; i < PossibleCheck.size(); ++i)
				{
					if(Piece->Location == PossibleCheck[i])
					{
						return true;
					}
				}
			} break;
		}
	}
	
	return false;
}

void PlayerMove(AE_Input* Input, chess_game* Chess)
{
	chess_board* Board = &Chess->Board;
	player* Player = Chess->State == STATE_PLAYER_WHITE ? 
					 &Chess->White : &Chess->Black;
		
	player* Opponent = Chess->State == STATE_PLAYER_WHITE ? 
					   &Chess->Black : &Chess->White;	
	
	Player->MoveData.Movable.Result = false;	
		
	switch(Player->MoveData.State)
	{
		case MOVE_IDLE:
		{
			if(Input->Mouse.Keys[AE_MB_LEFT].IsDown)
			{
				int SquareIndex = GetCurrentMouseTileIndex(Input, Board);
				Player->MoveData.Movable = TestPieceMovable(SquareIndex, Player);
			}
			
			if(Player->MoveData.Movable.Result)
			{
				Player->MoveData.State = MOVE_DRAG;
			}
			else
			{
				return;
			}
		} break;
		
		case MOVE_DRAG:
		{
			int PieceIndex = Player->MoveData.Movable.Index;
			if(Input->Mouse.Keys[AE_MB_LEFT].IsDown)
			{
				Player->PiecesData[PieceIndex].Rect.Pos = 
				V2(Input->MouseState) - Player->PiecesData[PieceIndex].Rect.Dim*0.5f;
			}
			else
			{
				int TileIndex = GetCurrentMouseTileIndex(Input, Board);
				if(Player->PiecesData[PieceIndex].Location == TileIndex)
				{
					ResetPiecePosition(Player, Board, PieceIndex);
					return;
				}
				else if(TileIndex >= 0)
				{
					if(CheckLegalMove(Chess, TileIndex, PieceIndex))
					{
						Player->MoveData.State = MOVE_IDLE;
						
						int n = GetTileNumber(TileIndex);
						char l = GetTileLetter(TileIndex);
						char c = GetPieceLetter(Player->PiecesData[PieceIndex].Value);
						
						printf("%c_%c%d\n", c, l, n);
#if 1
						if(IsSquareUnderCheckmate(Opponent, Player, Opponent->PiecesData[P_KING].Location))
						{
							printf("Check\n");
						}
						Chess->State = Chess->State == STATE_PLAYER_WHITE ? 
									   STATE_PLAYER_BLACK : STATE_PLAYER_WHITE;
									   
#endif
					}
					else
					{
						ResetPiecePosition(Player, Board, PieceIndex);
						return;
					}
				}
			}
		} break;
	}
}

bool IsKingDead(player* Player)
{
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		if(Player->PiecesData[i].Value == PIECE_KING)
		{
			return Player->PiecesData[i].Dead;
		}
	}
	
	return false;
}

void DrawPieces(AE_Graphics* Graphics, player* Player, AE_Texture* Pieces)
{
	for(int i = 0; i < MAX_CHESS_PIECE; ++i)
	{
		if(!Player->PiecesData[i].Dead)
		{
			int PieceValue = Player->PiecesData[i].Value;
			AE_DrawTexture(Graphics, &Pieces[PieceValue], 
						   0, &Player->PiecesData[i].Rect);
		}
	}
}

void DrawBoard(AE_Graphics* Graphics, chess_game* Chess)
{
	chess_board* Board = &Chess->Board;
	
	SDL_Rect BoardViewport = ToSDLRect(&Board->Rect);
	SDL_RenderSetViewport(Graphics->Renderer, &BoardViewport);
	for(int y = 0; y < CHESS_BOARD_Y; ++y)
	{
		for(int x = 0; x < CHESS_BOARD_X; ++x)
		{
			int Index = y * CHESS_BOARD_X + x;
			if(y % 2 == 0)
			{
				x % 2 == 0 ? 
				AE_DrawTexture(Graphics, &Board->SquareA, 
							   0, &Board->Tiles[Index]) :
				AE_DrawTexture(Graphics, &Board->SquareB, 
							   0, &Board->Tiles[Index]);
			}
			else
			{
				x % 2 == 0 ? 
				AE_DrawTexture(Graphics, &Board->SquareB, 
							   0, &Board->Tiles[Index]) :
				AE_DrawTexture(Graphics, &Board->SquareA, 
							   0, &Board->Tiles[Index]);
			}
		}
	}
	
	player* White = &Chess->White;
	player* Black = &Chess->Black;
	
	if(Chess->State == STATE_PLAYER_WHITE)
	{
		DrawPieces(Graphics, Black, Chess->BlackPieces);
		DrawPieces(Graphics, White, Chess->WhitePieces);
	}
	else
	{
		DrawPieces(Graphics, White, Chess->WhitePieces);
		DrawPieces(Graphics, Black, Chess->BlackPieces);
	}
	
	SDL_Rect WindowViewport = ToSDLRect(&Graphics->WinRect);
	SDL_RenderSetViewport(Graphics->Renderer, &WindowViewport);
}