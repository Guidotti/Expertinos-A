
#include "Player.h"
#include "BasicPlayer.h"

SoccerCommand Player::deMeer5(  )
{
	// Variaveis do jogo e do agente
	bool Kickable = WM->isBallKickable() ;
	bool Possesion = AR_isBallInOurPossesion();
	int AgentNumber = WM->getPlayerNumber() ;
	int Cycle = WM->getCurrentCycle() ;
	SoccerCommand soc(CMD_ILLEGAL);
	VecPosition   posAgent = WM->getAgentGlobalPosition();
	VecPosition   posBall  = WM->getBallPos();
	int           iTmp;

	// Variaveis para a AR
	int estado[num_chaves];

	AR->Ciclo_atual = Cycle;

	// Construção do vetor de estado ( CHAVES DE ESTADOS )
	if( Cycle > 0 )
	{
		for( int i=0; i < num_chaves; i++ ){
			estado[i] = lookStates(i+1) ;
		}
	}

	if( WM->isBeforeKickOff( ) )
	{
		AR->Ciclo_atual = 1;
		if( WM->isKickOffUs( ) ) // 9 takes kick
		{
			if( AgentNumber == 9 )
			{
				if( Kickable )
				{
					soc = lookActions( 5 ) ; // kick maximal
					Log.log( 100, "take kick off" );
				}
				else
				{
					soc = intercept( false );
					Log.log( 100, "move to ball to take kick-off" );
				}
				ACT->putCommandInQueue( soc );
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
			AR->prevPossesion = true;
			return soc;
		}
		else
			AR->prevPossesion = false;
		if( formations->getFormation() != FT_INITIAL || // not in kickoff formation
				posAgent.getDistanceTo( WM->getStrategicPosition() ) > 2.0 )
		{
			formations->setFormation( FT_INITIAL );       // go to kick_off formation
			ACT->putCommandInQueue( soc=teleportToPos( WM->getStrategicPosition() ));
		}
		else                                            // else turn to center
		{
			ACT->putCommandInQueue( soc=turnBodyToPoint( VecPosition( 0, 0 ), 0 ) );
			ACT->putCommandInQueue( alignNeckWithBody( ) );
		}
	}
	else if( AR->Ciclo_ant != AR->Ciclo_atual )
	{

		AR->SaldoGol_ant = AR->SaldoGol_atual ;
		AR->SaldoGol_atual = WM->getGoalDiff( ) ; // Atualiza o saldo de gols

		if( AR->prevPossesion != Possesion )
		{
			AR->changePossesion = true;
		}
		else
			AR->changePossesion = false;

		/*
		if(AgentNumber == 8) // exemplo utilizacao do changepossesion
		{
			if( Possesion == true )
			{
				if(AR->changePossesion == true )
				{
					cerr << "Recuperamos a bola\n";
				}
				else
				{
					cerr << "Estamos com a bola\n";
				}
			}
			else
			{
				if(AR->changePossesion == true )
				{
					cerr << "Eles recuperaram a bola\n";
				}
				else
				{
					cerr << "Eles estao com a bola\n";
				}
			}
		}*/

		if( AR->SaldoGol_ant != AR->SaldoGol_atual ) // Calculo do saldo de gol
		{
			if( AR->SaldoGol_ant > AR->SaldoGol_atual )
				AR->VetorRelatorio[num_acoes+2]++;
			else
				AR->VetorRelatorio[num_acoes+1]++;
		}

		formations->setFormation( FT_433_OFFENSIVE );
		soc.commandType = CMD_ILLEGAL;

		unsigned long long indiceCMAC[lut_m];

		if( WM->getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )
		{
			ACT->putCommandInQueue( soc = searchBall() );   // if ball pos unknown
			ACT->putCommandInQueue( alignNeckWithBody( ) ); // search for it
			AR->checkState = false;
		}
		else if ( AR->GetIndexCMAC( estado, indiceCMAC ) ) // Recebendo indice do estado do CMAC:
		{
			// Atribuição do valor Q:
			if(CheckIfNeedsNewElementInList( Possesion ))
			{
				if( AR->vetorHeuristica.size() > 0 )
				{
					// Complementa informacao do elemento anterior
					for( int i = 0; i < lut_m; i++  )
						AR->vetorHeuristica.back().indexCMAC_verificacao[i] = indiceCMAC[i];
					AR->vetorHeuristica.back().bolaViajando = isBallFloating();
					AR->vetorHeuristica.back().reforco = 0.0;
				}
				// Adiciona novo elemento
				AR->AddElementToVector(AR->vetorHeuristica);
			}
			else
			{
				// Complementa informacao do elemento anterior
				for( int i = 0; i < lut_m; i++  )
					AR->vetorHeuristica.back().indexCMAC_verificacao[i] = indiceCMAC[i];
				AR->vetorHeuristica.back().bolaViajando = isBallFloating();

				//Atualiza a matriz Q:
				AR->AssignMatrixesValues();

				// Deleta todos os elementos da lista e adiciona um novo
				AR->DeleteVector(AR->vetorHeuristica);
				AR->AddElementToVector(AR->vetorHeuristica);
			}

			unsigned long RandActSeed = ( AgentNumber + Cycle + WM->getAgentGlobalPosition().getMagnitude()*100
					+ WM->getAgentSpeed()*10000 + WM->getGlobalPosition( OBJECT_BALL ).getMagnitude()*100
					+ WM->getGlobalPosition( WM->getClosestRelativeInSet( OBJECT_SET_PLAYERS ) ).getMagnitude()*100 );
			if( WM->isBallKickable() )
			{
				if( AR->RandomAction( RandActSeed ) )
				{
					srand( RandActSeed + rand()%Cycle );
					AR->ActionTaken = rand()%acoes_bola + 1;
					soc = lookActions( AR->ActionTaken );
				}
				else
				{
					AR->ActionTaken = AR->GetBestAction( indiceCMAC, AR->prevIndex );
					soc = lookActions( AR->ActionTaken );
				}
				ACT->putCommandInQueue( soc );
				//ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
			else
			{
				if( WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL ) == WM->getAgentObjectType()
						&& !WM->isDeadBallThem()
						&& WM->getClosestInSetTo( OBJECT_SET_TEAMMATES , OBJECT_BALL ) == WM->getAgentObjectType() ) // intercept possibility
				{
					Log.log( 100, "I am fastest to ball; can get there in %d cycles", iTmp );
					soc = intercept( false );                      // intercept the ball

					if( soc.commandType == CMD_DASH &&             // if stamina low
							WM->getAgentStamina().getStamina() <
							SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
					{
						soc.dPower = 30.0 * WM->getAgentStamina().getRecovery(); // dash slow
						ACT->putCommandInQueue( soc );
						ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
					}
					else                                           // if stamina high
					{
						ACT->putCommandInQueue( soc );               // dash as intended
						ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
					}
					AR->ActionTaken = 97;
				}
				else
				{
					if( AR->RandomAction( RandActSeed ) )
					{
						srand( RandActSeed + rand()%Cycle );
						AR->ActionTaken = rand()%(num_acoes - acoes_bola) + acoes_bola + 1;
						soc = lookActions( AR->ActionTaken , false );
					}
					else
					{
						AR->ActionTaken = AR->GetBestAction( indiceCMAC, AR->prevIndex, false );
						soc = lookActions( AR->ActionTaken , false );
					}
				}
			}

			AR->checkState = true;
			AR->VetorRelatorio[AR->ActionTaken]++;
			AR->VetorRelatorio[0]++;

			AR->vetorHeuristica.back().acao = AR->ActionTaken;
			for( int i = 0; i < lut_m; i++  )
				AR->vetorHeuristica.back().indexCMAC_acao[i] = indiceCMAC[i];
		}
		else if( WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp )
				== WM->getAgentObjectType()  && !WM->isDeadBallThem() && !WM->isBallKickable())
		{                                                // if fastest to ball
			Log.log( 100, "I am fastest to ball; can get there in %d cycles", iTmp );
			soc = intercept( false );                      // intercept the ball

			if( soc.commandType == CMD_DASH &&             // if stamina low
					WM->getAgentStamina().getStamina() <
					SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
			{
				soc.dPower = 30.0 * WM->getAgentStamina().getRecovery(); // dash slow
				ACT->putCommandInQueue( soc );
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
			else                                           // if stamina high
			{
				ACT->putCommandInQueue( soc );               // dash as intended
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
			AR->checkState = false;
		}
		else if( WM->isBallKickable() )
		{
			soc = lookActions( 5 );
			ACT->putCommandInQueue( soc );
			AR->checkState = false;
		}
		else
		{
			soc = lookActions(97, false);
			ACT->putCommandInQueue( soc );
			AR->checkState = false;
		}

		AR->Num_Itera++; // Utilizado na regra de transição de estado (Epsilon)
		AR->prevPossesion = Possesion;
	}
	else
	{
		AR->checkState = false;
	}

	AR->Ciclo_ant = AR->Ciclo_atual;
	return soc;
}

/*!This method is a simple goalie based on the goalie of the simple Team of
   FC Portugal. It defines a rectangle in its penalty area and moves to the
   position on this rectangle where the ball intersects if you make a line
   between the ball position and the center of the goal. If the ball can
   be intercepted in the own penalty area the ball is intercepted and catched.
 */
SoccerCommand Player::deMeer5_goalie(  )
{
	int i;

	SoccerCommand soc;
	VecPosition   posAgent = WM->getAgentGlobalPosition();
	AngDeg        angBody  = WM->getAgentGlobalBodyAngle();

	// define the top and bottom position of a rectangle in which keeper moves
	static const VecPosition posLeftTop( -PITCH_LENGTH/2.0 +
			0.7*PENALTY_AREA_LENGTH, -PENALTY_AREA_WIDTH/4.0 );
	static const VecPosition posRightTop( -PITCH_LENGTH/2.0 +
			0.7*PENALTY_AREA_LENGTH, +PENALTY_AREA_WIDTH/4.0 );

	// define the borders of this rectangle using the two points.
	static Line  lineFront = Line::makeLineFromTwoPoints(posLeftTop,posRightTop);
	static Line  lineLeft  = Line::makeLineFromTwoPoints(
			VecPosition( -50.0, posLeftTop.getY()), posLeftTop );
	static Line  lineRight = Line::makeLineFromTwoPoints(
			VecPosition( -50.0, posRightTop.getY()),posRightTop );


	if( WM->isBeforeKickOff( ) )
	{
		if( formations->getFormation() != FT_INITIAL || // not in kickoff formation
				posAgent.getDistanceTo( WM->getStrategicPosition() ) > 2.0 )
		{
			formations->setFormation( FT_INITIAL );       // go to kick_off formation
			ACT->putCommandInQueue( soc=teleportToPos(WM->getStrategicPosition()) );
		}
		else                                            // else turn to center
		{
			ACT->putCommandInQueue( soc = turnBodyToPoint( VecPosition( 0, 0 ), 0 ));
			ACT->putCommandInQueue( alignNeckWithBody( ) );
		}
		return soc;
	}

	if( WM->getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )
	{                                                // confidence ball too  low
		ACT->putCommandInQueue( searchBall() );        // search ball
		ACT->putCommandInQueue( alignNeckWithBody( ) );
	}
	else if( WM->getPlayMode() == PM_PLAY_ON || WM->isFreeKickThem() ||
			WM->isCornerKickThem() )
	{
		if( WM->isBallCatchable() )
		{
			ACT->putCommandInQueue( soc = catchBall() );
			ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
		}
		else if( WM->isBallKickable() )
		{
			double safeDist, Dist1, Dist2;
			ObjectT recPlayer1 = WM->getClosestInSetTo( OBJECT_SET_TEAMMATES_NO_GOALIE, OBJECT_TEAMMATE_GOALIE );
			ObjectT opp1 =  WM->getClosestInSetTo( OBJECT_SET_OPPONENTS, recPlayer1, &Dist1 );
			ObjectT recPlayer2 = WM->getSecondClosestInSetTo( OBJECT_SET_TEAMMATES_NO_GOALIE, OBJECT_TEAMMATE_GOALIE );
			ObjectT opp2 =  WM->getClosestInSetTo( OBJECT_SET_OPPONENTS, recPlayer2, &Dist2 );
			ObjectT objFinal;

			if( Dist1 > Dist2 )
				objFinal = recPlayer1;
			else
				objFinal = recPlayer2;
			soc = kickTo( WM->getGlobalPositionLastSee( objFinal ) , PS->getPassEndSpeed() );
			ACT->putCommandInQueue( turnBodyToObject(objFinal) );
			ACT->putCommandInQueue( soc );
			ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
		}
		else if( WM->isInOwnPenaltyArea( getInterceptionPointBall( &i, true ) ) &&
				WM->getFastestInSetTo( OBJECT_SET_PLAYERS, OBJECT_BALL, &i ) ==
						WM->getAgentObjectType() )
		{
			ACT->putCommandInQueue( soc = intercept( true ) );
			ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
		}
		else
		{
			// make line between own goal and the ball
			VecPosition posMyGoal = ( WM->getSide() == SIDE_LEFT )
            		 ? SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_L, SIDE_LEFT )
			: SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_R, SIDE_RIGHT);
			Line lineBall = Line::makeLineFromTwoPoints( WM->getBallPos(),posMyGoal);

			// determine where your front line intersects with the line from ball
			VecPosition posIntersect = lineFront.getIntersection( lineBall );

			// outside rectangle, use line at side to get intersection
			if (posIntersect.isRightOf( posRightTop ) )
				posIntersect = lineRight.getIntersection( lineBall );
			else if (posIntersect.isLeftOf( posLeftTop )  )
				posIntersect = lineLeft.getIntersection( lineBall );

			if( posIntersect.getX() < -49.0 )
				posIntersect.setX( -49.0 );

			// and move to this position
			if( posIntersect.getDistanceTo( WM->getAgentGlobalPosition() ) > 0.5 )
			{
				soc = moveToPos( posIntersect, PS->getPlayerWhenToTurnAngle() );
				ACT->putCommandInQueue( soc );
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
			else
			{
				ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
		}
	}
	else if( WM->isFreeKickUs() == true || WM->isGoalKickUs() == true )
	{
		if( WM->isBallKickable() )
		{
			if( WM->getTimeSinceLastCatch() == 25 && WM->isFreeKickUs() )
			{
				// move to position with lesser opponents.
				if( WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
						Circle(posRightTop, 15.0 )) <
						WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
								Circle(posLeftTop,  15.0 )) )
					soc.makeCommand( CMD_MOVE,posRightTop.getX(),posRightTop.getY(),0.0);
				else
					soc.makeCommand( CMD_MOVE,posLeftTop.getX(), posLeftTop.getY(), 0.0);
				ACT->putCommandInQueue( soc );
			}
			else if( WM->getTimeSinceLastCatch() > 28 )
			{
				soc = soc = clearBall(CLEAR_BALL_DEFENSIVE);
				ACT->putCommandInQueue( soc );
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
				//soc = clearBall(CLEAR_BALL_DEFENSIVE) ; //kickTo( VecPosition(0,posAgent.getY()*2.0), 2.0 );
			}
			else if( WM->getTimeSinceLastCatch() < 25 )
			{
				VecPosition posSide( 0.0, posAgent.getY() );
				if( fabs( (posSide - posAgent).getDirection() - angBody) > 10 )
				{
					soc = turnBodyToPoint( posSide );
					ACT->putCommandInQueue( soc );
				}
				ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
			}
		}
		else if( WM->isGoalKickUs()  )
		{
			ACT->putCommandInQueue( soc = intercept( true ) );
			ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
		}
		else
			ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	}
	else
	{
		ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
		ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	}
	return soc;
}
