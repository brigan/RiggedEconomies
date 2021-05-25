/* 

	scriptSingleAgent.cpp: 

		This file contains a script to do experiments with a single individual. This single individual starts out with
		arbitrary actions towards all games, and by not rigging any of the results. The reminder of the society springs
		from it. 

*/ 

// Imports: 
#include <iostream> 
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <cstdlib> 
#include <vector>
#include <random>
#include "riggedSociety.h"
using namespace std; 



int main(int argc, char* argv[]){
	/* main routine: 

		Main routine of scriptSingleAgent. 

	*/ 


	// Variables that define the rigged society and agents: 
	int s=2342153; 
	int n=60, N_A=100, initMode=1; 
	double c_R=1., b=float(N_A)/4, c_C=10., p_C=1., b_0=0.; 
	// Variables to track time: 
	bool fGo = true; 
	int t=0, tMax = 1000, tReport = 1; 
	double *accruedB, *majorityActions, *fractionRiggedActions; 

	// I/O variables: 
	ostringstream baseName, strCall; // To write names of paths and folders.  
	baseName.str(""); strCall.str("");
	baseName << "./ResultsTmp/"; 
	ofstream fDataOutB, fDataOutMA, fDataOutFRA;


	// Are we getting specifications from outside? 
	for (int i=0; i<argc; i++){
		if (string(argv[i])=="-s"){
			s = atoi(argv[i+1]);
		}
		else if (string(argv[i])=="-n"){
			n = atoi(argv[i+1]);
		}
		else if (string(argv[i])=="-N_A"){
			N_A = atoi(argv[i+1]);
		}
		else if (string(argv[i])=="-initMode"){
			initMode = atoi(argv[i+1]);
		}
		else if (string(argv[i])=="-c_R"){
			c_R = atof(argv[i+1]);
		}
		else if (string(argv[i])=="-b"){
			b = atof(argv[i+1]);
		}
		else if (string(argv[i])=="-c_C"){
			c_C = atof(argv[i+1]);
		}
		else if (string(argv[i])=="-p_C"){
			p_C = atof(argv[i+1]);
		}
		else if (string(argv[i])=="-b_0"){
			b_0 = atof(argv[i+1]);
		}
		else if (string(argv[i])=="-tMax"){
			tMax = atoi(argv[i+1]);
		}
		else if (string(argv[i])=="-tReport"){
			tReport = atoi(argv[i+1]);
		}
		else if (string(argv[i])=="-path"){
			baseName.str(""); 
			baseName << string(argv[i+1]); 
		}
	}
	srand(s);

	// Initializing RiggedSociety: 
	rA::RiggingAgent reportingRiggingAgent; 
	rS::RiggedSociety thisRiggedSociety(n, N_A, c_R, b, c_C, p_C, b_0, initMode); 

	// Setting up files to save output data: 
	strCall << baseName.str() << "dataB.csv"; 
	fDataOutB.open(strCall.str().c_str(), ofstream::out); 
	strCall.str(""); 
	strCall << baseName.str() << "dataMA.csv"; 
	fDataOutMA.open(strCall.str().c_str(), ofstream::out); 
	strCall.str(""); 
	strCall << baseName.str() << "dataFRA.csv"; 
	fDataOutFRA.open(strCall.str().c_str(), ofstream::out); 
	strCall.str(""); 

	// Doing the loop: 
	while (fGo){
		// Implement a complete step: 
		thisRiggedSociety.doFullStep(0,0,0,0); 
			// 
			// This function implements a complete step of the model. This includes: i) rigging the games, ii) playing them, iii)
			// removing agents without enough wealth, and iii) producing new agents from successful ones. 
			// 
			// The integers passed to this function determine whether some alternative version each function is implemented. As
			// this is set up right now (0, 0, 0, 0), the original model from the paper is called. Some alternatives: 
			//
			// 		>> (1, 0, 0, 0): Votes of agentes are weighted by b, where b is the agent's wealth: all earnings minus all
			// 		losses due to rigging or descent. Thus, better performing agents have more to say in determining a winning
			// 		strategy. 
			// 		>> (2, 0, 0, 0): Votes of agentes are weighted by W, where W is b plus all expenses on descent. Descent might
			// 		leave a successful agent with the same wealth as an unsuccessful one, thus W is a better way of tracking the
			// 		agent's success over time. 
			// 

		// Report values: 
		if (not(t%tReport)){
			accruedB = thisRiggedSociety.getAccruedB(); 
			for (int iAgent=0; iAgent<thisRiggedSociety.getN_alive()-1; iAgent++){
				fDataOutB << accruedB[iAgent] << ", "; 
			}
			fDataOutB << accruedB[thisRiggedSociety.getN_alive()-1] << endl; 
			
			majorityActions = thisRiggedSociety.computeMajorityActions(); 
			fractionRiggedActions = thisRiggedSociety.computeFractionRiggedActions(); 
			for (int iGame=0; iGame<thisRiggedSociety.getN()-1; iGame++){
				fDataOutMA << majorityActions[iGame] << ", "; 
				fDataOutFRA << fractionRiggedActions[iGame] << ", "; 
			}
			fDataOutMA << majorityActions[thisRiggedSociety.getN()-1] << endl; 
			fDataOutFRA << fractionRiggedActions[thisRiggedSociety.getN()-1] << endl; 
		}

		if (not(thisRiggedSociety.getN_alive()) or t==tMax) fGo=false; 
		t++; 
	}



	return 0; 
}




