#!/bin/bash	
# This script run root macro's for the all file in the directory
# /home/dlab/dg_epool/zaba/data

dataPath="/home/dlab/dg_epool/zaba/data/"

 for i in $( ls -1 $dataPath ); do
   #file="/home/dlab/data/15Jul2016/"$i 
   file=$dataPath$i 
   root -b -q -l `printf "readBin.C(\"%s\")" "${file}"`
   echo "================================================="
 done
        
