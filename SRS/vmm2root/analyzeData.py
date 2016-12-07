import os
import subprocess
directory=os.path.dirname(os.path.abspath(__file__))

for filename in os.listdir(directory):
    if filename.endswith(".raw") and filename.startswith("run_"): 
        dataFileName = os.path.join(directory, filename)
        print dataFileName
        args = ['./rvmm2root', '-rd', dataFileName, '-tac', 125, '-bc', 2.5]
        subprocess.call(args) 
    else:
        continue

