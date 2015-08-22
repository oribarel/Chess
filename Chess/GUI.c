
#include "GUI.h"


int Init_GUI(void)
{
	Window *s;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *screen;
	return 0;
}

Window *createWindow()
{
	Window *openWindow;
	SDL_WM_SetCaption("Chess", "Chess");
	SDL_Surface *surface = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (surface == NULL) 
	{
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		quit();
	}
	openWindow = (Window *)calloc(1, sizeof(Window));
	if ( openWindow == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}
	openWindow->self = surface;
	openWindow->panelChild = NULL;
	return openWindow;
}



ControlComponent *createButton(SDL_Rect inpRect, SDL_Surface *pic, int (*f)(Window *), char ctrl )
{
	
	ControlComponent *comp = (ControlComponent *)calloc(1, sizeof(ControlComponent));
	if (comp == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}
	Button *btn = (Button *)calloc(1, sizeof(Button));
	if (btn == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}

	btn->pic = pic;
	btn->f = f;
	
	comp->next = NULL;
	comp->lbl = NULL;
	comp->pnl = NULL;
	comp->rect = inpRect;
	comp->btn = btn;

	return comp;
}

ControlComponent *createLabel(SDL_Rect rect, SDL_Surface *pic)
{
	ControlComponent *comp = (ControlComponent *)calloc(1, sizeof(ControlComponent));
	if (comp == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}
	Label *lbl = (Label *)calloc(1, sizeof(Label));
	if (lbl == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}

	lbl->pic = pic;
	
	comp->next = NULL;
	comp->lbl = lbl;
	comp->pnl = NULL;
	comp->rect = rect;
	comp->btn = NULL;

	return comp;
}

ControlComponent *createPanel(SDL_Rect rect, RGB rgb_triplet)
{
	ControlComponent *comp = (ControlComponent *)calloc(1, sizeof(ControlComponent));
	if (comp == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}
	Panel *pnl = (Panel *)calloc(1, sizeof(Panel));
	if (pnl == NULL)
	{
		printf("ERROR: standard function calloc has failed\n");
		quit();
	}
	
	pnl->rgb_triplet = rgb_triplet;
	pnl->children = NULL;

	comp->next = NULL;
	comp->lbl = NULL;
	comp->pnl = pnl;
	comp->rect = rect;
	comp->btn = NULL;

	return comp;
}

//TODO: not finished
void addPanelToWindow(Window *window, ControlComponent *panelComp)
{
	window->panelChild = panelComp;

	Uint32 color = SDL_MapRGB(window->self->format, panelComp->pnl->rgb_triplet.r, panelComp->pnl->rgb_triplet.g, panelComp->pnl->rgb_triplet.b);
	int DrawSuccess = SDL_FillRect(window->self, &(panelComp->rect), color);
}