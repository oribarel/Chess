
#include "Control_Components.h"

/****************************************************************************
* cc is short for ControlComponent.											*
* variables that are of type ControlComponent will be named according to:	*
* ccp<Name> if they function as a Panel										*
* ccb<Name> if they function as a Button									*
* ccl<Name> if they function as a Label										*
*****************************************************************************/

int init_GUI(void)
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
	SDL_Surface *surface = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
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

SDL_Rect createSDL_Rect(int w, int h, int x, int y)
{
	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;
	return rect;
}
RGB createRGB(int r, int g, int b)
{
	RGB rgb;
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	return rgb;
}

ControlComponent *createButton(SDL_Rect inpRect, SDL_Surface *pic, int (*f)(Window *, struct controlComponent *), char purpose) /*char ctrl )*/
{
	Coord crd = { -1, -1 };
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
	btn->crd = crd;
	btn->purpose = purpose;

	comp->next = NULL;
	comp->lbl = NULL;
	comp->pnl = NULL;
	comp->rect = inpRect;
	comp->btn = btn;

	return comp;
}

ControlComponent *createButton_square(SDL_Rect inpRect, SDL_Surface *pic, int(*f)(Window *, struct controlComponent *), Coord crd) /*char ctrl )*/
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
	btn->crd = crd;
	btn->purpose = 'a';

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

//returns 0 on failure and 1 on success.
int addPanelToWindow(Window *window, ControlComponent *ccp)
{
	window->panelChild = ccp;
	Uint32 color = getColorOfPanel(window, ccp);
	
	int DrawSuccess = SDL_FillRect(window->self, &(ccp->rect), color);
	if (DrawSuccess == -1)
	{
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		return 0;
	}
	return 1;
}

/*TODO: EDIT*/
int addButtonToPanel(ControlComponent *ccp, ControlComponent *ccb, Window *window)
{
	addControlToPanelList(ccp, ccb);

	ccb->rect.x += ccp->rect.x;
	ccb->rect.y += ccp->rect.y;
	if (SDL_BlitSurface(ccb->btn->pic, NULL, window->self, &(ccb->rect)) != 0)
	{
		SDL_FreeSurface(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

int addControlToPanelList(ControlComponent *panelComponent, ControlComponent *toAdd)
{
	ControlComponent *last = getLastInPanelList(panelComponent);
	if (last == NULL)
		panelComponent->pnl->children = toAdd;
	else
		last->next = toAdd;
	return 1;
}

//returns 0 on failure and 1 on success.
int addPanelToPanel(ControlComponent *ccpParent, ControlComponent *ccpChild, Window *window){

	addControlToPanelList(ccpParent, ccpChild);
	Uint32 color = getColorOfPanel(window, ccpChild);

	ccpChild->rect.x += ccpParent->rect.x;
	ccpChild->rect.y += ccpParent->rect.y;
	
	if (SDL_FillRect(window->self, &(ccpChild->rect), color) != 0)
	{
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

//returns 0 on failure and 1 on success.
int addLabelToPanel(ControlComponent *ccpParent, ControlComponent *cclChild, Window *window){
	
	addControlToPanelList(ccpParent, cclChild);

	cclChild->rect.x += ccpParent->rect.x;
	cclChild->rect.y += ccpParent->rect.y;
	if (SDL_BlitSurface(cclChild->lbl->pic, NULL, window->self, &(cclChild->rect)) != 0) 
	{
		SDL_FreeSurface(cclChild->lbl->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		quit();
		return 0;
	}
	return 1;
}

/*Returns Last CC in the children list of ccpParent.*/
ControlComponent *getLastInPanelList(ControlComponent* ccpParent)
{
	ControlComponent *ccChild;
	ccChild = ccpParent->pnl->children;
	if (ccChild == NULL)
		return NULL;
	while (ccChild->next != NULL)
		ccChild = ccChild->next;
	return ccChild;
}

Uint32 getColorOfPanel(Window *window, ControlComponent *ccp)
{
	Uint32 color = SDL_MapRGB(window->self->format, ccp->pnl->rgb_triplet.r, ccp->pnl->rgb_triplet.g, ccp->pnl->rgb_triplet.b);
	return color;
}

int nullFunction(Window *w, struct controlComponent *ccb)
{
	return 0;
}

int freeCCTree(ControlComponent *cc)
{
	/* Case 1: No Tree */
	if (cc == NULL)
		return 1;

	/* Case 2: */
	/* Recursively free next ccTree */
	if (cc->next != NULL)
		freeCCTree(cc->next);

	/* if cc is a ccp*/
	else if (cc->pnl != NULL)
	{
		freeCCTree(cc->pnl->children);
		free(cc->pnl);
		free(cc);
	}

	/* if cc is a ccb */
	else if (cc->btn != NULL)
	{
		SDL_FreeSurface(cc->btn->pic);
		free(cc->btn);
		free(cc);
	}

	/* if cc is a ccl */
	else if (cc->lbl != NULL)
	{
		SDL_FreeSurface(cc->lbl->pic);
		free(cc->lbl);
		free(cc);
	}
	return 1;
}

SDL_Surface *uploadPicture(const char *path)
{
	SDL_Surface *pic = SDL_LoadBMP(path);
	SDL_Surface *displayPic = SDL_DisplayFormat(pic);
	if (displayPic == NULL)
	{
		printf("ERROR: failed to convert image to new format: %s\n", SDL_GetError());
		quit();
	}
	SDL_FreeSurface(pic);
	return displayPic;
}