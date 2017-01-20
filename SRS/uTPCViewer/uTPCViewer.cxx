#define START_PATH "/Users/dpfeiffe/programming/ESS/apv2root/"

#include <stdlib.h>

#include "Riostream.h"
#include "TROOT.h"
#include "RVersion.h"
#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TSystemFile.h>
#include <TSystemDirectory.h>
#include <TGFileDialog.h>

#include <vector>
#include <iostream>
#include <map>
#include <unistd.h>

#include <TColor.h>
#include <TLine.h>
#include <TMarker.h>
#include <libgen.h> 
#include "uTPCViewer.h"

#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TMath.h"

enum ETestCommandIdentifiers
{
	M_FILE_OPEN, M_FILE_CLOSE, M_FILE_EXIT
};

const char *filetypes[] =
{ "ROOT files", "*.root", 0, 0 };

const Int_t colors[15] =
{ kGreen, kGreen + 1, kGreen + 2, kCyan - 7, kCyan - 3, kCyan + 2, kBlue - 7,
		kBlue, kBlue + 2, kMagenta - 7, kMagenta + 1, kMagenta + 2, kRed - 7,
		kRed, kRed + 2, };

uTPCViewer::uTPCViewer(const TGWindow *p, UInt_t w, UInt_t h) :
		TGMainFrame(p, w, h)
{

	int scaleGUI = 2;

	SetLayoutManager(new TGVerticalLayout(this));

	vframeTop = new TGHorizontalFrame(this, 800, 10);
	this->AddFrame(vframeTop, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
	vframeTop->SetLayoutManager(new TGHorizontalLayout(vframeTop));

	// tab widget
	fTab = new TGTab(this, 800, 1200);

	fCompositeFrame11 = fTab->AddTab("Tracks");
	fCompositeFrame11->SetLayoutManager(
			new TGVerticalLayout(fCompositeFrame11));

	fCompositeFrame21 = fTab->AddTab("Profiles");
	fCompositeFrame21->SetLayoutManager(
			new TGVerticalLayout(fCompositeFrame21));

	fCompositeFrame31 = fTab->AddTab("Spectra");
	fCompositeFrame31->SetLayoutManager(
			new TGVerticalLayout(fCompositeFrame31));

	fCompositeFrame41 = fTab->AddTab("Tree");
	fCompositeFrame41->SetLayoutManager(
			new TGVerticalLayout(fCompositeFrame41));

	this->AddFrame(fTab,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

	fTab->SetTab(0);
	fTab->Resize(fTab->GetDefaultSize());

	vframeBottom = new TGVerticalFrame(this, 800, 50);
	this->AddFrame(vframeBottom, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

	vframeTop11 = new TGVerticalFrame(fCompositeFrame11, 800, 150);
	vframeControls11 = new TGHorizontalFrame(fCompositeFrame11, 700, 75);
	vframeMiddle11 = new TGVerticalFrame(fCompositeFrame11, 800, 300);
	vframeMiddle12 = new TGVerticalFrame(fCompositeFrame11, 800, 300);

	vframeMiddle21 = new TGVerticalFrame(fCompositeFrame21, 800, 300);
	vframeMiddle22 = new TGVerticalFrame(fCompositeFrame21, 800, 300);


	fCompositeFrame11->AddFrame(vframeTop11,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
	fCompositeFrame11->AddFrame(vframeMiddle11,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	fCompositeFrame11->AddFrame(vframeMiddle12,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

	fCompositeFrame21->AddFrame(vframeMiddle21,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	fCompositeFrame21->AddFrame(vframeMiddle22,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

	vframeTop11->SetLayoutManager(new TGVerticalLayout(vframeTop11));
	vframeTop11->AddFrame(vframeControls11,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
	vframeControls11->SetLayoutManager(
			new TGHorizontalLayout(vframeControls11));

	fMenuFile = new TGPopupMenu(gClient->GetRoot());

	fMenuFile->AddEntry("&Open file...", M_FILE_OPEN);
	//fMenuFile->AddEntry("&Analyse file...", M_FILE_ANALYZE);
	//fMenuFile->AddEntry("S&ave pictures as...", M_FILE_SAVEAS);
	fMenuFile->AddEntry("&Close file", M_FILE_CLOSE);

	fMenuFile->AddSeparator();

	fMenuFile->AddEntry("E&xit", M_FILE_EXIT);

	fMBItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
	fMenuBar = new TGMenuBar(vframeTop, 100, 20, kHorizontalFrame);
	fMenuBar->AddPopup("&File", fMenuFile, fMBItemLayout);
	vframeTop->AddFrame(fMenuBar,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
	fMenuFile->Associate(this);

	fLabelThresholdAmplitude = new TGLabel(vframeControls11,
			"Threshold amplitude [ADC counts]:");

	fLabelThresholdDiscard = new TGLabel(vframeControls11,
			"Threshold Discard [points]:");

	fThresholdAmplitude = new TGNumberEntry(vframeControls11, 150, 4, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive,
			TGNumberFormat::kNELLimitMinMax, 1., 1000);

	fThresholdDiscard = new TGNumberEntry(vframeControls11, 200, 3, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive,
			TGNumberFormat::kNELLimitMinMax, 1., 999);

	fThresholdAmplitude->Connect("ValueSet(Long_t)", "uTPCViewer", this,
			"DoNumberEntry(Int_t)");
	fThresholdAmplitude->Connect("ValueChanged(Long_t)", "uTPCViewer", this,
			"DoNumberEntry(Int_t)");
	fThresholdAmplitude->GetNumberEntry()->Connect("ReturnPressed()",
			"uTPCViewer", this, "DoNumberEntry(Int_t=0)");

	fThresholdDiscard->Connect("ValueSet(Long_t)", "uTPCViewer", this,
			"DoNumberEntry(Int_t)");
	fThresholdDiscard->Connect("ValueChanged(Long_t)", "uTPCViewer", this,
			"DoNumberEntry(Int_t)");
	fThresholdDiscard->GetNumberEntry()->Connect("ReturnPressed()",
			"uTPCViewer", this, "DoNumberEntry(Int_t=0)");

	vframeControls11->AddFrame(fLabelThresholdAmplitude,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 15, 5, 20, 5));
	vframeControls11->AddFrame(fThresholdAmplitude,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 20, 5));

	vframeControls11->AddFrame(fLabelThresholdDiscard,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 20, 5));
	vframeControls11->AddFrame(fThresholdDiscard,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 20, 5));

	fEventNr = new TGNumberEntry(vframeControls11, 0, 8, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive,
			TGNumberFormat::kNELLimitMinMax, 1., 10000000);

	fEventNr->Connect("ValueSet(Long_t)", "uTPCViewer", this,
			"DoNumberEntry(Int_t)");
	fEventNr->Connect("ValueChanged(Long_t)", "uTPCViewer", this,
			"DoNumberEntry(Int_t)");
	fEventNr->GetNumberEntry()->Connect("ReturnPressed()", "uTPCViewer", this,
			"DoNumberEntry(Int_t=0)");

	fLabelEvent = new TGLabel(vframeControls11, "Event");
	fLabelEventTotal = new TGLabel(vframeControls11, "of 1000000 events");

	vframeControls11->AddFrame(fLabelEvent,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 15, 5, 20, 5));
	vframeControls11->AddFrame(fEventNr,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 20, 5));
	vframeControls11->AddFrame(fLabelEventTotal,
			new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 20, 5));

	vframeMiddle11->SetLayoutManager(new TGHorizontalLayout(vframeMiddle11));
	vframeMiddle12->SetLayoutManager(new TGHorizontalLayout(vframeMiddle12));
	vframeMiddle21->SetLayoutManager(new TGHorizontalLayout(vframeMiddle21));
	vframeMiddle22->SetLayoutManager(new TGHorizontalLayout(vframeMiddle22));

	vframe11 = new TGVerticalFrame(vframeMiddle11, 600 * scaleGUI,
			250 * scaleGUI);
	vframe12 = new TGVerticalFrame(vframeMiddle11, 600 * scaleGUI,
			250 * scaleGUI);
	vframe13 = new TGVerticalFrame(vframeMiddle12, 600 * scaleGUI,
			250 * scaleGUI);
	vframe14 = new TGVerticalFrame(vframeMiddle12, 600 * scaleGUI,
			250 * scaleGUI);
	vframe21 = new TGVerticalFrame(vframeMiddle21, 600 * scaleGUI,
			250 * scaleGUI);
	vframe22 = new TGVerticalFrame(vframeMiddle21, 600 * scaleGUI,
			250 * scaleGUI);
	vframe23 = new TGVerticalFrame(vframeMiddle22, 600 * scaleGUI,
			250 * scaleGUI);
	vframe24 = new TGVerticalFrame(vframeMiddle22, 600 * scaleGUI,
			250 * scaleGUI);

	vframeControls21 = new TGHorizontalFrame(vframe21, 600 * scaleGUI,
			50 * scaleGUI);
	vframeControls22 = new TGHorizontalFrame(vframe21, 600 * scaleGUI,
			50 * scaleGUI);
	vframeControls23 = new TGHorizontalFrame(vframe21, 600 * scaleGUI,
			50 * scaleGUI);
	vframeControls24 = new TGHorizontalFrame(vframe21, 600 * scaleGUI,
			50 * scaleGUI);
	vframeControls25 = new TGHorizontalFrame(vframe21, 600 * scaleGUI,
			50 * scaleGUI);
	vframeControls26 = new TGHorizontalFrame(vframe21, 600 * scaleGUI,
			50 * scaleGUI);

	vframeMiddle11->AddFrame(vframe11,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeMiddle11->AddFrame(vframe12,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeMiddle12->AddFrame(vframe13,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeMiddle12->AddFrame(vframe14,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

	vframeMiddle21->AddFrame(vframe21,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeMiddle21->AddFrame(vframe22,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeMiddle22->AddFrame(vframe23,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeMiddle22->AddFrame(vframe24,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

	fLabel11 = new TGLabel(vframe11, "x Clusters");
	vframe11->AddFrame(fLabel11, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas11 = new TRootEmbeddedCanvas("ec11", vframe11, 600 * scaleGUI,
			250 * scaleGUI);
	vframe11->AddFrame(fEcanvas11,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	fLabel12 = new TGLabel(vframe12, "y Clusters");
	vframe12->AddFrame(fLabel12, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas12 = new TRootEmbeddedCanvas("ec12", vframe12, 600 * scaleGUI,
			250 * scaleGUI);
	vframe12->AddFrame(fEcanvas12,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	fLabel13 = new TGLabel(vframe13, "x Time Structure");
	vframe13->AddFrame(fLabel13, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas13 = new TRootEmbeddedCanvas("ec13", vframe13, 600 * scaleGUI,
			250 * scaleGUI);
	vframe13->AddFrame(fEcanvas13,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	fLabel14 = new TGLabel(vframe14, "y Time Structure");
	vframe14->AddFrame(fLabel14, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas14 = new TRootEmbeddedCanvas("ec14", vframe14, 600 * scaleGUI,
			250 * scaleGUI);
	vframe14->AddFrame(fEcanvas14,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	fLabel21 = new TGLabel(vframeControls21, "Parameters");
	fLabelVariableName = new TGLabel(vframeControls22, "Variable name");
	fLabelX1 = new TGLabel(vframeControls23, "x start");
	fLabelX2 = new TGLabel(vframeControls23, "x end");
	fLabelY1 = new TGLabel(vframeControls24, "y start");
	fLabelY2 = new TGLabel(vframeControls24, "y end");
	fLabelSynchronize = new TGLabel(vframeControls25, "Synchronize views");
	fLabelFit = new TGLabel(vframeControls26, "Fit type");
	fListBoxVariables = new TGComboBox(vframeControls22, -1,
			kHorizontalFrame | kSunkenFrame | kOwnBackground);
	fListBoxVariables->SetName("fListBoxVariables");
	fListBoxVariables->AddEntry("xmax:ymax", 0);
	fListBoxVariables->AddEntry("xs:ys", 1);
	fListBoxVariables->AddEntry("x0:y0", 2);
	fListBoxVariables->AddEntry("xm:ym", 3);
	fListBoxVariables->AddEntry("xi:yi", 4);
	fListBoxVariables->AddEntry("xd:yd", 5);
	fListBoxVariables->AddEntry("xf:yf", 6);
	fListBoxVariables->AddEntry("stripX:stripY", 7);
	fListBoxVariables->Resize(60, 21);
	fListBoxVariables->Select(-1);
	fListBoxVariables->Connect("Selected(Int_t)", "uTPCViewer", this,
			"ShowHitmap()");
	fListBoxVariables->Connect("Selected(char*)", "uTPCViewer", this,
			"ShowHitmap()");

	fComboBoxFit = new TGComboBox(vframeControls26, -1,
			kHorizontalFrame | kSunkenFrame | kOwnBackground);
	fComboBoxFit->AddEntry("erfc(x)", 0);
	fComboBoxFit->AddEntry("erfc(-x)", 1);
	fComboBoxFit->AddEntry("double erfc", 2);
	fComboBoxFit->AddEntry("cylinder", 3);
	fComboBoxFit->Resize(60, 21);
	fComboBoxFit->Select(-1);
	fX1 = new TGNumberEntry(vframeControls23, 0, 3, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
			TGNumberFormat::kNELLimitMinMax, 0, 256);
	fX2 = new TGNumberEntry(vframeControls23, 256, 3, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
			TGNumberFormat::kNELLimitMinMax, 0, 256);
	fY1 = new TGNumberEntry(vframeControls24, 0, 3, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
			TGNumberFormat::kNELLimitMinMax, 0, 256);
	fY2 = new TGNumberEntry(vframeControls24, 256, 3, 1,
			TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
			TGNumberFormat::kNELLimitMinMax, 0, 256);
	fCheckButtonSynchronize = new TGCheckButton(vframeControls25,
			"fCheckButtonSynchronize");
	fCheckButtonSynchronize->SetTextJustify(36);
	fCheckButtonSynchronize->SetMargins(0, 0, 0, 0);
	fCheckButtonSynchronize->SetWrapLength(-1);
	fX1->Connect("ValueSet(Long_t)", "uTPCViewer", this, "zoomAll()");
	fX1->Connect("ValueChanged(Long_t)", "uTPCViewer", this, "zoomAll()");
	fX1->GetNumberEntry()->Connect("ReturnPressed()", "uTPCViewer", this,
			"zoomAll()");
	fX2->Connect("ValueSet(Long_t)", "uTPCViewer", this, "zoomAll()");
	fX2->Connect("ValueChanged(Long_t)", "uTPCViewer", this, "zoomAll()");
	fX2->GetNumberEntry()->Connect("ReturnPressed()", "uTPCViewer", this,
			"zoomAll()");
	fY1->Connect("ValueSet(Long_t)", "uTPCViewer", this, "zoomAll()");
	fY1->Connect("ValueChanged(Long_t)", "uTPCViewer", this, "zoomAll()");
	fY1->GetNumberEntry()->Connect("ReturnPressed()", "uTPCViewer", this,
			"zoomAll()");
	fY2->Connect("ValueSet(Long_t)", "uTPCViewer", this, "zoomAll()");
	fY2->Connect("ValueChanged(Long_t)", "uTPCViewer", this, "zoomAll()");
	fY2->GetNumberEntry()->Connect("ReturnPressed()", "uTPCViewer", this,
			"zoomAll()");

	vframeControls21->AddFrame(fLabel21,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls22->AddFrame(fLabelVariableName,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
	vframeControls22->AddFrame(fListBoxVariables,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls23->AddFrame(fLabelX1,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls23->AddFrame(fX1,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls23->AddFrame(fLabelX2,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls23->AddFrame(fX2,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls24->AddFrame(fLabelY1,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls24->AddFrame(fY1,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls24->AddFrame(fLabelY2,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls24->AddFrame(fY2,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls25->AddFrame(fLabelSynchronize,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls25->AddFrame(fCheckButtonSynchronize,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls26->AddFrame(fLabelFit,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframeControls26->AddFrame(fComboBoxFit,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));

	vframe21->AddFrame(vframeControls21,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframe21->AddFrame(vframeControls22,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframe21->AddFrame(vframeControls23,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframe21->AddFrame(vframeControls24,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframe21->AddFrame(vframeControls25,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	vframe21->AddFrame(vframeControls26,
			new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	//fEventNr = new TGNumberEntry(vframeControls11, 0, 8, 1,
	//			TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive,
	//			TGNumberFormat::kNELLimitMinMax, 1., 10000000);

	fLabel22 = new TGLabel(vframe22, "Projection x");
	vframe22->AddFrame(fLabel22, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas22 = new TRootEmbeddedCanvas("ec22", vframe22, 600 * scaleGUI,
			250 * scaleGUI);
	vframe22->AddFrame(fEcanvas22,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	fLabel23 = new TGLabel(vframe23, "Projection y");
	vframe23->AddFrame(fLabel23, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas23 = new TRootEmbeddedCanvas("ec23", vframe23, 600 * scaleGUI,
			250 * scaleGUI);
	vframe23->AddFrame(fEcanvas23,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	fLabel24 = new TGLabel(vframe24, "x:y");
	vframe24->AddFrame(fLabel24, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 1));
	fEcanvas24 = new TRootEmbeddedCanvas("ec24", vframe24, 600 * scaleGUI,
			250 * scaleGUI);
	vframe24->AddFrame(fEcanvas24,
			new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 1));

	Int_t parts[] =
	{ 60, 20, 20 };
	fStatusBar = new TGStatusBar(vframeBottom, 800 * scaleGUI, 10,
			kHorizontalFrame);
	fStatusBar->SetParts(parts, 3);
	fStatusBar->SetText("Filename", 0);
	fStatusBar->SetText("Event shown", 1);
	fStatusBar->SetText("Number of events", 2);

	vframeBottom->AddFrame(fStatusBar,
			new TGLayoutHints(kLHintsExpandX, 20, 5, 5, 5));

	fEcanvas11->GetCanvas()->SetBorderMode(0);
	fEcanvas12->GetCanvas()->SetBorderMode(0);
	fEcanvas13->GetCanvas()->SetBorderMode(0);
	fEcanvas14->GetCanvas()->SetBorderMode(0);

	//fEcanvas21->GetCanvas()->SetBorderMode(0);
	fEcanvas22->GetCanvas()->SetBorderMode(0);

	SetWindowName("uTPCViewer");
	MapSubwindows();

	vframeTop11->Resize();
	vframeMiddle11->Resize();
	vframeMiddle12->Resize();
	vframeBottom->Resize();
	vframeMiddle21->Resize();
	vframeMiddle22->Resize();

	Resize(GetDefaultSize());
	MapWindow();

	fStatusBar->SetText("", 0);
	fStatusBar->SetText("", 1);
	fStatusBar->SetText("", 2);
	rawData = 0;
	hx = 0;
	hy = 0;
	h2x = 0;
	h2y = 0;
	h2xy = 0;
	hprojx = 0;
	hprojy = 0;
	plotData = new uTPCPlotData();
	plotData->fThresholdAmplitude = fThresholdAmplitude->GetNumber();
	plotData->fThresholdDiscard = fThresholdDiscard->GetNumber();

}

void uTPCViewer::DoNumberEntry(Int_t pos)
{
	ShowEvent(fEventNr->GetNumber());
	TString s1 = "Event ";
	s1 += fEventNr->GetNumber();
	s1 += " shown";
	fStatusBar->SetText(s1, 1);
	plotData->fThresholdAmplitude = fThresholdAmplitude->GetNumber();
	plotData->fThresholdDiscard = fThresholdDiscard->GetNumber();

}

void show()
{
	new uTPCViewer(gClient->GetRoot(), 800, 800);
}

void uTPCViewer::CloseWindow()
{
	CloseFile();
	gApplication->Terminate(0);
}

Bool_t uTPCViewer::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
	switch (GET_MSG(msg))
	{

	case kC_COMMAND:
		switch (GET_SUBMSG(msg))
		{

		case kCM_MENU:
			switch (parm1)
			{

			case M_FILE_OPEN:
			{
				static TString dir(START_PATH);
				TGFileInfo fi;
				fi.fFileTypes = filetypes;
				fi.fIniDir = StrDup(dir);
				new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
				printf("Open file: %s (dir: %s)\n", fi.fFilename, fi.fIniDir);
				dir = fi.fIniDir;
				if (fi.fFilename)
					OpenFile(dir, basename(fi.fFilename), 0);

			}
				break;

			case M_FILE_CLOSE:
				CloseFile();
				break;

			case M_FILE_EXIT:
				CloseWindow();
				break;

			default:
				break;
			}
		default:
			break;
		}
	default:
		break;
	}

	return kTRUE;
}

void uTPCViewer::OpenFile(TString dir, TString fileName, Bool_t analyze)
{
	CloseFile();
	rawData = new uTPCRawData();

	int numEvents = rawData->importData(dir, fileName, analyze, 0,
			this->plotData, this);

	if (numEvents > 0)
	{
		fStatusBar->SetText(fileName, 0);
		TString s1 = "Event ";
		s1 += 1;
		s1 += " shown";
		TString s2 = "Number of events: ";
		s2 += numEvents;
		fStatusBar->SetText(s1, 1);
		fStatusBar->SetText(s2, 2);
		fEventNr->SetLimitValues(1, numEvents);

		TString s3 = "of ";
		s3 += numEvents;
		s3 += " events";
		fLabelEventTotal->SetText(s3);

		ShowEvent(1);
	}
}

void uTPCViewer::CloseFile()
{
	fStatusBar->SetText("", 0);
	fStatusBar->SetText("", 1);
	fStatusBar->SetText("", 2);

	if (h2x)
	{
		h2x->Delete();
		h2x = 0;
	}
	if (hx)
	{
		hx->Delete();
		hx = 0;
	}

	if (h2y)
	{
		h2y->Delete();
		h2y = 0;
	}
	if (hy)
	{
		hy->Delete();
		hy = 0;
	}
	if (h2xy)
	{
		h2xy->Delete();
		h2xy = 0;
	}
	if (hprojx)
	{
		hprojx->Delete();
		hprojx = 0;
	}
	if (hprojy)
	{
		hprojy->Delete();
		hprojy = 0;
	}

	if (rawData != 0)
	{
		delete rawData;
	}
	rawData = 0;

}

void uTPCViewer::ShowHitmap()
{

	if (h2xy)
	{
		h2xy->Delete();
		h2xy = 0;
	}
	if (hprojx)
	{
		hprojx->Delete();
		hprojx = 0;
	}
	if (hprojy)
	{
		hprojy->Delete();
		hprojy = 0;
	}

	TTree* t = rawData->getInputTree();

	h2xy = new TH2F("h2xy", ";x [0.4 mm];y [0.4 mm]", 256, 0., 256, 256, 0.,
			256);
	hprojx = new TH1F("hprojx", ";x [0.4 mm]", 256, 0., 256);
	hprojy = new TH1F("hprojy", ";y [0.4 mm]", 256, 0., 256);

	h2xy->SetStats(0);

	TString variableX, variableY;
	TString stringX, stringY, stringXY;
	TGTextLBEntry *filePointer =
			(TGTextLBEntry *) fListBoxVariables->GetSelectedEntry();
	TString variable = filePointer->GetTitle();
	TString cond1 = "";
		TString cond2 = "";

		TString cond3 = "";

	if (variable == "xmax:ymax")
	{
		variableX = "xmax";
		variableY = "ymax";
	}
	else if (variable == "xs:ys")
	{
		variableX = "xs";
		variableY = "ys";
	}
	else if (variable == "x0:y0")
	{
		variableX = "x0";
		variableY = "y0";
	}
	else if (variable == "xf:yf")
	{
		variableX = "xf";
		variableY = "yf";
	}
	else if (variable == "xd:yd")
	{
		variableX = "xd";
		variableY = "yd";
	}
	else if (variable == "xm:ym")
	{
		variableX = "xm";
		variableY = "ym";
	}
	else if (variable == "xi:yi")
	{
		variableX = "xi";
		variableY = "yi";
	}
	else if (variable == "stripX:stripY")
	{
		cond1 = "planeID==0";
		cond2 = "planeID==1";
		variableX = "strip";
		variableY = "strip";
	}
	else
	{
		return;
	}
	stringX = variableX + ">>hprojx";
	stringY = variableY + ">>hprojy";
	stringXY = variableY + ":" + variableX + ">>h2xy";

	fCanvas = fEcanvas23->GetCanvas();
	fCanvas->cd();
	t->Draw(stringY, cond1, "HIST");
	hprojy->SetTitle("");
	fCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "uTPCViewer",
			this, "zoomPads2DY(Int_t,Int_t,Int_t,TObject*)");
	fCanvas->Update();

	fCanvas = fEcanvas22->GetCanvas();
	fCanvas->cd();
	t->Draw(stringX, cond2, "HIST");
	hprojx->SetTitle("");
	fCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "uTPCViewer",
			this, "zoomPads2DX(Int_t,Int_t,Int_t,TObject*)");
	fCanvas->Update();

	fCanvas = fEcanvas24->GetCanvas();
	fCanvas->cd();
	t->Draw(stringXY, cond3, "COLZ");
	h2xy->SetTitle("");
	fCanvas->SetLogz();
	fCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "uTPCViewer",
			this, "zoomPadsXY(Int_t,Int_t,Int_t,TObject*)");
	fCanvas->Update();
	zoomAll();
}

void uTPCViewer::ShowEvent(int eventNr)
{

	if (!rawData)
		return;

	if (h2x)
	{
		h2x->Delete();
		h2x = 0;
	}
	if (hx)
	{
		hx->Delete();
		hx = 0;
	}

	if (h2y)
	{
		h2y->Delete();
		h2y = 0;
	}
	if (hy)
	{
		hy->Delete();
		hy = 0;
	}

	int chkx = 0;
	int chky = 0;

	h2x = new TH2F("h2x", ";x strip [0.4 mm];time bin [25 ns]", 260, 0., 260.,
			30, 0., 30.);
	h2y = new TH2F("h2y", ";y strip [0.4 mm];time bin [25 ns]", 260, 0., 260.,
			30, 0., 30.);
	h2x->SetStats(0);
	h2y->SetStats(0);
	rawData->selectEvent(eventNr, fThresholdAmplitude->GetNumber(), 0, 0, 0,
			fThresholdDiscard->GetNumber());

	rawData->analyzeEvent(plotData);

	uTPCEvent* evt = rawData->getEvent();

	bool xDraw = 1;
	bool yDraw = 1;

	std::map<unsigned int, unsigned int> clusterX = rawData->getClusterX();
	std::map<unsigned int, unsigned int> clusterY = rawData->getClusterY();

	if (clusterX.size() == 0)
	{
		xDraw = 0;
	}
	if (clusterY.size() == 0)
	{
		yDraw = 0;
	}

	int stripXMin = 512, stripXMax = -512, stripYMin = 512, stripYMax = -512;

	if (xDraw)
	{
		for (std::map<unsigned int, unsigned int>::iterator it =
				clusterX.begin(); it != clusterX.end(); ++it)
		{

			if (stripXMin > rawData->getStripNumber(it->second))
			{
				stripXMin = rawData->getStripNumber(it->second);
			}
			if (stripXMax < rawData->getStripNumber(it->second))
			{
				stripXMax = rawData->getStripNumber(it->second);
			}
			//if (it->second == 5)
			//{

			hx = new TH1F("hx", ";time bin [25 ns];charge [ADC counts]", 30, 0.,
					30.);
			hx->SetStats(0);
			hx->GetYaxis()->SetRangeUser(0, 1.1 * rawData->getMaxX());
			hx->GetYaxis()->SetTitleOffset(1.5);
			for (int timeBin = 0;
					timeBin < rawData->getWaveformSize(it->second); timeBin++)
			{

				h2x->Fill(rawData->getStripNumber(it->second) + 0.5,
						timeBin + 0.5,
						rawData->getWaveformAtTimeBin(it->second, timeBin));
				hx->Fill(timeBin + 0.5,
						rawData->getWaveformAtTimeBin(it->second, timeBin));

			}

			fCanvas = fEcanvas13->GetCanvas();
			fCanvas->cd();
			fCanvas->SetRightMargin(0.05);
			hx->SetLineColor(colors[chkx % 15]);
			//hx->SetLineColor(1);
			hx->SetLineWidth(3);
			if (!chkx)
			{
				hx->DrawCopy("hist");
			}
			else
			{
				hx->DrawCopy("hist same");
			}
			chkx++;
			fCanvas->Update();
			if (hx)
			{
				hx->Delete();
				hx = 0;
			}
			//}
		}

	}

	if (yDraw)
	{
		//int dummy = 0;
		for (std::map<unsigned int, unsigned int>::iterator it =
				clusterY.begin(); it != clusterY.end(); ++it)
		{
			if (stripYMin > rawData->getStripNumber(it->second))
			{
				stripYMin = rawData->getStripNumber(it->second);
			}
			if (stripYMax < rawData->getStripNumber(it->second))
			{
				stripYMax = rawData->getStripNumber(it->second);
			}
			//dummy++;
			//if (dummy == 18)
			//{

			hy = new TH1F("hy", ";time bin [25 ns];charge [ADC counts]", 30, 0.,
					30.);
			//hy = new TH1F("hy", ";;", 30, 0.,30.);
			hy->SetStats(0);
			hy->GetYaxis()->SetRangeUser(0, 1.1 * rawData->getMaxY());
			hy->GetYaxis()->SetTitleOffset(1.5);

			for (int timeBin = 0;
					timeBin < rawData->getWaveformSize(it->second); timeBin++)
			{

				h2y->Fill(rawData->getStripNumber(it->second) + 0.5,
						timeBin + 0.5,
						rawData->getWaveformAtTimeBin(it->second, timeBin));
				hy->Fill(timeBin + 0.5,
						rawData->getWaveformAtTimeBin(it->second, timeBin));

			}
			fCanvas = fEcanvas14->GetCanvas();
			fCanvas->cd();
			fCanvas->SetRightMargin(0.05);
			hy->SetLineColor(colors[chky % 15]);
			//hy->SetLineColor(1);
			hy->SetLineWidth(3);
			if (!chky)
			{
				hy->DrawCopy("hist");
			}
			else
			{
				hy->DrawCopy("hist same");
			}

			chky++;
			fCanvas->Update();
			if (hy)
			{
				hy->Delete();
				hy = 0;
			}
			//}
		}

	}

	fCanvas = fEcanvas11->GetCanvas();
	fCanvas->cd();

	if (rawData->getMinX() >= 0)
	{
		h2x->GetZaxis()->SetRangeUser(-1, rawData->getMaxX());
	}
	h2x->Draw("colz");
	fCanvas->SetRightMargin(0.17);
	h2x->GetZaxis()->SetTitle("charge [ADC counts]");
	h2x->GetZaxis()->SetTitleOffset(1.5);
	fCanvas->Update();

	TMarker* mX;

	for (int z = 0; z < plotData->fPointClustersX.size(); z++)
	{
		int strip = plotData->fStripMaximaX[plotData->fPointClustersX[z]];
		int time = plotData->fTimeMaximaX[plotData->fPointClustersX[z]];

		mX = new TMarker();

		mX->SetX(strip + 0.5);
		mX->SetY(time + 0.5);

		if (strip == evt->getClusterStripStartX()
				&& time == evt->getClusterTimeStartX())
		{
			mX->SetMarkerSize(1);

		}
		else
		{
			mX->SetMarkerSize(0.5);

		}

		mX->SetMarkerStyle(20);
		fCanvas = fEcanvas11->GetCanvas();
		fCanvas->cd();
		mX->Draw();

	}

	fCanvas = fEcanvas11->GetCanvas();
	fCanvas->cd();
	double minX = 0;
	double maxX = stripXMax + 2.5;

	if (stripXMin >= 1.5)
	{
		minX = stripXMin - 1.5;
	}
	else
	{
		minX = 0;
	}
	h2x->GetXaxis()->SetRangeUser(minX, maxX);
	fCanvas->Update();

	fCanvas = fEcanvas12->GetCanvas();
	fCanvas->cd();
	if (rawData->getMinY() >= 0)
	{
		h2y->GetZaxis()->SetRangeUser(-1, rawData->getMaxX());

	}
	h2y->Draw("colz");
	fCanvas->SetRightMargin(0.17);
	h2y->GetZaxis()->SetTitle("charge [ADC counts]");
	h2y->GetZaxis()->SetTitleOffset(1.5);
	fCanvas->Update();

	TMarker * mY;

	for (int z = 0; z < plotData->fPointClustersY.size(); z++)
	{
		int strip = plotData->fStripMaximaY[plotData->fPointClustersY[z]];
		int time = plotData->fTimeMaximaY[plotData->fPointClustersY[z]];

		mY = new TMarker();
		mY->SetX(strip + 0.5);
		mY->SetY(time + 0.5);
		if (strip == evt->getClusterStripStartY()
				&& time == evt->getClusterTimeStartY())
		{
			mY->SetMarkerSize(1);

		}
		else
		{
			mY->SetMarkerSize(0.5);

		}

		mY->SetMarkerStyle(20);
		fCanvas = fEcanvas12->GetCanvas();
		fCanvas->cd();
		mY->Draw();

	}

	fCanvas = fEcanvas12->GetCanvas();
	fCanvas->cd();
	maxX = stripYMax + 2.5;
	if (stripYMin >= 1.5)
	{
		minX = stripYMin - 1.5;
	}
	else
	{
		minX = 0;
	}

	h2y->GetXaxis()->SetRangeUser(minX, maxX);
	fCanvas->Update();

	fCanvas = fEcanvas13->GetCanvas();
	fCanvas->cd();
	if (chkx == 0)
	{
		hx = new TH1F("hx", ";time bin [25 ns];charge [ADC counts]", 30, 0.,
				30.);
		hx->SetStats(0);
		hx->GetYaxis()->SetTitleOffset(1.5);
		hx->DrawCopy();
	}
	fCanvas->Update();

	fCanvas = fEcanvas14->GetCanvas();
	fCanvas->cd();
	if (chky == 0)
	{
		hy = new TH1F("hy", ";time bin [25 ns];charge [ADC counts]", 30, 0.,
				30.);
		hy->SetStats(0);
		hy->GetYaxis()->SetTitleOffset(1.5);
		hy->DrawCopy();
	}

	fCanvas->Update();
	plotData->clearDataStructures();

}

/////////////////////// functions ///////////////////////////////////
//___________________________________________________________________

void uTPCViewer::zoomAll()
{
	Double_t x1 = fX1->GetNumber();
	Double_t x2 = fX2->GetNumber();
	Double_t y1 = fY1->GetNumber();
	Double_t y2 = fY2->GetNumber();
	fCanvas = fEcanvas22->GetCanvas();
	fCanvas->cd();
	hprojx->GetXaxis()->SetRangeUser(x1, x2);
	fCanvas->Modified();
	fCanvas->Update();
	fCanvas = fEcanvas23->GetCanvas();
	fCanvas->cd();
	hprojy->GetXaxis()->SetRangeUser(y1, y2);
	fCanvas->Modified();
	fCanvas->Update();
	fCanvas = fEcanvas24->GetCanvas();
	fCanvas->cd();
	h2xy->GetXaxis()->SetRangeUser(x1, x2);
	h2xy->GetYaxis()->SetRangeUser(y1, y2);
	fCanvas->Modified();
	fCanvas->Update();
}

void uTPCViewer::zoomPadsXY(Int_t event, Int_t px, Int_t py, TObject *)
{

	if (event == kButton1Up)
	{

		TCanvas *c = (TCanvas *) gTQSender;
		TPad *pad = (TPad *) c->GetSelectedPad();
		if (!pad)
			return;
		Double_t x1, x2, y1, y2;
		pad->GetRangeAxis(x1, y1, x2, y2);
		fCanvas = fEcanvas22->GetCanvas();
		fCanvas->cd();
		hprojx->GetXaxis()->SetRangeUser(x1, x2);
		fCanvas->Modified();
		fCanvas->Update();
		fCanvas = fEcanvas23->GetCanvas();
		fCanvas->cd();
		hprojy->GetXaxis()->SetRangeUser(y1, y2);
		fCanvas->Modified();
		fCanvas->Update();
		fX1->SetNumber(x1);
		fX2->SetNumber(x2);
		fY1->SetNumber(y1);
		fY2->SetNumber(y2);

	}
	else if (event == kButton1Double)
	{
		TCanvas *c = (TCanvas *) gTQSender;
		TPad *pad = (TPad *) c->GetSelectedPad();
		if (!pad)
			return;
		fCanvas = fEcanvas22->GetCanvas();
		fCanvas->cd();
		hprojx->GetXaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fCanvas = fEcanvas23->GetCanvas();
		fCanvas->cd();
		hprojy->GetXaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fCanvas = fEcanvas24->GetCanvas();
		fCanvas->cd();
		h2xy->GetXaxis()->SetRangeUser(0, 256);
		h2xy->GetYaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fX1->SetNumber(0);
		fX2->SetNumber(256);
		fY1->SetNumber(0);
		fY2->SetNumber(256);
	}

}

void uTPCViewer::zoomPads2DX(Int_t event, Int_t px, Int_t py, TObject *)
{

	if (event == kButton1Up)
	{

		TCanvas *c = (TCanvas *) gTQSender;
		TPad *pad = (TPad *) c->GetSelectedPad();
		if (!pad)
			return;
		Double_t x1, x2, y1, y2;
		pad->GetRangeAxis(x1, y1, x2, y2);
		printf("X x1=%.3g, y1=%.3g, x2=%.3g, y2=%.3g\n", x1, y1, x2, y2);
		fCanvas = fEcanvas24->GetCanvas();
		fCanvas->cd();
		h2xy->GetXaxis()->SetRangeUser(x1, x2);
		fCanvas->Modified();
		fCanvas->Update();
		fX1->SetNumber(x1);
		fX2->SetNumber(x2);

	}
	else if (event == kButton1Double)
	{
		fCanvas = fEcanvas22->GetCanvas();
		fCanvas->cd();
		hprojx->GetXaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fCanvas = fEcanvas24->GetCanvas();
		fCanvas->cd();
		h2xy->GetXaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fX1->SetNumber(0);
		fX2->SetNumber(256);

	}
}

void uTPCViewer::zoomPads2DY(Int_t event, Int_t px, Int_t py, TObject *)
{
	if (event == kButton1Up)
	{
		TCanvas *c = (TCanvas *) gTQSender;
		TPad *pad = (TPad *) c->GetSelectedPad();
		if (!pad)
			return;
		Double_t x1, x2, y1, y2;
		pad->GetRangeAxis(x1, y1, x2, y2);
		fCanvas = fEcanvas24->GetCanvas();
		fCanvas->cd();
		h2xy->GetYaxis()->SetRangeUser(x1, x2);
		fCanvas->Modified();
		fCanvas->Update();
		fY1->SetNumber(x1);
		fY2->SetNumber(x2);

	}
	else if (event == kButton1Double)
	{
		fCanvas = fEcanvas23->GetCanvas();
		fCanvas->cd();
		hprojy->GetXaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fCanvas = fEcanvas24->GetCanvas();
		fCanvas->cd();
		h2xy->GetYaxis()->SetRangeUser(0, 256);
		fCanvas->Modified();
		fCanvas->Update();
		fY1->SetNumber(0);
		fY2->SetNumber(256);
	}
}

int main(int argc, char **argv)
{

	TApplication theApp("App", &argc, argv);
	show();
	theApp.Run();

	return 0;
}
