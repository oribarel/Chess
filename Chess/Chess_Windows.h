#ifndef	CHESS_WINDOWS_H
#define CHESS_WINDOWS_H

#include "Chess_Controls.h"

/*#define SCREEN_W 1024
#define SCREEN_H 768*/

typedef ControlComponent ** board_g;

#define MAIN_MENU				0
#define PLAYER_SELECTION_MENU	1
#define AI_SETTINGS_MENU		2
#define GAME_MENU				3

#define PVC_MODE 1
#define PVP_MODE 0


#define NEW_GAME_BUTTON 'n'
#define LOAD_GAME_BUTTON 'l'
#define QUIT_GAME_BUTTON 'q'

#define START_GAME_BUTTON 'g'
#define GAME_MODE_TOGGLE_BUTTON 'm'
#define NEXT_PLAYER_TOGGLE_BUTTON 's'
#define PLAYER_SELECTION_CONTINUE_OR_START 'o'

#define BACK_TO_MAIN_MENU_BUTTON 'M'
#define BACK_TO_PLAYER_SELECTION_BUTTON 'P'

#define AI_DIFF_TOGGLE_BUTTON 'd'
#define AI_PLAYER_COLOR_TOGGLE_BUTTON 'c'

#define SQUARE_BUTTON 'a'


extern int intQuit;

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


/*int CreateMainWindow(Window *window, board_t passedBoard);
int createPlayerSelectionWindow(Window *window, ControlComponent *);
int createAI_SettingsWindow(Window *window, ControlComponent *);
int CreateGameWindow(Window *window, ControlComponent *);*/

int passTheBoard(board_t board);

int createMainMenu(Menu *mainMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);
int createPlayerSelectionMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);
int createAI_SettingsMenu(Menu *playerSelectionMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);
int createGameMenu(Menu *AI_SettingsMenu, ControlComponent *ccps, Panel *panel, ControlComponent *ccbs, Button *btns);


int LoadGame(Window *window, ControlComponent *);
int quitGame(Window *window, ControlComponent *);


int playerSelectionMenu_toggleTool(Menu *menu, struct controlComponent *);
int playerSelectionMenu_toggleGameMode(Menu *menu, struct controlComponent *);
int playerSelectionMenu_toggleNextPlayer(Menu *menu, struct controlComponent *);
int playerSelectionMenu_updateContinueOrPlayButton(Window *window);

int AI_settingsMenu_toggleDifficulty(Menu *menu, struct controlComponent *);
int AI_settingsMenu_togglePlayerColor(Menu *menu, struct controlComponent *);
int AI_SettingsMenu_toggleTool(Menu *menu, struct controlComponent *ccb);
int AI_Settings_updatePlayButton(Window *window);


int buttonPressHandler(Window *window, SDL_Event e);
int pressIfNeeded(Menu *menu, struct controlComponent *ccb, SDL_Event e);
int isPressInsideButton(SDL_Event e, ControlComponent *ccb);

int showMenu(Window *window, Menu *menu);

int showMainMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis);
int showPlayerSelectionMenu(Menu *menu, ControlComponent *buttonWhichPressCAlledThis);
int showAI_SettingsMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis);
int showLoadGameMenu(Menu *menu, ControlComponent *buttonWhichPressCAlledThis);
int showGamePlayMenu(Menu *menu, ControlComponent *buttonWhichPressCalledThis);

int createGuiBoard(board_g gBoard, ControlComponent *ccp_BoardSetting, board_t board);
int panelMaker(ControlComponent *ccp, Panel *pnl, SDL_Rect rect, RGB color);

int QuitGame(Menu *menu, ControlComponent *buttonWhichPressCAlledThis);



const char *getDifficultyPicName(void);
#endif
