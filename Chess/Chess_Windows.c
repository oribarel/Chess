#include "Chess_Windows.h"

static int intQuit = 0;
static int newgamepress = 0;

const char *str_purpose_NewGame = "NewGame";
const char *str_purpose_LoadGame = "LoadGame";
const char *str_purpose_QuitGame = "QuitGame";

const char *str_purpose_settings_NextPlayerW = "NextPlayerW";
const char *str_purpose_settings_NextPlayerB = "NextPlayerB";
const char *str_purpose_settings_PVP = "PVP";
const char *str_purpose_settings_PVC = "PVC";
const char *str_purpose_settings_continueOrPlay = "continueOrPlay";



board_t board;
ControlComponent *guiBoard[BOARD_SIZE]; // This array is a "row" of colums

ControlComponent guiBoard_col0[BOARD_SIZE];
ControlComponent guiBoard_col1[BOARD_SIZE];
ControlComponent guiBoard_col2[BOARD_SIZE];
ControlComponent guiBoard_col3[BOARD_SIZE];
ControlComponent guiBoard_col4[BOARD_SIZE];
ControlComponent guiBoard_col5[BOARD_SIZE];
ControlComponent guiBoard_col6[BOARD_SIZE];
ControlComponent guiBoard_col7[BOARD_SIZE];

SDL_Rect create1024x768Rect()
{
	SDL_Rect Rect1024x768 = createSDL_Rect(SCREEN_W, SCREEN_H, 0, 0);
	return Rect1024x768;
}

/* The main function of the GUI aspect of the chess game.*/
int CreateMainWindow(Window *window, board_t passedBoard)
{
	board = passedBoard;
	

	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect mainWindowButton = createSDL_Rect(200, 75, 100, 350);
	RGB whiteColorRGB = createRGB(0, 51, 102);

	
	ControlComponent *ccp_MainWindow = createPanel(Rect1024x768, whiteColorRGB);
	ControlComponent *ccb_NewGame = createButton(mainWindowButton, uploadPicture("NewGameButton.bmp"), createPlayerSelectionWindow, 'n');

	mainWindowButton.y += 95;
	ControlComponent *ccb_LoadGame = createButton(mainWindowButton, uploadPicture("LoadGameButton.bmp"), LoadGame, 'l');

	mainWindowButton.y += 95;
	ControlComponent *ccb_Quit = createButton(mainWindowButton, uploadPicture("QuitGameButton.bmp"), quitGame, 'q');

	freeBoardAndTree(window);
	addPanelToWindow(window, ccp_MainWindow);
	addButtonToPanel(ccp_MainWindow, ccb_NewGame, window);
	addButtonToPanel(ccp_MainWindow, ccb_LoadGame, window);
	addButtonToPanel(ccp_MainWindow, ccb_Quit, window);
	
	if (SDL_Flip(window->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	while (!intQuit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && intQuit == 0)
		{
			if (e.type == SDL_QUIT)
			{
				freeCCTree(window->panelChild);
				window->panelChild = NULL;
				SDL_FreeSurface(window->self);
				free(window);
				intQuit = 1;
				break;
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				buttonPressHandler(window, ccp_MainWindow, e);
				break;
			}
		}
	}
	return 0;
}


int createPlayerSelectionWindow(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;
	ControlComponent *ccb_board;

	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect(); 

	/* Panel Rects */
	SDL_Rect gameModePanel = createSDL_Rect(wGameModeSetting, 250, wSide, 9+150 );
	SDL_Rect nextPlayerPanel = createSDL_Rect(wGameModeSetting, 150 , wSide, 9+400);
	SDL_Rect continueOrPlayPanel = createSDL_Rect(wGameModeSetting, 150 , wSide, 9+550);
	SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);
	
	/* Button Rects */
	SDL_Rect versusButtonPvC = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect versusButtonPvP = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, 2*hSide + (hGameModeSetting - 4 * hSide)/2);

	SDL_Rect nextPlayerWhiteButton = createSDL_Rect(80, 80, 10 , 45);
	SDL_Rect nextPlayerBlackButton = createSDL_Rect(80, 80, 10 + 80 + 20, 45);

	SDL_Rect ContinueOrPlayButton = createSDL_Rect(wGameModeSetting - 2 * hSide, hGameModeSetting - 4 * hSide, wSide , 2 * hSide );

	/* RGB Colors*/
	RGB rgbMenuBlue = createRGB(0, 51, 102);
	RGB rgbBlack = createRGB(0, 0, 0);
	RGB rgbWhite = createRGB(255, 255, 255);

	/* Controls */
	/* 1. Panels*/
	ControlComponent *ccp_PlayerSelectionWindow = createPanel(Rect1024x768, rgbMenuBlue);

	ControlComponent *ccp_GameMode = createPanel(gameModePanel, createRGB(153, 0, 0));
	ControlComponent *ccp_NextPlayer = createPanel(nextPlayerPanel, createRGB(255, 128, 0));
	ControlComponent *ccp_ContinueOrPlay = createPanel(continueOrPlayPanel, createRGB(0, 153, 0));
	ControlComponent *ccp_BoardSetting = createPanel(boardSettingPanel, createRGB(127, 0, 255));

	/* 2. Buttons*/
	ControlComponent *ccb_PvC = createButton(versusButtonPvC, uploadPicture("PVC.bmp"),  setGameModePVC, 'c');
	ControlComponent *ccb_PVP = createButton(versusButtonPvP, uploadPicture("PVP.bmp"),  setGameModePVP, 'p');

	ControlComponent *ccb_NextPlayerWhite = createButton(nextPlayerWhiteButton, uploadPicture("NextPlayerWhite.bmp"),  setNextPlayerWhite,'w');
	ControlComponent *ccb_NextPlayerBlack = createButton(nextPlayerBlackButton, uploadPicture("NextPlayerBlack.bmp"),  setNextPlayerBlack, 'b');

	ControlComponent *ccb_ContinueOrPlay = createButton(ContinueOrPlayButton, uploadPicture("PVP.bmp"),  startGameOrCreateAI_Settings, 'g');

	/* Board */
	

	freeBoardAndTree(window);

	addPanelToWindow(window, ccp_PlayerSelectionWindow);
	
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_GameMode, window);
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_NextPlayer, window);
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_ContinueOrPlay, window);
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_BoardSetting, window);

	addButtonToPanel(ccp_GameMode, ccb_PvC, window);
	addButtonToPanel(ccp_GameMode, ccb_PVP, window);

	addButtonToPanel(ccp_NextPlayer, ccb_NextPlayerWhite, window);
	addButtonToPanel(ccp_NextPlayer, ccb_NextPlayerBlack, window);

	addButtonToPanel(ccp_ContinueOrPlay, ccb_ContinueOrPlay, window);

	createGuiBoard(window, ccp_BoardSetting, PLAYER_SELECTION_MENU);
	init_board(board);


	if (SDL_Flip(window->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	while (!intQuit)
	{

		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && intQuit == 0)
		{
			if (e.type == SDL_QUIT)
			{
				freeCCTree(window->panelChild);
				window->panelChild = NULL;
				SDL_FreeSurface(window->self);
				free(window);
				intQuit = 1;
				break;
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				buttonPressHandler(window, ccp_PlayerSelectionWindow, e);
				if (SDL_Flip(window->self) != 0)
				{
					printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
					quit();
					return 0;
				}
				break;
			}
		}
	}
	return 0;
}

/*	
	for PLAYER_SELECTION_MENU: toggle pieces function.
	for GAME_MENU: game mode function	
*/
int createGuiBoard(Window *window, ControlComponent *ccp, int windowType)
{
	guiBoard[0] = guiBoard_col0;
	guiBoard[1] = guiBoard_col1;
	guiBoard[2] = guiBoard_col2;
	guiBoard[3] = guiBoard_col3;
	guiBoard[4] = guiBoard_col4;
	guiBoard[5] = guiBoard_col5;
	guiBoard[6] = guiBoard_col6;
	guiBoard[7] = guiBoard_col7;

	ControlComponent *ccb;
	Coord crd;
	eTool type;
	int player;
	int(*toolFunc)(Window *, struct controlComponent *);

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			type = getInitialTypeOfCoord(crd);
			player = getInitialPlayerOfCoord(crd);
			if (windowType == PLAYER_SELECTION_MENU)
				toolFunc = playerSelectionMenu_toggleTool;
			else
				toolFunc = (int(*)(Window *, struct controlComponent *)) getGameModeFuncion(crd); //TODO: intimdating cast
			ccb = createSquareByToolType(window, ccp, crd, type, player, toolFunc); //also adds is to the ccp

			guiBoard[i][j] = *ccb;
		}
	}
	return 1;
}

/* Pre: coord is in board */
eTool getInitialTypeOfCoord(Coord crd)
{
	if (crd.j_coord > 1 && crd.j_coord < 6)
		return Empty;
	else if (crd.j_coord == 1 || crd.j_coord == 6)
		return Pawn;
	else
	{
		if (crd.i_coord == 0 || crd.i_coord == 7)
			return Rook;
		else if (crd.i_coord == 1 || crd.i_coord == 6)
			return Knight;
		else if (crd.i_coord == 2 || crd.i_coord == 5)
			return Bishop;
		else if (crd.i_coord == 3)
			return Queen;
		else
			return King;
	}
}

/* Pre: crd is in the board */
int getInitialPlayerOfCoord(Coord crd)
{
	if (crd.j_coord == 0 || crd.j_coord == 1)
		return WHITE_PLAYER;
	else if (crd.j_coord == 6 || crd.j_coord == 7)
		return BLACK_PLAYER;
	else
		return NO_PLAYER;
}

int(*getGameModeFuncion(Coord crd))(Window *, struct controlComponent *)
{
	return nullFunction;
}

int playerSelectionMenu_toggleTool(Window *window, struct controlComponent *ccb)
{
	Coord crd = ccb->btn->crd;
	eTool type;
	char tool = GetContentOfCoord(board, crd);

	char toggleArray[13] = { EMPTY, WHITE_P, WHITE_N, WHITE_B, WHITE_R, WHITE_Q, WHITE_K,
									BLACK_P, BLACK_N, BLACK_B, BLACK_R, BLACK_Q, BLACK_K };

	char eToolToggleArray[13] = { Empty, Pawn, Knight, Bishop, Rook, Queen, King,
										Pawn, Knight, Bishop, Rook, Queen, King };

	int i = 0;
	for (; i < 13; i++)
	{
		if (tool == toggleArray[i])
		{
			tool = toggleArray[mod(i + 1, 13)];
			break;
		}	
	}
	type = eToolToggleArray[mod(i + 1, 13)];

	setSlotInBoard(board, crd, tool);
	int player = NO_PLAYER;
	if (tool != EMPTY)
		player = (islower(tool) ? WHITE_PLAYER : BLACK_PLAYER);

	SDL_FreeSurface(ccb->btn->pic);
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type));
	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self , &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	return 1;
}

int setGameModePVP(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

int setGameModePVC(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

int setNextPlayerWhite(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

int setNextPlayerBlack(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

int startGameOrCreateAI_Settings(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

int RebuildSettingsMenu(Window *window, ControlComponent **second_settings_window, 
	ControlComponent **choose_color, ControlComponent **white, ControlComponent **black,
	ControlComponent **choose_difficulty, ControlComponent **one, ControlComponent **two,
	ControlComponent **three, ControlComponent **four, ControlComponent **best,
	ControlComponent **start_game)
{
	return 0;
}



/* difficulty should be 1 to 4 or 0 for best */
int setDifficulty(Window *window, int difficulty)
{
	return 0;
}

int LoadGame(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

int Load(Window *window, char *path)
{
	return 0;
}

int SetFromFileGui(char* path)
{
	return 0;
}

int loadGameNum(Window *window, int n)
{
	return 0;
}

int SaveGame(Window *window)
{
	return 0;
}

int SaveGUI(char* file_name)
{
	return 0;
}

int saveGameNum(Window *window, int n)
{
	return 0;
}

int BackFromSave(Window *window)
{
	return 0;
}

int inBoardBoundaries(Pixel px)
{
	return 0;
}

int colorByPixel(char** board, Pixel px)
{
	return 0;
}

int CreateGameWindow(Window *window)
{
	return 0;
}

int RebuildGameMenu(Window *window, ControlComponent **full_panel, ControlComponent **menu_panel, ControlComponent **restart, ControlComponent **save_game, ControlComponent **main_menu, ControlComponent **quit_b)
{
	return 0;
}

int CreateGame(Window *window, ControlComponent *panel_node, char** current_board)
{
	return 0; 
}

int SendErrorMessage(Window *window)
{
	return 0;
}

int ExecutePawnPromotion(Window *window)
{
	return 0;
}

int SetPromotionAs(Window *window, eTool type)
{
	return 0;
}

int restartGame(Window *window)
{
	return 0;
}

int buttonPressHandler(Window *window, ControlComponent *ccp, SDL_Event e)
{
	ControlComponent *currChild = ccp->pnl->children;
	ControlComponent *tmpChild;

	int pressNeeded = 1;

	while (currChild != NULL)
	{
		if (currChild->pnl != NULL) //if the currChild is a ccp
		{
			tmpChild = currChild->pnl->children;
			while (tmpChild != NULL)
			{
				if (tmpChild->btn != NULL)
				{
					pressNeeded = buttonPress(window, tmpChild, e);
					if (pressNeeded == 1)
					{
						return 1;
					}
				}
				tmpChild = tmpChild->next;
			}
		}
		else if (currChild->btn != NULL) // if the currChild is a ccb
		{
			pressNeeded = buttonPress(window, currChild, e);
			if (pressNeeded == 0)
			{
				return 1;
			}
		}
		currChild = currChild->next;
	}
	
	return 0;
}

/* Executes pressing of the button if and only if e indicates that button was pressed */
int buttonPress(Window *window, ControlComponent *ccb, SDL_Event e)
{
	if (isPressInsideButton(e, ccb))
	{
		ccb->btn->f(window, ccb);
		return 1;
	}
	return 0;
}

int isPressInsideButton(SDL_Event e, ControlComponent *ccb)
{
	if ((e.button.x > ccb->rect.x)					&&
		(e.button.x < ccb->rect.x + ccb->rect.w)	&&
		(e.button.y > ccb->rect.y)					&&
		(e.button.y < ccb->rect.y + ccb->rect.h))
		return 1;
	else
		return 0;
}

int freeBoardAndTree(Window *window)
{
	return 0;
}

int quitGame(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

