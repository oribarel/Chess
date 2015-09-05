#include "Chess_Windows.h"



/* Properties*/

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

//int gameMode = PVC_MODE;
//int nextPlayer = WHITE_PLAYER;
//int AI_Difficulty = 1;
//int playerColor = WHITE_PLAYER;

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
//what here?

/* In Game Logic: */
int playerTurnStages[5] = { WAITING, NONE_SELECTED, HIGHLIGHTED, MOVE_MADE, PROMOTE };
int  whitePlayerTurnStage = NONE_SELECTED, blackPlayerTurnStage = WAITING;
Coord selectedTool, highlightedSquares[BOARD_SIZE*BOARD_SIZE];

Coord src, dest;

/* Save/Load Game */
char selectedSlot = '0';





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

/* TODO: need to convert this */
int createSaveMenu(Menu *pMenu_Save, ControlComponent *ccp_SaveMenuCCPs, Panel *pnl_SaveMenuPanels, ControlComponent *ccb_SaveMenuCCBs, Button *btn_SaveMenuButtons)
{ /* This menu has a 'return to game', a 'quit', 'save' and 7 slots buttons. */
	
	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;
	
	/* Rects */
	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect returnsPanel = createSDL_Rect(wGameModeSetting, hGameModeSetting, wSide, 9);
	SDL_Rect slotsPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	SDL_Rect returnToMainMenu = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect returnToGameButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hGameModeSetting/2 + hSide);
	SDL_Rect slot_1 = createSDL_Rect(200, 75, 412, 100);
	SDL_Rect slot_2 = createSDL_Rect(200, 75, 412, 185);
	SDL_Rect slot_3 = createSDL_Rect(200, 75, 412, 270);
	SDL_Rect slot_4 = createSDL_Rect(200, 75, 412, 355);
	SDL_Rect slot_5 = createSDL_Rect(200, 75, 412, 440);
	SDL_Rect slot_6 = createSDL_Rect(200, 75, 412, 525);
	SDL_Rect slot_7 = createSDL_Rect(200, 75, 412, 610);
	SDL_Rect saveButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);


	createMenu(pMenu_Save, Rect1024x768, rgbMenuBlue, SAVE_MENU);

	/* Make the buttons */
	createButton(ccb_SaveMenuCCBs, btn_SaveMenuButtons, returnToMainMenu, uploadPicture("ReturnToMainMenu.bmp"), SaveMenu_ResetGameAndShowMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccb_SaveMenuCCBs + 1, btn_SaveMenuButtons + 1, slot_1, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '1');
	createButton(ccb_SaveMenuCCBs + 2, btn_SaveMenuButtons + 2, slot_2, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '2');
	createButton(ccb_SaveMenuCCBs + 3, btn_SaveMenuButtons + 3, slot_3, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '3');
	createButton(ccb_SaveMenuCCBs + 4, btn_SaveMenuButtons + 4, slot_4, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '4');
	createButton(ccb_SaveMenuCCBs + 5, btn_SaveMenuButtons + 5, slot_5, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '5');
	createButton(ccb_SaveMenuCCBs + 6, btn_SaveMenuButtons + 6, slot_6, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '6');
	createButton(ccb_SaveMenuCCBs + 7, btn_SaveMenuButtons + 7, slot_7, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '7');
	createButton(ccb_SaveMenuCCBs + 8, btn_SaveMenuButtons + 8, saveButton, uploadPicture("saveGame.bmp"), SaveMenu_saveGameSlot, SAVE_GAME_BUTTON);
	createButton(ccb_SaveMenuCCBs + 9, btn_SaveMenuButtons + 9, returnToGameButton, uploadPicture("ReturnToGame.bmp"), SaveMenu_ResetSaveMenuAndShowGamePlayMenu, BACK_TO_GAME);
	
	/* Make the Panels*/
	panelMaker(ccp_SaveMenuCCPs, pnl_SaveMenuPanels, returnsPanel, rgbRed);
	panelMaker(ccp_SaveMenuCCPs + 1, pnl_SaveMenuPanels + 1, slotsPanel, rgbPurple);

	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 1);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 2);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 3);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 4);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 5);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 6);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 7);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 8);
	addButtonToPanel(ccp_SaveMenuCCPs , ccb_SaveMenuCCBs + 9);

	/* Add the Panels To the Menu*/
	addPanelToMenu(pMenu_Save, ccp_SaveMenuCCPs, 1);
	addPanelToMenu(pMenu_Save, ccp_SaveMenuCCPs + 1, 2);
	addPanelToMenu(pMenu_Save, NULL, 3);
	addPanelToMenu(pMenu_Save, NULL, 4);

	return 1;
}

int createLoadMenu(Menu *pMenu_Load, ControlComponent *ccp_LoadMenuCCPs, Panel *pnl_LoadMenuPanels, ControlComponent *ccb_LoadMenuCCBs, Button *btn_LoadMenuButtons)
{
	/* This menu has a 'return to game', a 'quit', 'save' and 7 slots buttons. */

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;

	/* Rects */
	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect returnsPanel = createSDL_Rect(wGameModeSetting, hGameModeSetting, wSide, 9);
	SDL_Rect slotsPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	SDL_Rect returnToMainMenu = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	//SDL_Rect returnToGameButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hGameModeSetting / 2 + hSide);
	SDL_Rect slot_1 = createSDL_Rect(200, 75, 412, 100);
	SDL_Rect slot_2 = createSDL_Rect(200, 75, 412, 185);
	SDL_Rect slot_3 = createSDL_Rect(200, 75, 412, 270);
	SDL_Rect slot_4 = createSDL_Rect(200, 75, 412, 355);
	SDL_Rect slot_5 = createSDL_Rect(200, 75, 412, 440);
	SDL_Rect slot_6 = createSDL_Rect(200, 75, 412, 525);
	SDL_Rect slot_7 = createSDL_Rect(200, 75, 412, 610);
	SDL_Rect loadButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);


	createMenu(pMenu_Save, Rect1024x768, rgbMenuBlue, SAVE_MENU);

	/* Make the buttons */
	createButton(ccb_LoadMenuCCBs, btn_LoadMenuButtons, returnToMainMenu, uploadPicture("ReturnToMainMenu.bmp"), LoadMenu_Reset_LoadMenu_And_ShowMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccb_LoadMenuCCBs + 1, btn_LoadMenuButtons + 1, slot_1, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '1');
	createButton(ccb_LoadMenuCCBs + 2, btn_LoadMenuButtons + 2, slot_2, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '2');
	createButton(ccb_LoadMenuCCBs + 3, btn_LoadMenuButtons + 3, slot_3, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '3');
	createButton(ccb_LoadMenuCCBs + 4, btn_LoadMenuButtons + 4, slot_4, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '4');
	createButton(ccb_LoadMenuCCBs + 5, btn_LoadMenuButtons + 5, slot_5, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '5');
	createButton(ccb_LoadMenuCCBs + 6, btn_LoadMenuButtons + 6, slot_6, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '6');
	createButton(ccb_LoadMenuCCBs + 7, btn_LoadMenuButtons + 7, slot_7, uploadPicture("UnpressedGenericButton.bmp"), SaveMenu_selectSlot, '7');
	createButton(ccb_LoadMenuCCBs + 8, btn_LoadMenuButtons + 8, loadButton, uploadPicture("loadGame.bmp"), nullFunction, LOAD_GAME_BUTTON);
	/* Make the Panels*/
	panelMaker(ccp_LoadMenuCCPs, pnl_LoadMenuPanels, returnsPanel, rgbRed);
	panelMaker(ccp_LoadMenuCCPs + 1, pnl_LoadMenuPanels + 1, slotsPanel, rgbPurple);

	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 1);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 2);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 3);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 4);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 5);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 6);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 7);
	addButtonToPanel(ccp_LoadMenuCCPs + 1, ccb_LoadMenuCCBs + 8);

	/* Add the Panels To the Menu*/
	addPanelToMenu(pMenu_Load, ccp_LoadMenuCCPs, 1);
	addPanelToMenu(pMenu_Load, ccp_LoadMenuCCPs + 1, 2);
	addPanelToMenu(pMenu_Load, NULL, 3);
	addPanelToMenu(pMenu_Load, NULL, 4);

	return 1;
}

int createPlayerSelectionMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns)
{

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;


	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wGameModeSetting, hGameModeSetting, wSide, 9);
	SDL_Rect togglesPanel = createSDL_Rect(wGameModeSetting, 300, wSide, 9 + 250);
	SDL_Rect continueOrPlayPanel = createSDL_Rect(wGameModeSetting, 150, wSide, 9 + 550);
	SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	/* Button Rects */
	SDL_Rect returnToMainMenuButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect gameModeButton = createSDL_Rect(wGameModeSetting - 2 * wSide, hGameModeSetting / 2 - 2 * hSide, wSide, hSide);
	SDL_Rect nextPlayerButton = createSDL_Rect(80, 80, wGameModeSetting / 2 - 40, hGameModeSetting / 2);
	SDL_Rect ContinueOrPlayButton = createSDL_Rect(wGameModeSetting - 2 * hSide, hGameModeSetting - 4 * hSide, wSide, 2 * hSide);

	createMenu(playerSelectionMenu, Rect1024x768, rgbMenuBlue, PLAYER_SELECTION_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, returnToMainMenuButton, uploadPicture("ReturnToMainMenu.bmp"), showMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccbs + 1, btns + 1, gameModeButton, uploadPicture("PVP.bmp"), playerSelectionMenu_toggleGameMode, GAME_MODE_TOGGLE_BUTTON);
	createButton(ccbs + 2, btns + 2, nextPlayerButton, uploadPicture("NextPlayerWhite.bmp"), playerSelectionMenu_toggleNextPlayer, NEXT_PLAYER_TOGGLE_BUTTON);
	createButton(ccbs + 3, btns + 3, ContinueOrPlayButton, uploadPicture("StartGame.bmp"), showGamePlayMenu, PLAYER_SELECTION_CONTINUE_OR_START);

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

	/* Highlighted Squares  <--- this happens *here* only for sake of multi-functionality */
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
	{
		highlightedSquares[k].i_coord = -1;
		highlightedSquares[k].j_coord = -1;
	}
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

	createMenu(AI_SettingsMenu, Rect1024x768, rgbMenuBlue, AI_SETTINGS_MENU);

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

	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}


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
int createGUIBoard(board_g gBoard, ControlComponent *ccp_BoardSetting, Button *btn_BoardPanelButtons, board_t board)
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
			createSquareByToolType(ccp_BoardSetting, &(guiBoard[crd.i_coord][crd.j_coord]), (btn_BoardPanelButtons + i*BOARD_SIZE + j) , crd, type, player, toolFunc);
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
int updateGUIBoard(Menu *menu)
{
	ControlComponent *ccb;
	Coord crd;
	eTool type;
	int player, identfier = menu->identifier;
	btnFunc toolFunc;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;

			if (identfier == GAME_PLAY_MENU )
			{
				toolFunc = (btnFunc)getGameFunctionOfCoord(crd);
				if (properties[5] == PVC_MODE && properties[3] != properties[4])
					toolFunc = nullFunction;
			}
			else if (identfier == PLAYER_SELECTION_MENU)
				toolFunc = playerSelectionMenu_toggleTool;
			else if (identfier == AI_SETTINGS_MENU)
				toolFunc = AI_SettingsMenu_toggleTool;
			else
				toolFunc = nullFunction;

			guiBoard[crd.i_coord][crd.j_coord].btn->f = toolFunc;
		}
	}
	return 1;
}

int isOfPlayer(int player, char tool)
{
	if (player == WHITE_PLAYER && islower(tool) || player == BLACK_PLAYER && isupper(tool))
		return 1;
	return 0;
}

btnFunc getGameFunctionOfCoord(Coord crd)
{
	char tool = GetContentOfCoord(pBoard, crd);
	int stageInd;
	if (properties[5] == PVC_MODE) //case PVC mode
	{
		if (properties[3] == properties[4]) //case player's turn
		{
			stageInd = (properties[3] == WHITE_PLAYER) ? whitePlayerTurnStage : blackPlayerTurnStage;
			
			if (stageInd == NONE_SELECTED)
			{
				if (isOfPlayer(properties[4],tool))
				{
					return (btnFunc) getHighlightFunctionOfTool(tool);
				}
				else
				{
					return (btnFunc)nullFunction;
				}
			}
			else if (stageInd == HIGHLIGHTED)
			{
				if (isOfPlayer(properties[4], tool))
				{
					return changeHighlightedTool;
				}
				else if (isHighlighted(crd))
				{
					return gui_makeMove;
				}
				else
				{
					return (btnFunc)nullFunction;
				}
			}
		}
		else //case not player's turn.
		{
			return (btnFunc)nullFunction;
		}
	}
	else //case PVP mode
	{
		int player = properties[4], waiter = generateEnemyColor(player);
		stageInd = (properties[4] == WHITE_PLAYER) ? whitePlayerTurnStage : blackPlayerTurnStage;

		if (stageInd == NONE_SELECTED)
		{
			if (isOfPlayer(properties[4], tool))
			{
				return (btnFunc)getHighlightFunctionOfTool(tool);
			}
			else
			{
				return (btnFunc)nullFunction;
			}
		}
		else if (stageInd == HIGHLIGHTED)
		{
			if (isOfPlayer(properties[4], tool))
			{
				return changeHighlightedTool;
			}
			else if (isHighlighted(crd))
			{
				return gui_makeMove;
			}
			else
			{
				return (btnFunc)nullFunction;
			}
		}
	}
	return (btnFunc)nullFunction;
}

btnFunc getHighlightFunctionOfTool(char tool)
{
	if (tool == WHITE_P || tool == BLACK_P)
		return pawnHighlight;
	else if (tool == WHITE_N || tool == BLACK_N)
		return knightHighlight;
	else if (tool == WHITE_B || tool == BLACK_B)
		return bishopHighlight;
	else if (tool == WHITE_R || tool == BLACK_R)
		return rookHighlight;
	else if (tool == WHITE_Q || tool == BLACK_Q)
		return queenHighlight;
	else
		return kingHighlight;
}

int isHighlighted(Coord crd)
{
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE && isInBoard(highlightedSquares[k]); k++)
	{
		if (crd.i_coord == highlightedSquares[k].i_coord && crd.j_coord == highlightedSquares[k].j_coord)
			return 1;
	}
	return 0;
}

int updateGUIBoard_Vis(Menu *menu)
{
	Coord crd;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			int selectionStatus;
			/* If it is the selected Coord then SelectionStatus == SEL */
			if (properties[0] == 1)
				selectionStatus = NON_SEL;
			else if (selectedTool.i_coord == i && selectedTool.j_coord == j)
				selectionStatus = SEL;
			/* Else we go over the highlighted squares array and check for a match: */
			else
			{
				int inHighlightedSquares = 0;
				for (int k = 0; k < BOARD_SIZE*BOARD_SIZE && isInBoard(highlightedSquares[k]); k++)
					inHighlightedSquares |= (highlightedSquares[k].i_coord == i && highlightedSquares[k].j_coord == j);
				if (inHighlightedSquares)
					selectionStatus = ATT; /* If it is in the highlighted squares, then it is being attacked */
				else
					selectionStatus = NON_SEL;
			}



			crd.i_coord = i; crd.j_coord = j;
			guiBoard[i][j].btn->pic = uploadPicture(getPictureName_tools(crd, getColor(pBoard, crd),
				get_eToolFromType(GetContentOfCoord(pBoard, crd)), selectionStatus));
			
			if (SDL_BlitSurface(guiBoard[i][j].btn->pic, NULL, chessWindow->self, &(guiBoard[i][j].rect)) != 0)
			{
				SDL_FreeSurface(guiBoard[i][j].btn->pic);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				quit();
				return 0;
			}
		}
	}
	return 1;
}

eTool get_eToolFromType(char type)
{
	if (type == WHITE_P || type == BLACK_P)
		return Pawn;
	else if (type == WHITE_N || type == BLACK_N)
		return Knight;
	else if (type == WHITE_B || type == BLACK_B)
		return Bishop;
	else if (type == WHITE_R || type == BLACK_R)
		return Rook;
	else if (type == WHITE_Q || type == BLACK_Q)
		return Queen;
	else if (type == WHITE_K || type == BLACK_K)
		return King;
	else
		return Empty;
}


/* { WAITING, NONE_SELECTED, HIGHLIGHTED, MOVE_MADE, PROMOTE } */
int pawnHighlight(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = PawnMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);
	return 1;
}

int knightHighlight(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = KnightMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);
	return 1;
}

int bishopHighlight(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = BishopMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);
	return 1;
}

int rookHighlight(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = RookMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);
	return 1;
}

int queenHighlight(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = QueenMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);
	return 1;
}

int kingHighlight(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = KingMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);
	return 1;
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

int advanceTurnStage(int promotiveSituation)
{
	int player = properties[4], waiter = generateEnemyColor(player);
	int *playerStageInd = (properties[4] == WHITE_PLAYER) ? &whitePlayerTurnStage : &blackPlayerTurnStage;
	int *waiterStageInd = (properties[4] == WHITE_PLAYER) ? &blackPlayerTurnStage : &whitePlayerTurnStage;

	if (*playerStageInd == NONE_SELECTED)
		*playerStageInd = HIGHLIGHTED;
	
	else if (*playerStageInd == HIGHLIGHTED)
		*playerStageInd = MOVE_MADE;

	else if (*playerStageInd == PROMOTE)
	{
		*playerStageInd = WAITING;
		*waiterStageInd = NONE_SELECTED;
		properties[4] = waiter;
	}
	
	
	if (*playerStageInd == MOVE_MADE)
	{
		if (promotiveSituation)
			*playerStageInd = PROMOTE;
		else
		{
			*playerStageInd = WAITING;
			*waiterStageInd = NONE_SELECTED;
			properties[4] = waiter;
		}
	}

	return 1;
}

int changeHighlightedTool(struct menu *menu, struct controlComponent *ccb)
{
	/* Change selectedTool */
	selectedTool = ccb->btn->crd;

	/* Reset and fill again highlighted Coords Array */
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE && isInBoard(highlightedSquares[k]); k++)
	{
		highlightedSquares[k].i_coord = -1; highlightedSquares[k].j_coord = -1;
	}
	cMove *newMoves = movesByPieceType(pBoard, ccb->btn->crd);
	highlightMovesList(menu, ccb->btn->crd, newMoves);

	/* update gui board (and vis) */
	updateGUIBoard(menu);
	return 1;
}

int gui_makeMove(struct menu *menu, struct controlComponent *ccb)
{
	/* Update pBoard */
	char movedToolType = GetContentOfCoord(pBoard, selectedTool);
	setSlotInBoard(pBoard, ccb->btn->crd, movedToolType); //moves the tool to the new square
	setSlotInBoard(pBoard, selectedTool, EMPTY); //empties the square it was at before
	advanceTurnStage(0);
	
	/* Reset Highlight array*/
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE && isInBoard(highlightedSquares[k]); k++)
	{
		highlightedSquares[k].i_coord = -1;
		highlightedSquares[k].j_coord = -1;
	}
	selectedTool.i_coord = -1; selectedTool.j_coord = -1;
	
	/* Update GUIBoard */
	updateGUIBoard(menu);
	updateGUIBoard_Vis(menu);
	return 1;
}

int highlightMovesList(Menu *menu, Coord crd, cMove *moves)
{
	/* Reset Highlighted Squares Array */
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE && isInBoard(highlightedSquares[k]); k++)
	{
		highlightedSquares[k].i_coord = -1;
		highlightedSquares[k].j_coord = -1;
	}

	ControlComponent *ccb = &(guiBoard[crd.i_coord][crd.j_coord]);

	//SDL_FreeSurface(ccb->btn->pic);
	//ccb->btn->pic = uploadPicture("selected.bmp");
	selectedTool = ccb->btn->crd;
	/*if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}*/

	cMove *curr = moves;
	int count = 0;
	while (curr != NULL)
	{
		ccb = &(guiBoard[curr->dst.i_coord][curr->dst.j_coord]);
		//SDL_FreeSurface(ccb->btn->pic);
		//ccb->btn->pic = uploadPicture("threatened.bmp");
		highlightedSquares[count++] = ccb->btn->crd;

		/*if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
		{
			SDL_FreeSurface(ccb->btn->pic);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			quit();
			return 0;
		}*/
		curr = curr->next;
	}
	updateGUIBoard_Vis(menu);
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
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type,NON_SEL));

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
	int *gameMode = &(properties[5]);
	if (*gameMode == PVP_MODE)
	{
		/* Change to PVC mode: */
		*gameMode = PVC_MODE;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PVC.bmp");
	}
	else
	{
		/* Change to PVP mode: */
		*gameMode = PVP_MODE;
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
		ccb_continueOrPlay->btn->pic = uploadPicture(properties[5] == PVP_MODE ? "StartGameInactive.bmp" : "AI_SettingsInactive.bmp");
	}
	else
	{
		if (properties[5] == PVP_MODE)
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
	if (properties[4] == WHITE_PLAYER)
	{
		/* Change to Black Player: */
		properties[4] = BLACK_PLAYER;
		whitePlayerTurnStage = WAITING;
		blackPlayerTurnStage = NONE_SELECTED;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("NextPlayerBlack.bmp");
	}
	else
	{
		/* Change to White Player: */
		properties[4] = WHITE_PLAYER;
		whitePlayerTurnStage = NONE_SELECTED;
		blackPlayerTurnStage = WAITING;
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
	properties[2] = mod(properties[2] + 1, 5);
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
	if (properties[3] == WHITE_PLAYER)
	{
		properties[3] = BLACK_PLAYER;
		SDL_FreeSurface(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PlayerIsBlack.bmp");
	}
	else
	{
		properties[3] = WHITE_PLAYER;
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
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type,NON_SEL));

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
	showSaveGameMenu(menu, ccb);
	return 0;
}

int SaveMenu_saveGameSlot(struct menu *menu, struct controlComponent *ccb)
{
	if (selectedSlot == '0')
		return 0;

	char filename[12] = "chessX.xml \0";
	*(filename + 5) = selectedSlot;
	Save1(pBoard, filename);  //TODO: change to regular save.

	selectedSlot = '0';
	SaveOrLoad_Menu_UpdateVis(menu);
	
	return 1;
}

/* Serializes the game into an XML file */
int Save1(board_t board, char* file_name){
	int i, j;
	//replacing '\n' with '\0'
	file_name[strlen(file_name) - 1] = '\0';
	FILE *f = fopen(file_name, "w");
	if (f == NULL){
		printf("Wrong file name\n");
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
		if (properties[2] != 0)
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

/* Sets the game from XML file */
int LoadFromFile1(char* file_path, board_t board){
	int i, j;
	char str[51], curTag;
	FILE *file = fopen(file_path, "r");
	if (file == NULL)
	{
		printf("%s", "Wrong file name\n");
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
			properties[2] = 0;
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


int SaveMenu_selectSlot(struct menu *menu, struct controlComponent *ccb)
{
	if (selectedSlot != ccb->btn->purpose)
		selectedSlot = ccb->btn->purpose;
	else
		selectedSlot = '0';

	SaveOrLoad_Menu_UpdateVis(menu);
	return 1;
}

int SaveOrLoad_Menu_UpdateVis(struct menu *menu)
{
	/* Update Save Or Load Menu Vis */
	ControlComponent *ccbCurr = menu->panel_2->pnl->children;
	for (int i = 0; i < 7; i++)
	{
		ccbCurr->btn->pic = uploadPicture(i + '1' == selectedSlot ? "PressedGenericButton.bmp" : "UnpressedGenericButton.bmp");
		if (SDL_BlitSurface(ccbCurr->btn->pic, NULL, chessWindow->self, &(ccbCurr->rect)) != 0)
		{
			SDL_FreeSurface(ccbCurr->btn->pic);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			quit();
			return 0;
		}

		ccbCurr = ccbCurr->next;
	}
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int SaveMenu_ResetGameAndShowMainMenu(struct menu *menu, struct controlComponent *ccb)
{
	//TODO: Reset all data (board, default settings, kingcrd, highlights, turnstages...) and also this menu's data, and then:
	showMainMenu(menu, ccb);
	return 1;
}

int SaveMenu_ResetSaveMenuAndShowGamePlayMenu(struct menu *menu, struct controlComponent *ccb)
{
	selectedSlot = '0';
	SaveOrLoad_Menu_UpdateVis(menu);
	showGamePlayMenu(menu, ccb);
	return 1;
}

int LoadMenu_Reset_LoadMenu_And_ShowMainMenu(struct menu *menu, struct controlComponent *ccb)
{
	//TODO: Reset all of this menu's data ( selected slot... ) and then:
	showMainMenu(menu, ccb);
	return 1;
}

/* shared with main menu and game play menu */
int QuitGame(struct menu *menu, struct controlComponent *ccb)
{
	return 0;
}





/* Not in USE!!!*/
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


int rightClicksHandler(Window *chessWindow, SDL_Event e)
{
	Menu *menu = chessWindow->shownMenu;
	ControlComponent *currChild;

	if (menu->identifier == PLAYER_SELECTION_MENU)
	{
		if (menu->panel_4 != NULL)
		{
			currChild = menu->panel_4->pnl->children;
			while (currChild != NULL)
			{
				if (currChild->btn != NULL)
				{

					if (isPressInsideButton(e, currChild))
					{
						Coord crd = currChild->btn->crd;
						setSlotInBoard(pBoard, crd, EMPTY);
						playerSelectionMenu_updateContinueOrPlayButton(chessWindow);
						updateGUIBoard_Vis(menu);
						return 1;
					}

				}
				currChild = currChild->next;
			}
		}
		return 0;
	}
	return 0;
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
	if (properties[2] == 0)
		return "BestDiff.bmp";
	else if (properties[2] == 1)
		return "1Diff.bmp";
	else if (properties[2] == 2)
		return "2Diff.bmp";
	else if (properties[2] == 3)
		return "3Diff.bmp";
	else
		return "4Diff.bmp";
}

const char *getPlayerColorPicName()
{
	if (properties[3] == WHITE_PLAYER)
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
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_Main);
	return 0;
}

int showPlayerSelectionMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_PlayerSelection);
	return 0;
}

int showAI_SettingsMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_AI_settings);
	return 0;
}

int showGamePlayMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = GAME_MODE;
	Coord crd;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			char tool = GetContentOfCoord(pBoard, crd);
			if (tool == WHITE_K)
				WhiteKing = crd;
			if (tool == BLACK_K)
				BlackKing= crd;
		}
	}
	showMenu(chessWindow, pMenu_Game);
	return 0;
}

int showSaveGameMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	//properties[0] = XXXXXX_MODE; //TODO: should this line even be here, and what mode?
	showMenu(chessWindow, pMenu_Save);
	return 0;
}

int showLoadGameMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	//properties[0] = XXXXXX_MODE; //TODO: should this line even be here, and what mode?
	showMenu(chessWindow, pMenu_Load);
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

