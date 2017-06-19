import os
import subprocess
import re


#directory=os.path.dirname(os.path.abspath(__file__))
directory='/Users/dpfeiffe/data/vmm2TimingStudy/clustering'
for filename in os.listdir(directory):
	print filename
	if filename.endswith(".raw") and filename.startswith("run_658"): 
		dataFileName = os.path.join(directory, filename)
		print dataFileName
		args = ['./vmm2root', '-rd', dataFileName, '-tac', '1000', '-bc', '5', '-x', '0,1','-y','14,15','-readout','MM2','-th','80', '-cs','3']
		subprocess.call(args) 
	else:
		continue

