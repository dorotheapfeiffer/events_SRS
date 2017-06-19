/*****************************************************************************
*
*****************************************************************************/
#include <TCanvas.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TH1.h>
#include <TLine.h>
#include <TMultiGraph.h>
#include <TTimer.h>
#include <TObjArray.h>
#include <sys/time.h>

#include <iostream>
#include <string>

#include "AManager.h"
#include "ADisplay.h"
//#include "ARingBuffer.h"
#include "ATrack.h"

using namespace std;

 Int_t	aValues[100];
extern int gDEBUG_DISPLAY ;

ClassImp(ADisplay)

//===============================================================================
ADisplay::ADisplay(TCanvas *aCanvas) {
 #ifdef DEBUG
   if(gDEBUG_DISPLAY > 2) cout << "[ADisplay::ADisplay] " << endl;
  #endif
 
 aManager = &AManager::GetInstance();
 mCanvas = aCanvas;

 upperPad = new TPad("upperPad", "upperPad", .005, .105, .995, .995);
 lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .100);

 mCanvas->cd();
 upperPad->Draw(); 			       
 lowerPad->Draw();
 
 upperPad->cd(); 
 h = new TH1F("h","h",1024, 0, 1024);
 h->Draw(" ");
 h->SetAxisRange(-1000, 1000, "y");
   //h = upperPad->DrawFrame(0, -aManager->GetMaxRange() * 1.1 / 2., aManager->GetMaxSamples(), aManager->GetMaxRange() * 1.1 / 2., "TRACKS");
 upperPad->Update();

 lowerPad->cd();

 for(Int_t i = 0; i < nrFrame; i++){   
      aPaveText[i] = new TPaveText(float(i)/nrFrame, 0, float(i)/nrFrame + 1./nrFrame, 1);
      aPaveText[i]->AddText("           ");
      aPaveText[i]->Draw(" ");
      aPaveText[i]->SetFillColor(32);
      aPaveText[i]->SetTextColor(1);
      aPaveText[i]->SetBorderSize(1);
   }

 lowerPad->Update();

 mMemList = new TObjArray();

 mThresholdLine = 0;
 mThresholdText = 0;


 mMultiGraph = 0;
 mGraphs = 0; 		
 
 aNrGraphs = 0; 	// previous
 aMaxSample = 0;


 preTracks	= 0;
 preSamples	= 0;
 preRange	= 0;
 preThreshold	= 0;


 mRefreshGraph = 0;
 mRefreshData  = 0;

 mElapsedTime = 0;
 mCurrentTime = 0;
 mPrevRateTime = 0;

 mPrevEvents = 0;
 mPrevSize   = 0;
 
 fTimer = new TTimer(this, 80);
 fTimer->TurnOn();
}

//===============================================================================

ADisplay::~ADisplay(){
 #ifdef DEBUG
   if(gDEBUG_DISPLAY > 2) cout << "DEBUG [ADisplay::~ADisplay] " << endl;
  #endif
 delete mMultiGraph;
   if(h){ 
     delete h;
     h = 0;
     }

   for(Int_t i = 0; i < nrFrame; i++){
       if(aPaveText[i]){ 
          delete aPaveText[i];
          aPaveText[i] = 0;
          }
       }

   if(upperPad) {
      delete upperPad;
      upperPad = 0;
      }

   if(lowerPad) {
      delete lowerPad;
      lowerPad = 0;
      }

  mMemList->Delete();  

   mCanvas = 0;
   fTimer->TurnOff();
   delete fTimer;

}
//===============================================================================
void ADisplay::UpdateGraph(AEvent *aEvent){
 #ifdef DEBUG
   if(gDEBUG_DISPLAY > 2) cout << "[ADisplay::UpdateGraph] " << endl;
  #endif

 // if(aEvent == NULL){
 //   cout << "aEvent in Display == NULL" << endl;
 //   return;
  //  }

 
  //if( mRefreshGraph || aManager->GetMode() == MainFrame::sOSCI){  
  if( mRefreshGraph ){  
  
     UInt_t aTracks = InitGraphs(aEvent); // This function is always call before updateing graphs. In case there is nothing change in settings function return immediately.

     //aEvent.EventShow(); 

     for(UInt_t i = 0; i < aTracks; i++){
        UShort_t *data = aEvent->GetTrack(i)->GetData();
        for(UInt_t j = 0; j <  aEvent->GetTrack(i)->GetDataSize(); j++){
           mGraphs[i]->SetPoint(j, j, (data[j] - 2048) / 2.048);
           //mGraphs[i]->SetPoint(j, j, data[j]);
           }
        }

     UInt_t aa = InitThreshold(); 
     for(UInt_t i = 0; i < aa; i++){
         mThresholdLine[i]->SetY1(aValues[i]); 
         mThresholdLine[i]->SetY2(aValues[i]); 
         mThresholdText[i]->SetY(aValues[i]); 
        }

     upperPad->cd();
     upperPad->Modified();
     upperPad->Update();
  
     mRefreshGraph = 0;
    }




  if(mRefreshData > 10){

    mCurrentTime = GetLongTime();
    mElapsedTime = mCurrentTime - mPrevRateTime;
    lowerPad->cd();

    //------ Box nr 1 ----------
    char aaa[100];
    sprintf(aaa, "%d",aManager->GetEventNr()); 
    aPaveText[0]->Clear(); 
    aPaveText[0]->AddText("Events:            ");
    aPaveText[0]->AddText(aaa);
    //------ Box nr 2 ----------

    time_t time1;
    struct tm * timenow;
    time(&time1);
    time_t t = (time_t) difftime(time1, aManager->GetStartAcqTime());
    timenow = localtime (&t);
    sprintf(aaa, "%d:%02d:%02d", timenow->tm_hour - 1, timenow->tm_min, timenow->tm_sec);
    aPaveText[1]->Clear(); 
    aPaveText[1]->AddText("Acq Time:        ");
    aPaveText[1]->AddText(aaa); 

    //------ Box nr 3 ----------
    ULong_t mCurrentSize = aManager->GetDataSize();
    sprintf(aaa, "%1.2f MB/s",(float)(mCurrentSize - mPrevSize)/((float)mElapsedTime*1048.576f));
    aPaveText[2]->Clear(); 
    aPaveText[2]->AddText("Data Transfer: ");
    aPaveText[2]->AddText(aaa); 

    //------ Box nr 4 ----------
    aPaveText[3]->Clear(); 
    aPaveText[3]->AddText("Trigger Rate:    ");
    ULong_t mCurrentEvent = aManager->GetEventNr();
    sprintf(aaa, "%.2f Hz", (float)(mCurrentEvent - mPrevEvents)*1000.0f/(float)mElapsedTime);
    aPaveText[3]->AddText(aaa); 
  

    lowerPad->Modified();
    lowerPad->Update();
    
    mPrevEvents = mCurrentEvent;
    mPrevSize = mCurrentSize;
    mPrevRateTime = mCurrentTime;
    mRefreshData = 0;
    }

  /*         
  if(!(nrloop % 10)){
     static int i = 0;

     if(aManager->GetMode() == 2){
        aManager->SetLabelEnable(i%2);
        i++;
        }


     lowerPad->cd();
     char aaa[100];
     sprintf(aaa, "%d",aManager->GetEventNr()); 
     aPaveText[0]->Clear(); 
     aPaveText[0]->AddText("events:            ");
     aPaveText[0]->AddText(aaa);

     time_t time1;
     struct tm * timenow;
     time(&time1);
     time_t t = (time_t) difftime(time1, aManager->GetStartAcqTime());
     timenow = localtime (&t);
     sprintf(aaa, "%d:%02d:%02d", timenow->tm_hour - 1, timenow->tm_min, timenow->tm_sec);
     aPaveText[1]->Clear(); 
     aPaveText[1]->AddText("acq time:        ");
     aPaveText[1]->AddText(aaa); 
	    
     sprintf(aaa, "%1.2f",aManager->GetEventNr() / double (t)); 
     aPaveText[2]->Clear(); 
     aPaveText[2]->AddText("events / sec:    ");
     aPaveText[2]->AddText(aaa); 
            
     lowerPad->Modified();
     lowerPad->Update();
	     
             //ofstream fout("acqTimeTest.txt", ios::out | ios::app);
             //fout << aManager->GetEventNr() << "\t" << t << "\t" << aManager->GetEventNr() / double(t) << endl;
             //fout.close();

   }
*/

}


//===============================================================================
UInt_t ADisplay::InitThreshold(){
  #ifdef DEBUG
   if(gDEBUG_DISPLAY > 2) cout << "DEBUG [ADisplay::InitThreshold] " << endl;
  #endif
 

 UInt_t		aThreshold = 0;
 UInt_t		aSamples = 0;

 for(Int_t i = 0; i < aManager->GetNrDigitizers(); i++){ 
    if( !(aManager->GetDigitizer(i)->IsEnabled()) ) continue;
    for(Int_t j = 0; j < aManager->GetDigitizer(i)->GetNrOfGroups(); j++){
       if(aManager->GetDigitizer(i)->GetThresholdMode(j)){
         aValues[aThreshold] = aManager->GetDigitizer(i)->GetThreshold(j); 
         aSamples = aManager->GetDigitizer(i)->GetTrackLength();
         aThreshold++;
         }
       }
   }

 if(aThreshold == preThreshold){
    //cout << "Return without reinitialization" << endl;
    return aThreshold;
   }


 mMemList->Delete();
 if(mThresholdLine) { delete [] mThresholdLine; mThresholdLine = 0; }
 if(mThresholdText) { delete [] mThresholdText; mThresholdText = 0; }
 
 mThresholdLine = new TLine* [ aThreshold ];
 mThresholdText = new TText* [ aThreshold ];

 upperPad->cd();

 char name[20];
 for(UInt_t i = 0; i < aThreshold; i++){
    mThresholdLine[i] = new TLine(0, aValues[i], aSamples, aValues[i]);
    mThresholdLine[i]->SetLineStyle(7);
    mThresholdLine[i]->SetLineColor(0);
    mThresholdLine[i]->SetLineWidth(1);
    mThresholdLine[i]->Draw();   
    mMemList->Add(mThresholdLine[i]);

    sprintf(name,"T%d",i);
    mThresholdText[i] = new TText(aSamples, aValues[i], name);
    mThresholdText[i]->SetTextColor(0);
    mThresholdText[i]->SetTextSize(0.06);
    mThresholdText[i]->SetTextAlign(12);
    mThresholdText[i]->Draw();
    mMemList->Add(mThresholdText[i]);
  }
 
   upperPad->Modified();
   upperPad->Update();

   preThreshold = aThreshold;
 
    return aThreshold;
}
//===============================================================================

UInt_t ADisplay::InitGraphs(AEvent *aEvent){
   
 #ifdef DEBUG
   if(gDEBUG_DISPLAY > 2) cout << "DEBUG [ADisplay::InitGraphs] " << endl;
  #endif

 UInt_t aSamples	= 0;
 Int_t aRange		= 1500;
 UInt_t aTracks		= 0;

 if(aEvent){
    //cout << "InitGraph, Event data" << endl;
    aTracks = aEvent->GetNrTracks();
    for(UInt_t i = 0; i < aTracks; i++){     
       if( aSamples < aEvent->GetTrack(i)->GetDataSize() ){ 
           aSamples = aEvent->GetTrack(i)->GetDataSize(); 
          }
       }
    }
 else{
    //cout << "InitGraph, Event NULL" << endl;
   aTracks = 0; 
   for(Int_t i = 0; i < aManager->GetNrDigitizers(); i++){ 
       if( !(aManager->GetDigitizer(i)->IsEnabled()) ) continue;
       if( aSamples < static_cast<UInt_t>(aManager->GetDigitizer(i)->GetTrackLength() ))
           aSamples = aManager->GetDigitizer(i)->GetTrackLength();
       }      
   }

 // if nr of tracks and samples are the same as the previous event return immediately
 if(aTracks == preTracks && aSamples == preSamples ){
   // cout << "InitGraph, Return without reinitialization" << endl;
   return aTracks;	  
   } 

 //cout << "aTracks  = " << aTracks  << endl;
 //cout << "aSamples = " << aSamples << endl;
 // there are changes, the easiest is to delete everything and re-initialization again
 // delete graphs and multigraph 
 if(mMultiGraph){
   delete mMultiGraph;
   if(mGraphs) { delete [] mGraphs; mGraphs = 0;}
   }
 mMultiGraph = new TMultiGraph();
 
 // delete Threshold line and thereshold text

 // Create graphs and multigraph
 mGraphs = new TGraph* [ aTracks ];

 upperPad->cd();
 
 delete h;
 h = new TH1F("h","h",aSamples, 0, aSamples);
 h->Draw();
 h->SetAxisRange(-aRange, aRange, "y");
 preThreshold = 0;

 char name[20];
 for(UInt_t i = 0; i < aTracks; i++){
    sprintf(name,"g_%d",i);
    mGraphs[i] = new TGraph( aEvent->GetTrack(i)->GetDataSize() );
    //cout << " DEBUG [MainFrame::InitGraph] create graphs  [" << name << "]\t" << "nr points = " << aEvent.GetTrack(i)->GetDataSize() << endl;;
    //for(Int_t j = 0; j < aEvent->GetTrack(i)->GetDataSize(); j++){
    //    mGraphs[i]->SetPoint(j, j, 100+10*i);
    //   }
    mGraphs[i]->SetName(name);
    mGraphs[i]->SetMarkerColor(i+2);
    mGraphs[i]->SetLineColor(i+2);
    mGraphs[i]->SetMarkerStyle(21);
    mGraphs[i]->SetMarkerSize(0.3);

    mMultiGraph->Add(mGraphs[i],"lp");
    }
   
   mMultiGraph->Draw();

   upperPad->Modified();
   upperPad->Update();


   preTracks    = aTracks;
   preSamples   = aSamples;
   preRange     = aRange;

 return aTracks;
}


//===============================================================================

AEvent* ADisplay::GenerateEvent(){

 #ifdef DEBUG
   if(gDEBUG_DISPLAY > 2) cout << "DEBUG [ADisplay::GenerateEvent] " << endl;
  #endif

 mEvent.ClearEvent();

 Int_t aXLength = 0;

 for(Int_t i = 0; i < aManager->GetNrDigitizers(); i++){ 
    if( !(aManager->GetDigitizer(i)->IsEnabled() ) ) continue;

    // calculate the y-axis
    if( aXLength  < aManager->GetDigitizer(i)->GetTrackLength() )
        aXLength = aManager->GetDigitizer(i)->GetTrackLength();
   } 
 
Short_t data[ aXLength ];
for(Int_t i = 0; i < aXLength; i++)
        data[i] = 0;


 for(Int_t i = 0; i < aManager->GetNrDigitizers(); i++){ 
    if( !(aManager->GetDigitizer(i)->IsEnabled()) ) continue;

    // this part is if we have group channels
    for(Int_t j = 0; j < aManager->GetDigitizer(i)->GetNrOfGroups(); j++){
       if(aManager->GetDigitizer(i)->GetActiveGroup(j)){
          Int_t threshold =  aManager->GetDigitizer(i)->GetThreshold(j) ;
          mEvent.AddTrack( new ATrack(i, j, 1./65e6, 0, 1000, 0, threshold, data, aXLength) );
          }
       }
    // this part is if we have channels
    //for(Int_t j = 0; j < aManager->GetDigitizer(i)->GetNrOfChannels(); j++){
    //   if(aManager->GetDigitizer(i)->GetActiveChannel(j)){
    //      aEvent->AddTrack( new ATrack(i, j, 0, 0, 1000, 0, 0, (Short_t *)data, aManager->GetMaxSamples()) );
    //      }
    //   }
    
    }

return &mEvent;
}

//=========================================================================
//

Bool_t ADisplay::HandleTimer(TTimer *){
  
  mRefreshGraph++;
  mRefreshData++;

return kTRUE;
}

//===============================================================================


ULong_t ADisplay::GetLongTime(){

 struct timeval t1;
 struct timezone tz;
 gettimeofday(&t1, &tz);
 
return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}
/*
 Int_t aXLength = 0;
 Int_t aYLength = 0;
 Int_t aNrTracks = 0;

 for(Int_t i = 0; i < aManager->GetNrDigitizers(); i++){ 
    if( !(aManager->GetDigitizer(i)->IsEnabled() ) ) continue;

    // calculate the y-axis
    for(Int_t j = 0; j < aManager->GetDigitizer(i)->GetNrOfChannels(); j++){
        if( aYLength  < aManager->GetDigitizer(i)->GetRange(j) )
            aYLength = aManager->GetDigitizer(i)->GetRange(j);
       }

    if(aEvent == NULL){ // calculate the x-axis
       if( aXLength  < aManager->GetDigitizer(i)->GetTrackLength() )
           aXLength = aManager->GetDigitizer(i)->GetTrackLength();
       
       // calculate nr of tracks
       Int_t group = aManager->GetDigitizer(i)->GetNrOfGroups(); 
       if( group )
          aNrTracks += group;
       else
          aNrTracks += aManager->GetDigitizer(i)->GetNrOfChannels();
       
    }
   else{
       aXLength = aEvent->GetNrTracks();
       for(UInt_t i = 0; i < aXLength; i++){     
           if( aXLength < aEvent->GetTrack(i)->GetDataSize() ){ 
               aXLength = aEvent->GetTrack(i)->GetDataSize(); 
             }
          }		
      }
  } 


  cout << "Nr of Tracks = " << aNrTracks << endl;
  cout << "X-Length     = " << aXLength << endl;
  cout << "Y-Length     = " << aYLength << endl;
return;

 

*/
