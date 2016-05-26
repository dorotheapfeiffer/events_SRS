/*****************************************************************************
*
* Implementation class for Sim
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
//#include <atomic>

#include <TRandom.h>
#include <TSystem.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGSlider.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TGButton.h>
#include <TGColorSelect.h>
#include <TMath.h>


#include "AManager.h"
#include "SimCard.h"
#include "ATrack.h"

ClassImp(SimCard)
using namespace std;

extern int gDEBUG;
int SimCard::gDeviceNr = 0;


SimCard::SimCard(Int_t nr ) : aThread("aThread", ThreadFunc, (void*)this ), aRun(kTRUE), mNumEvents(nr)  {
 #ifdef DEBUG
    if(gDEBUG > 2)
      cout << "DEBUG [SIM::Constructor] arg = " << nr << endl;
  #endif
 
 cout << "TThread::kCanceledState   = " << TThread::kCanceledState   << endl;
 cout << "TThread::kCancelingState  = " << TThread::kCancelingState  << endl;
 cout << "TThread::kDeletingState   = " << TThread::kDeletingState   << endl;
 cout << "TThread::kFinishedState   = " << TThread::kFinishedState   << endl;
 cout << "TThread::kHighPiority     = " << TThread::kHighPriority    << endl;
 cout << "TThread::kInvalidState    = " << TThread::kInvalidState    << endl;
 cout << "TThread::kLowPiority      = " << TThread::kLowPriority     << endl;
 cout << "TThread::kNewState        = " << TThread::kNewState        << endl;
 cout << "TThread::kNormalPiority   = " << TThread::kNormalPriority  << endl;
 cout << "TThread::kRunningState    = " << TThread::kRunningState    << endl;
 cout << "TThread::kTerminatedState = " << TThread::kTerminatedState << endl;


 mCardNr = gDeviceNr;
 gDeviceNr++;
 mRecordLength = 1024;
 sprintf(mName,"Sim_%d",mCardNr); 

}

//=======================================================================================
SimCard::~SimCard() {
  #ifdef DEBUG
    if(gDEBUG > 2)
      cout << "DEBUG [SIM::Destructor]" << endl;
  #endif

 aRun = kFALSE;
 aThread.Join();

}


//=======================================================================================
Int_t SimCard::Initialize() {
 #ifdef DEBUG
  if(gDEBUG > 2){
    cout << "DEBUG [SimCard::Initialization ]" << mName << endl;
    }
  #endif

 return kFALSE;
}

//=======================================================================================
Int_t SimCard::Configure() {
 #ifdef DEBUG
    if(gDEBUG > 2)
    cout << "DEBUG [SimCard::Configure] " << mName << " aRun = " << aRun << endl;
  #endif

  
}
//=======================================================================================
void SimCard::StartAcq() {  //this is implementation of the RunThread

 #ifdef DEBUG
  if(gDEBUG > 2){
    cout << "DEBUG [SimCard::StartAcq] name:"<< mName << flush <<  endl;
    }
  #endif
 
 aRun = kTRUE;
 cout << "DEBUG [SimCard::StartAcq] aRun = " << aRun << endl;
 aThread.Run((void*)this);

}

//=======================================================================================
void SimCard::StopAcq() { //this is implementation of the StopThread
 aRun = kFALSE;
 Int_t retval = aThread.Join();
 cout << "DEBUG [SimCard::StopAcq] retval after Join = " << retval << " aRun = " << aRun << " state = " << aThread.GetState() << endl;
 

}


//===============================================================================
void* SimCard::ThreadFunc(void* aPtr){

   #ifdef DEBUG
     if(gDEBUG > 0) TThread::Printf(" THREAD [SimCard::ThreadFunction] " );
   #endif

   SimCard* p = (SimCard*)aPtr;

   TThread::Printf(" THREAD [SimCard::ThreadFunction] aRun = %d", p->aRun);

   static ULong_t 		aEventCounter = 0;
   AEvent			aEvent;
   ULong_t			aTriggerTimeTag = 0;
   UInt_t			aLoop = 0;
   UInt_t			aDataSize = p->mRecordLength;
   Short_t			aData[aDataSize];
   std::vector<AEvent *>	localEvents;

   TThread::Printf(" THREAD [SimCard::ThreadFunction before while] %d, %d, %d, %d, %d", p->aRun, &p->aThread, p->aThread.GetState(), aLoop, p->mNumEvents);
   while( (p->aRun) && (&p->aThread) && (p->aThread.GetState() == TThread::kRunningState) && (aLoop < static_cast<ULong_t>(p->mNumEvents)) ){
          
          TThread::Printf(" THREAD [SimCard::ThreadFunction] loop = %d, aRun = %d",aLoop, p->aRun);
	 AEvent *aSingleEvent = new AEvent();
	 for(Int_t j2 = 0; j2 < 32; j2++){
            if( ((p->mSaveChannel >> j2) & 1) ) {
	       p->GenerateEvent(aData, aDataSize);	
               cout << std::bitset<32>(p->mSaveChannel) << endl;
               aSingleEvent->AddTrack(new ATrack(p->mCardNr, j2, 1. / 65e6, 0, 1000, aTriggerTimeTag, p->mThreshold_mV[j2], aData, aDataSize));
               }
            }

         aSingleEvent->SetEventNr(aEventCounter++);
         localEvents.push_back(aSingleEvent);
 	 aLoop++;
         TThread::Sleep(0, 10*1000); // sleep 10ms   10 * 1000 * 1000 ns
	}

   for(vector<AEvent *>::iterator it = localEvents.begin(); it != localEvents.end(); ++it){
      p->mEvents.push_back( (*it) );
      delete (*it);
      }
 localEvents.clear();

         TThread::Printf(" THREAD [SimCard::ThreadFunction] END");

/*
cout << "  ThreadFunc: " << endl; 
cout << "\tiTrackLength = " << p->aTrackLength[p->iTrackLength] << endl;
   cout <<"\tchannel\tRange\tOffset\tSigma\tNoise\tCF"<<endl;
    for(int i = 0; i < p->nbrChannels; i++)
       cout << "\tch." << i << "("<< p->iChannelEnable[i] << ")\t" 
            << p->sRange[p->iRange[i]] << "\t" << p->iOffset[i] << "\t" << p->iSigma[i] << "\t" << p->iNoise[i] 
            << "\t" << p->iCF[i] << endl;
*/  


//cout << "DEBUG [SimCard::ThreadFunc] tracklengh = " << p->aTrackLength[p->iTrackLength] << endl;
/*
for(Int_t k1 = 0; k1 < p->iSegments; k1++){

   for(Int_t k2 = 0; k2 < p->nbrChannels; k2++){
      double amp = 2 * gRandom->Rndm() * p->aRange[ p->iRange[k2] ];
      double sig = (0.5 + gRandom->Rndm()) * p->iSigma[k2];

      for(Int_t k3 = -p->iPreTrigger; k3 < p->aTrackLength[ p->iTrackLength] - p->iPreTrigger; k3++){
         double y = TMath::Landau(k3, 0, sig) * amp;
         double noise = (gRandom->Rndm() * p->iNoise[k2]) - 0.5;
          
         //cout << k3 + p->iPreTrigger << " " << y << " " << p->iPreTrigger << " " << sig << " " << noise << endl; 
         pvData[k3 + p->iPreTrigger] = static_cast<Short_t>( y + noise );
         }

      p->aTrack[k1][k2]->AddData(0, k2, pvData, p->aTrackLength[p->iTrackLength]);
      }

   }


 p->aIsReady = 1;
*/
/* 
 Short_t* data0 = p->aTrack[0][0]->GetData();
 Short_t* data1 = p->aTrack[0][1]->GetData();
 Short_t* data2 = p->aTrack[0][2]->GetData();
 Short_t* data3 = p->aTrack[0][3]->GetData();

 ofstream fout("aTrackTest.txt", ios::out);
// fout << "size " << p->aTrackLength[p->iTrackLength] << endl;
 for(int i = 0; i < p->aTrackLength[p->iTrackLength]; i++){
    fout << data0[i] << " " << data1[i] << " " << data2[i] << " " << data3[i] << endl; 
    }
fout.close();
*/
/*
for(int k = 0; k < 100; k++){
   double amp = gRandom->Rndm() * 2;
   double pos = gRandom->Rndm() * 3;
   double sig = gRandom->Rndm() * 3;
   for(int i = -100; i < 100; i++){
      double y = TMath::Landau(i, pos, 1*sig);
      double nois = gRandom->Rndm() / 30.;
      h->SetBinContent(i+101, (y * amp) + nois);
      }

}
*/
return aPtr;
}


//=======================================================================================

void	SimCard::GenerateEvent(Short_t *data, Int_t size){

  double amp   = 2 * gRandom->Rndm() * 1000;
  double sig   = (0.5 + gRandom->Rndm()) ;
  double noise = gRandom->Rndm() - 0.5;


  for(Int_t j = 0; j < size; j++){
     double y = TMath::Landau(j, 0, sig) * amp;
          
     //cout << k3 + p->iPreTrigger << " " << y << " " << p->iPreTrigger << " " << sig << " " << noise << endl; 
     data[j] = static_cast<Short_t>( y + noise );
     }

}

//=======================================================================================

void	SimCard::Reset(){

}

//=======================================================================================

UInt_t	SimCard::GetData(){

 #ifdef DEBUG
  if(gDEBUG > 2){
 //   cout << "DEBUG [SimCard::GetData] " <<  endl;
    }
  #endif

 StopAcq();
 StartAcq();
 cout << "DEBUG [SimCard::GetData] mEvent.size = " << mEvents.size() << " aRun = " << aRun << endl;
 return mEvents.size();

}

//=======================================================================================

void	SimCard::Refresh(){

}

//=======================================================================================

void	SimCard::SendSWTrigger(){

}


//=======================================================================================
void SimCard::Close() {

}
//=======================================================================================
Int_t SimCard::ReadConfigFile(ifstream &fin){

 cout << "\n\n" << endl;
 cout << "SimCard::ReadConfigFile not implemented yet..." << endl;
return 0;
}




//=======================================================================================
Int_t SimCard::WriteConfigFile(ofstream & fout){
 fout << "\n\n" << endl;
 fout << "SimCard::WriteConfigFile not implemented yet..." << endl;

 return 1;
}

