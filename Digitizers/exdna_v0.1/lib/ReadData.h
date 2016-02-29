#ifndef ReadData_h
#define ReadData_h

#include <TString.h>
#include <iostream>
#include <exception>
#include <string>

#include "AEvent.h"
#include "ATrack.h"

#include <TObject.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TRootEmbeddedCanvas.h>

class MainWindow;

class ReadData : public TObject {

protected:
	static int 		mReadData;
	static int 		mRefresh;
	static int		mBreakLoop;

	std::vector< AEvent > 	allEvents;
	TMultiGraph		*mMultiGraph = 0; // new standard c++11
	TH1F			*h;
//---------------------------------------------------------------

	class BadDigitizerCreation : public std::exception {
        		std::string reason;
		public:
        		BadDigitizerCreation(std::string type){
        		reason = "Cannot create type " + type;
        		}

        		~BadDigitizerCreation() throw() {}
        		const char *what() const throw() {
        		return reason.c_str();
        		}
  	};

//---------------------------------------------------------------
public:
	static ReadData* Factory(const std::string&, const std::string&) throw(BadDigitizerCreation);

//---------------------------------------------------------------
	AEvent&	GetEvent(Int_t nr){ return allEvents.at( nr ); }

	Int_t	GetNrEvents(){	return allEvents.size(); }
	
	//Bool_t	HandleTimer(TTimer *);
	//void 	DoRefresh( MainWindow*, UInt_t);
	//void	DrawEvent( TRootEmbeddedCanvas*, Int_t, Int_t);
	//void	Analyse(MainWindow*, Int_t);

	virtual ~ReadData();

ClassDef(ReadData, 0)
};

#endif
