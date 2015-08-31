#ifndef CHESS_CONTROLS
#define CHESS_CONTROLS

#include "Control_Components.h"



#define SQUARE_SIDE	80

ControlComponent *createSquareByToolType(Window *window, ControlComponent *ccp, Coord crd, eTool type, int player, int(*f)(Window *, ControlComponent *));
const char *getPictureName_tools(Coord crd, int player, eTool type);



//SDL_Surface* uploadPicture(char *path);
//void createSquareByToolType(Window *window, ControlComponent *ccp, Coord crd, eTool type, int player);
//const char *getPictureName_tools(Coord crd, int player, eTool type);

#endif
