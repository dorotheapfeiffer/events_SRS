#ifndef DGMONITOR_H 
#define DGMONITOR_H

#include <TGraph.h>
#include <TText.h>
#include <TPaveText.h>
#include <TCanvas.h>
#include <TArrow.h>
#include <TEllipse.h>

#include "DTrigger.h"

#define TEXT_SIZE 0.04
#define THICK     3
#define RED       2
#define BOXCOLOR  93

//*****************************************************************************
class DGCable {

private:
  TGraph *fLine;          // line
  TText  *fLabel;         // label
  TText  *fCount;         // count
  Int_t   fLine_x0;       // x-start
  Int_t   fLine_x1;       // x-end
  Int_t   fLine_y;        // y (line is horizontal)
  Int_t   fLabel_x;       // x-position of label
  Int_t   fLabel_y;       // y-position of label
  Int_t   fCount_x;       // x-position of count
  Int_t   fCount_y;       // y-position of count

public:
  DGCable(Int_t x0, Int_t x1, Int_t y, Char_t *label);
  virtual ~DGCable();
  void SetLabel(const char *);
  void SetCount(Int_t n);
  void Excite();                     // make it thick and red
  void Relax();                      // make it thin and black
};
//*****************************************************************************
class DGSwitch {

private:
  TGraph *fLine;                      // line
  Int_t   fX0;                        // first point x
  Int_t   fY0;                        // first point y
  Int_t   fX1;                        // second point x when on
  Int_t   fY1;                        // second point y when on
  Int_t   fX2;                        // second point x when off
  Int_t   fY2;                        // second point y when off

public:
  DGSwitch(Int_t x, Int_t y, Int_t opt);
  virtual ~DGSwitch();
  void SetState(Int_t i);              // turn on or off
};
//*****************************************************************************
class DGBox {

private:
  TPaveText *fBox;
  
public:
  DGBox(Int_t x0, Int_t x1, Int_t y, Int_t height, char * label, Int_t color);
  virtual ~DGBox();
};
//*****************************************************************************
class DGMonitor {

private:
  DTrigger *fTrigger;                // pointer to DTrigger to be monitored
  TCanvas  *fCanvas;
  DGCable  *fPhysLogInpCable[8];
  DGSwitch *fPhysLogInpSwitch[8];
  DGCable  *fPhysLogOutCable[8];
  DGCable  *fPhysFLTOutCable[8];
  DGSwitch *fPhysFLTInpSwitch[8];
  DGCable  *fMTCALLogInpCable[8];
  DGCable  *fMTCALLogOutCable[8];
  DGCable  *fMTCALFLTOutCable[8];
  DGSwitch *fMTCALFLTInpSwitch[8];
  DGCable  *fFLTOutCable;
  DGCable  *fSLTCable;
  DGCable  *fAbortInpCable[4];
  DGSwitch *fAbortInpSwitch[4];
  DGCable  *fAbortOutCable;
  TArrow   *fAbortArrow;
  DGCable  *fBusyInpCable[8];
  DGSwitch *fBusyInpSwitch[8];
  TEllipse *fBusyInvert;
  DGCable  *fBusyOutCable;
  DGCable  *fResetCable;

public:
  DGMonitor(DTrigger *trigger, TCanvas *canvas);
  virtual ~DGMonitor();
  void Refresh();

  ClassDef(DGMonitor,0)                  // GUI for DTrigger monitoring
};
//*****************************************************************************
#endif
