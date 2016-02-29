/*****************************************************************************
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <TMath.h>
#include "ASignal.h"

using namespace std;
ClassImp(ASignal)

//===============================================================================
/*
ASignal::~ASignal(){
#ifdef DEBUG
    if(gDEBUG_EVENT > 3) cout << "DEBUG [ASignal:Destructor] Delete ASignal aEventNr = " << aEventNr << " + tracks " << endl;
#endif    
}
*/

//===============================================================================

//void ASignal::Draw(Option_t* option){
//}

//===============================================================================

void ASignal::BaseLineCorrection(UInt_t n){

Double_t avg = 0;
if(n > 0){
    for(Int_t i = 0; i < n; i++) {
       avg += GetBinContent(i+1);
       }
    avg /= double(n);
  for(Int_t i=1; i<= GetNbinsX(); i++){
       SetBinContent(i, GetBinContent(i) - avg);
       }
    }
//cout << "avg = " << avg << endl;

}
//===============================================================================
Int_t ASignal::FindPeakBin(Double_t &x, Double_t &y, Int_t beginX, Int_t endX){

Double_t maxvalue = 0.;
Double_t mMaxx = 0;
Double_t mMaxy = 0;
Int_t maxbin = 0;

if( beginX == -1) beginX = 1;
if( endX   == -1) endX   = GetNbinsX();

//cout << "beginX: " << beginX << "  endX: " << endX << endl;

for(Int_t i = beginX; i <= endX; i++){
   maxvalue =  GetBinContent( i );
   if( maxvalue > mMaxy ) {
        mMaxx = GetBinCenter(i);
        mMaxy = maxvalue;
        maxbin = i;
     }
   }
//cout << "maxbin = " << maxbin << " mMaxx: " << mMaxx << " mMaxy: " << mMaxy << endl;

/*************************  quadratic interpolation  ************************/
Int_t distance = 10;
Double_t xx[] = { GetBinCenter( maxbin - distance), mMaxx, GetBinCenter(maxbin + distance)  };
Double_t yy[] = { GetBinContent(maxbin - distance), mMaxy, GetBinContent(maxbin + distance) };

//cout << "maxbin: " << maxbin << "  xx[0]: " << xx[0] << "  xx[1]: " << xx[1]  << "  xx[2]: " << xx[2] << endl;
//cout << "maxbin: " << maxbin << "  yy[0]: " << yy[0] << "  yy[1]: " << yy[1]  << "  yy[2]: " << yy[2] << endl;

Double_t d1 = 0;
Double_t d2 = 0;
Double_t d = xx[2]-xx[0];
d2 = (yy[2]-yy[1]+yy[0]-yy[1])/d/d;
d1 = 0.5*(yy[2]-yy[0])/d;

x = xx[1] - d1/d2;
y = yy[1] + 0.5*d1*(x-xx[1]);

//cout << "x: " << x << "   y: " << y << "  d1: " << d1 << " d2: " << d2 << endl;
return 0;
}

//=========================================================
void ASignal::Filter(char* option, Double_t Ta)
{
Char_t name[100];
strcpy(name, GetName());

if(!strcmp(option, "rect")){
    strcpy(name + strlen(name), "_FRect");
    SetNameTitle(name, name);

    Double_t s = 0;

    for(Int_t i=1; i<= GetNbinsX(); i++){
       if( i < Ta ){
         s += GetBinContent(i);
         SetBinContent(i, s);
         }
       else {
         s +=  ( GetBinContent(i) -  GetBinContent(i-(Int_t)Ta) );
         SetBinContent(i, s / Double_t(Ta));
         }
       }
  }
else if(!strcmp(option, "crrc4") ){

Double_t* out = new Double_t[GetNbinsX()];
for(Int_t i = 1; i <= GetNbinsX(); i++){
   out[i-1] = GetBinContent(i);
}

Double_t aFreqCut = Ta * 1000;

Double_t x  =  TMath::Exp(-2.0 * TMath::Pi() / aFreqCut); // rise  time
Double_t a0 =  TMath::Power(1.0 - x, 4) * (1.0 + x) / 2.0;
Double_t a1 = -TMath::Power(1.0 - x, 4) * (1.0 + x) / 2.0;
Double_t b1 =  5.0 * x;
Double_t b2 = -10.0 * TMath::Power(x, 2);
Double_t b3 =  10.0 * TMath::Power(x, 3);
Double_t b4 = -5.0 * TMath::Power(x, 4);
Double_t b5 =  TMath::Power(x, 5);

Double_t* out2 = new Double_t[GetNbinsX()];
out2[0] = 0;
out2[1] = a0*out[1]+a1*out[0];
out2[2] = a0*out[2]+a1*out[1]+b1*out2[1];
out2[3] = a0*out[3]+a1*out[2]+b1*out2[2]+b2*out2[1];
out2[4] = a0*out[4]+a1*out[3]+b1*out2[3]+b2*out2[2]+b3*out2[1];

for(Int_t i=5; i< GetNbinsX(); i++){
   out2[i] = a0 * out[i] + a1 * out[i-1] +
            b1 * out2[i-1] + b2 * out2[i-2] + b3*out2[i-3] + b4*out2[i-4] + b5*out2[i-5];
}

for(Int_t i = 1; i <= GetNbinsX(); i++){
   SetBinContent(i, out2[i-1]);
   }

delete [] out2;
delete [] out;


  }
 else if(!strcmp(option, "butt"))
  {
    Double_t max_in = GetMaximum();
    strcpy(name + strlen(name), "_FButt");
    SetNameTitle(name, name);
        Double_t x  =         exp( -1.0 / Ta);   // powyzej 20 nie ma wiekszego sensu....
                                                 // x powinno byc (chyba) <0,1>
    Double_t a0 =         pow( 1.0 - x, 4 );
    Double_t b1 =   5.0 * x;
    Double_t b2 = -10.0 * pow( x, 2 );
    Double_t b3 =  10.0 * pow( x, 3 );
    Double_t b4 =  -5.0 * pow( x, 4 );
    Double_t b5 =         pow( x, 5 );
 
SetBinContent(1, 0);
    SetBinContent(2, a0 * GetBinContent(2) );
    SetBinContent(3, a0 * GetBinContent(3) + b1 * GetBinContent(2) );
    SetBinContent(4, a0 * GetBinContent(4) + b1 * GetBinContent(3) + b2 * GetBinContent(2) );
    SetBinContent(5, a0 * GetBinContent(5) + b1 * GetBinContent(4) + b2 * GetBinContent(3) +
    b3 * GetBinContent(2) );
    
    for(int i = 6; i <=GetNbinsX(); i++) {
       SetBinContent(i, a0 *GetBinContent(i)+b1*GetBinContent(i-1)+b2*GetBinContent(i-2) +
       b3 *GetBinContent(i-3)+b4*GetBinContent(i-4)+b5*GetBinContent(i-5) );
       }
    Double_t max_out = GetMaximum();
    Scale(max_in / max_out);


  }
 else if(!strcmp(option, "cr")){
   Double_t* out = new Double_t[GetNbinsX()];
   Double_t x = TMath::Exp(-2 * TMath::Pi() * Ta);
   Double_t a0 = (1 + x) / 2.;
   Double_t a1 = -(1 + x) / 2.;
   Double_t b1 = x;

   out[0] = 0;
   out[1] = a0 * GetBinContent(1);
   for(Int_t i = 2; i < GetNbinsX(); i++){
      out[i] = a0 * GetBinContent(i) + b1 * out[i-1];
      out[i] = a0 * GetBinContent(i) + a1 * GetBinContent(i-1) + b1 * out[i-1];
      }

   for(Int_t i = 1; i <= GetNbinsX(); i++){
       SetBinContent(i, out[i]);
      }

delete [] out;

}
 else if(!strcmp(option, "rc")){
   Double_t* out = new Double_t[GetNbinsX()];
   Double_t x = TMath::Exp(-2 * TMath::Pi() * Ta);
   Double_t a0 = (1 - x);
   Double_t b1 = x;

   out[0] = 0;
   out[1] = a0 * GetBinContent(1);
   for(Int_t i = 2; i < GetNbinsX(); i++){
      out[i] = a0 * GetBinContent(i) + b1 * out[i-1];
      }

   for(Int_t i = 1; i <= GetNbinsX(); i++){
       SetBinContent(i, out[i]);
      }

delete [] out;


 }


 else if(!strcmp(option, "rc4")){
   Double_t* out = new Double_t[GetNbinsX()];

   Double_t x = TMath::Exp(-2 * TMath::Pi() * Ta);
   Double_t a0 = TMath::Power((1 - x), 4);
   Double_t b1 = 4 * x;
   Double_t b2 = -6 * TMath::Power(x,2);
   Double_t b3 = 4 * TMath::Power(x,3);
   Double_t b4 = -TMath::Power(x,4);

   out[0] = 0;
   out[1] = a0 * GetBinContent(1);
   out[2] = a0 * GetBinContent(1) + b1 * out[1];
   out[3] = a0 * GetBinContent(1) + b1 * out[1] + b2 * out[2];
   for(Int_t i = 4; i < GetNbinsX(); i++){
       out[i] = a0 * GetBinContent(i) +
                  b1 * out[i-1] + b2 * out[i-2] + b3 * out[i-3] + b4 *  out[i-4];
      }

   for(Int_t i = 1; i <= GetNbinsX(); i++){
       SetBinContent(i, out[i]);
      }

delete [] out;

 }
 else
  {
  cout << "This fillter [ " << option << " ] is not supportet..." << endl;
  }

}

//====================================================================================================================
