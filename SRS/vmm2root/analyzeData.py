import os
import subprocess
import re


#directory=os.path.dirname(os.path.abspath(__file__))
directory='/Users/dpfeiffe/data/vmm2TimingStudy'
for filename in os.listdir(directory):
	print filename
	if filename.endswith(".raw") and filename.startswith("run_") and filename.find("500ns") > -1: 
		dataFileName = os.path.join(directory, filename)
		print dataFileName
		args = ['./vmm2root', '-rd', dataFileName, '-tac', '500', '-bc', '2.5', '-x', '0,1']
		subprocess.call(args) 
	else:
		continue

