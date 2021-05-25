"""

	pyScript_varyB.py: 

		This script calls successive instances of scriptSingleAgent varying the wealth allocated per game and generates plots
		showing how this trends. 

"""

import numpy as np; 
import matplotlib.pyplot as plt; 
import os, sys; 
from scipy.stats import entropy; 
import random as rnd; 
import helper as h; 


## Variables that define the rigged society and agents: 
# Seeds: 
s=1; 
s_=1; 

# Sizes: 
n = 20; 						# Number of games. 
N_A = 1000; 					# Maximum number of agents. 

# Init mode: 
initMode = 1; 					# Simulation starts with a single agent. 

# Costs and benefits: 
c_R = 1.; 						# Cost of attempting to rig a game. 
# BList = range(50, 3050, 50); 	# Range of values of B explored. 
BList = range(50, 1550, 50); 	# Range of values of B explored. 
c_C=10.; 						# Cost of having descent. 
b_0=0.; 						# Initial endowment to new agents (this option has not been explored). 

# Copying: 
p_C = 1.; 						# Likelihood of having descent (this option has not been explored). 

# Variables to track time: 
tMax = 1000; 
tReport = 1; 					# Thought of for large simulations in which we do not store every time step to the disk. 
tAverage = 500; 				# Number of time steps over which to average in the steady state. 


## I/O variables: 
#	Results of single simulations are stored in baseName. 
# 	Results of average as simulation parameters change are stored in baseName_. 
baseName = "./ResultsTmp/"; 
baseName_ = "./Results/"; 
if os.path.isdir(baseName): 
	strCall = "rm " + baseName + "* "; 
	os.system(strCall); 
else: 
	strCall = "mkdir " + baseName; 
	os.system(strCall); 
if not(os.path.isdir(baseName_)): 
	strCall = "mkdir " + baseName_; 
	os.system(strCall); 

# Files to keep summarized measures: 
fOutNA = open(os.path.join(baseName_, "dataNA.csv"), 'w'); 
fOutWealth = open(os.path.join(baseName_, "dataWealth.csv"), 'w'); 
fOutMA = open(os.path.join(baseName_, "dataMA.csv"), 'w'); 
fOutRP = open(os.path.join(baseName_, "dataRP.csv"), 'w'); 


### Looping over simulation conditions: 

# Loop over wealth per round: 
for B in BList: 

	# Generate a random seed (such that we do not just produce clones of a same run). 
	s_ = rnd.randint(1, 10000000); 

	## Setting benefit per game: 
	#	Uncomment each option to explore different scalings of wealth with economic complexity. 
	b = float(B)/n; 					# CaseI: Decreasing benefit per game. Total wealth constant. 
	# b = float(B); 					# CaseII: Constant benefit per game. Total wealth grows linealy with n. 
	# b = (np.log(n)+1)*float(B); 		# CaseIII: Benefit per game grows as log(n). Total wealth grows as n*log(n). 
	# b = n*float(B); 					# CaseIV: Benefit per game grows linearly as n. Total wealth grows as n^2. 

	## Launching actual simulation: 
	# String to launch from command line: 
	strCall = "./scriptSingleAgent -s " + str(s_) + " -n " + str(n) + " -N_A " + str(N_A); 
	strCall += " -initMode " + str(initMode) + " -c_R " + str(c_R) + " -b "; 
	strCall += str(b) + " -c_C " + str(c_C) + " -p_C " + str(p_C) + " -b_0 "; 
	strCall += str(b_0) + " -tMax " + str(tMax) + " -tReport " + str(tReport); 
	strCall += " -path " + baseName; 
	print(strCall); 
	# Run: 
	os.system(strCall); 


	## Extrating average measurements: 
	# Loading dataB.csv, which contains the wealth each agent over time: 
	fIn = open(os.path.join(baseName, "dataB.csv"), 'r'); 	# Can't use np.loadtxt() because number of agentes changes over time. 
	dR = fIn.read(); 
	fIn.close(); 
	dL = [ll for ll in dR.split('\n') if ll!='']; 
	accruedB = []; 
	nAgents = []; 
	thisWealth = []; 
	for ll in dL: 
		accruedB += [[float(elem) for elem in ll.split(', ') if ll!='']]; 	# List of lists with all the wealth of all the agents. 
		nAgents += [len(accruedB[-1])]; 									# Number of agents over time. 
		thisWealth += [sum(accruedB[-1])]; 									# Sum of the wealth of all the agents, over time. 


	# Loading preferred strategy in each game over time: 
	majorityAction = np.loadtxt(os.path.join(baseName, "dataMA.csv"), delimiter=', '); 
	if (n==1): 
		majorityAction = np.array([majorityAction]); 
		hMA = h.computeEntropy(majorityAction); 
		hMA_mean = np.mean(hMA, 0); 
	else: 
		hMA = h.computeEntropy(majorityAction); 
		hMA_mean = np.mean(hMA, 1); 

	# Loading rigging pressure: 
	fRiggingAction = np.loadtxt(os.path.join(baseName, "dataFRA.csv"), delimiter=', '); 	# This is rigging pressure in each game. 
	riggingPressure = fRiggingAction; 
	if (n>1): 
		riggingPressure = np.mean(fRiggingAction, 1); 	# This is rigging pressure per game. 


	## Measuring averages over the steady state, which is what will be reported in this script: 

	# Enough iterations? 
	if (len(nAgents)<tMax+1): 
		fOutNA.write(str(B) + ", " + str(n) + ", " + str(0.) + ", " + str(0.) + '\n'); 
		fOutWealth.write(str(B) + ", " + str(n) + ", " + str(0.) + ", " + str(0.) + '\n'); 
		fOutMA.write(str(B) + ", " + str(n) + ", " + str(0.) + ", " + str(0.) + '\n'); 
		fOutRP.write(str(B) + ", " + str(n) + ", " + str(0.) + ", " + str(0.) + '\n'); 
	else: 
		meanNAgents = np.mean(nAgents[-tAverage::]); 
		stdNAgents = np.std(nAgents[-tAverage::]); 
		meanWealth = np.mean(thisWealth[-tAverage::]); 
		stdWelath = np.std(thisWealth[-tAverage::]); 
		meanHMA_mean = np.mean(hMA_mean[-tAverage::]); 
		stdHMA_mean = np.std(hMA_mean[-tAverage::]); 
		meanRiggingPressure = np.mean(riggingPressure[-tAverage::]); 
		stdRiggingPressure = np.std(riggingPressure[-tAverage::]); 
		fOutNA.write(str(B) + ", " + str(n) + ", " + str(meanNAgents) + ', ' + str(stdNAgents) + '\n'); 
		fOutWealth.write(str(B) + ", " + str(n) + ", " + str(meanWealth) + ", " + str(stdWelath) + '\n'); 
		fOutMA.write(str(B) + ", " + str(n) + ", " + str(meanHMA_mean) + ', ' + str(stdHMA_mean) + '\n'); 
		fOutRP.write(str(B) + ", " + str(n) + ", " + str(meanRiggingPressure) + ', ' + str(stdRiggingPressure) + '\n'); 

# Closing files: 
fOutNA.close(); 
fOutWealth.close(); 
fOutMA.close(); 
fOutRP.close(); 


### Producing some plots: 
# 		Now we are interested in reporting averages in the steady state as a function of the changing parameters (in this
# 		case, B). Therefore we need to load the data from the respective files. 


## Loading and plotting number of agents: 
dataNAgents = np.loadtxt(os.path.join(baseName_, "dataNA.csv"), delimiter=", "); 

plt.figure(); 
plt.fill_between(dataNAgents[:,0], np.add(dataNAgents[:,2], dataNAgents[:,3]), np.subtract(dataNAgents[:,2], dataNAgents[:,3])); 
plt.plot(dataNAgents[:,0], dataNAgents[:,2], 'k'); 
plt.xlabel("Wealth per round"); 
plt.ylabel("Number of agents"); 


## Loading and plotting total wealth: 
dataWealth = np.loadtxt(os.path.join(baseName_, "dataWealth.csv"), delimiter=", "); 

plt.figure(); 
plt.fill_between(dataWealth[:,0], np.add(dataWealth[:,2], dataWealth[:,3]), np.subtract(dataWealth[:,2], dataWealth[:,3])); 
plt.plot(dataWealth[:,0], dataWealth[:,2], 'k'); 
plt.xlabel("Wealth per round"); 
plt.ylabel("Total wealth"); 


## Loading and plotting strategy entropy: 
dataMA = np.loadtxt(os.path.join(baseName_, "dataMA.csv"), delimiter=", "); 

plt.figure(); 
plt.fill_between(dataMA[:,0], np.add(dataMA[:,2], dataMA[:,3]), np.subtract(dataMA[:,2], dataMA[:,3])); 
plt.plot(dataMA[:,0], dataMA[:,2], 'k'); 
plt.xlabel("Wealth per round"); 
plt.ylabel("Average strategy entropy"); 


## Loading and plotting rigging pressure: 
dataRP = np.loadtxt(os.path.join(baseName_, "dataRP.csv"), delimiter=", "); 

plt.figure(); 
plt.fill_between(dataRP[:,0], np.add(dataRP[:,2], dataRP[:,3]), np.subtract(dataRP[:,2], dataRP[:,3])); 
plt.plot(dataRP[:,0], dataRP[:,2], 'k'); 
plt.xlabel("Wealth per round"); 
plt.ylabel("Average rigging pressure"); 


plt.show(); 
