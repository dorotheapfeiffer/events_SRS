#!/bin/bash	
# This script run root macro's for the all file in the directory
# /home/dlab/dg_epool/zaba/data

#dataPath="/home/dlab/data/26Jul2016/"
dataPath="/home/dlab/dg_epool/zaba/data/"
 for i in $( ls -1 $dataPath ); do
   root -b -q -l `printf "readBin.C(\"%s\")" "${dataPath}${i}"`
   echo "================================================="
 done
 
