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
Coord selectedTool, highlightedSquares[BOARD_SIZE*BOARD_SIZE], promoteSquare;

Coord src, dest;

/* Save/Load Game */
char selectedSlot = '0';

/* Hint Menu */
int hintLevel = 1;

/* Methods */

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

int labelMaker(ControlComponent *ccl, Label *lbl, SDL_Rect rect, char *filename)
{
	ccl->pnl = NULL;
	ccl->btn = NULL;
	ccl->lbl = lbl;
	ccl->next = NULL;
	ccl->rect = rect;

	ccl->lbl->pic = uploadPicture(filename);

	return 1;
}



int createMainMenu(Menu *mainMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns, ControlComponent *ccl, Label *lbl)
{
	/* Rects */
	SDL_Rect Rect1024x768 = create1024x768Rect();
	SDL_Rect newGameRect = createSDL_Rect(200, 75, 20, 430);
	SDL_Rect loadGameRect = createSDL_Rect(200, 75, 20, 530);
	SDL_Rect quitGameRect = createSDL_Rect(200, 75, 20, 630);

	/* label rects */
	SDL_Rect welcome = createSDL_Rect(768, 576, 240, 130);
	SDL_Rect logo = createSDL_Rect(240, 282, 0, 130);

	createMenu(mainMenu, Rect1024x768, rgbMenuBlue, MAIN_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, newGameRect, uploadPicture("NewGameButton.bmp"), showPlayerSelectionMenu, NEW_GAME_BUTTON);//NewGameButton
	createButton(ccbs + 1, btns + 1, loadGameRect, uploadPicture("LoadGameButton.bmp"), showLoadGameMenu, LOAD_GAME_BUTTON);
	createButton(ccbs + 2, btns + 2, quitGameRect, uploadPicture("QuitGameButton.bmp"), QuitGame, QUIT_GAME_BUTTON);

	/* Make the Panel*/
	panelMaker(ccps, panel, Rect1024x768, rgbMenuBlue);

	addButtonToPanel(ccps, ccbs);
	addButtonToPanel(ccps, ccbs + 1);
	addButtonToPanel(ccps, ccbs + 2);

	/* Add the Panels To the Menu*/
	addPanelToMenu(mainMenu, ccps, 1);

	/* Make the labels */
	labelMaker(ccl, lbl, welcome, "welcome.bmp");
	labelMaker(ccl + 1, lbl + 1, logo, "logo.bmp");
	addLabelToPanel(ccps, ccl);
	addLabelToPanel(ccps, ccl + 1);

	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}

int createPlayerSelectionMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns, ControlComponent *ccl, Label *lbl)
{

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	//int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	int hGameModeSetting = (SCREEN_H - 2 * hSide) / 3;


	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();


	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wGameModeSetting, hGameModeSetting, wSide, 9);
	SDL_Rect togglesPanel = createSDL_Rect(wGameModeSetting, 250, wSide, 9 + 250);
	SDL_Rect continueOrPlayPanel = createSDL_Rect(wGameModeSetting, 250, wSide, 9 + 500);
	//SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	/* Button Rects */
	SDL_Rect returnToMainMenuButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect gameModeButton = createSDL_Rect(wGameModeSetting - 2 * wSide, hGameModeSetting / 2 - 2 * hSide, wSide, hSide);
	SDL_Rect nextPlayerButton = createSDL_Rect(80, 80, wSide, hGameModeSetting / 2);
	SDL_Rect ContinueOrPlayButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (wGameModeSetting - 4 * hSide) / 2, wSide, hGameModeSetting / 2);

	/* label rects */
	SDL_Rect header = createSDL_Rect(wBoardSetting, 75, wGameModeSetting + wSide, 10);


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

	/* Make the labels */
	labelMaker(ccl, lbl, header, "PlayerSelectionHeader.bmp");

	/* Add the label to the Menu */
	playerSelectionMenu->header = (ControlComponent *)ccl;

	/* Highlighted Squares  <--- this happens *here* only for sake of multi-functionality */
	for (int k = 0; k < BOARD_SIZE*BOARD_SIZE; k++)
	{
		highlightedSquares[k].i_coord = -1;
		highlightedSquares[k].j_coord = -1;
	}
	Coord crd;
	crd.i_coord = -1; crd.j_coord = -1;
	promoteSquare = crd;


	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}

int createAI_SettingsMenu(Menu *AI_SettingsMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns, ControlComponent *ccl, Label *lbl)
{
	int wSide = 12;
	int hSide = 9;
	int wBoardPanel = 800;
	//int hBoardPanel = 750;
	int wTogglePanels = SCREEN_W - wBoardPanel - 2 * wSide;
	int hTogglePanels = (SCREEN_H - 2 * hSide) / 3;

	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9);
	SDL_Rect togglesPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 250);
	SDL_Rect playPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 500);

	/* Button Rects*/
	SDL_Rect backToMainMenuButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect backToPlayerSelectionButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	SDL_Rect difficultyButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect playerColorButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	SDL_Rect startGameButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hTogglePanels / 2);

	/* label rects */
	SDL_Rect header = createSDL_Rect(wBoardPanel, 75, wTogglePanels + wSide, 10);

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

	/* Make the labels */
	labelMaker(ccl, lbl, header, "AISettingsHeader.bmp");

	/* Add the label to the Menu */
	AI_SettingsMenu->header = (ControlComponent *)ccl;


	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}


int createSaveMenu(Menu *pMenu_Save, ControlComponent *ccp_SaveMenuCCPs, Panel *pnl_SaveMenuPanels, ControlComponent *ccb_SaveMenuCCBs, Button *btn_SaveMenuButtons, ControlComponent *ccl, Label *lbl)
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
	SDL_Rect returnToGameButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hGameModeSetting / 2 + hSide);


	SDL_Rect slot_1 = createSDL_Rect(200, 55, 0, 250 + hSide);
	SDL_Rect slot_2 = createSDL_Rect(200, 55, 0, 313 + hSide);
	SDL_Rect slot_3 = createSDL_Rect(200, 55, 0, 376 + hSide);
	SDL_Rect slot_4 = createSDL_Rect(200, 55, 0, 439 + hSide);
	SDL_Rect slot_5 = createSDL_Rect(200, 55, 0, 502 + hSide);
	SDL_Rect slot_6 = createSDL_Rect(200, 55, 0, 565 + hSide);
	SDL_Rect slot_7 = createSDL_Rect(200, 55, 0, 628 + hSide);
	SDL_Rect saveButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);

	/* label rects */
	SDL_Rect header = createSDL_Rect(wBoardSetting, 75, 196, 10 - hSide);// wGameModeSetting + wSide, 10);
	SDL_Rect welcome = createSDL_Rect(768, 576, 240 - wGameModeSetting - wSide, 130 - hSide);


	createMenu(pMenu_Save, Rect1024x768, rgbMenuBlue, SAVE_MENU);

	/* Make the buttons */
	createButton(ccb_SaveMenuCCBs, btn_SaveMenuButtons, returnToMainMenu, uploadPicture("ReturnToMainMenu.bmp"), SaveMenu_ResetGameAndShowMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccb_SaveMenuCCBs + 1, btn_SaveMenuButtons + 1, slot_1, uploadPicture("slot1.bmp"), SaveMenu_selectSlot, '1');
	createButton(ccb_SaveMenuCCBs + 2, btn_SaveMenuButtons + 2, slot_2, uploadPicture("slot2.bmp"), SaveMenu_selectSlot, '2');
	createButton(ccb_SaveMenuCCBs + 3, btn_SaveMenuButtons + 3, slot_3, uploadPicture("slot3.bmp"), SaveMenu_selectSlot, '3');
	createButton(ccb_SaveMenuCCBs + 4, btn_SaveMenuButtons + 4, slot_4, uploadPicture("slot4.bmp"), SaveMenu_selectSlot, '4');
	createButton(ccb_SaveMenuCCBs + 5, btn_SaveMenuButtons + 5, slot_5, uploadPicture("slot5.bmp"), SaveMenu_selectSlot, '5');
	createButton(ccb_SaveMenuCCBs + 6, btn_SaveMenuButtons + 6, slot_6, uploadPicture("slot6.bmp"), SaveMenu_selectSlot, '6');
	createButton(ccb_SaveMenuCCBs + 7, btn_SaveMenuButtons + 7, slot_7, uploadPicture("slot7.bmp"), SaveMenu_selectSlot, '7');
	/* In case we want to add more slots, need to insert extra lines (such as the lines above) for the extra slots here*/
	createButton(ccb_SaveMenuCCBs + 8, btn_SaveMenuButtons + 8, saveButton, uploadPicture("saveGame.bmp"), SaveMenu_saveGameSlot, SAVE_GAME_BUTTON);
	createButton(ccb_SaveMenuCCBs + 9, btn_SaveMenuButtons + 9, returnToGameButton, uploadPicture("ReturnToGame.bmp"), SaveMenu_ResetSaveMenuAndShowGamePlayMenu, BACK_TO_GAME);

	/* Make the Panels*/
	panelMaker(ccp_SaveMenuCCPs, pnl_SaveMenuPanels, returnsPanel, rgbRed);
	panelMaker(ccp_SaveMenuCCPs + 1, pnl_SaveMenuPanels + 1, slotsPanel, rgbMenuBlue);

	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 1);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 2);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 3);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 4);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 5);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 6);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 7);
	addButtonToPanel(ccp_SaveMenuCCPs + 1, ccb_SaveMenuCCBs + 8);
	addButtonToPanel(ccp_SaveMenuCCPs, ccb_SaveMenuCCBs + 9);

	/* Make the labels */
	labelMaker(ccl, lbl, header, "SaveMenuHeader.bmp");
	labelMaker(ccl + 1, lbl + 1, welcome, "welcome.bmp");
	addLabelToPanel(ccp_SaveMenuCCPs + 1, ccl);
	addLabelToPanel(ccp_SaveMenuCCPs + 1, ccl + 1);


	/* Add the Panels To the Menu*/
	addPanelToMenu(pMenu_Save, ccp_SaveMenuCCPs, 1);
	addPanelToMenu(pMenu_Save, ccp_SaveMenuCCPs + 1, 2);
	addPanelToMenu(pMenu_Save, NULL, 3);
	addPanelToMenu(pMenu_Save, NULL, 4);


	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}

int createGameMenu(Menu *GamePlayMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns, ControlComponent *ccl, Label *lbl)
{
	int wSide = 12;
	int hSide = 9;
	int wBoardPanel = 800;
	//int hBoardPanel = 750;
	int wTogglePanels = SCREEN_W - wBoardPanel - 2 * wSide;
	int hTogglePanels = (SCREEN_H - 2 * hSide) / 3;

	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9);
	SDL_Rect infoPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 250);
	SDL_Rect saveAndQuitPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 500);


	/* Button Rects*/
	SDL_Rect backToMainMenuButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect goToHintMenu = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));
	SDL_Rect saveGameButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect quitButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	/* label rects */
	SDL_Rect header = createSDL_Rect(wBoardPanel, 75, wTogglePanels + wSide, 10);
	SDL_Rect infoRect_top = createSDL_Rect(wTogglePanels - 2 * wSide, hTogglePanels / 2 - 2 * hSide, wSide, 0);
	SDL_Rect infoRect_bottom = createSDL_Rect(wTogglePanels - 2 * wSide, hTogglePanels / 2 - 2 * hSide, wSide, hTogglePanels / 2);

	createMenu(GamePlayMenu, Rect1024x768, rgbMenuBlue, GAME_PLAY_MENU);

	/* Make the buttons */
	createButton(ccbs, btns, backToMainMenuButton, uploadPicture("ReturnToMainMenu.bmp"), GamePlayMenu_endGameAndShowMainMenu, END_GAME_AND_RETURN_TO_MAIN_MENU_BUTTON);
	createButton(ccbs + 1, btns + 1, quitButton, uploadPicture("QuitGameButton_inGame.bmp"), QuitGame, QUIT_GAME_BUTTON);
	createButton(ccbs + 2, btns + 2, saveGameButton, uploadPicture("saveGame.bmp"), GamePlayMenu_SaveGame, SAVE_GAME_BUTTON);
	createButton(ccbs + 3, btns + 3, goToHintMenu, uploadPicture("Hint.bmp"), ShowHintMenu, HINT_MENU_BUTTON);

	/* Make the Panels*/
	panelMaker(ccps, panel, returnsPanel, rgbRed);
	panelMaker(ccps + 1, panel + 1, infoPanel, rgbOrange);
	panelMaker(ccps + 2, panel + 2, saveAndQuitPanel, rgbGreen);

	addButtonToPanel(ccps, ccbs);
	addButtonToPanel(ccps + 2, ccbs + 1);
	addButtonToPanel(ccps + 2, ccbs + 2);
	addButtonToPanel(ccps, ccbs + 3);

	/* Add the Panels To the Menu*/
	addPanelToMenu(GamePlayMenu, ccps, 1);
	addPanelToMenu(GamePlayMenu, ccps + 1, 2);
	addPanelToMenu(GamePlayMenu, ccps + 2, 3);

	/* Make the labels */
	labelMaker(ccl, lbl, header, "GameHeader.bmp");
	labelMaker(ccl + 1, lbl + 1, infoRect_top, "info_WhitePlayerTurn.bmp");
	labelMaker(ccl + 2, lbl + 2, infoRect_bottom, "info_makeMove.bmp");

	addLabelToPanel(GamePlayMenu->panel_2, ccl + 1);
	addLabelToPanel(GamePlayMenu->panel_2, ccl + 2);


	/* Add the header to the Menu */
	GamePlayMenu->header = (ControlComponent *)ccl;


	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}

int createLoadMenu(Menu *pMenu_Load, ControlComponent *ccp_LoadMenuCCPs, Panel *pnl_LoadMenuPanels, ControlComponent *ccb_LoadMenuCCBs, Button *btn_LoadMenuButtons, ControlComponent *ccl, Label *lbl)
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
	SDL_Rect buttonsPanel = createSDL_Rect(wGameModeSetting, hGameModeSetting, wSide, 9);
	SDL_Rect labelsPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);

	SDL_Rect returnToMainMenu = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);
	//SDL_Rect returnToGameButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hGameModeSetting / 2 + hSide);
	SDL_Rect slot_1 = createSDL_Rect(200, 55, 0, 250 + hSide);		//200, 75, 412, 100);
	SDL_Rect slot_2 = createSDL_Rect(200, 55, 0, 313 + hSide);		//200, 75, 412, 185);
	SDL_Rect slot_3 = createSDL_Rect(200, 55, 0, 376 + hSide);		//200, 75, 412, 270);
	SDL_Rect slot_4 = createSDL_Rect(200, 55, 0, 439 + hSide);		//200, 75, 412, 355);
	SDL_Rect slot_5 = createSDL_Rect(200, 55, 0, 502 + hSide);		//200, 75, 412, 440);
	SDL_Rect slot_6 = createSDL_Rect(200, 55, 0, 565 + hSide);		//200, 75, 412, 525);
	SDL_Rect slot_7 = createSDL_Rect(200, 55, 0, 628 + hSide);		//200, 75, 412, 610);
	SDL_Rect loadButton = createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hGameModeSetting / 2 + hSide); //createSDL_Rect(wGameModeSetting - 2 * wSide, (hGameModeSetting - 4 * hSide) / 2, wSide, hSide);

	/* label rects */
	SDL_Rect header = createSDL_Rect(wBoardSetting, 75, 0, 10 - hSide);// wGameModeSetting + wSide, 10);
	SDL_Rect welcome = createSDL_Rect(768, 576, 240 - wGameModeSetting - wSide, 130 - hSide);


	createMenu(pMenu_Load, Rect1024x768, rgbMenuBlue, LOAD_MENU);

	/* Make the buttons */
	createButton(ccb_LoadMenuCCBs, btn_LoadMenuButtons, returnToMainMenu, uploadPicture("ReturnToMainMenu.bmp"), LoadMenu_Reset_LoadMenu_And_ShowMainMenu, BACK_TO_MAIN_MENU_BUTTON);
	createButton(ccb_LoadMenuCCBs + 1, btn_LoadMenuButtons + 1, slot_1, uploadPicture("slot1.bmp"), SaveMenu_selectSlot, '1');
	createButton(ccb_LoadMenuCCBs + 2, btn_LoadMenuButtons + 2, slot_2, uploadPicture("slot2.bmp"), SaveMenu_selectSlot, '2');
	createButton(ccb_LoadMenuCCBs + 3, btn_LoadMenuButtons + 3, slot_3, uploadPicture("slot3.bmp"), SaveMenu_selectSlot, '3');
	createButton(ccb_LoadMenuCCBs + 4, btn_LoadMenuButtons + 4, slot_4, uploadPicture("slot4.bmp"), SaveMenu_selectSlot, '4');
	createButton(ccb_LoadMenuCCBs + 5, btn_LoadMenuButtons + 5, slot_5, uploadPicture("slot5.bmp"), SaveMenu_selectSlot, '5');
	createButton(ccb_LoadMenuCCBs + 6, btn_LoadMenuButtons + 6, slot_6, uploadPicture("slot6.bmp"), SaveMenu_selectSlot, '6');
	createButton(ccb_LoadMenuCCBs + 7, btn_LoadMenuButtons + 7, slot_7, uploadPicture("slot7.bmp"), SaveMenu_selectSlot, '7');
	/* In case we want to add more slots, need to insert extra lines (such as the lines above) for the extra slots here*/
	createButton(ccb_LoadMenuCCBs + 8, btn_LoadMenuButtons + 8, loadButton, uploadPicture("loadGame.bmp"), LoadGame, LOAD_GAME_BUTTON);

	/* Make the Panels*/
	panelMaker(ccp_LoadMenuCCPs, pnl_LoadMenuPanels, buttonsPanel, rgbRed);
	panelMaker(ccp_LoadMenuCCPs + 1, pnl_LoadMenuPanels + 1, labelsPanel, rgbMenuBlue);

	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 1);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 2);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 3);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 4);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 5);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 6);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 7);
	addButtonToPanel(ccp_LoadMenuCCPs, ccb_LoadMenuCCBs + 8);

	/* Make the labels */
	labelMaker(ccl, lbl, header, "LoadMenuHeader.bmp");
	labelMaker(ccl + 1, lbl + 1, welcome, "welcome.bmp");
	addLabelToPanel(ccp_LoadMenuCCPs + 1, ccl);
	addLabelToPanel(ccp_LoadMenuCCPs + 1, ccl + 1);


	/* Add the Panels To the Menu*/
	addPanelToMenu(pMenu_Load, ccp_LoadMenuCCPs, 1);
	addPanelToMenu(pMenu_Load, ccp_LoadMenuCCPs + 1, 2);
	addPanelToMenu(pMenu_Load, NULL, 3);
	addPanelToMenu(pMenu_Load, NULL, 4);


	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}


int createHintMenu(Menu *pMenu_Hint, ControlComponent *ccp_HintMenuCCPs, Panel *pnl_HintMenuPanels, ControlComponent *ccb_HintMenuCCBs, Button *btn_HintMenuButtons, ControlComponent *ccl, Label *lbl)
{
	int wSide = 12;
	int hSide = 9;
	int wBoardPanel = 800;
	//int hBoardPanel = 750;
	int wTogglePanels = SCREEN_W - wBoardPanel - 2 * wSide;
	//int hTogglePanels = (SCREEN_H - 2 * hSide) / 3;

	/* Screen Rect */
	SDL_Rect Rect1024x768 = create1024x768Rect();

	/* Panel Rects */
	SDL_Rect returnsPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9);
	SDL_Rect emptyPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 250);
	SDL_Rect showPanel = createSDL_Rect(wTogglePanels, 250, wSide, 9 + 500);


	/* Button Rects*/
	SDL_Rect backToGameButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);

	SDL_Rect levelToggleButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide);
	SDL_Rect showButton = createSDL_Rect(wTogglePanels - 2 * wSide, (wTogglePanels - 4 * hSide) / 2, wSide, hSide + (wTogglePanels*0.6));

	/* label rects */
	SDL_Rect header = createSDL_Rect(wBoardPanel, 75, wTogglePanels + wSide, 10);
	//SDL_Rect infoRect_top = createSDL_Rect(wTogglePanels - 2 * wSide, hTogglePanels / 2 - 2 * hSide, wSide, 0);
	//SDL_Rect infoRect_bottom = createSDL_Rect(wTogglePanels - 2 * wSide, hTogglePanels / 2 - 2 * hSide, wSide, hTogglePanels / 2);

	createMenu(pMenu_Hint, Rect1024x768, rgbMenuBlue, HINT_MENU);

	/* Make the buttons */
	createButton(ccb_HintMenuCCBs, btn_HintMenuButtons, backToGameButton, uploadPicture("ReturnToGame.bmp"), HintMenu_showGamePlayMenu, BACK_TO_GAME);
	createButton(ccb_HintMenuCCBs + 1, btn_HintMenuButtons + 1, levelToggleButton, uploadPicture("1Diff.bmp"), HintMenu_toggleDifficulty, AI_DIFF_TOGGLE_BUTTON);
	createButton(ccb_HintMenuCCBs + 2, btn_HintMenuButtons + 2, showButton, uploadPicture("ShowHint.bmp"), HintMenu_ShowHint, SHOW_HINT_BUTTON);


	/* Make the Panels*/
	panelMaker(ccp_HintMenuCCPs, pnl_HintMenuPanels, returnsPanel, rgbRed);
	panelMaker(ccp_HintMenuCCPs + 1, pnl_HintMenuPanels + 1, emptyPanel, rgbOrange);
	panelMaker(ccp_HintMenuCCPs + 2, pnl_HintMenuPanels + 2, showPanel, rgbGreen);

	addButtonToPanel(ccp_HintMenuCCPs, ccb_HintMenuCCBs);
	addButtonToPanel(ccp_HintMenuCCPs + 2, ccb_HintMenuCCBs + 1);
	addButtonToPanel(ccp_HintMenuCCPs + 2, ccb_HintMenuCCBs + 2);

	/* Add the Panels To the Menu*/
	addPanelToMenu(pMenu_Hint, ccp_HintMenuCCPs, 1);
	addPanelToMenu(pMenu_Hint, ccp_HintMenuCCPs + 1, 2);
	addPanelToMenu(pMenu_Hint, ccp_HintMenuCCPs + 2, 3);

	/* Make the labels */
	labelMaker(ccl, lbl, header, "HintHeader.bmp");

	/* Add the header to the Menu */
	pMenu_Hint->header = (ControlComponent *)ccl;



	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}


int showMenu(Window *window, Menu *menu)
{
	addMenuToWindow(chessWindow, menu);

	if (menu->identifier == PLAYER_SELECTION_MENU || menu->identifier == AI_SETTINGS_MENU || menu->identifier == GAME_PLAY_MENU || menu->identifier == HINT_MENU)
	{
		updateGUIBoard(menu);
	}

	if (menu->panel_1 != NULL)
	{
		if (drawPanelToMenu(menu->panel_1) == 0 || drawButtonsOfPanel(menu->panel_1) == 0 || drawLabelsOfPanel(menu->panel_1->pnl->children) == 0)
			return 0;
	}
	if (menu->panel_2 != NULL)
	{
		if (drawPanelToMenu(menu->panel_2) == 0 || drawButtonsOfPanel(menu->panel_2) == 0 || drawLabelsOfPanel(menu->panel_2->pnl->children) == 0)
			return 0;
	}
	if (menu->panel_3 != NULL)
	{
		if (drawPanelToMenu(menu->panel_3) == 0 || drawButtonsOfPanel(menu->panel_3) == 0)
			return 0;
	}
	if (menu->panel_4 != NULL)
	{
		if (drawPanelToMenu(menu->panel_4) == 0 || drawLabelsOfPanel(menu->panel_4->pnl->children) == 0 || drawButtonsOfPanel(menu->panel_4) == 0)
			return 0;
	}
	if (menu->header != NULL)
	{
		if (drawLabelsOfPanel((ControlComponent *)menu->header) == 0)
			return 0;
	}

	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}



int createGUIBoard(board_g gBoard, ControlComponent *ccp_BoardSetting, Button *btn_BoardPanelButtons, board_t board, ControlComponent *ccl, Label *lbl)
{
	/* This also adds each of the squares to the panel! */

	Coord crd;
	eTool type;
	int player;
	btnFunc toolFunc;

	labelMaker(ccl, lbl, createSDL_Rect(680, 680, 40, 80), "Indices.bmp");
	addLabelToPanel(ccp_BoardSetting, ccl);

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

			type = e_getInitialTypeOfCoord(crd);
			player = getInitialPlayerOfCoord(crd);
			toolFunc = playerSelectionMenu_toggleTool;
			createSquareByToolType(ccp_BoardSetting, &(guiBoard[crd.i_coord][crd.j_coord]), (btn_BoardPanelButtons + i*BOARD_SIZE + j), crd, type, player, toolFunc);
		}
	}

	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
		return 1;
}

int updateGUIBoard(Menu *menu)
{
	/* updates the function of the GUIBoard */
	Coord crd;
	int identfier = menu->identifier;
	btnFunc toolFunc;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			crd.i_coord = i; crd.j_coord = j;
			if (identfier == GAME_PLAY_MENU)
			{
				toolFunc = (btnFunc)getGameFunctionOfCoord(crd);
				/* If it is the computer's turn, don't produce events from the board */
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
			SDL_FreeSurface1(guiBoard[i][j].btn->pic); //TODO: be careful!
			guiBoard[i][j].btn->pic = uploadPicture(getPictureName_tools(crd, getColor(pBoard, crd),
				get_eToolFromType(GetContentOfCoord(pBoard, crd)), selectionStatus));

			if (SDL_BlitSurface(guiBoard[i][j].btn->pic, NULL, chessWindow->self, &(guiBoard[i][j].rect)) != 0)
			{
				SDL_FreeSurface1(guiBoard[i][j].btn->pic);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				terminateProgram();
				return 0;
			}
		}
	}
	return 1;
}



/* btnFuncs */

btnFunc getGameFunctionOfCoord(Coord crd)
{
	char tool = GetContentOfCoord(pBoard, crd);
	int stageInd;
	if (properties[5] == PVC_MODE) //case PVC mode
	{
		if (properties[3] != properties[4])
			return nullFunction;
		if (properties[3] == properties[4]) //case player's turn
		{
			stageInd = (properties[3] == WHITE_PLAYER) ? whitePlayerTurnStage : blackPlayerTurnStage;

			if (stageInd == NONE_SELECTED)
			{
				if (isOfPlayer(properties[4], tool))
				{
					return getHighlightFunctionOfTool(tool);
				}
				else
				{
					return nullFunction;
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
			else if (stageInd == PROMOTE)
			{

				if (isThePromotedPiece(crd) && GetContentOfCoord(pBoard, crd) != BLACK_P && GetContentOfCoord(pBoard, crd) != WHITE_P)
				{
					return endPromotionStage;
				}
				else
				{
					return nullFunction;
				}
			}
		}
	}
	else //case PVP mode
	{
		//int player = properties[4], waiter = generateEnemyColor(player);
		stageInd = (properties[4] == WHITE_PLAYER) ? whitePlayerTurnStage : blackPlayerTurnStage;

		if (stageInd == NONE_SELECTED)
		{
			if (isOfPlayer(properties[4], tool))
			{
				return getHighlightFunctionOfTool(tool);
			}
			else
			{
				return nullFunction;
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
		else if (stageInd == PROMOTE && GetContentOfCoord(pBoard, crd) != BLACK_P && GetContentOfCoord(pBoard, crd) != WHITE_P)
		{
			if (isThePromotedPiece(crd))
			{
				return endPromotionStage;
			}
			else
			{
				return nullFunction;
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

int pawnHighlight(struct menu *menu, struct controlComponent *ccb)
{
	/* { WAITING, NONE_SELECTED, HIGHLIGHTED, MOVE_MADE, PROMOTE } */
	advanceTurnStage(0);
	Coord crd = ccb->btn->crd;
	selectedTool = crd;
	cMove *moves = PawnMoves(pBoard, crd);
	highlightMovesList(menu, crd, moves);
	freeMovesList(moves);
	updateGUIBoard(menu);

	if (properties[1] == 1) //some picture allocation failed
		return 0;
	else
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

	if (properties[1] == 1)
		return 0;
	else
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

	if (properties[1] == 1)
		return 0;
	else
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

	if (properties[1] == 1)
		return 0;
	else
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

	if (properties[1] == 1)
		return 0;
	else
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

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int endPromotionStage(struct menu *menu, struct controlComponent *ccb)
{
	advanceTurnStage(0);
	updateGUIBoard(menu);

	return 1;
}

int advanceTurnStage(int promotiveSituation)
{

	/*
	WAITING			0
	NONE_SELECTED	1
	HIGHLIGHTED		2
	MOVE_MADE		3
	PROMOTE			4
	*/
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
	freeMovesList(newMoves);

	/* update gui board (and vis) */
	updateGUIBoard(menu);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int setPromoteSquare(Coord crd, int promotiveSituation)
{
	Coord tmp;
	if (promotiveSituation)
	{
		promoteSquare = crd;
	}
	else
	{
		tmp.i_coord = -1; tmp.j_coord = -1;
		promoteSquare = tmp;
	}
	return 1;
}

int gui_makeMove(struct menu *menu, struct controlComponent *ccb)
{
	/* Update pBoard */
	char movedToolType = GetContentOfCoord(pBoard, selectedTool);
	setSlotInBoard(pBoard, ccb->btn->crd, movedToolType); //moves the tool to the new square
	setSlotInBoard(pBoard, selectedTool, EMPTY); //empties the square it was at before
	int promotiveSituation = ((movedToolType == BLACK_P || movedToolType == WHITE_P) && (ccb->btn->crd.j_coord == 0 || ccb->btn->crd.j_coord == BOARD_SIZE - 1));
	advanceTurnStage(promotiveSituation);

	setPromoteSquare(ccb->btn->crd, promotiveSituation);


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

	if (properties[1] == 1)
		return 0;
	else
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

	SDL_FreeSurface1(ccb->btn->pic);
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type, NON_SEL));

	if (playerSelectionMenu_updateContinueOrPlayButton(chessWindow) == 0)
	{
		terminateProgram();
		return 0;
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int playerSelectionMenu_toggleGameMode(Menu *menu, struct controlComponent *ccb)
{
	int *gameMode = &(properties[5]);
	if (*gameMode == PVP_MODE)
	{
		/* Change to PVC mode: */
		*gameMode = PVC_MODE;
		SDL_FreeSurface1(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PVC.bmp");
	}
	else
	{
		/* Change to PVP mode: */
		*gameMode = PVP_MODE;
		SDL_FreeSurface1(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PVP.bmp");
	}
	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}
	if (playerSelectionMenu_updateContinueOrPlayButton(chessWindow) == 0)
		return 0;

	return 1;
}

int playerSelectionMenu_updateContinueOrPlayButton(Window *window)
{
	/* updates this button's function and picture (not global gameMode) */
	if (pMenu_PlayerSelection == NULL)
	{
		return 0;
	}

	ControlComponent *ccb_continueOrPlay;
	ccb_continueOrPlay = pMenu_PlayerSelection->panel_3->pnl->children; /* Should point to continueOrPlay Button*/

	if (!isValidBoardInitialization(pBoard))
	{
		ccb_continueOrPlay->btn->f = nullFunction;
		SDL_FreeSurface1(ccb_continueOrPlay->btn->pic);
		ccb_continueOrPlay->btn->pic = uploadPicture(properties[5] == PVP_MODE ? "StartGameInactive.bmp" : "AI_SettingsInactive.bmp");
	}
	else
	{
		if (properties[5] == PVP_MODE)
		{
			ccb_continueOrPlay->btn->f = showGamePlayMenu;
			SDL_FreeSurface1(ccb_continueOrPlay->btn->pic);
			ccb_continueOrPlay->btn->pic = uploadPicture("StartGame.bmp");
		}
		else
		{
			ccb_continueOrPlay->btn->f = showAI_SettingsMenu;
			SDL_FreeSurface1(ccb_continueOrPlay->btn->pic);
			ccb_continueOrPlay->btn->pic = uploadPicture("AI_Settings.bmp");
		}
	}

	if (SDL_BlitSurface(ccb_continueOrPlay->btn->pic, NULL, window->self, &(ccb_continueOrPlay->rect)) != 0)
	{
		SDL_FreeSurface1(ccb_continueOrPlay->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
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
		SDL_FreeSurface1(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("NextPlayerBlack.bmp");
	}
	else
	{
		/* Change to White Player: */
		properties[4] = WHITE_PLAYER;
		whitePlayerTurnStage = NONE_SELECTED;
		blackPlayerTurnStage = WAITING;
		SDL_FreeSurface1(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("NextPlayerWhite.bmp");
	}
	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}



int AI_settingsMenu_toggleDifficulty(Menu *menu, struct controlComponent *ccb)
{
	properties[2] = mod(properties[2] + 1, 5);
	SDL_FreeSurface1(ccb->btn->pic);
	ccb->btn->pic = uploadPicture(getDifficultyPicName());

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int AI_settingsMenu_togglePlayerColor(Menu *menu, struct controlComponent *ccb)
{
	if (properties[3] == WHITE_PLAYER)
	{
		properties[3] = BLACK_PLAYER;
		SDL_FreeSurface1(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PlayerIsBlack.bmp");
	}
	else
	{
		properties[3] = WHITE_PLAYER;
		SDL_FreeSurface1(ccb->btn->pic);
		ccb->btn->pic = uploadPicture("PlayerIsWhite.bmp");
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
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

	SDL_FreeSurface1(ccb->btn->pic);
	guiBoard[crd.i_coord][crd.j_coord].btn->pic = uploadPicture(getPictureName_tools(crd, player, type, NON_SEL));

	if (AI_Settings_updatePlayButton(chessWindow) == 0)
	{
		terminateProgram();
		return 0;
	}

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}


	if (properties[1] == 1)
		return 0;
	else
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
		SDL_FreeSurface1(ccb_Play->btn->pic);
		ccb_Play->btn->pic = uploadPicture("StartGameInactive.bmp");
	}
	else
	{
		ccb_Play->btn->f = showGamePlayMenu;
		SDL_FreeSurface1(ccb_Play->btn->pic);
		ccb_Play->btn->pic = uploadPicture("StartGame.bmp"); //TODO: maybe unnecessary allocation too many times. also in twin function.
	}

	if (SDL_BlitSurface(ccb_Play->btn->pic, NULL, window->self, &(ccb_Play->rect)) != 0)
	{
		SDL_FreeSurface1(ccb_Play->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


int GamePlayMenu_endGameAndShowMainMenu(struct menu *menu, struct controlComponent *ccb)
{
	properties[0] = 1;
	properties[2] = 1;
	properties[3] = WHITE_PLAYER;
	properties[4] = WHITE_PLAYER;
	properties[5] = PVP_MODE;
	/* TODO:
	update button images:
	player selection: white player, PVP
	AI settings: white player, 1diff
	*/
	showMainMenu(NULL, ccb);
	return 0;
}

int GamePlayMenu_SaveGame(struct menu *menu, struct controlComponent *ccb)
{
	showSaveGameMenu(menu, ccb);
	return 0;
}

int GamePlayMenu_PromoteToggle(struct menu *menu, struct controlComponent *ccb)
{
	char toggleArray_w[4] = { WHITE_Q, WHITE_R, WHITE_B, WHITE_N };
	char toggleArray_b[4] = { BLACK_Q, BLACK_R, BLACK_B, BLACK_N };
	char currTool = GetContentOfCoord(pBoard, ccb->btn->crd);

	if (currTool == WHITE_P)
	{
		setSlotInBoard(pBoard, ccb->btn->crd, WHITE_Q);
	}
	else if (currTool == BLACK_P)
	{
		setSlotInBoard(pBoard, ccb->btn->crd, BLACK_Q);
	}
	else
	{
		int i = 0;
		if (islower(currTool))
		{
			while (currTool != toggleArray_w[i])
			{
				i = mod(i + 1, 4);
			}
			setSlotInBoard(pBoard, ccb->btn->crd, toggleArray_w[mod(i + 1, 4)]);
		}
		else
		{
			while (currTool != toggleArray_b[i++])
			{
				i = mod(i + 1, 4);
			}
			setSlotInBoard(pBoard, ccb->btn->crd, toggleArray_b[mod(i + 1, 4)]);
		}
	}
	updateGUIBoard(pMenu_Game);
	return 1;
}


int SaveMenu_saveGameSlot(struct menu *menu, struct controlComponent *ccb)
{
	if (selectedSlot == '0')
		return 0;

	char filename[12] = "chessX.xml \0";
	*(filename + 5) = selectedSlot;
	Save(pBoard, filename);

	selectedSlot = '0';
	SaveOrLoad_Menu_UpdateVis(menu);

	return 1;
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
	char filenameUnpressed[12] = "slotX.bmp \0";
	char filenamePressed[13] = "slotXP.bmp \0";
	ControlComponent *ccbCurr = menu->panel_1->pnl->children->next;

	for (int i = 0; i < 7; i++)
	{
		if (ccbCurr->btn->pic != NULL)
			SDL_FreeSurface1(ccbCurr->btn->pic);
		*(filenameUnpressed + 4) = i + '1';
		*(filenamePressed + 4) = i + '1';
		ccbCurr->btn->pic = uploadPicture(i + '1' == selectedSlot ? filenamePressed : filenameUnpressed);
		if (SDL_BlitSurface(ccbCurr->btn->pic, NULL, chessWindow->self, &(ccbCurr->rect)) != 0)
		{
			SDL_FreeSurface1(ccbCurr->btn->pic);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			terminateProgram();
			return 0;
		}

		ccbCurr = ccbCurr->next;
	}
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
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
	properties[0] = GAME_MODE;
	showGamePlayMenu(menu, ccb);
	return 1;
}


int LoadMenu_Reset_LoadMenu_And_ShowMainMenu(struct menu *menu, struct controlComponent *ccb)
{
	selectedSlot = '0';
	SaveOrLoad_Menu_UpdateVis(menu);
	properties[0] = GAME_MODE;
	showMainMenu(menu, ccb);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int LoadGame(struct menu *menu, ControlComponent *buttonWhichPressCalledThisFunction)
{
	if (selectedSlot == '0')
		return 0;

	char filename[12] = "chessX.xml \0";
	*(filename + 5) = selectedSlot;
	LoadFromFile(filename, pBoard);

	selectedSlot = '0';
	properties[0] = GAME_MODE;

	if (properties[4] == WHITE_PLAYER)
	{
		whitePlayerTurnStage = NONE_SELECTED;
		blackPlayerTurnStage = WAITING;
	}
	else
	{
		whitePlayerTurnStage = WAITING;
		blackPlayerTurnStage = NONE_SELECTED;
	}

	selectedTool.i_coord = -1; selectedTool.j_coord = -1;

	SaveOrLoad_Menu_UpdateVis(chessWindow->shownMenu);
	updateGUIBoard(pMenu_Game);
	updateGUIBoard_Vis(chessWindow->shownMenu);
	showGamePlayMenu(pMenu_Game, NULL);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


int showMainMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	/* This functions as hard reset to all game */
	properties[0] = SETTINGS_MODE;
	init_board(pBoard);
	resetPlayerSelectionMenu();
	resetAISettingsMenu();

	selectedTool.i_coord = -1; selectedTool.j_coord = -1;
	promoteSquare.i_coord = -1; promoteSquare.j_coord = -1;

	showMenu(chessWindow, pMenu_Main);
	return 0;
}

int showPlayerSelectionMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_PlayerSelection);
	updateGUIBoard(pMenu_PlayerSelection);
	updateGUIBoard_Vis(pMenu_PlayerSelection);
	playerSelectionMenu_updateContinueOrPlayButton(chessWindow);


	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


int ShowHintMenu(struct menu *menu, struct controlComponent *ccb)
{
	if (properties[5] == PVC_MODE)
	{
		/* in PVC mode the toggle button for hint level is disabled */
		SDL_FreeSurface(pMenu_Hint->panel_3->pnl->children->btn->pic);
		pMenu_Hint->panel_3->pnl->children->btn->pic = uploadPicture("disabledHintToggle.bmp");
	}
	selectedTool.i_coord = -1; selectedTool.j_coord = -1;
	showMenu(chessWindow, pMenu_Hint);
	updateGUIBoard_Vis(pMenu_Hint);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int HintMenu_showGamePlayMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	showGamePlayMenu(menu, buttonWhichPressCalledThis);
	selectedTool.i_coord = -1; selectedTool.j_coord = -1;
	resetHighlights();
	updateGUIBoard(pMenu_Game);
	updateGUIBoard_Vis(pMenu_Game);
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


int HintMenu_toggleDifficulty(Menu *menu, struct controlComponent *ccb)
{
	if (properties[5] == PVC_MODE)
		return 1;
	hintLevel = mod(hintLevel + 1, 5);
	SDL_FreeSurface1(ccb->btn->pic);
	ccb->btn->pic = uploadPicture(getHintPicName());

	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}
	updateGUIBoard_Vis(menu);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int HintMenu_ShowHint(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{

	cMove *hintMove = NULL, hintMove_Copy, *pHintMove_Copy = &hintMove_Copy;
	//int scr;

	if (properties[5] == PVC_MODE)
		hintLevel = properties[2];

	// run minimax
	GameLabel(chessWindow, HINT_THINKING_H); //TODO: need to change to hintLabel
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}
	int oldHintLevel = hintLevel;
	int oldProperties_2 = properties[2];
	int best = oldHintLevel == 0 ? 1 : 0;

	properties[2] = hintLevel;
	if (properties[4] == WHITE_PLAYER)//if player's color is white
	{
		if (best)
		{
			hintLevel = GetBestDepth(pBoard, WHITE_PLAYER);
			properties[2] = hintLevel;
		}
		/*scr = */minimax_score(pBoard, WHITE_PLAYER, hintLevel, 1, &hintMove, MIN_VALUE, MAX_VALUE, best);
	}
	else
	{
		if (best)
		{
			hintLevel = GetBestDepth(pBoard, BLACK_PLAYER);
			properties[2] = hintLevel;
		}
		/*scr = */minimax_score(pBoard, BLACK_PLAYER, hintLevel, 1, &hintMove, MIN_VALUE, MAX_VALUE, best);
	}
	hintLevel = oldHintLevel;
	properties[2] = oldProperties_2;
	GameLabel(chessWindow, HINT_H);

	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	*pHintMove_Copy = *hintMove;
	pHintMove_Copy->next = NULL;
	freeMovesList(hintMove);
	highlightMovesList(pMenu_Hint, pHintMove_Copy->src, pHintMove_Copy);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


int showAI_SettingsMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_AI_settings);

	AI_Settings_updatePlayButton(chessWindow);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
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
				BlackKing = crd;
		}
	}
	if (properties[4] == WHITE_PLAYER)
	{
		whitePlayerTurnStage = NONE_SELECTED;
		blackPlayerTurnStage = WAITING;
	}
	else
	{
		whitePlayerTurnStage = WAITING;
		blackPlayerTurnStage = NONE_SELECTED;
	}
	int scr = score(pBoard, properties[4]);
	int threat = KingUnderThreat(pBoard, properties[4]);
	updateInfoLabels(scr, threat, properties[4] == WHITE_PLAYER ? whitePlayerTurnStage : blackPlayerTurnStage);

	showMenu(chessWindow, pMenu_Game);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int showSaveGameMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_Save);
	return 0;
}

int showLoadGameMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis)
{
	properties[0] = SETTINGS_MODE;
	showMenu(chessWindow, pMenu_Load);
	return 0;
}




int QuitGame(struct menu *menu, struct controlComponent *ccb)
{
	terminateProgram();
	return 0;
}



/* Button Handlers*/

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
}

int rightClicksHandler(Window *chessWindow, SDL_Event e)
{
	Menu *menu = chessWindow->shownMenu;
	ControlComponent *currChild;

	if (menu->identifier == PLAYER_SELECTION_MENU || menu->identifier == AI_SETTINGS_MENU || menu->identifier == GAME_PLAY_MENU)
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
						if (menu->identifier == PLAYER_SELECTION_MENU)
						{
							setSlotInBoard(pBoard, crd, EMPTY);
							playerSelectionMenu_updateContinueOrPlayButton(chessWindow);
							updateGUIBoard_Vis(menu);
							return 1;
						}
						else if (menu->identifier == AI_SETTINGS_MENU)
						{
							setSlotInBoard(pBoard, crd, EMPTY);
							AI_Settings_updatePlayButton(chessWindow);
							updateGUIBoard_Vis(menu);
							return 1;
						}
						else if (menu->identifier == GAME_PLAY_MENU)
						{
							int playerTurnStage = (properties[4] == WHITE_PLAYER) ? whitePlayerTurnStage : blackPlayerTurnStage;
							if (playerTurnStage == PROMOTE)
							{
								GamePlayMenu_PromoteToggle(pMenu_Game, currChild);
								updateGUIBoard_Vis(menu);
								return 1;
							}
						}
					}

				}
				currChild = currChild->next;
			}
		}
	}

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int pressIfNeeded(Menu *menu, struct controlComponent *ccb, SDL_Event e)
{
	/* Executes pressing of the button iff e indicates the button was pressed */
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

int resetPlayerSelectionMenu()
{
	SDL_FreeSurface1(pMenu_PlayerSelection->panel_2->pnl->children->btn->pic);
	pMenu_PlayerSelection->panel_2->pnl->children->btn->pic = uploadPicture("PVP.bmp");

	SDL_FreeSurface1(pMenu_PlayerSelection->panel_2->pnl->children->next->btn->pic);
	pMenu_PlayerSelection->panel_2->pnl->children->next->btn->pic = uploadPicture("NextPlayerWhite.bmp");

	SDL_FreeSurface1(pMenu_PlayerSelection->panel_3->pnl->children->btn->pic);
	pMenu_PlayerSelection->panel_3->pnl->children->btn->pic = uploadPicture("StartGame.bmp");

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int resetAISettingsMenu()
{
	SDL_FreeSurface1(pMenu_AI_settings->panel_2->pnl->children->btn->pic);
	pMenu_AI_settings->panel_2->pnl->children->btn->pic = uploadPicture("1Diff.bmp");

	SDL_FreeSurface1(pMenu_AI_settings->panel_2->pnl->children->next->btn->pic);
	pMenu_AI_settings->panel_2->pnl->children->next->btn->pic = uploadPicture("PlayerIsWhite.bmp");

	SDL_FreeSurface1(pMenu_AI_settings->panel_3->pnl->children->btn->pic);
	pMenu_AI_settings->panel_3->pnl->children->btn->pic = uploadPicture("StartGame.bmp");

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


int endGamePlay(Window *window)
{
	properties[0] = SETTINGS_MODE;
	return 1;
}

int freeMenu(Menu *menu)
{
	ControlComponent *ccl = (ControlComponent *)menu->header;
	if (ccl != NULL)
		SDL_FreeSurface1(ccl->lbl->pic);

	if (menu->panel_1 != NULL)
		freePanel(menu->panel_1);
	if (menu->panel_2 != NULL)
		freePanel(menu->panel_2);
	if (menu->panel_3 != NULL)
		freePanel(menu->panel_3);

	return 1;
}

int freePanel(ControlComponent *ccp)
{
	ControlComponent *child = ccp->pnl->children;
	while (child != NULL)
	{
		if (child->btn != NULL)
			SDL_FreeSurface1(child->btn->pic);
		else if (child->lbl != NULL)
			SDL_FreeSurface1(child->lbl->pic);

		child = child->next;
	}
	return 1;
}


int updateInfoLabels(int scr, int kingUnderThreat, int stageTurn)
{
	/* The score must be calculated for current player */

	SDL_FreeSurface1(pMenu_Game->panel_2->pnl->children->lbl->pic);
	SDL_FreeSurface1(pMenu_Game->panel_2->pnl->children->next->lbl->pic);
	if (scr == MATE_WIN_LOSE)
	{
		if (properties[4] == WHITE_PLAYER)
			pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_checkMate_BlackWins.bmp");
		else
			pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_checkMate_WhiteWins.bmp");
	}
	else if (scr == TIE_SCORE)
		pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_Tie.bmp");
	else if (kingUnderThreat)
	{
		if (properties[4] == WHITE_PLAYER)
			pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_WhitePlaysCheck.bmp");
		else
			pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_BlackPlaysCheck.bmp");
	}
	else
	{
		if (properties[4] == WHITE_PLAYER)
			pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_WhitePlayerTurn.bmp");
		else
			pMenu_Game->panel_2->pnl->children->lbl->pic = uploadPicture("info_BlackPlayerTurn.bmp");
	}

	if (stageTurn == PROMOTE)
	{
		pMenu_Game->panel_2->pnl->children->next->lbl->pic = uploadPicture("info_promote.bmp");
	}

	else
	{
		pMenu_Game->panel_2->pnl->children->next->lbl->pic = uploadPicture("info_makeMove.bmp");
	}
	drawLabelsOfPanel(pMenu_Game->panel_2->pnl->children);


	if (properties[1] == 1)
		return 0;
	else
		return 1;
}

int GameLabel(Window *window, int thinking)
{
	Menu *menu = window->shownMenu;
	ControlComponent *ccl_gameHeader = (ControlComponent*)menu->header;
	Label *lbl_gameHeader = ccl_gameHeader->lbl;

	//ControlComponent *ccl_info = menu->panel_2->pnl->children;
	//Label *lbl_infoTurn;

	char *filename;
	if (thinking == GAME_H)
		filename = "GameHeader.bmp";
	else if (thinking == GAME_THINKING_H)
		filename = "GameHeaderThinking.bmp";
	else if (thinking == HINT_THINKING_H)
		filename = "HintHeaderThinking.bmp";
	else if (thinking == HINT_H)
		filename = "HintHeader.bmp";

	SDL_FreeSurface1(lbl_gameHeader->pic);
	lbl_gameHeader->pic = uploadPicture(filename);


	if (SDL_BlitSurface(lbl_gameHeader->pic, NULL, chessWindow->self, &(ccl_gameHeader->rect)) != 0)
	{
		SDL_FreeSurface1(lbl_gameHeader->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	if (properties[1] == 1)
		return 0;
	else
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


	selectedTool = ccb->btn->crd;


	cMove *curr = moves;
	int count = 0;
	while (curr != NULL)
	{
		ccb = &(guiBoard[curr->dst.i_coord][curr->dst.j_coord]);

		highlightedSquares[count++] = ccb->btn->crd;

		curr = curr->next;
	}
	updateGUIBoard_Vis(menu);

	if (properties[1] == 1)
		return 0;
	else
		return 1;
}


/* Misc Logics Methods */

int isOfPlayer(int player, char tool)
{
	if ((player == WHITE_PLAYER && islower(tool)) || (player == BLACK_PLAYER && isupper(tool)))
		return 1;
	return 0;
}

int isValidBoardInitialization(board_t board)
{
	/* Returns 1 iff it is legal to start a game with the board setting */
	/* TODO: move this function to Chess_Logic.c */
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

int isThePromotedPiece(Coord crd)
{
	if (crd.i_coord == promoteSquare.i_coord && crd.j_coord == promoteSquare.j_coord)
		return 1;
	else
		return 0;
}

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

const char *getPlayerColorPicName()
{
	if (properties[3] == WHITE_PLAYER)
		return "PlayerIsWhite.bmp";
	else
		return "PlayerIsBlack.bmp";
}

const char *getDifficultyPicName(void)
{
	/* AI_Difficulty is between 0 and 4 inclusive */
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

const char *getHintPicName(void)
{
	/* AI_Difficulty is between 0 and 4 inclusive */
	if (hintLevel == 0)
		return "BestDiff.bmp";
	else if (hintLevel == 1)
		return "1Diff.bmp";
	else if (hintLevel == 2)
		return "2Diff.bmp";
	else if (hintLevel == 3)
		return "3Diff.bmp";
	else
		return "4Diff.bmp";
}



eTool e_getInitialTypeOfCoord(Coord crd)
{
	/* Pre: coord is in board */
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

int getInitialPlayerOfCoord(Coord crd)
{
	/* Pre: crd is in the board */
	if (crd.j_coord == 0 || crd.j_coord == 1)
		return WHITE_PLAYER;
	else if (crd.j_coord == 6 || crd.j_coord == 7)
		return BLACK_PLAYER;
	else
		return NO_PLAYER;
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

int resetHighlights(void)
{
	int i = 0;
	while (isInBoard(highlightedSquares[i]) && i < BOARD_SIZE * BOARD_SIZE)
	{
		highlightedSquares[i++] = GenerateCoord(-1, -1);
	}
	return 1;
}
