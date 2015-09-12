#ifndef CHESS_LOGIC_H
#define CHESS_LOGIC_H

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include <time.h>
#include <ctype.h>


//damka
#define WHITE_M 0
//damka
#define BLACK_M 1


#define PLAYER_WINS "player wins!"
#define DEBUG 1
#define DAMKA 1

#define MIN 0
#define MAX 1

#define MAX_SCORE 400 //TODO: what to do?
#define MAX_VALUE 1000001
#define MIN_VALUE -1000001
#define TIE_SCORE -999999
#define MATE_WIN_SCORE 1000000
#define MATE_WIN_LOSE -1000000


#define WHITE_P 'm'		//pawn		hayal
#define WHITE_B 'b'		//bishop	ratz
#define WHITE_N 'n'		//knight	parash
#define WHITE_R 'r'		//rook		tzariakh
#define WHITE_Q 'q'		//queen		malka
#define WHITE_K 'k'		//king		melekh

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'
#define EMPTY ' '

#define PAWN	"pawn"		//pawn		hayal
#define BISHOP	"bishop"	//bishop	ratz
#define KNIGHT	"knight"	//knight	parash
#define ROOK	"rook"		//rook		tzariakh
#define QUEEN	"queen"		//queen		malka
#define KING	"king"		//king		melekh

#define WHITE "white"
#define BLACK "black"

#define WHITE_CAP "White"
#define BLACK_CAP "Black"

#define WHITE_PLAYER 1
#define NO_PLAYER 0
#define BLACK_PLAYER -1

#define BOARD_SIZE 8

#define BEST_BOARDS_NUM 1000000

#define XML_FIRST_LINE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define BESTval 0




typedef char** board_t;

typedef struct Coord {
	int i_coord;
	int j_coord;
} Coord;

typedef struct chessMove {
	char toolType;
	Coord src;
	Coord dst;
	int eaten;  //0 if no captures made. else the char that represents the eaten piece
	int promote; //0 if no promotion needed. else the char that represents the new promoted piece
	struct chessMove *next;
} cMove;

typedef enum Tool
{
	Pawn, Knight, Bishop, Rook, Queen, King, Empty
} eTool;





/* Externs */
extern int properties[6];
extern Coord WhiteKing;
extern Coord BlackKing;
extern Coord WhiteKingDangerZone[BOARD_SIZE * 2];
extern Coord BlackKingDangerZone[BOARD_SIZE * 2];


void print_board(board_t board);
void init_board(board_t board);
void init_rowsE(board_t board);
void init_rowP(board_t board, int player);
void init_rowV(board_t board, int player);
int generateEnemyColor(int playerColor);
char generateTool(int player, eTool type);
int IsEnemy(char slot, int myColor);
int isInBoard(Coord coord);
int isTool(char c);
int promotePawn(int row, char myType);
char GetContentOfCoord(board_t board, Coord coord);
void setSlotInBoard(board_t board, Coord slot, char ch);
int IsAlly(char tool, int myColor);
Coord offsetCoord(Coord coord, int i_offset, int j_offset);
cMove *AddMove(cMove **head, char toolType, Coord src, Coord dst, int eater, int promote);
int addToDangerZone(int playerColor, Coord crd);
int ResetDangerZone(int playerColor);
int UpdateDangerZone(board_t board, int playerColor);
int DoesCrdContainsEnemy(board_t board, Coord coord, char tool);
int getColor(board_t board, Coord coord);
int getType(board_t board, Coord crd);
int AreTwoCoordsEqual(Coord c1, Coord c2);
int isAttacking(board_t board, Coord attacker, Coord victim);
int openKingDefences(board_t board, Coord src, Coord dst);
int safeToMoveKing(board_t board, int player, Coord dst);
cMove* PawnMoves(board_t board, Coord coord);
cMove* BishopMoves(board_t board, Coord coord);
cMove* RookMoves(board_t board, Coord coord);
cMove* KnightMoves(board_t board, Coord coord);
cMove *QueenMoves(board_t board, Coord coord);
cMove* KingMoves(board_t board, Coord coord);
cMove *movesByPieceType(board_t board, Coord coord);
cMove *getMoves(board_t board, int player);
int CountToolsOfType(board_t board, char type);
int NotTooManyOfType(board_t board, char type);
int score(board_t board, int player);
int canMoveThisTool(board_t board, Coord coord);
int isWhite(char type);
//int minimax_score(board_t board, int player, int depth, int minOrMax, cMove **bestMove, int a, int b, int boardsCounter);
//int makeMove_ComputeScore_Undo(board_t board, cMove *move, int player, int depth, int minOrMax, int a, int b, int boardsCounter);
int makeMove(board_t board, cMove *move);
char ToolNameToChar(char *toolFullName);
char* ToolCharToName(char toolChar);
int isEqualCoordinates(Coord c1, Coord c2);
void clearBoard(board_t board);
const char* getPlayerColor();
const char* getComputerColor();
char getGenericTool(int isComputer);
char getCorrespondingKing(char type);
int quit(void);
int imax(int a, int b);
int imin(int a, int b);
int mod(int a, int b);
int KingUnderThreat(board_t board, int player);
int Save(board_t board, char* file_name);
int LoadFromFile(char* file_path, board_t board);
void printMove(cMove *move);
void printMovesList(cMove *move);
int freeMovesList(cMove *move);
Coord GenerateCoord(int x, int y);
void printMoveToFile(FILE *f, cMove *move);

#endif

