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

//mMemList = new TObjArray();
  
for(UInt_t i = 0; i < 100; i++)
     numEvents[ i ] = 0;

} 

//============================================================================================
ControlAcq::~ControlAcq(){
#ifdef DEBUG
  if(gDEBUG_CONTROL > 0) cout << "DEBUG [ControlAcq::~ControlAcq]" << endl;
#endif

  aManager = 0;
}

//============================================================================================
void ControlAcq::DoTimeout(Int_t n){
  switch (n){
     case 2:
       mTimeout2 = 0;
       cout << "Timeout 2 "<< endl;
     break;
     default:
     break;
   }
}


//============================================================================================
// Run function is divided into two parts, one for OSCI mode second for ACQ mode. In such a case is easier to control all the acquisition
//
void ControlAcq::Run(){
   #ifdef DEBUG
     if(gDEBUG_CONTROL > 0) cout << "DEBUG [ControlAcq::Start], mode = " << aManager->GetMode() << endl;
   #endif

//----- Initialization ----

  Int_t  retval;
  ADisplay *aDisplay = aManager->GetFrame()->GetDisplay();

//----- OSCI mode ------
 StartAllCards();
 aManager->SetStartAcqTime();
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
       //cout << "Digitizer [" << i << "] Number of events  " << numEvents[i] << endl;	
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
         

    gSystem->ProcessEvents();
    gSystem->Sleep(10);    

   }

 StopAllCards();
 
////////////////////////////////////////////////////////////////////////////////////////////// 
//----- ACQ mode -------

 if( aManager->GetMode() != MainFrame::sACQUISITION ) return;


 ADataSave2 aDataSave(kTRUE);
 aManager->SetStartAcqTime();
 aManager->SetEventNr(0);
 UInt_t eventNr = 0;
 UInt_t loop = 0;
 TStopwatch t;
	      t.Start();

  while( LoopCondition( 2 ) ){
              //t.Stop();
              //t.Print();
	      //t.Start();
       for(UInt_t i = 0; i < numInst; i++) {   
          if( !aManager->GetDigitizer(i)->IsEnabled() ) continue; 

          numEvents[i] = aManager->GetDigitizer(i)->GetData();  		
          //cout << "hello 20 numEvents["<<i<<"] = " << numEvents[i] << endl;
          if( numEvents[i] ){
             aManager->SetDataSize(aManager->GetDataSize() + aManager->GetDigitizer(i)->GetDataSize());	

             for(UInt_t j = 0; j < numEvents[i]; j++){ 
 
                //cout << "\n========= GetEvents ======" << endl;
	        //t.Start();
                //aManager->SetEventNr(aManager->GetEventNr() + j);
                AEvent *aE = aManager->GetDigitizer(i)->GetEvent();
                aE->SetEventNr( eventNr++ );
                //t.Stop();
                //t.Print();
 
                //cout << "\n========= Display ======" << endl;
	        //t.Start();
                if(aManager->GetDisplay())
                   aDisplay->UpdateGraph( aE );
                else{
                  AEvent *aE = NULL;
                  aDisplay->UpdateGraph( aE );
                  }
                //t.Stop();
                //t.Print();
                //cout << "\n========= Save ======" << endl;
                aDataSave.SaveInBuffer( *aE );
                //t.Stop();
                //t.Print();

                gSystem->ProcessEvents();

                }
            }
            else{
              gSystem->ProcessEvents();
            }
    
            gSystem->Sleep(10); 
   
        }
      } 
         

    //  if( IsForceTrigger() )							// in case that software trigger is necessary...
    //    aManager->GetDigitizer(i)->SendSWTrigger();
    //mNrAllEvents = Aggregate();


    //if(Online)  DoOnlineAnalysis;
    //if(Display) DisplayEvent;
    //if(gnuplot) Gnuplot;


}
//============================================================================================
bool ControlAcq::LoopCondition(Int_t mode){
/*
  #ifdef DEBUG
     if(gDEBUG_CONTROL > 0) 
        cout << "DEBUG [ControlAcq::LoopCondition] Mode = " << aManager->GetMode() << "\tEvents = " << aManager->GetEvents() << "\tMaxEvents = " << aManager->GetMaxEvents() << endl;
        cout << "                                             \tFiles = " << aManager->GetFiles() << "\tMaxFiles = " << aManager->GetMaxFiles() << endl;
  #endif
*/

 if( aManager->GetMode() == MainFrame::sSTOP )  		// return 0 when acquisition stops
   return kFALSE; 

 if( aManager->GetMaxEvents() ){                                // return 0 when user define max number of events
   if( aManager->GetEventNr() > aManager->GetMaxEvents() ){
       return kFALSE;
     } 
   }

 if( aManager->GetMaxFiles() ){					// return 0 when user define max number of files
   if( aManager->GetFileNr() > aManager->GetMaxFiles())
     return kFALSE;
   }

 if( aManager->GetMode() == MainFrame::sACQUISITION && mode == 1){
  return kFALSE;
  }
 else if (aManager->GetMode() == MainFrame::sACQUISITION && mode == 2){
  return kTRUE;
 }

return kTRUE;
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
/*void ControlAcq::InitAcq(){

  #ifdef DEBUG
     if(gDEBUG_CONTROL > 0) cout << "DEBUG [ControlAcq::InitAcq] "<< endl; 
  #endif

}
*/
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
bool ControlAcq::IsForceTrigger(){
//this function should be in loop condition.....
 return kFALSE; 

  if(aManager->GetTimeout()){
     if(mTimeout2 == 0){
        mTimeout2 = 1;
        Disconnect(timer2);
        timer2->Start(aManager->GetTimeout(), kTRUE); 
        return kTRUE;
       }
     else
        return kFALSE;  
     }
   else
    return kFALSE;

}
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

Bool_t  findMinTimeStamp(AEvent* a, AEvent* b){
 return a->GetTimeStamp() > b->GetTimeStamp();
}
