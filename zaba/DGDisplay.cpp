#include "DGDisplay.h"
#include <TStyle.h>

  ClassImp(DGDisplay)

//-----------------------------------------------------------------------------
 DGDisplay::DGDisplay(DMultiGrid *multigrid, TCanvas *canvas) {
  std::cout << "Constructing DGDisplay" << std::endl; //ja


  fMultiGrid = multigrid;
  fCanvas = canvas;

  //gStyle->SetOptDate(0);
  //gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(kFALSE);
  //gStyle->SetPalette(1);

  fHistogramList = new TObjArray();

  fCanvas->Divide(5,2,0,0);
  fCanvas->cd();
 
  m_NrPads = 10;
  hPh = new TH1F* [m_NrPads];
  hPosition = new TH1F* [m_NrPads];

  // creating histograms to visualise on gui spectra from chanel 1 to 8
  char name[100];
  for(Int_t i = 0; i < 8; i++){
     sprintf(name,"hPh%d",i+1);
     hPh[i] = new TH1F(name, name, 4000, 0, 4000);
     sprintf(name,"hPosition%d",i);
     hPosition[i] = new TH1F(name, name, 4000, 0, 4000);
     fHistogramList->Add(hPh[i]);
     fHistogramList->Add(hPosition[i]);
     }
 

  hTTS = new TH1F("TTS", "TTS", 120000, 0, 120000);
  hEmpty = new TH1F("hE", "hE", 120000, 0, 120000);

  fHistogramList->Add( hTTS );
  fHistogramList->Add( hEmpty );

// Draw histograms for puls high and position  
  for(Int_t i = 0; i < 4; i++){
     fCanvas->cd(i+1);
     hPh[i]->Draw();
     fCanvas->cd(i+6);
     hPosition[i]->Draw();
     }     

// Draw histogram for Trigger Time Stamp, end hEmpty
  fCanvas->cd(5);
  hTTS->Draw();
  fCanvas->cd(10);
  hEmpty->Draw();
 

  fCanvas->Modified();
  fCanvas->Update();
  fCanvas->cd();
}
//-----------------------------------------------------------------------------
 DGDisplay::~DGDisplay() {
 
 fHistogramList->Delete();
 delete fHistogramList; 
 delete [] hPh;
 delete [] hPosition; 
}
//-----------------------------------------------------------------------------
 void DGDisplay::Reset() {

  TObject   *elem;
  TIterator *iter;
  iter = fHistogramList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((TH1*)elem)->Reset();
 }
//-----------------------------------------------------------------------------
 void DGDisplay::Refresh() {
  for(Int_t i = 0; i < 9; i++)
      fCanvas->cd(i+1)->Modified();

  fCanvas->Update();
  fCanvas->cd();
}
//-----------------------------------------------------------------------------
