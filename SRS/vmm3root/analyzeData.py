import os
import subprocess
import re


#directory=os.path.dirname(os.path.abspath(__file__))
directory='/Users/dpfeiffe/data/testbeam'
for filename in os.listdir(directory):
	if filename.endswith(".csv") and filename.startswith("Run"): 
		dataFileName = os.path.join(directory, filename)
		print dataFileName
		args = ['./vmm3root', '-d', dataFileName, '-tac', '60', '-bc', '20', '-x', '0,1','-y','14,15','-readout','GEM','-th','70', '-cs','3']
		subprocess.call(args) 
	else:
		continue

