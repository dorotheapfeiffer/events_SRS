#ifndef DGPATEDITOR_H 
#define DGPATEDITOR_H

#include <iostream>

#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGCanvas.h>
#include <TObjArray.h>

#include "DLogic.h"

#define kBlock   0x1
#define kRestore 0x2

//*****************************************************************************
class DGPatEditor : public TGTransientFrame {

private:
  DLogic               *fLogic;                // pointer to DLogic
  Int_t                 fSec;                  // which section (0 or 1)
  Int_t                 fOptions;              // options, see description
  UShort_t              fConfreg;              // buffer for DLogic conf reg
  UShort_t              fRam[256];             // buffer for DLogic RAM
  TObjArray            *fMemList;              // list containing all members
  TGLayoutHints        *fLHLeft;
  TGLayoutHints        *fLHRight;
  TGLayoutHints        *fLHTop;
  TGLayoutHints        *fLHBottom;
  TGLayoutHints        *fLHCanvas;
  TGVerticalFrame      *fLeftLabFrame;         // left-hand side labels 
  TGVerticalFrame      *fRightLabFrame;        // right-hand side labels
  TGCanvas             *fDataCanvas;
  TGCompositeFrame     *fDataFrame;
  TGVerticalFrame      *fButtonFrame;
  TGLabel              *fLeftLab[8];
  TGLabel              *fRightLab[8];
  TGButton             *fDataBit[256][8];
  TGLabel              *fBottomLab[256][3];
  TGTextButton         *fClearButton;
  TGTextButton         *fRefreshButton;
  TGTextButton         *fDoneButton;

public:
  DGPatEditor(const TGWindow *main, DLogic *, Int_t, Int_t opt = 0);
  virtual ~DGPatEditor();
  void Refresh();
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGPatEditor,0)       // Graphical pattern editor for DLogic
};
//*****************************************************************************

#endif

