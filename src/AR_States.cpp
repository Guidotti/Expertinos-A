/*
 * States.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: Expertinos 2D Soccer Simulation Team
 */

#include "BasicPlayer.h"

/* ###################################################################################
 * ###############################  TIME BASICO  #####################################
 * ###################################################################################
 */

// Metodo para completar o vetor de estados
int BasicPlayer::lookStates( int numState )
{
	switch(numState){
	case 1:
		return PitchDiscretization( WM->getAgentGlobalPosition() ); //16 valores
	case 2:
		return DistToBallDiscretization( WM->getAgentGlobalPosition() ); //5 valores
	case 3:
		return DistToPlayer4Discretization( WM->getAgentGlobalPosition(), getClosestTeammateToAgent() ); //4 valores
	case 4:
		return DistToPlayer4Discretization( WM->getAgentGlobalPosition(), getClosestOpponentToAgent() ); //4 valores
	case 5:
		return DistToPlayer4Discretization( WM->getAgentGlobalPosition(), getSecondClosestTeamToAgent() ); //4 valores
	case 6:
		return DistToPlayer4Discretization( WM->getAgentGlobalPosition(), getSecondClosestOpponentToAgent() ); //4 valores
	case 7:
		return DistToPlayer3Discretization( WM->getAgentGlobalPosition(), getThirdClosestTeamToAgent() ); //3 valores
	case 8:
		return DistToPlayer3Discretization( WM->getAgentGlobalPosition(), getThirdClosestOpponentToAgent() ); //3 valores
	case 9:
		return DistToPlayer3Discretization( WM->getAgentGlobalPosition(), getFourthClosestTeamToAgent() ); //3 valores
	case 10:
		return DirAgent4Discretization( WM->getAgentGlobalPosition() ); //4 valores
	case 11:
		return DirAgent4Discretization( getClosestTeammateToAgent() ); //4 valores
	case 12:
		return DirAgent4Discretization( getSecondClosestTeamToAgent() ); //4 valores
	case 13:
		return DirAgent3Discretization( getThirdClosestTeamToAgent() ); //3 valores
	case 14:
		return DirAgent3Discretization( getFourthClosestTeamToAgent() ); //3 valores
	case 15:
		return DirAgent4Discretization( getClosestOpponentToAgent() ); //4 valores
	case 16:
		return DirAgent3Discretization( getSecondClosestOpponentToAgent() ); //3 valores
	case 17:
		return DirAgent3Discretization( getThirdClosestOpponentToAgent() ); //3 valores
	default:
		cerr << "Error lookState!!" << endl;
		return -1;
	}
}

// Chave de estado 1
int BasicPlayer::PitchDiscretization( VecPosition PlayerPostion, double *dX, double *dY, int *numI, int *numJ )
{
	double p_posX = PlayerPostion.getX() + 52.5;
	double p_posY = PlayerPostion.getY() + 34.0;
	double i=0;
	double j=0;
	double divX=105.0/4.0;
	double divY=68.0/4.0;

	// If asked I pass the divisions values (eg. MoveToSector actions)
	if( dX != NULL )
		*dX = divX;
	if( dY != NULL )
		*dY = divY;

	while( true ){
		i=i+1.0;
		if( divX*i >= p_posX ){
			break;
		}
		if( i > 4.0 ){
			cerr << "Error PitchDiscretization: i ! -- (p_posX, p_posY): (" << p_posX << ", " << p_posY << ")" << endl;
			return -1;
		}
	}

	while( true ){
		j=j+1.0;
		if( divY*j >= p_posY ){
			break;
		}
		if( j > 4.0 ){
			cerr << "Error PitchDiscretization: j ! -- (p_posX, p_posY): (" << p_posX << ", " << p_posY << ")" << endl;
			return -1;
		}
	}

	// If asked I pass the num. of the line/column (eg. MoveToSector actions)
	if( numI != NULL )
		*numI = i;
	if( numJ != NULL )
		*numJ = j;

	return ((i-1)*4+j-1);
}

// Chave de estado 2
int BasicPlayer::DistToBallDiscretization( VecPosition PlayerPostion, ObjectT obj )
{
	VecPosition o_pos = WM->getGlobalPosition( obj );
	VecPosition rel_pos = PlayerPostion - o_pos;
	double dist = rel_pos.getMagnitude();

	if( dist < SS->getMaximalKickDist() ){
		return 0;
	}
	else if( dist < 5.0 ){
		return 1;
	}
	else if( dist < 12.5 ){
		return 2;
	}
	else if( dist < 25.0 ){
		return 3;
	}
	else
		return 4;
}

// Chave de estado 3,4,5 e 6
int BasicPlayer::DistToPlayer4Discretization( VecPosition PlayerPostion, ObjectT obj )
{
	VecPosition o_pos = WM->getGlobalPosition( obj );
	VecPosition rel_pos = PlayerPostion - o_pos;
	double dist = rel_pos.getMagnitude();

	if( dist < 5.0 ){
		return 0;
	}
	else if( dist < 12.5 ){
		return 1;
	}
	else if( dist < 25.0 ){
		return 2;
	}
	else
		return 3;
}

// Chave de estado 7,8 e 9
int BasicPlayer::DistToPlayer3Discretization( VecPosition PlayerPostion, ObjectT obj )
{
	VecPosition o_pos = WM->getGlobalPosition( obj );
	VecPosition rel_pos = PlayerPostion - o_pos;
	double dist = rel_pos.getMagnitude();

	if( dist < 10.0 ){
		return 0;
	}
	else if( dist < 25.0 ){
		return 1;
	}
	else{
		return 3;
	}
}

// Chave de estado 10
int BasicPlayer::DirAgent4Discretization( VecPosition PlayerPostion )
{
	if( PlayerPostion != 0 )
	{
		double goal_x = 105.0; // Gol adversario
		double goal_y = 0.0;
		double Agent_x = PlayerPostion.getX();
		double Agent_y = PlayerPostion.getY();
		double lineCoef = (goal_y - Agent_y)/(goal_x - Agent_x);
		double AngLine = atanDeg( lineCoef );
		double AngAgent = 0;

		if( WM->getAgentGlobalVelocity().getMagnitude() != 0 )
		{
			VecPosition AgentVel = WM->getAgentGlobalVelocity();
			AngAgent = AgentVel.getDirection();
		}
		else{
			AngAgent = WM->getAgentGlobalNeckAngle();
		}

		double relAng = VecPosition::normalizeAngle( AngLine - AngAgent );

		if( relAng >= -25.0 && relAng <= 25.0 )
			return 0;
		else if( relAng > 25.0 && relAng <= 155.0 )
			return 2;
		else if( relAng >= -155.0 && relAng < -25.0 )
			return 3;
		else
			return 1;
	}
	else{
		//cerr << "Invalid PlayerPosition" << endl;
		return 1;
	}
}

// Chave de estado 11, 12 e 15 (diferente do relatorio parcial, igual a chave 10)
int BasicPlayer::DirAgent4Discretization( ObjectT obj )
{
	if( obj != OBJECT_ILLEGAL && obj != OBJECT_UNKNOWN )
	{
		double goal_x = 105.0; // Gol adversario
		double goal_y = 0.0;
		VecPosition AgentPos = WM->getGlobalPosition( obj );
		double Agent_x = AgentPos.getX();
		double Agent_y = AgentPos.getY();
		double lineCoef = (goal_y - Agent_y)/(goal_x - Agent_x);
		double AngLine = atanDeg( lineCoef );
		double AngAgent = 0;

		if( WM->getGlobalVelocity( obj ).getMagnitude() != 0 )
		{
			VecPosition AgentVel = WM->getGlobalVelocity( obj );
			AngAgent = AgentVel.getDirection();
		}
		else{
			AngAgent = WM->getGlobalNeckAngle( obj );
		}

		double relAng = VecPosition::normalizeAngle( AngLine - AngAgent );

		if( relAng >= -25.0 && relAng <= 25.0 )
			return 0;
		else if( relAng > 25.0 && relAng <= 155.0 )
			return 2;
		else if( relAng >= -155.0 && relAng < -25.0 )
			return 3;
		else
			return 1;
	}
	else{
		//cerr << "Illegal or unknown Object" << endl;
		return 1;
	}
}

// Chave de estado 13, 14, 16 e 17 (diferente do relatorio parcial, igual a chave 10)
int BasicPlayer::DirAgent3Discretization( ObjectT obj )
{
	if( obj != OBJECT_ILLEGAL && obj != OBJECT_UNKNOWN )
	{
		// Linha do agente ao gol adversario
		double goal_x = 105.0;
		double goal_y = 0.0;
		VecPosition AgentPos = WM->getGlobalPosition( obj );
		double Agent_x = AgentPos.getX();
		double Agent_y = AgentPos.getY();
		double lineCoef = (goal_y - Agent_y)/(goal_x - Agent_x);
		double AngLine = atanDeg( lineCoef );
		double AngAgent = 0;

		if( WM->getGlobalVelocity( obj ).getMagnitude() != 0 )
		{
			VecPosition AgentVel = WM->getGlobalVelocity( obj );
			AngAgent = AgentVel.getDirection();
		}
		else{
			AngAgent = WM->getGlobalNeckAngle( obj );
		}

		double relAng = VecPosition::normalizeAngle( AngLine - AngAgent );

		if( relAng >= -35.0 && relAng <= 35.0 )
			return 0;
		else if( relAng > 35.0 && relAng <= 180.0 )
			return 1;
		else if( relAng >=-180.0 && relAng < -35.0 )
			return 2;
		else
			return 3;
	}
	else{
		//cerr << "Illegal or unknown Object" << endl;
		return 3;
	}
}


/* ###################################################################################
 * #############################  TIME AVANÇADO  #####################################
 * ###################################################################################
 */

/*
 * Numero de discretizacoes: 100 (sendo o menor valor 0)
 * O campo foi divido em 7 zonas, sendo que na area que pega o centro do campo foram feitas 12 subdivisoes desta zona,
 * nas regioes mais proximas dos gols forao realizadas mais subdivisoes. Nas areas proximas a regiao de escanteio foram
 * feitas 2 divisões, totalizando 8 divisoes com a 4 regiões de escanteio. Na zona proxima a grande area forão feitas outras
 * 40 subdivisões, totalizando 80 somando ambas grandes areas. Desta forma a tendencia é que o jogador tenha uma maior
 * percepção do campo nas zonas criticas, que são as proximas as linhas de fundo..
 *

// [0 - 99]
int Player::PitchDiscretization( VecPosition PlayerPostion )
{
	double p_posX = PlayerPostion.getX() + 52.5;
	double p_posY = PlayerPostion.getY() + 34.0;
	double i=0;
	double j=0;
	double divX;
	double divY;
	int acresc = 0;

	if( p_posX <= 24.5 && p_posY <= 11.0 )
	{
		divX = 12.25;
		j = 1.0;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 2.0 ){
				cerr << "Error PitchDiscretization: i - P.0 !!";
				return -1;
			}
		}
	}
	else if( p_posX <= 24.5 && p_posY <= 57.0 && p_posY >= 11.0)
	{
		acresc = 2;
		divX = 4.9;
		divY = 5.75;

		// Fixing position
		p_posY = p_posY - 11.0;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 5.0 ){
				cerr << "Error PitchDiscretization: i - P.1 !!";
				return -1;
			}
		}
		while( true ){
			j=j+1.0;
			if( j*divY >= p_posY )
				break;
			if (j > 8.0 ){
				cerr << "Error PitchDiscretization: j - P.1 !!";
				return -1;
			}
		}
	}
	else if( p_posX <= 24.5 && p_posY >= 57.0 )
	{
		acresc = 42;
		divX = 12.25;
		j = 1.0;

		// Fixing position
		p_posY = p_posY - 57.0;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 2.0 ){
				cerr << "Error PitchDiscretization: i - P.2 !!";
				return -1;
			}
		}
	}
	else if( p_posX >= 24.5 && p_posX <= 80.5 )
	{
		acresc = 44;
		divX = 18.6666667;
		divY = 17.0;

		// Fixing position
		p_posX = p_posX - 24.5;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 3.0 ){
				cerr << "Error PitchDiscretization: i - P.3 !!";
				return -1;
			}
		}
		while( true ){
			j=j+1.0;
			if( j*divY >= p_posY )
				break;
			if (j > 4.0 ){
				cerr << "Error PitchDiscretization: j - P.3 !!";
				return -1;
			}
		}
	}
	else if( p_posX >= 80.5 && p_posY <= 11.0 )
	{
		acresc = 56;
		divX = 12.25;
		j = 1.0;

		// Fixing position
		p_posX = p_posX - 80.5;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 2.0 ){
				cerr << "Error PitchDiscretization: i - P.4 !!";
				return -1;
			}
		}
	}
	else if( p_posX >=80.5 && p_posY <= 57.0 && p_posY >= 11.0 )
	{
		acresc = 58;
		divX = 4.9;
		divY = 5.75;

		// Fixing position
		p_posX = p_posX - 80.5;
		p_posY = p_posY - 11.0;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 5.0 ){
				cerr << "Error PitchDiscretization: i - P.5 !!";
				return -1;
			}
		}
		while( true ){
			j=j+1.0;
			if( j*divY >= p_posY )
				break;
			if (j > 8.0 ){
				cerr << "Error PitchDiscretization: j - P.5 !!";
				return -1;
			}
		}
	}
	else if( p_posX >= 80.5 && p_posY >= 57.0 )
	{
		acresc = 98;
		divX = 12.25;
		j = 1.0;

		// Fixing position
		p_posX = p_posX - 80.5;
		p_posY = p_posY - 57.0;

		while( true ){
			i=i+1.0;
			if( i*divX >= p_posX )
				break;
			if ( i > 2.0 ){
				cerr << "Error PitchDiscretization: i - P.6 !!";
				return -1;
			}
		}
	}

	// return ((i-1)*10+j-1 + acresc); <-- Precisa corigir isso, colunas e linhas variam (10 errado), precisa colocar para cada if
}

// [0 - 20]
int Player::AngleDiscretization	( bool isAgent, ObjectT o )
{
	AngDeg angBody;
	AngDeg loopAngle = -162;
	int DiscretizationValue = 0;

	if( isAgent )	{
		angBody -= WM->getAgentGlobalBodyAngle();
		angBody = VecPosition::normalizeAngle( angBody );
	}
	else if( o != OBJECT_ILLEGAL )	{
		angBody -= WM->getGlobalBodyAngle( o );
		angBody = VecPosition::normalizeAngle( angBody );
	}
	else{
		cerr << "Error AngleDiscretization: no ObjectT informed !!";
		return -1;
	}

	while( DiscretizationValue < 36 ){
		if( angBody <= loopAngle )
			break;
		else{
			DiscretizationValue++;
			loopAngle += 18;
		}
	}

	return DiscretizationValue;
}
// [0.0;0.05;0.10;0.15;0.20;0.25;0.30;0.35;0.40;0.45;0.50;0.50+]
// [0 - 12]
int Player::VelocDiscretization( bool isAgent, ObjectT o)
{
	VecPosition velocity;
	double LoopVeloc = 0;
	double VelocMag;
	int DiscretizationValue = 0;

	if( isAgent )
		velocity = WM->getAgentGlobalVelocity();
	else if( o != OBJECT_ILLEGAL )
		velocity = WM->getGlobalVelocity( o );
	else {
		cerr << "Error VelocDiscretization: no ObjectT informed !!";
		return -1;
	}

	VelocMag = velocity.getMagnitude();

	while( VelocMag < 0.50 ) {
		if( VelocMag <= LoopVeloc )
			break;
		else{
			DiscretizationValue++;
			LoopVeloc += 0.05;
		}
	}

	return DiscretizationValue;
}

// [200; 800; 1400; 2000; 2600; 3200; 4800; 4800+]
// [0 - 7]
int Player::StaminaDiscretization( )
{
	int DiscretizationValue = 0;
	double stmInfo = WM->AR_getAgentTiredNess();

	if( stmInfo < 200 )
		return 0;
	else if( stmInfo < 800 )
		return 1;
	else if( stmInfo < 1400 )
		return 2;
	else if( stmInfo < 2000 )
		return 3;
	else if( stmInfo < 2600 )
		return 4;
	else if( stmInfo < 3200 )
		return 5;
	else if( stmInfo < 4800 )
		return 6;
	else
		return 7;
}

int Player::ObjDiscretization( ObjectT o )
{
	VecPosition ObjPos = WM->getGlobalPosition( o );
	VecPosition AgentPos = WM->getAgentGlobalPosition();

	double dist = sqrt((ObjPos.getX() - AgentPos.getX())*(ObjPos.getX() - AgentPos.getX()) +
			(ObjPos.getY() - AgentPos.getY())*(ObjPos.getY() - AgentPos.getY()));



	return 0;
}
*/
