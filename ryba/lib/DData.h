#ifndef DData_h
#define DData_h

#include "AEvent.h"
#include "ATrack.h"

#include <TObject.h>
#include <TString.h>



class DData : public TObject {

        std::vector< AEvent *>  	 allEvents;
        std::vector< AEvent *>::iterator it;


public:
        DData() 		{ std::cout << "Normal constructor called..." << std::endl; }
        DData( TString ) ;
	virtual ~DData();

	AEvent* GetEvent(Int_t nr){ return allEvents.at( nr ); }

        Int_t   GetNrEvents(){  return allEvents.size(); }


ClassDef(DData, 1) // class to read and keep in vector actual data in memory
};

#endif

