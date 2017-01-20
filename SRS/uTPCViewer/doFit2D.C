#include "TF2.h"
#include "TH2.h"
#include "TMath.h"
#include "TVirtualFitter.h"
#include "TFitResultPtr.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TMatrix.h"
#include "TFitResult.h"
#include "TCanvas.h"


Double_t g2(Double_t *x, Double_t *par) {
   Double_t r1 = Double_t((x[0]-par[1])/par[2]);
   Double_t r2 = Double_t((x[1]-par[3])/par[4]);
   return par[0]*TMath::Exp(-0.5*(r1*r1+r2*r2));
}
Double_t fun2(Double_t *x, Double_t *par) {
   Double_t *p1 = &par[0];
   Double_t result = g2(x,p1);
   return result;
}

void doFit2D()
{

	TCanvas *mycanvas = new TCanvas("c1", "c1", 860, 500);
TCanvas *mycanvas2 = new TCanvas("c2", "c2", 860, 500);
mycanvas->cd();

	TFile *f =
			TFile::Open(
	"/Users/dpfeiffe/data/Norway_November_2015/root/uTPC__Ampl_150__Discard_70_ZS_3800V_3300V_8cm_8cm_BC_5mm_5mm_HEX14_constant_fraction_400mV.root");

/*
	TFile *f =
			TFile::Open(
	"/Users/dpfeiffe/data/Norway_November_2015/root/uTPC__Ampl_150__Discard_70_ZS_3800V_3300V_8cm_8cm_BC_5mm_5mm_3He_HEX14_constant_fraction_400mV.root");
*/


//ZS_3750V_3250V_8cm_8cm_BC_2cm_2cm_100kHz_HEX14_constant_fraction.root

//ZS_3700V_3200V_8cm_8cm_BC_2cm_2cm_90kHz_HEX14_constant_fraction.root

//ZS_3800V_3300V_8cm_8cm_BC_2cm_2cm_140kHz_HEX14_constant_fraction.root

//ZS_3850V_3350V_8cm_8cm_BC_2cm_2cm_185kHz_HEX14_constant_fraction.root
	//TFile *f = TFile::Open("./center_uTPC_zs_AmBe_565uA.root");

	TTree *t = (TTree*) f->Get("tree");

	t->SetLineColor(2);
	double xs, xe, ys, ye, fite, fits;
//t->Draw("ys:xs>>h2","ys>=161&&ys<=213&&xs>=59&&xs<=111")



/*
beam
*/
 	 ys = 153;
	 ye = 215;
	 xs = 47;
	 xe = 109;

	
/* 
He3 tube 

	 ys = 153;
	 ye = 215;
	 xs = 49;
	 xe = 111;
*/
	TString cond1 = "(x0 >=";
	cond1 += xs;
	cond1 += " && x0 <= ";
	cond1 += xe;
	cond1 += " && y0 >= ";
	cond1 += ys;
	cond1 += " && y0 <= ";
	cond1 += ye;
	cond1 += ")";

	TH2D* h1 = new TH2D("h1", "h1", xe-xs, xs, xe, ye-ys,ys, ye);
	TH2D* h2 = new TH2D("h2", "h2", xe-xs, xs, xe, ye-ys,ys, ye);
	
 
   
	t->Draw("ys:xs>>h1", cond1, "goff");
	
	
	t->SetLineColor(4);

	Double_t max1 = h1->GetMaximum();
	Double_t min1 = h1->GetMinimum();
	

	h1->SetLineColor(kRed);
	h1->SetLineWidth(2);
	h1->SetMarkerColor(kRed);

	TF2 * f1;

	//f1 = new TF2("f1", "[0]+[1]*x+[2]*y +[3]*(TMath::Erfc(-(x-[4])/[5])*TMath::Erfc((x-[6])/[7]))*(TMath::Erfc(-(y-[8])/[9])*TMath::Erfc((y-[10])/[11]))");
	
	f1 = new TF2("f1", "[0]+[1]*TMath::Exp(-0.5*( ( (x-[2])*(x-[2]) / ([3]*[3]) ) + ((y-[4])*(y-[4])/([5]*[5]))))+[6]*(TMath::Erfc(-(x-[7])/[8])*TMath::Erfc((x-[9])/[10]))*(TMath::Erfc(-(y-[11])/[12])*TMath::Erfc((y-[13])/[14]))");
    
    
    const Int_t npar = 15;

/*
BEAM
*/
   Double_t f2params[npar] =
      { 6.70615e+00,
       8.29422e+01 , 7.80873e+01,1.39244e+01 ,1.83997e+02 , 1.37247e+01 ,
       9.88178e+04 ,
       7.15097e+01 , 6.17210e+00, 8.46140e+01,6.72526e+00 ,  
       1.98086e+02 ,9.78259e+00,1.72890e+02   ,8.71928e+00  
        };
   
    f1->SetParameters(f2params);

	f1->FixParameter(6,  9.88178e+04 );
/*	
 FCN=16888.8 FROM MINOS     STATUS=SUCCESSFUL    272 CALLS        7270 TOTAL
                     EDM=1.14624e-06    STRATEGY= 1      ERROR MATRIX ACCURATE 
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  0            8.29426e+01   1.73565e+00  -1.33602e-02  -6.13716e-05
   2  1            7.80873e+01   6.94483e-02  -8.13931e-06   6.44555e-05
   3  2            1.39244e+01   1.58676e-01   1.74163e-03   5.61827e-05
   4  3            1.83997e+02   6.65512e-02   5.97667e-05   9.64029e-06
   5  4            1.37247e+01   1.57845e-01   1.70765e-03  -3.45272e-04
   6  5            9.88178e+04     fixed    
   7  6            7.15097e+01   1.06198e-01   1.00557e-04   1.69174e-03
   8  7            6.17209e+00   1.04381e-01   2.18003e-04  -4.55675e-03
   9  8            8.46140e+01   1.13881e-01  -1.05060e-04  -2.00403e-03
  10  9            6.72525e+00   1.07791e-01   2.06501e-04  -4.77391e-03
  11  10           1.98086e+02   9.18137e-01   3.50085e-04  -8.84960e-03
  12  11           9.78267e+00   2.94468e-01   2.14516e-04   1.44153e-02
  13  12           1.72890e+02   7.76880e-01   1.55209e-05   8.63668e-03
  14  13           8.71919e+00   2.29615e-01   9.79920e-05   1.21613e-02
  15  14           6.70617e+00   2.39661e-01   2.39661e-01   2.75320e-03
 sumg 121746  - sume 191883

*/

/*
TUBE

   Double_t f2params[npar] =
      {-1.07400e+01,
       1.51983e+01 ,    8.07472e+01  ,6.40307e+01   ,  1.88018e+02  ,  1.00008e+02    ,
        3.60149e+03    , 
        8.62278e+01  , 1.06500e+01  ,  7.26888e+01  ,  1.12625e+01 ,
       1.91298e+02  ,9.42622e+00   ,1.77291e+02,   9.71135e+00};
  
     f1->SetParameters(f2params);

	
	f1->FixParameter(6,  2.47345e+01 );

	 FUNCTION MUST BE MINIMIZED BEFORE CALLING MINOs     
 FCN=5083.64 FROM MINOS     STATUS=SUCCESSFUL    661 CALLS       13815 TOTAL
                     EDM=3.28467e-08    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  11.0 per cent
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  0            3.56037e+00   2.30713e-01   7.83299e-04   2.45397e-04
   2  1            8.05099e+01   4.92175e-01  -3.52852e-04  -1.80756e-04
   3  2            1.79437e+01   1.77521e+00  -1.59148e-03  -3.96338e-04
   4  3            1.86229e+02   6.41022e-01  -6.49828e-04  -4.42220e-05
   5  4            2.14506e+01   2.22076e+00  -5.51732e-03   1.56859e-04
   6  5            2.47345e+01     fixed    
   7  6            8.75457e+01   2.87805e-01   4.65848e-04   1.16300e-03
   8  7           -5.72919e+00   3.48703e-01   6.47365e-04  -3.92381e-04
   9  8            7.24967e+01   2.90663e-01  -4.12797e-04   3.15565e-04
  10  9           -5.36221e+00   3.89518e-01   7.71750e-04   3.15125e-04
  11  10           1.87238e+02   8.53433e-01   1.10674e-04  -5.95208e-04
  12  11           8.76081e+00   5.72174e-01  -2.70321e-04   3.00967e-04
  13  12           1.82441e+02   8.37224e-01  -2.84208e-04   8.68211e-04
  14  13           8.42234e+00   5.54712e-01  -1.18437e-04   6.11545e-04
  15  14           2.08344e+00   2.52464e-01   2.52464e-01  -3.91787e-04
 sumg 15054.9  - sume 9236.7

*/
	
	
	TFitResultPtr r = h1->Fit("f1", "S");
	h1->GetFunction("f1")->SetLineColor(kRed);
	h1->GetFunction("f1")->SetLineStyle(2);
	h1->GetFunction("f1")->SetLineWidth(2);
	f1->SetLineColor(kRed);
	f1->SetLineStyle(2);
	f1->SetLineWidth(2);
	Double_t resolution1 = ((int) (100 * f1->GetParameter(3) * 400)) / 100;
	Double_t error1 = ((int) (100 * f1->GetParError(3) * 400)) / 100;




	
	h1->Draw();
	//hs->Draw("NOSTACK");

	TLegend * legend = new TLegend(0.1, 0.6, 0.48, 0.9);

	TString result1 = "fit last-local-maximum (#sigma=";
	result1 += resolution1;
	result1 += " um +/- ";
	result1 += error1;
	result1 += " um)";


	// Adds legend entries:
	legend->AddEntry(h1, "last-local-maximum");
	legend->AddEntry(f1, result1);

	if (fite == ye && fits == ys)
	{
		h1->GetXaxis()->SetTitle("y strip [400 um]");

	}
	else
	{
		h1->GetXaxis()->SetTitle("x strip [400 um]");
	}

	h1->GetYaxis()->SetTitle("counts");

/*
	TString label;
	int z = 0;
	for (int i = 1; i <= fite - fits; i++)
	{
		double d = -50 + 0.4 * (fits + i);
		if (d == (int) d)
		{
			z = i;
			break;
		}

	}

	for (int i = z; i <= fite - fits; i += 5)
	{
		label = "";
		double d = -50 + 0.4 * (fits + i);
		label += d;
		h1->GetXaxis()->SetBinLabel(i, label);

	}

	h1->GetXaxis()->SetLabelSize(.05);
*/	
	//h1->Draw("NOSTACK");
	
	legend->Draw(); // Draws the legend
	


	double zg=0,ze=0;
	double sumg=0, sume=0;

		
	for(int x=xs; x<=xe;x++)
	{
	for(int y=ys; y<=ye;y++)
	{
		
		ze=f1->GetParameter(6)*(TMath::Erfc(-(x-f1->GetParameter(7))/f1->GetParameter(8))*TMath::Erfc((x-f1->GetParameter(9))/f1->GetParameter(10))*(TMath::Erfc(-(y-f1->GetParameter(11))/f1->GetParameter(12))*TMath::Erfc((y-f1->GetParameter(13))/f1->GetParameter(14))));

zg = f1->GetParameter(0)+f1->GetParameter(1)*TMath::Exp(-0.5*( ( (x-f1->GetParameter(2))*(x-f1->GetParameter(2)) / (f1->GetParameter(3)*f1->GetParameter(3)) ) + ((y-f1->GetParameter(4))*(y-f1->GetParameter(4))/(f1->GetParameter(5)*f1->GetParameter(5)))));
	
			
			sumg+=zg;
			sume+=ze;
		
	
			h2->Fill(x,y,zg);
		
	}
	}

   	//h2->Add(h1, -1.);
	mycanvas2->cd();
	h2->Draw();
	std::cout <<  " sumg " << sumg << "  - sume " << sume << std::endl;
	

TMatrixDSym cov = r->GetCovarianceMatrix();    
r->Print("V");    

TVirtualFitter *fitter = TVirtualFitter::GetFitter();


TMatrixD covMatrixGaus(6,6);
TMatrixD covMatrixError(8,8);

for (Int_t i=0; i<6; i++){
    for (Int_t j=0; j<6; j++){
      covMatrixGaus[i][j] = fitter->GetCovarianceMatrixElement(i,j);
    }
}            

for (Int_t i=0; i<8; i++){
    for (Int_t j=0; j<8; j++){
      covMatrixError[i][j] = fitter->GetCovarianceMatrixElement(i+6,j+6);
    }
} 

covMatrixGaus.Print();
covMatrixError.Print();


TF2 *fGauss = new TF2("fGauss","[0]+[1]*TMath::Exp(-0.5*( ( (x-[2])*(x-[2]) / ([3]*[3]) ) + ((y-[4])*(y-[4])/([5]*[5]))))");      
TF2 *fError = new TF2("fError","9.88178e+04*(TMath::Erfc(-(x-[0])/[1])*TMath::Erfc((x-[2])/[3]))*(TMath::Erfc(-(y-[4])/[5])*TMath::Erfc((y-[6])/[7]))");      
    

fGauss->SetParameters( f1->GetParameter(0),  f1->GetParameter(1), f1->GetParameter(2), f1->GetParameter(3), f1->GetParameter(4),f1->GetParameter(5)) ;
fError->SetParameters( f1->GetParameter(7), f1->GetParameter(8), f1->GetParameter(9), f1->GetParameter(10),f1->GetParameter(11), f1->GetParameter(12), f1->GetParameter(13), f1->GetParameter(14)) ;


float binsize = h2->GetXaxis()->GetBinWidth(1)*h2->GetYaxis()->GetBinWidth(1);
Double_t a[2] = {xs,ys};
Double_t b[2] = {xe,ye};

cout<<"\n"<<"Integral of entire fit f2: "<<f1->Integral(xs,xe,ys,ye)/binsize<<"\n"
<<"Error integral of entire fit f2: "<<f1->IntegralError(2,a,b,f1->GetParameters(), cov.GetMatrixArray())/binsize<<"\n";

cout<<"\n"<<"Integral of subfunction gauss: "<<fGauss->Integral(xs,xe,ys,ye)/binsize<<"\n"
<<"Error integral of subfunction gauss: "<<fGauss->IntegralError(2,a,b,fGauss->GetParameters(), covMatrixGaus.GetMatrixArray())/binsize<<"\n";

cout<<"\n"<<"Integral of subfunction error: "<<fError->Integral(xs,xe,ys,ye)/binsize<<"\n"
<<"Error integral of subfunction error: "<<fError->IntegralError(2,a,b,fError->GetParameters(), covMatrixError.GetMatrixArray())/binsize<<"\n";


}

