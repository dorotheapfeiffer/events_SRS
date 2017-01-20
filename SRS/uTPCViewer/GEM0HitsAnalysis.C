#define GEM0HitsAnalysis_cxx
// The class definition in GEM0HitsAnalysis.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("GEM0HitsAnalysis.C")
// Root > T->Process("GEM0HitsAnalysis.C","some options")
// Root > T->Process("GEM0HitsAnalysis.C+")
//

#include "GEM0HitsAnalysis.h"
#include <TH2.h>
#include <TStyle.h>


void GEM0HitsAnalysis::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1002210);
  gStyle->SetOptDate();
  gStyle->SetOptFit(1);
  gStyle->SetTimeOffset(dtime.Convert());

  view = TView::CreateView(1);
  view->SetRange(0,0,0,100000,10000,113.5575);

  cXHits = new TCanvas("cXHits", "cXHits",120,190,700,500);
  cXHits->Range(-0.75,-0.75,0.75,0.75);
  cXHits->SetFillColor(0);
  cXHits->SetBorderMode(0);
  cXHits->SetBorderSize(2);
  cXHits->SetTheta(90);
  cXHits->SetPhi(-360);
  cXHits->SetFrameBorderMode(0);

  cXQ = new TCanvas("cXQ", "cXQ",120,190,700,500);
  cXQ->Range(-0.75,-0.75,0.75,0.75);
  cXQ->SetFillColor(0);
  cXQ->SetBorderMode(0);
  cXQ->SetBorderSize(2);
  cXQ->SetTheta(90);
  cXQ->SetPhi(-360);
  cXQ->SetFrameBorderMode(0);

  cXNormalizedCharge = new TCanvas("cXNormalizedCharge", "cXNormalizedCharge",120,190,700,500);
  cXNormalizedCharge->Range(-0.75,-0.75,0.75,0.75);
  cXNormalizedCharge->SetFillColor(0);
  cXNormalizedCharge->SetBorderMode(0);
  cXNormalizedCharge->SetBorderSize(2);
  cXNormalizedCharge->SetTheta(90);
  cXNormalizedCharge->SetPhi(-360);
  cXNormalizedCharge->SetFrameBorderMode(0);

  cXCharge = new TCanvas("cXCharge", "cXCharge",120,190,700,500);
  cXCharge->Range(-0.75,-0.75,0.75,0.75);
  cXCharge->SetFillColor(0);
  cXCharge->SetBorderMode(0);
  cXCharge->SetBorderSize(2);
  cXCharge->SetTheta(90);
  cXCharge->SetPhi(-360);
  cXCharge->SetFrameBorderMode(0);

  cXSpectrum = new TCanvas("cXSpectrum", "cXSpectrum",120,190,700,500);
  cXSpectrum->Range(-0.75,-0.75,0.75,0.75);
  cXSpectrum->SetFillColor(0);
  cXSpectrum->SetBorderMode(0);
  cXSpectrum->SetBorderSize(2);
  cXSpectrum->SetTheta(90);
  cXSpectrum->SetPhi(-360);
  cXSpectrum->SetFrameBorderMode(0);

  cYSpectrum = new TCanvas("cYSpectrum", "cYSpectrum",120,190,700,500);
  cYSpectrum->Range(-0.75,-0.75,0.75,0.75);
  cYSpectrum->SetFillColor(0);
  cYSpectrum->SetBorderMode(0);
  cYSpectrum->SetBorderSize(2);
  cYSpectrum->SetTheta(90);
  cYSpectrum->SetPhi(-360);
  cYSpectrum->SetFrameBorderMode(0);

  cXSpectrum1 = new TCanvas("cXSpectrum1", "cXSpectrum1",120,190,700,500);
  cXSpectrum1->Range(-0.75,-0.75,0.75,0.75);
  cXSpectrum1->SetFillColor(0);
  cXSpectrum1->SetBorderMode(0);
  cXSpectrum1->SetBorderSize(2);
  cXSpectrum1->SetTheta(90);
  cXSpectrum1->SetPhi(-360);
  cXSpectrum1->SetFrameBorderMode(0);

  cXSpectrum2 = new TCanvas("cXSpectrum2", "cXSpectrum2",120,190,700,500);
  cXSpectrum2->Range(-0.75,-0.75,0.75,0.75);
  cXSpectrum2->SetFillColor(0);
  cXSpectrum2->SetBorderMode(0);
  cXSpectrum2->SetBorderSize(2);
  cXSpectrum2->SetTheta(90);
  cXSpectrum2->SetPhi(-360);
  cXSpectrum2->SetFrameBorderMode(0);
  
  cYSpectrum1 = new TCanvas("cYSpectrum1", "cYSpectrum1",120,190,700,500);
  cYSpectrum1->Range(-0.75,-0.75,0.75,0.75);
  cYSpectrum1->SetFillColor(0);
  cYSpectrum1->SetBorderMode(0);
  cYSpectrum1->SetBorderSize(2);
  cYSpectrum1->SetTheta(90);
  cYSpectrum1->SetPhi(-360);
  cYSpectrum1->SetFrameBorderMode(0);
  
  cYSpectrum2 = new TCanvas("cYSpectrum2", "cYSpectrum2",120,190,700,500);
  cYSpectrum2->Range(-0.75,-0.75,0.75,0.75);
  cYSpectrum2->SetFillColor(0);
  cYSpectrum2->SetBorderMode(0);
  cYSpectrum2->SetBorderSize(2);
  cYSpectrum2->SetTheta(90);
  cYSpectrum2->SetPhi(-360);
  cYSpectrum2->SetFrameBorderMode(0);

  cXYSpectrum = new TCanvas("cXYSpectrum", "cXYSpectrum",120,190,700,500);
  cXYSpectrum->Range(-0.75,-0.75,0.75,0.75);
  cXYSpectrum->SetFillColor(0);
  cXYSpectrum->SetBorderMode(0);
  cXYSpectrum->SetBorderSize(2);
  cXYSpectrum->SetTheta(90);
  cXYSpectrum->SetPhi(-360);
  cXYSpectrum->SetFrameBorderMode(0);

  cYHits = new TCanvas("cYHits", "cYHits",120,190,700,500);
  cYHits->Range(-0.75,-0.75,0.75,0.75);
  cYHits->SetFillColor(0);
  cYHits->SetBorderMode(0);
  cYHits->SetBorderSize(2);
  cYHits->SetTheta(90);
  cYHits->SetPhi(-360);
  cYHits->SetFrameBorderMode(0);

  cYQ = new TCanvas("cYQ", "cYQ",120,190,700,500);
  cYQ->Range(-0.75,-0.75,0.75,0.75);
  cYQ->SetFillColor(0);
  cYQ->SetBorderMode(0);
  cYQ->SetBorderSize(2);
  cYQ->SetTheta(90);
  cYQ->SetPhi(-360);
  cYQ->SetFrameBorderMode(0);

  cYCharge = new TCanvas("cYCharge", "cYCharge",120,190,700,500);
  cYCharge->Range(-0.75,-0.75,0.75,0.75);
  cYCharge->SetFillColor(0);
  cYCharge->SetBorderMode(0);
  cYCharge->SetBorderSize(2);
  cYCharge->SetTheta(90);
  cYCharge->SetPhi(-360);
  cYCharge->SetFrameBorderMode(0);

  cYNormalizedCharge = new TCanvas("cYNormalizedCharge", "cYNormalizedCharge",120,190,700,500);
  cYNormalizedCharge->Range(-0.75,-0.75,0.75,0.75);
  cYNormalizedCharge->SetFillColor(0);
  cYNormalizedCharge->SetBorderMode(0);
  cYNormalizedCharge->SetBorderSize(2);
  cYNormalizedCharge->SetTheta(90);
  cYNormalizedCharge->SetPhi(-360);
  cYNormalizedCharge->SetFrameBorderMode(0);



  cXYHits = new TCanvas("cXYHits", "cXYHits",120,190,700,500);
  cXYHits->Range(-0.75,-0.75,0.75,0.75);
  cXYHits->SetFillColor(0);
  cXYHits->SetBorderMode(0);
  cXYHits->SetBorderSize(2);
  cXYHits->SetTheta(90);
  cXYHits->SetPhi(-360);
  cXYHits->SetFrameBorderMode(0);

  cXYCharge = new TCanvas("cXYCharge", "cXYCharge",120,190,700,500);
  cXYCharge->Range(-0.75,-0.75,0.75,0.75);
  cXYCharge->SetFillColor(0);
  cXYCharge->SetBorderMode(0);
  cXYCharge->SetBorderSize(2);
  cXYCharge->SetTheta(90);
  cXYCharge->SetPhi(-360);
  cXYCharge->SetFrameBorderMode(0);

  cXYNormalizedCharge = new TCanvas("cXYNormalizedCharge", "cXYNormalizedCharge",120,190,700,500);
  cXYNormalizedCharge->Range(-0.75,-0.75,0.75,0.75);
  cXYNormalizedCharge->SetFillColor(0);
  cXYNormalizedCharge->SetBorderMode(0);
  cXYNormalizedCharge->SetBorderSize(2);
  cXYNormalizedCharge->SetTheta(90);
  cXYNormalizedCharge->SetPhi(-360);
  cXYNormalizedCharge->SetFrameBorderMode(0);


  TString option = GetOption();
  hXHits = new TH1F("hXHits","X Hits",100, -50., 50);
  hXQ = new TH1F("hXQ","Q[X]",100, -50., 50);
  hXSpectrum = new TH1F("hXSpectrum","X Spectrum",1500, 0., 15000);
  hXSpectrum1 = new TH1F("hXSpectrum1","X Spectrum 1",1500, 0., 15000);
  hXSpectrum2 = new TH1F("hXSpectrum2","X Spectrum Neutrons",1500, 0., 15000);
  hXNormalizedCharge = new TH1F("hXNormalizedCharge","X Normalized Charge", 100, -50., 50);
  hXCharge = new TH2F("hXCharge","X Peak Position", 1600,0.,1600000.,200, 0., 6000);
 
  hYHits = new TH1F("hYHits","Y Hits",100, -50., 50);
  hYQ = new TH1F("hYQ","Q[Y]",100, -50., 50);
  hYSpectrum = new TH1F("hYSpectrum","Y Spectrum",1500, 0., 15000);
  hYSpectrum1 = new TH1F("hYSpectrum1","Y Spectrum 1",1500, 0., 15000);
  hYSpectrum2 = new TH1F("hYSpectrum2","Y Spectrum Neutrons",1500, 0., 15000);
  hYNormalizedCharge = new TH1F("hYNormalizedCharge","Y Normalized Charge", 100, -50., 50);
  hYCharge = new TH2F("hYCharge","Y Peak Position", 1600,0.,1600000.,200, 0., 6000);

  hXYHits = new TH2F("hXYHits","XY Hits", 100,-50.,50.,100, -50., 50);
  hXYCharge = new TH2F("hXYCharge","XY Charge", 100,-50.,50.,100, -50., 50);
  hXYSpectrum = new TH2F("hXYSpectrum","XY Spectrum",150, 0., 15000,150, 0., 15000);
  hXYNormalizedCharge = new TH2F("hXYNormalizedCharge","XY Normalized Charge", 100,-50.,50.,100, -50., 50);

  hXQCLS = new TH2F("hXQCLS","charge_x:size_x",32,0,32,1000,0,34000);
  hYQCLS = new TH2F("hYQCLS","charge_y:size_y",32,0,32,1000,0,34000);
  bin_content=0.;
}

void GEM0HitsAnalysis::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t GEM0HitsAnalysis::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either GEM0HitsAnalysis::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   GetEntry(entry);
  
   //if (eventNumber <= 1500000 && x_global>=2 && x_global<= 48 && y_global>=2 && y_global<= 48)
   //if (x_global>=-48 && x_global<= -20 && y_global>=20 && y_global<= 48)
   //if (y_global>=-40 && y_global<= -20 && x_global>=20 && x_global<= 40 && size_x>=2 && size_y>=2 && charge_x>=0.05*charge_y && charge_y>=0.05*charge_x && (charge_x/charge_y<=1.25 || charge_x/charge_y>=1.45) )
//   if (y_global>=-40 && y_global<= -20 && x_global>=20 && x_global<= 40 && size_x>=2 && size_y>=2 && charge_x>=0.05*charge_y && charge_y>=0.05*charge_x && (size_x>1.5*size_y || size_y>=1.5*size_x) )
   //if (y_global>=-40 && y_global<= -20 && x_global>=20 && x_global<= 40 && size_x>=2 && size_y>=2 && charge_x>=0.05*charge_y && charge_y>=0.05*charge_x && (size_x>1.5*size_y || size_y>=1.5*size_x) )
   {
	   x_bin = TMath::FloorNint(x_global);
	   y_bin = TMath::FloorNint(y_global);

	   hXCharge->Fill(entry,charge_x);
	   hXSpectrum->Fill(charge_x);
	   hYCharge->Fill(entry,charge_y);
	   hYSpectrum->Fill(charge_y);
	   hXYSpectrum->Fill(charge_x,charge_y);
   
	   hXHits->Fill(x_bin);
	   hYHits->Fill(y_bin);
	   hXYHits->Fill(x_bin,y_bin);
	   hXYHits->GetXaxis()->SetTitle("x");
	   hXYHits->GetYaxis()->SetTitle("y");
	   hXYHits->GetXaxis()->CenterTitle();
	   hXYHits->GetYaxis()->CenterTitle();
	   hXYHits->GetXaxis()->SetTitleOffset(1.4);
	   hXYHits->GetYaxis()->SetTitleOffset(1.2);
	   

	   bin_content = hXQ->GetBinContent(x_bin+50);
	   hXQ->SetBinContent(x_bin+50,bin_content+charge_x);

	   bin_content = hYQ->GetBinContent(y_bin+50);
	   hYQ->SetBinContent(y_bin+50,bin_content+charge_y);

	   bin_content = hXYCharge->GetBinContent(x_bin+50,y_bin+50);
	   hXYCharge->SetBinContent(x_bin+50,y_bin+50,bin_content+charge_x+charge_y);
	   hXYCharge->GetXaxis()->SetTitle("x");
	   hXYCharge->GetYaxis()->SetTitle("y");
	   hXYCharge->GetXaxis()->CenterTitle();
	   hXYCharge->GetYaxis()->CenterTitle();
	   hXYCharge->GetXaxis()->SetTitleOffset(1.4);
	   hXYCharge->GetYaxis()->SetTitleOffset(1.2);
	   
	   hXQCLS->Fill(size_x,charge_x);
	   hYQCLS->Fill(size_y,charge_y);
   }
//   if (x_global>=20 && x_global<= 48 && y_global>=20 && y_global<= 48)
   {
	   x_bin = TMath::FloorNint(x_global);
	   y_bin = TMath::FloorNint(y_global);

	   hXSpectrum1->Fill(charge_x);
	   hYSpectrum1->Fill(charge_y);

   }
   return kTRUE;
}

void GEM0HitsAnalysis::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void GEM0HitsAnalysis::Terminate()
{
	
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   cout << "graphing...."<< endl;

   cXCharge->cd();
   hXCharge->Draw("lego2");

   cXSpectrum->cd();
   hXSpectrum->Draw();

   cYSpectrum->cd();
   hYSpectrum->Draw();

   cXSpectrum1->cd();
   hXSpectrum1->Draw();

   cYSpectrum1->cd();
   hYSpectrum1->Draw();
   
   cXSpectrum2->cd();
   hYSpectrum2->Add(hYSpectrum1,hYSpectrum,1,-1);
   hXSpectrum2->Draw();

   cYSpectrum2->cd();
   hXSpectrum2->Add(hXSpectrum1,hXSpectrum,1,-1);
   hYSpectrum2->Draw();
   
   
   cXYSpectrum->cd();
   hXYSpectrum->Draw("lego2");

   cYCharge->cd();
   hYCharge->Draw("lego2");


   cXYHits->cd();
   hXYHits->Draw("lego2");

   cXYCharge->cd();
   hXYCharge->Draw("lego2");

   cXYNormalizedCharge->cd();
   hXYNormalizedCharge->Divide(hXYCharge,hXYHits,1,1);
   hXYNormalizedCharge->GetXaxis()->SetRangeUser(-48,48);
   hXYNormalizedCharge->GetYaxis()->SetRangeUser(-48,48);
   hXYNormalizedCharge->Draw("lego2");
	hXYNormalizedCharge->GetXaxis()->SetTitle("x");
	hXYNormalizedCharge->GetYaxis()->SetTitle("y");
	hXYNormalizedCharge->GetXaxis()->CenterTitle();
	hXYNormalizedCharge->GetYaxis()->CenterTitle();
    hXYNormalizedCharge->GetXaxis()->SetTitleOffset(1.4);
	hXYNormalizedCharge->GetYaxis()->SetTitleOffset(1.2);

	   
   cXQ->cd();
   hXQ->Draw();

   cYQ->cd();
   hYQ->Draw();


   cXHits->cd();
   hXHits->Draw();

   cYHits->cd();
   hYHits->Draw();

   cXNormalizedCharge->cd();
   hXNormalizedCharge->Divide(hXQ,hXHits,1,1);
   hXNormalizedCharge->GetXaxis()->SetRangeUser(-48,48);
   hXNormalizedCharge->Draw();

   cYNormalizedCharge->cd();
   hYNormalizedCharge->Divide(hYQ,hYHits,1,1);
   hYNormalizedCharge->GetXaxis()->SetRangeUser(-48,48);
   hYNormalizedCharge->Draw();


   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");


   TCanvas *cXQCLS = new TCanvas("cXQCLS", "cXQCLS",375,350,680,520);
   cXQCLS->Range(-1.0,-1.25,1.0,1.25);
   TView *view_1 = TView::CreateView(1);
   view_1->SetRange(0,0,0,20,6800,10652.36);
   cXQCLS->SetFillColor(0);
   cXQCLS->SetBorderMode(0);
   cXQCLS->SetBorderSize(2);
   cXQCLS->SetTheta(57.80679);
   cXQCLS->SetPhi(-405.4827);
   cXQCLS->SetFrameBorderMode(0);
   cXQCLS->cd();

   hXQCLS->SetLineColor(ci);
   hXQCLS->GetXaxis()->SetTitle("size_x");
   hXQCLS->GetXaxis()->SetRange(1,20);
   hXQCLS->GetXaxis()->SetLabelFont(42);
   hXQCLS->GetXaxis()->SetLabelSize(0.035);
   hXQCLS->GetXaxis()->SetTitleSize(0.035);
   hXQCLS->GetXaxis()->SetTitleFont(42);
   hXQCLS->GetYaxis()->SetTitle("charge_x");
   hXQCLS->GetYaxis()->SetRange(1,200);
   hXQCLS->GetYaxis()->SetLabelFont(42);
   hXQCLS->GetYaxis()->SetLabelSize(0.035);
   hXQCLS->GetYaxis()->SetTitleSize(0.035);
   hXQCLS->GetYaxis()->SetTitleFont(42);
   hXQCLS->GetZaxis()->SetLabelFont(42);
   hXQCLS->GetZaxis()->SetLabelSize(0.035);
   hXQCLS->GetZaxis()->SetTitleSize(0.035);
   hXQCLS->GetZaxis()->SetTitleFont(42);
   hXQCLS->Draw("lego2");

   TPaveText *ptx = new TPaveText(0.31,0.94,0.69,0.995,"blNDC");
   ptx->SetName("title");
   ptx->SetBorderSize(0);
   ptx->SetFillColor(0);
   ptx->SetFillStyle(0);
   ptx->SetTextFont(42);
   TText *textx = ptx->AddText("charge_x:size_x");
   ptx->Draw();
   cXQCLS->Modified();
   cXQCLS->SetSelected(cXQCLS);

   TCanvas *cYQCLS = new TCanvas("cYQCLS", "cYQCLS",375,350,680,520);
   cYQCLS->Range(-1.0,-1.25,1.0,1.25);
   TView *view_2 = TView::CreateView(1);
   view_2->SetRange(0,0,0,20,6800,10652.36);
   cYQCLS->SetFillColor(0);
   cYQCLS->SetBorderMode(0);
   cYQCLS->SetBorderSize(2);
   cYQCLS->SetTheta(57.80679);
   cYQCLS->SetPhi(-405.4827);
   cYQCLS->SetFrameBorderMode(0);
   cYQCLS->cd();

   hYQCLS->SetLineColor(ci);
   hYQCLS->GetXaxis()->SetTitle("size_y");
   hYQCLS->GetXaxis()->SetRange(1,20);
   hYQCLS->GetXaxis()->SetLabelFont(42);
   hYQCLS->GetXaxis()->SetLabelSize(0.035);
   hYQCLS->GetXaxis()->SetTitleSize(0.035);
   hYQCLS->GetXaxis()->SetTitleFont(42);
   hYQCLS->GetYaxis()->SetTitle("charge_y");
   hYQCLS->GetYaxis()->SetRange(1,200);
   hYQCLS->GetYaxis()->SetLabelFont(42);
   hYQCLS->GetYaxis()->SetLabelSize(0.035);
   hYQCLS->GetYaxis()->SetTitleSize(0.035);
   hYQCLS->GetYaxis()->SetTitleFont(42);
   hYQCLS->GetZaxis()->SetLabelFont(42);
   hYQCLS->GetZaxis()->SetLabelSize(0.035);
   hYQCLS->GetZaxis()->SetTitleSize(0.035);
   hYQCLS->GetZaxis()->SetTitleFont(42);
   hYQCLS->Draw("lego2");

   TPaveText *pty = new TPaveText(0.31,0.94,0.69,0.995,"blNDC");
   pty->SetName("title");
   pty->SetBorderSize(0);
   pty->SetFillColor(0);
   pty->SetFillStyle(0);
   pty->SetTextFont(42);
   TText *texty = pty->AddText("charge_y:size_y");
   pty->Draw();
   cYQCLS->Modified();
   cYQCLS->SetSelected(cXQCLS);

}
