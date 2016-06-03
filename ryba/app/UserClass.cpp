#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

/// Root Includes
#include <TROOT.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TFile.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TObjArray.h>

#include "UserClass.h"
#include "AEvent.h"
#include "ATrack.h"
#include "ASignal.h"


ClassImp(UserClass)




UserClass::UserClass(TString name, int mAlgorythm) {

 //--------------------------------------------------------------------
 // remove the full path name and keep only last part without numbering
 // needed for output file, e.g. 
 // "/home/dlab/Desktop/SF/18Feb2016/cdiv/V1350_2_006.dat" -> "V1350_2"

// name = name.Remove(name.Length()-8);
 name = name.Remove(name.Length()-4);
 Int_t n = name.Last('/');
 name = name.Remove(0, n+1);
 mName = name;

 //--------------------------------------------------------------------
 mSignalList = new TObjArray();

 c = new TCanvas("c", "c", 1000, 640);
 c->SetWindowPosition(100, 300);

 c_2DV = new TCanvas("c_2DV", "c_2DV", 600, 600);
 c_2DV->SetWindowPosition(200, 400);
 DrawCanvas( mAlgorythm );

//-------------------------------------------------------------------------
//Update canvas (reuired by ROOT)

}

//============================================================
void UserClass::DrawCanvas(Int_t m_Algorythm){

 

 c->SetTitle("...::: MultiGrid result :::..");

 mSignalList->Delete();

 c->Clear();
 c->Divide(2,2);
 //gStyle->SetOptStat(0);

 m_GridAmplitude = new TH1F("GridAmplitude", "GridAmplitude", 4000, 0, 4000);
 m_WireAmplitude = new TH1F("WireAmplitude", "WireAmplitude", 4000, 0, 4000);
 m_GridPosition	 = new TH1F("GridPosition",  "GridPosition",  4000, 0, 4000);
 m_WirePosition	 = new TH1F("WirePosition",  "WirePosition",  4000, 0, 4000);
 m_2DView	 = new TH2F("2DView",  "2DView", 128, 0, 128, 128, 0, 128);
 m_TTS		 = new TGraph(10000);

 //mSignalList->Add();

 c->cd(1);
 m_GridAmplitude->Draw();
 gPad->SetLogy();

 c->cd(2);
 m_WireAmplitude->Draw();
 gPad->SetLogy();

 c->cd(3);
 m_GridPosition->Draw();
 gPad->SetLogy();

 c->cd(4);
 m_WirePosition->Draw();
 gPad->SetLogy();

 c->cd();
 c->Modified();
 c->Update();
// c->cd(5);
// m_TTS->Draw();

 c_2DV->SetTitle("...::: 2D view of the detector :::..");
 c_2DV->Clear();
 m_2DView->Draw("colz");
 c_2DV->Modified();
 c_2DV->Update();
 
}
//============================================================

UserClass::~UserClass() {
   //cout << " ((((((( destruktor )))))))" << endl;
   delete mSignalList;
}

//============================================================

Int_t UserClass::adc2pos(Int_t bin){
  // pos = 22mV/bin + 460(fixed_offset_adc) + 355*address;
  // values 0.46 - 3.3 V

  Int_t fixed_offset_adc = 460;
  Int_t address = 0;

 //return ((bin - fixed_offset_adc -  355*address ) / 22.) + 0.5;

return ((float(bin)/22. + fixed_offset_adc +  355*address )  + 0.5);
}

//============================================================


void UserClass::SignalProcessing(AEvent &aEvent){
   mSignalList->Clear();

}

//============================================================

void UserClass::DoAnalysis(AEvent &aEvent, int _mMode) {
  
  //cout << "DEBUG [UserClass::DoAnalysis] and immediately return" << endl;
  
  static int iref = 0;
  ++iref;

  Int_t nrTrack = aEvent.GetNrTracks();
  //cout << "\nTrack = " << nrTrack << endl;

  Int_t     ch[ nrTrack ];
  UShort_t *ptr; 
  Int_t     adc[ nrTrack ]; 
  ULong_t   tts; 
  UInt_t	idW, iDG;



  for(Int_t i = 0; i < nrTrack; i++){
     ch[i] = aEvent.GetTrack(i)->GetChannelNr();
     ptr = aEvent.GetTrack(i)->GetData();
     adc[i] = ptr[0];
    }

     m_GridAmplitude->Fill( adc[0] );       // adc[1] - GA
     m_WireAmplitude->Fill( adc[2] );       // adc[0] - WA
     m_GridPosition->Fill(  adc[1] );       // adc[3] - GP
     m_WirePosition->Fill(  adc[3] );       // adc[2] - WP
     m_2DView->Fill( adc2pos(adc[1]),  adc2pos(adc[3] ) );

     //tts = aEvent.GetTrack(i)->GetTimeStamp();
     //cout << iref << "\ti= " << i << "\tch: " << ch[i] << "\tadc: " << adc[i] << endl ;
     //cout << " adc0: " << adc[0] <<" adc[1]: " << adc[1] << " / " << adc2pos(adc[1] )
     //     << " adc2: " << adc[2] <<  " adc3: " << adc[3] << " / " << adc2pos(adc[3] ) << endl;



//----------------------------------------------------------------------


// Update histograms and canvases, every 500 events to speed up the program, refreshing every single event will slow down the program significantly
//

    if( !( iref % 5000)){ 
       c->cd(1);
       m_GridAmplitude->Draw();

       c->cd(2);
       m_WireAmplitude->Draw();

       c->cd(3);
       m_GridPosition->Draw();

       c->cd(4);
       m_WirePosition->Draw();

       c_2DV->cd();
       m_2DView->Draw("cont0"); 
       c_2DV->Modified();
       c_2DV->Update();


       c->cd();
       c->Modified();
       c->Update(); 
  
     }
 
//-----------------------------------------------------------------------
// save result to file analysed by Francesco in MATLAB
//
//  std::ofstream fout;
//  fout.open("output/" + mName + "_IND.txt", std::ofstream::out | std::ofstream::app); 
//     for(Int_t i = 0; i < 33; i++){
//        fout << mMaximumIND[i] << "\t";
//        }
//     fout << endl;
//  fout.close();
  
//-----------------------------------------------------------------------
// cleaning some stuff
//
//     for(Int_t i = 0; i < nrTrack; i++){
//        delete aSignal[i];
//        }


}
//========================================================================

 
