


#include "Console_Main.h"

//extern int properties[6];

cMove *pMove;

/* Sets the slot on the board, pay attention: there is a similar function with different arguments. */

void setSlot(board_t board, char *horiz, char *vert, char type)
{
	Coord coord;
	int i = (int)*horiz - 97;
	int j = (int)strtof(vert, NULL) - 1;
	coord.i_coord = i; coord.j_coord = j;
	if (isTool(type) || type == EMPTY)
	{

		if (isInBoard(coord))
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
			{//TODO: test the case in which the number of pieces equal to the maximum number of tools and the set command just changes such a piece to the exact same type
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
				board[i][j] = *(str + 7 + i);
			else
				board[i][j] = EMPTY;
		}
	}

	fclose(file);
	print_board(board);
	return 0;
}


int Save(board_t board, char* file_name){
	int i, j;
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
/*prints the route of the move*/
void printMove(cMove *move)
{
	if (properties[1])
		return;
	printf("<%c,%u> to ", (char)((move->src).i_coord) + 97, (move->src).j_coord + 1);

	printf("<%c,%u>", (char)((move->dst).i_coord) + 97, (move->dst).j_coord + 1);

	//if promotion needed
	if (move->promote)
		printf("%s", ToolCharToName(move->promote));

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

/*DELETE*/
/*
adds coord coordinate to the Coord array in index depth.
Returns a pointer to the last move in the list.
*/
#ifndef DAMKA
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
#endif



#ifndef DAMKA
/*filters out of the linked list of moves  which are illegal because
there are other moves that may eat more enemy discs*/
cMove *filterOutMoves(cMove *moves)
{
	cMove *start = moves, *include = NULL, *exclude = NULL, *next;
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
#endif




/* UNUSED */
#ifndef DAMKA
int isEaterMove(Move *move)

{
	if (move->len > 1) { return 1; }
	if (abs(move->route->i_coord - ((move->route) + 1)->i_coord) == 2){ return 1; }
	return 0;
}
#endif



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

#ifndef DAMKA
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
			if (tool == WHITE_K || tool == BLACK_K || ((tool == WHITE_M) && (crd1.j_coord > coord.j_coord)) || (tool == BLACK_M && crd1.j_coord < coord.j_coord))
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
#endif



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















#ifndef DAMKA
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
#endif

#ifndef DAMKA
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
#endif


/*DEBUG*/
#ifndef DAMKA
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
#endif

/*Parses the input inserted by the user and operates accordingly.
returns ints for convenience. specifically 10 is an important returned value: see inside.*/
int Parse(char *line, board_t board)
{
	/*
	properties[0] = Setting state
	properties[1] = quit
	properties[2] = Default minimax depth
	properties[3] = User color: WHITE_PLAYER or BLACK_PLAYER
	properties[4] = It is now white's\black's turn: WHITE_PLAYER for white, BLACK_PLAYER for black
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
					printf("%s", TWO_PLAYERS_GAME_MODE);
				else
					printf("%s", PLAYER_VS_AI_GAME_MODE);
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
				properties[2] = BESTval;//0 stands for best
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
				properties[3] = WHITE_PLAYER;
			else if (strcmp(token, black) == 0)
				properties[3] = BLACK_PLAYER;
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
				properties[4] = BLACK_PLAYER;
			else
				properties[4] = WHITE_PLAYER;

			/*if (DEBUG)
			printf("\nUpon updating next player, it is now %d\n", properties[4]);*/
			return 0;
		}


		else if (strcmp(token, cmmd7) == 0)
			//remove
		{
			char *horiz, *vert;
			Coord coord;
			int i, j;
			token = strtok(NULL, "<,");
			horiz = token;
			token = strtok(NULL, ",>");
			vert = token;

			//checking if the given coordiante resides inside the board:

			i = (int)*horiz - 97;
			j = (int)strtof(vert, NULL) - 1;
			coord.i_coord = i; coord.j_coord = j;
			if (isInBoard(coord))
				setSlot(board, horiz, vert, EMPTY);
			else
			{
				if (!properties[1])
					printf(WRONG_POSITION);
			}
			return 0;
		}


		else if (strcmp(token, cmmd8) == 0)
			//set
		{//TODO: make sure that there is at exactly one king of each type
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
			if (CountToolsOfType(board, WHITE_K) < 1 || CountToolsOfType(board, BLACK_K) < 1)
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
		if (strcmp(token, cmmd10) == 0)
			//quit
		{
			properties[1] = 1;
			return 0;
		}

		else if (strcmp(token, cmmd12) == 0)
			//move
		{
			token = strtok(NULL, "&");
			cMove *legalMove;
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
			legalMove = (cMove *)isLegalMove(token, board, pMove);
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



		else if (strcmp(token, cmmd13) == 0)
			//get_moves
		{
			Coord src;
			token = strtok(NULL, "&");
			char *currentPosition = token;
			char x_coor;
			int y_coor;

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




			//parse the input coordinate
			x_coor = currentPosition[1];
			currentPosition += 3;
			y_coor = (int)strtol(currentPosition, &currentPosition, 10);
			graphicCoordToRealCoord(&src, x_coor, y_coor);

			//get moves
			pMove = movesByPieceType(board, src);

			//print moves
			printMovesList(pMove);
			return 0;
		}




		else if (strcmp(token, cmmd14) == 0)
			//get_best_moves
		{
			int maxScore = MIN_VALUE;

			//parsing the depth d
			int d;
			char *currentPosition = NULL;
			token = strtok(NULL, "&");
			currentPosition = token;
			if (currentPosition[0] == 'b')
				d = BESTval;
			else
				d = currentPosition[0] - '0';

			//get  best moves in pMove
			minimax_score(board, properties[4], d, 1, &pMove, MIN_VALUE, MAX_VALUE, 0);

			//print moves
			printMovesList(pMove);
			return 0;
		}

		else if (strcmp(token, cmmd15) == 0)
			//get_score
		{
			int score = 0;

			//parsing the depth d
			int d;
			cMove *m;
			char *currentPosition = NULL;
			token = strtok(NULL, " ");
			currentPosition = token;
			if (currentPosition[0] == 'b')
				d = BESTval;
			else
				d = currentPosition[0] - '0';

			//parsing the move m
			token = strtok(NULL, "&");
			currentPosition = token;
			m = (cMove *)isLegalMove(currentPosition, board, pMove);


			//get  best moves in pMove
			score = makeMove_ComputeScore_Undo(board, m, properties[4], d, 1, MIN_VALUE, MAX_VALUE, 0);

			//print score
			if (!properties[1])
				printf("%d",score);
			return 0;
		}

		else if (strcmp(token, cmmd16) == 0)
			//save
		{
			token = strtok(NULL, "&");
			char *currentPosition = token;
			Save(board, currentPosition);

			return 0;
		}

	}
	if (!properties[1])
		printf(ILLEGAL_COMMAND);
	return 0;

}


/*return NULL if the move is illegal, and a pointer to the move otherwise*/
cMove* isLegalMove(char *token, board_t board, cMove *allPossibleMoves)
{
	char *currentPosition = token;
	char x_coor, i_coor;
	int y_coor, j_coor;
	int promoteNeeded = 0;
	int promoteTo = 0;
	char tool;
	Coord source, dest;
	int difference;


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


	allPossibleMoves = movesByPieceType(board, source);

	//check if the move equals one of the possible moves:
	while (allPossibleMoves != NULL)
	{
		difference = 0;
		if (!isEqualCoordinates(source, allPossibleMoves->src) || !isEqualCoordinates(dest, allPossibleMoves->dst) || promoteTo != allPossibleMoves->promote)
			difference = 1;
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
			if (GetContentOfCoord(board, coord) == EMPTY || IsEnemy(GetContentOfCoord(board, coord), properties[4])) //isWhite(GetContentOfCoord(board, coord)) == 1 - properties[3]) 
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



/*DEBUG*/
#ifndef DAMKA
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
#endif


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


int Console_Main(board_t board)
{

	//srand(time(NULL));
	
	int parseResult = 0;
	
	/*char* board[BOARD_SIZE];
	char row0[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row1[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row2[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row3[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row4[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row5[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row6[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char row7[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	board[0] = row0;
	board[1] = row1;
	board[2] = row2;
	board[3] = row3;
	board[4] = row4;
	board[5] = row5;
	board[6] = row6;
	board[7] = row7;*/

	board_t brd = board; //= NULL; TODO: experiement
	char *input = NULL;
	cMove *computerMove = NULL;
	cMove *allPossibleMoves = NULL, *tmp;

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
			if (properties[3] = BLACK_PLAYER)//if computer's color is white
				minimax_score(brd, WHITE_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, 0);
			else
				minimax_score(brd, BLACK_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, 0);

			//print computer's move
			if (computerMove != NULL)
			{
				if (!properties[1])
					printf("Computer: move ");
				//				printMove(computerMove);
				makeMove(brd, computerMove);
				free(computerMove);
				//print board
				print_board(brd);
			}


			//change player
			properties[4] = -properties[4];
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
			if (properties[4] == WHITE_PLAYER)
				printf(WHT_ENTER_YOUR_MOVE);
			else
				printf(BLK_ENTER_YOUR_MOVE);


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
			properties[4] = -properties[4];
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

