#include "Chess_Controls.h"

const char *str_purpose_BoardSquare = "BoardSquare";

const char *BDD = "bdd80.bmp";
const char *BDL = "bdl80.bmp";
const char *BLD = "bld80.bmp";
const char *BLL = "bll80.bmp";
const char *RDD = "rdd80.bmp";
const char *RDL = "rdl80.bmp";
const char *RLD = "rld80.bmp";
const char *RLL = "rll80.bmp";
const char *NDD = "ndd80.bmp";
const char *NDL = "ndl80.bmp";
const char *NLD = "nld80.bmp";
const char *NLL = "nll80.bmp";
const char *PDD = "pdd80.bmp";
const char *PDL = "pdl80.bmp";
const char *PLD = "pld80.bmp";
const char *PLL = "pll80.bmp";
const char *QDD = "qdd80.bmp";
const char *QDL = "qdl80.bmp";
const char *QLD = "qld80.bmp";
const char *QLL = "qll80.bmp";
const char *KDD = "kdd80.bmp";
const char *KDL = "kdl80.bmp";
const char *KLD = "kld80.bmp";
const char *KLL = "kll80.bmp";
const char *LSQ = "lsq80.bmp";
const char *DSQ = "dsq80.bmp";

const char *BDD_Att = "bdd80att.bmp";
const char *BDL_Att = "bdl80att.bmp";
const char *BLD_Att = "bld80att.bmp";
const char *BLL_Att = "bll80att.bmp";
const char *RDD_Att = "rdd80att.bmp";
const char *RDL_Att = "rdl80att.bmp";
const char *RLD_Att = "rld80att.bmp";
const char *RLL_Att = "rll80att.bmp";
const char *NDD_Att = "ndd80att.bmp";
const char *NDL_Att = "ndl80att.bmp";
const char *NLD_Att = "nld80att.bmp";
const char *NLL_Att = "nll80att.bmp";
const char *PDD_Att = "pdd80att.bmp";
const char *PDL_Att = "pdl80att.bmp";
const char *PLD_Att = "pld80att.bmp";
const char *PLL_Att = "pll80att.bmp";
const char *QDD_Att = "qdd80att.bmp";
const char *QDL_Att = "qdl80att.bmp";
const char *QLD_Att = "qld80att.bmp";
const char *QLL_Att = "qll80att.bmp";
const char *KDD_Att = "kdd80att.bmp";
const char *KDL_Att = "kdl80att.bmp";
const char *KLD_Att = "kld80att.bmp";
const char *KLL_Att = "kll80att.bmp";
const char *LSQ_Att = "lsq80att.bmp";
const char *DSQ_Att = "dsq80att.bmp";

const char *BDD_Sel = "bdd80sel.bmp";
const char *BDL_Sel = "bdl80sel.bmp";
const char *BLD_Sel = "bld80sel.bmp";
const char *BLL_Sel = "bll80sel.bmp";
const char *RDD_Sel = "rdd80sel.bmp";
const char *RDL_Sel = "rdl80sel.bmp";
const char *RLD_Sel = "rld80sel.bmp";
const char *RLL_Sel = "rll80sel.bmp";
const char *NDD_Sel = "ndd80sel.bmp";
const char *NDL_Sel = "ndl80sel.bmp";
const char *NLD_Sel = "nld80sel.bmp";
const char *NLL_Sel = "nll80sel.bmp";
const char *PDD_Sel = "pdd80sel.bmp";
const char *PDL_Sel = "pdl80sel.bmp";
const char *PLD_Sel = "pld80sel.bmp";
const char *PLL_Sel = "pll80sel.bmp";
const char *QDD_Sel = "qdd80sel.bmp";
const char *QDL_Sel = "qdl80sel.bmp";
const char *QLD_Sel = "qld80sel.bmp";
const char *QLL_Sel = "qll80sel.bmp";
const char *KDD_Sel = "kdd80sel.bmp";
const char *KDL_Sel = "kdl80sel.bmp";
const char *KLD_Sel = "kld80sel.bmp";
const char *KLL_Sel = "kll80sel.bmp";



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
const char *getPictureName_tools(Coord crd, int player, eTool type, int selectionStatus)
{
	if (mod(crd.i_coord + crd.j_coord, 2) == 0) //Dark
	{
		if (player == WHITE_PLAYER)
		{
			switch (type)
			{
			
			case Pawn:
				if (selectionStatus == NON_SEL)
					return PLD;
				else if (selectionStatus == SEL)
					return PLD_Sel;
				else
					return PLD_Att;
			
			case Rook:
				if (selectionStatus == NON_SEL)
					return RLD;
				else if (selectionStatus == SEL)
					return RLD_Sel;
				else
					return RLD_Att;
			
			case Knight:
				if (selectionStatus == NON_SEL)
					return NLD;
				else if (selectionStatus == SEL)
					return NLD_Sel;
				else
					return NLD_Att;
			
			case Bishop:
				if (selectionStatus == NON_SEL)
					return BLD;
				else if (selectionStatus == SEL)
					return BLD_Sel;
				else
					return BLD_Att;
			
			case Queen:
				if (selectionStatus == NON_SEL)
					return QLD;
				else if (selectionStatus == SEL)
					return QLD_Sel;
				else
					return QLD_Att;
			
			case King:
				if (selectionStatus == NON_SEL)
					return KLD;
				else if (selectionStatus == SEL)
					return KLD_Sel;
				else
					return KLD_Att;
			
			default:
				if (selectionStatus == NON_SEL)
					return DSQ;
				else
					return DSQ_Att;
				
			}
		}
		else
		{
			switch (type)
			{
			case Pawn:
				if (selectionStatus == NON_SEL)
					return PDD;
				else if (selectionStatus == SEL)
					return PDD_Sel;
				else
					return PDD_Att;
			
			case Rook:	
				if (selectionStatus == NON_SEL)
					return RDD;
				else if (selectionStatus == SEL)
					return RDD_Sel;
				else
					return RDD_Att;
			
			case Knight:
				if (selectionStatus == NON_SEL)
					return NDD;
				else if (selectionStatus == SEL)
					return NDD_Sel;
				else
					return NDD_Att;
			
			case Bishop:
				if (selectionStatus == NON_SEL)
					return BDD;
				else if (selectionStatus == SEL)
					return BDD_Sel;
				else
					return BDD_Att;
			
			case Queen:	
				if (selectionStatus == NON_SEL)
					return QDD;
				else if (selectionStatus == SEL)
					return QDD_Sel;
				else
					return QDD_Att;
			
			case King:	
				if (selectionStatus == NON_SEL)
					return KDD;
				else if (selectionStatus == SEL)
					return KDD_Sel;
				else
					return KDD_Att;
			
			default:
				if (selectionStatus == NON_SEL)
					return DSQ;
				else
					return DSQ_Att;
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
				if (selectionStatus == NON_SEL)
					return PLL;
				else if (selectionStatus == SEL)
					return PLL_Sel;
				else
					return PLL_Att;
			
			case Rook:
				if (selectionStatus == NON_SEL)
					return RLL;
				else if (selectionStatus == SEL)
					return RLL_Sel;
				else
					return RLL_Att;
			
			case Knight:
				if (selectionStatus == NON_SEL)
					return NLL;
				else if (selectionStatus == SEL)
					return NLL_Sel;
				else
					return NLL_Att;
			
			case Bishop:
				if (selectionStatus == NON_SEL)
					return BLL;
				else if (selectionStatus == SEL)
					return BLL_Sel;
				else
					return BLL_Att;
			
			case Queen:
				if (selectionStatus == NON_SEL)
					return QLL;
				else if (selectionStatus == SEL)
					return QLL_Sel;
				else
					return QLL_Sel;
			
			case King:
				if (selectionStatus == NON_SEL)
					return KLL;
				else if (selectionStatus == SEL)
					return KLL_Sel;
				else
					return KLL_Att;
			
			default:
				if (selectionStatus == NON_SEL)
					return LSQ;
				else
					return LSQ_Att;
			}
		}
		else
		{
			switch (type)
			{
			
			case Pawn:
				if (selectionStatus == NON_SEL)
					return PDL;
				else if (selectionStatus == SEL)
					return PDL_Sel;
				else
					return PDL_Att;
			
			case Rook:
				if (selectionStatus == NON_SEL)
					return RDL;
				else if (selectionStatus == SEL)
					return RDL_Sel;
				else
					return RDL_Att;
			
			case Knight:
				if (selectionStatus == NON_SEL)
					return NDL;
				else if (selectionStatus == SEL)
					return NDL_Sel;
				else
					return NDL_Att;
			
			case Bishop:
				if (selectionStatus == NON_SEL)
					return BDL;
				else if (selectionStatus == SEL)
					return BDL_Sel;
				else
					return BDL_Att;
			
			case Queen:
				if (selectionStatus == NON_SEL)
					return QDL;
				else if (selectionStatus == SEL)
					return QDL_Sel;
				else
					return QDL_Att;
			
			case King:
				if (selectionStatus == NON_SEL)
					return KDL;
				else if (selectionStatus == SEL)
					return KDL_Sel;
				else
					return KDL_Att;
			
			default:
				if (selectionStatus == NON_SEL)
					return LSQ;
				else
					return LSQ_Att;
			}
		}
	}
}

/*	Only for initial board setting.
creates the desired board square, adds it to the ccp and returns a pointer to the ccb */ //ccp_BoardSetting, crd, type, player, toolFunc
ControlComponent *createSquareByToolType( ControlComponent *ccp, ControlComponent *ccb, Button *button, Coord crd, eTool type, int player, btnFunc f)
{
	SDL_Rect rect = createSDL_RectForBoardSquare(crd);
	const char *pictureName = getPictureName_tools(crd, player, type, NON_SEL);
	createButton_square(ccb, button, rect, uploadPicture(pictureName), f);
	ccb->btn->crd = crd;
	addButtonToPanel(ccp, ccb);
	return ccb;
}
