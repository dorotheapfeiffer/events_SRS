#include "DGDisplay.h"
#include <TStyle.h>

  ClassImp(DGDisplay)

//-----------------------------------------------------------------------------
 DGDisplay::DGDisplay(DMultiGrid *multigrid, TCanvas *canvas) {
  std::cout << "Constructing DGDisplay" << std::endl; //ja


  fMultiGrid = multigrid;
  fCanvas = canvas;

  gStyle->SetOptDate(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  gStyle->SetPalette(1);

  fHistogramList = new TObjArray();

  fPadFullImage	 = new TPad("fPadFullImage" , "fPadFullImage",  .0, .0, .5, 1.);
  fPadFrontImage = new TPad("fPadFrontImage", "fPadFrontImage", .5, .0, .7, 1.);
  fPadPh	 = new TPad("fPadPh",         "fPadPh",         .7, .5, 1., 1);
  fPadPosition	 = new TPad("fPadPosition",   "fPadPosition",   .7, .0, 1., .5);

  fCanvas->cd();
  fPadFullImage->Draw();	
  fPadFrontImage->Draw(); 
  fPadPh->Draw(); 	
  fPadPosition->Draw(); 	

  fHistogramList->Add( hFullImage	= new TH2F("FullImage", "FullImage", 128, 0, 128, 96, 0, 96));
  fHistogramList->Add( hFrontImage	= new TH2F("FrontImage","FrontImage",8,   0,   8, 96, 0, 96));
  fHistogramList->Add( hPh		= new TH1F("Ph",        "Ph",       4000, 0, 4000));
  fHistogramList->Add( hPosition	= new TH1F("Position",  "Position", 4000, 0, 4000));

  fPadFullImage->cd();
  hFullImage->Draw();

  fPadFrontImage->cd();	
  hFrontImage->Draw();
 
  fPadPh->cd();	
  fPadPh->SetLogy();
  hPh->Draw();	
  	
  fPadPosition->cd();	
  fPadPosition->SetLogy();
  hPosition->Draw();	

  fCanvas->Modified();
  fCanvas->Update();
  fCanvas->cd();
}
//-----------------------------------------------------------------------------
 DGDisplay::~DGDisplay() {
 
 fHistogramList->Delete();
 delete fHistogramList; 

 delete fPadFullImage;	
 delete fPadFrontImage; 
 delete fPadPh;	        
 delete fPadPosition;	

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
  fPadFullImage->Modified();
  fPadFullImage->Update();
  
  fPadPh->Modified();
  fPadPh->Update();
  
  fPadFrontImage->Modified();
  fPadFrontImage->Update();
  
  fPadPosition->Modified();
  fPadPosition->Update();
  
  fCanvas->Modified();
  fCanvas->Update();
  fCanvas->cd();
}
//-----------------------------------------------------------------------------
