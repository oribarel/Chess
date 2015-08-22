#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_video.h>

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


int Init_GUI(void);





#endif

