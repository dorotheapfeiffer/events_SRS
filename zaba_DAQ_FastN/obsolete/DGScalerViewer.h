
#ifndef DGSCALERVIEWER_H 
#define DGSCALERVIEWER_H

#include <TSystem.h>
#include <TApplication.h>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TObjArray.h>
#include <TTimer.h>
#include <TStyle.h>
#include <TDatime.h>
#include <TFormula.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>

#define MAXNEVENTS  200000

/****************************************************************************/
class DGScalerViewer : public TGMainFrame {

public:
  TObjArray           *fMemList;         // list containing all members
  TTimer              *fTimer;           // timer for refreshing
  Char_t              *fFilename;        // scaler database filename
  std::fstream             *fFile;            // scaler database file
  Int_t                fEvents;          // current number of events (=bursts)
  UInt_t               fTime[MAXNEVENTS];     // time array
  UInt_t               fScaler[32][MAXNEVENTS]; // scaler data
  Int_t                fGraphEvents;     // number of events to be displayed
  Float_t             *fX;               // abscissa array
  Float_t             *fY[4];            // ordinate arrays
  TH2F                *fHist;            // histogram for axes etc.
  TGraph              *fGraph[4];        // graphs
  Int_t                fGraphOn[4];      // should graphs be displayed or not
  Int_t                fMin;             // display events from
  Int_t                fMax;             // display events to
  Float_t              fZoom;            // zoom
  TGHorizontalFrame   *fLabelFrame;      // frame for fLabel
  TRootEmbeddedCanvas *fCanvasFrame;     // canvas for plotting graphs
  TGHorizontalFrame   *fControlFrame;    // frame for buttons
  TGLabel             *fLabel;           // variable names reminder
  TGGroupFrame        *fSelectionFrame;  // what-to-plot frame
  TGGroupFrame        *fRangeFrame;      // range frame
  TGGroupFrame        *fZoomFrame;       // zoom frame
  TGGroupFrame        *fWindowFrame;     // miscel... how to spell this
  TGTextButton        *fLastHourButton;  
  TGTextButton        *fLastDayButton;
  TGTextButton        *fLastWeekButton;
  TGTextButton        *fLastMonthButton;
  TGTextButton        *fRangeLeftButton;
  TGTextButton        *fRangeRightButton;
  TGTextButton        *fRangeExpandButton;
  TGTextButton        *fRangeShrinkButton;
  TGTextButton        *fWindowRatesButton;
  TGTextButton        *fWindowExitButton;
  TGTextButton        *fZoomVexpandButton;
  TGTextButton        *fZoomVshrinkButton;
  TGTextButton        *fZoomHexpandButton;
  TGTextButton        *fZoomHshrinkButton;
  TGTextBuffer        *fSelectionB[4];
  TGTextEntry         *fSelectionE[4];
  
public:
  DGScalerViewer(Char_t *fina);
  virtual ~DGScalerViewer();
  void ReadAllEvents()                          {ReadData(fFilename, 0);}
  void ReadLastEvent();                      // read only last event
  void ReadData(Char_t *, Int_t);            // read file, fill fTime,fScaler
  void MakeVectors();                        // fTime, fScaler -> fX, fY
  void DrawGraphs();                         // fX, fY -> fGraph
  Int_t SecondsAgo(UInt_t);                  // -> fMin, fMax
  Int_t Chan(const char *);                  // return channel index
  Float_t EvalFormula(const char *, int);    // interpret formula
  Int_t FileChanged();                       // file changed since last time?
  void Rates(Int_t, UInt_t);                 // history
  Bool_t HandleTimer(TTimer *tim);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGScalerViewer,0)                  // CERES scaler history viewer
};
/*****************************************************************************/

#endif
