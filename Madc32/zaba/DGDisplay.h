#ifndef DGDISPLAY_H 
#define DGDISPLAY_H

#include "TH1F.h"
#include "TH2F.h"
//#include <TGraph.h>
//#include <TText.h>
//#include <TPaveText.h>
#include <TCanvas.h>
//#include <TArrow.h>
//#include <TEllipse.h>

#include "DMultiGrid.h"

class DGDisplay {

private:
public:
  DMultiGrid	*fMultiGrid;                // pointer to DMultiGrid to be monitored
  TCanvas	*fCanvas;
  TObjArray	*fHistogramList;
 
  TPad		*fPadFrontImage;
  TPad		*fPadFullImage;
  TPad		*fPadPh;
  TPad		*fPadPosition;

  TH2F		*hFrontImage;
  TH2F		*hFullImage;
  TH1F		*hPh;
  TH1F		*hPosition;
  

public:
  DGDisplay(){}
  DGDisplay(DMultiGrid *, TCanvas *canvas);
  virtual ~DGDisplay();
  void Refresh();
  void Reset();

  ClassDef(DGDisplay,0)                  // GUI for Online Display monitoring
};
//*****************************************************************************
#endif
