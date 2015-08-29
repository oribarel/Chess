#ifndef CONTROL_COMPONENTS_H
#define CONTROL_COMPONENTS_H

//TODO: declare all functions




#include "Chess.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

typedef struct rgb
{
	Uint8 r, g, b;
} RGB;

typedef struct window
{
	struct controlComponent *panelChild;
	struct SDL_Surface *self;
} Window;

typedef struct button
{
	int (*f) (Window *);
	struct SDL_Surface *pic;
	char gameControl;
} Button;

typedef struct panel
{
	struct rgb rgb_triplet;
	struct controlComponent *children;
} Panel;

typedef struct label
{
	struct SDL_Surface *pic;
} Label;

typedef struct controlComponent
{
	struct button *btn;
	struct panel *pnl;
	struct label *lbl;
	struct controlComponent *next;
	SDL_Rect rect;
} ControlComponent;

typedef struct pixel
{
	int x, y;
} Pixel;

typedef struct rectSize
{
	int h, w;
} RectSize;

int init_GUI(void);
int nullFunction(Window *w);
Window *createWindow();
ControlComponent *createButton(SDL_Rect inpRect, SDL_Surface *pic, int(*f)(Window *));/*, char ctrl);*/
ControlComponent *createLabel(SDL_Rect rect, SDL_Surface *pic);
ControlComponent *createPanel(SDL_Rect rect, RGB rgb_triplet);
int addPanelToWindow(Window *window, ControlComponent *ccp);
int addButtonToPanel(ControlComponent *ccp, ControlComponent *ccb, Window *s);
int addControlToPanelList(ControlComponent *panelComponent, ControlComponent *toAdd);
int addPanelToPanel(ControlComponent *ccpParent, ControlComponent *ccpChild, Window *window);
int addLabelToPanel(ControlComponent *ccpParent, ControlComponent *cclChild, Window *window);
ControlComponent *getLastInPanelList(ControlComponent* ccpParent);
Uint32 getColorOfPanel(Window *window, ControlComponent *ccp);
int freeCCTree(ControlComponent *cc);
SDL_Rect createSDL_Rect(int w, int h, int x, int y);
RGB createRGB(int r, int g, int b);
SDL_Surface *uploadPicture(const char *path);


#endif


