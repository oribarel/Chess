#include "Chess_Windows.h"



/* Misc Properties*/
int gameMode = PVC_MODE;
int nextPlayer = WHITE_PLAYER;
int AI_Difficulty = 1;
int playerColor = WHITE_PLAYER;

int windowsCreated[6] = { 0, 0, 0, 0, 0, 0 };
/*
0 - Main Menu
1 - Player Selection
2 - AI Settings Menu
3 - Game Manu
4 - Save Game Menu
5 - Load Game Menu
*/

static int intQuit = 0;
static int newgamepress = 0;

/* Global windows pointers */
ControlComponent *ccp_MainWindow = NULL;
ControlComponent *ccp_PlayerSelectionWindow = NULL;
ControlComponent *ccp_AI_SettingsWindow = NULL;

/* Board */
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

/* RGB Colors */
RGB rgbMenuBlue;
RGB rgbBlack;
RGB rgbWhite;
RGB rgbRed;
RGB rgbOrange;
RGB rgbGreen;
RGB rgbPurple;


SDL_Rect create1024x768Rect()
{
	SDL_Rect Rect1024x768 = createSDL_Rect(SCREEN_W, SCREEN_H, 0, 0);
	return Rect1024x768;
}

/* The main function of the GUI aspect of the chess game.*/
int CreateMainWindow(Window *window, board_t passedBoard)
{
	board = passedBoard;

	/* Initialize Colors */

	rgbMenuBlue = createRGB(0, 51, 102);
	rgbBlack = createRGB(0, 0, 0);
	rgbWhite = createRGB(255, 255, 255);
	rgbRed = createRGB(153, 0, 0);
	rgbOrange = createRGB(255, 128, 0);
	rgbGreen = createRGB(0, 153, 0);
	rgbPurple = createRGB(127, 0, 255);

	/* Initialize board */

	guiBoard[0] = guiBoard_col0;
	guiBoard[1] = guiBoard_col1;
	guiBoard[2] = guiBoard_col2;
	guiBoard[3] = guiBoard_col3;
	guiBoard[4] = guiBoard_col4;
	guiBoard[5] = guiBoard_col5;
	guiBoard[6] = guiBoard_col6;
	guiBoard[7] = guiBoard_col7;

	/* Rects */


	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect mainWindowButton = createSDL_Rect(200, 75, 100, 350);


	ccp_MainWindow = createPanel(Rect1024x768, rgbMenuBlue);
	ControlComponent *ccb_NewGame = createButton(mainWindowButton, uploadPicture("NewGameButton.bmp"), createPlayerSelectionWindow, 'n');

	mainWindowButton.y += 95;
	ControlComponent *ccb_LoadGame = createButton(mainWindowButton, uploadPicture("LoadGameButton.bmp"), LoadGame, 'l');

	mainWindowButton.y += 95;
	ControlComponent *ccb_Quit = createButton(mainWindowButton, uploadPicture("QuitGameButton.bmp"), quitGame, 'q');

	freeBoardAndTree(window); //TODO: NOTICE! this is not implemented <-----

	addPanelToWindow(window, ccp_MainWindow);
	windowsCreated[0] = 1;

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
			// add right click https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlmousebuttonevent.html?
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

	//TODO: return to main menu button.

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;


	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect gameModePanel = createSDL_Rect(wGameModeSetting, 250, wSide, 9 + 150);
	SDL_Rect nextPlayerPanel = createSDL_Rect(wGameModeSetting, 150, wSide, 9 + 400);
	SDL_Rect continueOrPlayPanel = createSDL_Rect(wGameModeSetting, 150, wSide, 9 + 550);
	SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	/* Button Rects */
	SDL_Rect versusButtonPvC = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide); // TODO: <---- change this name 

	SDL_Rect nextPlayerWhiteButton = createSDL_Rect(80, 80, 10, 45);

	SDL_Rect ContinueOrPlayButton = createSDL_Rect(wGameModeSetting - 2 * hSide, hGameModeSetting - 4 * hSide, wSide, 2 * hSide);


	/* Controls */
	/* 1. Panels*/
	ccp_PlayerSelectionWindow = createPanel(Rect1024x768, rgbMenuBlue);

	ControlComponent *ccp_GameMode = createPanel(gameModePanel, rgbRed);
	ControlComponent *ccp_NextPlayer = createPanel(nextPlayerPanel, rgbOrange);
	ControlComponent *ccp_ContinueOrPlay = createPanel(continueOrPlayPanel, rgbGreen);
	ControlComponent *ccp_BoardSetting = createPanel(boardSettingPanel, rgbPurple);

	/* 2. Buttons*/
	ControlComponent *ccb_PvC = createButton(versusButtonPvC, uploadPicture("PVC.bmp"), playerSelectionMenu_toggleGameMode, 'c');
	ControlComponent *ccb_NextPlayerWhite = createButton(nextPlayerWhiteButton, uploadPicture("NextPlayerWhite.bmp"), playerSelectionMenu_toggleNextPlayer, 'w');
	ControlComponent *ccb_ContinueOrPlay = createButton(ContinueOrPlayButton, uploadPicture("AI_Settings.bmp"), createAI_SettingsWindow, 'g');

	/* Board */
	freeBoardAndTree(window); //TODO: NOTICE! this is not implemented <-----

	addPanelToWindow(window, ccp_PlayerSelectionWindow);
	windowsCreated[1] = 1;

	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_GameMode, window);
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_NextPlayer, window);
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_ContinueOrPlay, window);
	addPanelToPanel(ccp_PlayerSelectionWindow, ccp_BoardSetting, window);

	addButtonToPanel(ccp_GameMode, ccb_PvC, window);

	addButtonToPanel(ccp_NextPlayer, ccb_NextPlayerWhite, window);

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

int createAI_SettingsWindow(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	//TODO: implement 2 relevant toggles.

	int wSide = 12;
	int hSide = 9;
	int wBoardPanel = 800;
	int hBoardPanel = 750;
	int wTogglePanels = SCREEN_W - wBoardPanel - 2 * wSide;
	int hTogglePanels = (SCREEN_H - 2 * hSide) / 3;

	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9);
	SDL_Rect togglesPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 250);
	SDL_Rect playPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 500);
	SDL_Rect boardPanel = createSDL_Rect(wBoardPanel, hBoardPanel, wSide + wTogglePanels, hSide);

	/* Button Rects*/
	SDL_Rect backToMainMenuButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect backToPlayerSelectionButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	SDL_Rect difficultyButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect playerColorButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	SDL_Rect startGameButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);

	/* Controls */
	/* 1. Panels*/
	ccp_AI_SettingsWindow = createPanel(Rect1024x768, rgbMenuBlue);

	ControlComponent *ccp_Returns = createPanel(returnsPanel, rgbRed);
	ControlComponent *ccp_Toggles = createPanel(togglesPanel, rgbOrange);
	ControlComponent *ccp_Play = createPanel(playPanel, rgbGreen);
	ControlComponent *ccp_Board = createPanel(boardPanel, rgbPurple);

	/* 2. Buttons*/
	ControlComponent *ccb_returnToMainMenu = createButton(backToMainMenuButton, uploadPicture("ReturnToMainMenu.bmp"), nullFunction, 'M');
	ControlComponent *ccb_returnToPlayerSelectionMenu = createButton(backToPlayerSelectionButton, uploadPicture("Back.bmp"), nullFunction, 'P');

	ControlComponent *ccb_toggleDifficulty = createButton(difficultyButton, uploadPicture(getDifficultyPicName()), AI_settingsMenu_toggleDifficulty, AI_DIFF_TOGGLE_BUTTON);
	ControlComponent *ccb_togglePlayerColor = createButton(playerColorButton, uploadPicture(getPlayerColorPicName()), AI_settingsMenu_togglePlayerColor, AI_PLAYER_COLOR_TOGGLE_BUTTON);

	ControlComponent *ccb_play = createButton(startGameButton, uploadPicture("StartGame.bmp"), nullFunction, 'g');

	/* Board */
	freeBoardAndTree(window); //TODO: NOTICE! this is not implemented <-----

	addPanelToWindow(window, ccp_AI_SettingsWindow);
	windowsCreated[2] = 1;

	addPanelToPanel(ccp_AI_SettingsWindow, ccp_Returns, window);
	addPanelToPanel(ccp_AI_SettingsWindow, ccp_Toggles, window);
	addPanelToPanel(ccp_AI_SettingsWindow, ccp_Play, window);
	addPanelToPanel(ccp_AI_SettingsWindow, ccp_Board, window);

	addButtonToPanel(ccp_Returns, ccb_returnToMainMenu, window);
	addButtonToPanel(ccp_Returns, ccb_returnToPlayerSelectionMenu, window);

	addButtonToPanel(ccp_Toggles, ccb_toggleDifficulty, window);
	addButtonToPanel(ccp_Toggles, ccb_togglePlayerColor, window);

	addButtonToPanel(ccp_Play, ccb_play, window);
	createGuiBoard(window, ccp_Board, AI_SETTINGS_MENU);

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
				buttonPressHandler(window, ccp_AI_SettingsWindow, e);
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
	for AI_SETTINGS_MENU: builds a guiBoard from board
	for GAME_MENU: game mode function
	*/
int createGuiBoard(Window *window, ControlComponent *ccp, int windowType)
{
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
			if (windowType == PLAYER_SELECTION_MENU)
			{
				type = getInitialTypeOfCoord(crd);
				player = getInitialPlayerOfCoord(crd);
				toolFunc = playerSelectionMenu_toggleTool;
				ccb = createSquareByToolType(window, ccp, crd, type, player, toolFunc); // Allocates the square, and also adds is to the ccp
			}
			else if (windowType == AI_SETTINGS_MENU)
			{
				type = GetContentOfCoord(board, crd);
				player = getColor(board, crd);
				toolFunc = AI_SettingsMenu_toggleTool;
				// Doesn't allocate the square, only adds it to the ccp, it has a picture, crd and everything so no need to set that.
				ccb = &(guiBoard[crd.i_coord][crd.j_coord]);
				ccb->btn->f = toolFunc;
				addButtonToPanel(ccp, ccb, window);

			}
			else
			{
				toolFunc = (int(*)(Window *, struct controlComponent *)) getGameModeFuncion(crd); //TODO: intimdating cast
			}


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

	if (playerSelectionMenu_updateContinueOrPlayButton(window) == 0)
	{
		quit();
		return 0;
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	return 1;
}



int playerSelectionMenu_toggleGameMode(Window *window, struct controlComponent *ccb)
{
	if (gameMode == PVP_MODE)
	{
		/* Change to PVC mode: */
		gameMode = PVC_MODE;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PVC.bmp");
	}
	else
	{
		/* Change to PVP mode: */
		gameMode = PVP_MODE;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PVP.bmp");
	}
	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	if (playerSelectionMenu_updateContinueOrPlayButton(window) == 0)
		return 0;

	return 1;
}

/* updates this button's function and picture (not global gameMode) */
int playerSelectionMenu_updateContinueOrPlayButton(Window *window)
{
	if (ccp_PlayerSelectionWindow == NULL)
	{
		return 0;
	}

	ControlComponent *ccb_continueOrPlay;
	ccb_continueOrPlay = ccp_PlayerSelectionWindow->pnl->children->next->next->pnl->children; /* Should point to continueOrPlay Button*/

	if (!isValidBoardInitialization(board))
	{
		ccb_continueOrPlay->btn->f = nullFunction;
		SDL_FreeSurface(ccb_continueOrPlay->btn->pic);
		ccb_continueOrPlay->btn->pic = uploadPicture(gameMode == PVP_MODE ? "StartGameInactive.bmp" : "AI_SettingsInactive.bmp");
	}
	else
	{
		if (gameMode == PVP_MODE)
		{
			ccb_continueOrPlay->btn->f = CreateGameWindow;
			SDL_FreeSurface(ccb_continueOrPlay->btn->pic);
			ccb_continueOrPlay->btn->pic = uploadPicture("StartGame.bmp");
		}
		else
		{
			ccb_continueOrPlay->btn->f = createAI_SettingsWindow;
			SDL_FreeSurface(ccb_continueOrPlay->btn->pic);
			ccb_continueOrPlay->btn->pic = uploadPicture("AI_Settings.bmp");
		}
	}

	if (SDL_BlitSurface(ccb_continueOrPlay->btn->pic, NULL, window->self, &(ccb_continueOrPlay->rect)) != 0)
	{
		SDL_FreeSurface(ccb_continueOrPlay->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int AI_Settings_updatePlayButton(Window *window)
{
	if (ccp_AI_SettingsWindow == NULL)
	{
		return 0;
	}

	ControlComponent *ccb_Play;
	ccb_Play = ccp_AI_SettingsWindow->pnl->children->next->next->pnl->children; /* Should point to Play Button*/

	if (!isValidBoardInitialization(board))
	{
		ccb_Play->btn->f = nullFunction;
		SDL_FreeSurface(ccb_Play->btn->pic);
		ccb_Play->btn->pic = uploadPicture("StartGameInactive.bmp");
	}
	else
	{
		ccb_Play->btn->f = CreateGameWindow;
		SDL_FreeSurface(ccb_Play->btn->pic);
		ccb_Play->btn->pic = uploadPicture("StartGame.bmp"); //TODO: maybe unnecessary allocation too many times. also in twin function.
	}

	if (SDL_BlitSurface(ccb_Play->btn->pic, NULL, window->self, &(ccb_Play->rect)) != 0)
	{
		SDL_FreeSurface(ccb_Play->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int playerSelectionMenu_toggleNextPlayer(Window *window, struct controlComponent *ccb)
{
	if (nextPlayer == WHITE_PLAYER)
	{
		/* Change to Black Player: */
		nextPlayer = BLACK_PLAYER;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("NextPlayerBlack.bmp");
	}
	else
	{
		/* Change to White Player: */
		nextPlayer = WHITE_PLAYER;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("NextPlayerWhite.bmp");
	}
	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}


int AI_settingsMenu_toggleDifficulty(Window *window, struct controlComponent *ccb)
{
	AI_Difficulty = mod(AI_Difficulty + 1, 5);
	SDL_FreeSurface(ccb->btn->pic);
	ccb->btn->pic = uploadPicture(getDifficultyPicName());

	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int AI_settingsMenu_togglePlayerColor(Window *window, struct controlComponent *ccb)
{
	if (playerColor == WHITE_PLAYER)
	{
		playerColor = BLACK_PLAYER;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PlayerIsBlack.bmp");
	}
	else
	{
		playerColor = WHITE_PLAYER;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PlayerIsWhite.bmp");
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int AI_SettingsMenu_toggleTool(Window *window, struct controlComponent *ccb)
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

	if (AI_Settings_updatePlayButton(window) == 0)
	{
		quit();
		return 0;
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	return 1;
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
	if ((e.button.x > ccb->rect.x) &&
		(e.button.x < ccb->rect.x + ccb->rect.w) &&
		(e.button.y > ccb->rect.y) &&
		(e.button.y < ccb->rect.y + ccb->rect.h))
		return 1;
	else
		return 0;
}

/* AI_Difficulty is between 0 and 4 inclusive */
const char *getDifficultyPicName(void)
{
	if (AI_Difficulty == 0)
		return "BestDiff.bmp";
	else if (AI_Difficulty == 1)
		return "1Diff.bmp";
	else if (AI_Difficulty == 2)
		return "2Diff.bmp";
	else if (AI_Difficulty == 3)
		return "3Diff.bmp";
	else
		return "4Diff.bmp";
}

const char *getPlayerColorPicName()
{
	if (playerColor == WHITE_PLAYER)
		return "PlayerIsWhite.bmp";
	else
		return "PlayerIsBlack.bmp";
}

/* Returns 1 iff it is legal to start a game with the board setting */
/* TODO: move this function to Chess_Logic.c */
int isValidBoardInitialization(board_t board)
{
	char types[] = { BLACK_P, BLACK_N, BLACK_B, BLACK_R, BLACK_Q, BLACK_K, WHITE_P, WHITE_N, WHITE_B, WHITE_R, WHITE_Q, WHITE_K };
	int result = 1;

	for (int i = 0; i < 12; i++)
	{
		result &= NotTooManyOfType(board, types[i]);
	}

	if (CountToolsOfType(board, WHITE_K) < 1 || CountToolsOfType(board, BLACK_K) < 1)
		result = 0;

	return result;
}



















int freeBoardAndTree(Window *window)
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

int CreateGameWindow(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
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



int quitGame(Window *window, ControlComponent *buttonWhichPressCalledThisFunction)
{
	return 0;
}

