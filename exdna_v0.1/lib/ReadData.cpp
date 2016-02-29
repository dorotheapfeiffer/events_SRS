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
#include "../src/MainWindow.h"
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
/*
void ReadData::Analyse(MainWindow *mMainWindow, Int_t mode){

cout << "DEBUG [ReadData::Analyse] start analysing" << endl;
 
 TTimer aTimer(this, 1000);
 aTimer.TurnOn();
 UInt_t i;
 //UserClass aUserAnalysis; 

    for(i = 0; i < allEvents.size(); i++){
       AEvent &aEvent = allEvents.at(i);
//       cout << "DEBUG [ReadData::Analyse] Analysing of event nr = " << allEvents.at(i).GetEventNr() << "\tloop = " << i << endl;
       mMainWindow->GetUserClass()->DoAnalysis(aEvent);
 
       //gSystem->Sleep(1);
       //--------------------------------------

       gSystem->ProcessEvents();

       //--------------------------------------
       if( mRefresh ){
         DoRefresh( mMainWindow, i);
         mRefresh = 0;
         } 

       //--------------------------------------

       if( mMainWindow->GetLoopCondition() == 0){
         aTimer.TurnOff();	
         break;
         }

       //--------------------------------------
       }

//mMainWindow->SetLoopCondition(0); 
DoRefresh(mMainWindow, i-1);

}
*/
//============================================================
/*
void ReadData::DoRefresh(MainWindow* mMainWindow, UInt_t i){

   //cout << "DEBUG [ReadData::DoRefresh]" << endl;
   DrawEvent(mMainWindow->GetEmbCanvas(), i, 0);
   mMainWindow->UpdateEventNumber(i);
   mMainWindow->UpdateProgressBar((char*)"analysing");

}
*/
//============================================================
/*
void ReadData::DrawEvent(TRootEmbeddedCanvas *aEmbCanvas, Int_t nr, Int_t mode){

   cout << "DEBUG [ReadData::DrawEvent]" << endl;

   TCanvas *c = aEmbCanvas->GetCanvas();
   c->cd();
   c->Clear();
   AEvent &aEvent = allEvents.at(nr);
                    
   Int_t nrTracks = aEvent.GetNrTracks();

//-----------------------------------------------------------
   UInt_t aSamples = 0;
   for(Int_t i = 0; i < nrTracks; i++){     
       if( aSamples < aEvent.GetTrack(i)->GetDataSize() ){
           aSamples = allEvents.at(nr).GetTrack(i)->GetDataSize();
          }
       }
   
//-----------------------------------------------------------

   //if(h) { delete h; h = 0; }
   //h = new TH1F("h","h",aSamples, 0, aSamples);
   //h->Draw();

//-----------------------------------------------------------
   
   if(mMultiGraph){ delete mMultiGraph; mMultiGraph = 0; }
   mMultiGraph = new TMultiGraph();

//-----------------------------------------------------------


   TGraph *gr;
   char grName[100];
   char multiGrName[100];
   sprintf(multiGrName, "Event:%d-", nr);

   for(Int_t i = 0; i < nrTracks; i++){
      sprintf(grName,"Inst:%d-ch:%d.", (Int_t)aEvent.GetTrack(i)->GetCardNr(), (Int_t)aEvent.GetTrack(i)->GetChannelNr());
      strcpy( multiGrName + strlen(multiGrName), grName);
      gr = new TGraph( (Int_t)aEvent.GetTrack(i)->GetDataSize() );

      UShort_t *data = aEvent.GetTrack(i)->GetData();
      for(UInt_t j = 0; j < aEvent.GetTrack(i)->GetDataSize(); j++){
          gr->SetPoint(j, j, data[j]);
          //cout << "data["<<j<<"] = " << data[j] << endl;
          }

      gr->SetName(grName);
      gr->SetMarkerColor(i+2);
      gr->SetLineColor(i+2);
      gr->SetMarkerStyle(21);
      gr->SetMarkerSize(0.3);
 
      mMultiGraph->Add(gr,"lp");
      }

   //sprintf(multiGrName,"Event:%d-%s", nr, multiGrName);
   mMultiGraph->SetTitle(multiGrName);
   mMultiGraph->Draw("a");	
   mMultiGraph->SetMinimum(0);	
   mMultiGraph->SetMaximum(5000);	
   c->Modified();
   c->Update();


}
*/
//============================================================
/* 
Bool_t ReadData::HandleTimer(TTimer *){

  mRefresh = 1;

return kTRUE;
}
*/
//============================================================


ReadData::~ReadData(){
   allEvents.clear();
}
