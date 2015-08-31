#include "GUI_Main.h"

/* Returns 0 on failure to init sdl video and 1 otherwise.*/
int GUI_Main(board_t passedBoard)
{

	Window *window;
	
	board_t board = passedBoard;


	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}

	window = createWindow();//SCREEN_W, SCREEN_H);
	CreateMainWindow(window, board);
	return 1;
}