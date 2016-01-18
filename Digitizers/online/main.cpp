#include <iostream>

#include <TApplication.h>
#include <TROOT.h>
#include "mySpy.h"

using namespace std;


int main(int argc, char **argv) {

 TApplication theApp("App",&argc,argv);

 new mySpy();

   cout << "Hello 50 "<< endl;
theApp.Run();
return 0;

}



