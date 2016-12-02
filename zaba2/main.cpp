#include <iostream>

#include <TApplication.h>
#include <TSystem.h>

#include "DAcquisition.h"
#include "DGAcquisition.h"

void setbit(UShort_t *, Int_t, Int_t){}

using namespace std;
std::string g_Path; // global path of working directory

int VME_CRATE = 0; // this global variable is to run the software without VME crate, just for testing
		     // normaly should be = 1, for test set it to 0

int main(int argc, char **argv) {

 TApplication theApp("App",&argc,argv);
 g_Path = gSystem->WorkingDirectory();

 DAcquisition *dMultiGrid = new DAcquisition("MultiGrid"); 
 new DGAcquisition( dMultiGrid );

 theApp.Run();
return 0;

}


