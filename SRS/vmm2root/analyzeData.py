import os
import subprocess
#directory=os.path.dirname(os.path.abspath(__file__))
directory='/Users/dpfeiffe/data/vmm2TimingStudy'
for filename in os.listdir(directory):
	print filename
	if filename.endswith(".raw") and filename.startswith("run_"): 
		dataFileName = os.path.join(directory, filename)
		print dataFileName
		args = ['./vmm2root', '-rd', dataFileName, '-tac', '125', '-bc', '2.5']
		subprocess.call(args) 
	else:
		continue

