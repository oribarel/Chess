#include "Chess_Windows.h"

static int intQuit = 0;
static int newgamepress = 0;

SDL_Rect create1024x768Rect()
{
	SDL_Rect Rect1024x768 = createSDL_Rect(SCREEN_W, SCREEN_H, 0, 0);
	return Rect1024x768;
}

/* The main function of the GUI aspect of the chess game.*/
int CreateMainWindow(Window *window)
{
	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect mainWindowButton = createSDL_Rect(200, 75, 100, 350);
	RGB whiteColorRGB = createRGB(0, 51, 102);

	

	ControlComponent *ccp_MainWindow = createPanel(Rect1024x768, whiteColorRGB);
	ControlComponent *ccb_NewGame = createButton(mainWindowButton, uploadPicture("NewGameButton.bmp"), createPlayerSelectionWindow);

	mainWindowButton.y += 95;
	ControlComponent *ccb_LoadGame = createButton(mainWindowButton, uploadPicture("LoadGameButton.bmp"), LoadGame);

	mainWindowButton.y += 95;
	ControlComponent *ccb_Quit = createButton(mainWindowButton, uploadPicture("QuitGameButton.bmp"), quitGame);

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



int createPlayerSelectionWindow(Window *window)
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
	ControlComponent *ccb_PvC = createButton(versusButtonPvC, uploadPicture("PVC.bmp"),  setGameModePVC);
	ControlComponent *ccb_PVP = createButton(versusButtonPvP, uploadPicture("PVP.bmp"),  setGameModePVP);

	ControlComponent *ccb_NextPlayerWhite = createButton(nextPlayerWhiteButton, uploadPicture("NextPlayerWhite.bmp"),  setNextPlayerWhite);
	ControlComponent *ccb_NextPlayerBlack = createButton(nextPlayerBlackButton, uploadPicture("NextPlayerBlack.bmp"),  setNextPlayerBlack);

	ControlComponent *ccb_ContinueOrPlay = createButton(ContinueOrPlayButton, uploadPicture("PVP.bmp"),  StartGameOrCreateAI_Settings);

	/* Board */
	ccb_board = create_ccb_BoardList();

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
				break;
			}
		}
	}
	return 0;
}



int setGameModePVP(Window *window)
{
	return 0;
}

int setGameModePVC(Window *window)
{
	return 0;
}

int setNextPlayerWhite(Window *window)
{
	return 0;
}

int setNextPlayerBlack(Window *window)
{
	return 0;
}

int StartGameOrCreateAI_Settings(Window *window)
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

int LoadGame(Window *window)
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
					if (pressNeeded == 0)
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
		ccb->btn->f(window);
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

int quitGame(Window *window)
{
	return 0;
}

