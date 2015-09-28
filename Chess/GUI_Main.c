#include "GUI_Main.h"

/*
 Brief explanation:

 ******************************************************************************************************************
 1. All of the gui components are statically allocated, that is because that as developers, our game is pre-defined
 in the sense that there aren't any gui components that 'surprise' us during run-time. 
 Hence, all the menus, panels, buttons and so on, are declared in the GUI_Main function below. 
 An exception to the former statement are the pictures, which all are loaded by dynamic allocation during run-time,
 and are the only GUI componenets to be freed on exit.
 Key componenets are externals (declared in the Chess_Windows header) and so can be used elsewhere in the program.

 ******************************************************************************************************************
 2. The GUI_Board is shared with all relevant menus (playerSelection, AI_settings, Game, Hint). the gui_board is
 comprised of 64 buttons that are the squares. Each of the squares is both a 2-dim array element of buttons,
 and also a child of it's parent panel. Throughout the game, each square button changes its function, as the
 flow of the game demands, using the update UpdateGUIBoard function (in Chess_Windows.c).

 ******************************************************************************************************************
 3. This file only contains the GUI_Main function that defines the flow of the game in Gui mode.
 Other files are:
 
 Chess_Windows: Contains the logic behind the menus of the game, its header declares important externs.
 
 Chess_Controls: Contains utility functions for elements of the Gui such as the board square rectangles, and
 the pictures for the squares.

 Control_Componenets: Contains functions for GUIs in general, i.e. none specific for a chess game. There we can find
 functions to create a panel, a button, or other componenets, add them to each other, upload pictures, etc.

 ******************************************************************************************************************
 4. The control componenets used in our program, and other structs:

 ControlComponenet: (refered as a CC in names and documentation for short): a struct with 3 pointer members, and a next.
 By definition, only 1 pointer (excluding the next) is not NULL at a given time. 
 The pointers are to a button, panel and label. Moreover, a CC has a rectangle.
 If a cc's button pointer is not NULL then it is a ccb.
 If a cc's pnl pointer is not null then it is a ccp.
 Same with labels and ccls.
 As said before, a cc cannot be of more than one type, although nothing enforces that.
 Notice: all CCs in the program are named ccX_<name>, when X is either p,b or l. A ccp contains a panel,
 a ccb contains a button, and a ccl contains a label. This way a CC's purpose (or type) is exacly clear
 when looking at it.

 Menu: a menu contains 4 ccp pointers, as out game only has 4 panels at most in each of its menus. Fixed menu pointers
 make work easy when each menu is pre-defined, like in our program. Moreover, it has a ccl for the menu header,
 an rgb(struct) for it's color, a rectangle, and its identifier. There is only a fixed amount of menus in the program:
 the main, the playerSelection, the AI_settings, the save, the load, the hint and the game Menus, and they are all externs.

 Window: the program uses only one window, it has a menu pointer, so every time a different menu may be shown upon the
 window. it has an SDL_Surface.

 RGB: contains a Uint8 triplet for sdl colour purposes.

 Button: this struct has a btnFunc (typedef) member, a function pointer. This function will be operated when the button
 is pressed. it has a picture, as an SDL_Surface.
 A Coord (struct, see Chess_Logic explanation) that is used for when poressing board squares.
 also a char member called purpose, for easier debugging.

 Label: only contains a picture as an SDL_Surface.

 Panel: contains an rgb for the panel's color, and a CC pointer for the panel's list of Childern (here comes to use
 the 'Next' member in the CC struct).

 the typedef btnFunc: is a format for button funtions, they all get as arguments the menu pointer to which they
 operate, and in general the pointer to the ccb that contains the pressed button.
 ******************************************************************************************************************

 5. Lastly, the project is intended to be well documented. When documentation seems to be missing take note that the
 method and variable names are long and cumbersome exactly for the reason that they are very informative, 
 so that no confusion should occur with deciphering the purpose of the method or the flow inside it.
 ******************************************************************************************************************
*/


board_t pBoard;
Window window, *chessWindow = &window;

Menu menu_Main, *pMenu_Main = &menu_Main;
Menu menu_PlayerSelection, *pMenu_PlayerSelection = &menu_PlayerSelection;
Menu menu_AI_settings, *pMenu_AI_settings = &menu_AI_settings;
Menu menu_Game, *pMenu_Game = &menu_Game;
Menu menu_Save, *pMenu_Save = &menu_Save;
Menu menu_Load, *pMenu_Load = &menu_Load;
Menu menu_Hint, *pMenu_Hint = &menu_Hint;

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
	
	ControlComponent ccl_MainMenuLabels[2];
	Label lbl_MainMenuLabels[2];




	/* Player Selection Menu CCPs, PNLs,CCBs, BTNs*/
	ControlComponent ccp_PlayerSelectionMenuCCPs[NUM_OF_PLAYER_SELECTION_MENU_PANELS];
	Panel pnl_PlayerSelectionMenuPanels[NUM_OF_PLAYER_SELECTION_MENU_PANELS];

	ControlComponent ccb_PlayerSelectionMenuCCBs[NUM_OF_PLAYER_SELECTION_MENU_BUTTONS];
	Button btn_PlayerSelectionMenuButtons[NUM_OF_PLAYER_SELECTION_MENU_BUTTONS];

	ControlComponent ccl_PlayerSelectionMenuHeader;
	Label lbl_PlayerSelectionMenuHeader;




	/* AI_Settings Menu CCPs, PNLs, BTNs */
	ControlComponent ccp_AI_SettingsMenuCCPs[NUM_OF_AI_MENU_PANELS];
	Panel pnl_AI_SettingsMenuPanels[NUM_OF_AI_MENU_PANELS];

	ControlComponent ccb_AI_SettingsCCBs[NUM_OF_AI_MENU_BUTTONS];
	Button btn_AI_SettingsButtons[NUM_OF_AI_MENU_BUTTONS];

	ControlComponent ccl_AI_settingsHeader;
	Label lbl_AI_settingsHeader;




	/* Game Play Menu CCPs, PNLs, BTNs */
	ControlComponent ccp_GamePlayMenuCCPs[NUM_OF_GAME_PLAY_MENU_PANELS];
	Panel pnl_GamePlayMenuPanels[NUM_OF_GAME_PLAY_MENU_PANELS];

	ControlComponent ccb_GamePlayMenuCCBs[NUM_OF_GAME_PLAY_MENU_BUTTONS];
	Button btnb_GamePlayMenuButtons[NUM_OF_GAME_PLAY_MENU_BUTTONS];

	ControlComponent ccl_gamePlayCCLs[3];
	Label lbl_gamePlayLabels[3];


	/* Board Panel CCPs, PNLs, BTNs*/
	ControlComponent ccp_Board, *pCCP_Board = &ccp_Board;
	Panel pnl_Board, *pPnl_Board = &pnl_Board;

	int wSide = 12;
	int hSide = 9;
	int wBoardSetting = 800;
	int hBoardSetting = 750;
	int wGameModeSetting = SCREEN_W - wBoardSetting - 2 * wSide;

	SDL_Rect boardSettingPanel = createSDL_Rect(wBoardSetting, hBoardSetting, wSide + wGameModeSetting, hSide);
	panelMaker(pCCP_Board, pPnl_Board, boardSettingPanel, rgbMenuBlue);

	Button btn_BoardPanelButtons[BOARD_SIZE*BOARD_SIZE]; // these are 64 buttons for the squares.

	ControlComponent ccl_BoardPanelCCLs[1];
	Label lbl_BoardPanelLabels[1];


	/* Save Menu  CCPs, PNLs, BTNs */
	ControlComponent ccp_SaveMenuCCPs[NUM_OF_SAVE_MENU_PANELS];
	Panel pnl_SaveMenuPanels[NUM_OF_SAVE_MENU_PANELS];

	ControlComponent ccb_SaveMenuCCBs[NUM_OF_SAVE_MENU_BUTTONS + NUMBER_OF_SLOTS_FOR_LOAD_AND_SAVE];
	Button btn_SaveMenuButtons[NUM_OF_SAVE_MENU_BUTTONS + NUMBER_OF_SLOTS_FOR_LOAD_AND_SAVE];

	ControlComponent ccl_saveMenuCCLs[2];
	Label lbl_saveMenuLabels[2];


	/* Load Menu  CCPs, PNLs, BTNs */
	ControlComponent ccp_LoadMenuCCPs[NUM_OF_LOAD_MENU_PANELS];
	Panel pnl_LoadMenuPanels[NUM_OF_LOAD_MENU_PANELS];

	ControlComponent ccb_LoadMenuCCBs[NUM_OF_LOAD_MENU_BUTTONS + NUMBER_OF_SLOTS_FOR_LOAD_AND_SAVE];
	Button btn_LoadMenuButtons[NUM_OF_LOAD_MENU_BUTTONS + NUMBER_OF_SLOTS_FOR_LOAD_AND_SAVE];

	ControlComponent ccl_loadMenuCCLs[2];
	Label lbl_loadMenuLabels[2];

	/* Hint Menu  CCPs, PNLs, BTNs */
	ControlComponent ccp_HintMenuCCPs[4];
	Panel pnl_HintMenuPanels[4];

	ControlComponent ccb_HintMenuCCBs[3];
	Button btn_HintMenuButtons[3];

	ControlComponent ccl_HintMenuCCLs[1];
	Label lbl_HintMenuLabels[1];

	/* Pass the board */
	pBoard = passedBoard;
	init_board(pBoard);
	passTheBoard(passedBoard);



	/* cMoves */
	cMove *computerMove = NULL;


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
	createMainMenu(pMenu_Main, pCCP_MainMenu, pPnl_MainMenu, ccb_mainMenuCCBs, btn_mainMenuButtons, ccl_MainMenuLabels, lbl_MainMenuLabels);
	createPlayerSelectionMenu(pMenu_PlayerSelection, ccp_PlayerSelectionMenuCCPs, pnl_PlayerSelectionMenuPanels, ccb_PlayerSelectionMenuCCBs, btn_PlayerSelectionMenuButtons, &ccl_PlayerSelectionMenuHeader, &lbl_PlayerSelectionMenuHeader);
	createAI_SettingsMenu(pMenu_AI_settings, ccp_AI_SettingsMenuCCPs, pnl_AI_SettingsMenuPanels, ccb_AI_SettingsCCBs, btn_AI_SettingsButtons, &ccl_AI_settingsHeader, &lbl_AI_settingsHeader);
	createGameMenu(pMenu_Game, ccp_GamePlayMenuCCPs, pnl_GamePlayMenuPanels, ccb_GamePlayMenuCCBs, btnb_GamePlayMenuButtons, ccl_gamePlayCCLs, lbl_gamePlayLabels);
	createSaveMenu(pMenu_Save, ccp_SaveMenuCCPs, pnl_SaveMenuPanels, ccb_SaveMenuCCBs, btn_SaveMenuButtons, ccl_saveMenuCCLs, lbl_saveMenuLabels);
	createLoadMenu(pMenu_Load, ccp_LoadMenuCCPs, pnl_LoadMenuPanels, ccb_LoadMenuCCBs, btn_LoadMenuButtons, ccl_loadMenuCCLs, lbl_loadMenuLabels);
	createHintMenu(pMenu_Hint, ccp_HintMenuCCPs, pnl_HintMenuPanels, ccb_HintMenuCCBs, btn_HintMenuButtons, ccl_HintMenuCCLs, lbl_HintMenuLabels);


	createGUIBoard(guiBoard, pCCP_Board, btn_BoardPanelButtons, pBoard,ccl_BoardPanelCCLs,lbl_BoardPanelLabels);
	addPanelToMenu(pMenu_PlayerSelection, pCCP_Board, 4);
	addPanelToMenu(pMenu_AI_settings, pCCP_Board, 4);
	addPanelToMenu(pMenu_Game, pCCP_Board, 4);
	addPanelToMenu(pMenu_Hint, pCCP_Board, 4);


	/* Start */
	showMenu(chessWindow, pMenu_Main);
	if (SDL_Flip(chessWindow->self) != 0)
	{
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		quit();
		return 0;
	}

	int lastCurrentPlayer = properties[4];
	int scr, currKingUnderThreat;
	while (!properties[1])
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) == 1 && properties[1] == 0)
		{
			SDL_Delay(40);
			if (e.type == SDL_QUIT)
			{
				/*freeCCTree(window->panelChild);
				window->panelChild = NULL;
				SDL_FreeSurface1(window->self);
				free(window);
				intQuit = 1;
				break;*/
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				/* Left Click*/
				if (e.button.button == SDL_BUTTON_LEFT)
					buttonPressHandler(chessWindow, e);
				/* Right Click https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlmousebuttonevent.html */
				else if (e.button.button == SDL_BUTTON_RIGHT)
					rightClicksHandler(chessWindow, e);
			}

			if (SDL_Flip(chessWindow->self) != 0)
			{
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				quit();
				return 0;
			}

			if ((properties[4] == WHITE_PLAYER ? whitePlayerTurnStage : blackPlayerTurnStage) == PROMOTE) //if current player promotes now
				updateInfoLabels(0, 0, PROMOTE);


			/* New Experiement*/
			if (properties[0] == GAME_MODE && chessWindow->shownMenu->identifier == GAME_PLAY_MENU)
			{
				scr = score(pBoard, properties[4]);
				currKingUnderThreat = KingUnderThreat(pBoard, properties[4]);

				updateInfoLabels(scr, currKingUnderThreat, properties[4] == WHITE_PLAYER ? whitePlayerTurnStage : blackPlayerTurnStage);
			}
			/* End new Experiement */


			/* PVP only when turn changes */
			if (properties[0] == GAME_MODE && properties[5] == PVP_MODE && lastCurrentPlayer != properties[4])
			{
				lastCurrentPlayer = properties[4];

				/*scr = score(pBoard, properties[4]);
				currKingUnderThreat = KingUnderThreat(pBoard, properties[4]);

				updateInfoLabels(scr, currKingUnderThreat, properties[4] == WHITE_PLAYER ? whitePlayerTurnStage : blackPlayerTurnStage);*/

				if (scr == MATE_WIN_LOSE || scr == TIE_SCORE)
				{
					endGamePlay(chessWindow);
				}
				else if (KingUnderThreat(pBoard, properties[4]))
				{
				}
			}



			/* PVC computer turn */
			if (properties[0] == GAME_MODE && properties[5] == PVC_MODE && properties[3] != properties[4])/* if in AI mode, in game state and it is the computer's turn */
			{
				computerMove = NULL;

				if (scr == MATE_WIN_LOSE || scr == TIE_SCORE)
				{
					endGamePlay(chessWindow);
				}
				else if (KingUnderThreat(pBoard, properties[4]))
				{
				}

				if (properties[0] == GAME_MODE)
				{

					/* Run minimax */

					GameLabel(chessWindow, GAME_THINKING_H);
					if (SDL_Flip(chessWindow->self) != 0)
					{
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						quit();
						return 0;
					}


					int oldDepth = properties[2];
					int best = (oldDepth == 0);

					if (properties[3] == BLACK_PLAYER)//if computer's color is white
					{
						properties[2] = getDepth(pBoard, WHITE_PLAYER);
						scr = minimax_score(pBoard, WHITE_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, best);
					}
					else
					{
						properties[2] = getDepth(pBoard, BLACK_PLAYER);
						scr = minimax_score(pBoard, BLACK_PLAYER, properties[2], 1, &computerMove, MIN_VALUE, MAX_VALUE, best);
					}
					properties[2] = oldDepth;
					GameLabel(chessWindow, GAME_H);

					if (SDL_Flip(chessWindow->self) != 0)
					{
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						quit();
						return 0;
					}


					/* Make the computer's move, (if there is one) */
					//printf("minimax score was: %d\n", scr);
					//printf("For this board, score for white is %d, and for black is %d. \n\n", Material(pBoard, WHITE_PLAYER), Material(pBoard, BLACK_PLAYER));
					advanceTurnStage(0);
					if (computerMove != NULL)
					{

						printf("computer: %c,%d to %c,%d \n", computerMove->src.i_coord + 'a', computerMove->src.j_coord+1, computerMove->dst.i_coord + 'a', computerMove->dst.j_coord+1);
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

						scr = score(pBoard, properties[4]);

						if (scr == MATE_WIN_LOSE || scr == TIE_SCORE)
						{
							endGamePlay(chessWindow);
						}
						
						else if (KingUnderThreat(pBoard, properties[4]))
						{
						}
						
						currKingUnderThreat = KingUnderThreat(pBoard, properties[4]);

						updateInfoLabels(scr, currKingUnderThreat, properties[4] == WHITE_PLAYER ? whitePlayerTurnStage : blackPlayerTurnStage);

						if (SDL_Flip(chessWindow->self) != 0)
						{
							printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
							quit();
							return 0;
						}
					}
				}
			}
			if (SDL_Flip(chessWindow->self) != 0)
			{
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				quit();
				return 0;
			}
		}
	}

	/*
	Here we free all of our resources (only pictures): 

	Main Menu: 2 labels, 3 buttons.
	Player Selection Menu: 4 buttons, 1 label
	AI settings Menu: 5 buttons, 1 label
	Game Menu: 3 buttons, 3 Labels
	Board: 64 buttons
	Save Menu: 10 buttons, (1 label)
	Load Menu: 9 buttons, (1 label)
	Hint Menu: 3 buttons, (1 label)
	*/
	freeMenu(pMenu_Main);
	freeMenu(pMenu_PlayerSelection);
	freeMenu(pMenu_AI_settings);
	freeMenu(pMenu_Save);
	freeMenu(pMenu_Load);
	freeMenu(pMenu_Game);
	freeMenu(pMenu_Hint);
	freePanel(pCCP_Board);
	SDL_Quit();
	//printf("%d pictures not freed.\n", picAllocs);
	//printf("ended\n");
	return 1;
}