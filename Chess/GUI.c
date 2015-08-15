#include <SDL.h>
#include <SDL_video.h>
#include "GUI.h"

int Init_GUI(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Quit();

	return 0;
}