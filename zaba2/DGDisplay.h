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

#include "DAcquisition.h"

class DGDisplay {

private:
public:
  DAcquisition	*fMultiGrid;                // pointer to DAcquisition to be monitored
  TCanvas	*fCanvas;
  TObjArray	*fHistogramList;

  Int_t 	m_NrPads; 
  TH1F		**hPh;
  TH1F		**hPosition;
  TH1F		*hTTS;
  TH1F		*hEmpty; // for future use....

public:
  DGDisplay(){}
  DGDisplay(DAcquisition *, TCanvas *canvas);
  virtual ~DGDisplay();
  void Refresh();
  void Reset();

  ClassDef(DGDisplay,0)                  // GUI for Online Display monitoring
};
//*****************************************************************************
#endif
