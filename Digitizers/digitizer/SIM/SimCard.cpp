/*****************************************************************************
*
* Implementation class for Sim
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>

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
//int SimCard::mNrOfCards = 0;



SimCard::SimCard() {
 #ifdef DEBUG
    if(gDEBUG > 2)
      cout << "DEBUG [SIM::Constructor]" << endl;
  #endif

 mCardNr = mNrOfCards;
 mNrOfCards++;

 sprintf(mName,"Sim_%d",mCardNr); 
 cout << "[SimCard::Sim] mCardNr = " << mCardNr << " name = " << GetName() << endl;

}

//=======================================================================================
SimCard::~SimCard() {
 #ifdef DEBUG
    if(gDEBUG > 2)
      cout << "DEBUG [SIM::Destructor]" << endl;
  #endif


}


//=======================================================================================
Int_t SimCard::Initialize() {
 #ifdef DEBUG
  if(gDEBUG > 2){
    cout << "DEBUG [SimCard::Initialization ]" << fName << endl;
    }
  #endif

 return true;
}

//=======================================================================================
void SimCard::Configure() {
 #ifdef DEBUG
    if(gDEBUG > 2)
    cout << "DEBUG [SimCard::Configure] " << fName << endl;
  #endif

  
}
//=======================================================================================
void SimCard::StartAcq() {

 #ifdef DEBUG
  if(gDEBUG > 2){
    cout << "DEBUG [SimCard::StartAcq] name:"<< GetName() << " " <<  GetNrOfSamples(0) << flush <<  endl;
    }
  #endif

}

//===============================================================================
void* SimCard::ThreadFunc(void* aPtr){

aPtr = 0;
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
return 0;
}

//=======================================================================================
void SimCard::StopAcq() {
 cout << "[SimCard::StopAcq]" << endl;
}


//=======================================================================================
void SimCard::Close() {

}
//=======================================================================================
Int_t SimCard::ReadConfigFile(ifstream &file){

return 0;
}




//=======================================================================================
Int_t SimCard::WriteConfigFile(ofstream & fout){
 fout << "\n\n" << endl;
 fout << "SimCard::WriteConfigFile not implemented yet..." << endl;

 return 1;
}

