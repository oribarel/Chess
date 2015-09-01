#include "Chess_Controls.h"

const char *str_purpose_BoardSquare = "BoardSquare";

const char *BDD = "bdd.bmp";
const char *BDL = "bdl.bmp";
const char *BLD = "bld.bmp";
const char *BLL = "bll.bmp";
const char *RDD = "rdd.bmp";
const char *RDL = "rdl.bmp";
const char *RLD = "rld.bmp";
const char *RLL = "rll.bmp";
const char *NDD = "ndd.bmp";
const char *NDL = "ndl.bmp";
const char *NLD = "nld.bmp";
const char *NLL = "nll.bmp";
const char *PDD = "pdd.bmp";
const char *PDL = "pdl.bmp";
const char *PLD = "pld.bmp";
const char *PLL = "pll.bmp";
const char *QDD = "qdd.bmp";
const char *QDL = "qdl.bmp";
const char *QLD = "qld.bmp";
const char *QLL = "qll.bmp";
const char *KDD = "kdd.bmp";
const char *KDL = "kdl.bmp";
const char *KLD = "kld.bmp";
const char *KLL = "kll.bmp";
const char *LSQ = "lsq.bmp";
const char *DSQ = "dsq.bmp";


SDL_Rect createSDL_RectForBoardSquare(Coord crd)
{
	SDL_Rect rect;
	rect.w = SQUARE_SIDE;
	rect.h = SQUARE_SIDE;
	rect.x = crd.i_coord * 80;
	rect.y = 560 - 80 * (crd.j_coord);
	return rect;
}


/* crd is a needed info for light or dark squares. player is a needed info for the color of the tool. type for the type. */
const char *getPictureName_tools(Coord crd, int player, eTool type)
{
	if (mod(crd.i_coord + crd.j_coord, 2) == 0) //Dark
	{
		if (player == WHITE_PLAYER)
		{
			switch (type)
			{
			case Pawn:
				return PLD;
			case Rook:
				return RLD;
			case Knight:
				return NLD;
			case Bishop:
				return BLD;
			case Queen:
				return QLD;
			case King:
				return KLD;
			default:
				return DSQ;
			}
		}
		else
		{
			switch (type)
			{
			case Pawn:
				return PDD;
			case Rook:	
				return RDD;
			case Knight:
				return NDD;
			case Bishop:
				return BDD;
			case Queen:	
				return QDD;
			case King:	
				return KDD;
			default:
				return DSQ;
			}
		}
	}
	else //Light
	{
		if (player == WHITE_PLAYER)
		{
			switch (type)
			{
			case Pawn:
				return PLL;
			case Rook:
				return RLL;
			case Knight:
				return NLL;
			case Bishop:
				return BLL;
			case Queen:
				return QLL;
			case King:
				return KLL;
			default:
				return LSQ;
			}
		}
		else
		{
			switch (type)
			{
			case Pawn:
				return PDL;
			case Rook:
				return RDL;
			case Knight:
				return NDL;
			case Bishop:
				return BDL;
			case Queen:
				return QDL;
			case King:
				return KDL;
			default:
				return LSQ;
			}
		}
	}
}

/*	Only for initial board setting.
	creates the desired board square, adds it to the ccp and returns a pointer to the ccb */
ControlComponent *createSquareByToolType(Window *window, ControlComponent *ccp, Coord crd, eTool type, int player, int (*f)(Window *, ControlComponent *))
{
	ControlComponent *ccb;
	SDL_Rect rect = createSDL_RectForBoardSquare(crd);
	const char *pictureName = getPictureName_tools(crd, player, type);
	ccb = createButton_square(rect, uploadPicture(pictureName), f, crd);
	addButtonToPanel(ccp, ccb, window);
	return ccb;
}

