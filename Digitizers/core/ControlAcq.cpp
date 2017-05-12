#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <time.h>



#include "ControlAcq.h"
#include "MainFrame.h"
#include "AManager.h"
#include "AEvent.h"
#include "ATrack.h"
#include "ADataSave2.h"
#include "ADisplay.h"
#include "ARingBuffer.h"

#include <TMath.h>
#include <TRandom.h>
#include <TObjArray.h>
#include <TStopwatch.h>
//#include <TList.h>

using namespace std;

extern int gDEBUG;
extern int gDEBUG_CONTROL;


ClassImp(ControlAcq)

long get_time()
{
    long time_ms;
#ifdef WIN32
    struct _timeb timebuffer;
    _ftime( &timebuffer );
    time_ms = (long)timebuffer.time * 1000 + (long)timebuffer.millitm;
#else
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
#endif
    return time_ms;
}

Int_t mBytesPerSec ; // for test only
Long_t mPrevRateTime;
Long_t mCurrentTime; 
Long_t mElapsedTime;
Int_t mEventsPerSec;
/* ============================================================================== */



Bool_t  findMinTimeStamp(AEvent* , AEvent* );
//============================================================================================

ControlAcq::ControlAcq( Bool_t a) : mSimOnly(a) {
#ifdef DEBUG
 if(gDEBUG_CONTROL > 0) cout << "DEBUG [ControlAcq::ControlAcq]" << endl;
#endif

aManager = &AManager::GetInstance();
numInst = aManager->GetNrDigitizers();

mNrAllEvents	= 0;
mMaxSegment	= 0;
mAcqStopTimer	= 0;

//mMemList = new TObjArray();
  
for(UInt_t i = 0; i < 100; i++)
     numEvents[ i ] = 0;

 aTimer = new TTimer();
 aTimer->Connect("Timeout()", "ControlAcq", this, "TimerDone()");
} 

//============================================================================================
ControlAcq::~ControlAcq(){
#ifdef DEBUG
  if(gDEBUG_CONTROL > 0) cout << "DEBUG [ControlAcq::~ControlAcq]" << endl;
#endif

  aManager = 0;
  mAggregateEvents.clear();
  aTimer->Disconnect("Timeout()");
  delete aTimer;
}


//============================================================================================
// Run function is divided into two parts, one for OSCI mode second for ACQ mode. In such a case is easier to control all the acquisition
//
void ControlAcq::Run(){
   #ifdef DEBUG
     if(gDEBUG_CONTROL > 0) cout << "DEBUG [ControlAcq::Run], mode = " << aManager->GetMode() << endl;
   #endif

//----- Initialization ----

  Int_t  retval;
  ADisplay *aDisplay = aManager->GetFrame()->GetDisplay();

//----- OSCI mode ------
 StartAllCards();
 aManager->SetStartAcqTime();
 mPrevRateTime = get_time();
 aManager->SetEventNr(0);

 Bool_t allEventsZero = kFALSE;
 Bool_t oneEventZero = kFALSE;
 Bool_t proceedFlag = kFALSE;

 ULong_t	aTimeStamp;
 ULong_t	aMinTimeStamp;
 Int_t		aEventInstrument;
 UInt_t		aLoop = 0; 

 

  while( LoopCondition( 1 ) ){   // transfer data from digitizers to PC memory
    AEvent *aGlobalEvent = NULL;
    for(UInt_t i = 0; i < numInst; i++) {   
       // if( !aManager->GetDigitizer(i)->IsEnabled() ) continue; 
       if(aManager->GetDigitizer(i)->Configure())
          aManager->GetDigitizer(i)->StartAcq();

       numEvents[i] = aManager->GetDigitizer(i)->GetData();  	
       mBytesPerSec += aManager->GetDigitizer(i)->GetDataSize();
       }
    
    for(UInt_t i = 0; i < numInst; i++) {   
       allEventsZero |= (Bool_t) numEvents[i];
       oneEventZero  &= (Bool_t) numEvents[i];  // at least one event!
       }
                       
    if( oneEventZero && proceedFlag ) { 
       proceedFlag = kTRUE; 
       continue; 
       }

    proceedFlag = kFALSE;

    while( allEventsZero ){
	 
	 aMinTimeStamp = -1L;
	 aEventInstrument = -1;
         for(UInt_t i = 0; i < numInst; i++){
            if( numEvents[i] ){   
              aTimeStamp = aManager->GetDigitizer(i)->GetEvent()->GetTimeStamp();
              //cout << "DEBUG [ControlAcq::StartAcq] Name = " << aManager->GetDigitizer(i)->GetName() << " TimeStamp = " << aTimeStamp << endl;
              if(aTimeStamp <= aMinTimeStamp){
                aMinTimeStamp = aTimeStamp;  
                aEventInstrument = i;
                }
              }
           }

  //       if(aEventInstrument == -1) continue;  
     
         aGlobalEvent = aManager->GetDigitizer(aEventInstrument)->GetEvent();
	 aGlobalEvent->SetEventNr( aLoop++ ); 
	 aGlobalEvent->SetTimeStamp(aMinTimeStamp);
         aManager->GetDigitizer(aEventInstrument)->PopEvent(); 
	 numEvents[aEventInstrument]--;	 

         for(UInt_t i = 0; i < numInst; i++){
	     while(numEvents[i]){
                   //cout << "DEBUG [] " << aManager->GetDigitizer(i)->GetEvent()->GetTimeStamp() << " - " << aGlobalEvent->GetTimeStamp() 
                   //     << " = " << aManager->GetDigitizer(i)->GetEvent()->GetTimeStamp()/1000 - aGlobalEvent->GetTimeStamp()/1000 << endl;    
                   Int_t difference = aManager->GetDigitizer(i)->GetEvent()->GetTimeStamp() - aGlobalEvent->GetTimeStamp();
                   if( difference < 220){
                      aGlobalEvent->AddEvent( aManager->GetDigitizer(i)->GetEvent() )  ; 
                      aManager->GetDigitizer(i)->PopEvent(); 
	              numEvents[i]--;	 
                      }
                   else {
                       //cout << "difference = " << difference << endl;
                       break;
                      } 
                  }
            }


         //aGlobalEvent->SaveEvent();
         //aGlobalEvent->EventShow();
	 aManager->SetEventNr( aManager->GetEventNr()+1);
         //cout << "Nr of TRACKS = " << aGlobalEvent->GetNrTracks() << endl;
         aDisplay->UpdateGraph( aGlobalEvent );
         delete aGlobalEvent;
	 aGlobalEvent = NULL;

         // checking if there are still some local events which have to be added to the global event
         allEventsZero = kFALSE;    
         for(UInt_t i = 0; i < numInst; i++)    
            allEventsZero |= (Bool_t) numEvents[i];

    }
         
    UpdateRates();
    gSystem->ProcessEvents();
    gSystem->Sleep(10);    

   }

 StopAllCards();
 
 if( aManager->GetMode() != MainFrame::sACQUISITION ) return;

////////////////////////////////////////////////////////////////////////////////////////////// 
//----- ACQ mode -------

 UInt_t eventNr = 0;
 UInt_t loop = 0;

 //---
 ADataSave2 aDataSave(kTRUE);
 aManager->SetEventNr(0);
 aManager->SetStartAcqTime();

 //---
 if( aManager->GetMaxAcqTime() > 0){
     aTimer->Start(aManager->GetMaxAcqTime() * 1000); // div by 1000 to have it in msec
     cout << "Sstar Timer: " << aManager->GetMaxAcqTime() <<"s\t mAcqStopTimer = " << mAcqStopTimer << endl;
   }
 //---
 StartAllCards();

  while( LoopCondition( 2 ) ){
       for(UInt_t i = 0; i < numInst; i++) {   
          //if( !aManager->GetDigitizer(i)->IsEnabled() ) continue; 

          numEvents[i] = aManager->GetDigitizer(i)->GetData();  		
          if( numEvents[i] ){
             aManager->SetDataSize(aManager->GetDataSize() + aManager->GetDigitizer(i)->GetDataSize());	

             for(UInt_t j = 0; j < numEvents[i]; j++){ 
 
                AEvent *aE = aManager->GetDigitizer(i)->GetEvent();
                aE->SetEventNr( eventNr++ );
 
                if(aManager->GetDisplay())
                   aDisplay->UpdateGraph( aE );
                else{
                  AEvent *aE = NULL;
                  aDisplay->UpdateGraph( aE );
                  }

                aDataSave.SaveInBuffer( *aE );
                aManager->GetDigitizer(i)->PopEvent(); 
                gSystem->ProcessEvents();
	        aManager->SetEventNr( aManager->GetEventNr()+1);
                delete aE;

                }
            }
            else{
              gSystem->ProcessEvents();
            }
    
            UpdateRates();
            gSystem->Sleep(10); 
   
        }
      } 
         

    //  if( IsForceTrigger() )							// in case that software trigger is necessary...
    //    aManager->GetDigitizer(i)->SendSWTrigger();
    //mNrAllEvents = Aggregate();


    //if(Online)  DoOnlineAnalysis;
    //if(Display) DisplayEvent;
    //if(gnuplot) Gnuplot;
    aDataSave.EmptyBuffer();
 StopAllCards();
 aManager->SetFileNr(0);
 
}
//============================================================================================
bool ControlAcq::LoopCondition(Int_t mode){
/*
//  #ifdef DEBUG
//     if(gDEBUG_CONTROL > 0) 
        cout << "  DEBUG [ControlAcq::LoopCondition] " 
             << "\tLoop mode = "        << mode 	            << endl; 
             << "\tmAcqStopTimer = "    << mAcqStopTimer            << endl;     
             << "\tAManager->mAcqMode ="<< aManager->GetMode()      << endl; 
	     << "\tEvents = "           << aManager->GetEvents()    << endl;
             << "\tMaxEvents = "        << aManager->GetMaxEvents() << endl;
        cout << "                                             \tFiles = " << aManager->GetFiles() << "\tMaxFiles = " << aManager->GetMaxFiles() << endl;
//  #endif
*/
//cout << "Dupa jas 1" << endl;


 if(mAcqStopTimer){ 
     return kFALSE; 				// return kFALSE when acquisition timeout happened
   }

//cout << "Dupa jas 2" << endl;
 if( aManager->GetMode() == MainFrame::sSTOP ) { 		// return 0 when acquisition stops by pressing the button
   cout << "[ControlAcq::LoopCondition]:stop mAcqMode = " << aManager->GetMode() <<"\t LoopCondition(mode) = " << mode << endl;     
   return kFALSE; 
   }

//cout << "Dupa jas 3" << endl;
 if( aManager->GetMaxEvents() ){                                // return 0 when user define max number of events
   if( aManager->GetEventNr() > aManager->GetMaxEvents() ){
       //cout << "[ControlAcq::LoopCondition]:stop mMaxEvents = " << aManager->GetMaxEvents() << "eventNr = "<< aManager->GetEventNr() << endl;     
       return kFALSE;
     } 
   }

//cout << "aManager->GetMaxFiles(): " << aManager->GetMaxFiles() << "\taManager->GetFileNr(): " << aManager->GetFileNr() << endl;
 if( aManager->GetMaxFiles() ){					// return 0 when user define max number of files
   //cout << "aManager->GetMaxFiles(): " << aManager->GetMaxFiles() << "\taManager->GetFileNr(): " << aManager->GetFileNr() << endl;
   if( aManager->GetFileNr() >= aManager->GetMaxFiles()){
       //cout << "[ControlAcq::LoopCondition]:stop mMaxFiles = " << aManager->GetMaxFiles() << "fileNr = "<< aManager->GetFileNr() << endl;     
     return kFALSE;
     }
   }

 if( (aManager->GetMode() == MainFrame::sACQUISITION) && mode == 1){
 // cout << "[ControlAcq::LoopCondition]:stop mode = " << mode << endl;
  return kFALSE;
  }
 else if ((aManager->GetMode() == MainFrame::sACQUISITION) && mode == 2){
//  cout << "[ControlAcq::LoopCondition]:run mode = " << mode << endl;
  return kTRUE;
 }


//cout << "Dupa jas 5" << endl;
return kTRUE;
}

//============================================================================================
void ControlAcq::UpdateRates(){

  static int i = 0;
  mEventsPerSec = aManager->GetEventNr(); 
  mCurrentTime = get_time();
  mElapsedTime = mCurrentTime - mPrevRateTime;
  if( mElapsedTime  > 1000L) {
    //printf("DEBUG::::Reading at %.2f MB/s (Trg Rate: %.2f Hz)\n", (float)mBytesPerSec/((float)mElapsedTime*1048.576f), (float)mEventsPerSec*1000.0f/(float)mElapsedTime);
    
    mBytesPerSec = 0;
    mEventsPerSec = 0;
    mPrevRateTime = mCurrentTime;
    aManager->GetFrame()->SetLabelAcq( Bool_t(i%2));
    i++;
    }
 // else if(mElapsedTime > 500L){
 //   aManager->GetFrame()->SetLabelAcq(1);
 //   }

}
//============================================================================================
Int_t ControlAcq::Aggregate(){
     #ifdef DEBUG
       if(gDEBUG_CONTROL > 2)
         cout << "DEBUG [ControlAcq::AgregateEvents] "<< endl;
     #endif

  mAggregateEvents.clear();

  for(UInt_t i = 0; i < numInst; i++) {   
     for(UInt_t j = 0; j < numEvents[i]; j++){
        AEvent *aE = aManager->GetDigitizer(i)->GetEvent();
	mAggregateEvents.push_back(*aE);
        }
     }

  for(UInt_t i1 = 0; i1 < numInst; i1++)
     numEvents[ i1 ] = 0;
 
 return mAggregateEvents.size();

/*
// just for test......

  const Int_t pvDataSize = 1024;
  Short_t pvData[pvDataSize];
  Int_t channels = 5;
  Int_t range = 9000;
  Int_t sigma = 10;

  TRandom  r;

   for(Int_t i = 0; i < channels; i++){
      double amp = gRandom->Rndm() * range;
      double sig = (0.5 + gRandom->Rndm()) * sigma;

      for(Int_t k3 = 0; k3 < pvDataSize; k3++){
         double y = TMath::Landau(k3, pvDataSize/2, sig) * amp;
         double noise = (gRandom->Rndm() - 0.5) * 10.;
          
         pvData[k3] = static_cast<Short_t>( y + noise );
         }
      aEvent.AddTrack(new ATrack(0, i, 1e-5, 0, range, pvData, pvDataSize));
      }

// end test......
*/



/*
 for(Int_t i = 0; i < mMaxSegment; i++){
    for(Int_t j = 0; j < mCards; j++){ if(aManager->GetDigitizer(j)->IsEnabled()){
       for(Int_t k = 0; k < aManager->GetDigitizer(j)->GetNrOfChannels(); k++){ 
            if(aManager->GetDigitizer(j)->GetActiveChannel(k)){
              aEvent->AddTrack( aManager->GetDigitizer(j)->GetTrack(i,k) );
              }
            }
       }}
    aEvent->SetEventNr(mLoopNr);
 
     #ifdef DEBUG
       if(gDEBUG_CONTROL > 3){
         cout << "DEBUG [ControlAcq::BuildEvent::nr_of_tracks]  = " << aEvent->GetNrTracks() << endl; 
	 //for(Int_t i = 0; i < aEvent->GetNrTracks(); i++)
         //cout << "...............................   data_size]  = " << aEvent->GetTrack(i)->GetDataSize() << endl;
         }
     #endif

   // Short_t* data0 = aEvent->GetTrack(0)->GetData();
   // Short_t* data1 = aEvent->GetTrack(1)->GetData();
   // Short_t* data2 = aEvent->GetTrack(2)->GetData();
   // Short_t* data3 = aEvent->GetTrack(3)->GetData();

   // ofstream fout("aTrackTest.txt", ios::out);
   // for(int i = 0; i < aEvent->GetTrack(0)->GetDataSize(); i++){
   //    fout << i << " " << data0[i] << " " << data1[i] << " " << data2[i] << " " << data3[i] << endl;
   //    }
   // fout.close();/


  //  ofstream fout("aTrackTest.txt", ios::out | ios::app);
  //  aEvent->Write2F(fout);
  //  fout.close();


    if(aOnlineBuffer){ 
       #ifdef DEBUG
         if(gDEBUG_CONTROL > 2)
           cout << "DEBUG [ControlAcq::BuildEvent] write to OnlineBuffer"<< endl;
       #endif
       //if(!aOnlineBuffer)
       //aOnlineBuffer = aManager->GetOnlineBuffer(); 
       //aOnlineBuffer->push_front_if_not_full(*aEvent);     // this in both states
      } 

    if( aSaveBuffer ){ 
       #ifdef DEBUG
         if(gDEBUG_CONTROL > 2)
           cout << "DEBUG [ControlAcq::BuildEvent] write to SaveBuffer"<< endl;
       #endif
       //aSaveBuffer->push_front(*aEvent); 		   // only in acq state
      } 


    if(aManager->GetDisplay()){
       #ifdef DEBUG
         if(gDEBUG_CONTROL > 2)
            cout << "DEBUG [ControlAcq::BuildEvent] write to DisplayBuffer"<< endl;
       #endif
           //aDisplayBuffer->push_front_if_not_full(*aEvent); // this should be during acq  state
           //aDisplayBuffer->push_front(*aEvent);             // this should be during osci state

	//gSystem->Sleep(1000);
     }


     aEvent->ClearEvent();
    }
*/
return 0;
}

//============================================================================================
/*
void ControlAcq::DeleteAcq(){
     #ifdef DEBUG
       if(gDEBUG_CONTROL > 0)
          cout << "DEBUG [ControlAcq::DeleteAcq]" << endl;
     #endif

}
*/
//============================================================================================

void ControlAcq::StopAllCards(){

     #ifdef DEBUG
       if(gDEBUG_CONTROL > 0) cout << endl << endl << "DEBUG [ControlAcq::StopAllCards]" << endl;
     #endif
 for(UInt_t i = 0; i < numInst; i++) {    
     aManager->GetDigitizer(i)->StopAcq();
     }
}

//============================================================================================

void ControlAcq::StartAllCards(){

 #ifdef DEBUG
   if(gDEBUG_CONTROL > 0) cout << endl << endl << "DEBUG [ControlAcq::StartAllCards]" << endl;
 #endif
 
 for(UInt_t i = 0; i < numInst; i++) {    
     aManager->GetDigitizer(i)->Configure();  	
     }
 for(UInt_t i = 0; i < numInst; i++) {    
     aManager->GetDigitizer(i)->StartAcq();
     }
}

//============================================================================================
/*
Int_t ControlAcq::WaitUntilReady(){
     #ifdef DEBUG
       if(gDEBUG_CONTROL > 0) TThread::Printf("DEBUG [ControlAcq::WaitUntilReady] ");
     #endif
return 0;
}
*/
//============================================================================================
/*
Int_t ControlAcq::Stop(Int_t timeout_ms){
     #ifdef DEBUG
       if(gDEBUG_CONTROL > 0)
         cout << "DEBUG [ControlAcq::StopAcq] "<< endl;
     #endif

return 0;
}
*/
//============================================================================================
/*
Int_t ControlAcq::IsBadEvent(){

return 0;
}
*/
//============================================================================================   
Bool_t ControlAcq::TimerDone(){

  mAcqStopTimer = 1;
  aTimer->TurnOff();
  cout << "DEBUG [ControlAcq::TimerDone] mAcqStopTimer = "<<  mAcqStopTimer << endl;
return kTRUE;
}

//============================================================================================   
Bool_t  findMinTimeStamp(AEvent* a, AEvent* b){
 return a->GetTimeStamp() > b->GetTimeStamp();
}
