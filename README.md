# RiggedEconomies
Repository with code to reproduce the results from the paper "Games in rigged economies". 

Code is written in Python3 and c++. 

	>> c++ libraries riggingAgents.cpp and riggedSociety.cpp implement the model. 

	>> Python scripts run specific simulations and produce plots from the main part of the paper. 

	>> pyScript_singleRun.py: 
		> Executes a single instance of the model: an economy with fixed wealth per round and number of games, fixed costs and benefits, and a variable number of agents according to the model's rules. 
		> Generates plots to visualize the dynamics of this single simulation over time. 

	>> pyScript_varyB.py: 
		> Executes a series of model simulations as total economy size grows. 
		> Generates plots of several quantities (e.g. stable population size, rigging pressure, etc.) as economy size changes. 

	>> pyScript_vary-n.py: 
		> Executes a series of model simulations as economic complexity (as measured by number of games) changes. 
		> Generates plots of several quantities (e.g. stable population size, rigging pressure, etc.) as complexity changes. 

	>> pyScript_varyB-n.py: 
		> Executes a series of model simulations as both economic complexity (as measured by number of games) and economic
		size varies. 
		> Generates the two-dimensional charts of model economies. 


