"""

	helper.py: 

		Quick helper file to aid in analyzing the data. 

"""


import numpy as np; 
import matplotlib.pyplot as plt; 
import os, sys; 
from scipy.stats import entropy; 


def computeEntropy(majorityActions): 
	""" computeEntropy function: 

			This function computes the entropy of the majority actions to measure how polarized they are. 

			Inputs: 
				>> majorityActions: np matrix with the fractions of agents vouching for each of the options in each game over time. 

	"""

	(tMax, nGames) = majorityActions.shape; 
	h = np.zeros([tMax, nGames]); 
	for (t, iteration) in enumerate(majorityActions): 
		dummyP = np.array([iteration, 1.-iteration]); 
		h[t,:] = entropy(dummyP)/np.log(2); 

	return h; 




