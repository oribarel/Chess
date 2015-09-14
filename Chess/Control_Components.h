#ifndef CONTROL_COMPONENTS_H
#define CONTROL_COMPONENTS_H

//TODO: declare all functions


#include <SDL.h>
#include <SDL_video.h>
#include "Chess_Logic.h"

/* Definitions */
#define SCREEN_W 1024
#define SCREEN_H 768

/* externs*/

extern int picAllocs;

/* Structs */

struct menu;
struct controlComponent;
typedef int(*btnFunc)(struct menu *, struct controlComponent *);

typedef struct rgb
{
	Uint8 r, g, b;
} RGB;

typedef struct label
{
	struct SDL_Surface *pic;
} Label;


typedef struct button
{
	btnFunc f;
	struct SDL_Surface *pic;
	Coord crd;
	char purpose;
} Button;

typedef struct panel
{
	struct rgb rgb_triplet;
	struct controlComponent *children;
} Panel;


typedef struct controlComponent
{
	struct button *btn;
	struct panel *pnl;
	struct label *lbl;
	struct controlComponent *next;
	SDL_Rect rect;
} ControlComponent;


typedef struct menu
{
	struct controlComponent *panel_1;
	struct controlComponent *panel_2;
	struct controlComponent *panel_3;
	struct controlComponent *panel_4;
	
	struct controlComponent *header;
	
	struct rgb rgb_triplet;
	
	SDL_Rect rect;
	int identifier;
} Menu;

typedef struct window
{
	struct menu *shownMenu;
	struct SDL_Surface *self;
} Window;


typedef struct pixel
{
	int x, y;
} Pixel;

typedef struct rectSize
{
	int h, w;
} RectSize;



/* Extern Variables */
extern Window *chessWindow;


/* Functions Declarations*/

int init_GUI(void);
int nullFunction(struct menu *pMenu, struct controlComponent *ccb);

int createWindow(Window *window);
int createMenu(Menu *pMenu, SDL_Rect rect, RGB color, int identifier);
int createButton(ControlComponent *ccbParent, Button *btn, SDL_Rect rect, SDL_Surface *pic, btnFunc f , char purpose);
int createButton_square(ControlComponent *ccbParent, Button *button, SDL_Rect rect, SDL_Surface *pic, btnFunc f);

RGB createRGB(int r, int g, int b);
SDL_Rect createSDL_Rect(int w, int h, int x, int y);

int addPanelToWindow(Window *window, ControlComponent *ccp);
int addButtonToPanel(ControlComponent *ccp, ControlComponent *ccb);
int addControlToPanelList(ControlComponent *panelComponent, ControlComponent *toAdd);
int addPanelToPanel(ControlComponent *ccpParent, ControlComponent *ccpChild, Window *window);
int addLabelToPanel(ControlComponent *ccpParent, ControlComponent *cclChild);
int addPanelToMenu(Menu *menuParent, ControlComponent *ccp, int panelNum);
int addMenuToWindow(Window *window, Menu *pMenu);

int drawPanelToMenu(ControlComponent *ccp);
int drawButtonsOfPanel(ControlComponent *ccp);
int drawLabelsOfPanel(ControlComponent *ccl);

ControlComponent *getLastInPanelList(ControlComponent* ccpParent);
Uint32 getColorOfPanel(Window *window, ControlComponent *ccp);
Uint32 getColorOfMenu(Window *window, Menu *pMenu);

int freeCCTree(ControlComponent *cc);

SDL_Surface *uploadPicture(const char *path);

void SDL_FreeSurface1(SDL_Surface *surface);
int terminateProgram(void);
#endif


