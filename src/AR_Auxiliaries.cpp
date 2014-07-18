
#include "BasicPlayer.h"

ObjectT BasicPlayer::getClosestTeammateToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT teammate = WM->getClosestInSetTo( OBJECT_SET_TEAMMATES, o, dDist ) ;
	return teammate ;
}

ObjectT BasicPlayer::getClosestOpponentToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT opponent = WM->getClosestInSetTo( OBJECT_SET_OPPONENTS, o, dDist ) ;
	return opponent ;
}

ObjectT BasicPlayer::getSecondClosestTeamToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT teammate = WM->getSecondClosestInSetTo( OBJECT_SET_TEAMMATES, o, dDist ) ;
	return teammate ;
}

ObjectT BasicPlayer::getSecondClosestOpponentToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT opponent = WM->getSecondClosestInSetTo( OBJECT_SET_OPPONENTS, o, dDist ) ;
	return opponent ;
}

ObjectT BasicPlayer::getThirdClosestTeamToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT teammate = getThirdClosestInSetTo( OBJECT_SET_TEAMMATES, o, dDist ) ;
	return teammate ;
}

ObjectT BasicPlayer::getThirdClosestOpponentToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT opponent = getThirdClosestInSetTo( OBJECT_SET_OPPONENTS, o, dDist ) ;
	return opponent ;
}

ObjectT BasicPlayer::getFourthClosestTeamToAgent( double *dDist )
{
	ObjectT o = WM->getAgentObjectType() ;
	ObjectT teammate = getFourthClosestInSetTo( OBJECT_SET_TEAMMATES, o, dDist ) ;
	return teammate ;
}

ObjectT BasicPlayer::getThirdClosestInSetTo ( ObjectSetT set, ObjectT obj,
		double *dDist,  double dConfThr )
{
	VecPosition v;
	ObjectT     closestObject       = OBJECT_ILLEGAL;
	ObjectT     secondClosestObject = OBJECT_ILLEGAL;
	ObjectT     thirdClosestObject  = OBJECT_ILLEGAL;
	double      dMinMag             = 1000.0;
	double      dSecondMinMag       = 1000.0;
	double      dThirdMinMag        = 1000.0;
	int         iIndex;
	if( dConfThr == -1.0 ) dConfThr = WM->PS->getPlayerConfThr();
	for( ObjectT o = WM->iterateObjectStart( iIndex, set, dConfThr );
			o != OBJECT_ILLEGAL;
			o = WM->iterateObjectNext ( iIndex, set, dConfThr ) )
	{
		if( o != obj )
		{
			v = WM->getGlobalPosition( obj ) - WM->getGlobalPosition( o );
			if( v.getMagnitude() < dMinMag )                  // closer then first
					{
				dThirdMinMag          = dSecondMinMag;
				thirdClosestObject    = secondClosestObject;
				dSecondMinMag         = dMinMag;                // put first to second
				secondClosestObject   = closestObject;
				dMinMag               = v.getMagnitude();       // and this to first
				closestObject         = o;
					}
			else if( v.getMagnitude() < dSecondMinMag )       // between 1st and 2nd
			{
				dThirdMinMag          = dSecondMinMag;
				thirdClosestObject    = secondClosestObject;
				dSecondMinMag         = v.getMagnitude();       // put this to second
				secondClosestObject   = o;
			}
			else if( v.getMagnitude() < dThirdMinMag )       // between 2nd and 3rd
			{
				dThirdMinMag         = v.getMagnitude();       // put this to third
				thirdClosestObject   = o;
			}
		}
	}
	WM->iterateObjectDone( iIndex );
	if( dDist != NULL )
		*dDist = dThirdMinMag;
	if( thirdClosestObject == OBJECT_ILLEGAL || thirdClosestObject != OBJECT_UNKNOWN )
		return WM->getSecondClosestInSetTo( set, obj, dDist ) ;
	else
		return thirdClosestObject;
}

ObjectT BasicPlayer::getFourthClosestInSetTo ( ObjectSetT set, ObjectT obj,
		double *dDist,  double dConfThr )
{
	VecPosition v;
	ObjectT     closestObject       = OBJECT_ILLEGAL;
	ObjectT     secondClosestObject = OBJECT_ILLEGAL;
	ObjectT     thirdClosestObject  = OBJECT_ILLEGAL;
	ObjectT     fourthClosestObject = OBJECT_ILLEGAL;
	double      dMinMag             = 1000.0;
	double      dSecondMinMag       = 1000.0;
	double      dThirdMinMag        = 1000.0;
	double      dFourthMinMag       = 1000.0;
	int         iIndex;

	if( dConfThr == -1.0 ) dConfThr = WM->PS->getPlayerConfThr();

	for( ObjectT o = WM->iterateObjectStart( iIndex, set, dConfThr );
			o != OBJECT_ILLEGAL;
			o = WM->iterateObjectNext ( iIndex, set, dConfThr ) )
	{
		if( o != obj )
		{
			v = WM->getGlobalPosition( obj ) - WM->getGlobalPosition( o );
			if( v.getMagnitude() < dMinMag )                  // closer then first
					{
				dFourthMinMag         = dThirdMinMag;			// put third to fourth
				fourthClosestObject   = thirdClosestObject;
				dThirdMinMag          = dSecondMinMag;			// put second to third
				thirdClosestObject    = secondClosestObject;
				dSecondMinMag         = dMinMag;                // put first to second
				secondClosestObject   = closestObject;
				dMinMag               = v.getMagnitude();       // and this to first
				closestObject         = o;
					}
			else if( v.getMagnitude() < dSecondMinMag )       // between 1st and 2nd
			{
				dFourthMinMag         = dThirdMinMag;
				fourthClosestObject   = thirdClosestObject;
				dThirdMinMag          = dSecondMinMag;
				thirdClosestObject    = secondClosestObject;
				dSecondMinMag         = v.getMagnitude();       // put this to second
				secondClosestObject   = o;
			}
			else if( v.getMagnitude() < dThirdMinMag )       // between 2nd and 3rd
			{
				dFourthMinMag         = dThirdMinMag;
				fourthClosestObject   = thirdClosestObject;
				dThirdMinMag          = v.getMagnitude();       // put this to third
				thirdClosestObject    = o;
			}
			else if( v.getMagnitude() < dFourthMinMag )       // between 3nd and 4th
			{
				dFourthMinMag        = v.getMagnitude();		// put this to fourth
				fourthClosestObject  = o;
			}
		}
	}
	WM->iterateObjectDone( iIndex );
	if( dDist != NULL )
		*dDist = dFourthMinMag;
	if( fourthClosestObject == OBJECT_ILLEGAL || fourthClosestObject != OBJECT_UNKNOWN )
		return getThirdClosestInSetTo( set, obj, dDist ) ;
	else
		return fourthClosestObject;
}

bool BasicPlayer::AR_isBallInOurPossesion()
{
	if( WM->isDeadBallUs() )
		return true;
	else if( WM->isDeadBallThem() )
		return false;

	if( SoccerTypes::isTeammate( WM->getClosestInSetTo(OBJECT_SET_PLAYERS, OBJECT_BALL) )
	&& !isBallFloating())
		return true;
	else if(areWeLastTeamBallHolder())
		return true;
	else
	{
		if(!SoccerTypes::isTeammate( WM->getClosestInSetTo(OBJECT_SET_PLAYERS, OBJECT_BALL) )
		&& !isBallFloating())
			return false;
		else if(!areWeLastTeamBallHolder())
			return false;
	}
	return false;
}

bool BasicPlayer::areWeLastTeamBallHolder()
{
	if( SoccerTypes::isTeammate( WM->getClosestInSetTo(OBJECT_SET_PLAYERS, OBJECT_BALL) ))
		return true;
	else if(AR->prevPossesion && isBallFloating())
		return true;
	return false;
}

bool BasicPlayer::isBallFloating()
{
	double dConfThr = WM->PS->getPlayerConfThr();
	int iIndex;
	ObjectSetT set = OBJECT_SET_PLAYERS;

	for( ObjectT o = WM->iterateObjectStart( iIndex, set, dConfThr );
			o != OBJECT_ILLEGAL;
			o = WM->iterateObjectNext ( iIndex, set, dConfThr ) )
	{
		if( ( WM->getGlobalPosition( o ) - WM->getGlobalPosition( OBJECT_BALL ) ).getMagnitude() < SS->getMaximalKickDist() )
			return false;
	}

	return true;
}
