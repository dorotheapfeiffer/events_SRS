 /*\brief
 *********************************************************************************************
 ********************************************************************************************/


#ifndef DKeyboard_h
#define DKeyboard_h

#include <sys/time.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <TObject.h>

using namespace std;

class DKeyboard : public TObject {

	DKeyboard(){
	 m_ch[0] = 32;
	 for(int i = 1; i < 64; i++)
            m_ch[i] = 0;		 
	 for(int i = 1; i < 8; i++)
            m_group[i] = 0;

	 m_start	= 0;
	 m_quit 	= 0;
 	 m_display 	= 0;
	 m_zoom 	= 0;
	 m_start 	= 0;
	}
	~DKeyboard(){}

	DKeyboard( const DKeyboard& );	//!< disable copy contructor
	DKeyboard operator=( DKeyboard );	//!< disable =operator




public:
	static DKeyboard& GetInstance(){
                static DKeyboard m_keyboard; 
                return m_keyboard;
                }

	int          CheckKey();
	static void  cooked(void); // must be static!
	void         raw(void);
	int          kbhit();
	int          getch(void);

	int m_start;
	int m_quit;
	int m_display;
	int m_ch[64];
	int m_group[8];
	int m_zoom;
	int m_restart;

ClassDef(DKeyboard, 0)

};

#endif

