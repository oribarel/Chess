
#include "Chess_Logic.h"

int properties[6] = { 1, 0, 1, WHITE_PLAYER, WHITE_PLAYER, 1 };
/*	'settings'		0
'quit'				1
'minimax_depth'		2
'player color'		3
'current player'	4
'game mode'			5


the initial state of the global properties array:
properties[0] = 1;	//Setting state
properties[1] = 0;	//Don't quit
properties[2] = 1;	//Default minimax depth. 0 stands for best
properties[3] = WHITE_PLAYER;	//Default player color white
properties[4] = WHITE_PLAYER //It is now white's\black's turn: WHITE_PLAYER for white, BLACK_PLAYER for black
properties[5] = 1;	//Default game mode is "two players mode"
*/

int gameInfo[6] = { 0, 0, 0, 0, 0, 0 };
/*
'number of black tools in White King's danger zone'		0
'number of white tools in Black King's danger zone		1
*/

Coord WhiteKing = { -1, -1 };
Coord BlackKing = { -1, -1 };

Coord WhiteKingDangerZone[BOARD_SIZE * 2] = { 0 };
Coord BlackKingDangerZone[BOARD_SIZE * 2] = { 0 };





const char *constWhite = "white";
const char *constBlack = "black";

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
	crd.j_coord = j;
	for (; i < BOARD_SIZE; i++)
	{
		crd.i_coord = i;
		setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_P : BLACK_P);
	}
}

/*DCLR*/
//higher tools row
void init_rowV(board_t board, int player)
{
	Coord crd;
	int j = player == WHITE_PLAYER ? 0 : 7;
	int i = 0;
	crd.i_coord = i;
	crd.j_coord = j;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_R : BLACK_R);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_N : BLACK_N);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_B : BLACK_B);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_Q : BLACK_Q);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_K : BLACK_K);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_B : BLACK_B);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_N : BLACK_N);
	crd.i_coord++;
	setSlotInBoard(board, crd, player == WHITE_PLAYER ? WHITE_R : BLACK_R);
}


int generateEnemyColor(int playerColor)
{
	if (playerColor == WHITE_PLAYER)
		return BLACK_PLAYER;
	else
		return WHITE_PLAYER;
}

char generateTool(int player, eTool type)
{
	if (player == WHITE_PLAYER)
	{
		if (type == Pawn)
			return WHITE_P;
		if (type == Knight)
			return WHITE_N;
		if (type == Rook)
			return WHITE_R;
		if (type == Bishop)
			return WHITE_B;
		if (type == Queen)
			return WHITE_Q;
		if (type == King)
			return WHITE_K;
	}
	if (player == BLACK_PLAYER)
	{
		if (type == Pawn)
			return BLACK_P;
		if (type == Knight)
			return BLACK_N;
		if (type == Rook)
			return BLACK_R;
		if (type == Bishop)
			return BLACK_B;
		if (type == Queen)
			return BLACK_Q;
		if (type == King)
			return BLACK_K;
	}
	return EMPTY;
}



cMove *QueenMoves(board_t board, Coord coord)
{
	cMove *rookAspect = NULL, *bishopAspect = NULL, *tmp;
	rookAspect = (cMove *)RookMoves(board, coord);
	bishopAspect = (cMove *)BishopMoves(board, coord);
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

/*CONVD*/
/*returns 0 if the two chars reperesent allies*/
int IsEnemy(char slot, int myColor)
{
	if (myColor == WHITE_PLAYER) //player is white
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
	else //player is black
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
	return 0; //shouldn't get here
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

/*return the char in the specific location coord in the board.*/
char GetContentOfCoord(board_t board, Coord coord)
{
	return board[coord.i_coord][coord.j_coord];
}

/*TODO: consider this*/
/*sets the coordinate slot in the board to be ch.*/
void setSlotInBoard(board_t board, Coord slot, char ch)
{
	/*if (anointAKing(slot.j_coord, ch))
	board[slot.i_coord][slot.j_coord] = getCorrespondingKing(ch);
	else*/

	//set
	board[slot.i_coord][slot.j_coord] = ch;

	//update kings' coordinates and danger zones
	if (ch == WHITE_K || ch == BLACK_K)
	{
		if (ch == WHITE_K)
		{
			WhiteKing = slot;

		}
		if (ch == BLACK_K)
		{
			BlackKing = slot;

		}
	}
	UpdateDangerZone(board, WHITE_PLAYER);
	UpdateDangerZone(board, BLACK_PLAYER);
}



/*CONVD*/
/*TODO: change this: returns 0 if tool is an oponnent tool
return 1 if tool is ally man
retrun 3 if tool is ally king
*/
int IsAlly(char tool, int myColor)
{
	if (myColor == BLACK_PLAYER) //player is black
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
	else //player is white
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
	res.i_coord += i_offset;
	res.j_coord += j_offset;
	return res;
}


int freeMovesList(cMove *move)
{
	cMove *curr = move, *tmp;
	while (curr != NULL)
	{
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
	return 1;
}

/*add a new move in the beginning of the linked list*/
cMove *AddMove(cMove **head, char toolType, Coord src, Coord dst, int eater, int promote)
{
	cMove *newMove = NULL;
	//empty cell -> no eating
	if (eater == EMPTY)
		eater = 0;
	newMove = (cMove*)calloc(1, sizeof(cMove));
	newMove->toolType = toolType;
	newMove->src = src;
	newMove->dst = dst;
	newMove->eaten = eater;
	newMove->promote = promote;
	newMove->next = *head;
	*head = newMove;
	return *head;
}

int addToDangerZone(int playerColor, Coord crd)
{
	if (playerColor == WHITE_PLAYER)
	{
		WhiteKingDangerZone[gameInfo[0]] = crd;
		gameInfo[0]++;
	}
	else
	{
		BlackKingDangerZone[gameInfo[1]] = crd;
		gameInfo[1]++;
	}
	return 0;
}

int ResetDangerZone(int playerColor)
{
	int j;
	Coord *dangerZone = playerColor == WHITE_PLAYER ? WhiteKingDangerZone : BlackKingDangerZone;
	if (playerColor == WHITE_PLAYER)
		j = 0;
	else
		j = 1;
	for (int i = 0; i < gameInfo[j]; i++)
	{
		dangerZone[i].i_coord = 0;
		dangerZone[i].j_coord = 0;
	}
	gameInfo[j] = 0;
	return 0;
}

int UpdateDangerZone(board_t board, int playerColor)
{
	Coord KingCrd = (playerColor == WHITE_PLAYER) ? WhiteKing : BlackKing;
	Coord tmp = KingCrd, possibilities[8];
	int d = playerColor == WHITE_PLAYER ? 1 : -1;
	ResetDangerZone(playerColor);
	gameInfo[playerColor == WHITE_PLAYER ? 0 : 1] = 0;

	//pawns
	tmp = offsetCoord(KingCrd, -1, d);
	if (isInBoard(tmp) && (GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Pawn) ||
		GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Queen)))
		addToDangerZone(playerColor, tmp);

	tmp = offsetCoord(KingCrd, 1, d);
	if (isInBoard(tmp) && (GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Pawn) ||
		GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Queen)))
		addToDangerZone(playerColor, tmp);

	//knights
	possibilities[0] = offsetCoord(KingCrd, 1, 2);
	possibilities[1] = offsetCoord(KingCrd, 2, 1);
	possibilities[2] = offsetCoord(KingCrd, 2, -1);
	possibilities[3] = offsetCoord(KingCrd, 1, -2);
	possibilities[4] = offsetCoord(KingCrd, -1, -2);
	possibilities[5] = offsetCoord(KingCrd, -2, -1);
	possibilities[6] = offsetCoord(KingCrd, -2, 1);
	possibilities[7] = offsetCoord(KingCrd, -1, 2);

	for (int i = 0; i < 8; i++)
	{
		if (isInBoard(possibilities[i]) &&
			GetContentOfCoord(board, possibilities[i]) == generateTool(generateEnemyColor(playerColor), Knight))
			addToDangerZone(playerColor, possibilities[i]);
	}

	//rooks
	for (int h = -1; h < 2; h += 2)
	{
		tmp = KingCrd;
		while (isInBoard(tmp))
		{
			if (GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Rook) ||
				GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Queen))
				addToDangerZone(playerColor, tmp);
			tmp = offsetCoord(tmp, h, 0);
		}
	}

	for (int v = -1; v < 2; v += 2)
	{
		tmp = KingCrd;
		while (isInBoard(tmp))
		{
			if (GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Rook) ||
				GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Queen))
				addToDangerZone(playerColor, tmp);
			tmp = offsetCoord(tmp, 0, v);
		}
	}

	//Bishops
	for (int h = -1; h < 2; h += 2)
	{
		for (int v = -1; v < 2; v += 2)
		{
			tmp = offsetCoord(KingCrd, h, v);
			while (isInBoard(tmp))
			{
				if (GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Bishop) ||
					GetContentOfCoord(board, tmp) == generateTool(generateEnemyColor(playerColor), Queen))
					addToDangerZone(playerColor, tmp);
				tmp = offsetCoord(tmp, h, v);
			}
		}
	}

	//king


	possibilities[0] = offsetCoord(KingCrd, 1, 1);
	possibilities[1] = offsetCoord(KingCrd, 0, 1);
	possibilities[2] = offsetCoord(KingCrd, 1, 0);
	possibilities[3] = offsetCoord(KingCrd, 1, -1);
	possibilities[4] = offsetCoord(KingCrd, 0, -1);
	possibilities[5] = offsetCoord(KingCrd, -1, -1);
	possibilities[6] = offsetCoord(KingCrd, -1, 0);
	possibilities[7] = offsetCoord(KingCrd, -1, 1);

	for (int i = 0; i < 8; i++)
	{
		if (isInBoard(possibilities[i]) &&
			GetContentOfCoord(board, possibilities[i]) == generateTool(generateEnemyColor(playerColor), King))
			addToDangerZone(playerColor, possibilities[i]);
	}

	return 0;
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

/*return 0 if empty, otherwish WHITE_PLAYER or BLACK_PLAYER according to coord*/
int getColor(board_t board, Coord coord)
{
	if (GetContentOfCoord(board, coord) == EMPTY)
		return 0;
	int color = islower(GetContentOfCoord(board, coord)) ? WHITE_PLAYER : BLACK_PLAYER;
	return color;
}

int getType(board_t board, Coord crd)
{
	char ch = GetContentOfCoord(board, crd);
	if (ch == WHITE_P || ch == BLACK_P)
		return Pawn;
	if (ch == WHITE_N || ch == BLACK_N)
		return Knight;
	if (ch == WHITE_R || ch == BLACK_R)
		return Rook;
	if (ch == WHITE_B || ch == BLACK_B)
		return Bishop;
	if (ch == WHITE_Q || ch == BLACK_Q)
		return Queen;
	if (ch == WHITE_K || ch == BLACK_K)
		return King;
	return EMPTY;
}

int AreTwoCoordsEqual(Coord c1, Coord c2)
{
	if (c1.i_coord == c2.i_coord && c1.j_coord == c2.j_coord)
		return 1;
	else
		return 0;
}

int isAttacking(board_t board, Coord attacker, Coord victim)
{
	int h, v;
	Coord tmp;
	if (getColor(board, attacker) == getColor(board, victim) || GetContentOfCoord(board, attacker) == EMPTY)
	{
		return 0;
	}
	else
	{
		if (getType(board, attacker) == Pawn)
		{
			int d = getColor(board, attacker) == WHITE_PLAYER ? 1 : -1;
			if (attacker.j_coord + d == victim.j_coord &&
				((attacker.i_coord + 1 == victim.i_coord) || (attacker.i_coord - 1 == victim.i_coord)))
			{
				return 1;
			}
		}
		if (getType(board, attacker) == Knight)
		{
			if (AreTwoCoordsEqual(offsetCoord(victim, 1, 2), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 2, 1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 2, -1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 1, -2), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -1, -2), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -2, -1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -2, 1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -1, 2), attacker))
			{
				return 1;
			}
		}
		if (getType(board, attacker) == Bishop || getType(board, attacker) == Queen)
		{
			if (abs(attacker.i_coord - victim.i_coord) == abs(attacker.j_coord - victim.j_coord)) //are the two tools on the same diagonal
			{
				h = (attacker.i_coord > victim.i_coord) ? 1 : -1;
				v = (attacker.j_coord > victim.j_coord) ? 1 : -1;
				tmp = victim;
				tmp = offsetCoord(tmp, h, v);
				while (!AreTwoCoordsEqual(tmp, attacker))
				{
					if (GetContentOfCoord(board, tmp) != EMPTY)
						return 0;
					tmp = offsetCoord(tmp, h, v);
				}
				return 1;
			}
			//else
			//	return 0;
		}
		if (getType(board, attacker) == Rook || getType(board, attacker) == Queen)
		{
			if (attacker.i_coord == victim.i_coord)  //are the two tools on the same row
			{
				v = (attacker.j_coord > victim.j_coord) ? 1 : -1;

				tmp = victim;
				tmp = offsetCoord(tmp, 0, v);
				while (!AreTwoCoordsEqual(tmp, attacker))
				{
					if (GetContentOfCoord(board, tmp) != EMPTY)
						return 0;
					tmp = offsetCoord(tmp, 0, v);
				}
				return 1;
			}
			if (attacker.j_coord == victim.j_coord) //are the two tools on the same column
			{
				h = (attacker.i_coord > victim.i_coord) ? 1 : -1;

				tmp = victim;
				tmp = offsetCoord(tmp, h, 0);
				while (!AreTwoCoordsEqual(tmp, attacker))
				{
					if (GetContentOfCoord(board, tmp) != EMPTY)
						return 0;
					tmp = offsetCoord(tmp, h, 0);
				}
				return 1;
			}
			else
				return 0;
		}
		if (getType(board, attacker) == King)
		{
			if (AreTwoCoordsEqual(offsetCoord(victim, 0, 1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 1, 1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 1, 0), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 1, -1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, 0, -1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -1, -1), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -1, 0), attacker) ||
				AreTwoCoordsEqual(offsetCoord(victim, -1, 1), attacker))
			{
				return 1;
			}
			else
				return 0;

		}

		return 0;
	}
}

/*returns 1 when moving src tool to dst coord will open king defences
Not to be used when src tool is the King itself*/
int openKingDefences(board_t board, Coord src, Coord dst)
{
	int player = getColor(board, src);
	char srcType = GetContentOfCoord(board, src);
	char dstType = GetContentOfCoord(board, dst);
	Coord *dangerZone, kingCrd;
	int dangerZone_AmountOfPieces;
	dangerZone = player == WHITE_PLAYER ? WhiteKingDangerZone : BlackKingDangerZone;
	dangerZone_AmountOfPieces = player == WHITE_PLAYER ? gameInfo[0] : gameInfo[1];
	kingCrd = player == WHITE_PLAYER ? WhiteKing : BlackKing;

	setSlotInBoard(board, src, EMPTY);
	setSlotInBoard(board, dst, srcType);

	int result = 0;
	for (int i = 0; i < dangerZone_AmountOfPieces; i++)
	{
		result |= isAttacking(board, dangerZone[i], kingCrd);
	}

	setSlotInBoard(board, src, srcType);
	setSlotInBoard(board, dst, dstType);

	return result;
}

/*Pre: dst coord is adjacent to KingCrd coord*/
int safeToMoveKing(board_t board, int player, Coord dst)
{
	char dstType = GetContentOfCoord(board, dst);
	Coord *dangerZone, kingCrd;
	int dangerZone_AmountOfPieces;

	kingCrd = player == WHITE_PLAYER ? WhiteKing : BlackKing;

	setSlotInBoard(board, kingCrd, EMPTY);
	setSlotInBoard(board, dst, generateTool(player, King));



	//UpdateDangerZone(board, player);
	dangerZone = player == WHITE_PLAYER ? WhiteKingDangerZone : BlackKingDangerZone;
	dangerZone_AmountOfPieces = player == WHITE_PLAYER ? gameInfo[0] : gameInfo[1];

	int result = 1;
	for (int i = 0; i < dangerZone_AmountOfPieces; i++)
	{
		result &= !isAttacking(board, dangerZone[i], dst);
	}

	setSlotInBoard(board, kingCrd, generateTool(player, King));
	setSlotInBoard(board, dst, dstType);
	//UpdateDangerZone(board, player);

	return result;
}


cMove* PawnMoves(board_t board, Coord coord)
{
	cMove *head = NULL;
	int i = coord.i_coord, j = coord.j_coord;
	char tool = GetContentOfCoord(board, coord), tmpTool;
	int color = islower(tool) ? 1 : -1;
	Coord tmpCoord = coord;

	//no eating
	tmpCoord.j_coord += color * 1;
	if (isInBoard(tmpCoord) == 1 && !openKingDefences(board, coord, tmpCoord) && GetContentOfCoord(board, tmpCoord) == EMPTY)
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


	//for black:south-east eating, for white:north-west eating
	tmpCoord = coord;
	tmpCoord.i_coord -= color * 1;
	tmpCoord.j_coord += color * 1;
	if (isInBoard(tmpCoord) == 1 && !openKingDefences(board, coord, tmpCoord) && DoesCrdContainsEnemy(board, tmpCoord, tool))
	{
		tmpTool = GetContentOfCoord(board, tmpCoord);
		if ((j + color * 1 == BOARD_SIZE - 1 && color == 1) || (j + color * 1 == 0 && color == -1)) //if promotion needed
		{
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'r' : 'R');
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'b' : 'B');
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'q' : 'Q');
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'n' : 'N');
		}
		else
			AddMove(&head, tool, coord, tmpCoord, tmpTool, 0);// if promotion isn't needed
	}


	//for black:south-west eating, for white:north-east eating
	tmpCoord = coord;
	tmpCoord.i_coord += color * 1;
	tmpCoord.j_coord += color * 1;
	if (isInBoard(tmpCoord) == 1 && !openKingDefences(board, coord, tmpCoord) && DoesCrdContainsEnemy(board, tmpCoord, tool))
	{
		tmpTool = GetContentOfCoord(board, tmpCoord);
		if ((j + color * 1 == BOARD_SIZE - 1 && color == 1) || (j + color * 1 == 0 && color == -1)) //if promotion needed
		{
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'r' : 'R');
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'b' : 'B');
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'q' : 'Q');
			AddMove(&head, tool, coord, tmpCoord, tmpTool, color == 1 ? 'n' : 'N');
		}
		else
			AddMove(&head, tool, coord, tmpCoord, tmpTool, 0);// if promotion isn't needed
	}
	return head;
}

cMove* BishopMoves(board_t board, Coord coord){
	cMove *head = NULL;
	int i = coord.i_coord, j = coord.j_coord;
	char tool = GetContentOfCoord(board, coord);
	int color = islower(tool) ? 1 : -1;
	int openDif = 0;



	//north-east
	int k = 1;
	Coord tmpCoord = offsetCoord(coord, k, k);
	while (isInBoard(tmpCoord))
	{
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1)
		{
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		}
		if (GetContentOfCoord(board, tmpCoord) == EMPTY)
		{
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, k, k);
	}
	openDif = 0;
	k = 1;
	//north-west
	tmpCoord = offsetCoord(coord, -k, k);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		} if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, -k, k);
	}

	openDif = 0;
	k = 1;
	//south-east
	tmpCoord = offsetCoord(coord, k, -k);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		} if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, k, -k);
	}
	openDif = 0;
	k = 1;
	//south-west
	tmpCoord = offsetCoord(coord, -k, -k);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		} if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, -k, -k);
	}
	return head;
}

cMove* RookMoves(board_t board, Coord coord){
	cMove *head = NULL;
	int i = coord.i_coord, j = coord.j_coord;
	char tool = GetContentOfCoord(board, coord);
	int color = islower(tool) ? 1 : -1;
	int openDif = 0;

	//north
	int k = 1;
	Coord tmpCoord = offsetCoord(coord, 0, k);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		} if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, 0, k);
	}

	//south
	openDif = 0;
	k = 1;
	tmpCoord = offsetCoord(coord, 0, -k);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		} if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, 0, -k);
	}



	//east
	openDif = 0;
	k = 1;
	tmpCoord = offsetCoord(coord, k, 0);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		} if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, k, 0);
	}

	//west
	openDif = 0;
	k = 1;
	tmpCoord = offsetCoord(coord, -k, 0);
	while (isInBoard(tmpCoord)) {
		openDif = openKingDefences(board, coord, tmpCoord);
		if (getColor(board, tmpCoord) == color * -1){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, GetContentOfCoord(board, tmpCoord), 0);//eating
			break;
		}
		if (GetContentOfCoord(board, tmpCoord) == EMPTY){
			if (!openDif)
				AddMove(&head, tool, coord, tmpCoord, 0, 0);//no-eating
			k++;
		}
		else
			break;
		tmpCoord = offsetCoord(coord, -k, 0);
	}
	return head;
}






cMove* KnightMoves(board_t board, Coord coord)
{
	cMove *head = NULL;

	int color = getColor(board, coord);
	char tool = color == WHITE_PLAYER ? WHITE_N : BLACK_N;

	Coord tmpCrd = coord, possibilities[8];
	possibilities[0] = offsetCoord(coord, 1, 2);
	possibilities[1] = offsetCoord(coord, 2, 1);
	possibilities[2] = offsetCoord(coord, 2, -1);
	possibilities[3] = offsetCoord(coord, 1, -2);
	possibilities[4] = offsetCoord(coord, -1, -2);
	possibilities[5] = offsetCoord(coord, -2, -1);
	possibilities[6] = offsetCoord(coord, -2, 1);
	possibilities[7] = offsetCoord(coord, -1, 2);
	for (int i = 0; i < 8; i++)
	{
		if (isInBoard(possibilities[i]) &&
			(GetContentOfCoord(board, possibilities[i]) == EMPTY || DoesCrdContainsEnemy(board, possibilities[i], tool)))
		{
			if (!openKingDefences(board, coord, possibilities[i]))
				AddMove(&head, tool, coord, possibilities[i], GetContentOfCoord(board, possibilities[i]), 0);
		}
	}
	return head;
}

cMove* KingMoves(board_t board, Coord coord)
{
	cMove *head = NULL;

	int color = getColor(board, coord);
	char tool = color == WHITE_PLAYER ? WHITE_K : BLACK_K;

	Coord tmpCrd = coord, possibilities[8];
	possibilities[0] = offsetCoord(coord, 0, 1);
	possibilities[1] = offsetCoord(coord, 1, 1);
	possibilities[2] = offsetCoord(coord, 1, 0);
	possibilities[3] = offsetCoord(coord, 1, -1);
	possibilities[4] = offsetCoord(coord, 0, -1);
	possibilities[5] = offsetCoord(coord, -1, -1);
	possibilities[6] = offsetCoord(coord, -1, 0);
	possibilities[7] = offsetCoord(coord, -1, 1);


	for (int i = 0; i < 8; i++)
	{
		if (isInBoard(possibilities[i]) &&
			(GetContentOfCoord(board, possibilities[i]) == EMPTY || DoesCrdContainsEnemy(board, possibilities[i], tool)))
		{
			if (safeToMoveKing(board, color, possibilities[i]))
				AddMove(&head, tool, coord, possibilities[i], GetContentOfCoord(board, possibilities[i]) != EMPTY, 0);
		}
	}
	return head;
}

//TODO: implement 
cMove *movesByPieceType(board_t board, Coord coord)
{
	char cordTool = GetContentOfCoord(board, coord);
	switch (cordTool)
	{
	case WHITE_P:
	case BLACK_P:
		return PawnMoves(board, coord);
		break;
	case WHITE_B:
	case BLACK_B:
		return BishopMoves(board, coord);
		break;
	case WHITE_N:
	case BLACK_N:
		return KnightMoves(board, coord);
		break;
	case WHITE_R:
	case BLACK_R:
		return RookMoves(board, coord);
		break;
	case WHITE_Q:
	case BLACK_Q:
		return QueenMoves(board, coord);
		break;
	case WHITE_K:
	case BLACK_K:
		return KingMoves(board, coord);
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}

/*returns all moves that a player may do with the specific configuration of the board (filtered).*/
cMove *getMoves(board_t board, int player)
{
	cMove *allMoves = NULL, *tmpMoves, *tmpMovesEnd;
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

	/*allMoves = filterOutMoves(allMoves);*/

	//if (DEBUG)
	//{
	//	//printf("\nafter filtering moves list was:\n");
	//	printMovesList(allMoves);
	//	printf("\n");
	//}

	return allMoves;
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
		if (numberOfTools <= 8)
			result = 1;
		break;
	case WHITE_B:
	case BLACK_B:
	case WHITE_N:
	case BLACK_N:
	case WHITE_R:
	case BLACK_R:
		if (numberOfTools <= 2)
			result = 1;
		break;
	case WHITE_Q:
	case BLACK_Q:
	case WHITE_K:
	case BLACK_K:
		if (numberOfTools <= 1)
			result = 1;
		break;

	default:
		break;
	}
	return result;
}

Coord GenerateCoord(int x, int y)
{
	Coord crd;
	crd.i_coord = x;
	crd.j_coord = y;
	return crd;
}

/*return 1 if the king of the given player is under threat*/
int KingUnderThreat(board_t board, int player)
{
	Coord kingCrd, tmpCrd;
	kingCrd = WHITE_PLAYER == player ? WhiteKing : BlackKing;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			tmpCrd = GenerateCoord(i, j);
			if (isAttacking(board, tmpCrd, kingCrd))
				return 1;
		}
	}
	return 0;
}

/* the scoring function:
See instructions for more details.*/
int score(board_t board, int player)
{
	int canMove = 0;

	Coord coord;
	int score = 0, ally, enemy, playerBlocked = 1, opponentBlocked = 1;
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
	{
		coord.i_coord = (int)mod(k, BOARD_SIZE);
		coord.j_coord = k / BOARD_SIZE;
		ally = IsAlly(GetContentOfCoord(board, coord), player);
		enemy = IsEnemy(GetContentOfCoord(board, coord), player);
		score += (ally - enemy);
		if (GetContentOfCoord(board, coord) != EMPTY)
		{
			canMove = canMoveThisTool(board, coord);
			if (ally > 0 && canMove == 1)
				playerBlocked = 0;
			if (enemy > 0 && canMove == 1)
				opponentBlocked = 0;
		}

	}


	/*TODO: consider what to do with this part*/
	if (playerBlocked == 1)
		if (KingUnderThreat(board, player))
			return MATE_WIN_LOSE;//player lost
		else
			return TIE_SCORE; //tie
	if (opponentBlocked == 1)
		if (KingUnderThreat(board, -player))
			return MATE_WIN_SCORE;//player won
		else
			return TIE_SCORE; //tie
	if (DEBUG && score == MATE_WIN_LOSE
		)
	{
		print_board(board);
		printf("%d \n", score);
	}

	return score;

}

int canMoveThisTool(board_t board, Coord coord)
{
	int result = 0;
	cMove *head = movesByPieceType(board, coord);
	if (head != NULL)
	{
		freeMovesList(head);
		result = 1;
	}
	return result;
}

/*returns 1 if type is white and 0 otherwise.*/
int isWhite(char type)
{
	if (type == WHITE_R || type == WHITE_N || type == WHITE_B || type == WHITE_K || type == WHITE_Q || type == WHITE_P)
		return 1;
	return 0;
}






/*changes the board so it describes the state created after the move is made.*/
int makeMove(board_t board, cMove *move)
{
	char myColor = GetContentOfCoord(board, move->src);//TODO: consider delete this line
	Coord crd = move->src;
	setSlotInBoard(board, crd, EMPTY);
	if (move->promote)
		setSlotInBoard(board, move->dst, move->promote);
	else
		setSlotInBoard(board, move->dst, move->toolType);
	return 0;


}



/*DCLR*/
char ToolNameToChar(char *toolFullName)
{
	char toolFirstChar = *toolFullName;
	switch (toolFirstChar)
	{
	case 'p':
		return 'm';
	case 'b':
		return 'b';
	case 'k':
		if (toolFullName[1] == 'i')
			return 'k';
		else
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


/*DCLR*/
char* ToolCharToName(char toolChar)
{
	char c = tolower(toolChar);
	switch (toolChar)
	{
	case 'b':
		return BISHOP;
	case 'k':
		return KING;
	case 'r':
		return ROOK;
	case 'q':
		return QUEEN;
	case 'm':
		return PAWN;
	case 'n':
		return KNIGHT;
	default:
		break;
	}
	return NULL;

}


/*DCLR*/
int isEqualCoordinates(Coord c1, Coord c2)
{
	if (c1.i_coord == c2.i_coord&& c1.j_coord == c2.j_coord)
		return 1;
	return 0;
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
	if (properties[3] == WHITE_PLAYER)//if player is white
		return constWhite;
	else//if player is black
		return constBlack;
}

/*returns a string of the computer color*/
const char* getComputerColor()
{
	if (properties[3] != WHITE_PLAYER) //if computer is white
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


/*returns the corresponding king of that arg type*/
char getCorrespondingKing(char type)
{
	if (isWhite(type)) //if type is white
		return WHITE_K;
	else
		return BLACK_K;
}


//TODO: implement
int quit(void)
{
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


/*modulus of two integers a mod b*/
int mod(int a, int b)
{
	int ret = a % b;
	if (ret < 0)
		ret += b;
	return ret;
}



int Save(board_t board, char* file_name){
	int i, j;
	//replacing '\n' with '\0'
	file_name[strlen(file_name) - 1] = '\0';
	FILE *f = fopen(file_name, "w");
	if (f == NULL){
		printf(WRONG_FILE_NAME);
		return 0;
	}
	fprintf(f, XML_FIRST_LINE);
	fprintf(f, "<game>\n");
	if (properties[4] == WHITE_PLAYER)
		fprintf(f, "\t<next_turn>%s</next_turn>\n", WHITE);
	else
		fprintf(f, "\t<next_turn>%s</next_turn>\n", BLACK);
	fprintf(f, "\t<game_mode>%d</game_mode>\n", properties[5]);


	if (properties[5] == 2){
		if (properties[2] != BESTval)
			fprintf(f, "\t<difficulty>%d</difficulty>\n", properties[2]);
		else
			fprintf(f, "\t<difficulty>best/difficulty>\n");
		if (properties[3] == WHITE_PLAYER)
			fprintf(f, "\t<user_color>%s</user_color>\n", WHITE);
		else
			fprintf(f, "\t<user_color>%s</user_color>\n", BLACK);
	}
	else
	{
		fprintf(f, "\t<difficulty></difficulty>\n");
		fprintf(f, "\t<user_color></user_color>\n");
	}

	fprintf(f, "\t<board>\n");
	for (j = BOARD_SIZE; j > 0; j--){
		fprintf(f, "\t\t<row_%d>", j);
		for (i = 0; i < BOARD_SIZE; i++){
			if (board[i][j - 1] == EMPTY)
				fprintf(f, "_");
			else
				fprintf(f, "%c", board[i][j - 1]);
		}
		fprintf(f, "</row_%d>\n", j);
	}
	fprintf(f, "\t</board>\n");
	fprintf(f, "</game>\n");
	fclose(f);
	return 0;
}


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
			properties[4] = WHITE_PLAYER;
		else
			properties[4] = BLACK_PLAYER;
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
			properties[2] = BESTval;
		else
			properties[2] = *(str + 12) - '0';
		//read next line
		fscanf(file, "%s", str);
	}
	else
		properties[2] = 1;


	if (*(str + 1) == 'u'){ //user color (optional tag)
		if (*(str + 12) == 'W')
			properties[3] = WHITE_PLAYER;
		else
			properties[3] = BLACK_PLAYER;
		//read next line
		fscanf(file, "%s", str);
	}
	else
		properties[3] = BLACK_PLAYER;


	//reading board slots:
	for (j = BOARD_SIZE - 1; j >= 0; j--){
		fscanf(file, "%s", str);
		for (i = 0; i < BOARD_SIZE; i++){
			if (*(str + 7 + i) != '_')
			{
				board[i][j] = *(str + 7 + i);
				//update kings' coordinates
				if (*(str + 7 + i) == WHITE_K)
				{
					WhiteKing.i_coord = i;
					WhiteKing.j_coord = j;
				}
				if (*(str + 7 + i) == BLACK_K)
				{
					BlackKing.i_coord = i;
					BlackKing.j_coord = j;
				}


			}
			else
				board[i][j] = EMPTY;
		}
	}

	fclose(file);
	print_board(board);
	return 0;
}


/*CONVD*/
/*prints the route of the move*/
void printMove(cMove *move)
{
	if (properties[1])
		return;
	printf("<%c,%u> to ", (char)((move->src).i_coord) + 97, (move->src).j_coord + 1);

	printf("<%c,%u>", (char)((move->dst).i_coord) + 97, (move->dst).j_coord + 1);

	//if promotion needed
	if (move->promote)
		printf(" %s", ToolCharToName(move->promote));

	//print end of line
	printf("\n");


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