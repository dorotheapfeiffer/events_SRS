#include <stdlib.h>
#include <iostream>
#include <string.h>

/// Root Includes
#include <TObject.h>
#include <TTimer.h>
#include <TSystem.h>

#include "AEvent.h"
#include "ATrack.h"
#include "ReadData.h"
#include "ReadDataNet.h"
#include "ReadDataFile.h"
#include "MainWindow.h"
#include "UserClass.h"


using namespace std;

ClassImp(ReadData)

//============================================================

Int_t ReadData::mReadData = 0;
Int_t ReadData::mRefresh  = 0;
Int_t ReadData::mBreakLoop= 0;

ReadData* ReadData::Factory(const std::string& type, const std::string& name) throw(ReadData::BadDigitizerCreation) {
 if(type == "FILE")      { return new ReadDataFile( name );     }
 if(type == "NET")       { return new ReadDataNet( name );     }
 throw BadDigitizerCreation(type);
}

//============================================================

ReadData::~ReadData(){
   for(it = allEvents.begin(); it != allEvents.end(); ++it){
      delete *it;
      *it = 0;
     }
   allEvents.clear();
}
