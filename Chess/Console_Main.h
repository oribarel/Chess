#ifndef CHESS_
#define CHESS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include <time.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_video.h>


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
#define MAX_VALUE 801
#define MIN_VALUE -801


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

#define WHITE_PLAYER 1
#define BLACK_PLAYER -1

#define BOARD_SIZE 8

#define BEST_BOARDS_NUM 1000000


typedef char** board_t;


// board initialization
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"  
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"
#define WHT_ENTER_YOUR_MOVE "white player - enter your move:\n"
#define BLK_ENTER_YOUR_MOVE "black player - enter your move:\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"

#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"
#define NOT_YOUR_PIECE "The specified position does not contain your piece\n"

#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message));


#define XML_FIRST_LINE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"

#define perror_message1(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name); properties[1] = 1; return NULL;)
#define BEST "best"
#define BESTval 5

typedef struct Coord {
	int i_coord;
	int j_coord;
} Coord;

typedef struct Move {
	int eater; //0 if no captures made. else, 1.
	int score;
	int len;//number of jumps, i.e the length of route minus 1
	struct Move *next; //next possible move
	Coord *route; //the coordinates of this move
} Move;

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


/*typedef struct chessMoveArray {
	cMove *arr;
	int length;
	int nonEmptyNum;
} chessMoveArr;*/

int Console_Main();
//void print_board(char **);
//void init_board(char **);
int mod(int, int);
//char *getLine(void);
//void print_line();
//board_t createBoard(void);
//void copyBoard(board_t, board_t);
//void printMove(cMove *move);
//void printMovesList(cMove *move);
//Move *createRegularMove(Coord src, Coord dst, int depth, int eater);
//Move *kingMoves(board_t board, Coord coord);
//Move *kingMovesDirected(board_t board, Coord coord, int n, int m);
//int IsEnemy(char slot, char myColor);
//int isInBoard(Coord coord);
//int isTool(char c);
//int anointAKing(int row, char myType);
//Move *moveAfterRec(Coord coord, int depth, Move *recursionMoves);
//char GetContentOfCoord(board_t board, Coord coord);
//void setSlotInBoard(board_t board, Coord slot, char ch);
//Move *manMoves(board_t board, Coord coord, int depth);
//int IsAlly(char tool, char player);
//cMove *getMoves(board_t board, int player);
//Move *filterOutMoves(Move *moves);
//int isEaterMove(Move *move);
//void setSlot(board_t board, char *horiz, char *vert, char type);
//void setBoard(board_t board, char *config);
//int canMoveThisTool(board_t board, Coord coord);
//int score(board_t board, char player);
//int isWhite(char type);
//char getEnemy(char player);
//int minimax_score(board_t board, char player, int depth, int minOrMax, Move **bestMove);
//int makeMove_ComputeScore_Undo(board_t board, Move *move, char player, int depth, int minOrMax);
//int makeMove(board_t board, Move *move);
//int imax(int a, int b);
//int imin(int a, int b);
//long random_at_most(long max);
//board_t GenerateRandomConfiguration();
//char *configuration(int num);
//board_t unitTest_movements(int i);
//int Parse(char *line, board_t board);
//Move* isLegalMove(char *token, board_t board, Move *allPossibleMoves);
//int areAllCoordsValid(char *token);
//int isLegalInitialPosition(char *token, board_t board);
//void graphicCoordToRealCoord(Coord *crd, char x_coor, int y_coor);
//void clearBoard(board_t board);
//const char* getPlayerColor();
//const char* getComputerColor();
//char getGenericTool(int isComputer);
//Move* chooseMoveRandonly(board_t brd);
//char getCorrespondingKing(char type);
//int printWinner(int computerWins);
//void quit(board_t brd, int freeBoard);
//int getColor(board_t board, Coord coord);
int quit(void);


#endif  
