
#ifndef _BASICPLAYER_
#define _BASICPLAYER_

#include "ActHandler.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <float.h> // for DBL_MAX
#include <vector> // for  Heuristics

/////////////////////////////// Constantes /////////////////////////////////////
// Não mudar o nome dessas variaveis!

#define num_estados 11466178560
#define num_chaves 17
#define num_acoes 99
#define acoes_bola 93 //Numero de acoes COM bola - As acoes com bola devem vir primeiro em lookActions (metodo)
const int vetorChaves[num_chaves] = {16, 5, 4, 4, 4, 4, 3, 3, 3, 4, 4, 4, 3, 3, 4, 3, 3};
#define AlfaQ 0.3
#define GamaQ 0.9

// LookUp Table (CMAC):
#define lut_m 8 			// Numero de linhas
#define lut_resolution 16	// Resolucao (colunas) - Maior valor da chave de estado
#define lut_overlap 4
#define CMAC_Mult 10000
const unsigned long long num_linesQ = ((CMAC_Mult*lut_m) + lut_m);

//////////////////////////// Constantes (FIM) /////////////////////////////////

enum CloseKickT {
	CK_LONG,
	CK_MED,
	CK_SHO
} ;

enum MoveDistT {
	MD_LONG,
	MD_MED,
	MD_SHO,
	MD_VSHO
} ;

/******************************************************************************/
/******************** CLASS APRENDIZAGEM_REFORCO ******************************/
/******************************************************************************/
/* Esta classe possui as principais funções e variáveis para executar o algoritmo
 * Q_Learning.
 * */

class Aprendizagem_Reforco
{
protected:
	WorldModel *WM ;

public:

	int Ciclo_ant;      						// Armazena o numero do ciclo anterior
	int Ciclo_atual;    						// Armazena o numero do ciclo atual
	int SaldoGol_ant;   						// Armazena o saldo de gols anterior
	int SaldoGol_atual; 						// Armazena o saldo de gols atual
	int ActionTaken;							// Armazena a acao tomada para calcular o Q
	int PosActionTaken;
	unsigned long long prevIndex;				// Armazena o indece de estado CMAC em que a acao foi tomada
	unsigned long long posPrevIndex;
	bool prevPossesion;							// Armazena se a gente estava com a bola no ciclo anterior, usado
												// 		para verificar se perdeu/recuperou a bola.
	bool changePossesion;						// Verdadeiro se houve mudanca de dominio de bola
	bool checkState;							// True se foi executada uma ação que vai ser considerada na AR
	bool posCheckState;
	double Epsilon;								// Utilizado na regra de transição de estado
	double porcentagemNaoNulosNaMatrizQ	;		// Relatorio
	double porcentagemNaoNulosNaMatrizH;		// Relatorio
	unsigned long long indiceCMAC_ant[lut_m];	// Indeces dos estado (CMAC)
	unsigned long long posindiceCMAC_ant[lut_m];// Indeces dos estado (CMAC) - Para calcular valor Q de acao com bola flutuante
	double MatrizQ[num_linesQ][num_acoes];		// Matriz Q
	double MatrizH[num_linesQ][num_acoes];		// Matriz H

	unsigned long long Num_Jogos;				// Numero de jogos executados no total
	unsigned long long Num_Itera;				// Numero de iterações executadas no total

	// Vetor que armazena o numero de vezes que cada acao foi tomada, e o total de acoes tomadas, durante a partida, sendo:
	// Indice: 0 - total de ações | num_acao + 1 - gols feitos | num_acao + 2 - gols sofridos. Demais indeces representa
	// o numero de vezes que uma ação (de determinado indice) foi executada.
	// Dois ultimos indeces reference aos valores de epsilon e exploracao da matrizQ/H
	double VetorRelatorio[num_acoes+6];

	/*
	 * ####################################################
	 * #############  METODOS PARA AR_Base.cpp ############
	 * ####################################################
	 */

	Aprendizagem_Reforco                    (                            	   	);
	~Aprendizagem_Reforco                   (                             	  	);
	
	struct HeuristicVector
	{
		unsigned long long indexCMAC_acao[lut_m]; // verificar o antcmacindex utilizado em assignQvalue em ar_base!!!11
		unsigned long long indexCMAC_verificacao[lut_m];
		int acao;
		bool bolaViajando;
		double reforco;
	};

	std::vector<HeuristicVector> vetorHeuristica;
	
	unsigned long long GetState			( int vetorEstado[num_chaves]		); // - Nao utilizado (para q-learning puro)

	void LookUpTable						( int table[lut_m][lut_resolution]	);
	unsigned long long ConcatValues		( int *VectorValues, int num		);
	bool GetIndexCMAC						( int vetorEstado[num_chaves],
											  unsigned long long* CMACindex	);
	int GetBestAction						( unsigned long long* IndexesCMAC,
											  unsigned long long &IndexAct,
											  bool kickable = true				);
	void AssignMatrixesValues			(									);
	void AssignValue						( unsigned long long* IndexesCMAC,
											  unsigned long long* IndexAct,
											  int numAct, double Reforco,
											  double Matriz[num_linesQ][num_acoes] );
	bool RandomAction						( int seed							);
	void UpdateEpsilon					(									);

	void AddElementToVector				( vector<HeuristicVector> &VetorParam );
	void DeleteVector						( vector<HeuristicVector> &VetorParam );
	void DeleteLastElementInVector		( vector<HeuristicVector> &vetorParam );
	void loadMatrixQ						( int playerNum						);
	void saveMatrixQ						( int playerNum						);
	void loadMatrixH						( int playerNum						);
	void saveMatrixH						( int playerNum						);
	void loadInfo							( int playerNum						);
	void saveInfo							( int playerNum						);
	void loadReport						( int playerNum						);
	void saveReport						( int playerNum						);
};

extern Logger Log; /*!< This is a reference to Logger to write log info to*/

/*! This class defines the skills that can be used by an agent. No
    functionality is available that chooses when to execute which skill, this
    is done in the Player class. The WorldModel is used to determine the way
    in which the skills are performed. */
class BasicPlayer
{
protected:
  ActHandler      *ACT; /*!< ActHandler to which commands can be sent        */
  WorldModel      *WM;  /*!< WorldModel that contains information of world   */
  Aprendizagem_Reforco *AR;
  ServerSettings  *SS;  /*!< All parameters used by the server               */
  PlayerSettings  *PS;  /*!< All parameters used for the player              */

  double dPlayerVersion;				/*the version of the player. Some new functions changes their
	 functionality according to the player version. Example: Tackle*/
  ////////////////////////// LOW-LEVEL SKILLS /////////////////////////////////

  SoccerCommand   alignNeckWithBody       (                                  );
  SoccerCommand   turnBodyToPoint         ( VecPosition   pos,
                                            int           iPos = 1           );
  SoccerCommand   turnBackToPoint         ( VecPosition   pos,
                                            int           iPos = 1           );
   SoccerCommand turnSideToPoint( VecPosition pos, AngDeg angle, int iCycles  =1);
  SoccerCommand   turnNeckToPoint         ( VecPosition   pos,
                                            SoccerCommand com                );
  SoccerCommand   searchBall              (                                  );
  SoccerCommand   dashToPoint             ( VecPosition   pos,
                                            int           iCycles = 1,        
										   bool 		sideDash = false);
  SoccerCommand   freezeBall              (                                  );
  SoccerCommand   kickBallCloseToBody     ( AngDeg        ang,
					    double        dKickRatio = 0.16  );
  SoccerCommand   accelerateBallToVelocity( VecPosition   vel                );
  SoccerCommand   catchBall               (                                  );
  SoccerCommand   communicate             ( char          *str               );
  SoccerCommand   teleportToPos           ( VecPosition   pos                );
  SoccerCommand   listenTo                ( ObjectT       obj                );
  SoccerCommand   tackle                  ( double angle, bool foul              );

  ////////////////////////// INTERMEDIATE SKILLS //////////////////////////////

  SoccerCommand   turnBodyToObject        ( ObjectT       o                  );
  SoccerCommand   turnNeckToObject        ( ObjectT       o,
                                            SoccerCommand com                );
  SoccerCommand   directTowards           ( VecPosition   posTo,
                                            AngDeg        angWhenToTurn,
                                            VecPosition   *pos = NULL,
                                            VecPosition   *vel = NULL,
                                            AngDeg        *angBody  = NULL   );
  SoccerCommand   moveToPos               ( VecPosition   posTo,
                                            AngDeg        angWhenToTurn,
                                            double        dDistDashBack = 0.0,
                                            bool          bMoveBack = false,
                                            int           iCycles = 1        );
  SoccerCommand moveToPos2( VecPosition posTo, AngDeg angWhenToTurn,
                            double angle);
  SoccerCommand   collideWithBall         (                                  );
  SoccerCommand   interceptClose          (                                  );
  SoccerCommand   interceptCloseGoalie    (                                  );
  SoccerCommand   kickTo                  ( VecPosition   posTarget,
                                            double        dEndSpeed          );
  SoccerCommand   turnWithBallTo          ( AngDeg        ang,
                                            AngDeg        angKickThr,
                                            double        dFreezeThr         );
  SoccerCommand   moveToPosAlongLine      ( VecPosition   pos,
                                            AngDeg        ang,
                                            double        dDistThr,
                                            int           iSign,
                                            AngDeg        angThr,
                                            AngDeg        angCorr            );


  ////////////////////////// HIGH-LEVEL SKILLS ////////////////////////////////

  SoccerCommand   intercept               ( bool          isGoalie           );
  SoccerCommand   dribble                 ( AngDeg        ang,
                                            DribbleT      d                  );
  SoccerCommand   directPass              ( VecPosition   pos,
                                            PassT         passType           );
  SoccerCommand   leadingPass             ( ObjectT       o,
                                            double        dDist,
                                            DirectionT    dir = DIR_NORTH    );
  SoccerCommand   throughPass             ( ObjectT       o,
                                            VecPosition   posEnd,
                                            AngDeg        *angMax = NULL     );
  SoccerCommand   outplayOpponent         ( ObjectT       o,
                                            VecPosition   pos,
                                            VecPosition   *posTo = NULL      );
  SoccerCommand   clearBall               ( ClearBallT    type,
                                            SideT         s = SIDE_ILLEGAL,
                                            AngDeg        *angMax = NULL     );
  SoccerCommand   mark                    ( ObjectT       o,
                                            double        dDist,
                                            MarkT         mark               );
  SoccerCommand   defendGoalLine          ( double        dDist              );
  SoccerCommand   interceptScoringAttempt (                                  );
  SoccerCommand   holdBall                (                                  );

  ////////////////////////// UTILITY METHODS //////////////////////////////////

  VecPosition     getThroughPassShootingPoint( ObjectT       objTeam,
                                               VecPosition   posEnd,
					       AngDeg        *angMax         );
  VecPosition     getInterceptionPointBall(    int           *iCyclesBall,
                                               bool          isGoalie        );
  VecPosition     getActiveInterceptionPointBall
                                          ( int           *iCyclesBall,
                                            bool          isGoalie           );
  VecPosition     getDribblePoint         ( DribbleT      dribble,
                                            double        *dDist             );
  VecPosition     getShootPositionOnLine  ( VecPosition   p1,
                                            VecPosition   p2,
                                            AngDeg        *angLargest = NULL );
  double          getEndSpeedForPass      ( ObjectT       o,
                                            VecPosition   posPass            );
  VecPosition     getMarkingPosition      ( ObjectT       o,
                                            double        dDist,
                                            MarkT         mark               );
  double getPlayerVersion();
  SoccerCommand syncSee();


  /*
   * ####################################################
   * ###########  METODOS PARA AR_Actions.cpp ###########
   * ####################################################
   */

  SoccerCommand lookActions					( int			 numAction,
		  	  	  	  	  	  	  	  	  	  bool    withBall = true 			);
  // Passes velocidade definida:
  SoccerCommand STpassFAST    		    	(                                   );
  SoccerCommand STpassMED    				(                                   );
  SoccerCommand STpassSLOW    			    (                                   );
  SoccerCommand NDpassFAST    			    (                                   );
  SoccerCommand NDpassMED    				(                                   );
  SoccerCommand NDpassSLOW    		    	(                                   );
  SoccerCommand RDpassFAST    		 	    (                                   );
  SoccerCommand RDpassMED    				(                                   );
  SoccerCommand RDpassSLOW    		    	(                                   );
  SoccerCommand THpassFAST    			    (                                   );
  SoccerCommand THpassMED    				(                                   );
  SoccerCommand THpassSLOW    			    (                                   );
  // Passe velocidade maxima
  SoccerCommand STpassMAX    		    	(                                   );
  SoccerCommand NDpassMAX    				(                                   );
  SoccerCommand RDpassMAX    		    	(                                   );
  SoccerCommand THpassMAX    		    	(                                   );
  // Lançamentos normais:
  SoccerCommand STchipLONG  	  		    (                                   );
  SoccerCommand STchipMED	    			(                                   );
  SoccerCommand STchipSHO	    			(                                   );
  SoccerCommand STchipVSHO    		    	(                                   );
  SoccerCommand NDchipLONG    			    (                                   );
  SoccerCommand NDchipMED	    	    	(                                   );
  SoccerCommand NDchipSHO	    	    	(                                   );
  SoccerCommand NDchipVSHO    		  		(                                   );
  SoccerCommand RDchipLONG    		 	  	(                                   );
  SoccerCommand RDchipMED	    	    	(                                   );
  SoccerCommand RDchipSHO	    	    	(                                   );
  SoccerCommand RDchipVSHO    		    	(                                   );
  SoccerCommand THchipLONG    		    	(                                   );
  SoccerCommand THchipMED	    	    	(                                   );
  SoccerCommand THchipSHO	    	  	    (                                   );
  SoccerCommand THchipVSHO    		   		(                                   );
  // Lançamentos entre defensores:
  SoccerCommand STthroughLONG  		    	(                                   );
  SoccerCommand STthroughMED  		 	 	(                                   );
  SoccerCommand STthroughSHO  			    (                                   );
  SoccerCommand STthroughVSHO  			    (                                   );
  SoccerCommand NDthroughLONG  		    	(                                   );
  SoccerCommand NDthroughMED  			    (                                   );
  SoccerCommand NDthroughSHO  			    (                                   );
  SoccerCommand NDthroughVSHO  			    (                                   );
  SoccerCommand RDthroughLONG  			    (                                   );
  SoccerCommand RDthroughMED  			    (                                   );
  SoccerCommand RDthroughSHO  			    (                                   );
  SoccerCommand RDthroughVSHO  		    	(                                   );
  SoccerCommand THthroughLONG  		    	(                                   );
  SoccerCommand THthroughMED  			    (                                   );
  SoccerCommand THthroughSHO  			    (                                   );
  SoccerCommand THthroughVSHO	  		    (                                   );
  // Chutes a gol:
  SoccerCommand GoalKickCenter 		    	(                                   );
  SoccerCommand GoalKickRight100  	    	(                                   );
  SoccerCommand GoalKickRight75  	   		(                                   );
  SoccerCommand GoalKickRight50  	  	 	(                                   );
  SoccerCommand GoalKickRight25  	   		(                                   );
  SoccerCommand GoalKickLeft25  	   		(                                   );
  SoccerCommand GoalKickLeft50  	  	 	(                                   );
  SoccerCommand GoalKickLeft75  	  	 	(                                   );
  SoccerCommand GoalKickLeft100  	  	  	(                                   );
  // Outras acoes com a bola
  SoccerCommand OutplayStClosestOppLong		(									);
  SoccerCommand OutplayNdClosestOppLong		(									);
  SoccerCommand OutplayStClosestOppMed		(									);
  SoccerCommand OutplayNdClosestOppMed		(									);
  SoccerCommand OutplayStClosestOppSho		(									);
  SoccerCommand OutplayNdClosestOppSho		(									);
  SoccerCommand CloseKickLong				(									);
  SoccerCommand CloseKickMed				(									);
  SoccerCommand CloseKickSho				(									);
  SoccerCommand DribbleSlowToP15 			(									);
  SoccerCommand DribbleSlowToP30 			(									);
  SoccerCommand DribbleSlowToP45 			(									);
  SoccerCommand DribbleSlowToP60 			(									);
  SoccerCommand DribbleSlowToP75 			(									);
  SoccerCommand DribbleSlowToP90 			(									);
  SoccerCommand DribbleSlowToM15 			(									);
  SoccerCommand DribbleSlowToM30 			(									);
  SoccerCommand DribbleSlowToM45 			(									);
  SoccerCommand DribbleSlowToM60 			(									);
  SoccerCommand DribbleSlowToM75 			(									);
  SoccerCommand DribbleSlowToM90 			(									);
  SoccerCommand DribbleFastToP15 			(									);
  SoccerCommand DribbleFastToP30 			(									);
  SoccerCommand DribbleFastToP45 			(									);
  SoccerCommand DribbleFastToP60 			(									);
  SoccerCommand DribbleFastToP75 			(									);
  SoccerCommand DribbleFastToP90 			(									);
  SoccerCommand DribbleFastToM15 			(									);
  SoccerCommand DribbleFastToM30 			(									);
  SoccerCommand DribbleFastToM45 			(									);
  SoccerCommand DribbleFastToM60 			(									);
  SoccerCommand DribbleFastToM75 			(									);
  SoccerCommand DribbleFastToM90 			(									);
  SoccerCommand CloseKick               	( CloseKickT    ckType              );
  SoccerCommand DribbleToAng            	( AngDeg        ang,
		  	  	  	  	  	  	  	  	  	  DribbleT      d               	);
  SoccerCommand MarkOppAndBall				(									);
  SoccerCommand InterceptBall				(									);
  // Acoes sem bola:
  SoccerCommand AR_moveToPos				( VecPosition pos				 	);
  VecPosition MoveToObjLine					( ObjectT obj, MoveDistT distType	);
  VecPosition MoveToObjGoalLine				( ObjectT obj, MoveDistT distType	);
  void MoveToSector							( MoveDistT distType				);


  /*
   * ####################################################
   * ###########  METODOS PARA AR_States.cpp ###########
   * ####################################################
   */

  int lookStates						 ( int numState							);
  int PitchDiscretization  				 ( VecPosition PlayerPostion,
		  	  	  	  	  	  	  	  	  	  double *dX=NULL, double *dY=NULL,
		  	  	  	  	  	  	  	  	  	  int *numI=NULL,
		  	  	  	  	  	  	  	  	  	  int *numJ=NULL					);
  int DistToBallDiscretization			 ( VecPosition PlayerPostion,
		  	  	  	  	  	  	  	  	  	  ObjectT obj = OBJECT_BALL  		);
  int DistToPlayer4Discretization		 ( VecPosition PlayerPostion,
		  	  	  	  	  	  	  	  	  	  ObjectT obj 				 		);
  int DistToPlayer3Discretization		 ( VecPosition PlayerPostion,
		  	  	  	  	  	  	  	  	  	  ObjectT obj 				  		);
  int DirAgent4Discretization			 ( VecPosition PlayerPostion = 0	 	);
  int DirAgent4Discretization			 ( ObjectT obj = OBJECT_ILLEGAL			);
  int DirAgent3Discretization			 ( ObjectT obj = OBJECT_ILLEGAL			);

  /*avançado
  int PitchDiscretization				( VecPosition 	PlayerPostion 		);

  int DistToBallDiscretization  		( VecPosition 	PlayerPostion,
  		  	  	  	  	  	  	  	  	  ObjectT       obj = OBJECT_BALL);

  int DistToPlayer4Discretization  		( VecPosition 	PlayerPostion,
		  	  	  	  	  	  	  	  	  ObjectT       obj = OBJECT_ILLEGAL);

  int DistToPlayer3Discretization  		( VecPosition 	PlayerPostion,
		  	  	  	  	  	  	  	  	  ObjectT       obj = OBJECT_ILLEGAL);

  int DirObj4Discretization  			( VecPosition 	PlayerPostion,
		  	  	  	  	  	  	  	  	  ObjectT       obj = OBJECT_ILLEGAL);

  int AngleDiscretization				( bool 			isAgent = true,
		  	  	  	  	  	  	  	  	  ObjectT   	o = OBJECT_ILLEGAL	);
  int VelocDiscretization				( bool 			isAgent = true,
  		  	  	  	  	  	  	  	  	  ObjectT   	o = OBJECT_ILLEGAL	);
  int StaminaDiscretization				( 									);
  int ObjDiscretization					( ObjectT   	o					);
  int PlayModeDiscretization 			( PlayModeT     mode				);*/


  /*
   * ####################################################
   * #########  METODOS PARA AR_Auxiliaries.cpp #########
   * ####################################################
   */

  ObjectT getClosestTeammateToAgent       ( double       *dDist = NULL        );
  ObjectT getClosestOpponentToAgent       ( double       *dDist = NULL        );
  ObjectT getSecondClosestTeamToAgent     ( double       *dDist = NULL        );
  ObjectT getSecondClosestOpponentToAgent ( double       *dDist = NULL        );
  ObjectT getThirdClosestTeamToAgent      ( double       *dDist = NULL        );
  ObjectT getThirdClosestOpponentToAgent  ( double       *dDist = NULL        );
  ObjectT getFourthClosestTeamToAgent     ( double       *dDist = NULL        );
  ObjectT getThirdClosestInSetTo          ( ObjectSetT     objectSet,
		  	  	  	  	  	  	  	  	  	  ObjectT        o,
		  	  	  	  	  	  	  	  	  	  double         *dDist = NULL,
		  	  	  	  	  	  	  	  	  	  double         dConfThr = -1.0  );
  ObjectT getFourthClosestInSetTo         ( ObjectSetT     objectSet,
		  	  	  	  	  	  	  	  	  	  ObjectT        o,
		  	  	  	  	  	  	  	  	  	  double         *dDist = NULL,
		  	  	  	  	  	  	  	  	  	  double         dConfThr = -1.0  );
  bool AR_isBallInOurPossesion();
  bool areWeLastTeamBallHolder();


  bool isBallFloating					  ();
  /// Other

  bool CheckIfNeedsNewElementInList (bool Possesion);
  double GetReinforcement				  (bool ChangePossesion,
		  bool SaldoGol, int Action		  );
} ;

#endif
