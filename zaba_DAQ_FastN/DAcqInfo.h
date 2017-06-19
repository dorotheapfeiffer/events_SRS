 /*\brief
 *********************************************************************************************
 ********************************************************************************************/


#ifndef DAcqInfo_h
#define DAcqInfo_h

#include <sys/time.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <TObject.h>

using namespace std;

class DAcqInfo : public TObject {

	DAcqInfo(){
	}
	~DAcqInfo(){}

	DAcqInfo( const DAcqInfo& );	//!< disable copy contructor
	DAcqInfo operator=( DAcqInfo );	//!< disable =operator




public:
	static DAcqInfo& GetInstance(){
                static DAcqInfo m_dacqinfo; 
                return m_dacqinfo;
                }



ClassDef(DAcqInfo, 0)

};

#endif

