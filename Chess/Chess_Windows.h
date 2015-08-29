#ifndef	CHESS_WINDOWS_H
#define CHESS_WINDOWS_H

#include "Chess_Controls.h"

#define SCREEN_W 1024
#define SCREEN_H 768

int CreateFirstSettingsWindow(Window *window);
int LoadGame(Window *window);
int quitGame(Window *window);

#endif
