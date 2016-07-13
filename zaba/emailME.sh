#!/bin/bash
COUNT=0


while true; do
        # search for the szaba process, return the process ID or 0 if do not exist
	lsal=`pidof szaba` 

        # if exist print, if not print the time and date and send email to user.
        if [ $lsal ]; then
           echo "szaba is running.... " $lsal
        else
	   echo "szaba is not running!!!!!!"
	   # date in central europe and america EDT (like in new york, should be ok is 6h later...)
	   dateE=`date`
	   dateA=`TZ=America/New_York date`

	   # print information on the terminal
	   echo 'Stops at:' $dateE
	   echo 'Stops at:' $dateA
           
	   # craete a temp file with the neccesary information, you can add whatever you want.... 
	   title='szaba stops at:'
           echo -e $title "\n" $dateE "\n" $dateA > temp
	
	   # send email
	   `mail -s "szaba is not running!!!" tomasz.brys@esss.se < temp` 

	   # remove temp file, is not needed any more
	   `rm temp`
	   exit 0
	fi
 
	sleep 5
done 
