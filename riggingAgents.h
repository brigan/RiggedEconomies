/* 

	riggingAgents.h: 

		This file contains the implementation of rigged agents for the rigged games project. 

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
using namespace std; 
std::default_random_engine generator;

namespace rA{

	/*

		riggingAgent name space. 

	*/ 

	class RiggingAgent{
		/* RiggingAgent class: 

			Objects of this class take actions regarding a series of games, collect a benefit depending on the
			performance on those games, and use this benefit to i) rig the outcome of future games and ii) reproduce
			themselves.

			Variables (these variables suffice to implement the first version of the model): 
				>> n: number of games that the RiggingAgent is engaged in. 
				>> nRiggingActions: number of games the RiggingAgent attempts to rig. 
				>> nUnriggedGames: number of games that the RiggingAgent is not attempting to rig (n - nRiggingActions). 
				>> actions: bit string defining the actions that the agent takes in each of the games available. 
				>> riggingActions: collection of integers indicating what actions this agent will try to rig. 
				>> unriggedGames: collection of integers storing what games the agent does not attempt to rig. 
				>> b: benefit accumulated over time by playing the games. 
				>> p_C: probability that the agent will replicate (if the accumulated benefit is larger than the replicate cost). 

			Additional variables to assist on newer versions of the model: 
				>> age: Number of times that the agent has played a game -- helps us program a limited life span. 
				>> W: Wealth accumulated discounting the cost of copying. 

		*/ 


		public: 

			// Variables: 
			// 	ACH! I have made variables public because I cannot understand how inheritance works in c++... 
			int n, nRiggingActions, nUnriggedGames; 
			vector<int> actions, riggingActions, unriggedGames; 
			double b, p_C, mRateActions, mRateRiggingActions; 
			// Additional variables (for extended model): 
			int age; 
			double W; 



			// Init functions: 
			RiggingAgent(), RiggingAgent(int, double, double, int, double, double); 
			RiggingAgent(int, vector<int>, vector<int>, double, double, double, double); 

			// Set and generate functions: 
			void setN(int), setP_C(double), setB(double), addToB(double); 
			void generateActions(int), generateRiggingActions(int); 
			void setAction(int, int), setActions(vector<int>), setRiggingActions(vector<int>); 
			void setMRateActions(double), setMRateRiggingActions(double); 
			// 
			void setAge(int), tickAge(), setW(double), addToW(double); 

			// Get functions: 
			int getN(), getNRiggingActions(), getNUnriggedGames(); 
			double getP_C(), getB(); 
			int getAction(int), getRiggingAction(int); 
			vector<int> getActions(), getRiggingActions(); 
			double getMRateActions(), getMRateRiggingActions(); 
			// 
			int getAge(); 
			double getW(); 

			// Functional functions: 
			void mutate(); 

	}; 

	///////////////////////////////
	// 
	//  RiggingAgent functions: 

		///////////////////////////////
		// 
		//  Init Functions: 

		RiggingAgent::RiggingAgent(){return;}

		RiggingAgent::RiggingAgent(int n_, double p_C_=1., double b_=0., int initMode=0, double mRateActions_=0.1, double mRateRiggingActions_=0.1){
			/* init function for class RiggingAgent: 

				This function initializes a RiggingAgent with a given number of games in which it participates and on of
				several available ways. By default, agents will be initialized randomly -- i.e. with their participation
				and rigging behavior generated by drawing ones and zeros with probability 0.5. 

				Inputs: 
					>> n_: number of games in which the RiggingAgent is engaged. 
					>> p_C=1.: probability that the agent will replicate .
					>> b_=0.: the agent might be created with an initial benefit. 
					>> initMode=0: mode to initialize the behavior of the agent. 
						> 0: both actions and riggingActions are generated by drawing 0s and 1s from a uniform distribution. 
						> 1: both actions are generated by drawing 0s and 1s from a uniform distribution. No rigging actions. 
					>> mRateActions=0.1: mutation rate of the actions -- likelihood with which it switches a bit. 
					>> mRateRiggingActions=0.1: mutation rate of the rigging actions -- likelihood with which it switches a bit. 

			*/


			setN(n_); 
			setP_C(p_C_); 
			setB(b_); 
			generateActions(initMode); 
			generateRiggingActions(initMode); 
			setMRateActions(mRateActions_); 
			setMRateRiggingActions(mRateRiggingActions_); 
			// 
			setAge(0); 
			setW(b_); 

			return; 
		}

		RiggingAgent::RiggingAgent(int n_, vector<int> actions_, vector<int> riggingActions_, double p_C_=1., double b_=0., double mRateActions_=0.1, double mRateRiggingActions_=0.1){
			/* init function for class RiggingAgent: 

				This function initializes RiggingAgents with a given number of games in which it participates, as well
				as the bahvior and rigging behavior towards those games. 

				Inputs: 
					>> n_ number of games in which the RiggingAgent is engaged. 
					>> actions_: behavior of the agent towards each of these games. 
					>> riggingActions_: behavior of the agent towards rigging each of the games. 
					>> p_C=1.: probability that the agent will replicate .
					>> b_=0: the agent might be created with an initial benefit. 

			*/ 

			setN(n_); 
			setP_C(p_C_); 
			setB(b_); 
			setActions(actions_); 
			setRiggingActions(riggingActions_); 
			setMRateActions(mRateActions_); 
			setMRateRiggingActions(mRateRiggingActions_); 
			// 
			setAge(0); 
			setW(b_); 

			return; 
		}


		///////////////////////////////
		// 
		//  Set and generate functions: 

		void RiggingAgent::setN(int n_){
			/* setN function: */

			n = n_; 

			return; 
		}

		void RiggingAgent::setP_C(double p_C_){
			/* setP_C function: */ 

			p_C = p_C_; 

			return; 
		}

		void RiggingAgent::setB(double b_){
			/* setB function: */ 

			b = b_; 

			return; 
		}

		void RiggingAgent::addToB(double dB){
			/* addToB function: 

				This function adds a quantity to the accrued b. This quantity might be negative. This spares some
				operations when implementing because getB() does not need to be called repeateedly. 

				Inputs: 
					>> dB: amount to be added to b. 

			*/ 

			b += dB; 

			return; 
		}

		void RiggingAgent::generateActions(int initMode=0){
			/* generateActions function: 

				This function generates a set of n actions from scratch according to one of a series of modes specified
				by initMode. 

				Inputs: 
					>> initMode=0: specifies how the actions are generated. 
						> 0: random uniform. 

			*/ 

			if (initMode < 2){
				actions.clear(); 
				for (int i=0; i<n; i++){
					actions.push_back(rand()%2); 
				}
			}


			return; 
		}

		void RiggingAgent::generateRiggingActions(int initMode=0){
			/* generateRiggingActions function: 

				This function generates the behavior of the agent during the rigging part of the game. The
				initialization is done according to one of a series of modes specified by initMode. 

				Inputs: 
					>> initMode=0: specifies how the rigging behavior is generated. 
						> 0: random uniform. 

			*/ 

			// Efficiently generating riggingActions: 

			if (initMode==0){
				// Generate Poisson distro: 
				std::poisson_distribution<int> distribution(0.5*n);
	 			nRiggingActions = distribution(generator); 
	 			nRiggingActions = min(nRiggingActions, n); // ACHTUNG!! This matters! 
	 			int iToKnock; // To store what games are rigged! 

	 			// Store all games in unriggedGames, from which we will read rigged games one by one: 
	 			unriggedGames.clear(); 
	 			for (int i=0; i<n; i++){
	 				unriggedGames.push_back(i); 
	 			}

	 			// Extract random games from unriggedGames and push them back to riggingActions: 
				riggingActions.clear(); 
				for (int i=0; i<nRiggingActions; i++){
					iToKnock = rand()%unriggedGames.size(); 				// Extrac random game. 
					riggingActions.push_back(unriggedGames[iToKnock]); 		// Append to riggingActions. 
					unriggedGames.erase(unriggedGames.begin()+iToKnock); 	// Extract from unriggedGames. 
				}
			}
			else if (initMode==1){
				// Actions are generated randomly, but the agent does not attempt to rig any game: 
				nRiggingActions = 0; 
	 			unriggedGames.clear(); 
	 			for (int i=0; i<n; i++){
	 				unriggedGames.push_back(i); 
	 			}

	 			riggingActions.clear(); 
			}
	 		
	 		nUnriggedGames = n - nRiggingActions; 


			return; 
		}

		void RiggingAgent::setAction(int iAction, int whatAction){
			/* setActions function: 

				This function sets a single strategy for this agent. 

				Inputs: 
					>> iAction: to be set (needs to be within number of games and actions vector needs to have been initialized). 
					>> whatAction: To what action we set the corresponding strategy. 

			*/

			actions[iAction] = whatAction; 

			return; 
		}

		void RiggingAgent::setActions(vector<int> actions_){
			/* setActions function: */ 

			actions.clear(); 
			for (int i=0; i<n; i++){
				actions.push_back(actions_[i]); 
			}

			return; 
		}

		void RiggingAgent::setRiggingActions(vector<int> riggingActions_){
			/* setRiggingActions function: 

				This function sets a collection of rigging actions. As a consecuence it also sets the collection of
				unrigged games by the RiggingAgent, as well as the number of rigging actions and unrigged games. 

				Inputs: 
					>> riggingActions: to be set. 

			*/ 

			// Dummy to store unrigged games. 
			bool unriggedGamesDumy[n]; 
			for (int i=0; i<n; i++){
				unriggedGamesDumy[i] = true; 
			}

			// Compute nRiggingActions, nUnriggedGames, and read riggingActions: 
			nRiggingActions = riggingActions_.size(); 
			nUnriggedGames = n - nRiggingActions; 
			riggingActions.clear(); 
			for (int i=0; i<nRiggingActions; i++){
				riggingActions.push_back(riggingActions_[i]); 
				unriggedGamesDumy[riggingActions.back()] = false; 
			}

			// Storing unrigged games in the corresponding vector: 
			unriggedGames.clear(); 
			for (int i=0; i<n; i++){
				if (unriggedGamesDumy[i]){
					unriggedGames.push_back(i); 
				}
			}

			return; 
		}

		void RiggingAgent::setMRateActions(double mRateActions_){
			/* setMRateActions function: */ 

			mRateActions = mRateActions_; 

			return; 
		}

		void RiggingAgent::setMRateRiggingActions(double mRateRiggingActions_){
			/* setMRateRiggingActions function: */ 

			mRateRiggingActions = mRateRiggingActions_; 
			
			return; 
		}

		// 

		void RiggingAgent::setAge(int age_=0){
			/* setAge function: */

			age = age_; 

			return; 
		}

		void RiggingAgent::tickAge(){
			/* tickAge function: 

				This function increases age in one unit. 

			*/

			age++; 

			return; 
		}

		void RiggingAgent::setW(double W_){
			/* setW function: */

			W = W_; 

			return; 
		}

		void RiggingAgent::addToW(double dW){
			/* addToW function: 

				This function adds a finite quantity (that might be negative) to W. 

			*/

			W += dW; 

			return; 
		}


		///////////////////////////////
		// 
		//  Get functions: 

		int RiggingAgent::getN(){
			/* getN function: 

				Returns: 
					<< n: number of games in which the agent is engaged. 

			*/ 

			return n; 
		} 

		int RiggingAgent::getNRiggingActions(){
			/* getNRiggingActions functions: 

				Returns: 
					<< nRiggingActions: number of games that the RiggingAgent attempts to rig. 

			*/ 

			return nRiggingActions; 
		}

		int RiggingAgent::getNUnriggedGames(){
			/* getNUnriggedGames function: 

				Returns: 
					<< nUnriggedGames: number of games that the RiggingAgent is not attempting to rig. 

			*/ 

			return nUnriggedGames; 
		}

		double RiggingAgent::getP_C(){
			/* getP_C function: 

				Returns: 
					<< p_C: probability that agent replicates if it has an accrued benefit larger than the replication cost. 

			*/ 
			
			return p_C; 
		}

		double RiggingAgent::getB(){
			/* getB function: 

				Returns: 
					<< b: benefit accumulated by the agent. 

			*/

			return b; 
		}

		int RiggingAgent::getAction(int iAction){
			/* getAction function: 

				This function returns the action of this RiggingAgent towards game iAction. This way we avoid collecting
				the whole array every time, thus speeding up the process. 

				Inputs: 
					>> iAction: index of the action to be retrieved. 

				Returns: 
					<< actions[iAction]: action of the RiggingAgent in game iAction. 

			*/ 

			return actions[iAction]; 
		}

		vector<int> RiggingAgent::getActions(){
			/* getActions function: 

				Returns: 
					<< actions: bits indicating what action the agent takes in each of the games. 

			*/ 

			return actions; 
		}


		int RiggingAgent::getRiggingAction(int iRiggingAction){
			/* getRiggingAction function: 

				This function returns the rigging action of this RiggingAgent in position iRiggingAction of the
				corresponding vector. This way we avoid collecting the whole array every time, thus speeding up the
				process.

				Inputs: 
					>> iRiggingAction: index of the rigging action to be retrieved. 

				Returns: 
					<< riggingActions[iAction]: game in position iRiggingAction that the agent will attempt to rig. 

			*/ 

			return riggingActions[iRiggingAction]; 
		}

		vector<int> RiggingAgent::getRiggingActions(){
			/* getRiggingActions function: 
			
				Returns: 
					<< riggingActions: bits indicating whether the agent attempts to rig each of the games. 

			*/ 

			return riggingActions; 
		}

		double RiggingAgent::getMRateActions(){
			/* getMRateActions function: 

				Returns: 
					<< mRateActions: mutation rate of the actions. 

			*/ 

			return mRateActions; 
		}

		double RiggingAgent::getMRateRiggingActions(){
			/* getMRateRiggingActions function: 

				Returns: 
					<< mRateRiggingActions: mutation rate of the rigging actions. 

			*/ 

			return mRateRiggingActions; 
		}

		// 

		int RiggingAgent::getAge() {
			/* getAge function: 

				Returns: 
					<< age: Number of times that the agent has played the games. 

			*/

			return age; 
		}

		double RiggingAgent::getW(){
			/* getW function: 

				Returns: 
					<< W: Total wealth that the agent has amassed in the games -- i.e. not subtracting the cost of descent. 

			*/

			return W; 
		}


		///////////////////////////////
		// 
		//  Functional functions: 

		void RiggingAgent::mutate(){
			/* mutate function: 

				This function mutates the actions and rigging actions of the current agent. Therefore it generates a
				Poisson variable to tell how many bits must be switched. 

			*/ 

			//// Mutations in actions (easy): 
			std::poisson_distribution<int> distribution1(mRateActions*n);
			int nMutations = distribution1(generator), iMutate; 
			// mRateActions<<1, so we assume they will come uncorrelated: 
			for (int i=0; i<nMutations; i++){
				iMutate = rand()%n; 
				actions[iMutate] = abs(actions[iMutate]-1); 
			}

			//// Mutations in rigging actions (more complicated): 
			// First, how many become rigged? 
			vector<int> indexBuffer; 
			indexBuffer.clear(); 
			std::poisson_distribution<int> distribution2(mRateRiggingActions*nUnriggedGames);
			nMutations = min(distribution2(generator), nUnriggedGames); // Important here because nUnriggedGames can be small. 
			for (int i=0; i<nMutations; i++){
				iMutate = rand()%unriggedGames.size(); 
				indexBuffer.push_back(unriggedGames[iMutate]); 
				unriggedGames.erase(unriggedGames.begin()+iMutate); 
			}

			// Now, how many stop being rigged? 
			std::poisson_distribution<int> distribution3(mRateRiggingActions*nRiggingActions);
			nMutations = min(distribution3(generator), nRiggingActions); // Important here because nUnriggedGames can be small. 
			for (int i=0; i<nMutations; i++){
				iMutate = rand()%riggingActions.size(); 
				unriggedGames.push_back(riggingActions[iMutate]); 
				riggingActions.erase(riggingActions.begin()+iMutate); 
			}

			// Incorporate mutations from unrigged games: 
			for (int i=0; i<indexBuffer.size(); i++){
				riggingActions.push_back(indexBuffer[i]); 
			}

			nRiggingActions = riggingActions.size(); 
			nUnriggedGames = n - nRiggingActions; 

			return; 
		}


}


