//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep  7 12:14:24 2012 by ROOT version 5.33/03
// from TTree GEM0Hits/GEM0Hits
// found on file: BEAMANALYSIS_RunAugustGem10fe55-694uA-UniformityRun.root
//////////////////////////////////////////////////////////

#ifndef GEM0HitsAnalysis_h
#define GEM0HitsAnalysis_h

#include <iostream> 
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TGraph.h>
#include "TDatime.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TColor.h"
#include "TText.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TView3D.h"
#include <stdio.h>


// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class GEM0HitsAnalysis : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // my Declaration

   TH2F *hXQCLS;
   TH2F *hYQCLS;



   TH1F *hXHits;
   TH1F *hXQ;
   TH1F *hXSpectrum;
   TH1F *hXNormalizedCharge;
   TH2F *hXCharge;

   TH1F *hYHits;
   TH1F *hYQ;
   TH1F *hYSpectrum;
   TH1F *hYNormalizedCharge;
   TH2F *hYCharge;

   TH2F *hXYCharge;
   TH2F *hXYSpectrum;
   TH2F *hXYNormalizedCharge;
   TH2F *hXYHits;

   TH1F *hXSpectrum1;
   TH1F *hXSpectrum2;
   TH1F *hYSpectrum1;
   TH1F *hYSpectrum2;
    
   TDatime dtime;
   Double_t bin_content;
   Int_t x_bin;
   Int_t y_bin;

   TCanvas *cXQ;
   TCanvas *cXSpectrum;
   TCanvas *cYQ;
   TCanvas *cYSpectrum;
   TCanvas *cYSpectrum1;
   TCanvas *cXSpectrum1;
   TCanvas *cYSpectrum2;
   TCanvas *cXSpectrum2;
   TCanvas *cXNormalizedCharge;
   TCanvas *cYNormalizedCharge;
   TCanvas *cXHits;
   TCanvas *cYHits;

   TCanvas *cXCharge;
   TCanvas *cYCharge;
   TCanvas *cXYCharge;
   TCanvas *cXYSpectrum;
   TCanvas *cXYNormalizedCharge;
   TCanvas *cXYHits;
   TView *view;

   // Declaration of leaf types
   Float_t         eventNumber;
   Float_t         x_local;
   Float_t         y_local;
   Float_t         z_local;
   Float_t         x_global;
   Float_t         y_global;
   Float_t         z_global;
   Float_t         charge_x;
   Float_t         charge_y;
   Float_t         size_x;
   Float_t         size_y;

   // List of branches
   TBranch        *b_eventNumber;   //!
   TBranch        *b_x_local;   //!
   TBranch        *b_y_local;   //!
   TBranch        *b_z_local;   //!
   TBranch        *b_x_global;   //!
   TBranch        *b_y_global;   //!
   TBranch        *b_z_global;   //!
   TBranch        *b_charge_x;   //!
   TBranch        *b_charge_y;   //!
   TBranch        *b_size_x;   //!
   TBranch        *b_size_y;   //!

   GEM0HitsAnalysis(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~GEM0HitsAnalysis() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(GEM0HitsAnalysis,0);
};

#endif

#ifdef GEM0HitsAnalysis_cxx
void GEM0HitsAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("x_local", &x_local, &b_x_local);
   fChain->SetBranchAddress("y_local", &y_local, &b_y_local);
   fChain->SetBranchAddress("z_local", &z_local, &b_z_local);
   fChain->SetBranchAddress("x_global", &x_global, &b_x_global);
   fChain->SetBranchAddress("y_global", &y_global, &b_y_global);
   fChain->SetBranchAddress("z_global", &z_global, &b_z_global);
   fChain->SetBranchAddress("charge_x", &charge_x, &b_charge_x);
   fChain->SetBranchAddress("charge_y", &charge_y, &b_charge_y);
   fChain->SetBranchAddress("size_x", &size_x, &b_size_x);
   fChain->SetBranchAddress("size_y", &size_y, &b_size_y);
}

Bool_t GEM0HitsAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef GEM0HitsAnalysis_cxx
