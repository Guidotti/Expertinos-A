
#include "BasicPlayer.h"

SoccerCommand BasicPlayer::lookActions( int numAction, bool withBall )
{
	if ( withBall )
	{
		switch (numAction){
		case 1:
			return STpassFAST() ;
		case 2:
			return NDpassFAST() ;
		case 3:
			return RDpassFAST() ;
		case 4:
			return THpassFAST() ;
		case 5:
			return STpassMED() ;
		case 6:
			return NDpassMED() ;
		case 7:
			return RDpassMED() ;
		case 8:
			return THpassMED() ;
		case 9:
			return STpassSLOW() ;
		case 10:
			return NDpassSLOW() ;
		case 11:
			return RDpassSLOW() ;
		case 12:
			return THpassSLOW() ;
		case 13:
			return STpassMAX() ;
		case 14:
			return NDpassMAX() ;
		case 15:
			return RDpassMAX() ;
		case 16:
			return THpassMAX() ;
		case 17:
			return STchipLONG() ;
		case 18:
			return STchipMED() ;
		case 19:
			return STchipSHO() ;
		case 20:
			return STchipVSHO() ;
		case 21:
			return NDchipLONG() ;
		case 22:
			return NDchipMED() ;
		case 23:
			return NDchipSHO() ;
		case 24:
			return NDchipVSHO() ;
		case 25:
			return RDchipLONG() ;
		case 26:
			return RDchipMED() ;
		case 27:
			return RDchipSHO() ;
		case 28:
			return RDchipVSHO() ;
		case 29:
			return THchipLONG() ;
		case 30:
			return THchipMED() ;
		case 31:
			return THchipSHO() ;
		case 32:
			return THchipVSHO() ;
		case 33:
			return STthroughLONG() ;
		case 34:
			return STthroughMED() ;
		case 35:
			return STthroughSHO() ;
		case 36:
			return STthroughVSHO() ;
		case 37:
			return NDthroughLONG() ;
		case 38:
			return NDthroughMED() ;
		case 39:
			return NDthroughSHO() ;
		case 40:
			return NDthroughVSHO() ;
		case 41:
			return RDthroughLONG() ;
		case 42:
			return RDthroughMED() ;
		case 43:
			return RDthroughSHO() ;
		case 44:
			return RDthroughVSHO() ;
		case 45:
			return THthroughLONG() ;
		case 46:
			return THthroughMED() ;
		case 47:
			return THthroughSHO() ;
		case 48:
			return THthroughVSHO() ;
		case 49:
			return GoalKickCenter() ;
		case 50:
			return GoalKickRight100() ;
		case 51:
			return GoalKickRight75() ;
		case 52:
			return GoalKickRight50() ;
		case 53:
			return GoalKickRight25() ;
		case 54:
			return GoalKickLeft25() ;
		case 55:
			return GoalKickLeft50() ;
		case 56:
			return GoalKickLeft75() ;
		case 57:
			return GoalKickLeft100() ;
		case 58:
			return OutplayStClosestOppLong() ;
		case 59:
			return OutplayNdClosestOppLong() ;
		case 60:
			return OutplayStClosestOppMed() ;
		case 61:
			return OutplayNdClosestOppMed() ;
		case 62:
			return OutplayStClosestOppSho() ;
		case 63:
			return OutplayNdClosestOppSho() ;
		case 64:
			return CloseKickLong() ;
		case 65:
			return CloseKickMed() ;
		case 66:
			return CloseKickSho() ;
		case 67:
			return DribbleSlowToP15() ;
		case 68:
			return DribbleSlowToP30() ;
		case 69:
			return DribbleSlowToP45() ;
		case 70:
			return DribbleSlowToP60() ;
		case 71:
			return DribbleSlowToP75() ;
		case 72:
			return DribbleSlowToP90() ;
		case 73:
			return DribbleSlowToM15() ;
		case 74:
			return DribbleSlowToM30() ;
		case 75:
			return DribbleSlowToM45() ;
		case 76:
			return DribbleSlowToM60() ;
		case 77:
			return DribbleSlowToM75() ;
		case 78:
			return DribbleSlowToM90() ;
		case 79:
			return DribbleFastToP15() ;
		case 80:
			return DribbleFastToP30() ;
		case 81:
			return DribbleFastToP45() ;
		case 82:
			return DribbleFastToP60() ;
		case 83:
			return DribbleFastToP75() ;
		case 84:
			return DribbleFastToP90() ;
		case 85:
			return DribbleFastToM15() ;
		case 86:
			return DribbleFastToM30() ;
		case 87:
			return DribbleFastToM45() ;
		case 88:
			return DribbleFastToM60() ;
		case 89:
			return DribbleFastToM75() ;
		case 90:
			return DribbleFastToM90() ;
		case 91:
			return holdBall() ;
		case 92:
			return clearBall( CLEAR_BALL_DEFENSIVE ) ;
		case 93:
			return clearBall( CLEAR_BALL_OFFENSIVE ) ;
		}
	}
	else
	{
		switch(numAction)
		{
		case 94:
			return turnBodyToObject( OBJECT_BALL );
		case 95:
			return moveToPos( WM->getStrategicPosition(), PS->getPlayerWhenToTurnAngle() );
		case 96:
			return InterceptBall();
		case 97:
			return alignNeckWithBody();
		case 98:
			return searchBall();
		case 99:
			return turnNeckToObject( OBJECT_BALL, turnBodyToObject( OBJECT_BALL ) );
		//case 98:MoveToBallLineLONG
		//case 99:MoveToBallLineMED
		//case 100:MoveToBallLineSHO
		//case 101:MoveToBallLineVSHO
		}
	}
	cerr << "ERROR Invalid CMD_ACT_NUM" << endl;
	return CMD_ILLEGAL ;
}

SoccerCommand BasicPlayer::STpassFAST()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::NDpassFAST()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::RDpassFAST()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::THpassFAST()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::STpassMED()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	return kickTo( vector, ( PS->getFastPassEndSpeed() + PS->getPassEndSpeed() )/2.0);
}

SoccerCommand BasicPlayer::NDpassMED()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	return kickTo( vector, ( PS->getFastPassEndSpeed() + PS->getPassEndSpeed() )/2.0 ) ;
}

SoccerCommand BasicPlayer::RDpassMED()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	return kickTo( vector, ( PS->getFastPassEndSpeed() + PS->getPassEndSpeed() )/2.0 ) ;
}

SoccerCommand BasicPlayer::THpassMED()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	return kickTo( vector, ( PS->getFastPassEndSpeed() + PS->getPassEndSpeed() )/2.0 ) ;
}

SoccerCommand BasicPlayer::STpassSLOW()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	return kickTo( vector, PS->getPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::NDpassSLOW()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	return kickTo( vector, PS->getPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::RDpassSLOW()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	return kickTo( vector, PS->getPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::THpassSLOW()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	return kickTo( vector, PS->getPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::STpassMAX()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	return kickTo( vector, SS->getBallSpeedMax() ) ;
}

SoccerCommand BasicPlayer::NDpassMAX()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	return kickTo( vector, SS->getBallSpeedMax() ) ;
}

SoccerCommand BasicPlayer::RDpassMAX()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	return kickTo( vector, SS->getBallSpeedMax() ) ;
}

SoccerCommand BasicPlayer::THpassMAX()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	return kickTo( vector, SS->getBallSpeedMax() ) ;
}

SoccerCommand BasicPlayer::STchipLONG()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	double newX = vector.getX() + 15.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::STchipMED()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX);
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::STchipSHO()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	double newX = vector.getX() + 7.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX);
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::STchipVSHO()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;
	double newX = vector.getX() + 4.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX);
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::NDchipLONG()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	double newX = vector.getX() + 15.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::NDchipMED()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::NDchipSHO()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	double newX = vector.getX() + 7.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::NDchipVSHO()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;
	double newX = vector.getX() + 4.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::RDchipLONG()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	double newX = vector.getX() + 15.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::RDchipMED()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::RDchipSHO()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	double newX = vector.getX() + 7.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::RDchipVSHO()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;
	double newX = vector.getX() + 4.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::THchipLONG()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	double newX = vector.getX() + 15.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::THchipMED()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::THchipSHO()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	double newX = vector.getX() + 7.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::THchipVSHO()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;
	double newX = vector.getX() + 4.0 ;
	if( newX >= 51.5 ){
		newX = vector.getX() + ( 52.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return kickTo( vector, PS->getFastPassEndSpeed() ) ;
}

SoccerCommand BasicPlayer::STthroughLONG()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;

	double newX = vector.getX() + 20.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( STteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::STthroughMED()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;

	double newX = vector.getX() + 17.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( STteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::STthroughSHO()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;

	double newX = vector.getX() + 14.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( STteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::STthroughVSHO()
{
	ObjectT STteamClosest = getClosestTeammateToAgent() ;
	VecPosition vector = WM->getGlobalPosition( STteamClosest ) ;

	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( STteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::NDthroughLONG()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;

	double newX = vector.getX() + 20.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( NDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::NDthroughMED()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;

	double newX = vector.getX() + 17.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( NDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::NDthroughSHO()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;

	double newX = vector.getX() + 14.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( NDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::NDthroughVSHO()
{
	ObjectT NDteamClosest = getSecondClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( NDteamClosest ) ;

	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( NDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::RDthroughLONG()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;

	double newX = vector.getX() + 20.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( RDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::RDthroughMED()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;

	double newX = vector.getX() + 17.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( RDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::RDthroughSHO()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;

	double newX = vector.getX() + 14.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( RDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::RDthroughVSHO()
{
	ObjectT RDteamClosest = getThirdClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( RDteamClosest ) ;

	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( RDteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::THthroughLONG()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;

	double newX = vector.getX() + 20.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( THteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::THthroughMED()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;

	double newX = vector.getX() + 17.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( THteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::THthroughSHO()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;

	double newX = vector.getX() + 14.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( THteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::THthroughVSHO()
{
	ObjectT THteamClosest = getFourthClosestTeamToAgent() ;
	VecPosition vector = WM->getGlobalPosition( THteamClosest ) ;

	double newX = vector.getX() + 10.0 ;
	if( newX >= 51.0 ){
		newX = vector.getX() + ( 51.5 - vector.getX() )/2.0 ;
	}
	vector.setX(newX) ;
	return throughPass( THteamClosest, vector ) ;
}

SoccerCommand BasicPlayer::GoalKickCenter()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, 0 );

	return kickTo(shotPos, SS->getBallSpeedMax());
}

SoccerCommand BasicPlayer::GoalKickRight100()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, -1.0*( SS->getGoalWidth()/2.0 - SS->getBallSize()*2.0 ) ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickRight75()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, -1.0*( (SS->getGoalWidth()/2.0)*0.75 ) ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickRight50()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, -1.0*( (SS->getGoalWidth()/2.0)*0.5 ) ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickRight25()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, -1.0*( (SS->getGoalWidth()/2.0)*0.25 ) ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickLeft25()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, (SS->getGoalWidth()/2.0)*0.25 ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickLeft50()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, (SS->getGoalWidth()/2.0)*0.5 ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickLeft75()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, (SS->getGoalWidth()/2.0)*0.75 ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::GoalKickLeft100()
{
	VecPosition shotPos( PITCH_LENGTH/2.0, SS->getGoalWidth()/2.0 - SS->getBallSize()*2.0 ) ;

	return kickTo(shotPos, SS->getBallSpeedMax()) ;
}

SoccerCommand BasicPlayer::OutplayStClosestOppLong()
{
	ObjectT Opp = WM->getClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	VecPosition AgentPos = WM->getAgentGlobalPosition();
	double DistFactor = 20.0;

	VecPosition ShotPos = AgentPos + VecPosition( DistFactor, WM->getAgentGlobalBodyAngle(), POLAR );

	//ShotPos.setX( AgentPos.getX() + DistFactor*cosDeg( WM->getAgentGlobalBodyAngle() ) ) ;
	//ShotPos.setY( AgentPos.getX() + DistFactor*sinDeg( WM->getAgentGlobalBodyAngle() ) ) ;

	if( fabs( ShotPos.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		ShotPos.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(ShotPos.getY()) );
	if( fabs( ShotPos.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		ShotPos.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(ShotPos.getX()) );

	return outplayOpponent( Opp, ShotPos ) ;
}

SoccerCommand BasicPlayer::OutplayNdClosestOppLong()
{
	ObjectT Opp = WM->getSecondClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	VecPosition AgentPos = WM->getAgentGlobalPosition();
	double DistFactor = 20.0;

	VecPosition ShotPos = AgentPos + VecPosition( DistFactor, WM->getAgentGlobalBodyAngle(), POLAR );

	//ShotPos.setX( AgentPos.getX() + DistFactor*cosDeg( WM->getAgentGlobalBodyAngle() ) ) ;
	//ShotPos.setY( AgentPos.getX() + DistFactor*sinDeg( WM->getAgentGlobalBodyAngle() ) ) ;

	if( fabs( ShotPos.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		ShotPos.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(ShotPos.getY()) );
	if( fabs( ShotPos.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		ShotPos.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(ShotPos.getX()) );

	return outplayOpponent( Opp, ShotPos ) ;
}

SoccerCommand BasicPlayer::OutplayStClosestOppMed()
{
	ObjectT Opp = WM->getClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	VecPosition AgentPos = WM->getAgentGlobalPosition();
	double DistFactor = 13.0;

	VecPosition ShotPos = AgentPos + VecPosition( DistFactor, WM->getAgentGlobalBodyAngle(), POLAR );

	//ShotPos.setX( AgentPos.getX() + DistFactor*cosDeg( WM->getAgentGlobalBodyAngle() ) ) ;
	//ShotPos.setY( AgentPos.getX() + DistFactor*sinDeg( WM->getAgentGlobalBodyAngle() ) ) ;

	if( fabs( ShotPos.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		ShotPos.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(ShotPos.getY()) );
	if( fabs( ShotPos.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		ShotPos.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(ShotPos.getX()) );

	return outplayOpponent( Opp, ShotPos ) ;
}

SoccerCommand BasicPlayer::OutplayNdClosestOppMed()
{
	ObjectT Opp = WM->getSecondClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	VecPosition AgentPos = WM->getAgentGlobalPosition();
	double DistFactor = 13.0;

	VecPosition ShotPos = AgentPos + VecPosition( DistFactor, WM->getAgentGlobalBodyAngle(), POLAR );

	//ShotPos.setX( AgentPos.getX() + DistFactor*cosDeg( WM->getAgentGlobalBodyAngle() ) ) ;
	//ShotPos.setY( AgentPos.getX() + DistFactor*sinDeg( WM->getAgentGlobalBodyAngle() ) ) ;

	if( fabs( ShotPos.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		ShotPos.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(ShotPos.getY()) );
	if( fabs( ShotPos.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		ShotPos.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(ShotPos.getX()) );

	return outplayOpponent( Opp, ShotPos ) ;
}

SoccerCommand BasicPlayer::OutplayStClosestOppSho()
{
	ObjectT Opp = WM->getClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	VecPosition AgentPos = WM->getAgentGlobalPosition();
	double DistFactor = 9.0;

	VecPosition ShotPos = AgentPos + VecPosition( DistFactor, WM->getAgentGlobalBodyAngle(), POLAR );

	//ShotPos.setX( AgentPos.getX() + DistFactor*cosDeg( WM->getAgentGlobalBodyAngle() ) ) ;
	//ShotPos.setY( AgentPos.getX() + DistFactor*sinDeg( WM->getAgentGlobalBodyAngle() ) ) ;

	if( fabs( ShotPos.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		ShotPos.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(ShotPos.getY()) );
	if( fabs( ShotPos.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		ShotPos.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(ShotPos.getX()) );

	return outplayOpponent( Opp, ShotPos ) ;
}

SoccerCommand BasicPlayer::OutplayNdClosestOppSho()
{
	ObjectT Opp = WM->getSecondClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	VecPosition AgentPos = WM->getAgentGlobalPosition();
	double DistFactor = 9.0;

	VecPosition ShotPos = AgentPos + VecPosition( DistFactor, WM->getAgentGlobalBodyAngle(), POLAR );

	//ShotPos.setX( AgentPos.getX() + DistFactor*cosDeg( WM->getAgentGlobalBodyAngle() ) ) ;
	//ShotPos.setY( AgentPos.getX() + DistFactor*sinDeg( WM->getAgentGlobalBodyAngle() ) ) ;

	if( fabs( ShotPos.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		ShotPos.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(ShotPos.getY()) );
	if( fabs( ShotPos.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		ShotPos.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(ShotPos.getX()) );

	return outplayOpponent( Opp, ShotPos ) ;
}

SoccerCommand BasicPlayer::CloseKickLong()
{
	return CloseKick( CK_LONG ) ;
}

SoccerCommand BasicPlayer::CloseKickMed()
{
	return CloseKick( CK_MED ) ;
}

SoccerCommand BasicPlayer::CloseKickSho()
{
	return CloseKick( CK_SHO ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToP15()
{
	return DribbleToAng( 15, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToP30()
{
	return DribbleToAng( 30, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToP45()
{
	return DribbleToAng( 45, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToP60()
{
	return DribbleToAng( 60, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToP75()
{
	return DribbleToAng( 75, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToP90()
{
	return DribbleToAng( 90, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToM15()
{
	return DribbleToAng( -15, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToM30()
{
	return DribbleToAng( -30, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToM45()
{
	return DribbleToAng( -45, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToM60()
{
	return DribbleToAng( -60, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToM75()
{
	return DribbleToAng( -75, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleSlowToM90()
{
	return DribbleToAng( -90, DRIBBLE_SLOW ) ;
}

SoccerCommand BasicPlayer::DribbleFastToP15()
{
	return DribbleToAng( 15, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToP30()
{
	return DribbleToAng( 30, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToP45()
{
	return DribbleToAng( 45, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToP60()
{
	return DribbleToAng( 60, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToP75()
{
	return DribbleToAng( 75, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToP90()
{
	return DribbleToAng( 90, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToM15()
{
	return DribbleToAng( -15, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToM30()
{
	return DribbleToAng( -30, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToM45()
{
	return DribbleToAng( -45, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToM60()
{
	return DribbleToAng( -60, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToM75()
{
	return DribbleToAng( -75, DRIBBLE_FAST ) ;
}

SoccerCommand BasicPlayer::DribbleFastToM90()
{
	return DribbleToAng( -90, DRIBBLE_FAST ) ;
}

/*
 * Com esse metodo o agent irá executar um chute proximo ao seu corpo, mantendo o angulo de seu corpo.
 * com isso é esperado que a bola mantenha-se com o próprio jogador. Há tres tipos de CloseKick, CK_LONG
 * CK_MED e CK_SHO, que repesentam as distâncias do Agent até posição final que a bola será chutada.
 */
SoccerCommand BasicPlayer::CloseKick( CloseKickT ckType)
{
	double kickDist;
	VecPosition posAgent = WM->getAgentGlobalPosition();

	switch( ckType ){
	case CK_LONG:
		kickDist = 8.0;
		break;
	case CK_MED:
		kickDist = 5.5;
		break;
	case CK_SHO:
		kickDist = 3.0;
		break;
	default:
		kickDist = 0.0;
		break;
	}

	VecPosition posKick = posAgent + VecPosition( kickDist, WM->getAgentGlobalBodyAngle(), POLAR );

	if( fabs( posKick.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		posKick.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(posKick.getY()) );
	if( fabs( posKick.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		posKick.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(posKick.getX()) );

	SoccerCommand soc = kickTo( posKick, 0.6 );

	SoccerCommand soc2 = intercept( false );
	if( soc.dPower < 7  && WM->isCollisionAfterCommand( soc2 ) == false )
	{
		Log.log( 560, "wanted to dribble, but only small kick %f", soc.dPower );
		return soc2;
	}

	return soc;
}

SoccerCommand BasicPlayer::DribbleToAng(AngDeg ang, DribbleT d)
{
	double        dLength;
	AngDeg        angBody  = WM->getAgentGlobalBodyAngle();
	VecPosition   posAgent = WM->getAgentGlobalPosition();

	AngDeg angDiff = VecPosition::normalizeAngle( ang - angBody );
	if( fabs( angDiff ) > PS->getDribbleAngThr() )
		return turnWithBallTo( ang, PS->getTurnWithBallAngThr(),
				PS->getTurnWithBallFreezeThr() );

	switch( d ){
	case DRIBBLE_SLOW:
		dLength = 3.5;
		break;
	case DRIBBLE_FAST:
		dLength = 6.0;
		break;
	default:
		dLength = 0.0;
		break;
	}

	VecPosition posDribble = posAgent + VecPosition( dLength, angBody, POLAR );

	if( fabs( posDribble.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
		posDribble.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(posDribble.getY()) );
	if( fabs( posDribble.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
		posDribble.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(posDribble.getX()) );

	SoccerCommand soc = kickTo( posDribble, 0.5 );

	SoccerCommand soc2 = intercept( false );
	if( soc.dPower < 7  && WM->isCollisionAfterCommand( soc2 ) == false )
	{
		Log.log( 560, "wanted to dribble, but only small kick %f", soc.dPower );
		return soc2;
	}

	return soc;
}

SoccerCommand BasicPlayer::MarkOppAndBall()
{

	ObjectT o = WM->getClosestRelativeInSet( OBJECT_SET_OPPONENTS ) ;
	if( o == OBJECT_ILLEGAL ) return moveToPos( WM->getStrategicPosition(), PS->getPlayerWhenToTurnAngle() );
	double dDist = 5.0 ;
	MarkT mark = MARK_BALL ;
	VecPosition posMark  = getMarkingPosition( o, dDist, mark );
	VecPosition posAgent = WM->getAgentGlobalPosition();
	VecPosition posBall  = WM->getGlobalPosition( OBJECT_BALL );

	if( o == OBJECT_BALL )
	{
		if( posMark.getDistanceTo( posAgent ) < 1.5 )
			return turnBodyToObject( OBJECT_BALL );
		else
			return moveToPos( posMark, 30.0, 3.0, false );
	}

	if( posAgent.getDistanceTo( posMark ) < 2.0 )
	{
		AngDeg angOpp = (WM->getGlobalPosition( o ) - posAgent).getDirection();
		AngDeg angBall = (posBall - posAgent).getDirection();
		if( isAngInInterval( angBall, angOpp,
				VecPosition::normalizeAngle( angOpp + 180 ) ) )
			angOpp += 80;
		else
			angOpp -= 80;
		angOpp  = VecPosition::normalizeAngle( angOpp );
		Log.log( 513, "mark: turn body to ang %f", angOpp );
		return turnBodyToPoint( posAgent + VecPosition( 1.0, angOpp, POLAR )  );
	}
	Log.log( 513, "move to marking position" );
	return moveToPos( posMark, 25, 3.0, false );
}

SoccerCommand BasicPlayer::InterceptBall()
{
	SoccerCommand soccmd = intercept( false );

	if( soccmd.commandType == CMD_DASH &&
			WM->getAgentStamina().getStamina() <
			SS->getRecoverDecThr()*SS->getStaminaMax()+200 ) // if stamina low
	{
		soccmd.dPower = 30.0 * WM->getAgentStamina().getRecovery(); // dash slow
		ACT->putCommandInQueue( soccmd );
		ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soccmd ) );
	}
	else                                           // if stamina high
	{
		ACT->putCommandInQueue( soccmd );               // dash as intended
	}

	return soccmd;
}

SoccerCommand BasicPlayer::AR_moveToPos( VecPosition pos )
{
	return moveToPos( pos, PS->getPlayerWhenToTurnAngle() );
}

VecPosition BasicPlayer::MoveToObjLine( ObjectT obj, MoveDistT distType )
{
	VecPosition objPos = WM->getGlobalPosition( obj );
	VecPosition dirVector = WM->getGlobalVelocity( obj );
	VecPosition movePos;
	double DistFactor;

	switch( distType )
	{
	case MD_LONG:
		DistFactor = 15.0;
		break;
	case MD_MED:
		DistFactor = 11.0;
		break;
	case MD_SHO:
		DistFactor = 7.0;
		break;
	case MD_VSHO:
		DistFactor = 3.0;
		break;
	}

	if( dirVector.getMagnitude() == 0 ) // Cannot make the line
		movePos = MoveToObjLine( obj, distType );
	else
		movePos = objPos + VecPosition( DistFactor, dirVector.getDirection(), POLAR );

	if( abs( movePos.getX() ) >= 52.5 || abs( movePos.getY() ) >= 34.0 )
	{
		if( movePos.getX() > 0 )
		{
			movePos.setX( 52.5 - ( movePos.getX() - 52.5 )/2.0 );
		}
		else
		{
			movePos.setX( -52.5 + ( movePos.getX() + 52.5 )/2.0 );
		}
		if( movePos.getY() > 0 )
		{
			movePos.setY( 34.0 - ( movePos.getY() - 34.0 )/2.0 );
		}
		else
		{
			movePos.setX( -34.0 + ( movePos.getX() + 34.0 )/2.0 );
		}
	}

	return movePos;
}

VecPosition BasicPlayer::MoveToObjGoalLine( ObjectT obj, MoveDistT distType )
{
	VecPosition objPos = WM->getGlobalPosition( obj );
	VecPosition movePos;
	double DistFactor;

	switch( distType )
	{
	case MD_LONG:
		DistFactor = 15.0;
		break;
	case MD_MED:
		DistFactor = 10.0;
		break;
	case MD_SHO:
		DistFactor = 8.0;
		break;
	case MD_VSHO:
		DistFactor = 3.5;
		break;
	}

	movePos.setX( 52.0 ); // Goal X pos
	movePos.setY( 0.0 ); // Goal Y pos

	movePos = objPos + VecPosition( DistFactor,
			atanDeg( (movePos.getY() - objPos.getY())/(movePos.getX() - objPos.getX()) ), POLAR );

	if( abs( movePos.getX() ) >= 52.5 || abs( movePos.getY() ) >= 34.0 )
	{
		if( movePos.getX() > 0 )
		{
			movePos.setX( 52.5 - ( movePos.getX() - 52.5 )/2.0 );
		}
		else
		{
			movePos.setX( -52.5 + ( movePos.getX() + 52.5 )/2.0 );
		}
		if( movePos.getY() > 0 )
		{
			movePos.setY( 34.0 - ( movePos.getY() - 34.0 )/2.0 );
		}
		else
		{
			movePos.setX( -34.0 + ( movePos.getX() + 34.0 )/2.0 );
		}
	}

	return movePos;
}

void BasicPlayer::MoveToSector( MoveDistT distType )
{
	double divX, divY;
	int i,j;
	int Sector = PitchDiscretization( WM->getAgentGlobalPosition(), &divX, &divY, &i, &j );

	if( divX*i <= WM->getAgentGlobalPosition().getX() && divY*j <= WM->getAgentGlobalPosition().getY() )
		cerr << "WOW IAM SEXY AND I KNOW IT !!!" << endl;
}
