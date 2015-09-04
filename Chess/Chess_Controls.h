#ifndef CHESS_CONTROLS
#define CHESS_CONTROLS

#include "Control_Components.h"



#define SQUARE_SIDE	80

#define NON_SEL 0
#define SEL	1
#define ATT 2

ControlComponent *createSquareByToolType(ControlComponent *ccp, ControlComponent *ccb, Button *button, Coord crd, eTool type, int player, btnFunc f);
const char *getPictureName_tools(Coord crd, int player, eTool type, int selectionStatus);
SDL_Rect createSDL_RectForBoardSquare(Coord crd);


//SDL_Surface* uploadPicture(char *path);
//void createSquareByToolType(Window *window, ControlComponent *ccp, Coord crd, eTool type, int player);
//const char *getPictureName_tools(Coord crd, int player, eTool type);

#endif
