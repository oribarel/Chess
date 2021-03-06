


#include "Console_Main.h"

//extern int properties[6];
//extern Coord WhiteKing;
//extern Coord BlackKing;

cMove *pMove;

/* Sets the slot on the board, pay attention: there is a similar function with different arguments. */

void setSlot(board_t board, char *horiz, char *vert, char type)
{
	Coord coord;
	char originalContent;
	int i = (int)*horiz - 97;
	int j = (int)strtof(vert, NULL) - 1;
	coord.i_coord = i; coord.j_coord = j;
	if (isTool(type) || type == EMPTY)
	{

		if (isInBoard(coord))
		{
			originalContent = board[i][j];
			board[i][j] = type;
			if (!NotTooManyOfType(board, type))
			{
				board[i][j] = originalContent;
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
	//intialize board
	board_t board = (board_t)calloc(BOARD_SIZE, sizeof(char*));
	for (int i = 0; i < BOARD_SIZE; i++)
		board[i] = (char*)calloc(BOARD_SIZE, sizeof(char));
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			board[i][j] = EMPTY;


	//print_board(board);

	//set random number of tools per player
	int numberOfWhitePawns = random_at_most(BOARD_SIZE - 1) + 1, numberOfBlackPawns = random_at_most(BOARD_SIZE - 1) + 1;
	int numberOfWhiteRooks = random_at_most(2 - 1) + 1, numberOfBlackRooks = random_at_most(2 - 1) + 1;
	int numberOfWhiteBishops = random_at_most(2 - 1) + 1, numberOfBlackBishops = random_at_most(2 - 1) + 1;
	int numberOfWhiteKnights = random_at_most(2 - 1) + 1, numberOfBlackKnights = random_at_most(2 - 1) + 1;
	int numberOfTooleToSet = numberOfWhitePawns + numberOfBlackPawns + numberOfWhiteRooks + numberOfBlackRooks + numberOfWhiteBishops + numberOfBlackBishops + numberOfWhiteKnights + numberOfBlackKnights + 1 + 1 + 2;
	int places[(BOARD_SIZE*BOARD_SIZE)];
	int numberOfFreePlaces = BOARD_SIZE*BOARD_SIZE;
	int index = 0;
	board_t brd;
	brd = board;
	//initialize places array
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			places[index] = i * 10 + j;
			index++;
		}
	assert(index == numberOfFreePlaces);

	//set white tools:
	for (int i = 0; i < numberOfWhitePawns; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		//printf("%u\n", x);
		//printf("%u\n", y);
		//printf("%u\n\n\n", places[place]);
		Coord crd;
		//int toolRand = random_at_most(5);
		char tool = WHITE_P;
		crd.i_coord = x;	crd.j_coord = y;
		int toolRand = random_at_most(4);
		if (y == 7)
		{
			if (toolRand == 0)
				tool = WHITE_B;
			else if (toolRand == 1)
				tool = WHITE_N;
			else if (toolRand == 2)
				tool = WHITE_R;
			else
				tool = WHITE_Q;
		}
		setSlotInBoard(brd, crd, tool);
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;

	}

	for (int i = 0; i < numberOfWhiteRooks; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = WHITE_R;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}


	for (int i = 0; i < numberOfWhiteBishops; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = WHITE_B;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}

	for (int i = 0; i < numberOfWhiteKnights; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = WHITE_N;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}

	for (int i = 0; i < 1; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = WHITE_Q;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}


	for (int i = 0; i < 1; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = WHITE_K;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}





	//set black tools:
	for (int i = 0; i < numberOfBlackPawns; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		//printf("%u\n", x);
		//printf("%u\n", y);
		//printf("%u\n\n\n", places[place]);
		Coord crd;
		//int toolRand = random_at_most(5);
		char tool = BLACK_P;
		crd.i_coord = x;	crd.j_coord = y;
		int toolRand = random_at_most(4);
		if (y == 0)
		{
			if (toolRand == 0)
				tool = BLACK_B;
			else if (toolRand == 1)
				tool = BLACK_N;
			else if (toolRand == 2)
				tool = BLACK_R;
			else
				tool = BLACK_Q;
		}
		setSlotInBoard(brd, crd, tool);
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;

	}

	for (int i = 0; i < numberOfBlackRooks; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = BLACK_R;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}


	for (int i = 0; i < numberOfBlackBishops; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = BLACK_B;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}

	for (int i = 0; i < numberOfBlackKnights; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = BLACK_N;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}

	for (int i = 0; i < 1; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = BLACK_Q;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}


	for (int i = 0; i < 1; i++)
	{
		int place = random_at_most(numberOfFreePlaces);
		int x = places[place] % 10;
		int y = places[place] / 10;
		char tool = BLACK_K;
		Coord crd;
		crd.i_coord = x;	crd.j_coord = y;
		setSlotInBoard(brd, crd, tool);
		brd[x][y] = tool;
		places[place] = places[numberOfFreePlaces - 1];
		numberOfFreePlaces--;
		numberOfTooleToSet--;
	}
	assert(numberOfTooleToSet == 0);

	print_board(brd);
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
				{
					properties[2] = 1;
					printf("%s", PLAYER_VS_AI_GAME_MODE);
				}
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
			if (strcmp(token, BEST) == 0)
			{
				properties[2] = BESTval;//0 stands for best
				return 0;
			}
			token = strtok(NULL, " ");
			long depth = strtol(token, &token, BOARD_SIZE);
			if (depth > 0 && depth < 5)
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
			if (properties[5] == 1)
			{
				properties[3] = WHITE_PLAYER;
				properties[2] = 1;				
			}
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
			//free(line);
			properties[1] = 1;
			return 0;
		}
		else if (strcmp(token, cmmd11) == 0)
			//start
		{
			Coord tmpCrd;
			if (CountToolsOfType(board, WHITE_K) != 1 || CountToolsOfType(board, BLACK_K) != 1)
			{
				if (!properties[1])
					printf(WRONG_BOARD_INITIALIZATION);
				return 0;
			}
			else
			{
				properties[0] = 0;//change from setting state to game state				
			}



			//update kings' coordinates
			for (int j = BOARD_SIZE - 1; j >= 0; j--){
				for (int i = 0; i < BOARD_SIZE; i++){
					tmpCrd.i_coord = i;
					tmpCrd.j_coord = j;
					if (GetContentOfCoord(board, tmpCrd) == WHITE_K)
					{
						WhiteKing.i_coord = i;
						WhiteKing.j_coord = j;
					}
					if (GetContentOfCoord(board, tmpCrd) == BLACK_K)
					{
						BlackKing.i_coord = i;
						BlackKing.j_coord = j;
					}
				}
			}







			/*checing check mate or tie before starting the game*/
			int curScore = score(board, properties[4]);

			if (properties[5] == 1)
			{
				if (curScore == MATE_WIN_LOSE)//no legal move for the computer (computer lost)
				{
					printWinner(-properties[4]);//print player wins;
					properties[1] = 1;
					return 0;
				}

				else if (curScore == TIE_SCORE)//no legal move for the computer or for the player and king unthreatened (tie)
				{
					printf(TIE);//print tie;
					properties[1] = 1;
					return 0;
				}
				else if (KingUnderThreat(board, properties[4]))
					printf(CHECK);


				/*checing check mate or tie before starting the game*/
			}


				if (curScore == MATE_WIN_SCORE || KingUnderThreat(board, -properties[4]))//no legal move for the computer (computer lost)
				{
					printWinner(properties[4]);//print player wins;
					properties[1] = 1;
					return 0;
				}
			
			
			



			
			if (DEBUG)
				print_board(board);

			return 0;


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
			freeMovesList(legalMove);
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
			freeMovesList(pMove);
			pMove = NULL;
			return 0;
		}




		else if (strcmp(token, cmmd14) == 0)
			//get_best_moves
		{
			//int maxScore = MIN_VALUE;
			int best = 0;

			//parsing the depth d
			int d, tmpD = properties[2];
			char *currentPosition = NULL;
			token = strtok(NULL, "&");
			currentPosition = token;
			if (currentPosition[0] == 'b')
			{
				best = 1;
				d = GetBestDepth(board, properties[4]);
			}
			else
				d = currentPosition[0] - '0';

			//get  best moves by pMove
			properties[2] = d;
			minimax_score(board, properties[4], d, 1, &pMove, MIN_VALUE, MAX_VALUE, best);

			//print moves
			printMovesList(pMove);
			freeMovesList(pMove);
			pMove = NULL;
			properties[2] = tmpD;
			return 0;
		}

		else if (strcmp(token, cmmd15) == 0)
			//get_score
		{
			int score = 0;
			int best = 0;
			//parsing the depth d
			int d, tmpD = properties[2];
			cMove *m;
			char *currentPosition = NULL;
			token = strtok(NULL, " ");
			currentPosition = token;
			if (currentPosition[0] == 'b')
			{
				best = 1;
				d = GetBestDepth(board, properties[4]);
			}
			else
				d = currentPosition[0] - '0';
			//parsing the move m
			token = strtok(NULL, " ");
			token = strtok(NULL, "&");
			currentPosition = token;
			m = (cMove *)isLegalMove(currentPosition, board, pMove);


			//get  best moves in pMove
			properties[2] = d;
			score = makeMove_ComputeScore_Undo(board, m, properties[4], d, 1, MIN_VALUE, MAX_VALUE, best);
			freeMovesList(m);

			//print score
			if (!properties[1])
				printf("%d\n", score);
			properties[2] = tmpD;
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
	//int promoteNeeded = 0;
	int promoteTo = 0;
	//char tool;
	Coord source, dest;
	int difference;
	cMove* tmp = NULL;


	//fill coordinates
	x_coor = *(currentPosition + 1);
	currentPosition += 3;
	y_coor = (int)strtol(currentPosition, &currentPosition, 10);
	graphicCoordToRealCoord(&source, x_coor, y_coor);
	currentPosition += 6;
	i_coor = *currentPosition;
	currentPosition += 2;
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
		{
			return allPossibleMoves;
		}
		tmp = allPossibleMoves;
		allPossibleMoves = allPossibleMoves->next;
		free(tmp);
	}
	freeMovesList(allPossibleMoves);
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

cMove* chooseMoveRandonly(board_t brd)
{
	int counter = 0;
	cMove *tmp = pMove;
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


/*prints to screen who the winner is*/
int printWinner(int player)
{
	if (properties[1])
		return 0;
	if (player == WHITE_PLAYER)
		printf(WHT_MATE_DCLR);
	else
		printf(BLK_MATE_DCLR);
	return 0;
}




int Console_Main(board_t board)
{

	//srand(time(NULL));

	int parseResult = 10;

	

	board_t brd = board; 
	char *input = NULL;
	cMove *computerMove = NULL;
	

	init_board(brd);

	

	print_board(brd);



	/* One loop for both settings and game */
	while (!properties[1])//while not quit
	{
		if (!properties[1] && properties[0])
			printf(ENTER_SETTINGS);
		if (properties[5] == 2 && !properties[0] && (properties[4] != properties[3]))//if in AI mode, in game state and it is the computer's turn
		{
			int curScore = score(brd, properties[4]);
			computerMove = NULL;
			if (curScore == MATE_WIN_LOSE && !properties[1])//no legal move for the computer (computer lost)
			{
				printWinner(-properties[4] && !properties[1]);//print player wins;
				properties[1] = 1;
			}
			else if (curScore == TIE_SCORE && !properties[1])//no legal move for the computer and for the player (tie)
			{
				printf(TIE);//print tie;
				properties[1] = 1;
			}
			else if (KingUnderThreat(board, properties[4]) && !properties[1])
				printf(CHECK);



			//run MINIMAX
			int oldDepth = properties[2];
			int best = oldDepth == 0 ? 1 : 0;
			if (properties[3] == BLACK_PLAYER)//if computer's color is white
			{
				properties[2] = getDepth(brd, WHITE_PLAYER);
				minimax_score(brd, WHITE_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, best);
			}
			else
			{
				properties[2] = getDepth(brd, BLACK_PLAYER);
				minimax_score(brd, BLACK_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, best);
			}
			properties[2] = oldDepth;
			//print computer's move
			if (computerMove != NULL)
			{
				if (!properties[1])
					printf("Computer: move ");
				printMove(computerMove);
				makeMove(brd, computerMove);
				freeMovesList(computerMove);
				computerMove = NULL;
				//print board
				print_board(brd);
			}


			//change player
			properties[4] = -properties[4];
			parseResult = 10;
			//curScore = score(brd, properties[4]);			
			//if (curScore == MATE_WIN_LOSE && !properties[1])//if no legal move for the player (player lost - MATE!)
			//{
			//	printWinner(-properties[4]);//print computer wins;
			//	properties[1] = 1;

			//}
			//else if (curScore == TIE_SCORE && !properties[1])//no legal move for the computer or for the player and (tie)
			//{
			//	printf(TIE);//print tie;
			//	properties[1] = 1;
			//}
			//else if (KingUnderThreat(board, properties[4]) && !properties[1])
			//	printf(CHECK);
			continue;
		}
		

		if (properties[1])//if quit
			continue;


		if (parseResult == 10 && !properties[1])
		{
			if (score(brd, properties[4]) == MATE_WIN_LOSE)//no legal move for the other player (other plyer lost - MATE!)
			{
				printWinner(-properties[4]);//print current player wins;
				properties[1] = 1;
			}
			else if (score(brd, properties[4]) == TIE_SCORE)//no legal move for both players(tie)
			{
				printf(TIE);//print tie;
				properties[1] = 1;
			}
			else if (KingUnderThreat(board, properties[4]))
				printf(CHECK);

		}


		if (!properties[0])
		{

			if (properties[1]) //if quit
				continue;
			if (properties[4] == WHITE_PLAYER)
				printf(WHT_ENTER_YOUR_MOVE);
			else
				printf(BLK_ENTER_YOUR_MOVE);


		}

		input = getLine();

		if (!properties[1])//if not quit
			parseResult = Parse(input, brd);

		if (parseResult == 10 && !properties[1])// user has successfully inserted a valid move
		{
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
	exit(0);
	return 0;


	
}



int freeBoard(board_t brd)
{
	/*for (int i = 0; i < BOARD_SIZE; i++)
		free(brd[i]);
		free(brd);*/
	return 1;
}



































/**************************************************************
*************************** TEST ******************************
**************************************************************/

int Test()
{

	//srand(time(NULL));
	srand(198487);

	//int parseResult = 0;
	int iterations = 5;
	int printMode = 1;
	int COMPUTER_COLOR = BLACK_PLAYER;
	int record = 1;
	properties[0] = 0;
	//properties[3] = BLACK_PLAYER;	
	//difficulty
	properties[2] = 3;
	properties[5] = 2;
	properties[4] = WHITE_PLAYER;
	int iterationToRecord = 42;


	/**************************************************************
	*************************** TEST ******************************
	**************************************************************/



	board_t brd; //= NULL; 
	//char *input = NULL;
	cMove *computerMove = NULL;
	cMove *allPossibleMoves = NULL, *tmp;

	//init_board(brd);


	brd = GenerateRandomConfiguration();
	/*if (DEBUG) //tests
	{
	for (int i = 1; i < 12; i++)
	{
	if (i==9)
	brd = unitTest_movements(i);

	}
	}*/
	//printf("%c\n\n", brd[0][0]);
	print_board(brd);
	FILE *f;
	if (record)
	{
		f = fopen("RECORD RUN", "w");
		fprintf(f, "clear\n");
		//fprintf(f, "user_color black\n");
		//fprintf(f, "user_color White\n");

		fprintf(f, "game_mode 2\n");
		fprintf(f, "difficulty 2\n");
		/*for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
			if (brd[i][j] != EMPTY)
			{
			char x = 'a' + i;
			int y = j + 1;
			if (islower(brd[i][j]))
			fprintf(f, "set <%c,%d> %s %s\n", x, y, WHITE, ToolCharToName(brd[i][j]));
			else
			fprintf(f, "set <%c,%d> %s %s\n", x, y, BLACK, ToolCharToName(brd[i][j]));
			}
			fprintf(f, "start\n");*/
		/*char *s = (char*)calloc(20, sizeof(char));
		char s2[16] = "chess1.xml\n";
		for (int i = 0; i < 16; i++)
			s[i] = s2[i];
		Save(brd, s);*/
	}



	int curScore = 0;
	int countMoves = 0;




	/* One loop for both settings and game */
	while (!properties[1] && iterations > 0)//while not quit
	{
		if (iterations == iterationToRecord&&countMoves == 0)
		{
			record = 1;
			char *s = (char*)calloc(20, sizeof(char));
			char s2[16] = "chess3.xml\n";
			for (int i = 0; i < 16; i++)
				s[i] = s2[i];
			Save(brd, s);
			Record(brd, f);		
		}
		else
			record = 0;
		countMoves++;
		if (countMoves == 45) // MAIMUM NUMBER OF MOVES
		{
			countMoves = 0;
			properties[4] = COMPUTER_COLOR;
			freeBoard(brd);
			brd = GenerateRandomConfiguration();
			iterations--;
			SetDefaultProperties();
			continue;
		}
		if (!properties[1] && properties[0])
			printf(ENTER_SETTINGS);
		printf("%u\n", iterations);
		if (properties[5] == 2 && !properties[0] && (properties[4] != properties[3]))//if in AI mode, in game state and it is the computer's turn
		{
			curScore = score(brd, properties[4]);
			computerMove = NULL;
			if (curScore == MATE_WIN_LOSE)//no legal move for the computer (computer lost)
			{
				printWinner(-properties[4]);//print player wins;
				freeBoard(brd);
				brd = GenerateRandomConfiguration();
				properties[4] = COMPUTER_COLOR;
				iterations--;
				countMoves = 0;
				SetDefaultProperties();
				continue;
			}
			else if (curScore == TIE_SCORE)//no legal move for the computer or for the player and king unthreatened (tie)
			{
				printf(TIE);//print tie;
				freeBoard(brd);
				brd = GenerateRandomConfiguration();
				properties[4] = COMPUTER_COLOR;
				countMoves = 0;
				iterations--;
				SetDefaultProperties();
				continue;
			}
			else if (KingUnderThreat(brd, properties[4]))
				printf(CHECK);
			/**************************************************************
			*************************** TEST ******************************
			**************************************************************/


			//run MINIMAX
			int oldDepth = properties[2];
			int best = oldDepth == 0 ? 1 : 0;
			if (properties[3] == BLACK_PLAYER)//if computer's color is white
			{
				properties[2] = getDepth(brd, WHITE_PLAYER);
				minimax_score(brd, WHITE_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, best);
			}
			else
			{
				properties[2] = getDepth(brd, BLACK_PLAYER);
				minimax_score(brd, BLACK_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, best);
			}
			properties[2] = oldDepth;

			if (computerMove == NULL)
			{
				printf("computerMove==NULL\n");
				print_board(brd);
			}

			//print computer's move
			if (computerMove != NULL)
			{
				if (!properties[1])
					printf("Computer: move ");
				printMove(computerMove);
				makeMove(brd, computerMove);
				free(computerMove);
				//print board
				print_board(brd);
			}


			//change player

			//properties[4] = -properties[4];
		}
		else
		{
			curScore = score(brd, properties[4]);
			if ((curScore == MATE_WIN_LOSE))//if no legal move for the player (player lost - MATE!)
			{
				printWinner(-properties[4]);//print computer wins;
				freeBoard(brd);
				brd = GenerateRandomConfiguration();
				properties[4] = COMPUTER_COLOR;
				countMoves = 0;
				iterations--;
				SetDefaultProperties();
				continue;

			}
			else if (curScore == TIE_SCORE)//no legal move for the computer or for the player and (tie)
			{
				printf(TIE);//print tie;
				freeBoard(brd);
				brd = GenerateRandomConfiguration();
				properties[4] = COMPUTER_COLOR;
				countMoves = 0;
				iterations--;
				SetDefaultProperties();
				continue;
			}
			else if (KingUnderThreat(brd, properties[4]))
				printf(CHECK);
			//continue;

			if (properties[3] == WHITE_PLAYER)//if player is white
				allPossibleMoves = getMoves(brd, WHITE_PLAYER);
			else //if player is black
				allPossibleMoves = getMoves(brd, BLACK_PLAYER);
			pMove = allPossibleMoves;

			if (pMove != NULL)
				computerMove = chooseMoveRandonly(brd);
			else
			{
				curScore = score(brd, properties[4]);
				if ((curScore == MATE_WIN_LOSE))//if no legal move for the player (player lost - MATE!)
				{
					printWinner(-properties[4]);//print computer wins;
					brd = GenerateRandomConfiguration();
					properties[4] = COMPUTER_COLOR;
					countMoves = 0;
					iterations--;
					SetDefaultProperties();
					continue;

				}
				exit(1);
			}

			makeMove(brd, computerMove);
			if (printMode)
			{
				printf("player: move ");
				printMove(computerMove);
				if (record)
					printMoveToFile(f, computerMove);
				print_board(brd);
			}
			tmp = allPossibleMoves;
			while (allPossibleMoves != NULL)
			{
				allPossibleMoves = allPossibleMoves->next;
				free(tmp);
				tmp = allPossibleMoves;
			}
		}
		properties[4] = -properties[4];
	}



	//printf(ENTER_YOUR_MOVE);


	if (record)
	{
		fprintf(f, "quit\n");
		fclose(f);
	}

	return 0;
}




int Record(board_t brd, FILE *f)
{
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			if (brd[i][j] != EMPTY)
			{
				char x = 'a' + i;
				int y = j + 1;
				if (islower(brd[i][j]))
					fprintf(f, "set <%c,%d> %s %s\n", x, y, WHITE, ToolCharToName(brd[i][j]));
				else
					fprintf(f, "set <%c,%d> %s %s\n", x, y, BLACK, ToolCharToName(brd[i][j]));
			}
	fprintf(f, "start\n");
	return 0;
}

int SetDefaultProperties()
{
	properties[4] = WHITE_PLAYER;
	return 0;
}