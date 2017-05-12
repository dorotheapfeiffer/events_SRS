import os
import subprocess
directory=os.path.dirname(os.path.abspath(__file__))

for filename in os.listdir(directory):
    if filename.endswith(".raw") : 
        dataFileName = os.path.join(directory, filename)
        print dataFileName
        args = ['./vmm2root', '-rd', dataFileName]
        subprocess.call(args) 
    else:
        continue

