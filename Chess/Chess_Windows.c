#include "Chess_Windows.h"

static int quit = 0;

SDL_Rect create1024x768Rect()
{
	SDL_Rect Rect1024x768 = createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	return Rect1024x768;
}

/* The main function of the GUI aspect of the chess game.*/
int CreateMainWindow(Window *window)
{
	SDL_Rect Rect1024x768 = create1024x768Rect(); //createSDL_Rect(SCREEN_W, SCREEN_W, 0, 0);
	SDL_Rect mainWindowButton = createSDL_Rect(300, 150, 150, 150);
	RGB ageanBlue = createRGB(151, 197, 209);

	ControlComponent *ccp_MainWindow = createPanel(Rect1024x768, ageanBlue);
	ControlComponent *ccb_NewGame = createButton(mainWindowButton, uploadPicture("NewGameButton.bmp"), CreateFirstSettingsWindow);

	mainWindowButton.y += 300;
	ControlComponent *ccb_LoadGame = createButton(mainWindowButton, uploadPicture("LoadGameButton.bmp"), LoadGame);

	mainWindowButton.y += 300;
	ControlComponent *ccb_Quit = createButton(mainWindowButton, uploadPicture("QuitGameButton.bmp"), quitGame);

	freeBoardAndTree(window);
	addPanelToWindow(window, ccp_MainWindow);
	addButtonToPanel(ccp_MainWindow, ccb_NewGame, window);
	addButtonToPanel(ccp_MainWindow, ccb_LoadGame, window);
	addButtonToPanel(ccp_MainWindow, ccb_Quit, window);

	while (quit == 0)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && quit == 0)
		{
			if (e.type == SDL_QUIT)
			{
				freeCCTree(window->panelChild);
				window->panelChild = NULL;
				SDL_FreeSurface(window->self);
				free(window);
				quit = 1;
				break;
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				buttonPressHandler(window, ccp_MainWindow, e);
				break;
			}
		}
	}
	return 0;
}

int CreateFirstSettingsWindow(Window *window)
{
	return 0;
}

int PlayerVsPlayer(Window *window)
{
	return 0;
}

int CreateSecondSettingsWindow(Window *window)
{
	return 0;
}

int RebuildSettingsMenu(Window *window, ControlComponent **second_settings_window, 
	ControlComponent **choose_color, ControlComponent **white, ControlComponent **black,
	ControlComponent **choose_difficulty, ControlComponent **one, ControlComponent **two,
	ControlComponent **three, ControlComponent **four, ControlComponent **best,
	ControlComponent **start_game)
{
	return 0;
}

int SetPlayerAs(Window *window, int WhiteOrBlackPlayer)
{
	return 0;
}

/* difficulty should be 1 to 4 or 0 for best */
int setDifficulty(Window *window, int difficulty)
{
	return 0;
}

int LoadGame(Window *window)
{
	return 0;
}

int Load(Window *window, char *path)
{
	return 0;
}

int SetFromFileGui(char* path)
{
	return 0;
}

int loadGameNum(Window *window, int n)
{
	return 0;
}

int SaveGame(Window *window)
{
	return 0;
}

int SaveGUI(char* file_name)
{
	return 0;
}

int saveGameNum(Window *window, int n)
{
	return 0;
}

int BackFromSave(Window *window)
{
	return 0;
}

int inBoardBoundaries(Pixel px)
{
	return 0;
}

int colorByPixel(char** board, Pixel px)
{
	return 0;
}

int CreateGameWindow(Window *window)
{
	return 0;
}

int RebuildGameMenu(Window *window, ControlComponent **full_panel, ControlComponent **menu_panel, ControlComponent **restart, ControlComponent **save_game, ControlComponent **main_menu, ControlComponent **quit_b)
{
	return 0;
}

int CreateGame(Window *window, ControlComponent *panel_node, char** current_board)
{
	return 0; 
}

int SendErrorMessage(Window *window)
{
	return 0;
}

int ExecutePawnPromotion(Window *window)
{
	return 0;
}

int SetPromotionAs(Window *window, eTool type)
{
	return 0;
}

int restartGame(Window *window)
{
	return 0;
}

int buttonPressHandler(Window *window, ControlComponent *ccp, SDL_Event e)
{
	ControlComponent *currChild = ccp->pnl->children;
	ControlComponent *tmpChild;

	int pressNeeded = 1;

	while (currChild != NULL)
	{
		if (currChild->pnl != NULL) //if the currChild is a ccp
		{
			tmpChild = currChild->pnl->children;
			while (tmpChild != NULL)
			{
				if (tmpChild->btn != NULL)
				{
					pressNeeded = buttonPress(window, tmpChild, e);
					if (pressNeeded == 0)
					{
						return 1;
					}
				}
				tmpChild = tmpChild->next;
			}
		}
		else if (currChild->btn != NULL) // if the currChild is a ccb
		{
			pressNeeded = buttonPress(window, currChild, e);
			if (pressNeeded == 0)
			{
				return 1;
			}
		}
		currChild = currChild->next;
	}
	return 0;
}

/* Executes pressing of the button if and only if e indicates that button was pressed */
int buttonPress(Window *window, ControlComponent *ccb, SDL_Event e)
{
	if (isPressInsideButton(e, ccb))
	{
		ccb->btn->f(window);
	}
	return 0;
}

int isPressInsideButton(SDL_Event e, ControlComponent *ccb)
{
	if ((e.button.x > ccb->rect.x)					&&
		(e.button.x < ccb->rect.x + ccb->rect.w)	&&
		(e.button.y > ccb->rect.y)					&&
		(e.button.y < ccb->rect.y + ccb->rect.h))
		return 1;
	else
		return 0;
}

int freeBoardAndTree(Window *window)
{
	return 0;
}

int quitGame(Window *window)
{
	return 0;
}