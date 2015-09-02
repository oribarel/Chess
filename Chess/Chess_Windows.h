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

#define BACK_TO_MAIN_MENU_BUTTON 'M'
#define BACK_TO_PLAYER_SELECTION_BUTTON 'P'

#define AI_DIFF_TOGGLE_BUTTON 'd'
#define AI_PLAYER_COLOR_TOGGLE_BUTTON 'c'

#define SQUARE_BUTTON 'a'


int CreateMainWindow(Window *window, board_t passedBoard);
int createPlayerSelectionWindow(Window *window, ControlComponent *);
int createAI_SettingsWindow(Window *window, ControlComponent *);
int CreateGameWindow(Window *window, ControlComponent *);


int LoadGame(Window *window, ControlComponent *);
int quitGame(Window *window, ControlComponent *);


int playerSelectionMenu_toggleTool(Window *window, struct controlComponent *);
int playerSelectionMenu_toggleGameMode(Window *window, struct controlComponent *);
int playerSelectionMenu_toggleNextPlayer(Window *window, struct controlComponent *);
int playerSelectionMenu_updateContinueOrPlayButton(Window *window);

int AI_settingsMenu_toggleDifficulty(Window *window, struct controlComponent *);
int AI_settingsMenu_togglePlayerColor(Window *window, struct controlComponent *);
int AI_SettingsMenu_toggleTool(Window *window, struct controlComponent *ccb);
int AI_Settings_updatePlayButton(Window *window);


const char *getDifficultyPicName(void);
#endif
