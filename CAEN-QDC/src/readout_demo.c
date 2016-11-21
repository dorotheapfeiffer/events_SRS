/******************************************************************************
* 
* CAEN SpA - Front End Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* +390594388398 - www.caen.it
*
******************************************************************************/

#include "dpp_qdc.h"

void usage() {
  printf("Syntax : readout_demo <acquisition_runtime (seconds)>\n");
  printf("acquisition_runtime must be an integer > 0 and <long number\n");
}

/* ============================================================================== */
/* main                                                                           */
/* ============================================================================== */
int main(int argc, char* argv[])
{
    int ret;
    int acquisition_runtime = 0; /* 0 = infinite run (exit with 'q') */
    int          running    = 0;


    /* Command line processing */
    if (argc == 2) 
    {
       acquisition_runtime = atoi(argv[1]) * 1000;
       if (acquisition_runtime < 0) {
         usage();
         exit(-1);
       }
       if (errno == ERANGE) {
         perror("acquisition_runtime parameter too big:\n");
         exit(-1);
       }
     }

    /* Init statistics */
    gAcqStats.TotEvCnt = 0;
    gAcqStats.nb       = 0;

    running        = 1;

	/* Board configuration prior to run */
    ret = setup_acquisition();
    if (ret) {
      printf("Error during acquisition setup (ret = %d) .... Exiting\n", ret);
	  running = 0;
	}

    /* Init time global variables */
    gCurrTime      = get_time();
    gRunStartTime  = gCurrTime;
    gPrevTime      = gCurrTime;
    gPrevHPlotTime = gCurrTime;
    gPrevWPlotTime = gCurrTime;


    /* Readout Loop */
    while(running) {

        if (gRestart) {
           gRestart = 0;
           if(load_configuration_from_file(CONFIG_FILE_NAME, &gParams)) {
             printf("Error during acquisition restart: configuration file %s reload failed! .... Exiting\n", CONFIG_FILE_NAME);
             break;
           }
            
                         
           if(setup_acquisition()) {
             printf("Error during acquisition restart: acquisition setup failed! .... Exiting\n");
             break;
           }
                                                 
           gAcqStats.nb = 0;
           gLoops = 0;

        }
 
        /* Get data */
        ret = run_acquisition();
        if (ret) { 
          printf("Error during acquisition loop (ret = %d) .... Exiting\n", ret);
          break;
        }

        /* Periodic console update -... could be a separate thread .... */
        print_statistics();

        /* 
        ** Check acquisition run time.
        ** exit from loop if programmed time is elapsed.
        */
        if (acquisition_runtime)
           if (gAcqStats.gRunElapsedTime > acquisition_runtime)
              running = 0;

        /* Periodic keyboard input -... could be a separate thread .... */
        ret = check_user_input();
        if (ret) running = 0;



    } /* end of readout loop */

    /* Final memory cleanup */
    cleanup_on_exit();

    return 0;
}
