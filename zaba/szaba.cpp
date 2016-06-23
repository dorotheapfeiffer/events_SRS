#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <sys/time.h> 
#include <termios.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <DMadc32.h>
#include <DV1718.h>
#include <DMadc32.h>


static struct termios g_old_kbd_mode;

void		ShowData();
int		CheckKeyboard();
static long 	get_time();
static void 	cooked(void);
static void 	raw(void);
int 		kbhit();
int 		getch(void);


using namespace std;


int main(int argc, char **argv) {


DMadc32 dMadc32((char*)"MultiGrid",   0xd0000000);
DV1718  dV1718((char*)"VME_USB_Bridge",   0x00000000);

dMadc32.InitModule();

dMadc32.LoadConfig();

dMadc32.StartAcq();
dV1718.StartAcq();
 

std::time_t result = std::time(0);
cout << "Acquisition start " << std::asctime(std::localtime(&result)) << endl;


while( CheckKeyboard() ){

   dMadc32.ReadVME() 
   dV1718.ReadVME();

   dMadc32.SaveData();
   
   ShowData();
   std::this_thread::sleep_for(std::chrono::milliseconds(500));

}

result = std::time(0);
cout << "Acquisition stop " << std::asctime(std::localtime(&result)) << endl;

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
void ShowData(){

 static uint64_t CurrentTime, PrevRateTime, ElapsedTime;
 CurrentTime = get_time();
 ElapsedTime = CurrentTime - PrevRateTime;

 if (ElapsedTime < 1000)
 return; 
 
 cout << "Tu se bede cos wypisywal" << endl; 

 PrevRateTime = CurrentTime;

}
//===============================================================================
static long get_time() {
    long time_ms;
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
    return time_ms;
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






