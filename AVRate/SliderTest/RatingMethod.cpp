#include "StdAfx.h"
#include "Controller.h"
#include "RatingMethod.h"
#include "Playlist.h"
#include "Types.h"

using namespace SliderTest;

RatingMethod::RatingMethod(Controller^ c)
{
	this->controller = c;
}

//STANDARD--------------------------------------------------------------------------------------------
RatingMethodStandard::RatingMethodStandard(Controller^ c) : RatingMethod(c)
{
}
void RatingMethodStandard::Init(GeneralSettings^ gs)
{
}
int RatingMethodStandard::getNextItem(void)
{
	int nextpos = controller->currentPos+1;
	if(nextpos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		nextpos = RESULT_ALLITEMSPLAYED;
	return(nextpos);
}
int RatingMethodStandard::getsetNextItem(void)
{
	controller->currentPos += 1;
	if(controller->currentPos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		controller->currentPos = RESULT_ALLITEMSPLAYED;
	return(controller->currentPos);
}
//--------------------------------------------------------------------------------------------

//CONSTANT--------------------------------------------------------------------------------------------
RatingMethodConstant::RatingMethodConstant(Controller^ c) : RatingMethod(c)
{
}
void RatingMethodConstant::Init(GeneralSettings^ gs)
{
	maxnumtrials = gs->ratingmethodbreakcount;
	if(maxnumtrials < 1)
		maxnumtrials = DEFAULT_MAXNUMTRIALS;
	controller->currentPos = nextpos = System::Random().Next(0,controller->GetTotalVideoNumber() );
}
int RatingMethodConstant::getNextItem(void)
{
	if( controller->GetItemsPlayed() >= maxnumtrials)
		return(RESULT_ALLITEMSPLAYED);
	else
		return(nextpos);
}
int RatingMethodConstant::getsetNextItem()
{
	int npos = nextpos;
	controller->currentPos = npos;

	if( controller->GetItemsPlayed() >= maxnumtrials)
		npos = nextpos = RESULT_ALLITEMSPLAYED;
	else
		nextpos = System::Random().Next(0,controller->GetTotalVideoNumber() );

	return(npos);
}
//--------------------------------------------------------------------------------------------

//STAIRCASE--------------------------------------------------------------------------------------------
RatingMethodStaircase::RatingMethodStaircase(Controller^ c) : RatingMethod(c)
{
}
void RatingMethodStaircase::Init(GeneralSettings^ gs)
{
//	direction = 0;
	maxnumturns = gs->ratingmethodbreakcount;
	upAnswer = 0;
	downAnswer = 0;
	upTarget = gs->ratingmethoduptarget;
	downTarget= gs->ratingmethoddowntarget;
	maxexceeded = 0;
	minexceeded = 0;
	numturns = 0;
	
}
bool RatingMethodStaircase::wasLastTurn(void)
{
	// If last choice was a turn, must be evaluated based on next coming plist item
	int nturns = numturns;
	int curpos = controller->currentPos;
	int direction=0;
	if(curpos == -1)	// begin of plist
		curpos = 0;
	else if(curpos >= 0)
	{
		if(controller->getCurrentAnswer()==0)
		{
			int up = upAnswer + 1;
			if(!(up % upTarget))
				direction = -1;
		}
		else if(controller->getCurrentAnswer()==1)
		{
			//upAnswer = 0;
			int down = downAnswer+1;
			if(!(down % downTarget))
				direction = 1;
		}
		// check if max number of turnarounds reached
		//if(direction && (controller->getLastAnswer()>=0) && (controller->getCurrentAnswer()!=controller->getLastAnswer()) )
		if( (direction && controller->getLastDirection() ) && (direction != controller->getLastDirection() ) )
			return(true);
	}
	return(false);
}
int RatingMethodStaircase::getNextItem(void)
{
	int nturns = numturns;
	int curpos = controller->currentPos;
	int direction=0;
	if(curpos == -1)	// begin of plist
		curpos = 0;
	else if(curpos >= 0)
	{
		if(controller->getCurrentAnswer()==0)
		{
			int up = upAnswer + 1;
			if(!(up % upTarget))
				direction = -1;
		}
		else if(controller->getCurrentAnswer()==1)
		{
			//upAnswer = 0;
			int down = downAnswer+1;
			if(!(down % downTarget))
				direction = 1;
		}
		// check if max number of turnarounds reached
		//if(direction && (controller->getLastAnswer()>=0) && (controller->getCurrentAnswer()!=controller->getLastAnswer()) )
		if( (direction && controller->getLastDirection() ) && (direction != controller->getLastDirection() ) )
			nturns++;
	}
	int nextpos = curpos + direction;
	if(nturns >= maxnumturns)
		nextpos = RESULT_MAXNUMTURNS;
	else if(nextpos<0)	//beginning of playlist
	{
		int minx = minexceeded+1;
		if(minx >= MAXOF_MINEXCEEDED)
			nextpos = RESULT_MINEXCEEDED;
		else
			nextpos=0;
	}
	else if(nextpos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
	{
		// END OF PLAYLIST
		int maxx = maxexceeded+1;
		if(maxx >= MAXOF_MAXEXCEEDED)
			nextpos = RESULT_MAXEXCEEDED;
		else
			nextpos = curpos;
	}

	return(nextpos);
}
int RatingMethodStaircase::getsetNextItem()
{
	// Finds next item and sets it

	// 1U1D ---> upTarget=1, downTarget=1;
	// 2U1D ---> upTarget=2, downTarget=1;
	//controller->setLastWasTurn(false);
	int curpos = controller->currentPos;
	int direction=0;
	if(curpos == -1)	// begin of plist
		curpos = 0;
	else if(curpos >= 0)
	{
		if(controller->getCurrentAnswer()==0)
		{
			downAnswer = 0;
			upAnswer++;
			if(!(upAnswer % upTarget))
				direction = -1;;
		}
		else if(controller->getCurrentAnswer()==1)
		{
			upAnswer = 0;
			downAnswer++;
			if(!(downAnswer % downTarget))
				direction = 1;
		}
		// check if max number of turnarounds reached
		//if(direction && (controller->getLastAnswer()>=0) && (controller->getCurrentAnswer()!=controller->getLastAnswer()) )
		//	numturns++;
		if( (direction && controller->getLastDirection() ) && (direction != controller->getLastDirection() ) )
		{
			numturns++;
			//controller->setLastWasTurn(true);
		}
	}
	if(direction)
		controller->setLastDirection(direction);
	int nextpos = curpos + direction;
	if(numturns >= maxnumturns)
		nextpos = RESULT_MAXNUMTURNS;
	else if(nextpos<0)	//beginning of playlist
	{
		minexceeded++;
		if(minexceeded >= MAXOF_MINEXCEEDED)
			nextpos = RESULT_MINEXCEEDED;
		else
			nextpos=0;
	}
	else if(nextpos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
	{
		// END OF PLAYLIST
		maxexceeded++;
		if(maxexceeded >= MAXOF_MAXEXCEEDED)
			nextpos = RESULT_MAXEXCEEDED;
		else
			nextpos = curpos;
	}

	controller->currentPos = nextpos;
	return(nextpos);
}
//--------------------------------------------------------------------------------------------

//DOUBLE STAIRCASE--------------------------------------------------------------------------------------------
RatingMethodDouble::RatingMethodDouble(Controller^ c) : RatingMethod(c)
{
	upAnswer = gcnew array<int>(2){0,0};
	downAnswer = gcnew array<int>(2){0,0};
	numturns = gcnew array<int>(2){0,0};
	
}
void RatingMethodDouble::Init(GeneralSettings^ gs)
{
//	direction = 0;
	maxnumturns = gs->ratingmethodbreakcount;
	upTarget = gs->ratingmethoduptarget;
	downTarget= gs->ratingmethoddowntarget;
	maxexceeded = 0;
	minexceeded = 0;
	nextplaylist = 0;//System::Random().Next(0,2);
	currentplaylist = 1;// for first time usage
	controller->currentPosDouble=controller->GetTotalVideoNumber() + controller->GetNumAutoItems();// this is one more than possible pos's
}
bool RatingMethodDouble::wasLastTurn(void)
{
	// If last choice was a turn, must be evaluated based on next coming plist item
	// of the same playlist (regarding double method)
	int curplist = currentplaylist;
	int nturns = numturns[curplist];
	int curpos = controller->currentPos;
	int direction=0;
	if(curpos == -1)	// begin of 1st plist
		curpos = 0;
	else if(curpos == (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		curpos -= 1;	// begin of 2nd plist
	else //if(curpos >= 0)
	{
		if(controller->getCurrentAnswer()==0)
		{
			int up = upAnswer[curplist]+1;
			if(!(up % upTarget))
				direction = -1;
		}
		else if(controller->getCurrentAnswer()==1)
		{
			int down = downAnswer[curplist]+1;
			if(!(down % downTarget))
				direction = 1;
		}
		// check if max number of turnarounds reached
		//if(direction && (controller->getLastAnswer()>=0) && (controller->getCurrentAnswer()!=controller->getLastAnswer()) )
		//	nturns++;
		if( (direction && controller->getLastDirection() ) && (direction != controller->getLastDirection() ) )
			return(true);
	}
	return(false);
}
int RatingMethodDouble::getNextItem(void)
{
	int addnumturn = 0;
	int curplist = currentplaylist;
	if(nextplaylist != currentplaylist)
	{
		if(wasLastTurn())	// FInd it for the previous, non-switched rating.
			addnumturn = 1;
		controller->switchPlaylistSet();//temporary switch
		curplist = nextplaylist;
	}
	int nturns = numturns[curplist];
	int curpos = controller->currentPos;
	int direction=0;
	if(curpos == -1)	// begin of 1st plist
		curpos = 0;
	else if(curpos == (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		curpos -= 1;	// begin of 2nd plist
	else //if(curpos >= 0)
	{
		if(controller->getCurrentAnswer()==0)
		{
			int up = upAnswer[curplist]+1;
			if(!(up % upTarget))
				direction = -1;
		}
		else if(controller->getCurrentAnswer()==1)
		{
			int down = downAnswer[curplist]+1;
			if(!(down % downTarget))
				direction = 1;
		}
		// check if max number of turnarounds reached
		//if(direction && (controller->getLastAnswer()>=0) && (controller->getCurrentAnswer()!=controller->getLastAnswer()) )
		//	nturns++;
		if( (direction && controller->getLastDirection() ) && (direction != controller->getLastDirection() ) )
			nturns++;
	}

	int nextpos = curpos + direction;
	if( (nturns >= maxnumturns)&& (numturns[curplist^1] + addnumturn >= maxnumturns) )
		nextpos = RESULT_MAXNUMTURNS;
	else if(nextpos<0)	//beginning of playlist
	{
		int minx = minexceeded+1;
		if(minx >= MAXOF_MINEXCEEDED)
			nextpos = RESULT_MINEXCEEDED;
		else
			nextpos=0;
	}
	else if(nextpos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
	{
		// END OF PLAYLIST
		int maxx = maxexceeded+1;
		if(maxx >= MAXOF_MAXEXCEEDED)
			nextpos = RESULT_MAXEXCEEDED;
		else
			nextpos = curpos;
	}

	// switch back to original procedure set
	if(nextplaylist != currentplaylist)
		controller->switchPlaylistSet();

	return(nextpos);
}
int RatingMethodDouble::getsetNextItem()
{
	// Finds next item and sets it
	//
	// works AFTER last rating was evaluated in Controller
	// 1U1D ---> upTarget=1, downTarget=1;
	// 2U1D ---> upTarget=2, downTarget=1;

	int addnumturn = 0;
	// Set other playlist value set if Randomizer points to other procedure set
	if(nextplaylist != currentplaylist)
	{
		// Evaluate end of Playlist early
		// if not, all breaking rule numturns are evaluated only AFTER switching once back and forth
		// to original playlist.
		if(wasLastTurn())	// FInd it for the previous, non-switched rating.
			addnumturn = 1;

		// Switch to other set. All values are kept in Controller.
		controller->switchPlaylistSet();
		currentplaylist = nextplaylist;
	}
	//controller->setLastWasTurn(false);
//#define TESTMODE
#ifdef TESTMODE
	if(nextplaylist == 0)
		nextplaylist = 1;
	else
		nextplaylist = 0;
#else
	// Find next procedure to execute
	nextplaylist = System::Random().Next(0,2);
#endif
	int curpos = controller->currentPos;
	int direction=0;
	if(curpos == -1)	// begin of 1st plist
		curpos = 0;
	else if(curpos == (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		curpos -= 1;	// begin of 2nd plist
	else //if(curpos >= 0)
	{
		if(controller->getCurrentAnswer()==0)
		{
			downAnswer[currentplaylist] = 0;
			upAnswer[currentplaylist]++;
			if(!(upAnswer[currentplaylist] % upTarget))
				direction = -1;
		}
		else if(controller->getCurrentAnswer()==1)
		{
			upAnswer[currentplaylist] = 0;
			downAnswer[currentplaylist]++;
			if(!(downAnswer[currentplaylist] % downTarget))
				direction = 1;
		}
		//if(direction)
		//	controller->setLastWasTurn(false);
		// check if max number of turnarounds reached
		//if(direction && (controller->getLastAnswer()>=0) && (controller->getCurrentAnswer()!=controller->getLastAnswer()) )
		//	numturns[currentplaylist]++;
		if( (direction && controller->getLastDirection() ) && (direction != controller->getLastDirection() ) )
		{
			numturns[currentplaylist]++;
			//controller->setLastWasTurn(true);
		}
	}
	if(direction)
		controller->setLastDirection(direction);
	int nextpos = curpos + direction;
	if( (numturns[currentplaylist] >= maxnumturns) && (numturns[currentplaylist^1] + addnumturn >= maxnumturns) )
		nextpos = RESULT_MAXNUMTURNS;
	else if(nextpos<0)	//beginning of playlist
	{
		minexceeded++;
		if(minexceeded >= MAXOF_MINEXCEEDED)
			nextpos = RESULT_MINEXCEEDED;
		else
			nextpos=0;
	}
	else if(nextpos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
	{
		// END OF PLAYLIST
		maxexceeded++;
		if(maxexceeded >= MAXOF_MAXEXCEEDED)
			nextpos = RESULT_MAXEXCEEDED;
		else
			nextpos = curpos;
	}

	controller->currentPos=nextpos;
	return(nextpos);
}

//--------------------------------------------------------------------------------------------
// SAMVIQ

RatingMethodSAMVIQ::RatingMethodSAMVIQ(Controller^ c) : RatingMethod(c)
{
}
void RatingMethodSAMVIQ::Init(GeneralSettings^ gs)
{
}
int RatingMethodSAMVIQ::getNextItem(void)
{
	int nextpos = controller->currentPos+1;
	if(nextpos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		nextpos = RESULT_ALLITEMSPLAYED;
	return(nextpos);
}
int RatingMethodSAMVIQ::getsetNextItem(void)
{
	controller->currentPos += 1;
	if(controller->currentPos >= (controller->GetNumAutoItems() + controller->GetTotalVideoNumber()) )
		controller->currentPos = RESULT_ALLITEMSPLAYED;
	return(controller->currentPos);
}


