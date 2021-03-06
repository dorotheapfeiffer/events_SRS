#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <sys/time.h> 
#include <termios.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include "DAcquisition.h"
#include "DKeyboard.h"

std::string exec(char*);


using namespace std;

std::string g_Path; // global path of working directory

Int_t VME_CRATE = 1; 
DKeyboard *dKeyboard = &DKeyboard::GetInstance();

int main(int argc, char **argv) {


 g_Path = string(get_current_dir_name());	 
 std::cout << "path = " << g_Path << endl;
 //g_Path = exec((char*)"pwd");

 DAcquisition	*dAcquisition = new DAcquisition("MultiGrid");

 dAcquisition->StartAcq();

 while( dKeyboard->CheckKey() and dAcquisition->CheckCondition() ){
   dAcquisition->ReadVME(); 
   if(dKeyboard->m_display) dAcquisition->GnuplotOnline();
   //dAcquisition->GnuplotOnline();
   dAcquisition->BuildEvent();
   dAcquisition->DataSave();
   dAcquisition->ShowData(); // no arguments because we do not use graphics in the szaba version, prints only on the terminal
   int delay = 10; 
   std::this_thread::sleep_for(std::chrono::microseconds(10000*delay));

 }

 dAcquisition->StopAcq();
 delete dAcquisition;

return 0;
}

//=============================================================================
std::string exec(char* cmd){
   FILE* pipe = popen(cmd, "r");
   if(!pipe) return "ERROR";

   char buffer[128];
   std::string result = "";
   while(!feof(pipe)){
       if(fgets(buffer, 128, pipe) != NULL)
         result += buffer;
       }
   pclose(pipe);
   result = result.substr(0, result.length()-1);
   return result;
}







