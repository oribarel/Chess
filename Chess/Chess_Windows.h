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

int CreateMainWindow(Window *window, board_t passedBoard);
int createPlayerSelectionWindow(Window *window, ControlComponent *);
int createAI_SettingsWindow(Window *window, ControlComponent *);
int CreateGameWindow(Window *window, ControlComponent *);


int LoadGame(Window *window, ControlComponent *);
int quitGame(Window *window, ControlComponent *);

int setGameModePVC(Window *window, ControlComponent *);
int setGameModePVP(Window *window, ControlComponent *);

int setNextPlayerWhite(Window *window, ControlComponent *);
int setNextPlayerBlack(Window *window, ControlComponent *);

int playerSelectionMenu_toggleTool(Window *window, struct controlComponent *);
int playerSelectionMenu_toggleGameMode(Window *window, struct controlComponent *);
int playerSelectionMenu_toggleNextPlayer(Window *window, struct controlComponent *ccb);

int playerSelectionMenu_updateContinueOrPlayButton(Window *window);
#endif
