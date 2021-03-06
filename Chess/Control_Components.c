
#include "Control_Components.h"

/****************************************************************************
* cc is short for ControlComponent.											*
* variables that are of type ControlComponent will be named according to:	*
* ccp<Name> if they function as a Panel										*
* ccb<Name> if they function as a Button									*
* ccl<Name> if they function as a Label										*
*****************************************************************************/

int picAllocs = 0;


int init_GUI(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	return 0;
}

int createWindow(Window *pWindow)
{
	/* 1 on successful run, 0 if SDL failed */
	SDL_WM_SetCaption("Chess", "Chess");
	SDL_Surface *surface = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF); // for fullscreen add: SDL_FULLSCREEN);
	if (surface == NULL)
	{
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}

	pWindow->self = surface;
	pWindow->shownMenu = NULL;
	return 1;
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

int createButton(ControlComponent *ccbParent, Button *btn, SDL_Rect rect, SDL_Surface *pic, btnFunc f, char purpose)
{
	Coord crd = { -1, -1 };

	btn->pic = pic;
	btn->f = f;
	btn->crd = crd;
	btn->purpose = purpose;

	ccbParent->next = NULL;
	ccbParent->lbl = NULL;
	ccbParent->pnl = NULL;
	ccbParent->rect = rect;
	ccbParent->btn = btn;

	return 1;
}


/* As of now, this function allocates the btn itself. */
int createButton_square(ControlComponent *ccbParent, Button *button, SDL_Rect rect, SDL_Surface *pic, btnFunc f)//int(*f)(Menu *, struct controlComponent *))
{
	Coord crd = { -1, -1 };

	button->pic = pic;
	button->f = f;
	button->crd = crd;
	button->purpose = 'a';

	ccbParent->next = NULL;
	ccbParent->lbl = NULL;
	ccbParent->pnl = NULL;
	ccbParent->rect = rect;
	ccbParent->btn = button;

	return 1;
}


int createMenu(Menu *pMenu, SDL_Rect rect, RGB color, int identifier)
{
	pMenu->rect = rect;
	pMenu->rgb_triplet = color;

	pMenu->panel_1 = NULL;
	pMenu->panel_2 = NULL;
	pMenu->panel_3 = NULL;
	pMenu->panel_4 = NULL;

	pMenu->identifier = identifier;
	return 1;
}


int addMenuToWindow(Window *window, Menu *pMenu)
{
	/* 1 on successful run, 0 if SDL failed */
	window->shownMenu = pMenu;
	Uint32 color = getColorOfMenu(window, pMenu);

	int DrawSuccess = SDL_FillRect(window->self, &(pMenu->rect), color);
	if (DrawSuccess == -1)
	{
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}
	return 1;
}

int addButtonToPanel(ControlComponent *ccp, ControlComponent *ccb)
{
	/*Doesn't Draw, only adds */
	addControlToPanelList(ccp, ccb);

	ccb->rect.x += ccp->rect.x;
	ccb->rect.y += ccp->rect.y;

	return 1;
}


int drawButtonToPanel(ControlComponent *ccb)
{
	/* Pre: has a pic */
	if (SDL_BlitSurface(ccb->btn->pic, NULL, chessWindow->self, &(ccb->rect)) != 0)
	{
		//SDL_FreeSurface1(ccb->btn->pic);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		terminateProgram();
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


int addPanelToPanel(ControlComponent *ccpParent, ControlComponent *ccpChild, Window *window)
{
	/* 1 on successful run, 0 if SDL failed */
	addControlToPanelList(ccpParent, ccpChild);
	Uint32 color = getColorOfPanel(window, ccpChild);

	ccpChild->rect.x += ccpParent->rect.x;
	ccpChild->rect.y += ccpParent->rect.y;

	if (SDL_FillRect(window->self, &(ccpChild->rect), color) != 0)
	{
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}
	return 1;
}

int addPanelToMenu(Menu *menuParent, ControlComponent *ccp, int panelNum)
{
	//Doesn't Draw!. returns 0 on failure and 1 on success.
	switch (panelNum)
	{
	case 1:
		menuParent->panel_1 = ccp;
		break;
	case 2:
		menuParent->panel_2 = ccp;
		break;
	case 3:
		menuParent->panel_3 = ccp;
		break;
	case 4:
		menuParent->panel_4 = ccp;
		break;
	default:
		break;
	}
	if (ccp != NULL)
	{
		ccp->rect.x += menuParent->rect.x;
		ccp->rect.y += menuParent->rect.y;
	}

	return 1;
}

/* Only fills the rect of ccp with its color*/
int drawPanelToMenu(ControlComponent *ccp)
{
	Uint32 color = getColorOfPanel(chessWindow, ccp);
	if (SDL_FillRect(chessWindow->self, &(ccp->rect), color) != 0)
	{
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		terminateProgram();
		return 0;
	}
	return 1;
}

int drawButtonsOfPanel(ControlComponent *ccp)
{
	/* 1 on successful run, 0 if SDL failed */
	ControlComponent *ccb = ccp->pnl->children;
	while (ccb != NULL)
	{
		if (ccb->btn != NULL)
		{
			if (drawButtonToPanel(ccb) == 0)
				return 0;
		}
		ccb = ccb->next;
	}
	return 1;
}

int drawLabelsOfPanel(ControlComponent *ccl)
{
	while (ccl != NULL)
	{
		if (ccl->lbl != NULL)
		{
			if (SDL_BlitSurface(ccl->lbl->pic, NULL, chessWindow->self, &(ccl->rect)) != 0)
			{
				//SDL_FreeSurface1(ccl->lbl->pic);
				printf("ERROR: failed to blit image: %s\n", SDL_GetError());
				terminateProgram();
				return 0;
			}
		}
		ccl = ccl->next;
	}
	return 1;
}


int addLabelToPanel(ControlComponent *ccpParent, ControlComponent *cclChild)
{

	addControlToPanelList(ccpParent, cclChild);

	cclChild->rect.x += ccpParent->rect.x;
	cclChild->rect.y += ccpParent->rect.y;

	return 1;
}

ControlComponent *getLastInPanelList(ControlComponent* ccpParent)
{
	/*Returns Last CC in the children list of ccpParent.*/
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

Uint32 getColorOfMenu(Window *window, Menu *pMenu)
{
	Uint32 color = SDL_MapRGB(window->self->format, pMenu->rgb_triplet.r, pMenu->rgb_triplet.g, pMenu->rgb_triplet.b);
	return color;
}

int nullFunction(struct menu *pMenu, struct controlComponent *ccb)
{
	return 0;
}


SDL_Surface *uploadPicture(const char *path)
{
	SDL_Surface *pic = SDL_LoadBMP(path);
	if (pic == NULL)
	{
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		terminateProgram();
		return NULL;
	}
	SDL_Surface *displayPic = SDL_DisplayFormat(pic);
	if (displayPic == NULL)
	{
		printf("ERROR: failed to convert image to new format: %s\n", SDL_GetError());
		terminateProgram();
		return NULL;
	}
	picAllocs++;
	SDL_FreeSurface(pic);
	return displayPic;
}

void SDL_FreeSurface1(SDL_Surface *surface)
{
	picAllocs--;
	SDL_FreeSurface(surface);
	return;
}

int terminateProgram(void)
{
	properties[1] = 1;
	return 1;
}