TString choice = "lin";
TString plotChoice = "fit";
TString stripChoice = "strip";
const double rotation = -7;
const double inverserot = -rotation;
TString rot = "";

void doFit(TTree *t, TString edge, int xs, int xe, int ys, int ye, int fits,
		int fite, TString region, TString dataName, TString plotName,
		TString cond1, TString cond2, TString condition, TString treeCond1,
		TString treeCond2)
{
	TCanvas *c = new TCanvas("c", "c", 860, 500);
	t->SetLineColor(2);
	std::cout << region << " " << dataName << " " << plotName << std::endl;

	TH1D* h1 = new TH1D("h1", "h1", fite - fits, fits, fite);
	TH1D* h2 = new TH1D("h2", "h2", fite - fits, fits, fite);
	TString condition1;
	TString condition2;
	/*
	 * if (cond1 == "")
	 {
	 condition1 = "(xd >=";
	 condition1 += xs;
	 condition1 += " && xd < ";
	 condition1 += xe;
	 condition1 += " && yd >= ";
	 condition1 += ys;
	 condition1 += " && yd < ";
	 condition1 += ye;
	 condition1 += ")" + condition;
	 }
	 else
	 {
	 condition1 = cond1 + condition;

	 }


	 if (cond2 == "")
	 {
	 condition2 = "(x0 >=";
	 condition2 += xs;
	 condition2 += " && x0 < ";
	 condition2 += xe;
	 condition2 += " && y0 >= ";
	 condition2 += ys;
	 condition2 += " && y0 < ";
	 condition2 += ye;
	 condition2 += ")" + condition;
	 }
	 else
	 {
	 condition2 = cond2 + condition;

	 }
	 */
	/*
	 if (cond1 == "")
	 {
	 condition1 = "(xs >=";
	 condition1 += xs;
	 condition1 += " && xs < ";
	 condition1 += xe;
	 condition1 += " && ys >= ";
	 condition1 += ys;
	 condition1 += " && ys < ";
	 condition1 += ye;
	 condition1 += ")" + condition;
	 }
	 else
	 {
	 condition1 = cond1 + condition;

	 }
	 */
	if (cond1 == "")
	{
		condition1 = "(xfirst >=";
		condition1 += xs;
		condition1 += " && xfirst < ";
		condition1 += xe;
		condition1 += " && yfirst >= ";
		condition1 += ys;
		condition1 += " && yfirst < ";
		condition1 += ye;
		condition1 += ")" + condition;
	}
	else
	{
		condition1 = cond1 + condition;

	}
	if (cond2 == "")
	{
		condition2 = "(xm >=";
		condition2 += xs;
		condition2 += " && xm < ";
		condition2 += xe;
		condition2 += " && ym >= ";
		condition2 += ys;
		condition2 += " && ym < ";
		condition2 += ye;
		condition2 += ")" + condition;
	}
	else
	{
		condition2 = cond2 + condition;

	}
	t->Draw(treeCond1, condition1);
	t->Draw(treeCond2, condition2);

	t->SetLineColor(4);

	Double_t max1 = h1->GetMaximum();
	Double_t max2 = h2->GetMaximum();
	Double_t min1 = h1->GetMinimum();
	Double_t min2 = h2->GetMinimum();

	Double_t sum1 = h1->GetEntries();
	Double_t sum2 = h2->GetEntries();

	std::cout << "H1: " << sum1 << "   SN " << max1 / min1 << std::endl;
	std::cout << "H2: " << sum2 << "   SN " << max2 / min2 << std::endl;

	THStack *hs = new THStack("hs", ""); //Define the histogram stack object

	h1->SetLineColor(kRed);
	h1->SetLineWidth(2);
	h1->SetMarkerColor(kRed);

	h2->SetLineColor(kBlue);
	h2->SetLineWidth(2);

	h2->SetMarkerColor(kBlue);

	hs->Add(h1);
	hs->Add(h2);

	TF1 * f1;
	TF1 * f2;
	if (edge == "right")
	{
		f1 = new TF1("f1", "[0]+[1]*TMath::Erfc(-(x-[2])/[3])");
		f2 = new TF1("f2", "[0]+[1]*TMath::Erfc(-(x-[2])/[3])");
	}
	else
	{
		f1 = new TF1("f1", "[0]+[1]*TMath::Erfc((x-[2])/[3])");
		f2 = new TF1("f2", "[0]+[1]*TMath::Erfc((x-[2])/[3])");
	}

	f1->SetParameters(min1, 0.5 * max1, (fits + fite) / 2, 2);
	h1->Fit("f1", "same");
	h1->GetFunction("f1")->SetLineColor(kRed);
	h1->GetFunction("f1")->SetLineStyle(2);
	h1->GetFunction("f1")->SetLineWidth(2);
	f1->SetLineColor(kRed);
	f1->SetLineStyle(2);
	f1->SetLineWidth(2);
	Double_t resolution1 = ((int) (100 * f1->GetParameter(3) / TMath::Sqrt(2)
			* 400)) / 100;
	Double_t error1 = ((int) (100 * f1->GetParError(3) / TMath::Sqrt(2) * 400))
			/ 100;
	/*
	 std::cout << "\n Parameter 0: " << f1->GetParameter(0) << " "
	 << f1->GetParError(0) << std::endl;
	 std::cout << "\n Parameter 1: " << f1->GetParameter(1) << " "
	 << f1->GetParError(1) << std::endl;
	 std::cout << "\n Parameter 2: " << f1->GetParameter(2) << " "
	 << f1->GetParError(2) << std::endl;
	 std::cout << "\n Parameter 3: " << f1->GetParameter(3) << " "
	 << f1->GetParError(3) << std::endl;
	 */
	f2->SetParameters(min2, 0.5 * max2, (fits + fite) / 2, 10);
	h2->Fit("f2", "same");
	h2->GetFunction("f2")->SetLineColor(kBlue);
	h2->GetFunction("f2")->SetLineStyle(2);
	h2->GetFunction("f2")->SetLineWidth(2);

	f2->SetLineColor(kBlue);
	f2->SetLineStyle(2);
	f2->SetLineWidth(2);
	Double_t resolution2 = ((int) (100 * f2->GetParameter(3) / TMath::Sqrt(2)
			* 400)) / 100;
	Double_t error2 = ((int) (100 * f2->GetParError(3) / TMath::Sqrt(2) * 400))
			/ 100;

	h1->Draw();
	hs->Draw("NOSTACK");

	TLegend * legend = 0;
	if (edge == "right")
	{
		legend = new TLegend(0.105, 0.7, 0.49, 0.9);
	}
	else
	{
		legend = new TLegend(0.49, 0.7, 0.875, 0.9);
	}
	/*


	 TString result1 = "fit time-over-threshold (#sigma=";
	 result1 += resolution1;
	 result1 += " um +/- ";
	 result1 += error1;
	 result1 += " um)";

	 TString result2 = "fit over-threshold (#sigma=";
	 result2 += resolution2;
	 result2 += " um +/- ";
	 result2 += error2;
	 result2 += " um)";

	 // Adds legend entries:
	 legend->AddEntry(h1, "longest time-over-threshold");
	 legend->AddEntry(f1, result1);
	 legend->AddEntry(h2, "last-over-threshold");
	 legend->AddEntry(f2, result2);
	 */
	TString result1 = "fit local-maximum (#sigma=";
	result1 += resolution1;
	result1 += " um +/- ";
	result1 += error1;
	result1 += " um)";

	//TString result2 = "fit last-maximum (#sigma=";
	TString result2 = "fit centroid-of-charge (#sigma=";
	result2 += resolution2;
	result2 += " um +/- ";
	result2 += error2;
	result2 += " um)";

	// Adds legend entries:

	legend->AddEntry(h1, "last-local-maximum");
	legend->AddEntry(f1, result1);
	legend->AddEntry(h2, "centroid-of-charge");
	legend->AddEntry(f2, result2);

	hs->GetYaxis()->SetTitle("counts");

	if (stripChoice != "strip")
	{
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
		if (fite == ye && fits == ys)
		{
			hs->GetXaxis()->SetTitle("y position [mm]");

		}
		else
		{
			hs->GetXaxis()->SetTitle("x position [mm]");
		}
	}
	else
	{
		if (fite == ye && fits == ys)
		{
			hs->GetXaxis()->SetTitle("y strip [0.4 mm]");

		}
		else
		{
			hs->GetXaxis()->SetTitle("x strip [0.4 mm]");
		}

	}
	hs->Draw("NOSTACK");
	legend->Draw(); // Draws the legend
	c->Print(
			"fit_" + stripChoice + "_" + region + "_" + rot + "_rot_" + dataName
					+ "_" + plotName);
	if (h1)
	{
		h1->Delete();
		h1 = 0;
	}
	if (h2)
	{
		h2->Delete();
		h2 = 0;
	}

}

void draw()
{
	TString plotName;
	TString dataName;
	TString condition = "";

	int xs, xe, ys, ye, fite, fits;
	TString edge;

	/* 3 mm x 10 cm */
	Double_t y1[5] =
	{ 48, 48, 63, 63, 48 };
	Double_t x1[5] =
	{ 100, 115, 115, 100, 100 };

	Double_t y2[5] =
	{ 81, 81, 96, 96, 81 };
	Double_t x2[5] =
	{ 102, 117, 117, 102, 102 };

	Double_t y3[5] =
	{ 145, 145, 160, 160, 145 };
	Double_t x3[5] =
	{ 108, 123, 123, 108, 108 };

	Double_t y4[5] =
	{ 176, 176, 191, 191, 176 };
	Double_t x4[5] =
	{ 115, 130, 130, 115, 115 };

	Double_t y5n[5] =
	{ 41, 41, 60, 60, 0 };
	Double_t x5n[5] =
	{ 97, 116, 116, 97, 0 };

	Double_t x5[5] =
	{ 0, 0, 0, 0, 0 };
	Double_t y5[5] =
	{ 0, 0, 0, 0, 0 };

	for (int n = 0; n < 4; n++)
	{
		x5[n] = (x5n[n] - x5n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				- (y5n[n] - y5n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ x5n[4];

		y5[n] = (x5n[n] - x5n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ (y5n[n] - y5n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				+ y5n[4];

	}
	x5[4] = x5[0];
	y5[4] = y5[0];

	Double_t y6n[5] =
	{ 113, 113, 132, 132, 0 };
	Double_t x6n[5] =
	{ 77, 96, 96, 77, 0 };

	Double_t x6[5] =
	{ 0, 0, 0, 0, 0 };
	Double_t y6[5] =
	{ 0, 0, 0, 0, 0 };

	for (int n = 0; n < 4; n++)
	{
		x6[n] = (x6n[n] - x6n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				- (y6n[n] - y6n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ x6n[4];

		y6[n] = (x6n[n] - x6n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ (y6n[n] - y6n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				+ y6n[4];

	}
	x6[4] = x6[0];
	y6[4] = y6[0];

	Double_t y7n[5] =
	{ 113, 113, 132, 132, 0 };
	Double_t x7n[5] =
	{ 96, 115, 115, 96, 0 };

	Double_t x7[5] =
	{ 0, 0, 0, 0, 0 };
	Double_t y7[5] =
	{ 0, 0, 0, 0, 0 };

	for (int n = 0; n < 4; n++)
	{
		x7[n] = (x7n[n] - x7n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				- (y7n[n] - y7n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ x7n[4];

		y7[n] = (x7n[n] - x7n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ (y7n[n] - y7n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				+ y7n[4];

	}
	x7[4] = x7[0];
	y7[4] = y7[0];

	Double_t y8n[5] =
	{ 136, 136, 155, 155, 0 };
	Double_t x8n[5] =
	{ 77, 96, 96, 77, 0 };

	Double_t x8[5] =
	{ 0, 0, 0, 0, 0 };
	Double_t y8[5] =
	{ 0, 0, 0, 0, 0 };

	for (int n = 0; n < 4; n++)
	{
		x8[n] = (x8n[n] - x8n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				- (y8n[n] - y8n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ x8n[4];

		y8[n] = (x8n[n] - x8n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ (y8n[n] - y8n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				+ y8n[4];

	}
	x8[4] = x8[0];
	y8[4] = y8[0];

	Double_t y9n[5] =
	{ 136, 136, 155, 155, 0 };
	Double_t x9n[5] =
	{ 96, 115, 115, 96, 0 };

	Double_t x9[5] =
	{ 0, 0, 0, 0, 0 };
	Double_t y9[5] =
	{ 0, 0, 0, 0, 0 };

	for (int n = 0; n < 4; n++)
	{
		x9[n] = (x9n[n] - x9n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				- (y9n[n] - y9n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ x9n[4];

		y9[n] = (x9n[n] - x9n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ (y9n[n] - y9n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				+ y9n[4];

	}
	x9[4] = x9[0];
	y9[4] = y9[0];

	Double_t y10n[5] =
	{ 210, 210, 229, 229, 0 };
	Double_t x10n[5] =
	{ 79, 98, 98, 79, 0 };

	Double_t x10[5] =
	{ 0, 0, 0, 0, 0 };
	Double_t y10[5] =
	{ 0, 0, 0, 0, 0 };

	for (int n = 0; n < 4; n++)
	{
		x10[n] = (x10n[n] - x10n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				- (y10n[n] - y10n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ x10n[4];

		y10[n] = (x10n[n] - x10n[4]) * TMath::Sin(rotation * TMath::Pi() / 180)
				+ (y10n[n] - y10n[4]) * TMath::Cos(rotation * TMath::Pi() / 180)
				+ y10n[4];

	}
	x10[4] = x10[0];
	y10[4] = y10[0];

	/*
	 Double_t y7[5] =
	 { 102, 100, 115, 117, 102 };
	 Double_t x7[5] =
	 { 114, 129, 131, 116, 114 };

	 Double_t y8[5] =
	 { 126, 124, 139, 141, 126 };
	 Double_t x8[5] =
	 { 96, 111, 113, 98, 96 };

	 Double_t y9[5] =
	 { 126, 124, 139, 141, 126 };
	 Double_t x9[5] =
	 { 117, 132, 134, 119, 117 };

	 Double_t y10[5] =
	 { 200, 198, 213, 215, 200 };
	 Double_t x10[5] =
	 { 107, 122, 124, 109, 107 };
	 */

	/*2 cm x 2 cm */
	Double_t y11[5] =
	{ 80, 80, 95, 95, 80 };
	Double_t x11[5] =
	{ 108, 123, 123, 108, 108 };

	Double_t y12[5] =
	{ 80, 80, 95, 95, 80 };
	Double_t x12[5] =
	{ 136, 151, 151, 136, 136 };

	Double_t y13[5] =
	{ 100, 100, 115, 115, 100 };
	Double_t x13[5] =
	{ 151, 166, 166, 151, 151 };

	Double_t y14[5] =
	{ 128, 128, 143, 143, 128 };
	Double_t x14[5] =
	{ 151, 166, 166, 151, 151 };

	Double_t y15[5] =
	{ 144, 144, 159, 159, 144 };
	Double_t x15[5] =
	{ 136, 151, 151, 136, 136 };

	Double_t y16[5] =
	{ 144, 144, 159, 159, 144 };
	Double_t x16[5] =
	{ 108, 123, 123, 108, 108 };

	Double_t y17[5] =
	{ 128, 128, 143, 143, 128 };
	Double_t x17[5] =
	{ 93, 108, 108, 93, 93 };

	Double_t y18[5] =
	{ 100, 100, 115, 115, 100 };
	Double_t x18[5] =
	{ 93, 108, 108, 93, 93 };

	/* 2 mm x 2 mm */
	Double_t y0[5] =
	{ 111, 111, 121, 121, 111 };
	Double_t x0[5] =
	{ 112, 122, 122, 112, 112 };

	for (int d = 3; d <= 3; d++)
	{

		//Double_t w = 1300;
		//Double_t h = 1200;
		Double_t w = 825;
		Double_t h = 800;
		const Int_t contours = 20;

		if (choice == "log")
		{
			gStyle->SetOptLogz(1);
		}
		else
		{
			gStyle->SetOptLogz(0);
		}
		gStyle->SetOptStat(0);
		gStyle->SetNumberContours(contours);
		gStyle->SetPadRightMargin(0.125);
		gStyle->SetPadLeftMargin(0.105);
		TFile* f;

		if (d == 1)
		{
			f =
					TFile::Open(
							"/Users/dpfeiffe/data/Norway/uTPC__Ampl_150__Dist_6000__PCA_3600__Gamma_4__Discard_99_GEM_3300V_4000V_2cm_2cm_dataTree.root");
			dataName = "2cm_2cm";
		}
		else if (d == 2)
		{
			f =
					TFile::Open(
							"/Users/dpfeiffe/data/Norway/uTPC__Ampl_150__Dist_6000__PCA_3600__Gamma_4__Discard_65_GEM_3300V_4000V_2cm_2cm_lead_dataTree.root");
			dataName = "2cm_2cm_lead";
		}
		else if (d == 3)
		{
			f =
			//TFile::Open(
			//		"/Users/dpfeiffe/data/Norway/uTPC__GEM_3300V_4000V_3mm_10cm_dataTree.root");

					TFile::Open("/Users/dpfeiffe/data/Norway/3mm_10cm.root");
			dataName = "3mm_10cm";
		}
		else if (d == 4)
		{
			f =
					TFile::Open(
							"/Users/dpfeiffe/data/Norway/uTPC__Ampl_150__Dist_6000__PCA_3600__Gamma_4__Discard_65_GEM_3300V_4000V_2mm_2mm_dataTree.root");
			dataName = "2mm_2mm";
		}
		else if (d == 5)
		{
			f =
					TFile::Open(
							"/Users/dpfeiffe/data/Norway/uTPC__Ampl_150__Dist_6000__PCA_3600__Gamma_4__Discard_65_GEM_3300V_4000V_3mm_10cm_Cd_dataTree.root");
			dataName = "3mm_10cm_Cd";
		}

		TCanvas * c;
		TH2F* h2;
		if (plotChoice == "plot")
		{
			c = new TCanvas("c", "c", w, h);
			h2 = new TH2F("h2", "h2", 256, 0, 256, 256, 0, 256);
		}

		TTree * t = (TTree*) f->Get("tree");

		for (int option = 1; option <= 1; option++)
		{
			if (plotChoice == "plot")
			{
				if (option == 1)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 &&(fParticleTypeX!=0 && fParticleTypeY!=0)");
					plotName = "all.pdf";
				}
				else if (option == 2)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 && (fParticleTypeX!=0 && fParticleTypeY!=0) && (fParticleTypeX==1 || fParticleTypeY==1)");
					plotName = "XorY_particleType1.pdf";
				}
				else if (option == 3)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 && fParticleTypeX==1 && fParticleTypeY==1");
					plotName = "XandY_particleType1.pdf";
				}
				else if (option == 4)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 &&(fParticleTypeX!=0 && fParticleTypeY!=0)&& (fParticleTypeX!=2 && fParticleTypeY!=2)");
					plotName = "not_particleType2.pdf";
				}

				else if (option == 5)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 && fParticleTypeX==3 && fParticleTypeY ==3");
					plotName = "XandY_particleType3.pdf";

				}

				else if (option == 6)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 && fParticleTypeX==2 && fParticleTypeY==2");
					plotName = "XandY_particleType2.pdf";
				}

				else if (option == 7)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 &&(fParticleTypeX!=0 && fParticleTypeY!=0) && (fParticleTypeX==3 || fParticleTypeY==3)");
					plotName = "XorY_particleType3.pdf";
				}

				else if (option == 8)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 &&(fParticleTypeX!=0 && fParticleTypeY!=0) && (fParticleTypeX==2 || fParticleTypeY==2)");
					plotName = "XorY_particleType2.pdf";
				}

				else if (option == 9)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 &&(fParticleTypeX!=0 && fParticleTypeY!=0)&& (fParticleTypeX!=1 && fParticleTypeY!=1)");
					plotName = "not_particleType1.pdf";
				}
				else if (option == 10)
				{
					t->Draw("ys:xs>>h2",
							"xs > 0 && ys > 0 &&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX!=3 && fParticleTypeY!=3)");
					plotName = "not_particleType3.pdf";
				}

				const Int_t Number = 21;
				Double_t Red[Number] =
				{ 0.90, 0.90, 0.70, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
						0.00, 0.00, 0.50, 0.80, 1.00, 1.00, 1.00, 0.80, 0.60,
						0.30, 0.00 };
				Double_t Green[Number] =
				{ 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.60, 0.80, 1.00, 0.50,
						0.70, 0.90, 1.00, 1.00, 0.80, 0.50, 0.00, 0.00, 0.00,
						0.00, 0.00 };
				Double_t Blue[Number] =
				{ 0.90, 1.00, 1.00, 0.80, 0.80, 1.00, 1.00, 1.00, 1.00, 0.50,
						0.30, 0.20, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
						0.00, 0.00 };

				Int_t FI;
				Int_t MyPalette[contours];
				Double_t Length[Number] =
				{ 0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45,
						0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90,
						0.95, 1.00 };
				FI = TColor::CreateGradientColorTable(Number, Length, Red,
						Green, Blue, contours);

				for (int i = 0; i < contours; i++)
				{
					MyPalette[i] = FI + i;
				}
				gStyle->SetPalette(contours, MyPalette);

				c->SetRightMargin(0.15);
				c->SetLeftMargin(0.10);
				c->SetTopMargin(0.03);
				c->SetBottomMargin(0.08);
				h2->GetZaxis()->SetTitleSize(0.03);
				h2->GetZaxis()->SetLabelSize(0.03);
				h2->GetZaxis()->SetTitleOffset(1.5);
				h2->GetYaxis()->SetTitleOffset(1.3);

				h2->SetZTitle("counts");
				h2->Draw("colz");

				h2->GetXaxis()->CenterTitle();
				h2->GetYaxis()->CenterTitle();
				h2->SetTitle("");
				h2->GetZaxis()->CenterTitle();
				h2->GetZaxis()->SetTitleSize(0.03);
				h2->GetZaxis()->SetLabelSize(0.03);
				h2->GetZaxis()->SetTitleOffset(1.25);
				h2->GetYaxis()->SetTitleOffset(1.3);

				if (stripChoice != "strip")
				{
					TString label;
					int z = 0;
					for (int i = 1; i <= 256; i++)
					{
						double d = -50 + 0.4 * i;
						if (d == (int) d)
						{
							z = i;
							break;
						}

					}

					for (int i = z; i <= 256; i += 25)
					{
						label = "";
						double d = -50.12 + 0.4 * (fits + i);
						label += d;
						h2->GetXaxis()->SetBinLabel(i, label);
						h2->GetYaxis()->SetBinLabel(i, label);

					}

					h2->GetXaxis()->SetLabelSize(.05);
					h2->GetYaxis()->SetLabelSize(.05);
					h2->SetXTitle("x position [mm]");
					h2->SetYTitle("y position [mm]");
				}
				else
				{
					h2->SetXTitle("x strip [0.4 mm]");
					h2->SetYTitle("y strip [0.4 mm]");
				}

				if (choice == "log")
				{
					Double_t levels[Number] =
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80,
							90, 100, 200 };

					h2->SetContour(contours, levels);
					h2->GetZaxis()->SetRangeUser(1, 200);
				}
				else
				{
					h2->GetZaxis()->SetRangeUser(0, 200);
				}
				if (d == 1 || d == 2)
				{
					TLatex l;
					l.SetTextSize(0.04);
					l.SetTextAngle(0.);

					//1
					TPolyLine *pline1 = new TPolyLine(5, x11, y11);
					pline1->SetLineColor(1);
					pline1->SetLineWidth(3);
					pline1->Draw();
					l.DrawLatex(x11[0] + (x11[1] - x11[0]) / 3,
							y11[2] - (y11[2] - y11[0]) / 1.25, Form("%d", 1));

					//2
					TPolyLine *pline2 = new TPolyLine(5, x12, y12);
					pline2->SetLineColor(1);
					pline2->SetLineWidth(3);
					pline2->Draw();
					l.DrawLatex(x12[0] + (x12[1] - x12[0]) / 3,
							y12[2] - (y12[2] - y12[0]) / 1.25, Form("%d", 2));

					//3
					TPolyLine *pline3 = new TPolyLine(5, x13, y13);
					pline3->SetLineColor(1);
					pline3->SetLineWidth(3);
					pline3->Draw();
					l.DrawLatex(x13[0] + (x13[1] - x13[0]) / 3,
							y13[2] - (y13[2] - y13[0]) / 1.25, Form("%d", 3));

					//4
					TPolyLine *pline4 = new TPolyLine(5, x14, y14);
					pline4->SetLineColor(1);
					pline4->SetLineWidth(3);
					pline4->Draw();
					l.DrawLatex(x14[0] + (x14[1] - x14[0]) / 3,
							y14[2] - (y14[2] - y14[0]) / 1.25, Form("%d", 4));

					//5
					TPolyLine *pline5 = new TPolyLine(5, x15, y15);
					pline5->SetLineColor(1);
					pline5->SetLineWidth(3);
					pline5->Draw();
					l.DrawLatex(x15[0] + (x15[1] - x15[0]) / 3,
							y15[2] - (y15[2] - y15[0]) / 1.25, Form("%d", 5));

					//6
					TPolyLine *pline6 = new TPolyLine(5, x16, y16);
					pline6->SetLineColor(1);
					pline6->SetLineWidth(3);
					pline6->Draw();
					l.DrawLatex(x16[0] + (x16[1] - x16[0]) / 3,
							y16[2] - (y16[2] - y16[0]) / 1.25, Form("%d", 6));

					//7
					TPolyLine *pline7 = new TPolyLine(5, x17, y17);
					pline7->SetLineColor(1);
					pline7->SetLineWidth(3);
					pline7->Draw();
					l.DrawLatex(x17[0] + (x17[1] - x17[0]) / 3,
							y17[2] - (y17[2] - y17[0]) / 1.25, Form("%d", 7));

					//8
					TPolyLine *pline8 = new TPolyLine(5, x18, y18);
					pline8->SetLineColor(1);
					pline8->SetLineWidth(3);
					pline8->Draw();
					l.DrawLatex(x18[0] + (x18[1] - x18[0]) / 3,
							y18[2] - (y18[2] - y18[0]) / 1.25, Form("%d", 8));

				}

				else if (d == 3)
				{
					TLatex l;
					l.SetTextSize(0.04);
					l.SetTextAngle(0.);

					TPolyLine *pline1 = new TPolyLine(5, x1, y1);
					pline1->SetLineColor(1);
					pline1->SetLineWidth(3);
					pline1->Draw();

					l.DrawLatex(x1[0] + (x1[1] - x1[0]) / 3,
							y1[2] - (y1[2] - y1[0]) / 1.25, Form("%d", 1));

					TPolyLine *pline2 = new TPolyLine(5, x2, y2);
					pline2->SetLineColor(1);
					pline2->SetLineWidth(3);
					pline2->Draw();
					l.DrawLatex(x2[0] + (x2[1] - x2[0]) / 3,
							y2[2] - (y2[2] - y2[0]) / 1.25, Form("%d", 2));

					TPolyLine *pline3 = new TPolyLine(5, x3, y3);
					pline3->SetLineColor(1);
					pline3->SetLineWidth(3);
					pline3->Draw();
					l.DrawLatex(x3[0] + (x3[1] - x3[0]) / 3,
							y3[2] - (y3[2] - y3[0]) / 1.25, Form("%d", 3));

					TPolyLine *pline4 = new TPolyLine(5, x4, y4);
					pline4->SetLineColor(1);
					pline4->SetLineWidth(3);
					pline4->Draw();
					l.DrawLatex(x4[0] + (x4[1] - x4[0]) / 3,
							y4[2] - (y4[2] - y4[0]) / 1.25, Form("%d", 4));

					TPolyLine *pline5 = new TPolyLine(5, x5, y5);
					pline5->SetLineColor(1);
					pline5->SetLineWidth(3);
					pline5->Draw();
					l.DrawLatex(x5[0] + (x5[1] - x5[0]) / 3,
							y5[2] - (y5[2] - y5[0]) / 1.25, Form("%d", 5));

					TPolyLine *pline6 = new TPolyLine(5, x6, y6);
					pline6->SetLineColor(1);
					pline6->SetLineWidth(3);
					pline6->Draw();
					l.DrawLatex(x6[0] + (x6[1] - x6[0]) / 3,
							y6[2] - (y6[2] - y6[0]) / 1.25, Form("%d", 6));

					TPolyLine *pline7 = new TPolyLine(5, x7, y7);
					pline7->SetLineColor(1);
					pline7->SetLineWidth(3);
					pline7->Draw();
					l.DrawLatex(x7[0] + (x7[1] - x7[0]) / 3,
							y7[2] - (y7[2] - y7[0]) / 1.25, Form("%d", 7));

					TPolyLine *pline8 = new TPolyLine(5, x8, y8);
					pline8->SetLineColor(1);
					pline8->SetLineWidth(3);
					pline8->Draw();
					l.DrawLatex(x8[0] + (x8[1] - x8[0]) / 3,
							y8[2] - (y8[2] - y8[0]) / 1.25, Form("%d", 8));

					TPolyLine *pline9 = new TPolyLine(5, x9, y9);
					pline9->SetLineColor(1);
					pline9->SetLineWidth(3);
					pline9->Draw();
					l.DrawLatex(x9[0] + (x9[1] - x9[0]) / 3,
							y9[2] - (y9[2] - y9[0]) / 1.25, Form("%d", 9));

					TPolyLine *pline10 = new TPolyLine(5, x10, y10);
					pline10->SetLineColor(1);
					pline10->SetLineWidth(3);
					pline10->Draw();
					l.DrawLatex(x10[0] + (x10[1] - x10[0]) / 8,
							y10[2] - (y10[2] - y10[0]) / 1.25, Form("%d", 10));
				}
				else if (d == 4)
				{
					TPolyLine *pline1 = new TPolyLine(5, x0, y0);
					pline1->SetLineColor(1);
					pline1->SetLineWidth(3);
					pline1->Draw();

				}

				std::cout << plotName << std::endl;
				c->Print(
						"regions_" + choice + "_" + stripChoice + "_" + dataName
								+ "_" + plotName);
			}
			else
			{

				if (option == 1)
				{
					condition = "&&(fParticleTypeX!=0 && fParticleTypeY!=0)";
					plotName = "all.pdf";
				}
				else if (option == 2)
				{
					condition =
							"&&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX==1 || fParticleTypeY==1)";
					plotName = "XorY_particleType1.pdf";
				}

				else if (option == 3)
				{
					condition = "&&fParticleTypeX==1 && fParticleTypeY==1";
					plotName = "XandY_particleType1.pdf";
				}
				else if (option == 4)
				{
					condition =
							"&&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX!=2 && fParticleTypeY!=2)";
					plotName = "not_particleType2.pdf";
				}
				else if (option == 5)
				{
					condition =
							"&&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX==2 || fParticleTypeY==2)";
					plotName = "XorY_particleType2.pdf";
				}
				else if (option == 6)
				{
					condition = "&&fParticleTypeX==3 && fParticleTypeY ==3";
					plotName = "XandY_particleType3.pdf";

				}

				else if (option == 7)
				{
					condition = "&&fParticleTypeX==2 && fParticleTypeY==2";
					plotName = "XandY_particleType2.pdf";
				}

				else if (option == 8)
				{
					condition =
							"&&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX==3 || fParticleTypeY==3)";
					plotName = "XorY_particleType3.pdf";
				}

				else if (option == 9)
				{
					condition =
							"&&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX!=3 && fParticleTypeY!=3)";
					plotName = "not_particleType3.pdf";
				}
				else if (option == 10)
				{
					condition =
							"&&(fParticleTypeX!=0 && fParticleTypeY!=0)&&(fParticleTypeX!=1 && fParticleTypeY!=1)";
					plotName = "not_particleType1.pdf";
				}

				if (d == 1 || d == 2)
				{
					rot = "";
					rot += inverserot;
					edge = "right";
					xs = x11[0];
					xe = x11[2];
					ys = y11[0];
					ye = y11[2];
					fite = ye;
					fits = ys;

					doFit(t, edge, xs, xe, ys, ye, fits, fite, "1", dataName,
							plotName, "", "", condition, "ys>>h1", "ym>>h2");

					edge = "right";
					xs = x12[0];
					xe = x12[2];
					ys = y12[0];
					ye = y12[2];
					fite = ye;
					fits = ys;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "2", dataName,
							plotName, "", "", condition, "ys>>h1", "ym>>h2");

					edge = "left";
					xs = x13[0];
					xe = x13[2];
					ys = y13[0];
					ye = y13[2];
					fite = xe;
					fits = xs;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "3", dataName,
							plotName, "", "", condition, "xs>>h1", "xm>>h2");

					edge = "left";
					xs = x14[0];
					xe = x14[2];
					ys = y14[0];
					ye = y14[2];
					fite = xe;
					fits = xs;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "4", dataName,
							plotName, "", "", condition, "xs>>h1", "xm>>h2");

					edge = "left";
					xs = x15[0];
					xe = x15[2];
					ys = y15[0];
					ye = y15[2];
					fite = ye;
					fits = ys;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "5", dataName,
							plotName, "", "", condition, "ys>>h1", "ym>>h2");

					edge = "left";
					xs = x16[0];
					xe = x16[2];
					ys = y16[0];
					ye = y16[2];
					fite = ye;
					fits = ys;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "6", dataName,
							plotName, "", "", condition, "ys>>h1", "ym>>h2");

					edge = "right";
					xs = x17[0];
					xe = x17[2];
					ys = y17[0];
					ye = y17[2];
					fite = xe;
					fits = xs;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "7", dataName,
							plotName, "", "", condition, "xs>>h1", "xm>>h2");

					edge = "right";
					xs = x18[0];
					xe = x18[2];
					ys = y18[0];
					ye = y18[2];
					fite = xe;
					fits = xs;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "8", dataName,
							plotName, "", "", condition, "xs>>h1", "xm>>h2");

				}

				else if (d == 3)
				{
					rot = "";
					rot += inverserot;
					/*
					 edge = "left";
					 xs = x1[0];
					 xe = x1[2];
					 ys = y1[0];
					 ye = y1[2];
					 fite = ye;
					 fits = ys;
					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "1", dataName,
					 plotName, "", "", condition, "yfirst>>h1", "ym>>h2");

					 edge = "right";
					 xs = x2[0];
					 xe = x2[2];
					 ys = y2[0];
					 ye = y2[2];
					 fite = ye;
					 fits = ys;
					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "2", dataName,
					 plotName, "", "", condition, "yfirst>>h1", "ym>>h2");
					 */
					edge = "left";
					xs = x3[0];
					xe = x3[2];
					ys = y3[0];
					ye = y3[2];
					fite = ye;
					fits = ys;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "3", dataName,
							plotName, "", "", condition, "yfirst>>h1",
							"ym>>h2");
					/*
					 edge = "right";
					 xs = x4[0];
					 xe = x4[2];
					 ys = y4[0];
					 ye = y4[2];
					 fite = ye;
					 fits = ys;
					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "4", dataName,
					 plotName, "", "", condition, "yfirst>>h1", "ym>>h2");

					 edge = "left";
					 xs = x5n[0];
					 xe = x5n[2];
					 ys = y5n[0];
					 ye = y5n[2];
					 fite = xe;
					 fits = xs;
					 TString cond1 = "(xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) >=";
					 cond1 += xs;
					 cond1 += " && xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) < ";
					 cond1 += xe;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) >= ";
					 cond1 += ys;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) < ";
					 cond1 += ye;
					 cond1 += ")";

					 TString cond2 = "(xm*cos(" + rot + "*pi/180)-ym*sin("
					 + rot + "*pi/180) >=";
					 cond2 += xs;
					 cond2 += " && xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) < ";
					 cond2 += xe;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) >= ";
					 cond2 += ys;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) < ";
					 cond2 += ye;
					 cond2 += ")";

					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "5", dataName,
					 plotName, cond1, cond2, condition,
					 "xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180)>>h1",
					 "xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180)>>h2");

					 edge = "right";
					 xs = x6n[0];
					 xe = x6n[2];
					 ys = y6n[0];
					 ye = y6n[2];
					 fite = xe;
					 fits = xs;
					 cond1 = "(xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) >=";
					 cond1 += xs;
					 cond1 += " && xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) < ";
					 cond1 += xe;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) >= ";
					 cond1 += ys;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) < ";
					 cond1 += ye;
					 cond1 += ")";

					 cond2 = "(xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) >=";
					 cond2 += xs;
					 cond2 += " && xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) < ";
					 cond2 += xe;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) >= ";
					 cond2 += ys;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) < ";
					 cond2 += ye;
					 cond2 += ")";

					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "6", dataName,
					 plotName, cond1, cond2, condition,
					 "xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180)>>h1",
					 "xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180)>>h2");
					 edge = "left";
					 xs = x7n[0];
					 xe = x7n[2];
					 ys = y7n[0];
					 ye = y7n[2];
					 fite = xe;
					 fits = xs;
					 cond1 = "(xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) >=";
					 cond1 += xs;
					 cond1 += " && xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) < ";
					 cond1 += xe;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) >= ";
					 cond1 += ys;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) < ";
					 cond1 += ye;
					 cond1 += ")";

					 cond2 = "(xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) >=";
					 cond2 += xs;
					 cond2 += " && xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) < ";
					 cond2 += xe;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) >= ";
					 cond2 += ys;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) < ";
					 cond2 += ye;
					 cond2 += ")";

					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "7", dataName,
					 plotName, cond1, cond2, condition,
					 "xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180)>>h1",
					 "xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180)>>h2");
					 edge = "right";
					 xs = x8n[0];
					 xe = x8n[2];
					 ys = y8n[0];
					 ye = y8n[2];
					 fite = xe;
					 fits = xs;
					 cond1 = "(xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) >=";
					 cond1 += xs;
					 cond1 += " && xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) < ";
					 cond1 += xe;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) >= ";
					 cond1 += ys;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) < ";
					 cond1 += ye;
					 cond1 += ")";

					 cond2 = "(xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) >=";
					 cond2 += xs;
					 cond2 += " && xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) < ";
					 cond2 += xe;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) >= ";
					 cond2 += ys;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) < ";
					 cond2 += ye;
					 cond2 += ")";

					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "8", dataName,
					 plotName, cond1, cond2, condition,
					 "xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180)>>h1",
					 "xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180)>>h2");
					 edge = "left";
					 xs = x9n[0];
					 xe = x9n[2];
					 ys = y9n[0];
					 ye = y9n[2];
					 fite = xe;
					 fits = xs;
					 cond1 = "(xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) >=";
					 cond1 += xs;
					 cond1 += " && xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) < ";
					 cond1 += xe;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) >= ";
					 cond1 += ys;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) < ";
					 cond1 += ye;
					 cond1 += ")";

					 cond2 = "(xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) >=";
					 cond2 += xs;
					 cond2 += " && xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) < ";
					 cond2 += xe;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) >= ";
					 cond2 += ys;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) < ";
					 cond2 += ye;
					 cond2 += ")";

					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "9", dataName,
					 plotName, cond1, cond2, condition,
					 "xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180)>>h1",
					 "xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180)>>h2");

					 edge = "right";
					 xs = x10n[0];
					 xe = x10n[2];
					 ys = y10n[0];
					 ye = y10n[2];
					 fite = xe;
					 fits = xs;

					 cond1 = "(xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) >=";
					 cond1 += xs;
					 cond1 += " && xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180) < ";
					 cond1 += xe;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) >= ";
					 cond1 += ys;
					 cond1 += " && xs*sin(" + rot + "*pi/180)+ys*cos(" + rot
					 + "*pi/180) < ";
					 cond1 += ye;
					 cond1 += ")";

					 cond2 = "(xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) >=";
					 cond2 += xs;
					 cond2 += " && xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180) < ";
					 cond2 += xe;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) >= ";
					 cond2 += ys;
					 cond2 += " && xm*sin(" + rot + "*pi/180)+ym*cos(" + rot
					 + "*pi/180) < ";
					 cond2 += ye;
					 cond2 += ")";

					 doFit(t, edge, xs, xe, ys, ye, fits, fite, "10", dataName,
					 plotName, cond1, cond2, condition,
					 "xs*cos(" + rot + "*pi/180)-ys*sin(" + rot
					 + "*pi/180)>>h1",
					 "xm*cos(" + rot + "*pi/180)-ym*sin(" + rot
					 + "*pi/180)>>h2");
					 */
				}
				else if (d == 4)
				{

					edge = "left";

					xs = x0[0];
					xe = x0[2];
					ys = y0[0];
					ye = y0[2];
					fite = xe;
					fits = xs;
					doFit(t, edge, xs, xe, ys, ye, fits, fite, "1", dataName,
							plotName, "", "", condition, "xs>>h1", "x0>>h1");

				}

			}
		}

	}
}

