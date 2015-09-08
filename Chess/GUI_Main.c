#include "GUI_Main.h"

board_t pBoard;
Window window, *chessWindow = &window;

Menu menu_Main, *pMenu_Main = &menu_Main;
Menu menu_PlayerSelection, *pMenu_PlayerSelection = &menu_PlayerSelection;
Menu menu_AI_settings, *pMenu_AI_settings = &menu_AI_settings;
Menu menu_Game, *pMenu_Game = &menu_Game;
Menu menu_Save, *pMenu_Save = &menu_Save;
Menu menu_Load, *pMenu_Load = &menu_Load;
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

	/* Initialize default properties: */
	properties[0] = 1; //settings
	properties[1] = 0; //not-quit
	properties[2] = 1; //AI default depth
	properties[3] = WHITE_PLAYER; //default player color
	properties[4] = WHITE_PLAYER; //default starting player
	properties[5] = PVP_MODE; //default game mode


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
	panelMaker(pCCP_Board, pPnl_Board, boardSettingPanel, rgbPurple);

	ControlComponent ccb_BoardPanelCCBs[4]; //These are for promoting purposes
	Button btn_BoardPanelButtons[BOARD_SIZE*BOARD_SIZE + 4]; // these are 64 buttons for the squares and 4 for the promoting purpose.

	/* Save Menu  CCPs, PNLs, BTNs */
	ControlComponent ccp_SaveMenuCCPs[NUM_OF_SAVE_MENU_PANELS];
	Panel pnl_SaveMenuPanels[NUM_OF_SAVE_MENU_PANELS];

	ControlComponent ccb_SaveMenuCCBs[NUM_OF_SAVE_MENU_BUTTONS];
	Button btn_SaveMenuButtons[NUM_OF_SAVE_MENU_BUTTONS];

	/* Load Menu  CCPs, PNLs, BTNs */
	ControlComponent ccp_LoadMenuCCPs[NUM_OF_LOAD_MENU_PANELS];
	Panel pnl_LoadMenuPanels[NUM_OF_LOAD_MENU_PANELS];

	ControlComponent ccb_LoadMenuCCBs[NUM_OF_LOAD_MENU_BUTTONS];
	Button btn_LoadMenuButtons[NUM_OF_LOAD_MENU_BUTTONS];

	/* Pass the board */
	pBoard = passedBoard;
	init_board(pBoard);
	passTheBoard(passedBoard);

	/* cMoves */
	cMove *computerMove = NULL;
	cMove *allPossibleMoves = NULL, *tmp;


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
	createPlayerSelectionMenu(pMenu_PlayerSelection, ccp_PlayerSelectionMenuCCPs, pnl_PlayerSelectionMenuPanels, ccb_PlayerSelectionMenuCCBs, btn_PlayerSelectionMenuButtons);
	createAI_SettingsMenu(pMenu_AI_settings, ccp_AI_SettingsMenuCCPs, pnl_AI_SettingsMenuPanels, ccb_AI_SettingsCCBs, btn_AI_SettingsButtons);
	createGameMenu(pMenu_Game, ccp_GamePlayMenuCCPs, pnl_GamePlayMenuPanels, ccb_GamePlayMenuCCBs, btnb_GamePlayMenuButtons);
	createSaveMenu(pMenu_Save, ccp_SaveMenuCCPs, pnl_SaveMenuPanels, ccb_SaveMenuCCBs, btn_SaveMenuButtons);
	createLoadMenu(pMenu_Load, ccp_LoadMenuCCPs, pnl_LoadMenuPanels, ccb_LoadMenuCCBs, btn_LoadMenuButtons);


	createGUIBoard(guiBoard, pCCP_Board, btn_BoardPanelButtons, pBoard);
	addPanelToMenu(pMenu_PlayerSelection, pCCP_Board, 4);
	addPanelToMenu(pMenu_AI_settings, pCCP_Board, 4);
	addPanelToMenu(pMenu_Game, pCCP_Board, 4);


	/* Start */
	showMenu(chessWindow, pMenu_Main);
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	while (!properties[1])
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && properties[1] == 0)
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
				if (e.button.button == SDL_BUTTON_LEFT)
					buttonPressHandler(chessWindow, e);
				else if (e.button.button == SDL_BUTTON_RIGHT)
					rightClicksHandler(chessWindow, e);
			}

			if (SDL_Flip(chessWindow->self) != 0)
			{
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				quit();
				return 0;
			}

			///* Update the Kings Coordinates*/
			//if (properties[0] == GAME_MODE)
			//{
			//	Coord crd;
			//	for (int i = 0; i < BOARD_SIZE; i++)
			//	{
			//		for (int j = 0; j < BOARD_SIZE; j++)
			//		{
			//			crd.i_coord = i; crd.j_coord = j;
			//			char tool = GetContentOfCoord(pBoard, crd);
			//			if (tool == WHITE_K)
			//				WhiteKing = crd;
			//			if (tool == BLACK_K)
			//				BlackKing = crd;
			//		}
			//	}
			//}
			///* reset turn data */
			//selectedTool.i_coord = -1; selectedTool.j_coord = -1;
			//promoteSquare.i_coord = -1; promoteSquare.j_coord = -1;



			/* Handle Computer's Turn */
			/* if in AI mode, in game state and it is the computer's turn */
			int scr;
			if (properties[0] == GAME_MODE && properties[5] == PVC_MODE && properties[3] != properties[4])
			{
				//computerMove = NULL;
				//if (score(pBoard, getGenericTool(1)) == -100)//no legal move for the computer (computer lost)
				//{
				//	//TODO: show that player won, disable all buttons except for returns.
				//	properties[1] = 1; // <---- consider this //TODO:
				//}

				////run MINIMAX
				//if (properties[3] == BLACK_PLAYER)//if computer's color is white
				//	minimax_score(pBoard, WHITE_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, 0);
				//else
				//	minimax_score(pBoard, BLACK_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, 0);

				//advanceTurnStage(0);
				//if (computerMove != NULL)
				//{
				//	printf("computer: %d,%d to %d,%d \n", computerMove->src.i_coord, computerMove->src.j_coord, computerMove->dst.i_coord, computerMove->dst.j_coord);
				//	selectedTool = computerMove->src;
				//	gui_makeMove(chessWindow->shownMenu, &(guiBoard[computerMove->dst.i_coord][computerMove->dst.j_coord]));
				//	//advanceTurnStage(0); //computerMove->promote);
				//	free(computerMove);
				//	updateGUIBoard(chessWindow->shownMenu);
				//	updateGUIBoard_Vis(chessWindow->shownMenu);
				//	if (SDL_Flip(chessWindow->self) != 0)
				//	{
				//		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				//		quit();
				//		return 0;
				//	}
				//}

				computerMove = NULL;
				if (properties[2] != 0)
					scr = bestScore(pBoard, properties[4]);
				else
					scr = score(pBoard, properties[4]);

				/* If both computer and player have no valid moves, it is a TIE */
				if (scr == -999999)
				{
					//print TIE
					labelTIE(chessWindow);
					endGamePlay(chessWindow);
				}
				/* If the computer has no valid moves (happens because of MATE), it has lost: */
				else if (scr == -1000000)
				{
					labelMATE(chessWindow);
					endGamePlay(chessWindow);
					//print CHECK MATE
				}
				/* If the computer has valid moves, but KingUnderThreat(board, -properties[4]), it is CHECK */
				else if (KingUnderThreat(pBoard, properties[4]))

				{
					// ---- So make it seen.
					labelCHECK(chessWindow);
				}

				/* Run minimax */
				if (properties[3] == BLACK_PLAYER)//if computer's color is white
					scr = minimax_score(pBoard, WHITE_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, 0);
				else
					scr = minimax_score(pBoard, BLACK_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, 0);
				/* Make the computer's move, (if there is one) */
				printf("minimax score was: %d\n", scr);
				printf("For this board, score for white is %d, and for black is %d. \n\n", Material(pBoard, WHITE_PLAYER), Material(pBoard, BLACK_PLAYER));
				advanceTurnStage(0);
				if (computerMove != NULL)
				{

					printf("computer: %d,%d to %d,%d \n", computerMove->src.i_coord, computerMove->src.j_coord, computerMove->dst.i_coord, computerMove->dst.j_coord);
					selectedTool = computerMove->src;

					makeMove(pBoard, computerMove);
					setPromoteSquare(computerMove->dst, computerMove->promote);
					advanceTurnStage(computerMove->promote);

					selectedTool.i_coord = -1; selectedTool.j_coord = -1;
					if (computerMove->promote)
					{
						advanceTurnStage(0);
					}

					freeMovesList(computerMove);
					updateGUIBoard(chessWindow->shownMenu);
					updateGUIBoard_Vis(chessWindow->shownMenu);
					if (SDL_Flip(chessWindow->self) != 0)
					{
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						quit();
						return 0;
					}
				}
				if (properties[2] != 0)
					scr = bestScore(pBoard, properties[4]);
				else
					scr = score(pBoard, properties[4]);
				/* If both computer and player have no valid moves, it is a TIE */
				if (scr == -999999)
				{
					//print TIE
					labelTIE(chessWindow);
					endGamePlay(chessWindow);
				}
				/* If the computer has no valid moves (happens because of MATE), it has lost: */
				else if (scr == -1000000)
				{
					labelMATE(chessWindow);
					endGamePlay(chessWindow);
					//print CHECK MATE
				}
				/* If the computer has valid moves, but KingUnderThreat(board, -properties[4]), it is CHECK */
				else if (KingUnderThreat(pBoard, properties[4]))

				{
					// ---- So make it seen.
					labelCHECK(chessWindow);
				}
			}
		}
	}
	printf("ended");
	return 1;
}