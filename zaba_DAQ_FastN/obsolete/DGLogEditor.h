#ifndef DGLOGEDITOR_H 
#define DGLOGEDITOR_H

#include <iostream>

#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGCanvas.h>
#include <TObjArray.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TFormula.h>

#include "DLogic.h"

//*****************************************************************************
class DGLogEditor : public TGTransientFrame {

private:
  DLogic               *fLogic;                // pointer to DLogic
  Int_t                 fSec;                  // which section (0 or 1)
  TObjArray            *fMemList;              // list containing all members
  TGLayoutHints        *fLHLeft;
  TGLayoutHints        *fLHTop;
  TGLayoutHints        *fLHBottom;
  TGVerticalFrame      *fLabFrame;
  TGVerticalFrame      *fOutDefFrame;
  TGVerticalFrame      *fButtonFrame;
  TGLabel              *fLab[8];
  TGTextBuffer         *fTbuf[8];
  TGTextEntry          *fTent[8];
  TGTextButton         *fClearButton;
  TGTextButton         *fRefreshButton;
  TGTextButton         *fDoneButton;

public:
  DGLogEditor(const TGWindow *, DLogic *, Int_t);
  virtual ~DGLogEditor();
  void Refresh();
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGLogEditor,0)           // Graphical logic editor for DLogic
};
//*****************************************************************************
#endif
