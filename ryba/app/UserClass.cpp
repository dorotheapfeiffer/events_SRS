#include <iostream>
#include <fstream>
#include <memory>

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

using namespace std;

ClassImp(UserClass)




UserClass::UserClass(TString name, int mAlgorythm) : 
	TObject(), 
	m_SignalList(),
	m_NrADC(8), 
	c1("c1", "c1", 1000, 640),
	c2("c2", "c2", 1000, 640)
	{

 //--------------------------------------------------------------------
 // remove the full path name and keep only last part without numbering
 // needed for output file, e.g. 
 // "/home/dlab/Desktop/SF/18Feb2016/cdiv/V1350_2_006.dat" -> "V1350_2"

// name = name.Remove(name.Length()-8);
 name = name.Remove(name.Length()-4);
 Int_t n = name.Last('/');
 name = name.Remove(0, n+1);
 m_Name = name;

 //--------------------------------------------------------------------
 c1.SetWindowPosition(100, 300);
 c2.SetWindowPosition(200, 400);
 
DrawCanvas( mAlgorythm );

//-------------------------------------------------------------------------
//Update canvas (reuired by ROOT)

}

//============================================================
void UserClass::DrawCanvas(Int_t m_Algorythm){

 

 c1.SetTitle("...::: MultiGrid result :::..");

 m_SignalList.Delete();

 c1.Clear();
 c1.Divide((m_NrADC + 1) / 2, 2);
 //gStyle->SetOptStat(0);

 for(Int_t i = 0; i < m_NrADC; i++){
     char name[256];
     sprintf(name,"adc_%d", i);
     m_ADC[i] = new TH1F(name, name, 4000, 0, 4000);
     }
 m_tts  = new TH1F("tts", "tts", 100000, 10, 300000);

 for(Int_t i = 0; i < m_NrADC; i++){
     c1.cd(i+1); m_ADC[i]->Draw(); gPad->SetLogy();
     }

 c1.cd(); c1.Modified(); c1.Update();

 c2.SetTitle("...::: MultiGrid Trigger Time Stamp :::..");
 c2.cd(); m_tts->Draw(); 
 c2.Modified(); c2.Update();
 
}
//============================================================

UserClass::~UserClass() {
   //cout << " ((((((( destruktor )))))))" << endl;
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

void UserClass::UpdateCanvas(){

       c1.cd(); c1.Modified(); c1.Update();
       c2.cd(); c2.Modified(); c2.Update();
}
//============================================================

void UserClass::SignalProcessing(AEvent &aEvent){
   m_SignalList.Clear();

}

//============================================================

void UserClass::DoAnalysis(AEvent &aEvent, int _mMode) {
  
  //cout << "DEBUG [UserClass::DoAnalysis] and immediately return" << endl;
  
  static int iref = 0;
  ++iref;

  for(Int_t i = 0; i < aEvent.GetNrTracks(); i++){
     m_ADC[i]->Fill( *(aEvent.GetTrack(i)->GetData())  );  
    }
  m_tts->Fill( aEvent.GetTrack(0)->GetTimeStamp());

//----------------------------------------------------------------------


// Update histograms and canvases, every 100000 events to speed up the program, refreshing every single event will slow down the program significantly
//

    if( !( iref % 500000)){ 
       c1.cd(1); m_ADC[0]->Draw();
       c1.cd(2); m_ADC[1]->Draw();
       c1.cd(3); m_ADC[2]->Draw();
       c1.cd(4); m_ADC[3]->Draw();
       c1.cd(5); m_ADC[4]->Draw();
       c1.cd(6); m_ADC[5]->Draw();
       c1.cd(7); m_ADC[6]->Draw();
       c1.cd(8); m_ADC[7]->Draw();
       c1.cd(); c1.Modified(); c1.Update();
       c2.cd(); c2.Modified(); c2.Update();
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

 
