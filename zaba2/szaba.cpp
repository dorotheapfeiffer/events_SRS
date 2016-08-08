#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <sys/time.h> 
#include <termios.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include "DMultiGrid.h"


static struct termios g_old_kbd_mode;

int		CheckKeyboard();
static void 	cooked(void);
static void 	raw(void);
int 		kbhit();
int 		getch(void);
std::string exec(char*);


using namespace std;

std::string g_Path; // global path of working directory

Int_t VME_CRATE = 1; 

int main(int argc, char **argv) {

 g_Path = string(get_current_dir_name());	 
 std::cout << "path = " << g_Path << endl;
 //g_Path = exec((char*)"pwd");

 DMultiGrid	*dMultiGrid = new DMultiGrid("MultiGrid");

 dMultiGrid->StartAcq();

 while( CheckKeyboard() and dMultiGrid->CheckCondition() ){
   dMultiGrid->ReadVME(); 
   dMultiGrid->BuildEvent();
   //dMultiGrid->DataSave();
   //dMultiGrid->ShowData(NULL); // null because we do not use graphics in the szaba version, prints only on the terminal
   
   //std::this_thread::sleep_for(std::chrono::microseconds(1000));

 }

 dMultiGrid->StopAcq();
 delete dMultiGrid;

return 0;
}


//===============================================================================
int CheckKeyboard(){

   int c = 0;

   if(!kbhit())
        return 1;

   c = getch();
   if(c == 'q') return 0;
   else return 1; 
}

//===============================================================================

int getch(void) {
        unsigned char temp;

        raw();
    /* stdin = fd 0 */
        if(read(0, &temp, 1) != 1)
                return 0;
        return temp;

}
//===============================================================================
int kbhit() {

        struct timeval timeout;
        fd_set read_handles;
        int status;

        raw();
    /* check stdin (fd 0) for activity */
        FD_ZERO(&read_handles);
        FD_SET(0, &read_handles);
        timeout.tv_sec = timeout.tv_usec = 0;
        status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
        if(status < 0)
        {
                printf("select() failed in kbhit()\n");
                exit(1);
        }
    return (status);
}

//===============================================================================
static void cooked(void)
{
        tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}

//===============================================================================
static void raw(void) {
        static char init;
/**/
        struct termios new_kbd_mode;

        if(init)
                return;
/* put keyboard (stdin, actually) in raw, unbuffered mode */
        tcgetattr(0, &g_old_kbd_mode);
        memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
        new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
        new_kbd_mode.c_cc[VTIME] = 0;
        new_kbd_mode.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &new_kbd_mode);
/* when we exit, go back to normal, "cooked" mode */
        atexit(cooked);

        init = 1;
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








