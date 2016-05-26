#include <iostream>

#include <TApplication.h>
#include <TSystem.h>

#include "DMultiGrid.h"
#include "DGMultiGrid.h"

void setbit(UShort_t *, Int_t, Int_t){}

using namespace std;

int main(int argc, char **argv) {

 TApplication theApp("App",&argc,argv);

 new DGMultiGrid( new DMultiGrid());

 theApp.Run();
return 0;

}


