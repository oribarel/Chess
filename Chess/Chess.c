/*
The structs are:
- Coord: a coordinate on the board. see header file.
- Move(functions as a linked list node): which contains some data about the movement,
and also the route of coordinates (as an array) that the designated disk should do while making the move.

A brief explaination about implementation:
the board is two-dim char array, allocated statically only once in the main.
no board duplications were made. in order to achieve better performance the getMoves functions (of all kinds),
and the minimax function (including the	MakeMove_ComputeScore_Undo) make reversable alterations on the given board,
making board-duplications unnecessary.

public array properties contains 5 cells which are relevant throughtout the program. see below.

Functions that are labeled:
- DEBUG: are for debugging purposes only
- UNUSED: are unused although not deleted.

*/


#include "Chess.h"



int properties[6] = { 1, 0, 1, 0, 0, 0 };
/*	'settings'		0
'quit'				1
'minimax_depth'		2
'player color'		3
'current player'	4
'game mode'			5

the initial state of the global properties array:
properties[0] = 1;	//Setting state
properties[1] = 0;	//Don't quit
properties[2] = 1;	//Default minimax depth
properties[3] = 0;	//Default player color white
properties[4] = 0 //It is now white's\black's turn: 0 for white, 1 for black
properties[5] = 1;	//Default game mode is "two players mode"
*/
const char *constWhite = "white";
const char *constBlack = "black";

Move *pMove;

/*DEBUG: simulate failure of allocation*/
void *myalloc(int a, int b)
{
	static int i = 0;
	i++;
	//if (i % 500 == 0){ printf("Alloc #%d\n", i); }
	printf("Alloc #%d\n", i);
	if (i < 37)
	{
		return calloc(a, b);
	}
	else
	{
		return NULL;
	}
}


/*modulus of two integers a mod b*/
int mod(int a, int b)
{
	int ret = a % b;
	if (ret < 0)
		ret += b;
	return ret;
}

/*NO NEED FOR CONV*/
/*returns a char array which contains the input from the user.*/
char *getLine(void)
{
	char *line = calloc(20, 1), *linep = line;

	if (line == NULL)
	{
		if (!properties[1])
			perror_message("calloc");
		properties[1] = 1;
		return NULL;
	}

	char *linen = NULL;
	size_t lenMax = 20, len = lenMax;
	int c;

	for (;;)
	{
		c = fgetc(stdin);
		if (c == EOF)
		{
			if (!properties[1])
				perror_message("fgetc");
			properties[1] = 1;
			return NULL;
		}

		if (--len == 0)
		{
			len = lenMax;
			linen = realloc(linep, lenMax *= 2);

			if (linen == NULL)
			{
				if (!properties[1])
					perror_message("realloc");
				properties[1] = 1;
				return NULL;
			}

			line = linen + (line - linep);
			linep = linen;
		}

		if ((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}

/*NO NEED FOR CONV*/
/*Print a seperation line in the board*/
void print_line()
{
	int i;
	if (properties[1])
		return;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++)
	{
		printf("-");
	}
	printf("|\n");
}

/* Prints the board to command line*/
void print_board(board_t board)
{
	int i, j;
	if (board == NULL || properties[1])
		return;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

/*CONVD*/
/* Initializes the board to the regular game setting.*/
void init_board(board_t board)
// j is rows, i is columns
{
	int i = 0, j = 0;
	Coord crd;
	crd.i_coord = i; crd.j_coord = j;

	//fill white
	init_rowV(board, WHITE_PLAYER);
	init_rowP(board, WHITE_PLAYER);

	//fill black
	init_rowV(board, BLACK_PLAYER);
	init_rowP(board, BLACK_PLAYER);

	//empty
	init_rowsE(board);

}

/*DCLR*/
//empty
void init_rowsE(board_t board)
{
	Coord crd;
	int j = 2;
	int i = 0;
	crd.i_coord = i; crd.j_coord = j;
	for (; i < BOARD_SIZE; i++)
	{
		for (j = 2; j < 6; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			setSlotInBoard(board, crd, EMPTY);
		}
	}
}
/*DCLR*/
//pawns row
void init_rowP(board_t board, int player)
{
	Coord crd;
	int j = player == WHITE_PLAYER ? 1 : 6;
	int i = 0;
	crd.i_coord = i; crd.j_coord = j;
	for (; i < BOARD_SIZE; i++)
	{
		setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_P : BLACK_P);
	}
}
/*DCLR*/
//higher tools row
void init_rowV(board_t board, int player)
{
	Coord crd;
	int j = player==WHITE_PLAYER ? 0 : 7;
	int i = 0;
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_R : BLACK_R);
	crd.i_coord++;			  
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_N : BLACK_N);
	crd.i_coord++;			  
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_B : BLACK_B);
	crd.i_coord++;			  
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_K : BLACK_K);
	crd.i_coord++;			  
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_Q : BLACK_Q);
	crd.i_coord++;			  
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_B : BLACK_B);
	crd.i_coord++;			 
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_N : BLACK_N);
	crd.i_coord++;		
	setSlotInBoard(board, crd,player==WHITE_PLAYER ? WHITE_R : BLACK_R);
}

/*CONVD*/
/*prints the route of the move*/
void printMove(cMove *move)
{
	if (properties[1])
		return;
	printf("<%c,%u> to ", (char)((move->src).i_coord) + 97, (move->src).j_coord + 1);

	printf("<%c,%u>\n", (char)((move->dst).i_coord) + 97, (move->dst).j_coord + 1);
	//TODO: promotion
}

/*CONVD*/
/*prints an entire linked list of Moves*/
void printMovesList(cMove *move)
{
	while (move != NULL)
	{
		printMove(move);
		move = move->next;
	}
}

/*DCLR*/
/*CONVD*/
Move *createMove(Coord src, Coord dst, char eaten)
{
	cMove *move = (cMove *)calloc(1, sizeof(cMove)); //TODO:cast like that in all allocs
	if (move == NULL) // safety
	{
		if (!properties[1])
			perror_message("calloc");
		properties[1] = 1;
		return NULL;
	}

	(move->src).i_coord = src.i_coord;
	(move->src).j_coord = src.j_coord;

	(move->dst).i_coord = dst.i_coord;
	(move->dst).j_coord = dst.j_coord;

	move->eaten = eaten;
}

/*returns a linked list of moves the king in the coordinate coord may do (unfiltered)*/
Move *kingMoves(board_t board, Coord coord)
{
	Move *allMoves = NULL, *tmpMoves = allMoves, *recursionMoves;

	for (int n = -1; n < 2; n += 2)
	{
		for (int m = -1; m < 2; m += 2)
		{
			recursionMoves = (Move *)kingMovesDirected(board, coord, n, m);
			if (recursionMoves != NULL)
			{
				tmpMoves = recursionMoves;
				while (recursionMoves->next != NULL)
				{
					recursionMoves = recursionMoves->next;
				}
				recursionMoves->next = allMoves;
				allMoves = tmpMoves;
			}
		}
	}
	return allMoves;
}

cMove *QueenMoves(board_t board, Coord coord)
{
	cMove *rookAspect = NULL, *bishopAspect = NULL, *tmp;
	rookAspect = RookMoves(board, coord);
	bishopAspect = BishopMoves(board, coord);
	if (rookAspect == NULL)
		return bishopAspect;
	else
	{
		tmp = rookAspect;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = bishopAspect;
		return rookAspect;
	}

}

/*called by KingMoves func, returns the moves a king may do in (n,m) direction.*/
Move *kingMovesDirected(board_t board, Coord coord, int n, int m)
/* n,m are the directions in which the king traverses the board.*/
{

	Move *allMoves = NULL, *tmpMoves = allMoves, *recursionMoves;

	Coord crd;
	crd = coord;
	crd.i_coord += n; crd.j_coord += m;

	char myColor = GetContentOfCoord(board, coord);

	while (isInBoard(crd) && GetContentOfCoord(board, crd) == EMPTY)
		/* i.e. before eating anything: every empty slot on the way is a possible move, allegedly.*/
	{
		tmpMoves = allMoves;
		allMoves = createRegularMove(coord, crd, 0, 0);
		if (allMoves == NULL) //safety
			return tmpMoves;
		allMoves->next = tmpMoves;
		crd.i_coord += n; crd.j_coord += m;
	}
	if (isInBoard(crd) && IsEnemy(GetContentOfCoord(board, crd), myColor))
		/*enemy found in way.*/
	{
		crd.i_coord += n; crd.j_coord += m;
		if (isInBoard(crd) && GetContentOfCoord(board, crd) == EMPTY)
			/*there is an empty slot beyond the enemy.*/
		{
			//perform the capture:
			setSlotInBoard(board, coord, EMPTY);
			/*we are now on the far, empty slot. going back to the occupied one*/
			crd.i_coord -= n; crd.j_coord -= m;
			char tmpType = GetContentOfCoord(board, crd);
			setSlotInBoard(board, crd, EMPTY);
			/*advancing again to the far, empty slot.*/
			crd.i_coord += n; crd.j_coord += m;
			setSlotInBoard(board, crd, myColor);


			recursionMoves = (Move *)manMoves(board, crd, 1);

			/*turning back config to what it was:*/
			setSlotInBoard(board, crd, EMPTY);

			crd.i_coord -= n; crd.j_coord -= m;
			setSlotInBoard(board, crd, tmpType);

			setSlotInBoard(board, coord, myColor);

			crd.i_coord += n; crd.j_coord += m;
			if (recursionMoves == NULL)
			{
				tmpMoves = allMoves;
				allMoves = createRegularMove(coord, crd, 0, 1);
				if (allMoves == NULL) //safety
					return tmpMoves;
				allMoves->next = tmpMoves;
			}
			else
			{
				tmpMoves = (Move *)moveAfterRec(coord, 0, recursionMoves);
				tmpMoves->next = allMoves;
				allMoves = recursionMoves;
			}
		}
	}
	return allMoves;
}

/*CONVD*/
/*returns 0 if the two chars reperesent allies*/
int IsEnemy(char slot, char myColor)
{
	if (islower(myColor)) //player is white
	{
		switch (slot)
		{
		case BLACK_P:
			return 1;
			break;
		case BLACK_B:
			return 3;
			break;
		case BLACK_N:
			return 3;
			break;
		case BLACK_R:
			return 5;
			break;
		case BLACK_Q:
			return 9;
			break;
		case BLACK_K:
			return MAX_SCORE;
			break;
		default:
			return 0;
		}
	}
	if (isupper(myColor)) //player is white
	{
		switch (slot)
		{
		case WHITE_P:
			return 1;
			break;
		case WHITE_B:
			return 3;
			break;
		case WHITE_N:
			return 3;
			break;
		case WHITE_R:
			return 5;
			break;
		case WHITE_Q:
			return 9;
			break;
		case WHITE_K:
			return MAX_SCORE;
			break;
		default:
			return 0;
		}
	}
}

/*returns 1 if coord is within the boundaries of the board.*/
int isInBoard(Coord coord)
{
	if (coord.i_coord > -1 && coord.i_coord < BOARD_SIZE &&
		coord.j_coord > -1 && coord.j_coord < BOARD_SIZE)
	{
		return 1;
	}
	return 0;
}

/*CONVD*/
//TODO: change next line
/*return 1 if c is either 'm', 'k', 'M', 'K'etc.; otherwise 0.*/
int isTool(char c)
{
	if (c == WHITE_P || c == WHITE_B || c == WHITE_N || c == WHITE_R
		|| c == WHITE_Q || c == WHITE_K || c == BLACK_P || c == BLACK_B ||
		c == BLACK_N || c == BLACK_R || c == BLACK_Q || c == BLACK_K){
		return 1;
	}
	return 0;
}

/*DCLR*/
/* return whether or not a a pawn is promoted, by type and row number.*/
int promotePawn(int row, char myType)
{
	if (myType == WHITE_P && row == BOARD_SIZE - 1) { return 1; }
	if (myType == BLACK_P && row == 0) { return 1; }
	return 0;
}

/*DELETE*/
/*
adds coord coordinate to the Coord array in index depth.
Returns a pointer to the last move in the list.
*/
Move *moveAfterRec(Coord coord, int depth, Move *recursionMoves)
{
	Move *move = NULL;
	while (recursionMoves != NULL)
	{
		move = recursionMoves;
		*(recursionMoves->route + depth) = coord;
		recursionMoves = recursionMoves->next;
	}
	return move;
}

/*return the char in the specific location coord in the board.*/
char GetContentOfCoord(board_t board, Coord coord)
{
	return board[coord.i_coord][coord.j_coord];
}

/*TODO: consider this*/
/*sets the coordinate coord in the board to be ch.*/
void setSlotInBoard(board_t board, Coord slot, char ch)
{
	/*if (anointAKing(slot.j_coord, ch))
	board[slot.i_coord][slot.j_coord] = getCorrespondingKing(ch);
	else*/
	board[slot.i_coord][slot.j_coord] = ch;
}

/* j is rows, i is columns
when depth is 0 we create a new moves list for the man on i,j
when depth is greater that 0 it means we recursively determine a complex move.
Ultimately, returns all the moves a man on the coord in the board may do.*/
Move *manMoves(board_t board, Coord coord, int depth)
{

	Move *allMoves = NULL, *tmpMoves = allMoves, *recursionMoves;
	char myType = GetContentOfCoord(board, coord);
	char tmpType;
	int d;
	Coord crd;
	Coord exCrd;

	//d = (myType == WHITE_M || myType == WHITE_K) ? 1 : -1; //White or Black plays
	d = 1;

	//W northwest move (B southeast move):
	//W northeast move (B southwest move):
	for (int dx = -1; dx < 2; dx += 2)
	{
		crd = coord;
		//adjacent
		crd.i_coord += d*dx; crd.j_coord += d;
		if (isInBoard(crd))
		{
			//if empty slot:
			if (GetContentOfCoord(board, crd) == EMPTY && depth == 0)
			{
				tmpMoves = allMoves;
				allMoves = createRegularMove(coord, crd, depth, 0);
				if (allMoves == NULL)
					return tmpMoves;
				allMoves->next = tmpMoves;
			}
			//if occupied by enemy
			else if (IsEnemy(GetContentOfCoord(board, crd), myType))
			{
				//exCrd <- coordinate beyond crd
				exCrd = crd;
				exCrd.i_coord += d*dx; exCrd.j_coord += d;
				//if there is an empty slot beyond
				if (isInBoard(exCrd) && GetContentOfCoord(board, exCrd) == EMPTY)
				{
					if (anointAKing(exCrd.j_coord, myType)) //anoints the king and stops its turn.
					{
						tmpMoves = allMoves;
						allMoves = createRegularMove(coord, exCrd, depth, 1);
						if (allMoves == NULL)
							return tmpMoves;
						allMoves->next = tmpMoves;
					}
					else //no new king was anointed.
					{
						tmpType = GetContentOfCoord(board, crd); //the tool that is being eaten

						//performing the capture on board
						setSlotInBoard(board, coord, EMPTY);
						setSlotInBoard(board, crd, EMPTY);
						setSlotInBoard(board, exCrd, myType);


						//the list of possible moves that can be done from this position
						recursionMoves = manMoves(board, exCrd, depth + 1);

						//turning the board back to its original configuration before performing the capture.
						setSlotInBoard(board, coord, myType);
						setSlotInBoard(board, crd, tmpType);
						setSlotInBoard(board, exCrd, EMPTY);

						if (recursionMoves == NULL)
						{
							tmpMoves = allMoves;
							allMoves = createRegularMove(coord, exCrd, depth, 1);
							if (allMoves == NULL)
								return tmpMoves;
							allMoves->next = tmpMoves;
						}
						else
						{
							//adding coord to the route of each of the moves in the list recursionMoves
							tmpMoves = moveAfterRec(coord, depth, recursionMoves);
							tmpMoves->next = allMoves;
							allMoves = recursionMoves;
						}
					}
				}
			}
		}
	}

	//W southwest move (B northeast move):
	//W southeast move (B northwest move):
	for (int dx = -1; dx < 2; dx += 2)
	{
		exCrd = coord;
		crd = coord;
		crd.i_coord += d*dx; crd.j_coord -= d;
		exCrd.i_coord += 2 * d*dx; exCrd.j_coord -= 2 * d;

		//(W) if far coord to the southeast is in board
		if (isInBoard(exCrd))
		{
			//and the close one is occupied by enemy:
			if (IsEnemy(GetContentOfCoord(board, crd), myType))
			{
				//if there is an empty slot beyond
				if (GetContentOfCoord(board, exCrd) == EMPTY)
				{

					tmpType = GetContentOfCoord(board, crd);

					setSlotInBoard(board, coord, EMPTY);
					setSlotInBoard(board, crd, EMPTY);
					setSlotInBoard(board, exCrd, myType);

					recursionMoves = manMoves(board, exCrd, depth + 1);

					setSlotInBoard(board, coord, myType);
					setSlotInBoard(board, crd, tmpType);
					setSlotInBoard(board, exCrd, EMPTY);

					if (recursionMoves == NULL)
					{
						tmpMoves = allMoves;
						allMoves = createRegularMove(coord, exCrd, depth, 1);
						if (allMoves == NULL)
							return tmpMoves;
						allMoves->next = tmpMoves;
					}
					else
					{
						tmpMoves = moveAfterRec(coord, depth, recursionMoves);
						tmpMoves->next = allMoves;
						allMoves = recursionMoves;
					}
				}
			}
		}
	}
	tmpMoves = allMoves;
	return tmpMoves;
}

/*CONVD*/
/*returns 0 if tool is an oponnent tool
return 1 if tool is ally man
retrun 3 if tool is ally king
*/
int IsAlly(char tool, char myColor)
{
	if (isupper(myColor)) //player is white
	{
		switch (tool)
		{
		case BLACK_P:
			return 1;
			break;
		case BLACK_B:
			return 3;
			break;
		case BLACK_N:
			return 3;
			break;
		case BLACK_R:
			return 5;
			break;
		case BLACK_Q:
			return 9;
			break;
		case BLACK_K:
			return MAX_SCORE;
			break;
		default:
			return 0;
		}
	}
	if (islower(myColor)) //player is white
	{
		switch (tool)
		{
		case WHITE_P:
			return 1;
			break;
		case WHITE_B:
			return 3;
			break;
		case WHITE_N:
			return 3;
			break;
		case WHITE_R:
			return 5;
			break;
		case WHITE_Q:
			return 9;
			break;
		case WHITE_K:
			return MAX_SCORE;
			break;
		default:
			return 0;
		}
	}
	return 0;
}


Coord offsetCoord(Coord coord, int i_offset, int j_offset)
{
	Coord res = coord;
	coord.i_coord += i_offset;
	coord.j_coord += j_offset;
	return res;
}

/*add a new move in the beginning of the linked list*/
cMove *AddMove(cMove **head, char toolType, Coord src, Coord dst, int eater, int promote)
{
	//empty cell -> no eating
	if (eater == EMPTY)
		eater = 0;
	cMove *newMove = (cMove*)calloc(1, sizeof(cMove));
	newMove->toolType = toolType;
	newMove->src = src;
	newMove->dst = dst;
	newMove->eaten = eater;
	newMove->promote = promote;
	newMove->next = *head;
	*head = newMove;
	return *head;
}


int DoesCrdContainsEnemy(board_t board, Coord coord, char tool)
{
	char c = GetContentOfCoord(board, coord);
	if (c == EMPTY)
		return 0;
	if (islower(tool))
	{
		if (iswlower(c))
			return 0;
		else
			return 1;
	}
	else
	{
		if (islower(c))
			return 1;
		else
			return 0;
	}

}

int getColor(board_t board, Coord coord)
{
	int color = islower(GetContentOfCoord(board,coord)) ? WHITE_PLAYER : BLACK_PLAYER;
	return color;
}

cMove* PawnMoves(board_t board, Coord coord)
{
	cMove *head = NULL;
	int i = coord.i_coord, j = coord.j_coord;
	char tool = GetContentOfCoord(board, coord);
	int color = islower(tool) ? 1 : -1;
	Coord tmpCoord = coord;

	//no eating
	tmpCoord.j_coord += color * 1;
	if (isInBoard(tmpCoord) == 1 && !isKingUnderThreat(board, coord, tmpCoord) && GetContentOfCoord(board, tmpCoord) == EMPTY)
	{
		if ((j + color * 1 == BOARD_SIZE-1 && color == 1) || (j + color * 1 == 0 && color == -1)) //if promotion needed
		{
			AddMove(&head,tool, coord, tmpCoord, 0, color == 1 ? 'r' : 'R');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'b' : 'B');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'q' : 'Q');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'n' : 'N');
		}
		else
			AddMove(&head, tool, coord, tmpCoord, 0, 0);// if promotion isn't needed
	}


	//for black:south-east eating, for white:north-west eating
	tmpCoord = coord;
	tmpCoord.i_coord -= color * 1;
	tmpCoord.j_coord += color * 1;
	if (isInBoard(tmpCoord) == 1 && !isKingUnderThreat(board, coord, tmpCoord) && DoesCrdContainsEnemy(board, coord, tool))
	{
		if ((j + color * 1 == BOARD_SIZE - 1 && color == 1) || (j + color * 1 == 0 && color == -1)) //if promotion needed
		{
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'r' : 'R');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'b' : 'B');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'q' : 'Q');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'n' : 'N');
		}
		else
			AddMove(&head, tool, coord, tmpCoord, 0, 0);// if promotion isn't needed
	}


	//for black:south-west eating, for white:north-east eating
	tmpCoord = coord;
	tmpCoord.i_coord += color * 1;
	tmpCoord.j_coord += color * 1;
	if (isInBoard(tmpCoord) == 1 && !isKingUnderThreat(board, coord, tmpCoord)  && DoesCrdContainsEnemy(board, coord, tool))
	{
		if ((j + color * 1 == BOARD_SIZE-1 && color == 1) || (j + color * 1 == 0 && color == -1)) //if promotion needed
		{
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'r' : 'R');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'b' : 'B');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'q' : 'Q');
			AddMove(&head, tool, coord, tmpCoord, 0, color == 1 ? 'n' : 'N');
		}
		else
			AddMove(&head, tool, coord, tmpCoord, 0, 0);// if promotion isn't needed
	}
}

void BishopMoves(board_t board, Coord coord){
	cMove *head = NULL;
	int i = coord.i_coord, j = coord.j_coord;
	char tool = GetContentOfCoord(board, coord);
	int color = islower(tool) ? 1 : -1;


	
	//north-east
	Coord tmpCoord = offsetCoord(coord, k, k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord),0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating

	//north-west
	tmpCoord = offsetCoord(coord, -k, k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating



	//south-east
	tmpCoord = offsetCoord(coord, k, -k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating

	//south-west
	tmpCoord = offsetCoord(coord, -k, -k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
void RookMoves(board_t board, Coord coord){
	cMove *head = NULL;
	int i = coord.i_coord, j = coord.j_coord;
	char tool = GetContentOfCoord(board, coord);
	int color = islower(tool) ? 1 : -1;


	//north
	Coord tmpCoord = offsetCoord(coord, k, k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating

	//south
	tmpCoord = offsetCoord(coord, -k, k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating



	//east
	tmpCoord = offsetCoord(coord, k, -k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating

	//west
	tmpCoord = offsetCoord(coord, -k, -k);
	while (isInBoard(tmpCoord)) {
		if (getColor(&head, tmpCoord) == color * -1){
			AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
		} if (GetContentOfCoord == EMPTY){
			AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating






void KnightMoves(board_t board, Coord coord)
{
	cMove *head = NULL;
	
	int color = getColor(board, coord);
	char tool = color == WHITE_PLAYER ? WHITE_N : BLACK_N;

	Coord tmpCrd = coord, 
		tmp0 = offsetCoord(coord, 1, 2),
		tmp1 = offsetCoord(coord, 2, 1),
		tmp2 = offsetCoord(coord, 2, -1),
		tmp3 = offsetCoord(coord, 1, -2),
		tmp4 = offsetCoord(coord, -1, -2),
		tmp5 = offsetCoord(coord, -2, -1),
		tmp6 = offsetCoord(coord, -2, 1),
		tmp7 = offsetCoord(coord, -1, 2),
		possibilities[8] =
	{	 
		tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7
	};
		
	for (int i = 0; i < 8; i++)
	{
		if (isInBoard(possibilities[i]) && 
			(GetContentOfCoord(board, possibilities[i]) == EMPTY || DoesCrdContainsEnemy(board, possibilities[i], tool)))
		{
			AddMove(&head, tool, coord, possibilities[i], GetContentOfCoord(board, possibilities[i]) , 0);
		}
	}
}

void KingMoves(board_t board, Coord coord)
{
	cMove *head = NULL;

	int color = getColor(board, coord);
	char tool = color == WHITE_PLAYER ? WHITE_N : BLACK_N;

	Coord tmpCrd = coord, 
		tmp0 = offsetCoord(coord, 0, 1),
		tmp1 = offsetCoord(coord, 1, 1),
		tmp2 = offsetCoord(coord, 1, 0),
		tmp3 = offsetCoord(coord, 1, -1),
		tmp4 = offsetCoord(coord, 0, -1),
		tmp5 = offsetCoord(coord, -1, -1),
		tmp6 = offsetCoord(coord, -1, 0),
		tmp7 = offsetCoord(coord, -1, 1),
		possibilities[8] = { tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7	};

	for (int i = 0; i < 8; i++)
	{
		if (isInBoard(possibilities[i]) &&
			(GetContentOfCoord(board, possibilities[i]) == EMPTY || DoesCrdContainsEnemy(board, possibilities[i], tool)))
		{
			AddMove(&head, tool, coord, possibilities[i], GetContentOfCoord(board, possibilities[i]) != EMPTY, 0);
		}
	}
}


/*returns all moves that a player may do with the specific configuration of the board (filtered).*/
cMove *getMoves(board_t board, int player)
{
	Move *allMoves = NULL, *tmpMoves, *tmpMovesEnd;
	Coord coord;
	int ally;
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE && properties[1] != 1; k++)
	{
		coord.i_coord = (int)mod(k, BOARD_SIZE);
		coord.j_coord = k / BOARD_SIZE; /*(int)floor(k / BOARD_SIZE);*/
		ally = IsAlly(GetContentOfCoord(board, coord), player);
		if (ally)
			/*it is an ally:*/
		{
			tmpMoves = movesByPieceType(board, coord);

			tmpMovesEnd = tmpMoves;
			if (tmpMovesEnd != NULL)
			{
				while (tmpMovesEnd->next != NULL)
				{
					tmpMovesEnd = tmpMovesEnd->next;
				}
				tmpMovesEnd->next = allMoves;
				allMoves = tmpMoves;
			}
		}
		ally = 0;
	}
	/*if (DEBUG)
	{
	printf("\nbefore filtering moves list was:\n");
	printMovesList(allMoves);
	}*/

	allMoves = (cMove *)filterOutMoves(allMoves);

	//if (DEBUG)
	//{
	//	//printf("\nafter filtering moves list was:\n");
	//	printMovesList(allMoves);
	//	printf("\n");
	//}

	return allMoves;
}



/*filters out of the linked list of moves  which are illegal because
there are other moves that may eat more enemy discs*/
Move *filterOutMoves(Move *moves)
{
	Move *start = moves, *include = NULL, *exclude = NULL, *next;
	int maxLen = 0;
	int filterNonEaters = 0;
	/*check whether filtering is required, and the maximal length of any move.*/
	while (moves != NULL)
	{
		if (moves->eater == 1){ filterNonEaters = 1; }
		if (moves->len > maxLen){ maxLen = moves->len; }
		moves = moves->next;
	}
	moves = start;
	if (filterNonEaters == 1)
		/*filtering itself: */
	{
		while (moves != NULL)
		{
			next = moves->next; /*at this point moves cannot be null*/
			if (moves->eater == 0 || moves->len < maxLen)
			{
				moves->next = exclude;
				exclude = moves;
			}
			else
			{
				moves->next = include;
				include = moves;
			}
			moves = next;
		}
		while (exclude != NULL)
		{
			next = exclude->next;
			free(exclude->route);
			free(exclude);
			exclude = next;
		}
		return include;
	}
	return moves;
}

/* UNUSED */
int isEaterMove(Move *move)
{
	if (move->len > 1) { return 1; }
	if (abs(move->route->i_coord - ((move->route) + 1)->i_coord) == 2){ return 1; }
	return 0;
}

/* Sets the slot on the board, pay attention: there is a similar function with different arguments. */
void setSlot(board_t board, char *horiz, char *vert, char type)
{
	Coord coord;
	int i = (int)*horiz - 97;
	int j = (int)strtof(vert, NULL) - 1;
	coord.i_coord = i; coord.j_coord = j;
	if (isTool(type) || type == EMPTY)
	{

		if (isInBoard(coord) && mod(i + j, 2) == 0)
		{
			if (NotTooManyOfType(board, type))
			{
				board[i][j] = type;
				if (isWhite(type) == 1 && coord.j_coord == BOARD_SIZE - 1)
					board[i][j] = WHITE_K;
				if (isWhite(type) == 0 && type != EMPTY && coord.j_coord == 0)
					board[i][j] = BLACK_K;
			}
			else
			{
				printf(NO_PIECE);
			}
		}

		else
		{
			if (!properties[1])
				printf(WRONG_POSITION);
		}

	}
}


/*DCLR*/
/*return the number of tools of the given type that appear on the board*/
int CountToolsOfType(board_t board, char type)
{
	int counter = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	for (int j = 0; j < BOARD_SIZE; j++)
	if (board[i][j] == type)
		counter++;
	return counter;
}

/*DCLR*/
/*return 1 if the number of tools of the given type doesn't exceed the legal number and 0 otherwise*/
int NotTooManyOfType(board_t board, char type)
{
	int result = 0;
	int numberOfTools = CountToolsOfType(board, type);
	switch (type)
	{
	case WHITE_P:
	case BLACK_P:
		if (numberOfTools < 8)
			result = 1;
		break;
	case WHITE_B:
	case BLACK_B:
	case WHITE_N:
	case BLACK_N:
	case WHITE_R:
	case BLACK_R:
		if (numberOfTools < 2)
			result = 1;
		break;
	case WHITE_Q:
	case BLACK_Q:
	case WHITE_K:
	case BLACK_K:
		if (numberOfTools < 1)
			result = 1;
		break;

	default:
		break;
	}
	return result;
}


/* DEBUG
sets a given board by a given configuration represented by a string.*/
void setBoard(board_t board, char *config)
{
	char *horiz, *vert, *type;
	int first = 1;
	while (config != NULL)
	{
		horiz = (first == 1) ? strtok(config, ",") : strtok(NULL, ",");
		first = 0;
		vert = strtok(NULL, ",");
		type = strtok(NULL, " ");
		if (horiz != NULL && vert != NULL && type != NULL)
		{
			setSlot(board, horiz, vert, *type);
		}
		config = type;
	}
}


/*int score(board_t board, char player)
TODO: win/stuck/lost implement inside
{
Coord coord;
int score = 0, ally, enemy, playerExists = 0, opponentExists = 0;
for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
{
coord.i_coord = (int)mod(k, BOARD_SIZE);
coord.j_coord = k / BOARD_SIZE;
ally = IsAlly(GetContentOfCoord(board, coord), player);
enemy = IsEnemy(GetContentOfCoord(board, coord), player);
score += (ally - enemy);
if (!playerExists && ally > 0){ playerExists = 1; }
if (!opponentExists && enemy > 0){ opponentExists = 1; }
}
if (!opponentExists && playerExists){ return 100; }
if (!playerExists && opponentExists){ return -100; }
return score;
}*/

/* an O(1) function to see if a disc is movable or stuck.
returns 1 if movable and 0 otherwise.*/
int canMoveThisTool(board_t board, Coord coord)
{
	Coord crd1, crd2;
	crd1 = coord;
	char tool = GetContentOfCoord(board, coord);
	//if there is any empty adjacent diagonal cell or enemy to eat
	for (int i = -1; i < 2; i += 2)
	{
		for (int j = -1; j < 2; j += 2)
		{

			crd1 = coord;
			crd2 = coord;
			crd1.i_coord += i; crd1.j_coord += j;
			crd2.i_coord += 2 * i; crd2.j_coord += 2 * j;
			if (tool == WHITE_K || tool == BLACK_K || (tool == WHITE_M && crd1.j_coord > coord.j_coord) || (tool == BLACK_M && crd1.j_coord < coord.j_coord))
			{
				if (isInBoard(crd1) && GetContentOfCoord(board, crd1) == EMPTY)//if there is any empty adjacent diagonal cell
					return 1;
			}
			if (isInBoard(crd2) && GetContentOfCoord(board, crd2) == EMPTY)//if there is any empty adjacent diagonal cell or enemy to eat
			{
				if (
					((tool == WHITE_K && (isWhite(GetContentOfCoord(board, crd1)) == 0)))
					|| ((tool == BLACK_K && (isWhite(GetContentOfCoord(board, crd1)) == 1)))
					|| ((tool == WHITE_M && (isWhite(GetContentOfCoord(board, crd1)) == 0) && GetContentOfCoord(board, crd1) != EMPTY))
					|| ((tool == BLACK_M && isWhite(GetContentOfCoord(board, crd1)) == 1) && GetContentOfCoord(board, crd1) != EMPTY))

					return 1;
			}
		}
	}


	return 0;

}

/* the scoring function:
See instructions for more details.*/
int score(board_t board, char player)

{
	Coord coord;
	int score = 0, ally, enemy, playerExists = 0, opponentExists = 0, playerBlocked = 1, opponentBlocked = 1;
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
	{
		coord.i_coord = (int)mod(k, BOARD_SIZE);
		coord.j_coord = k / BOARD_SIZE;
		ally = IsAlly(GetContentOfCoord(board, coord), player);
		enemy = IsEnemy(GetContentOfCoord(board, coord), player);
		score += (ally - enemy);
		if (GetContentOfCoord(board, coord) != EMPTY)
		{
			if (ally > 0)
				playerExists = 1;
			if (enemy > 0)
				opponentExists = 1;
			if (ally > 0 && canMoveThisTool(board, coord) == 1)
				playerBlocked = 0;
			if (enemy > 0 && canMoveThisTool(board, coord) == 1)
				opponentBlocked = 0;
		}

	}
	if (!opponentExists && playerExists)
		return 100;
	if (!playerExists && opponentExists)
		return -100;
	if (playerBlocked == 1)
		return -100;
	if (opponentBlocked == 1)
		return 100;

	return score;
}

/*returns 1 if type is white and 0 otherwise.*/
int isWhite(char type)
{
	if (type == WHITE_R || type == WHITE_N || type == WHITE_B || type == WHITE_K || type == WHITE_Q || type == WHITE_P)
		return 1;
	return 0;
}

/*int minimax_score(board_t board, char player, int depth, int minOrMax)
{
Move *movesList, *tmp;
int scr = 101;
if (depth == 0)
return score(board, player);
else
{
movesList = getMoves(board, player);
tmp = movesList;
if (movesList != NULL)
{
do
{
scr = (scr == 101) ? makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax) :
((minOrMax) ? imax(scr, makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax)) :
imin(scr, makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax)));

movesList = movesList->next;
free(tmp->route);
free(tmp);
tmp = movesList;

} while (movesList != NULL);
return scr;

}
else
{
return score(board, player);
}
}
}*/

/*if player is any white tool return black man. else, return white man*/
char getEnemy(char player)
{
	return (player == WHITE_K || player == WHITE_M) ? BLACK_M : WHITE_M;
}

/* the minimax function as instructed.
pay attention: this function only returns integers. the argument bestMove
is an address to a pointer that exists outside of minimax_score, and when obtaining the information about
which move is truely optimal (due to depth restrictions) the content of the address is set to the address of that said move. */
int minimax_score(board_t board, char player, int depth, int minOrMax, Move **bestMove)
{
	Move *movesList, *tmp;
	int bestValue, val;
	if (depth == 0)//base case
	{
		if (minOrMax == 1)
			return score(board, player);
		else
			return score(board, getEnemy(player));
	}
	if (minOrMax == 1)
	{
		bestValue = -101;
		movesList = getMoves(board, player);
		tmp = movesList;


		while (movesList != NULL)
		{
			int bestUpdated = 0;
			//compute score and return board to its original state
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax);
			if (depth == properties[2] && val >= bestValue) //if depth == minimax_depth
			{
				if (*bestMove != NULL)
				{
					free((*bestMove)->route);
					free(*bestMove);
				}
				*bestMove = movesList;
				bestUpdated = 1;
			}
			bestValue = imax(bestValue, val);
			movesList = movesList->next;
			if (bestUpdated == 0)
			{
				free(tmp->route);
				free(tmp);
			}
			tmp = movesList;
		}
		return bestValue;
	}
	else
	{
		bestValue = 101;
		movesList = getMoves(board, player);
		tmp = movesList;

		while (movesList != NULL)
		{
			int bestUpdated = 0;
			//compute score and return board to its original state
			val = makeMove_ComputeScore_Undo(board, movesList, player, depth, minOrMax);
			if (depth == properties[2] && val <= bestValue) //if depth == minimax_depth
			{
				if (*bestMove != NULL)
				{
					free((*bestMove)->route);
					free(*bestMove);
				}
				*bestMove = movesList;
				bestUpdated = 1;
			}
			bestValue = imin(bestValue, val);
			movesList = movesList->next;
			if (bestUpdated == 0)
			{
				free(tmp->route);
				free(tmp);
			}
			tmp = movesList;



		}
		return bestValue;
	}
}

/*Called from within the minimax_score func. make reversable alterations on the board: see description inside.*/
int makeMove_ComputeScore_Undo(board_t board, Move *move, char player, int depth, int minOrMax)
/*Saves all the eaten tool types in the eatenTools array, all while making the move on the given board.
Then, run minimax algorithm in recursion to produce a score. At last, undo the move, using the array, and return the score conputed.*/
{
	char eatentools_types[MAX_NUMBER_OF_TOOLS_PER_PLAYER];
	Coord eatentools_coords[MAX_NUMBER_OF_TOOLS_PER_PLAYER];
	int scr;
	/*justification: according to the rules, at most as 20 tools may be eaten in one move,
	as over 20 pieces for a player is unallowed.*/
	char myColor = GetContentOfCoord(board, move->route[0]);

	/*1)	setting the board as after the move, while saving the eaten tools type and location.*/
	Coord crd = move->route[0];
	setSlotInBoard(board, crd, EMPTY);

	if (move->eater)
	{
		for (int i = 1; i <= move->len; i++)
		{
			int dx = (move->route[i].i_coord > move->route[i - 1].i_coord) ? 1 : -1;
			int dy = (move->route[i].j_coord > move->route[i - 1].j_coord) ? 1 : -1;

			do
			{
				crd.i_coord += dx; crd.j_coord += dy;
			} while (!isTool(GetContentOfCoord(board, crd)));

			eatentools_types[i - 1] = GetContentOfCoord(board, crd);
			eatentools_coords[i - 1] = crd; //assignment between structs
			setSlotInBoard(board, crd, EMPTY);
			crd.i_coord += dx; crd.j_coord += dy;
		}


	}

	setSlotInBoard(board, move->route[move->len], myColor);

	scr = minimax_score(board, getEnemy(player), depth - 1, 1 - minOrMax, NULL);

	//reconstruct the board as before

	setSlotInBoard(board, move->route[move->len], EMPTY);
	setSlotInBoard(board, move->route[0], myColor);
	if (move->eater)
	{
		for (int i = 0; i < move->len; i++)
		{
			setSlotInBoard(board, eatentools_coords[i], eatentools_types[i]);
		}
	}
	return scr;

}

/*changes the board so it describes the state created after the move is made.*/
int makeMove(board_t board, Move *move)
{
	char myColor = GetContentOfCoord(board, move->route[0]);
	Coord crd = move->route[0];
	setSlotInBoard(board, crd, EMPTY);

	if (move->eater)
	{

		for (int i = 1; i <= move->len; i++)
		{
			int dx = (move->route[i].i_coord > move->route[i - 1].i_coord) ? 1 : -1;
			int dy = (move->route[i].j_coord > move->route[i - 1].j_coord) ? 1 : -1;

			do
			{
				crd.i_coord += dx; crd.j_coord += dy;
			} while (!isTool(GetContentOfCoord(board, crd)));

			setSlotInBoard(board, crd, EMPTY);
			crd.i_coord += dx; crd.j_coord += dy;
		}
	}
	setSlotInBoard(board, move->route[move->len], myColor);
	return 0;


}

/* returns a if a is greater than b, and b otherwise*/
int imax(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

/*returns a if a is smaller than b, and b otherwise*/
int imin(int a, int b)
{
	if (a < b)
		return a;
	return b;
}


/*DEBUG*/
long random_at_most(long max) {
	// Assumes 0 <= max <= RAND_MAX
	// Returns in the half-open interval [0, max]
	return rand() % max;
	/*unsigned long
	// max <= RAND_MAX < ULONG_MAX, so this is okay.
	num_bins = (unsigned long)max + 1,
	num_rand = (unsigned long)RAND_MAX + 1,
	bin_size = num_rand / num_bins,
	defect = num_rand % num_bins;

	long x;
	do {
	x = rand();
	}
	// This is carefully written not to overflow
	while (num_rand - defect <= (unsigned long)x);

	// Truncated division is intentional
	return x / bin_size;*/
}

/*DEBUG: automatic random configuration generator*/
board_t GenerateRandomConfiguration()
{
	//set random number of tools per player
	int numberOfWhiteTools = random_at_most(MAX_NUMBER_OF_TOOLS_PER_PLAYER - 1) + 1, numberOfBlackTools = random_at_most(MAX_NUMBER_OF_TOOLS_PER_PLAYER - 1) + 1;
	int numberOfTooleToSet = numberOfWhiteTools + numberOfBlackTools;
	int places[(BOARD_SIZE*BOARD_SIZE + 1) / 2];
	int numberOfFreePlaces = (BOARD_SIZE*BOARD_SIZE + 1) / 2;
	int index = 0;
	board_t brd = NULL;
	brd = createBoard();
	//initialize places array
	for (int i = 0; i < BOARD_SIZE; i++)
	for (int j = 0; j < BOARD_SIZE; j++)
	{
		if ((i + j) % 2 == 0)
		{
			places[index] = i * 10 + j;
			index++;
		}
	}
	assert(index == numberOfFreePlaces);

	//set white tools:
	for (int i = 0; i < numberOfWhiteTools; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		//printf("%u\n", x);
		//printf("%u\n", y);
		//printf("%u\n\n\n", places[place]);
		Coord crd;
		int toolRand = random_at_most(5);
		char tool = (toolRand == 0) ? WHITE_K : WHITE_M;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;

	}
	//set black tools:
	for (int i = 0; i < numberOfBlackTools; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		int toolRand = random_at_most(5);
		char tool = (toolRand == 0) ? BLACK_K : BLACK_M;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}
	assert(numberOfTooleToSet == 0);
	return brd;
}


/*DEBUG: configurations generator.*/
char *configuration(int num)
{
	char *res;
	const char *config1 = "c,3,k c,5,M e,5,M e,7,K g,9,m i,9,k";
	const char *config2 = "c,3,m d,4,M d,6,K";
	const char *config3 = "b,4,M c,5,m e,7,k g,9,m i,9,m";
	const char *config4 = "b,4,m c,5,M e,7,K g,9,M i,9,M";
	const char *config7 = "b,4,m";//no black tools
	const char *config8 = "c,1,M";//no white tools + check the auto-anoint of a man to a king in the stage of settings
	//const char *config9 = "a,1,K b,2,m c,3,m";//black is blocked
	//const char *config9 = "a,9,M b,8,k c,7,m";//black is blocked
	//const char *config9 = "a,3,m b,4,K c,5,M";//white is blocked
	//const char *config9 = "a,1,K b,2,M c,1,m";//black is blocked
	const char *config9 = "a,3,M b,2,k c,1,m c,5,m";//black is blocked
	const char *config10 = "a,1,m b,2,K c,3,K";//white black tools
	//const char *config11 = "c,3,K a,3,m";//exception - problem!!!
	const char *config11 = "a,1,K b,2,m";//exception - problem!!!
	const char *filtering1 = "a,1,m b,2,M b,4,M d,4,K d,6,M e,7,m";
	const char *checkKing = "b,2,k f,6,M";

	if (num == 1)
	{
		res = calloc(strlen(config1) + 1, 1);
		strcpy(res, config1);
	}

	if (num == 2)
	{
		res = calloc(strlen(config2) + 1, 1);
		strcpy(res, config2);
	}
	if (num == 3)
	{
		res = calloc(strlen(config3) + 1, 1);
		strcpy(res, config3);
	}
	if (num == 4)
	{
		res = calloc(strlen(config4) + 1, 1);
		strcpy(res, config4);
	}
	if (num == 5)
	{
		res = calloc(strlen(filtering1) + 1, 1);
		strcpy(res, filtering1);
	}
	if (num == 6)
	{
		res = calloc(strlen(checkKing) + 1, 1);
		strcpy(res, checkKing);
	}
	if (num == 7)
	{
		res = calloc(strlen(config7) + 1, 1);
		strcpy(res, config7);
	}
	if (num == 8)
	{
		res = calloc(strlen(config8) + 1, 1);
		strcpy(res, config8);
	}
	if (num == 9)
	{
		res = calloc(strlen(config9) + 1, 1);
		strcpy(res, config9);
	}
	if (num == 10)
	{
		res = calloc(strlen(config10) + 1, 1);
		strcpy(res, config10);
	}
	if (num == 11)
	{
		res = calloc(strlen(config11) + 1, 1);
		strcpy(res, config11);
	}
	return res;
}

/*DEBUG*/
board_t unitTest_movements(int i)
{
	board_t brd = NULL;
	char *config;
	brd = createBoard();
	config = configuration(i);
	setBoard(brd, config);
	print_board(brd);

	Move *moves;
	printf("\nThe score for this board is: %d for W player and %d for B player.\n\n", score(brd, WHITE_K), score(brd, BLACK_K));
	printf("moves for white are:\n");
	moves = getMoves(brd, WHITE_K);
	printMovesList(moves);

	printf("\nmoves for black are:\n");
	moves = getMoves(brd, BLACK_K);
	printMovesList(moves);
	return brd;
}

/*Parses the input inserted by the user and operates accordingly.
returns ints for convenience. specifically 10 is an important returned value: see inside.*/
int Parse(char *line, board_t board)
{
	/*
	properties[0] = Setting state
	properties[1] = quit
	properties[2] = Default minimax depth
	properties[3] = User color: 0 for default white, 1 for black
	properties[4] = It is now white's\black's turn: 0 for white, 1 for black
	properties[5] = game mode


	*/

	const char *cmmd1 = "game_mode";
	const char *cmmd2 = "difficulty";
	const char *cmmd3 = "user_color";
	const char *cmmd4 = "load";
	const char *cmmd5 = "clear";
	const char *cmmd6 = "next_player";
	const char *cmmd7 = "rm";
	const char *cmmd8 = "set";
	const char *cmmd9 = "print";
	const char *cmmd10 = "quit";
	const char *cmmd11 = "start";
	const char *cmmd12 = "move";
	const char *cmmd13 = "get_moves";
	const char *cmmd14 = "get_best_moves";
	const char *cmmd15 = "get_score";
	const char *cmmd16 = "save";

	const char *white = "white";
	const char *black = "black";
	//const char *man = "m";
	//const char *king = "k";


	if (properties[0])
		/*Setting State*/
	{
		char *token = strtok(line, " \n");

		if (strcmp(token, cmmd1) == 0)
			//game mode
		{
			token = strtok(NULL, " ");
			long mode = strtol(token, &token, BOARD_SIZE);
			if (mode == 1 || mode == 2)
			{
				properties[5] = (int)mode;
				if (mode == 1)
					prinft("%s", TWO_PLAYERS_GAME_MODE);
				else
					prinft("%s", PLAYER_VS_AI_GAME_MODE);
			}
			else
			{
				if (!properties[1])
				{
					printf(WRONG_GAME_MODE);
					properties[5] = 1;
				}
			}

			/*if (DEBUG)
			printf("\nUpon updading minimax_depth, it is now %d\n", properties[2]);*/
			return 0;
		}


		else if (strcmp(token, cmmd2) == 0)
			//difficulty
		{
			if (properties[5] == 1)
			{
				printf("%s", ILLEGAL_COMMAND);
				return 0;
			}


			token = strtok(NULL, " ");
			if (strcmp(token, BEST))
			{
				properties[2] = BESTval;//5 stands for best
				return 0;
			}
			long depth = strtol(token, &token, BOARD_SIZE);
			if (depth > 0 && depth < 7)
				properties[2] = (int)depth;
			else
			{
				if (!properties[1])
					printf(WRONG_MINIMAX_DEPTH);
			}

			/*if (DEBUG)
			printf("\nUpon updading minimax_depth, it is now %d\n", properties[2]);*/
			return 0;
		}


		else if (strcmp(token, cmmd3) == 0)
			//player color
		{
			if (properties[5] == 1)
			{
				printf("%s", ILLEGAL_COMMAND);
				return 0;
			}
			token = strtok(NULL, " \n");
			if (strcmp(token, white) == 0)
				properties[3] = 0;
			else if (strcmp(token, black) == 0)
				properties[3] = 1;
			return 0;
		}

		else if (strcmp(token, cmmd4) == 0)
			//load
		{
			token = strtok(NULL, " \n");
			LoadFromFile(token, board);
			return 0;
		}


		else if (strcmp(token, cmmd5) == 0)
			//clear
		{
			clearBoard(board);
			return 0;
		}

		else if (strcmp(token, cmmd6) == 0)
			//next player
		{
			token = strtok(NULL, " \n");
			if (strcmp(token, black) == 0)
				properties[4] = 1;
			else
				properties[4] = 0;

			/*if (DEBUG)
			printf("\nUpon updating next player, it is now %d\n", properties[4]);*/
			return 0;
		}


		else if (strcmp(token, cmmd7) == 0)
			//remove
		{
			char *horiz, *vert;
			token = strtok(NULL, "<,");
			horiz = token;
			token = strtok(NULL, ",>");
			vert = token;
			setSlot(board, horiz, vert, EMPTY);
			return 0;
		}


		else if (strcmp(token, cmmd8) == 0)
			//set
		{
			char *horiz, *vert, *color, *type, tool;

			token = strtok(NULL, "<,");
			horiz = token;

			token = strtok(NULL, ",>");
			vert = token;

			token = strtok(NULL, " ");
			color = token;

			token = strtok(NULL, " \n");
			type = token;


			if (strcmp(color, white) == 0)
			{
				if (strcmp(type, KING) == 0)
					tool = WHITE_K;
				else if (strcmp(type, QUEEN) == 0)
					tool = WHITE_Q;
				else if (strcmp(type, ROOK) == 0)
					tool = WHITE_R;
				else if (strcmp(type, KNIGHT) == 0)
					tool = WHITE_N;
				else if (strcmp(type, BISHOP) == 0)
					tool = WHITE_B;
				else if (strcmp(type, PAWN) == 0)
					tool = WHITE_P;
				else
				{
					{
						if (!properties[1])
						printf(ILLEGAL_COMMAND);
						return 0;
					}
				}
			}
			else if (strcmp(color, black) == 0)
			{
				if (strcmp(type, KING) == 0)
					tool = BLACK_K;
				else if (strcmp(type, QUEEN) == 0)
					tool = BLACK_Q;
				else if (strcmp(type, ROOK) == 0)
					tool = BLACK_R;
				else if (strcmp(type, KNIGHT) == 0)
					tool = BLACK_N;
				else if (strcmp(type, BISHOP) == 0)
					tool = BLACK_B;
				else if (strcmp(type, PAWN) == 0)
					tool = BLACK_P;
				else
				{
					if (!properties[1])
						printf(ILLEGAL_COMMAND);
					return 0;
				}
			}
			else
			{
				if (!properties[1])
					printf(ILLEGAL_COMMAND);
				return 0;
			}

			setSlot(board, horiz, vert, tool);
			return 0;
		}



		else if (strcmp(token, cmmd9) == 0)
			//print
		{
			print_board(board);
			return 0;
		}

		else if (strcmp(token, cmmd10) == 0)
			//quit
		{
			properties[1] = 1;
			return 0;
		}
		else if (strcmp(token, cmmd11) == 0)
			//start
		{
			if (CountToolsOfType(board, WHITE_K)<1 || CountToolsOfType(board, BLACK_K)<1)
			{
				if (!properties[1])
					printf(WROND_BOARD_INITIALIZATION);
				return 0;
			}
			else
			{
				properties[0] = 0;//change from setting state to game state
				return 0;
			}
		}
		if (!properties[1])
			printf(ILLEGAL_COMMAND);
		return 0;

	}



	else
		/*Game State*/
	{
		char *token = strtok(line, " \n");
		if (strcmp(token, cmmd7) == 0)
			//quit
		{
			properties[1] = 1;
			return 0;
		}

		else if (strcmp(token, cmmd12) == 0)
			//move
		{
			token = strtok(NULL, "&");
			Move *legalMove;
			if (!areAllCoordsValid(token))//if either one of the positions in the command is invalid
			{
				if (!properties[1])
					printf(WRONG_POSITION);
				return 1;
			}
			if (!isLegalInitialPosition(token, board))//if position <x, y> does not contain a disc of the user's color
			{
				if (!properties[1])
					printf(NOT_YOUR_PIECE);
				return 1;
			}
			legalMove = (Move *)isLegalMove(token, board, pMove);
			if (legalMove == NULL)//input move is illegal
			{
				if (!properties[1])
					printf(ILLEGAL_MOVE);
				return 1;
			}

			makeMove(board, legalMove);
			print_board(board);
			return 10;//successful move was made
		}
		else if (strcmp(token, cmmd10) == 0)
			//get_moves
		{
			printMovesList(pMove);
			return 0;
		}
	}
	if (!properties[1])
		printf(ILLEGAL_COMMAND);
	return 0;

}


/*DCLR*/
char ToolNameToChar(char *toolFullName)
{
	char toolFirstChar = *toolFullName;
	switch (toolFirstChar)
	{
	case 'b':
		return 'b';
	case 'k':
		return 'n';
	case 'r':
		return 'r';
	case 'q':
		return 'q';
	default:
		break;
	}
	return '\0';

}

/*return NULL if the move is illegal, and a pointer to the move otherwise*/
Move* isLegalMove(char *token, board_t board, Move *allPossibleMoves)
{
	char *currentPosition = token;
	char x_coor, i_coor;
	int y_coor, j_coor;
	int promoteNeeded = 0;
	char promoteTo = '\0';
	char tool;
	Coord source, dest;


	//fill coordinates
	x_coor = *(currentPosition + 1);
	currentPosition += 3;
	y_coor = (int)strtol(currentPosition, &currentPosition, 10);
	graphicCoordToRealCoord(&source, x_coor, y_coor);
	currentPosition += 6;
	i_coor = *currentPosition;
	currentPosition += 3;
	j_coor = (int)strtol(currentPosition, &currentPosition, 10);
	graphicCoordToRealCoord(&dest, i_coor, j_coor);

	//check if promotion needed
	if (*(currentPosition + 1) != '\n') //if  promotion needed
	{
		currentPosition += 2;
		promoteTo = ToolNameToChar(currentPosition);
		if (isupper(GetContentOfCoord(board, source)))
			promoteTo = toupper(promoteTo);
	}





	char *currentPosition = token;
	char x_coor;
	int y_coor;
	int numOfCoords = 0;
	int index = 0;
	int difference = 0;
	Coord route[MAX_NUMBER_OF_TOOLS_PER_PLAYER + 1];


	//counte number of coordinates
	while (currentPosition[0] != '\n')
	{
		if (currentPosition[0] == '<')
		{
			numOfCoords++;
		}
		currentPosition++;
	}
	if (numOfCoords > MAX_NUMBER_OF_TOOLS_PER_PLAYER + 1)
		return NULL;


	//fill route with coordinates:
	currentPosition = token;
	Coord tmpCrd;
	while (*currentPosition != '\n')
	{
		if (*currentPosition == '<')
		{
			x_coor = *(currentPosition + 1);
			currentPosition += 3;
			y_coor = (int)strtol(currentPosition, &currentPosition, 10);
			graphicCoordToRealCoord(&tmpCrd, x_coor, y_coor);
			route[index].i_coord = tmpCrd.i_coord; route[index].j_coord = tmpCrd.j_coord;
			index++;

		}
		else
			currentPosition++;
	}
	index--;


	//check if the route equals one of the routes of possible moves:
	while (allPossibleMoves != NULL)
	{
		difference = 0;
		if (allPossibleMoves->len != index)
		{
			allPossibleMoves = allPossibleMoves->next;
			continue;
		}
		for (int i = 0; i <= index; i++)
		if (route[i].i_coord != allPossibleMoves->route[i].i_coord ||
			route[i].j_coord != allPossibleMoves->route[i].j_coord)
		{
			difference = 1;
			break;
		}
		if (difference == 0)//if a legal move has been found
			return allPossibleMoves;
		allPossibleMoves = allPossibleMoves->next;
	}
	return NULL;

}

/*CONVED*/
/*token is a part of the command the used inserted and should represent all valid coordinations.
returns 1 if truly all coords are valid and 0 otherwise.*/
int areAllCoordsValid(char *token)
{
	char *currentPosition = token;
	char x_coor;
	int y_coor;
	Coord crd;

	while (currentPosition[0] != '\n')
	{
		if (currentPosition[0] == '<')
		{
			x_coor = currentPosition[1];
			currentPosition += 3;
			y_coor = (int)strtol(currentPosition, &currentPosition, 10);
			graphicCoordToRealCoord(&crd, x_coor, y_coor);
			if (!isInBoard(crd))
				return 0;
		}
		else
			currentPosition++;
	}
	return 1;
}


/*return 1 if the player has a disc in the intial position, else 0*/
int isLegalInitialPosition(char *token, board_t board)
{
	char *currentPosition = token;
	char x_coor;
	int y_coor;
	Coord coord;

	while (currentPosition[0] != '\n')
	{
		if (currentPosition[0] == '<')
		{
			x_coor = currentPosition[1];
			currentPosition += 3;
			y_coor = (int)strtol(currentPosition, &currentPosition, 10);
			graphicCoordToRealCoord(&coord, x_coor, y_coor);
			if (GetContentOfCoord(board, coord) == EMPTY || IsEnemy(GetContentOfCoord(board, coord), (properties[4]) ? BLACK_P : WHITE_P)) //isWhite(GetContentOfCoord(board, coord)) == 1 - properties[3]) 
				return 0;
			else
				return 1;
		}
		else
			currentPosition++;
	}
	return 0;

}

/*indices of coords from the graphic printed board and the implemented 2-dim char array differ from one another.
this function "corrects" the coordinate crd from the first to the latter.*/
void graphicCoordToRealCoord(Coord *crd, char x_coor, int y_coor)
{
	crd->i_coord = (int)(x_coor - 97);
	crd->j_coord = y_coor - 1;
}

/*writes EMPTY to each cell of the board*/
void clearBoard(board_t board)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			board[i][j] = EMPTY;
		}
	}
}

/*returns a string of the player color*/
const char* getPlayerColor()
{
	if (properties[3] == 0)//if player is white
		return constWhite;
	else//if player is black
		return constBlack;
}

/*returns a string of the computer color*/
const char* getComputerColor()
{
	if (properties[3] != 0) //if computer is white
		return constWhite;
	else //if computer is black
		return constBlack;
}

/* 0 for player, 1 for computer*/
char getGenericTool(int isComputer)
{
	if (isComputer)
	{
		if (strcmp(getComputerColor(), WHITE) == 0)
			return WHITE_M;
		else
			return BLACK_M;
	}
	else
	{
		if (strcmp(getPlayerColor(), WHITE) == 0)
			return WHITE_M;
		else
			return BLACK_M;
	}

}

/*DEBUG*/
Move* chooseMoveRandonly(board_t brd)
{
	int counter = 0;
	Move *tmp = pMove;
	while (tmp != NULL)
	{
		counter++;
		tmp = tmp->next;
	}
	if (counter == 0)
		print_board(brd);
	//printf("%u\n", counter);
	counter = random_at_most(counter);
	//printf("%u\n\n", counter);
	//counter--;
	tmp = pMove;
	for (int i = 0; i < counter; i++)
		tmp = tmp->next;
	return tmp;
}

/*returns the corresponding king of that arg type*/
char getCorrespondingKing(char type)
{
	if (isWhite(type)) //if type is white
		return WHITE_K;
	else
		return BLACK_K;
}

/*prints to screen who the winner is*/
int printWinner(int computerWins)
{
	if (properties[1])
		return 0;
	if (computerWins)
		printf("%s", getComputerColor());
	else
		printf("%s", getPlayerColor());
	printf(" %s", PLAYER_WINS);
	return 0;

}

/*DCLR*/
int LoadFromFile(char* file_path, board_t board){
	int i, j;
	char str[51], curTag;
	FILE *file = fopen(file_path, "r");
	if (file == NULL)
	{
		printf("%s", WRONG_FILE_NAME);
		return 1;
	}




	//placing the file pointer on the first tag after <game> tag
	for (i = 0; i < 5; i++)
		fscanf(file, "%s", str);

	if (*(str + 1) == 'n'){ //next turn
		if (*(str + 11) == 'W')
			properties[4] = 0;
		else
			properties[4] = 1;
		//read next line
		fscanf(file, "%s", str);
	}

	if (*(str + 1) == 'g'){ //game mode
		properties[5] = *(str + 11) - '0';
		//read next line
		fscanf(file, "%s", str);
	}
	else
		properties[5] = 1;


	if (*(str + 1) == 'd'){ //difficulty (optional tag)
		if (*(str + 12) == 'b')
			properties[2] = 5;
		else
			properties[2] = *(str + 12) - '0';
		//read next line
		fscanf(file, "%s", str);
	}
	else
		properties[2] = 1;


	if (*(str + 1) == 'u'){ //user color (optional tag)
		if (*(str + 12) == 'W')
			properties[3] = 0;
		else
			properties[3] = 1;
		//read next line
		fscanf(file, "%s", str);
	}
	else
		properties[3] = 1;


	//reading board slots:
	for (j = BOARD_SIZE - 1; j >= 0; j--){
		fscanf(file, "%s", str);
		for (i = 0; i < BOARD_SIZE; i++){
			if (*(str + 7 + i) != '_')
				board[i][j] = *(str + 7 + i);
			else
				board[i][j] = EMPTY;
		}
	}

	fclose(file);
	PrintBoard(board);
	return 0;
}








/* UNUSED*/
void quit(board_t brd, int freeBoard)
{
	if (freeBoard)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			free(brd[i]);
		free(brd);
	}
	exit(1);
}


int main()
{

	//srand(time(NULL));
	int parseResult = 0;
	char* board[BOARD_SIZE];
	char row0[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row1[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row2[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row3[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row4[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row5[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row6[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row7[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	board[0] = row0; board[1] = row1; board[2] = row2; board[3] = row3; board[4] = row4;
	board[5] = row5; board[6] = row6; board[7] = row7;

	board_t brd = board;//= NULL; TODO: experiement
	char *input = NULL;
	Move *computerMove = NULL;
	Move *allPossibleMoves = NULL, *tmp;




	//freeBoard = 0;// TODO: experiement
	//brd = //createBoard(); TODO: experiement
	/*if (brd == NULL)
	{
	freeBoard = 0;
	}*/

	init_board(brd);

	/*if (DEBUG)
	brd = GenerateRandomConfiguration();
	if (DEBUG) //tests
	{
	for (int i = 1; i < 12; i++)
	{
	if (i==9)
	brd = unitTest_movements(i);

	}
	}*/

	//print_board(brd);
	if (!properties[1])
		printf(ENTER_SETTINGS);


	/* One loop for both settings and game */
	while (!properties[1])//while not quit
	{
		if (properties[5] == 2 && !properties[0] && (properties[4] != properties[3]))//if in AI mode, in game state and it is the computer's turn
		{
			computerMove = NULL;
			if (score(brd, getGenericTool(1)) == -100)//no legal move for the computer (computer lost)
			{
				printWinner(0);//print player wins;
				properties[1] = 1;
			}

			//run MINIMAX
			if (properties[3])//if computer's color is white
				minimax_score(brd, WHITE_M, properties[2], 1, &computerMove);
			else
				minimax_score(brd, BLACK_M, properties[2], 1, &computerMove);

			//print computer's move
			if (computerMove != NULL)
			{
				if (!properties[1])
					printf("Computer: move ");
				printMove(computerMove);
				makeMove(brd, computerMove);
				free(computerMove->route);
				free(computerMove);
				//print board
				print_board(brd);
			}


			//change player
			properties[4] = 1 - properties[4];
			if ((score(brd, getGenericTool(0)) == -100))//if no legal move for the player (player lost)
			{
				printWinner(1);//print computer wins;
				properties[1] = 1;

			}
			continue;
		}
		else if (!properties[0])
		{
			/*if (allPossibleMoves == NULL)
			{
			if (properties[3] == 0)//if player is white
			allPossibleMoves = getMoves(brd, WHITE_M);
			else //if player is black
			allPossibleMoves = getMoves(brd, BLACK_M);
			pMove = allPossibleMoves;
			}
			if (properties[1]) //if quit
			{
			pMove = allPossibleMoves;

			while (allPossibleMoves != NULL)
			{
			allPossibleMoves = allPossibleMoves->next;
			free(pMove->route);
			free(pMove);
			pMove = allPossibleMoves;
			}
			continue;
			}*/
			if (properties[1]) //if quit
				continue;
			if (properties[4] == 0)
				printf(WHT_ENTER_YOUR_MOVE);
			else
				prinft(BLK_ENTER_YOUR_MOVE);


		}

		if (properties[1])//if quit
			continue;

		input = getLine();

		if (!properties[1])//if not quit
			parseResult = Parse(input, brd);

		if (parseResult == 10 || properties[1] == 1)// user has successfully inserted a valid move, we now want to free allPossibleMoves
		{
			/*tmp = allPossibleMoves;
			while (allPossibleMoves != NULL)
			{
			allPossibleMoves = allPossibleMoves->next;
			free(tmp->route);
			free(tmp);
			tmp = allPossibleMoves;
			}*/
			//change color of current player
			properties[4] = 1 - properties[4];
		}

		if (input != NULL)
		{
			free(input);
			input = NULL;
		}
	}
	if (input != NULL)
		free(input);


	//quit(brd, freeBoard);
	exit(1);
	return 0;


	/*DEBUG*/
	/*
	int iterations = 250;
	int printMode = 0;


	while (!properties[1]&&iterations>0)//while not quit
	{
	computerMove = NULL;
	if (!properties[0] && (properties[4] != properties[3]))//if in game state and it is the computer's turn
	{

	if (score(brd, getGenericTool(1)) == -100)//no legal move for the computer (computer lost)
	{
	printWinner(0);//print player wins;
	//TODO:terminate the game!
	brd = GenerateRandomConfiguration();
	//brd = unitTest_movements(9);
	iterations--;
	printf("%u\n", iterations);
	if (iterations == 4504)
	printMode = 1;
	continue;
	}

	//run MINIMAX
	if (properties[3])//if computer's color is white
	minimax_score(brd, WHITE_M, properties[2], 1, &computerMove);
	else
	minimax_score(brd, BLACK_M, properties[2], 1, &computerMove);

	//print computer's move
	if (computerMove != NULL)
	{
	if (printMode)
	{
	printf("Computer: move\n ");
	printMove(computerMove);
	print_board(brd);
	}

	}
	if (computerMove == NULL)
	print_board(brd);
	makeMove(brd, computerMove);
	free(computerMove->route);
	free(computerMove);
	//print board
	//print_board(brd);
	//change player

	if ((score(brd, getGenericTool(0)) == -100))//if no legal move for the player (player lost)
	{
	printWinner(1);//print computer wins;
	//TODO:terminate the game!
	brd = GenerateRandomConfiguration();
	//brd = unitTest_movements(9);
	iterations--;
	printf("%u\n", iterations);
	if (iterations == 4504)
	printMode = 1;
	continue;
	}
	properties[4] = 1 - properties[4];
	continue;
	}
	else if (!properties[0] && (properties[4] == properties[3]))//if in game state and it is the player's turn
	{
	if (allPossibleMoves == NULL)
	{
	if (properties[3] == 0)//if player is white
	allPossibleMoves = getMoves(brd, WHITE_M);
	else //if player is black
	allPossibleMoves = getMoves(brd, BLACK_M);
	pMove = allPossibleMoves;
	}
	//printf(ENTER_YOUR_MOVE);
	}

	computerMove = chooseMoveRandonly(brd);
	makeMove(brd, computerMove);
	if (printMode)
	{
	printf("player: move ");
	printMove(computerMove);
	print_board(brd);
	}
	tmp = allPossibleMoves;
	while (allPossibleMoves != NULL)
	{
	allPossibleMoves = allPossibleMoves->next;
	free(tmp->route);
	free(tmp);
	tmp = allPossibleMoves;
	}
	properties[4] = 1 - properties[4];
	}
	printf("bye");

	return 0;*/
}

