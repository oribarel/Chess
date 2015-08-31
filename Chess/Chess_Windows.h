#ifndef	CHESS_WINDOWS_H
#define CHESS_WINDOWS_H

#include "Chess_Controls.h"

/*#define SCREEN_W 1024
#define SCREEN_H 768*/

typedef ControlComponent ** board_g;


int CreateMainWindow(Window *window, board_t passedBoard);
int createPlayerSelectionWindow(Window *window);
int LoadGame(Window *window);
int quitGame(Window *window);

int setGameModePVC(Window *window);
int setGameModePVP(Window *window);

int setNextPlayerWhite(Window *window);
int setNextPlayerBlack(Window *window);
int startGameOrCreateAI_Settings(Window *window);

#endif
