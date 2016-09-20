#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

/// Root Includes
#include <TROOT.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TObjArray.h>

#include "UserClass.h"
#include "AEvent.h"
#include "ATrack.h"
#include "ASignal.h"


ClassImp(UserClass)


TH1F *pHW   = new TH1F("pH","pH",1000, 0, 1000);
TH1F *posW  = new TH1F("posW","posW",300, 0, 1);
TH1F *pHS   = new TH1F("pH","pH",1000, 0, 1000);
TH1F *posS  = new TH1F("posS","posS",300, 0, 1);
TH2F *p2d1 = new TH2F("p2d1","p2d1",600, 0, 1000, 600, 0, 1000);
TH2F *p2d2 = new TH2F("p2d2","p2d2",600, 0, 1000, 600, 0, 1000);

static int mRefresh = 0;

UserClass::UserClass() {

 mSignalList = new TObjArray();

c = new TCanvas("c", "c", 800, 600);
c->Divide(2,3);

c->cd(5); posS->Draw();
c->cd(6); posW->Draw();

/*
c->cd(2); pHW->Draw();
c->cd(4); pHS->Draw();
c->cd(5); pHS->Draw();
c->cd(6); pHS->Draw();
c->cd();
*/
}

//============================================================

UserClass::~UserClass() {
   delete mSignalList;
/*
pHW->SaveAs("phW.C");
posS->SaveAs("posS.C");
p2d1->SaveAs("p2d1.C");
p2d2->SaveAs("p2d2.C");
delete c;
*/
}

//============================================================

void UserClass::SignalProcessing(AEvent &aEvent){
   mSignalList->Clear();

}

//============================================================

void UserClass::DoAnalysis(AEvent &aEvent) {

  //if( CasetteNr(aEvent) & 0x3 ) 
  //   cout << "Analysing casette 1 or 2 or 5" << endl;
  static int i = 0;
  i++;
  if(i < 300) return;
  //cout << "i= " << i << endl;

      ASignal	*aSignal[32]; 
      Double_t	mMaximum[32];
      Int_t	mChannel[32];

     Int_t nrTrack = aEvent.GetNrTracks();
     //cout << "EventNr: " << aEvent.GetEventNr() << "\ti=" << i++ << "\tnrTrack = " << nrTrack << endl;


     for(Int_t i = 0; i < 32; i++){
        mMaximum[i] = 0.;
        mChannel[i] = 0;     
        }

     for(Int_t i = 0; i < nrTrack; i++){
        mChannel[i] = aEvent.GetTrack(i)->GetChannelNr(); 
        aSignal[i] = new ASignal(*aEvent.GetTrack(i)) ;
        char name[100];
        sprintf(name,"%d", mChannel[i]);  
        aSignal[i]->SetNameTitle(name, name);
        if(mChannel[i] < 16 ){
           aSignal[i]->Multiply( -1. );
           }  
        }



     Int_t baseline = 100;
     for(Int_t i = 0; i < nrTrack; i++){
        aSignal[i]->BaseLineCorrection( baseline ); 
        for(Int_t j = 510; j <= aSignal[i]->GetNbinsX(); j++) 
           aSignal[i]->SetBinContent(j, 0.);
        }

    /* 
     for(Int_t i = 0; i < 4; i++){
        c->cd(i+1);
        aSignal[i]->Draw(); 
        aSignal[i]->SetLineColor(2);
        aSignal[i]->SetMaximum(2000);  
        aSignal[i]->SetMinimum(-2000);  
        }
     c->cd();
     c->Modified();
     c->Update(); 
*/
     Int_t minWindow = 100, maxWindow = 500;
     Double_t x[nrTrack];
     for(Int_t i = 0; i < nrTrack; i++){
        aSignal[i]->FindPeakBin(x[i], mMaximum[ mChannel[i] ], minWindow, maxWindow);
        //cout << "przed, mChannel[i] = " << mChannel[i] << " " << mMaximum[ mChannel[i] ] << " " << x[i] << endl;
        }

    ASignal *sumS = (ASignal*) aSignal[0]->Clone("sumS");
    ASignal *sumW = (ASignal*) aSignal[2]->Clone("sumW");
    //sumS->Add(aSignal[1]);
    //sumW->Add(aSignal[3]);

    //Double_t xS, yS, xW, yW;
    //sumS->FindPeakBin(xS, yS, minWindow, maxWindow);
    //sumW->FindPeakBin(xW, yW, minWindow, maxWindow);
 
    
    //mMaximum[ mChannel[0] ] = aSignal[0]->Interpolate(xS);
    //mMaximum[ mChannel[1] ] = aSignal[1]->Interpolate(xS);
    //mMaximum[ mChannel[2] ] = aSignal[2]->Interpolate(xS);
    //mMaximum[ mChannel[3] ] = aSignal[3]->Interpolate(xS);
    
    //for(Int_t i = 0; i < nrTrack; i++)
        //cout << "po, " << mMaximum[ mChannel[i] ] << " " << xS << endl;

    posS->Fill( mMaximum[ mChannel[1]] / ( mMaximum[mChannel[1]] + mMaximum[mChannel[0]] ) );
    posW->Fill( mMaximum[ mChannel[3]] / ( mMaximum[mChannel[2]] + mMaximum[mChannel[3]] ) );
    
    if( !( i % 500)){ 
      c->cd(5); posS->Draw();
      c->cd(6); posW->Draw(); 
      c->cd();
      c->Modified();
      c->Update(); 
    } 
   
  std::ofstream fout;
  fout.open("dataM.txt", std::ofstream::out | std::ofstream::app); 
     for(Int_t i = 0; i < 32; i++){
        fout << mMaximum[i] << "\t";
        }
     fout << endl;
  fout.close();
 

     for(Int_t i = 0; i < nrTrack; i++){
        delete aSignal[i];
        }
     delete sumS;
     delete sumW;   
/*
      posW->Fill( mMax30 / (mMax30 + mMax31));
      pHW->Fill( mMax30 + mMax31 );

      posS->Fill( mMax14 / (mMax14 + mMax15));
      pHS->Fill( mMax14 + mMax15 );
    
      cout << mMax14 << "\t" << mMax15 << "\t" << mMax14 / (mMax14 + mMax15) << "\t" 
           << mMax30 << "\t" << mMax31 << "\t" << mMax30 / (mMax30 + mMax31) << endl;


     c->cd();
     c->cd(5);
     s14->Draw(); c14->Draw("same"); c14->SetLineColor(2); c14->SetMaximum(4000); c14->SetMinimum(-4000); 
     c->cd(6);
     s15->Draw(); c15->Draw("same"); c15->SetLineColor(2); c15->SetMaximum(4000); c15->SetMinimum(-4000);

        c->Modified();
        c->Update(); 

      //pos->Draw();
      //pH->Draw();

      //s1->Filter("crrc4", 0.1);
      //s2->Filter("crrc4", 0.1);
  
      if( !(mRefresh % 500) ){
        c->cd();
        c->cd(1);
        posW->Draw();
    
        c->cd(2);
        pHW->Draw();
       
        c->cd(3);
        posS->Draw();
        
        c->cd(4);
        pHS->Draw();

        c->Modified();
        c->Update(); 
        }

      mRefresh++;

  
  */   

  //      gSystem->Sleep(2000); 
}

//============================================================

Int_t UserClass::CasetteNr(AEvent &aEvent){
 
  Int_t retval = 0;
  Int_t nrTrack = aEvent.GetNrTracks();
 
  for(Int_t i = 0; i < nrTrack; i++){
     Int_t chn = aEvent.GetTrack(i)->GetChannelNr();  
     //cout << "chn = " << chn << endl;
     if( chn == 14 || chn == 15 || chn == 30 || chn == 31 )  retval |= (1 << 0); // casette 1
     if( chn == 12 || chn == 13 || chn == 28 || chn == 29 )  retval |= (1 << 1); // casette 2
     if( chn == 10 || chn == 11 || chn == 26 || chn == 27 )  retval |= (1 << 2); // casette 3
     if( chn ==  8 || chn ==  9 || chn == 24 || chn == 25 )  retval |= (1 << 3); // casette 4
     if( chn ==  6 || chn ==  7 || chn == 22 || chn == 23 )  retval |= (1 << 4); // casette 5
     if( chn ==  4 || chn ==  5 || chn == 20 || chn == 21 )  retval |= (1 << 5); // casette 5
     if( chn ==  2 || chn ==  3 || chn == 18 || chn == 19 )  retval |= (1 << 6); // casette 7
     if( chn ==  0 || chn ==  1 || chn == 16 || chn == 17 )  retval |= (1 << 7); // casette 8
     }

  //cout << "toto powinno zwrocic retval: " << retval << endl;
return retval;
}
//========================================================================

TH1F* UserClass::MakeTH1F(ATrack &aTrack) {
   char name[100];
   static Int_t nr = 0;
   sprintf(name, "TH1F_%d", nr++);
 
   TH1F *h =  new TH1F(name, name, aTrack.GetDataSize(), 0, aTrack.GetDataSize() );
   
   for(UInt_t i = 0; i < aTrack.GetDataSize(); i++){
      //UShort_t *a = aTrack.GetData();
      //Double_t b = a[i];
      //h->SetBinContent((Int_t)(i+1), b);
      h->SetBinContent((Int_t)(i+1), (Float_t)(aTrack.GetData()[i]));
      }
   return h;
}

//============================================================

