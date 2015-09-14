#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "Chess_Windows.h"


#define NUM_OF_MAIN_MENU_PANELS					1
#define NUM_OF_MAIN_MENU_BUTTONS				3

#define NUM_OF_PLAYER_SELECTION_MENU_PANELS		3
#define NUM_OF_PLAYER_SELECTION_MENU_BUTTONS	4

#define NUM_OF_AI_MENU_PANELS					3
#define NUM_OF_AI_MENU_BUTTONS					5

#define NUM_OF_GAME_PLAY_MENU_PANELS			4
#define NUM_OF_GAME_PLAY_MENU_BUTTONS			4

#define NUM_OF_SAVE_MENU_PANELS					2
#define NUM_OF_SAVE_MENU_BUTTONS				3

#define NUM_OF_LOAD_MENU_PANELS					2
#define NUM_OF_LOAD_MENU_BUTTONS				2


#define NUMBER_OF_SLOTS_FOR_LOAD_AND_SAVE		7

int GUI_Main(board_t board);

#endif
