#include "DKeyboard.h"

using namespace std;

static struct termios g_old_kbd_mode;

ClassImp(DKeyboard)

//==================================================================================
int DKeyboard::CheckKey(){

   int c = 0;
   int temp = 0;

   if(!kbhit())
      return 1;

   c = getch();
   //===============================================
   if(c == 'q') 
      return 0;
   //===============================================
   else if(c == 'd'){
     m_display = m_display ? 0 : 1;
      return 1;
   }
   //===============================================
   else if(c == 'g'){
      printf("Group = ");
      scanf("%d", &temp);
      if(temp >= 0 && temp < 8){
         for(int i = 0; i < 64; i++){
            if (i == temp) m_ch[i] = 1;
	    else           m_ch[i] = 0;
         }	
      }
      m_group[temp] = m_group[temp] ? 0 : 1;
      return 1;
   }//==============================================
   else if(c == 'c'){
      printf("Channel = ");
      scanf("%d", &temp);
      if(temp >= 0 && temp < 64){
         for(int i = 0; i < 8; i++){
            m_group[i] = 0;
         }	 
         m_ch[temp] = m_ch[temp] ? 0 : 1;
      } 
      else
         for(int i = 0; i < 64; i++){
            m_ch[i] = 0;
         }	 

      return 1;
   }//==============================================
   else if(c == 'z') 
	   m_zoom = 1;
   //===============================================
   else if(c == 'r') 
	   m_restart = 1;
   //===============================================
   else if(c == 's') 
	   m_start = m_start?  0 : 1;
   //===============================================
   else return 1;
}
//==================================================================================
int DKeyboard::getch(void) {

   unsigned char temp;

   raw();
   /* stdin = fd 0 */
   if(read(0, &temp, 1) != 1)
   return 0;
   
return temp;
}
//===============================================================================

int DKeyboard::kbhit() {

   struct timeval	timeout;
   fd_set		read_handles;
   int			status;

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
void DKeyboard::cooked(void) {
   tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}

//===============================================================================

void DKeyboard::raw(void) {

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
   atexit( cooked );
											        
   init = 1;
}
//=============================================================================
//

