/*****************************************************************************
*
*****************************************************************************/

#ifndef BuildEvent_h
#define BuildEvent_h


#include <iostream>
#include <string>
#include <exception>

#include <TH1F.h>
#include <TGFrame.h>


using namespace std;

class ABuildEvent: public TQObject{

 
 
public:
	ABuildEvent();
virtual	~ABuildEvent();

ClassDef(ABuildEvent, 0)
};




#endif

