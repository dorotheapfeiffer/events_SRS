
void doFit()
{

	TCanvas *mycanvas = new TCanvas("c", "c", 860, 500);

	TFile *f =
			TFile::Open(	"/Users/dpfeiffe/data/Norway/3mm_10cm.root");

//ZS_3750V_3250V_8cm_8cm_BC_2cm_2cm_100kHz_HEX14_constant_fraction.root

//ZS_3700V_3200V_8cm_8cm_BC_2cm_2cm_90kHz_HEX14_constant_fraction.root

//ZS_3800V_3300V_8cm_8cm_BC_2cm_2cm_140kHz_HEX14_constant_fraction.root

//ZS_3850V_3350V_8cm_8cm_BC_2cm_2cm_185kHz_HEX14_constant_fraction.root

	//TFile *f = TFile::Open("./center_uTPC_zs_AmBe_565uA.root");

	TTree *t = (TTree*) f->Get("tree");

	t->SetLineColor(2);
	int xs, xe, ys, ye, fite, fits;
//t->Draw("ys:xs>>h2","ys>=161&&ys<=213&&xs>=59&&xs<=111")


	 ys = 145;
	 ye = 160;
	 xs = 108;
	 xe = 123;

	fite = ye;
	fits = ys;

/*
 	 xs = 62;
	 xe = 94;
	 ys = 183;
	 ye = 186;

	fite = xe;
	fits = xs;
*/
	TString edge = "left";

	TString cond1 = "(xs >=";
	cond1 += xs;
	cond1 += " && xs <= ";
	cond1 += xe;
	cond1 += " && ys >= ";
	cond1 += ys;
	cond1 += " && ys <= ";
	cond1 += ye;
	cond1 += ")";


	TString cond2 = "(xm >=";
	cond2 += xs;
	cond2 += " && xm <= ";
	cond2 += xe;
	cond2 += " && ym >= ";
	cond2 += ys;
	cond2 += " && ym <= ";
	cond2 += ye;
	cond2 += ")";

/*
	TString cond3 = "(xmax >=";
	cond3 += xs;
	cond3 += " && xmax <= ";
	cond3 += xe;
	cond3 += " && ymax >= ";
	cond3 += ys;
	cond3 += " && ymax <= ";
	cond3 += ye;
	cond3 += ")";
*/

	TH1D* h1 = new TH1D("h1", "h1", fite - fits, fits, fite);
	TH1D* h2 = new TH1D("h2", "h2", fite - fits, fits, fite);
	//TH1D* h3 = new TH1D("h3", "h3", fite - fits, fits, fite);

	if (fite == ye && fits == ys)
	{
		t->Draw("ys>>h1", cond1, "goff");
		t->Draw("ym>>h2", cond2, "goff");
		//t->Draw("ymax>>h3", cond3, "goff");
	}
	else
	{
		t->Draw("xs>>h1", cond1, "goff");
		t->Draw("xm>>h2", cond2, "goff");
		//t->Draw("xmax>>h3", cond3, "goff");
	}

	t->SetLineColor(4);

	Double_t max1 = h1->GetMaximum();
	Double_t max2 = h2->GetMaximum();
	Double_t min1 = h1->GetMinimum();
	Double_t min2 = h2->GetMinimum();
	/*
	Double_t min3 = h3->GetMinimum();
	Double_t max3 = h3->GetMaximum();
*/
	std::cout << min1 << std::endl;

	THStack *hs = new THStack("hs", ""); // Define the histogram stack object

	h1->SetLineColor(kRed);
	h1->SetLineWidth(2);
	h1->SetMarkerColor(kRed);

	h2->SetLineColor(kBlue);
	h2->SetLineWidth(2);
	h2->SetMarkerColor(kBlue);

	//h3->SetLineColor(kGreen-2);
	//h3->SetLineWidth(2);
	//h3->SetMarkerColor(kGreen-2);


	hs->Add(h1);
	hs->Add(h2);
	//hs->Add(h3);
	
	TF1 * f1;
	TF1 * f2;
	//TF1 * f3;
	if (edge == "double")
	{
		f1 = new TF1("f1", "[0]+[1]*(TMath::Erfc(-(x-[2])/[3])*TMath::Erfc((x-[4])/[3]))");
		f2 = new TF1("f2", "[0]+[1]*(TMath::Erfc(-(x-[2])/[3])*TMath::Erfc((x-[4])/[3]))");
		//f3 = new TF1("f3", "[0]+[1]*(TMath::Erfc(-(x-[2])/[3])*TMath::Erfc((x-[4])/[3]))");
	}
	else if (edge == "right")
	{
		f1 = new TF1("f1", "[0]+[1]*TMath::Erfc(-(x-[2])/[3])");
		f2 = new TF1("f2", "[0]+[1]*TMath::Erfc(-(x-[2])/[3])");
		//f3 = new TF1("f3", "[0]+[1]*TMath::Erfc(-(x-[2])/[3])");
	}
	else
	{
		f1 = new TF1("f1", "[0]+[1]*TMath::Erfc((x-[2])/[3])");
		f2 = new TF1("f2", "[0]+[1]*TMath::Erfc((x-[2])/[3])");
		//f3 = new TF1("f3", "[0]+[1]*TMath::Erfc((x-[2])/[3])");
	}

	if (edge == "double")
	{
/*
		f1->SetParameters(min1, 0.5 * max1, 70, 2,90);
		f2->SetParameters(min1, 0.5 * max1, 70, 2,90);
		f3->SetParameters(min1, 0.5 * max1, 70, 2,90);
*/

		f1->SetParameters(min1, 0.5 * max1, 175, 2,195);
		f2->SetParameters(min1, 0.5 * max1, 175, 2,195);
		//f3->SetParameters(min1, 0.5 * max1, 175, 2,195);

	}
	else
	{
		f1->SetParameters(min1, 0.5 * max1, (fits + fite) / 2, 2);
		f2->SetParameters(min2, 0.5 * max2, (fits + fite) / 2, 2);
		//f3->SetParameters(min3, 0.5 * max3, (fits + fite) / 2, 2);
		
	}
	h1->Fit("f1", "same");
	h1->GetFunction("f1")->SetLineColor(kRed);
	h1->GetFunction("f1")->SetLineStyle(2);
	h1->GetFunction("f1")->SetLineWidth(2);
	f1->SetLineColor(kRed);
	f1->SetLineStyle(2);
	f1->SetLineWidth(2);
	Double_t resolution1 = ((int) (100 * f1->GetParameter(3) * 400/ TMath::Sqrt(2) )) / 100;
	Double_t error1 = ((int) (100 * f1->GetParError(3) * 400/ TMath::Sqrt(2) )) / 100;

	std::cout << "\n Parameter 0: " << f1->GetParameter(0) << " "
			<< f1->GetParError(0) << std::endl;
	std::cout << "\n Parameter 1: " << f1->GetParameter(1) << " "
			<< f1->GetParError(1) << std::endl;
	std::cout << "\n Parameter 2: " << f1->GetParameter(2) << " "
			<< f1->GetParError(2) << std::endl;
	std::cout << "\n Parameter 3: " << f1->GetParameter(3) << " "
			<< f1->GetParError(3) << std::endl;
	
	
	h2->Fit("f2", "same");
	h2->GetFunction("f2")->SetLineColor(kBlue);
	h2->GetFunction("f2")->SetLineStyle(2);
	h2->GetFunction("f2")->SetLineWidth(2);

	f2->SetLineColor(kBlue);
	f2->SetLineStyle(2);
	f2->SetLineWidth(2);
	Double_t resolution2 = ((int) (100 * f2->GetParameter(3) * 400/ TMath::Sqrt(2) )) / 100;
	Double_t error2 = ((int) (100 * f2->GetParError(3) * 400/ TMath::Sqrt(2) )) / 100;

/*
	
	h3->Fit("f3", "same");
	h3->GetFunction("f3")->SetLineColor(kGreen-2);
	h3->GetFunction("f3")->SetLineStyle(2);
	h3->GetFunction("f3")->SetLineWidth(2);

	f3->SetLineColor(kGreen-2);
	f3->SetLineStyle(2);
	f3->SetLineWidth(2);
	Double_t resolution3 = ((int) (100 * f3->GetParameter(3) * 400)) / 100;
	Double_t error3 = ((int) (100 * f3->GetParError(3) * 400)) / 100;
	
*/
	
	h1->Draw();
	hs->Draw("NOSTACK");

	TLegend * legend = new TLegend(0.1, 0.6, 0.48, 0.9);

	TString result1 = "fit last-local-maximum (#sigma=";
	result1 += resolution1;
	result1 += " um +/- ";
	result1 += error1;
	result1 += " um)";

	TString result2 = "fit centroid-of-charge (#sigma=";
	result2 += resolution2;
	result2 += " um +/- ";
	result2 += error2;
	result2 += " um)";
/*
	TString result3 = "fit global-maximum (#sigma=";
	result3 += resolution3;
	result3 += " um +/- ";
	result3 += error3;
	result3 += " um)";
*/

	// Adds legend entries:
	legend->AddEntry(h1, "last-local-maximum");
	legend->AddEntry(f1, result1);
	legend->AddEntry(h2, "centroid-of-charge");
	legend->AddEntry(f2, result2);
	//legend->AddEntry(h3, "global-maximum");
	//legend->AddEntry(f3, result3);

	if (fite == ye && fits == ys)
	{
		hs->GetXaxis()->SetTitle("y strip [400 #mum]");

	}
	else
	{
		hs->GetXaxis()->SetTitle("x strip [400 #mum]");
	}

	hs->GetYaxis()->SetTitle("counts");
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
		hs->GetXaxis()->SetBinLabel(i, label);

	}

	hs->GetXaxis()->SetLabelSize(.05);
	*/
	hs->Draw("NOSTACK");
	legend->Draw(); // Draws the legend

}
