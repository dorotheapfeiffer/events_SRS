import os
import subprocess
import re


#directory=os.path.dirname(os.path.abspath(__file__))
directory='/Users/dpfeiffe/data/vmm2TimingStudy/reset'
for filename in os.listdir(directory):
	print filename
	if filename.endswith(".raw"): 
		dataFileName = os.path.join(directory, filename)
		print dataFileName
		args = ['./vmm2root', '-rd', dataFileName, '-tac', '125', '-bc', '40', '-x', '0,1']
		subprocess.call(args) 
	else:
		continue

