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
#include <TH2F.h>
#include <TObjArray.h>

#include "UserClass.h"
#include "AEvent.h"
#include "ATrack.h"
#include "ASignal.h"


ClassImp(UserClass)




UserClass::UserClass(TString name, int mAlgorythm) {

 chdiv_readout = mAlgorythm;
 if(chdiv_readout)
    cout << "Charge division readout[" << chdiv_readout << "]" << endl;  
 else
    cout << "Individual readout[" << chdiv_readout << "]" << endl;  


 //--------------------------------------------------------------------
 // remove the full path name and keep only last part without numbering
 // needed for output file, e.g. 
 // "/home/dlab/Desktop/SF/18Feb2016/cdiv/V1350_2_006.dat" -> "V1350_2"

 name = name.Remove(name.Length()-8);
 Int_t n = name.Last('/');
 name = name.Remove(0, n+1);
 mName = name;

 //--------------------------------------------------------------------
 mSignalList = new TObjArray();

 c = new TCanvas("c", "c", 1000, 640);
 c->SetWindowPosition(100, 300);

 if(chdiv_readout){
   DrawCdivCanvas();
  }
 else{
   DrawIndCanvas();
   }

//-------------------------------------------------------------------------
//Update canvas (reuired by ROOT)

}

//============================================================
  void UserClass::DrawIndCanvas(){
     chdiv_readout = 0;
     //cout << "Individual readout[" << chdiv_readout << "], delete and create histograms..." << endl;  
     c->SetTitle("...::: Individual readout :::..");
     mSignalList->Delete();
     c->Clear();
     c->Divide(3,3);
     gStyle->SetOptStat(0);

 posIND = new TH1F("","",330,0,33);
 mSignalList->Add(posIND);

 for(Int_t i = 0; i < 32; i++){
    char name[100];
    sprintf(name, "phIND__%d",i);
    phIND[i] = new TH1F(name,name,250, 0, 6000);
    mSignalList->Add(phIND[i]);
    }

 c->cd(1);
 phIND[0]->Draw();

 c->cd(2);
 for(Int_t i = 1; i < 6; i++){
    phIND[i]->Draw();
    } 
 
 c->cd(3);
 for(Int_t i = 6; i < 11; i++){
    phIND[i]->Draw();
    } 
 
 c->cd(4);
 for(Int_t i = 11; i < 16; i++){
    phIND[i]->Draw();
    } 

 c->cd(5);
 for(Int_t i = 16; i < 21; i++){
    phIND[i]->Draw();
    } 

 c->cd(6);
 for(Int_t i = 21; i < 26; i++){
    phIND[i]->Draw();
    } 

 c->cd(7);
 for(Int_t i = 26; i < 31; i++){
    phIND[i]->Draw();
    } 

 c->cd(8);
 phIND[31]->Draw();

 c->cd(9);
 posIND->Draw();
 
 c->cd();
 c->Modified();
 c->Update();
  }
//============================================================
  void UserClass::DrawCdivCanvas(){
    chdiv_readout = 1;
    //cout << "Charge division readout[" << chdiv_readout << "], delete and create histograms..." << endl;  
 // create canvas to display Pulse High
     mSignalList->Delete();
     c->Clear();
     c->SetTitle("...::: Charge Division readout :::...");
     gStyle->SetOptStat(0);
     c->Divide(8,5,0.0001,0.0001);

 //--------------------------------------------------------------------
 //create puls high histograms
 for(Int_t i = 0; i < 8; i++){
    char name[100];
    char title[100];
    sprintf(name, "Wires Cassete_%d",i);
    sprintf(title, "PH Wires Casette_%d",i);
    pHW[i] = new TH1F(name,title,300, 0, 6000);
    mSignalList->Add(pHW[i]);
    //pHW[i]->SetMaximum(300);
    pHW[i]->SetFillColor(2);
   
    sprintf(name, "Strips Cassete_%d",i);
    sprintf(title, "PH Strips Casette_%d",i);
    pHS[i] = new TH1F(name,title,300, 0, 6000);
    mSignalList->Add(pHS[i]);
    //pHS[i]->SetMaximum(300);
    pHS[i]->SetFillColor(2);

    }

 //--------------------------------------------------------------------
 //create position histograms
 for(Int_t i = 0; i < 8; i++){
    char name[100];
    char title[100];
    sprintf(name, "Wires Pos Cassete_%d",i);
    sprintf(title, "Position Wires Casette_%d",i);
    posW[i] = new TH1F(name,title,300, 0, 1);
    mSignalList->Add(posW[i]);
//    posW[i]->SetFillColor(2); 

    sprintf(name, "Strips Pos Cassete_%d",i);
    sprintf(title, "Position Strips Casette_%d",i);
    posS[i] = new TH1F(name,title,300, 0, 1);
    mSignalList->Add(posS[i]);
//    posS[i]->SetFillColor(2); 

    }

 //--------------------------------------------------------------------
 //create position histograms for all casettes
 posAll = new TH1F("AllCasettes","AllCasettes", 8*posW[0]->GetNbinsX(), 0, 8); 
 mSignalList->Add(posAll); 

//-----------------------------------------------------------------------
// Draw all histograms on one canvas
 for(Int_t i = 0; i < 8; i++){
    c->cd(i+1); 
    pHW[i]->Draw();
    c->cd(i+9);
    pHS[i]->Draw();
    c->cd(i+17);
    posW[i]->Draw();
    c->cd(i+25);
    posS[i]->Draw();
    }
    c->cd(40);
    posAll->Draw();
  
 c->cd();
 c->Modified();
 c->Update();
  }
//============================================================

UserClass::~UserClass() {
   //cout << " ((((((( destruktor )))))))" << endl;
   delete mSignalList;
}

//============================================================

void UserClass::SignalProcessing(AEvent &aEvent){
   mSignalList->Clear();

}

//============================================================

void UserClass::DoAnalysis(AEvent &aEvent, int _mMode) {
  
   mMode = _mMode;
   
   if(chdiv_readout)              // 1 - Charge division readout
      ChargeDiviReadout(aEvent);  // 0 - Indyvidual readout
   else                       
      IndyvidualReadout(aEvent);

}

//========================================================================

void UserClass::IndyvidualReadout(AEvent &aEvent){

    //cout << "Individual readout[" << chdiv_readout << "], filling histograms..." << endl;  
  static int iref = 0;
  ++iref;

  Int_t nrTrack = aEvent.GetNrTracks();

//--------------------------------------------------------------------
// set all tab variables to 0 
  for(Int_t i = 0; i < 32; i++){
      mMaximumIND[i] = 0.;
      mChannel[i] = 0;     
     }

//----------------------------------------------------------------------
//write timestamp to the last element of the tables
//each tracks in the same event have the same timestamp

  mMaximumIND[32] = aEvent.GetTrack(0)->GetTimeStamp();


//--------------------------------------------------------------------
// convert AEvents to ASignal and multiply strips, which are negative signals, by -1
  for(Int_t i = 0; i < nrTrack; i++){
      mChannel[i] = aEvent.GetTrack(i)->GetChannelNr(); 
      aSignal[i] = new ASignal(*aEvent.GetTrack(i)) ;
      char name[100];
      sprintf(name,"%d", mChannel[i]);  
      aSignal[i]->SetNameTitle(name, name);
      }

//----------------------------------------------------------------------
//restoring the baseline for each signals
//the baseline is calculated as a average of first 1228 points
//
     Int_t baseline = 128;
     for(Int_t i = 0; i < nrTrack; i++){
        aSignal[i]->BaseLineCorrection( baseline ); 
        for(Int_t j = 510; j <= aSignal[i]->GetNbinsX(); j++) 
           aSignal[i]->SetBinContent(j, 0.);
        }

//-------------------------------------------------------------------------
//calculate maximum of each signal using FindPeakBin (calculating [p,q] of the quadratic function)
//this is a "MAX algorythm" - takes maximum of each signal
//
     Int_t minWindow = baseline*1.05, maxWindow = aSignal[0]->GetNbinsX()*0.95;
     Double_t x[nrTrack];
     for(Int_t i = 0; i < nrTrack; i++){
        aSignal[i]->FindPeakBin(x[i], mMaximumIND[ mChannel[i] ], minWindow, maxWindow);
        }


//-------------------------------------------------------------------------
// Filling histogram position for individual readout.
// This is a simplyfied version, looking only for a ONE MAXIMUM wire.
// The detailed analysis, cetntre of gravity, is in Francesco script in MATLAB.

     Double_t maxval = 0.;
     Int_t    maxi;
     for(Int_t i = 0; i < nrTrack; i++){
         if( mMaximumIND[i] > maxval ){
            maxval = mMaximumIND[i];
            maxi = i;
            }
        }
     posIND->Fill(maxi + 0.95);

//-------------------------------------------------------------------------
// Filling histogram pulse high for individual readout.
    
     Double_t threshold = 50;
     for(Int_t i = 0; i < nrTrack; i++){
        if(mMaximumIND[i] > threshold)
           phIND[i]->Fill(mMaximumIND[i]);
        }
//-----------------------------------------------------------------------
// Update histograms and canvases, every 500 events to speed up the program, refreshing every single event will slow down the program significantly
//

    if( !( iref % 500)){ 
       c->cd(1);
       phIND[0]->Draw();

       c->cd(2);
       Int_t color = 2;
       for(Int_t i = 1; i < 6; i++){
           if(i == 1) phIND[i]->Draw();
           else       { phIND[i]->Draw("same"); phIND[i]->SetLineColor(color++); }
           } 
 
       c->cd(3);
       color = 1;
       for(Int_t i = 6; i < 11; i++){
           if(i == 6) phIND[i]->Draw();
           else       { phIND[i]->Draw("same"); phIND[i]->SetLineColor(color++); }
          } 
 
       c->cd(4);
       color = 1;
       for(Int_t i = 11; i < 16; i++){
          if(i == 11) phIND[i]->Draw();
          else       { phIND[i]->Draw("same"); phIND[i]->SetLineColor(color++); }
          } 

       c->cd(5);
       color = 1;
       for(Int_t i = 16; i < 21; i++){
          if(i == 16) phIND[i]->Draw();
          else       { phIND[i]->Draw("same"); phIND[i]->SetLineColor(color++); }
          } 

       c->cd(6);
       color = 1;
       for(Int_t i = 21; i < 26; i++){
           if(i == 21) phIND[i]->Draw();
           else       { phIND[i]->Draw("same"); phIND[i]->SetLineColor(color++); }
           } 

       c->cd(7);
       color = 1;
       for(Int_t i = 26; i < 31; i++){
           if(i == 26) phIND[i]->Draw();
           else       { phIND[i]->Draw("same"); phIND[i]->SetLineColor(color++); }
          } 

       c->cd(8);
       phIND[31]->Draw();

       c->cd(9);
       posIND->Draw();
 

      c->cd();
      c->Modified();
      c->Update(); 
  
}
 
//-----------------------------------------------------------------------
// save result to file analysed by Francesco in MATLAB
//
  std::ofstream fout;
  fout.open("output/" + mName + "_IND.txt", std::ofstream::out | std::ofstream::app); 
     for(Int_t i = 0; i < 33; i++){
        fout << mMaximumIND[i] << "\t";
        }
     fout << endl;
  fout.close();

//-----------------------------------------------------------------------
// cleaning some stuff
//
     for(Int_t i = 0; i < nrTrack; i++){
        delete aSignal[i];
        }


}
//========================================================================

void UserClass::ChargeDiviReadout(AEvent &aEvent){
    //cout << "Charge division readout[" << chdiv_readout << "], filling histograms..." << endl;  
  
  static int iref = 0;
  iref++;


  Int_t nrTrack = aEvent.GetNrTracks();

//--------------------------------------------------------------------
// set all tab variables to 0 
  for(Int_t i = 0; i < 32; i++){
      mMaximumSUM[i] = 0.;
      mMaximumMAX[i] = 0.;
      mChannel[i] = 0;     
     }

//--------------------------------------------------------------------
// convert AEvents to ASignal and multiply strips, which are negative signals, by -1
   for(Int_t i = 0; i < nrTrack; i++){
       mChannel[i] = aEvent.GetTrack(i)->GetChannelNr(); 
       aSignal[i] = new ASignal(*aEvent.GetTrack(i)) ;
       char name[100];
       sprintf(name,"%d", mChannel[i]);  
       aSignal[i]->SetNameTitle(name, name);
       if(mChannel[i] > 15 ){
          aSignal[i]->Multiply( -1. );
          }  
       }

//----------------------------------------------------------------------
//write timestamp to the last element of the tables
//each tracks in the same event have the same timestamp

    mMaximumMAX[32] = mMaximumSUM[32] = aEvent.GetTrack(0)->GetTimeStamp();

//----------------------------------------------------------------------
//restoring the baseline for each signals
//the baseline is calculated as a average of first 1228 points
    Int_t baseline = 128;
    for(Int_t i = 0; i < nrTrack; i++){
        aSignal[i]->BaseLineCorrection( baseline ); 
        for(Int_t j = 510; j <= aSignal[i]->GetNbinsX(); j++) 
           aSignal[i]->SetBinContent(j, 0.);
        }

//-------------------------------------------------------------------------
//calculate maximum of each signal using FindPeakBin (calculating [p,q] of the quadratic function)
//this is a "MAX algorythm" - takes maximum of each signal
    Int_t minWindow = 140, maxWindow = 500;
    Double_t x[nrTrack];

    for(Int_t i = 0; i < nrTrack; i++){
        aSignal[i]->FindPeakBin(x[i], mMaximumMAX[ mChannel[i] ], minWindow, maxWindow);
        }

//-------------------------------------------------------------------------
// calculate maximum of each signal using FindPeakBin (calculating [p,q] of the quadratic function)
// this is a "SUM algorythm" - takes maximum of the sum of both signals (strips and wires)
// we always use both algorytms, it is needed for file saveing

     for(Int_t i = 0; i < nrTrack/4; i++){
         ASignal *sumS = (ASignal*) aSignal[2*i]->Clone("sumS");
         sumS->Add(aSignal[2*i+1]);
         ASignal *sumW = (ASignal*) aSignal[16+2*i]->Clone("sumW");
         sumW->Add(aSignal[17+2*i]);

         Double_t xS, yS, xW, yW;
         sumS->FindPeakBin(xS, yS, minWindow, maxWindow);
         sumW->FindPeakBin(xW, yW, minWindow, maxWindow);
 
         //mMaximum[ mChannel[2*i]    ] = aSignal[2*i]->GetBinContent(xS);
         //mMaximum[ mChannel[2*i+1]  ] = aSignal[2*i+1]->GetBinContent(xS);
         //mMaximum[ mChannel[2*i+16] ] = aSignal[2*i+16]->GetBinContent(xW);
         //mMaximum[ mChannel[2*i+17] ] = aSignal[2*i+17]->GetBinContent(xW);

         mMaximumSUM[ mChannel[2*i] ]    = aSignal[2*i]->Interpolate(xS);
         mMaximumSUM[ mChannel[2*i+1] ]  = aSignal[2*i+1]->Interpolate(xS);
         mMaximumSUM[ mChannel[2*i+16] ] = aSignal[2*i+16]->Interpolate(xW);
         mMaximumSUM[ mChannel[2*i+17] ] = aSignal[2*i+17]->Interpolate(xW);
       
         delete sumS;
         delete sumW;   
         }

//----------------------------------------------------------------------
// Fill Puls High histograms 
    Int_t threshold  = 50;
    for(Int_t i = 0; i < 8; i++){
       if( mMaximumSUM[2*i] > threshold && mMaximumSUM[2*i+1] > threshold)
          pHW[i]->Fill( mMaximumSUM[2*i] + mMaximumSUM[2*i+1]);
       if( mMaximumSUM[2*i+16] > threshold && mMaximumSUM[2*i+17] > threshold)
          pHS[i]->Fill( mMaximumSUM[2*i+16] + mMaximumSUM[2*i+17]);
       }

//----------------------------------------------------------------------
// Fill Position histograms 
// here the condition is set that all 4 signals form the casette (2 wires and 2 strips) must fire and be higher than threshold
    for(Int_t i = 0; i < 8; i++){
       if( (mMaximumSUM[2*i] > threshold) && (mMaximumSUM[2*i+1] > threshold) && (mMaximumSUM[2*i+16] > threshold) && (mMaximumSUM[2*i+17] > threshold) ){
          posW[i]->Fill( mMaximumSUM[2*i]    / ( mMaximumSUM[2*i]    + mMaximumSUM[2*i+1] ) );
          posS[i]->Fill( mMaximumSUM[2*i+16] / ( mMaximumSUM[2*i+16] + mMaximumSUM[2*i+17]) );
         }
       }

//----------------------------------------------------------------------
// Fill Position histogram for all casettes

    for(Int_t i = 0; i < 8; i++){
        Int_t k = posW[i]->GetNbinsX();
        for(Int_t j = 1; j <= k; j++){
           posAll->SetBinContent(j + k*i, posW[i]->GetBinContent(j));
           } 
       }
//-----------------------------------------------------------------------
// Update histograms and canvases, every 500 events to speed up calculation, refreshing every single event will slow down calculation significantly
//
    if( !( iref % 500)){ 
       for(Int_t i = 0; i < 8; i++){
           c->cd(i+1); 
           pHW[i]->Draw();
          }
       for(Int_t i = 0; i < 8; i++){
           c->cd(i+9); 
           pHS[i]->Draw();
          }
       for(Int_t i = 0; i < 8; i++){
           c->cd(i+17); 
           posW[i]->Draw();
          }
       for(Int_t i = 0; i < 8; i++){
           c->cd(i+25); 
           posS[i]->Draw();
          }
    
       c->cd(40);
       posAll->Draw();

       c->cd();
       c->Modified();
       c->Update();
    } 
 
//-----------------------------------------------------------------------
// save result to file analysed by Francesco in MATLAB
//
 
  std::ofstream foutSUM;
  std::ofstream foutMAX;

  TString fNameSUM = "output/" + mName + "_CD_SUM.txt";
  TString fNameMAX = "output/" + mName + "_CD_MAX.txt";

  foutSUM.open(fNameSUM.Data(), std::ofstream::out | std::ofstream::app); 
  if(!foutSUM.is_open()) cout << fNameSUM << endl;
        for(Int_t i = 0; i < 33; i++){
            foutSUM << mMaximumSUM[i] << "\t";
            }
  foutSUM << endl;
  foutSUM.close();


  foutMAX.open(fNameMAX.Data(), std::ofstream::out | std::ofstream::app); 
  if(!foutMAX.is_open()) cout << fNameMAX << endl;
        for(Int_t i = 0; i < 33; i++){
            foutMAX << mMaximumMAX[i] << "\t";
            }
  foutMAX << endl;
  foutMAX.close();


//-----------------------------------------------------------------------
// cleaning some stuff
//
     for(Int_t i = 0; i < nrTrack; i++){
        delete aSignal[i];
        }

}
//========================================================================
