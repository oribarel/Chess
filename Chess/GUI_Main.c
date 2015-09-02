#include "GUI_Main.h"

board_t pBoard;
Window window, *chessWindow = &window;

Menu menu_Main, *pMenu_Main = &menu_Main;
Menu menu_PlayerSelection, *pMenu_PlayerSelection = &menu_PlayerSelection;
Menu menu_AI_settings, *pMenu_AI_settings = &menu_AI_settings;
Menu menu_Game, *pMenu_Game = &menu_Game;
//Menu *currMenu = NULL;

/* Returns 0 on failure to init sdl video and 1 otherwise.*/
int GUI_Main(board_t passedBoard)
{

	

	/* Main Menu  CCPs, PNLs, BTNs*/
	ControlComponent ccp_MainMenu, *pCCP_MainMenu = &ccp_MainMenu;
	Panel pnl_MainMenu, *pPnl_MainMenu = &pnl_MainMenu;

	ControlComponent ccb_mainMenuCCBs[3];
	
	Button btn_mainMenuButtons[3];
	

	/* Player Selection Menu CCPs, PNLs,CCBs, BTNs*/
	ControlComponent ccp_PlayerSelectionMenuCCPs[3];
	Panel pnl_PlayerSelectionMenuPanels[3];
	
	ControlComponent ccb_PlayerSelectionMenuCCBs[4];
	Button btn_PlayerSelectionMenuButtons[4];
	
	
	/* AI_Settings Menu CCPs, PNLs, BTNs */
	ControlComponent ccp_AI_SettingsMenuCCPs[3];
	Panel pnl_AI_SettingsMenuPanels[3];

	ControlComponent ccb_AI_SettingsCCBs[5];
	Button btn_AI_SettingsButtons[5];

	

	/* Game Play Menu CCPs, PNLs, BTNs */
	ControlComponent ccp_GamePlayMenuCCPs[1];
	Panel pnl_GamePlayMenuPanels[1];

	ControlComponent ccb_GamePlayMenuCCBs[2];
	Button btnb_GamePlayMenuButtons[2];
	

	/* Board Panel CCPs, PNLs, BTNs*/
	ControlComponent ccp_Board, *pCCP_Board = &ccp_Board;
	Panel pnl_Board, *pPnl_Board = &pnl_Board;

	ControlComponent ccb_BoardPanelCCBs[4];
	Button btnb_BoardPanelButtons[4];

	/* Initialize Gui Board */

	guiBoard[0] = guiBoard_col0;
	guiBoard[1] = guiBoard_col1;
	guiBoard[2] = guiBoard_col2;
	guiBoard[3] = guiBoard_col3;
	guiBoard[4] = guiBoard_col4;
	guiBoard[5] = guiBoard_col5;
	guiBoard[6] = guiBoard_col6;
	guiBoard[7] = guiBoard_col7;

	/* Initialize Colors */

	rgbMenuBlue = createRGB(0, 51, 102);
	rgbBlack = createRGB(0, 0, 0);
	rgbWhite = createRGB(255, 255, 255);
	rgbRed = createRGB(153, 0, 0);
	rgbOrange = createRGB(255, 128, 0);
	rgbGreen = createRGB(0, 153, 0);
	rgbPurple = createRGB(127, 0, 255);

	/* Initialize SDL Video */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}

	/* Pass the board */
	pBoard = passedBoard;
	init_board(pBoard);
	passTheBoard(passedBoard);

	/* Create the window */
	createWindow(chessWindow);
	
	/* Create Menus */
	createMainMenu(pMenu_Main, pCCP_MainMenu, pPnl_MainMenu, ccb_mainMenuCCBs, btn_mainMenuButtons);
	createPlayerSelectionMenu(pMenu_PlayerSelection,ccp_PlayerSelectionMenuCCPs,pnl_PlayerSelectionMenuPanels,ccb_PlayerSelectionMenuCCBs,btn_PlayerSelectionMenuButtons);
	createAI_SettingsMenu(pMenu_AI_settings,ccp_AI_SettingsMenuCCPs,pnl_AI_SettingsMenuPanels,ccb_AI_SettingsCCBs,btn_AI_SettingsButtons);
	createGameMenu(pMenu_Game,ccp_GamePlayMenuCCPs,pnl_GamePlayMenuPanels,ccb_GamePlayMenuCCBs,btnb_GamePlayMenuButtons);
	
	/* Start */
	showMenu(chessWindow, pMenu_Main);
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	while (!intQuit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && intQuit == 0)
		{
			if (e.type == SDL_QUIT)
			{
				/*freeCCTree(window->panelChild);
				window->panelChild = NULL;
				SDL_FreeSurface(window->self);
				free(window);
				intQuit = 1;
				break;*/
			}
			// add right click https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlmousebuttonevent.html?
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				printf("%s", "SDL_MOUSEBUTTONUP does works. \n");
				buttonPressHandler(chessWindow, e);
				//break;
			}
			
			if (SDL_Flip(chessWindow->self) != 0)
			{
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				quit();
				return 0;
			}
		}
	}
	return 1;
}