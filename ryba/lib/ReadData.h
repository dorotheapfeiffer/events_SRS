#ifndef ReadData_h
#define ReadData_h

#include "AEvent.h"
#include "ATrack.h"

#include <TObject.h>
#include <TString.h>



class ReadData : public TObject {

        std::vector< AEvent *>  	 allEvents;
        std::vector< AEvent *>::iterator it;


public:
        ReadData() 		{ std::cout << "Normal constructor called..." << std::endl; }
        ReadData( TString ) ;
	virtual ~ReadData();

	AEvent* GetEvent(Int_t nr){ return allEvents.at( nr ); }

        Int_t   GetNrEvents(){  return allEvents.size(); }


ClassDef(ReadData, 1) // class to read and keep in vector actual data in memory
};

#endif

