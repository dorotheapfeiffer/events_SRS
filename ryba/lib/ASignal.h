/*****************************************************************************
*
*****************************************************************************/

#ifndef ASignal_h
#define ASignal_h


#include <iostream>
#include <string>
#include <exception>
#include <TObject.h>
#include <TH1F.h>
#include "ATrack.h"


//using namespace std;

class ASignal : public TH1F{
private:

  ASignal& operator= (const ASignal&);
  ASignal(const ASignal&);

public:
  ASignal() : TH1F() {}
  ASignal(TString s,int size) : TH1F(s,s,size,0,size){ }

  ASignal(ATrack &aTrack) : TH1F("s","s",aTrack.GetDataSize(), 0, aTrack.GetDataSize()){
     TString s;
     s += aTrack.GetName();
     SetNameTitle(s,s);
     UShort_t *data = aTrack.GetData();
     for(Int_t i=1; i<= GetNbinsX(); i++){
         SetBinContent(i, data[i]);
         }
  }

  virtual 	~ASignal(){}

  Bool_t	Multiply(Double_t a){
     			for(Int_t i=1; i<= GetNbinsX(); i++){
         		   SetBinContent(i, a*GetBinContent(i));
                           }
                return kTRUE;
                }

  Int_t		FindPeakBin(Double_t &x, Double_t &y, Int_t beginX = -1, Int_t endX = -1);
  void 		BaseLineCorrection(UInt_t);
  void		Filter(char* option, Double_t Ta);
//  void		Draw(Option_t* option = "");

ClassDef(ASignal, 1) // extended TH1F class for signal processing

};

#endif

