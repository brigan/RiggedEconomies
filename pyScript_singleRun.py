"""

	pyScript_singleRun.py: 

		This script calls a single instance of scriptSingleAgent and generates some plots associated to it. 

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
n = 2; 					# Number of games. 
N_A = 1000; 			# Maximum number of agents. 

# Init mode: 
initMode = 1; 			# Simulation starts with a single agent. 

# Costs and benefits: 
c_R = 1.; 				# Cost of attempting to rig a game. 
B = 600; 				# Total wealth per round. 
c_C=10.; 				# Cost of having descent. 
b_0=0.; 				# Initial endowment to new agents (this option has not been explored). 

# Copying: 
p_C = 1.; 				# Likelihood of having descent (this option has not been explored). 

# Variables to track time: 
tMax = 5000; 
tReport = 1; 			# Thought of for large simulations in which we do not store every time step to the disk. 
tAverage = 500; 		# Number of time steps over which to average in the steady state. 


## I/O variables: 
#  Results are stored to baseName folder with a generic name: 
baseName = "./ResultsTmp/"; 
if os.path.isdir(baseName): 
	strCall = "rm " + baseName + "* "; 
	os.system(strCall); 
else: 
	strCall = "mkdir " + baseName; 
	os.system(strCall); 


### Looping over simulation conditions: 
# 	This script runs a single simulation with fixed conditions, so it does not need to loop over conditions. 

# Generate a random seed (unimportant in this case). 
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


### Producing some plots: 

# Number of agents over time: 
plt.figure(); 
plt.plot(nAgents); 
plt.xlabel("Time"); 
plt.ylabel("Number of agents"); 

# Total wealth over time: 
plt.figure(); 
plt.plot(thisWealth); 
plt.xlabel("Time"); 
plt.ylabel("Total wealth"); 

# Total wealth per agent over time: 
plt.figure(); 
plt.plot(np.divide(thisWealth, nAgents)); 
plt.xlabel("Time"); 
plt.ylabel("Average wealth per agent"); 

# Rigging pressure over time: 
plt.figure(); 
plt.plot(riggingPressure); 
plt.xlabel("Time"); 
plt.ylabel("Average rigging pressure"); 


## Dynamics of a single game: 
if (n==1): 
	# Preferred strategy on that game: 
	plt.figure(); 
	plt.plot(np.squeeze(majorityAction)); 
	plt.xlabel("Time"); 
	plt.ylabel("Preferred action"); 

	# Strategy entropy on that game: 
	plt.figure(); 
	plt.plot(np.squeeze(hMA)); 
	plt.xlabel("Time"); 
	plt.ylabel("Strategy entropy"); 

	# Rigging pressure on that game: 
	plt.figure(); 
	plt.plot(np.squeeze(fRiggingAction)); 
	plt.xlabel("Time"); 
	plt.ylabel("Rigging pressure"); 

else: 

	# Plotting the dynamcis of the first two games as an illustration: 

	# Preferred strategy on that game: 
	plt.figure(); 
	plt.plot(majorityAction[:,0]); 
	plt.plot(majorityAction[:,1]); 
	plt.xlabel("Time"); 
	plt.ylabel("Preferred action"); 

	# Strategy entropy on that game: 
	plt.figure(); 
	plt.plot(hMA[:,0]); 
	plt.plot(hMA[:,1]); 
	plt.xlabel("Time"); 
	plt.ylabel("Strategy entropy"); 

	# Rigging pressure on that game: 
	plt.figure(); 
	plt.plot(fRiggingAction[:,0]); 
	plt.plot(fRiggingAction[:,1]); 
	plt.xlabel("Time"); 
	plt.ylabel("Rigging pressure"); 

plt.show(); 
