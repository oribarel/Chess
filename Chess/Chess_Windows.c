#include "Chess_Windows.h"



/* Misc Properties*/
int gameMode = PVC_MODE;
int nextPlayer = WHITE_PLAYER;
int AI_Difficulty = 1;
int playerColor = WHITE_PLAYER;

int intQuit = 0;
static int newgamepress = 0;



////Global Menu pointers
//ControlComponent *ccp_MainWindow = NULL;
//ControlComponent *ccp_PlayerSelectionWindow = NULL;
//ControlComponent *ccp_AI_SettingsWindow = NULL;
//ControlComponent *ccp_nowVisible = NULL;

/* Logic Board */
board_t pBoard = NULL; //Comes from chessprog main

/* Gui Board */
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

/* current Screen: */


SDL_Rect create1024x768Rect()
{
	SDL_Rect Rect1024x768 = createSDL_Rect(SCREEN_W, SCREEN_H, 0, 0);
	return Rect1024x768;
}

int passTheBoard(board_t board)
{
	pBoard = board;
	return 1;
}

int createMainMenu(Menu *mainMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns)
{

	/* Rects */
	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect newGameRect = createSDL_Rect(200, 75, 100, 350);
	SDL_Rect loadGameRect = createSDL_Rect(200, 75, 100, 450);
	SDL_Rect quitGameRect = createSDL_Rect(200, 75, 100, 550);


	createMenu(mainMenu, Rect1024x768, rgbMenuBlue, MAIN_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, newGameRect, uploadPicture("NewGameButton.bmp"), showPlayerSelectionMenu, NEW_GAME_BUTTON);
	createButton(ccbs + 1, btns + 1, loadGameRect, uploadPicture("LoadGameButton.bmp"), showLoadGameMenu, LOAD_GAME_BUTTON);
	createButton(ccbs + 2, btns + 2, quitGameRect, uploadPicture("QuitGameButton.bmp"), QuitGame, QUIT_GAME_BUTTON);

	/* Make the Panel*/
	panelMaker(ccps, panel, Rect1024x768, rgbMenuBlue);
	/*ccps->pnl = panel;
	ccps->btn = NULL;
	ccps->lbl = NULL;
	ccps->next = NULL;
	ccps->rect = Rect1024x768;
	ccps->pnl->children = NULL;
	ccps->pnl->rgb_triplet = rgbRed;*/

	addButtonToPanel(ccps, ccbs);
	addButtonToPanel(ccps, ccbs + 1);
	addButtonToPanel(ccps, ccbs + 2);

	/* Add the Panels To the Menu*/
	addPanelToMenu(mainMenu, ccps, 1);

	return 1;
}

int createPlayerSelectionMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns)
{
	//TODO: board isn't attached.

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;


	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wGameModeSetting, hGameModeSetting , wSide, 9);
	SDL_Rect togglesPanel = createSDL_Rect(wGameModeSetting, 300, wSide, 9 + 250);
	SDL_Rect continueOrPlayPanel = createSDL_Rect(wGameModeSetting, 150, wSide, 9 + 550);
	SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	/* Button Rects */
	SDL_Rect returnToMainMenuButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect gameModeButton = createSDL_Rect(wGameModeSetting - 2 * wSide, hGameModeSetting/2 - 2 * hSide, wSide, hSide);
	SDL_Rect nextPlayerButton = createSDL_Rect(80, 80, wGameModeSetting / 2 - 40, hGameModeSetting / 2 );
	SDL_Rect ContinueOrPlayButton = createSDL_Rect(wGameModeSetting - 2 * hSide, hGameModeSetting - 4 * hSide, wSide, 2 * hSide);

	createMenu(playerSelectionMenu, Rect1024x768, rgbMenuBlue, PLAYER_SELECTION_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, returnToMainMenuButton, uploadPicture("ReturnToMainMenu.bmp"), showMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccbs + 1, btns + 1, gameModeButton, uploadPicture("PVC.bmp"), playerSelectionMenu_toggleGameMode, GAME_MODE_TOGGLE_BUTTON);
	createButton(ccbs + 2, btns + 2, nextPlayerButton, uploadPicture("NextPlayerWhite.bmp"), playerSelectionMenu_toggleNextPlayer, NEXT_PLAYER_TOGGLE_BUTTON);
	createButton(ccbs + 3, btns + 3, ContinueOrPlayButton, uploadPicture("AI_Settings.bmp"), showAI_SettingsMenu, PLAYER_SELECTION_CONTINUE_OR_START);

	/* Make the Panels*/
	panelMaker(ccps, panel, returnsPanel, rgbRed);
	panelMaker(ccps + 1, panel + 1, togglesPanel, rgbOrange);
	panelMaker(ccps + 2, panel + 2, continueOrPlayPanel, rgbGreen);


	addButtonToPanel(ccps, ccbs);
	addButtonToPanel(ccps + 1, ccbs + 1);
	addButtonToPanel(ccps + 1, ccbs + 2);
	addButtonToPanel(ccps + 2, ccbs + 3);

	/* Add the Panels To the Menu*/
	addPanelToMenu(playerSelectionMenu, ccps, 1);
	addPanelToMenu(playerSelectionMenu, ccps + 1, 2);
	addPanelToMenu(playerSelectionMenu, ccps + 2, 3);

	return 1;
}

int createAI_SettingsMenu(Menu *AI_SettingsMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns)
{
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
	//SDL_Rect boardPanel = createSDL_Rect(wBoardPanel, hBoardPanel, wSide + wTogglePanels, hSide); //wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide

	/* Button Rects*/
	SDL_Rect backToMainMenuButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect backToPlayerSelectionButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	SDL_Rect difficultyButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect playerColorButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	SDL_Rect startGameButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);

	createMenu(AI_SettingsMenu, Rect1024x768, rgbMenuBlue,AI_SETTINGS_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, backToMainMenuButton, uploadPicture("ReturnToMainMenu.bmp"), showMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccbs + 1, btns + 1, backToPlayerSelectionButton, uploadPicture("Back.bmp"), showPlayerSelectionMenu, BACK_TO_PLAYER_SELECTION_BUTTON);
	createButton(ccbs + 2, btns + 2, difficultyButton, uploadPicture("1Diff.bmp"), AI_settingsMenu_toggleDifficulty, AI_DIFF_TOGGLE_BUTTON);
	createButton(ccbs + 3, btns + 3, playerColorButton, uploadPicture("PlayerIsWhite.bmp"), AI_settingsMenu_togglePlayerColor, AI_PLAYER_COLOR_TOGGLE_BUTTON);
	createButton(ccbs + 4, btns + 4, startGameButton, uploadPicture("StartGame.bmp"), showGamePlayMenu, START_GAME_BUTTON);

	/* Make the Panels*/
	panelMaker(ccps, panel, returnsPanel, rgbRed);
	panelMaker(ccps + 1, panel + 1, togglesPanel, rgbOrange);
	panelMaker(ccps + 2, panel + 2, playPanel, rgbGreen);

	addButtonToPanel(ccps, ccbs);
	addButtonToPanel(ccps, ccbs + 1);
	addButtonToPanel(ccps + 1, ccbs + 2);
	addButtonToPanel(ccps + 1, ccbs + 3);
	addButtonToPanel(ccps + 2, ccbs + 4);

	/* Add the Panels To the Menu*/
	addPanelToMenu(AI_SettingsMenu, ccps, 1);
	addPanelToMenu(AI_SettingsMenu, ccps + 1, 2);
	addPanelToMenu(AI_SettingsMenu, ccps + 2, 3);

	return 1;

}

int createGameMenu(Menu *GamePlayMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns)
{
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
	SDL_Rect emptyPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 250);
	SDL_Rect saveAndQuitPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 500);
	

	/* Button Rects*/
	SDL_Rect backToMainMenuButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect saveGameButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect quitButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	createMenu(GamePlayMenu, Rect1024x768, rgbMenuBlue, GAME_PLAY_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, backToMainMenuButton, uploadPicture("ReturnToMainMenu.bmp"), GamePlayMenu_endGameAndShowMainMenu, END_GAME_AND_RETURN_TO_MAIN_MENU_BUTTON);
	createButton(ccbs + 1, btns + 1, quitButton, uploadPicture("QuitGameButton_inGame.bmp"), QuitGame, QUIT_GAME_BUTTON);
	createButton(ccbs + 2, btns + 2, saveGameButton, uploadPicture("saveGame.bmp"), GamePlayMenu_SaveGame, SAVE_GAME_BUTTON);

	/* Make the Panels*/
	panelMaker(ccps, panel, returnsPanel, rgbRed);
	panelMaker(ccps + 1, panel + 1, emptyPanel, rgbOrange);
	panelMaker(ccps + 2, panel + 2, saveAndQuitPanel, rgbGreen);

	addButtonToPanel(ccps, ccbs);
	addButtonToPanel(ccps + 2, ccbs + 1);
	addButtonToPanel(ccps + 2, ccbs + 2);

	/* Add the Panels To the Menu*/
	addPanelToMenu(GamePlayMenu, ccps, 1);
	addPanelToMenu(GamePlayMenu, ccps + 1, 2);
	addPanelToMenu(GamePlayMenu, ccps + 2, 3);

	return 1;
}


int showMenu(Window *window, Menu *menu)
{
	/*
	1. Need to attach the menu to the window.
	2. Need to paint the rect of the menu (menuBlue).
	3. Need to paint each of the rects of the panels, and
	4. Inside each panel, paint the buttons.
	*/
	
	//window->shownMenu = menu;
	addMenuToWindow(chessWindow, menu);
	
	if (menu->identifier == PLAYER_SELECTION_MENU || menu->identifier == AI_SETTINGS_MENU || menu->identifier == GAME_PLAY_MENU)
	{
		updateGUIBoard(menu);
	}



	if (menu->panel_1 != NULL)
	{
		if (drawPanelToMenu(menu->panel_1) == 0 || drawButtonsOfPanel(menu->panel_1) == 0)
			return 0;
	}
	if (menu->panel_2 != NULL)
	{
		if (drawPanelToMenu(menu->panel_2) == 0 || drawButtonsOfPanel(menu->panel_2) == 0)
			return 0;
	}
	if (menu->panel_3 != NULL)
	{
		if (drawPanelToMenu(menu->panel_3) == 0 || drawButtonsOfPanel(menu->panel_3) == 0)
			return 0;
	}
	if (menu->panel_4 != NULL)
	{
		if (drawPanelToMenu(menu->panel_4) == 0 || drawButtonsOfPanel(menu->panel_4) == 0)
			return 0;
	}
	return 1;
}

/* This also adds each of the squares to the panel! */
int createGUIBoard(board_g gBoard, ControlComponent *ccp_BoardSetting, board_t board)
{
	
	Coord crd;
	eTool type;
	int player;
	btnFunc toolFunc;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			ControlComponent *ccb = &(guiBoard[crd.i_coord][crd.j_coord]);
			
			ccb->lbl = NULL;
			ccb->next = NULL;
			ccb->pnl = NULL;
			ccb->rect = createSDL_RectForBoardSquare(crd);

			
			type = getInitialTypeOfCoord(crd);
			player = getInitialPlayerOfCoord(crd);
			toolFunc = playerSelectionMenu_toggleTool;
			//guiBoard[crd.i_coord][crd.j_coord] = createSquareByToolType(ccp_BoardSetting, crd, type, player, toolFunc); // Allocates the square, and also adds is to the ccp
			createSquareByToolType(ccp_BoardSetting, &(guiBoard[crd.i_coord][crd.j_coord]), crd, type, player, toolFunc);
		}
	}
	return 1;
}


/*
	This function only updates the function of the GUIBoard
	for PLAYER_SELECTION_MENU: toggle pieces function.
	for AI_SETTINGS_MENU: builds a guiBoard from board
	for GAME_MENU: game mode function
	*/
int updateGUIBoard( Menu *menu)
{
	ControlComponent *ccb;
	Coord crd;
	eTool type;
	int player, identfier = menu->identifier;
	btnFunc toolFunc; //int(*toolFunc)(Window *, struct controlComponent *);

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			if (identfier == PLAYER_SELECTION_MENU)// && !windowsCreated[1])
			{
				toolFunc = playerSelectionMenu_toggleTool;
				//type = getInitialTypeOfCoord(crd);
				//player = getInitialPlayerOfCoord(crd);
				//ccb = createSquareByToolType(window, ccp, crd, type, player, toolFunc); // Allocates the square, and also adds is to the ccp 
				//ControlComponent *ccp, ControlComponent *ccb, Coord crd, eTool type, int player, btnFunc f
			}
			//else if (identfier == PLAYER_SELECTION_MENU)// && windowsCreated[1])
			//{
			//	type = GetContentOfCoord(pBoard, crd);
			//	player = getColor(pBoard, crd);
			//	toolFunc = playerSelectionMenu_toggleTool;
			//	// Doesn't allocate the square, only adds it to the ccp, it has a picture, crd and everything so no need to set that.
			//	ccb = &(guiBoard[crd.i_coord][crd.j_coord]);
			//	ccb->btn->f = toolFunc;
			//	ccb->rect = createSDL_RectForBoardSquare(crd);
			//}
			else if (identfier == AI_SETTINGS_MENU)
			{
				//type = GetContentOfCoord(pBoard, crd);
				//player = getColor(pBoard, crd);
				//toolFunc = AI_SettingsMenu_toggleTool;
				//// Doesn't allocate the square, only adds it to the ccp, it has a picture, crd and everything so no need to set that.
				//ccb = &(guiBoard[crd.i_coord][crd.j_coord]);
				//ccb->btn->f = toolFunc;
				//ccb->rect = createSDL_RectForBoardSquare(crd);
				////addButtonToPanel(ccp, ccb, window);
				toolFunc = AI_SettingsMenu_toggleTool;
				
			}
			else
			{
				toolFunc = (btnFunc)getGameFunctionOfTool(GetContentOfCoord(pBoard,crd)); //TODO: intimdating cast was (int(*)(Window *, struct controlComponent *))
				
			}
			guiBoard[crd.i_coord][crd.j_coord].btn->f = toolFunc;

			//guiBoard[i][j] = *ccb;
		}
	}
	return 1;
}

btnFunc getGameFunctionOfTool(char tool)
{
	if (tool == WHITE_P || tool == BLACK_P)
		return pawnClick;
	if (tool == WHITE_N || tool == BLACK_N)
		return knightClick;
	if (tool == WHITE_B || tool == BLACK_B)
		return bishopClick;
	if (tool == WHITE_R || tool == BLACK_R)
		return rookClick;
	if (tool == WHITE_Q || tool == BLACK_Q)
		return queenClick;
	if (tool == WHITE_K || tool == BLACK_K)
		return kingClick;
	return nullFunction;
}

int pawnClick(struct menu *menu, struct controlComponent *ccb)
{
	Coord crd = ccb->btn->crd;
	printf("You have clicked a %s pawn.\n", islower(GetContentOfCoord(pBoard, crd)) ? "white" : "black");
	return 1;
}

int knightClick(struct menu *menu, struct controlComponent *ccb)
{
	return 1;
}

int bishopClick(struct menu *menu, struct controlComponent *ccb)
{
	return 1;
}

int rookClick(struct menu *menu, struct controlComponent *ccb)
{
	return 1;
}

int queenClick(struct menu *menu, struct controlComponent *ccb)
{
	return 1;
}

int kingClick(struct menu *menu, struct controlComponent *ccb)
{
	return 1;
}


int playerSelectionMenu_toggleTool(struct menu *menu, struct controlComponent *ccb)
{
	Coord crd = ccb->btn->crd;
	eTool type;
	char tool = GetContentOfCoord(pBoard, crd);

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

	setSlotInBoard(pBoard, crd, tool);
	int player = NO_PLAYER;
	if (tool != EMPTY)
		player = (islower(tool) ? WHITE_PLAYER : BLACK_PLAYER);

	SDL_FreeSurface(ccb->btn->pic);
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type));

	if (playerSelectionMenu_updateContinueOrPlayButton(chessWindow) == 0)
	{
		quit();
		return 0;
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	return 1;
}

int playerSelectionMenu_toggleGameMode(Menu *menu, struct controlComponent *ccb)
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
	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	if (playerSelectionMenu_updateContinueOrPlayButton(chessWindow) == 0)
		return 0;

	return 1;
}

/* updates this button's function and picture (not global gameMode) */
int playerSelectionMenu_updateContinueOrPlayButton(Window *window)
{
	if (pMenu_PlayerSelection == NULL)
	{
		return 0;
	}

	ControlComponent *ccb_continueOrPlay;
	ccb_continueOrPlay = pMenu_PlayerSelection->panel_3->pnl->children; /* Should point to continueOrPlay Button*/

	if (!isValidBoardInitialization(pBoard))
	{
		ccb_continueOrPlay->btn->f = nullFunction;
		SDL_FreeSurface(ccb_continueOrPlay->btn->pic);
		ccb_continueOrPlay->btn->pic = uploadPicture(gameMode == PVP_MODE ? "StartGameInactive.bmp" : "AI_SettingsInactive.bmp");
	}
	else
	{
		if (gameMode == PVP_MODE)
		{
			ccb_continueOrPlay->btn->f = showGamePlayMenu;
			SDL_FreeSurface(ccb_continueOrPlay->btn->pic);
			ccb_continueOrPlay->btn->pic = uploadPicture("StartGame.bmp");
		}
		else
		{
			ccb_continueOrPlay->btn->f = showAI_SettingsMenu;
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

int playerSelectionMenu_toggleNextPlayer(Menu *menu, struct controlComponent *ccb)
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
	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}





int AI_settingsMenu_toggleDifficulty(Menu *menu, struct controlComponent *ccb)
{
	AI_Difficulty = mod(AI_Difficulty + 1, 5);
	SDL_FreeSurface(ccb->btn->pic);
	ccb->btn->pic = uploadPicture(getDifficultyPicName());

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int AI_settingsMenu_togglePlayerColor(Menu *menu, struct controlComponent *ccb)
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

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int AI_SettingsMenu_toggleTool(struct menu *menu, struct controlComponent *ccb)
{
	Coord crd = ccb->btn->crd;
	eTool type;
	char tool = GetContentOfCoord(pBoard, crd);

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

	setSlotInBoard(pBoard, crd, tool);
	int player = NO_PLAYER;
	if (tool != EMPTY)
		player = (islower(tool) ? WHITE_PLAYER : BLACK_PLAYER);

	SDL_FreeSurface(ccb->btn->pic);
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type));

	if (AI_Settings_updatePlayButton(chessWindow) == 0)
	{
		quit();
		return 0;
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	return 1;
}

int AI_Settings_updatePlayButton(Window *window)
{
	if (pMenu_AI_settings == NULL)
	{
		return 0;
	}

	ControlComponent *ccb_Play;
	ccb_Play = pMenu_AI_settings->panel_3->pnl->children; /* Should point to Play Button*/

	if (!isValidBoardInitialization(pBoard))
	{
		ccb_Play->btn->f = nullFunction;
		SDL_FreeSurface(ccb_Play->btn->pic);
		ccb_Play->btn->pic = uploadPicture("StartGameInactive.bmp");
	}
	else
	{
		ccb_Play->btn->f = showGamePlayMenu;
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


int GamePlayMenu_endGameAndShowMainMenu(struct menu *menu, struct controlComponent *ccb)
{
	return 0;
}

int GamePlayMenu_SaveGame(struct menu *menu, struct controlComponent *ccb)
{
	return 0;
}


/* shared with main menu and game play menu */
int QuitGame(struct menu *menu, struct controlComponent *ccb)
{
	return 0;
}






/* If Menu has n sub-panels then this function should be ran on it with i_next from 0 to n-1. */
void blitMenu(ControlComponent *ccpParent, int i_next, Window *window)
{
	ControlComponent *curr = ccpParent->pnl->children;

	while (i_next-- > 0)
		curr = curr->next;

	Uint32 color = getColorOfPanel(window, curr);
	if (SDL_FillRect(window->self, &(curr->rect), color) != 0)
	{
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		quit();

	}

	ControlComponent *ccbCurr = curr->pnl->children;
	while (ccbCurr != NULL)
	{
		if (ccbCurr->btn != NULL)
		{
			if (SDL_BlitSurface(ccbCurr->btn->pic, NULL, window->self, &(ccbCurr->rect)) != 0)
			{
				SDL_FreeSurface(ccbCurr->btn->pic);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				quit();

			}
		}
		ccbCurr = ccbCurr->next;
	}
}




int buttonPressHandler(Window *window, SDL_Event e)
{
	Menu *menu = window->shownMenu;
	ControlComponent *currChild;

	if (menu->panel_1 != NULL)
	{
		currChild = menu->panel_1->pnl->children;
		
		while (currChild != NULL)
		{
			if (currChild->btn != NULL)
			{

				if (pressIfNeeded(menu, currChild, e) == 1)
				{
					return 1;
				}

			}
			currChild = currChild->next;
		}
	}
	if (menu->panel_2 != NULL)
	{
		
		currChild = menu->panel_2->pnl->children;
		while (currChild != NULL)
		{
			if (currChild->btn != NULL)
			{

				if (pressIfNeeded(menu, currChild, e) == 1)
				{
					return 1;
				}

			}
			currChild = currChild->next;
		}
	}
	if (menu->panel_3 != NULL)
	{
		
		currChild = menu->panel_3->pnl->children;
		while (currChild != NULL)
		{
			if (currChild->btn != NULL)
			{

				if (pressIfNeeded(menu, currChild, e) == 1)
				{
					return 1;
				}

			}
			currChild = currChild->next;
		}
	}
	if (menu->panel_4 != NULL)
	{
		
		currChild = menu->panel_4->pnl->children;
		while (currChild != NULL)
		{
			if (currChild->btn != NULL)
			{

				if (pressIfNeeded(menu, currChild, e) == 1)
				{
					return 1;
				}

			}
			currChild = currChild->next;
		}
	}
	return 0;

	//int pressNeeded = 1;
	//while (currChild != NULL)
	//{
	//	if (currChild->pnl != NULL) //if the currChild is a ccp
	//	{
	//		tmpChild = currChild->pnl->children;
	//		while (tmpChild != NULL)
	//		{
	//			if (tmpChild->btn != NULL)
	//			{
	//				pressNeeded = buttonPress(window, tmpChild, e);
	//				if (pressNeeded == 1)
	//				{
	//					return 1;
	//				}
	//			}
	//			tmpChild = tmpChild->next;
	//		}
	//	}
	//	else if (currChild->btn != NULL) // if the currChild is a ccb
	//	{
	//		pressNeeded = buttonPress(window, currChild, e);
	//		if (pressNeeded == 0)
	//		{
	//			return 1;
	//		}
	//	}
	//	currChild = currChild->next;
	//}

}

/* Executes pressing of the button if and only if e indicates that button was pressed */
int pressIfNeeded(Menu *menu, struct controlComponent *ccb, SDL_Event e)
{
	if (isPressInsideButton(e, ccb))
	{
		ccb->btn->f(menu, ccb);
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


int panelMaker(ControlComponent *ccp, Panel *pnl, SDL_Rect rect, RGB color)
{
	ccp->pnl = pnl;
	ccp->btn = NULL;
	ccp->lbl = NULL;
	ccp->next = NULL;
	ccp->rect = rect;
	ccp->pnl->children = NULL;
	ccp->pnl->rgb_triplet = color;

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

int showMainMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	showMenu(chessWindow, pMenu_Main);
	return 0;
}

int showPlayerSelectionMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	showMenu(chessWindow, pMenu_PlayerSelection);
	return 0;
}

int showAI_SettingsMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	showMenu(chessWindow, pMenu_AI_settings);
	return 0;
}

int showGamePlayMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	showMenu(chessWindow, pMenu_Game);
	return 0;
}

int showLoadGameMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	return 0;
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

