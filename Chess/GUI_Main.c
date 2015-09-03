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
	
	/* Initialize Colors */

	rgbMenuBlue = createRGB(0, 51, 102);
	rgbBlack = createRGB(0, 0, 0);
	rgbWhite = createRGB(255, 255, 255);
	rgbRed = createRGB(153, 0, 0);
	rgbOrange = createRGB(255, 128, 0);
	rgbGreen = createRGB(0, 153, 0);
	rgbPurple = createRGB(127, 0, 255);

	

	/* Main Menu  CCPs, PNLs, BTNs*/
	ControlComponent ccp_MainMenu, *pCCP_MainMenu = &ccp_MainMenu;
	Panel pnl_MainMenu, *pPnl_MainMenu = &pnl_MainMenu;

	ControlComponent ccb_mainMenuCCBs[NUM_OF_MAIN_MENU_BUTTONS];
	
	Button btn_mainMenuButtons[NUM_OF_MAIN_MENU_BUTTONS];
	

	/* Player Selection Menu CCPs, PNLs,CCBs, BTNs*/
	ControlComponent ccp_PlayerSelectionMenuCCPs[NUM_OF_PLAYER_SELECTION_MENU_PANELS];
	Panel pnl_PlayerSelectionMenuPanels[NUM_OF_PLAYER_SELECTION_MENU_PANELS];
	
	ControlComponent ccb_PlayerSelectionMenuCCBs[NUM_OF_PLAYER_SELECTION_MENU_BUTTONS];
	Button btn_PlayerSelectionMenuButtons[NUM_OF_PLAYER_SELECTION_MENU_BUTTONS];
	
	
	/* AI_Settings Menu CCPs, PNLs, BTNs */
	ControlComponent ccp_AI_SettingsMenuCCPs[NUM_OF_AI_MENU_PANELS];
	Panel pnl_AI_SettingsMenuPanels[NUM_OF_AI_MENU_PANELS];

	ControlComponent ccb_AI_SettingsCCBs[NUM_OF_AI_MENU_BUTTONS];
	Button btn_AI_SettingsButtons[NUM_OF_AI_MENU_BUTTONS];

	

	/* Game Play Menu CCPs, PNLs, BTNs */
	ControlComponent ccp_GamePlayMenuCCPs[NUM_OF_GAME_PLAY_MENU_PANELS];
	Panel pnl_GamePlayMenuPanels[NUM_OF_GAME_PLAY_MENU_PANELS];

	ControlComponent ccb_GamePlayMenuCCBs[NUM_OF_GAME_PLAY_MENU_BUTTONS];
	Button btnb_GamePlayMenuButtons[NUM_OF_GAME_PLAY_MENU_BUTTONS];
	

	/* Board Panel CCPs, PNLs, BTNs*/
	ControlComponent ccp_Board, *pCCP_Board = &ccp_Board;
	Panel pnl_Board, *pPnl_Board = &pnl_Board;

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;
	
	SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);
	panelMaker(pCCP_Board, pPnl_Board, boardSettingPanel,rgbPurple);
	
	ControlComponent ccb_BoardPanelCCBs[4];
	Button btnb_BoardPanelButtons[4];
	
	/* Pass the board */
	pBoard = passedBoard;
	init_board(pBoard);
	passTheBoard(passedBoard);
	
	

	
	/* Initialize SDL Video */
	init_GUI();
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}

	/* Initialize Gui Board */

	guiBoard[0] = guiBoard_col0;
	guiBoard[1] = guiBoard_col1;
	guiBoard[2] = guiBoard_col2;
	guiBoard[3] = guiBoard_col3;
	guiBoard[4] = guiBoard_col4;
	guiBoard[5] = guiBoard_col5;
	guiBoard[6] = guiBoard_col6;
	guiBoard[7] = guiBoard_col7;

	

	/* Create the window */
	createWindow(chessWindow);
	
	/* Create Menus */
	createMainMenu(pMenu_Main, pCCP_MainMenu, pPnl_MainMenu, ccb_mainMenuCCBs, btn_mainMenuButtons);
	createPlayerSelectionMenu(pMenu_PlayerSelection,ccp_PlayerSelectionMenuCCPs,pnl_PlayerSelectionMenuPanels,ccb_PlayerSelectionMenuCCBs,btn_PlayerSelectionMenuButtons);
	createAI_SettingsMenu(pMenu_AI_settings,ccp_AI_SettingsMenuCCPs,pnl_AI_SettingsMenuPanels,ccb_AI_SettingsCCBs,btn_AI_SettingsButtons);
	createGameMenu(pMenu_Game,ccp_GamePlayMenuCCPs,pnl_GamePlayMenuPanels,ccb_GamePlayMenuCCBs,btnb_GamePlayMenuButtons);
	
	createGUIBoard(guiBoard, pCCP_Board, pBoard);
	addPanelToMenu(pMenu_PlayerSelection, pCCP_Board,4);
	addPanelToMenu(pMenu_AI_settings, pCCP_Board,4);
	addPanelToMenu(pMenu_Game, pCCP_Board, 4);

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
				//printf("%s", "SDL_MOUSEBUTTONUP does works. \n");
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