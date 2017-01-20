#include <TGFrame.h>
#include <TGTab.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TRootEmbeddedCanvas.h>
#include <TGMenu.h>
#include <TGStatusBar.h>
#include <TGNumberEntry.h>
#include <TTreeViewer.h>
#include <TFile.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TGTextView.h>
#include <TGProgressBar.h>
#include <TGButtonGroup.h>
#include <TGComboBox.h>
#include <TSystem.h>

#include "uTPCAnalysis.h"

class uTPCViewer: public TGMainFrame
{
public:
	uTPCViewer(const TGWindow *p, UInt_t w, UInt_t h);

	void CloseWindow();
	void ShowEvent(int eventNr);
	void ShowHitmap();
	void OpenFile(TString dir, TString fileName, Bool_t analyze);
	void CloseFile();
	void DoNumberEntry(Int_t pos = 0);
	void zoomAll();
	int GetThresholdAmplitude()
	{
		return fThresholdAmplitude->GetNumber();
	}

	int GetThresholdDiscard()
	{
		return fThresholdDiscard->GetNumber();
	}

	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
	void zoomPadsXY(Int_t event, Int_t px, Int_t py, TObject *);
	void zoomPads2DX(Int_t event, Int_t px, Int_t py, TObject *);
	void zoomPads2DY(Int_t event, Int_t px, Int_t py, TObject *);

	uTPCPlotData* plotData;ClassDef(uTPCViewer,0)
private:
	bool fHistosExist;
	bool updateHistos;

	uTPCRawData* rawData;

	TH2F * h2x;
	TH2F * h2y;
	TH1F * hx;
	TH1F * hy;

	TH2F * h2xy;
	TH1F * hprojx;
	TH1F * hprojy;

	TGNumberEntry* fEventNr;
	TGNumberEntry* fThresholdAmplitude;
	TGNumberEntry* fThresholdGamma;
	TGNumberEntry* fThresholdDiscard;

	TGLabel* fLabelThresholdAmplitude;
	TGLabel* fLabelThresholdGamma;
	TGLabel* fLabelThresholdDiscard;

	TGStatusBar* fStatusBar;
	TGMenuBar* fMenuBar;
	TGPopupMenu* fMenuFile;

	TGTab *fTab;
	TGCompositeFrame *fCompositeFrame11;
	TGCompositeFrame *fCompositeFrame21;
	TGCompositeFrame *fCompositeFrame31;
	TGCompositeFrame *fCompositeFrame41;

	TGLayoutHints* fMBItemLayout;
	TGCompositeFrame *vframeTop;
	TGCompositeFrame *vframeTop11;
	TGCompositeFrame *vframeMiddle11;
	TGCompositeFrame *vframeMiddle12;
	TGCompositeFrame *vframeBottom;
	TGCompositeFrame *vframeTop21;
	TGCompositeFrame *vframeMiddle21;
	TGCompositeFrame *vframeMiddle22;

	TGVerticalFrame *vframe11;
	TGVerticalFrame *vframe12;
	TGVerticalFrame *vframe13;
	TGVerticalFrame *vframe14;
	TGVerticalFrame *vframe21;
	TGVerticalFrame *vframe22;
	TGVerticalFrame *vframe23;
	TGVerticalFrame *vframe24;
	TGHorizontalFrame *vframeControls11;

	TGHorizontalFrame *vframeControls21;
	TGHorizontalFrame *vframeControls22;
	TGHorizontalFrame *vframeControls23;
	TGHorizontalFrame *vframeControls24;
	TGHorizontalFrame *vframeControls25;
	TGHorizontalFrame *vframeControls26;

	TCanvas *fCanvas;
	TRootEmbeddedCanvas* fEcanvas11;
	TRootEmbeddedCanvas* fEcanvas12;
	TRootEmbeddedCanvas* fEcanvas13;
	TRootEmbeddedCanvas* fEcanvas14;

	//TRootEmbeddedCanvas* fEcanvas21;
	TRootEmbeddedCanvas* fEcanvas22;
	TRootEmbeddedCanvas* fEcanvas23;
	TRootEmbeddedCanvas* fEcanvas24;

	TRootEmbeddedCanvas* fEcanvas31;
	TRootEmbeddedCanvas* fEcanvas32;
	TRootEmbeddedCanvas* fEcanvas33;
	TRootEmbeddedCanvas* fEcanvas34;

	TGLabel* fLabel11;
	TGLabel* fLabel12;
	TGLabel* fLabel13;
	TGLabel* fLabel14;

	TGLabel* fLabel21;
	TGLabel* fLabel22;
	TGLabel* fLabel23;
	TGLabel* fLabel24;

	TGLabel* fLabelVariableName;
	TGLabel* fLabelFit;
	TGLabel* fLabelSynchronize;
	TGLabel* fLabelX1;
	TGLabel* fLabelX2;
	TGLabel* fLabelY1;
	TGLabel* fLabelY2;

	TGNumberEntry* fX1;
	TGNumberEntry* fX2;
	TGNumberEntry* fY1;
	TGNumberEntry* fY2;

	TGComboBox *fListBoxVariables;
	TGCheckButton *fCheckButtonSynchronize;
	TGComboBox *fComboBoxFit;

	TGLabel* fLabel31;
	TGLabel* fLabel32;
	TGLabel* fLabel33;
	TGLabel* fLabel34;

	TGLabel* fLabelEvent;
	TGLabel* fLabelEventTotal;

};

