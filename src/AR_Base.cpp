#include "BasicPlayer.h"

Aprendizagem_Reforco::Aprendizagem_Reforco()
{
	SaldoGol_ant = 0 ;
	SaldoGol_atual = 0 ;
	Ciclo_ant = 0;
	Ciclo_atual = 0;
	Epsilon = 0;
	checkState = false;
	posCheckState = false;
	changePossesion = false;

	for( int i = 0; i<(num_acoes + 3); i++ )
	{
		VetorRelatorio[i] = 0;
	}
}

Aprendizagem_Reforco::~Aprendizagem_Reforco()
{
	DeleteVector(vetorHeuristica);
}

unsigned long long Aprendizagem_Reforco::GetState( int vetorEstado[num_chaves] )
{
	unsigned long long multiplicador[num_chaves];
	unsigned long long estado = 0;

	for( int i=0; i<num_chaves; i++ )
	{
		multiplicador[i] = 1;
		for( int j=i+1; j<num_chaves; j++ )
		{
			multiplicador[i] = multiplicador[i]*vetorChaves[j];
		}
	}

	for( int i=0; i<num_chaves; i++ )
		estado = estado + multiplicador[i]*vetorEstado[i];

	return estado;
}

void Aprendizagem_Reforco::LookUpTable( int table[lut_m][lut_resolution] )
{
	int overlapI_cont = lut_overlap;
	int overlapJ_cont = 1;
	int value = 1;

	for( int i = 0; i<lut_m; i++ )
	{
		for( int j = 0; j< lut_resolution; j++ )
		{
			table[i][j] = value;
			overlapJ_cont++;
			if( overlapJ_cont > lut_overlap || j == (lut_resolution-1) )
			{
				value++;
				overlapJ_cont = 1;
			}
		}
		overlapJ_cont = overlapI_cont;
		overlapI_cont -= 1;
		if( overlapI_cont == 0 )
			overlapI_cont = lut_overlap;
	}
}

unsigned long long Aprendizagem_Reforco::ConcatValues( int *VectorValues, int num )
{
	unsigned long long concated = VectorValues[0];
	unsigned long long multiplier;
	unsigned long long aux;

	for( int i = 1; i < num; i++)
	{
		multiplier = 1;
		aux = VectorValues[i];

		while(multiplier <= aux)
			multiplier *= 10;
		concated = multiplier * concated + aux;
	}
	return concated;
}

// here's where the magic happens (be careful)
bool Aprendizagem_Reforco::GetIndexCMAC( int vetorEstado[num_chaves], unsigned long long* CMACindex )
{
	int lut[lut_m][lut_resolution];
	unsigned long long ConcatedValues[lut_m];
	double HashCode[lut_m];

	LookUpTable(lut); // Initializing lookup table

	// Auxiliaries variables
	int temp; // for sorting

	// Remove duplicated values in array vetorEstado
	int aux_num = num_chaves;
	for( int i = 0; i<aux_num; i++ )
	{
		for( int j = i+1; j<aux_num; )
		{
			if( vetorEstado[j] == vetorEstado[i] )
			{
				for( int k = j; k<aux_num; k++ )
					vetorEstado[k] = vetorEstado[k+1];
				aux_num--;
			}
			else
				j++;
		}
	}

	// these variables need to be here, do NOT move'em (due aux_num)
	int newVetorEstado[aux_num];
	int valuesByLut[lut_m][aux_num];

	// Reinitializing array vetorEstados without repeated values
	for(int i=0;i<aux_num;i++)
	{
		newVetorEstado[i]=vetorEstado[i];
		if( newVetorEstado[i] < 0 )
			return false;
	}

	// Sorting array newVetorEstado in ascending order
	for( int i = 0; i<aux_num; i++ )
	{
		for( int j = i; j<aux_num; j++ )
		{
			if(newVetorEstado[i] > newVetorEstado[j])
			{
				temp=newVetorEstado[i];
				newVetorEstado[i]=newVetorEstado[j];
				newVetorEstado[j]=temp;
			}
		}
	}

	// Get the values from the lookup table using the array newVetorEstado
	for( int i = 0; i<aux_num; i++ )
	{
		for( int j = 0; j<lut_m; j++ )
		{
			valuesByLut[j][i] = lut[j][newVetorEstado[i]];
		}
	}

	// Concat the values of the line in the matrix obtained before
	// ConcatedValues[] are going to be our seeds for the hashcode
	int aux[aux_num];
	for( int i = 0; i<lut_m; i++ )
	{
		for( int j = 0; j < aux_num; j++)
		{
			aux[j] = valuesByLut[i][j];
		}
		ConcatedValues[i] = ConcatValues( aux, aux_num );
	}

	for( int i = 0; i<lut_m; i++ ) // Hash Coding
	{
		srand( ConcatedValues[i] ); // Defining the seed
		HashCode[i] = ((double) rand() / (RAND_MAX)); // pseudo-random value [0,1]
		CMACindex[i] = HashCode[i]*CMAC_Mult; // Final index for CMAC
	}

	/* for visualization purpose only
	cerr << "V_E: [";
	for(int i=0;i<aux_num;i++){
		if( i < aux_num-1 )
			cerr << newVetorEstado[i] << ",";
		else
			cerr << newVetorEstado [i] << "]";
	}

	cerr << endl << endl;
	for( int i = 0; i< lut_m ; i++ )
	{
		for( int j = 0; j<aux_num; j++ )
		{
			if( valuesByLut[i][j] < 10 )
				cerr << "0" << valuesByLut[i][j] << " ";
			else
				cerr << valuesByLut[i][j] << " ";
		}
		cerr << endl;
	}
	cerr << endl << endl;

	cerr << "CV: [";
	for(int i=0;i<lut_m;i++){
		if( i < lut_m-1 )
			cerr << ConcatedValues[i] << ",";
		else
			cerr << ConcatedValues [i] << "]\n\n";
	}

	cerr << "HashCode: [";
	for(int i=0;i<lut_m;i++){
		if( i < lut_m-1 )
			cerr << HashCode[i] << ",";
		else
			cerr << HashCode[i] << "]\n\n";
	}

	cerr << "Index: [";
	for(int i=0;i<lut_m;i++){
		if( i < lut_m-1 )
			cerr << CMACindex[i] << ",";
		else
			cerr << CMACindex[i] << "]\n\n";
	}*/
	return true;
}

int Aprendizagem_Reforco::GetBestAction( unsigned long long *IndexesCMAC, unsigned long long &IndexAct, bool kickable )
{
	// Variable that will receive the values from MatrizQ:
	double QValues[lut_m][num_acoes];
	double maxQ = -DBL_MAX;
	int numAction = -1;

	for( int i = 0; i<lut_m; i++ )
	{
		for( int j = (num_acoes - 1); j>=0; j-- ) // prioridade para as primeiras acoes
		{
			unsigned long long line = IndexesCMAC[i] + CMAC_Mult*i + i;
			if( MatrizQ[line][j] >= maxQ )
			{
				if( kickable && j < acoes_bola )
				{
					maxQ = QValues[i][j];
					numAction = j;
					IndexAct = line;
				}
				else if( !kickable && j >= acoes_bola)
				{
					maxQ = QValues[i][j];
					numAction = j;
					IndexAct = line;
				}
			}
		}
	}
	return (numAction + 1); //Verificacao comeca em 0, mas lookActions comeca em 1
}

void Aprendizagem_Reforco::AssignMatrixesValues()
{
	if(vetorHeuristica.size() > 0)
	{
		// ultima ação está associada a matriz Q (Q-learning simples)
		// Demais ações vão para a matriz H
		int i = vetorHeuristica.size() - 1;
		AssignValue( vetorHeuristica[i].indexCMAC_verificacao,
				vetorHeuristica[i].indexCMAC_acao, vetorHeuristica[i].acao,
				vetorHeuristica[i].reforco, MatrizQ);

		for( i = vetorHeuristica.size() - 2; i >= 0; i--)
		{
			AssignValue( vetorHeuristica[i].indexCMAC_verificacao,
					vetorHeuristica[i].indexCMAC_acao, vetorHeuristica[i].acao,
					vetorHeuristica[i].reforco, MatrizH);
		}
	}
}

void Aprendizagem_Reforco::AssignValue( unsigned long long* IndexesCMAC, unsigned long long* IndexAct,
		int numAct, double Reforco, double Matriz[num_linesQ][num_acoes] )
{
	double maxQ = -DBL_MAX;
	numAct--; // LookAction vai de [1,99], matriz vai de [0,98]

	for( int j = 0; j < lut_m; j++ )
	{
		unsigned long long line = IndexesCMAC[j] + CMAC_Mult*j + j;
		if( MatrizH[line][numAct] > maxQ )
			maxQ = MatrizH[line][numAct];
		if( MatrizQ[line][numAct] > maxQ )
			maxQ = MatrizQ[line][numAct];
	}

	// Atribuindo valor somente para IndexAct - Nao parece a melhor coisa rsrsrsrs
	// MatrizQ[IndexAct][numAct] = MatrizQ[IndexAct][numAct] + AlfaQ*( Reforco + GamaQ*maxQ - MatrizQ[IndexAct][numAct] );
	unsigned long long index;
	for( int i = 0; i < lut_m; i++ )
	{
		index = IndexAct[i];
		Matriz[index][numAct] = Matriz[index][numAct] + AlfaQ*( Reforco + GamaQ*maxQ - Matriz[index][numAct] );
	}
}

bool BasicPlayer::CheckIfNeedsNewElementInList( bool Possesion )
{
	// Verifica se levou ou fez um gol
	if(AR->SaldoGol_ant > AR->SaldoGol_atual)
	{
		if( AR->vetorHeuristica.size() > 0 )
			AR->vetorHeuristica.back().reforco = -1000.0;
		return false;
	} else if(AR->SaldoGol_ant < AR->SaldoGol_atual){
		if( AR->vetorHeuristica.size() > 0 )
			AR->vetorHeuristica.back().reforco = 1000.0;
		return false;
	}

	// Verifica se houve mudança na posse de bola
	if( Possesion )
	{
		if( AR->changePossesion == true )
		{
			if( AR->vetorHeuristica.size() > 0 )
				AR->vetorHeuristica.back().reforco = 500.0;
			return false;
		} else{
			if( AR->vetorHeuristica.size() > 0 )
				AR->vetorHeuristica.back().reforco = 0.5;
			return true;
		}
	} else{
		if( AR->changePossesion == true )
		{
			if( AR->vetorHeuristica.size() > 0 )
				AR->vetorHeuristica.back().reforco = -500.0;
			return false;
		} else{
			if( AR->vetorHeuristica.size() > 0 )
				AR->vetorHeuristica.back().reforco = -0.5;
			return true;
		}
	}

	return true;
}

double BasicPlayer::GetReinforcement( bool ChangePossesion, bool SaldoGol, int Action )
{
	double reinforcement = 0.0;
	if( SaldoGol ) // Gol feito
		return 50.0;
	else if( ChangePossesion && Action != 97 )
	{
		if( !WM->isBallInOurPossesion() )
			return -5.0;
		else if( !WM->isDeadBallUs() )
			return 4.0;
	}

	if( Action <= 48 )
	{
		if( WM->isBallInOurPossesion() )
			return 10.0;
	}
	else if( Action == 97 )
	{
		if( WM->getClosestInSetTo( OBJECT_SET_TEAMMATES , OBJECT_BALL ) == WM->getAgentObjectType() ||
				WM->getSecondClosestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL ) == WM->getAgentObjectType() )
			reinforcement = reinforcement - 5.0;
		if( WM->isGoalKickUs() && WM->getAgentGlobalPosition().getX() < -36.00 &&
				WM->getAgentGlobalPosition().getY() < 20.00 && WM->getAgentGlobalPosition().getY() > -20.00)
		{
			reinforcement = reinforcement - 30.0;
		}
	}
	else if( Action > 93 && WM->getAgentGlobalPosition().getDistanceTo( WM->getStrategicPosition() ) > 8.0 )
		return -3.0;

	return reinforcement;
}

bool Aprendizagem_Reforco::RandomAction( int seed )
{
	// The seed needs to be very random:
	srand( seed );
	double prob = ((double) rand() / (RAND_MAX)); // pseudo-random value [0,1]
	UpdateEpsilon();

	if( prob <= Epsilon )
	{
		return true;
	}
	return false;
}

void Aprendizagem_Reforco::UpdateEpsilon()
{
	Epsilon = 1.0/(exp( Num_Itera/pow10( 7 ) ));
}

void Aprendizagem_Reforco::AddElementToVector( vector<HeuristicVector> &VetorParam )
{
	HeuristicVector novoElemento;
	VetorParam.push_back(novoElemento);
}

void Aprendizagem_Reforco::DeleteVector( vector<HeuristicVector> &VetorParam )
{
	if( VetorParam.size() > 0 )
		VetorParam.clear();
}

void Aprendizagem_Reforco::DeleteLastElementInVector( vector<HeuristicVector> &VetorParam )
{
	if( VetorParam.size() > 0 )
		VetorParam.erase(VetorParam.end());
}

void Aprendizagem_Reforco::loadMatrixQ( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "matrizes/MatrizQXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[16] = fileNumber[0];
	dir[17] = fileNumber[1];

	ifstream Fin( dir, ios::in );
	if (!Fin) // "Tried to read but failed, recreating/overwriting file"
	{
		ofstream Fout( dir, ios::out );
		if (!Fout) // Couldn't open for writing! path doesn't exist ? (check dir)
		{
			cerr << "ERROR while creating MatrizQ for player " << playerNum << "\n";
			for( int i=0; i<num_linesQ; i++ )
			{
				for( int j=0; j<num_acoes; j++ )
					MatrizQ[i][j] = 0.0; // passing values to array MatrizQ
			}
		}
		else // File successfully opened for writing
		{
			for( int i=0; i<num_linesQ; i++ )
			{
				for( int j=0; j<num_acoes; j++ )
				{
					Fout << "0 ";
					MatrizQ[i][j] = 0.0; // passing values to array MatrizQ
				}
				Fout << "\n";
			}
			cerr << "Successfully created Matrix for player " << playerNum << "\n";
		}
		Fout.close();
	}
	else // "Successful in reading, pass values to array MatrizQ"
	{
		for( int i=0; i<num_linesQ; i++ )
		{
			for( int j=0; j<num_acoes; j++ )
			{
				Fin >> MatrizQ[i][j];
			}
		}
		cerr << "Successfully read Matrix for player " << playerNum << "\n";
	}
	Fin.close();
}

void Aprendizagem_Reforco::saveMatrixQ( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "matrizes/MatrizQXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[16] = fileNumber[0];
	dir[17] = fileNumber[1];

	ofstream Fout( dir, ios::out );

	if (!Fout) // Couldn't open for writing! path doesn't exist ? (check dir)
	{
		cerr << "ERROR while saving MatrizQ for player " << playerNum << "\n";
	}
	else // "Successful in opening the file, saving values of array MatrizQ"
	{
		unsigned long long numElementos = 0;
		unsigned long long difZero = 0;

		for( int i=0; i<num_linesQ; i++ )
		{
			for( int j=0; j<num_acoes; j++ )
			{
				Fout << MatrizQ[i][j] << " ";
				numElementos++;
				if( MatrizQ[i][j] != 0 )
					difZero++;
			}
			Fout << "\n";
		}
		cerr << "Successfully saved Matrix for player " << playerNum << "\n";
		porcentagemNaoNulosNaMatrizQ = (difZero/((double)numElementos*1.0))*100.0;
		cerr << "Matrix info: " << "(" << difZero << "/" << numElementos << ") = "
				<< porcentagemNaoNulosNaMatrizQ << "% explored for player " << playerNum << "\n";
	}
	Fout.close();
}

void Aprendizagem_Reforco::loadMatrixH( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "matrizes/MatrizHXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[16] = fileNumber[0];
	dir[17] = fileNumber[1];

	ifstream Fin( dir, ios::in );
	if (!Fin) // "Tried to read but failed, recreating/overwriting file"
	{
		ofstream Fout( dir, ios::out );
		if (!Fout) // Couldn't open for writing! path doesn't exist ? (check dir)
		{
			cerr << "ERROR while creating MatrizH for player " << playerNum << "\n";
			for( int i=0; i<num_linesQ; i++ )
			{
				for( int j=0; j<num_acoes; j++ )
					MatrizH[i][j] = 0.0; // passing values to array MatrizH
			}
		}
		else // File successfully opened for writing
		{
			for( int i=0; i<num_linesQ; i++ )
			{
				for( int j=0; j<num_acoes; j++ )
				{
					Fout << "0 ";
					MatrizH[i][j] = 0.0; // passing values to array MatrizH
				}
				Fout << "\n";
			}
			cerr << "Successfully created Matrix for player " << playerNum << "\n";
		}
		Fout.close();
	}
	else // "Successful in reading, pass values to array MatrizH"
	{
		for( int i=0; i<num_linesQ; i++ )
		{
			for( int j=0; j<num_acoes; j++ )
			{
				Fin >> MatrizH[i][j];
			}
		}
		cerr << "Successfully read Matrix for player " << playerNum << "\n";
	}
	Fin.close();
}

void Aprendizagem_Reforco::saveMatrixH( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "matrizes/MatrizHXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[16] = fileNumber[0];
	dir[17] = fileNumber[1];

	ofstream Fout( dir, ios::out );

	if (!Fout) // Couldn't open for writing! path doesn't exist ? (check dir)
	{
		cerr << "ERROR while saving MatrizH for player " << playerNum << "\n";
	}
	else // "Successful in opening the file, saving values of array MatrizH"
	{
		unsigned long long numElementos = 0;
		unsigned long long difZero = 0;

		for( int i=0; i<num_linesQ; i++ )
		{
			for( int j=0; j<num_acoes; j++ )
			{
				Fout << MatrizH[i][j] << " ";
				numElementos++;
				if( MatrizH[i][j] != 0 )
					difZero++;
			}
			Fout << "\n";
		}
		cerr << "Successfully saved Matrix for player " << playerNum << "\n";
		porcentagemNaoNulosNaMatrizH = (difZero/((double)numElementos*1.0))*100.0;
		cerr << "Matrix info: " << "(" << difZero << "/" << numElementos << ") = "
				<< porcentagemNaoNulosNaMatrizH << "% explored for player " << playerNum << "\n";
	}
	Fout.close();
}

void Aprendizagem_Reforco::loadInfo( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "info/InfoXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[9] = fileNumber[0];
	dir[10] = fileNumber[1];

	ifstream Fin( dir, ios::in );
	if (!Fin) // "Tried to read but failed, recreating/overwriting file"
	{
		ofstream Fout( dir, ios::out );
		if (!Fout) // Couldn't open for writing! path doesn't exist ? (check dir)
		{
			cerr << "ERROR while creating Info for player " << playerNum << "\n";
		}
		else // File successfully opened for writing
		{
			Fout << 0 << "\n" << 0 << "\n";
			Num_Jogos = 0;
			Num_Itera = 0;
		}
		Fout.close();
	}
	else
	{
		Fin >> Num_Jogos;
		Fin >> Num_Itera;
	}
	Fin.close();
}

void Aprendizagem_Reforco::saveInfo( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "info/InfoXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[9] = fileNumber[0];
	dir[10] = fileNumber[1];

	ofstream Fout( dir, ios::out );

	if( Fout ) // opened for writing (appending to file)
	{
		Fout << Num_Jogos << "\n" << Num_Itera << "\n";
		cerr << "Successfully saved Info for player " << playerNum << "\n";
	}

	Fout.close();
}

void Aprendizagem_Reforco::loadReport( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "report/ReportXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[13] = fileNumber[0];
	dir[14] = fileNumber[1];

	ifstream Fin( dir, ios::in );
	if (!Fin) // "Tried to read but failed, recreating/overwriting file"
	{
		ofstream Fout( dir, ios::out );
		if (!Fout) // Couldn't open for writing! path doesn't exist ? (check dir)
		{
			cerr << "ERROR while creating Report for player " << playerNum << "\n";
		}
		else // File successfully opened for writing
		{
			Fout << "Num_Jogo ";
			Fout << "Num_Acoes ";
			for( int i = 0; i<num_acoes; i++ )
			{
				Fout << "A" << i+1 << " ";
			}
			Fout << "Gols_Feitos ";
			Fout << "Gols_Sofridos ";
			Fout << "Epsilon ";
			Fout << "Exp_Q(%) ";
			Fout << "Exp_H(%) ";
			Fout << "\n";
		}
		Fout.close();
	}
	Fin.close();
}

void Aprendizagem_Reforco::saveReport( int playerNum )
{
	char fileNumber[2]; // player number
	char dir[] = "report/ReportXX.csv"; // better not change this

	switch( playerNum ){
	case 0:
		fileNumber[0] = '0';
		fileNumber[1] = '0';
		break;
	case 1:
		fileNumber[0] = '0';
		fileNumber[1] = '1';
		break;
	case 2:
		fileNumber[0] = '0';
		fileNumber[1] = '2';
		break;
	case 3:
		fileNumber[0] = '0';
		fileNumber[1] = '3';
		break;
	case 4:
		fileNumber[0] = '0';
		fileNumber[1] = '4';
		break;
	case 5:
		fileNumber[0] = '0';
		fileNumber[1] = '5';
		break;
	case 6:
		fileNumber[0] = '0';
		fileNumber[1] = '6';
		break;
	case 7:
		fileNumber[0] = '0';
		fileNumber[1] = '7';
		break;
	case 8:
		fileNumber[0] = '0';
		fileNumber[1] = '8';
		break;
	case 9:
		fileNumber[0] = '0';
		fileNumber[1] = '9';
		break;
	case 10:
		fileNumber[0] = '1';
		fileNumber[1] = '0';
		break;
	case 11:
		fileNumber[0] = '1';
		fileNumber[1] = '1';
		break;
	case 12:
		fileNumber[0] = '1';
		fileNumber[1] = '2';
		break;
	}

	dir[13] = fileNumber[0];
	dir[14] = fileNumber[1];

	ofstream Fout( dir, ios::app );

	if( Fout ) // opened for writing (appending to file)
	{
		Fout << Num_Jogos << " ";
		for( int i = 0; i<num_acoes+6; i++ )
		{
			Fout << VetorRelatorio[i] << " ";
		}
		Fout << "\n";
		cerr << "Successfully saved Report for player " << playerNum << "\n";
	}

	Fout.close();
}
