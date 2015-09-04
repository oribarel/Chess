#ifndef	CHESS_WINDOWS_H
#define CHESS_WINDOWS_H

#include "Chess_Controls.h"

/*#define SCREEN_W 1024
#define SCREEN_H 768*/

typedef ControlComponent ** board_g;

#define MAIN_MENU				0
#define PLAYER_SELECTION_MENU	1
#define AI_SETTINGS_MENU		2
#define GAME_PLAY_MENU			3


#define SETTINGS_MODE 1
#define GAME_MODE 0
#define PVC_MODE 2
#define PVP_MODE 1


#define NEW_GAME_BUTTON								'n'
#define LOAD_GAME_BUTTON							'l'
#define QUIT_GAME_BUTTON							'q'

#define START_GAME_BUTTON							'g'
#define GAME_MODE_TOGGLE_BUTTON						'm'
#define NEXT_PLAYER_TOGGLE_BUTTON					's'
#define PLAYER_SELECTION_CONTINUE_OR_START			'o'

#define BACK_TO_MAIN_MENU_BUTTON					'M'
#define BACK_TO_PLAYER_SELECTION_BUTTON				'P'

#define AI_DIFF_TOGGLE_BUTTON						'd'
#define AI_PLAYER_COLOR_TOGGLE_BUTTON				'c'

#define END_GAME_AND_RETURN_TO_MAIN_MENU_BUTTON		'e'
#define SAVE_GAME_BUTTON							'S'

#define SQUARE_BUTTON								'a'

#define WAITING			0
#define NONE_SELECTED	1
#define HIGHLIGHTED		2
#define MOVE_MADE		3
#define PROMOTE			4



extern ControlComponent *guiBoard[BOARD_SIZE];
extern ControlComponent guiBoard_col0[BOARD_SIZE];
extern ControlComponent guiBoard_col1[BOARD_SIZE];
extern ControlComponent guiBoard_col2[BOARD_SIZE];
extern ControlComponent guiBoard_col3[BOARD_SIZE];
extern ControlComponent guiBoard_col4[BOARD_SIZE];
extern ControlComponent guiBoard_col5[BOARD_SIZE];
extern ControlComponent guiBoard_col6[BOARD_SIZE];
extern ControlComponent guiBoard_col7[BOARD_SIZE];

extern RGB rgbMenuBlue;
extern RGB rgbBlack;
extern RGB rgbWhite;
extern RGB rgbRed;
extern RGB rgbOrange;
extern RGB rgbGreen;
extern RGB rgbPurple;

extern Menu *pMenu_Main;
extern Menu *pMenu_PlayerSelection;
extern Menu *pMenu_AI_settings;
extern Menu *pMenu_Game;
//extern Menu *currMenu;

extern Coord selectedTool;

int passTheBoard(board_t board);

/* Create Menus Functions */
int createMainMenu(Menu *mainMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);
int createPlayerSelectionMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);
int createAI_SettingsMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);
int createGameMenu(Menu *AI_SettingsMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);

/* mainMenu */
int LoadGame(Window *window, ControlComponent *);
int quitGame(Window *window, ControlComponent *);
int QuitGame(Menu *menu, ControlComponent *buttonWhichPressCAlledThis);

/* playerSelectionMenu */
int playerSelectionMenu_toggleTool(Menu *menu, struct controlComponent *);
int playerSelectionMenu_toggleGameMode(Menu *menu, struct controlComponent *);
int playerSelectionMenu_toggleNextPlayer(Menu *menu, struct controlComponent *);
int playerSelectionMenu_updateContinueOrPlayButton(Window *window);

/* AI_settingsMenu  */
const char *getDifficultyPicName(void);
int AI_settingsMenu_toggleDifficulty(Menu *menu, struct controlComponent *);
int AI_settingsMenu_togglePlayerColor(Menu *menu, struct controlComponent *);
int AI_SettingsMenu_toggleTool(Menu *menu, struct controlComponent *ccb);
int AI_Settings_updatePlayButton(Window *window);


/* GamePlayMenu */
int GamePlayMenu_SaveGame(struct menu *menu, struct controlComponent *ccb);
int GamePlayMenu_endGameAndShowMainMenu(struct menu *menu, struct controlComponent *ccb);

/* Game Play Functions */
int pawnHighlight(struct menu *menu, struct controlComponent *ccb);
int knightHighlight(struct menu *menu, struct controlComponent *ccb);
int bishopHighlight(struct menu *menu, struct controlComponent *ccb);
int rookHighlight(struct menu *menu, struct controlComponent *ccb);
int queenHighlight(struct menu *menu, struct controlComponent *ccb);
int kingHighlight(struct menu *menu, struct controlComponent *ccb);
int changeHighlightedTool(struct menu *menu, struct controlComponent *ccb);
int gui_makeMove(struct menu *menu, struct controlComponent *ccb);
int highlightMovesList(Menu *menu, Coord crd, cMove *moves);
int advanceTurnStage(int promotiveSituation);

/* Button handling */
int buttonPressHandler(Window *window, SDL_Event e);
int rightClicksHandler(Window *chessWindow, SDL_Event e);
int pressIfNeeded(Menu *menu, struct controlComponent *ccb, SDL_Event e);
int isPressInsideButton(SDL_Event e, ControlComponent *ccb);

/* Show menus */
int showMenu(Window *window, Menu *menu);

int showMainMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis);
int showPlayerSelectionMenu(Menu *menu, ControlComponent *buttonWhichPressCAlledThis);
int showAI_SettingsMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis);
int showGamePlayMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis);
int showLoadGameMenu(Menu *menu, ControlComponent *buttonWhichPressCAlledThis);

/* GUI Board */
int panelMaker(ControlComponent *ccp, Panel *pnl, SDL_Rect rect, RGB color);
int createGUIBoard(board_g gBoard, ControlComponent *ccp_BoardSetting, Button *btn_BoardPanelButtons, board_t board);
int updateGUIBoard(Menu *menu);
int updateGUIBoard_Vis(Menu *menu);
btnFunc getGameFunctionOfCoord(Coord crd);


/* Misc */
int isOfPlayer(int player, char tool);
int freeMovesList(cMove *move);
eTool get_eToolFromType(char type);

#endif
