#include <iostream>

#include <TApplication.h>
#include "AManager.h"

using namespace std;

/*\brief  
 * variable used for debugging
 * for the final compilation remove the DEBUG flag from the makefile.mk
 * the bigest degug value the more information is printed in the console
 *
 */

int gDEBUG = 5;			//!< 
int gDEBUG_CAEN = 5;			//!< 
int gDEBUG_CONTROL = 3;		//!< debuging in the acq_control class
int gDEBUG_SAVE = 0;		//!< debuging in the data_save class
int gDEBUG_ONLINE = 0;		//!< debuging in the online server class
int gDEBUG_MAIN = 5;		//!< debuging in the mainFRAME
int gDEBUG_MANAGER = 5;		//!< debuging in the mainFRAME
int gDEBUG_EVENT = 0;		//!< debuging in the event class
int gDEBUG_DISPLAY = 0;		//!< debuging in the display class

int main(int argc, char **argv) {

 TApplication theApp("App",&argc,argv);

  AManager* aManager = &AManager::GetInstance();
  aManager->CreateGui();

theApp.Run();
return 0;

}



