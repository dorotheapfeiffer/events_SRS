#ifndef DGLOGIC_H 
#define DGLOGIC_H

#include "DGModule.h"
#include "DLogic.h"
#include "DGPatEditor.h"
#include "DGLogEditor.h"

//*****************************************************************************
class DGLogic : public DGModule {

private:
  TGLayoutHints    *fLHor0,*fLHor5;
  TGLayoutHints    *fLVer0,*fLVer5;
  TGLayoutHints    *fLVerLab;
  TGLayoutHints    *fLVerChe;
  TGLayoutHints    *fLVerEnt;
  TGPopupMenu      *fMenuRAM;
  TGPopupMenu      *fMenuARAM;
  TGPopupMenu      *fMenuBRAM;
  TGCompositeFrame *fSecFrame[2];
  TGCompositeFrame *fNrFrame[2];
  TGCompositeFrame *fInFrame[2];
  TGCompositeFrame *fOutFrame[2];
  TGGroupFrame     *fOutDefFrame[2];
  TGGroupFrame     *fInRegFrame[2];
  TGGroupFrame     *fOutRegFrame[2];
  TGLabel          *fNrLab[2][8];
  TGRadioButton    *fInpExt[2];
  TGRadioButton    *fInpVME[2];
  TGRadioButton    *fInpPat[2];
  TGRadioButton    *fOutRAM[2];
  TGRadioButton    *fOutVME[2];
  TGCheckButton    *fInRegBit[2][8];
  TGCheckButton    *fOutRegBit[2][8];
  TGTextBuffer     *fRAMText[2][256];
  TGTextEntry      *fRAMEntr[2][256];
  TGLabel          *fOutDefLab[2][8];

public:
  DGLogic(DLogic *module);
  ~DGLogic();
  void Refresh();
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGLogic,0)                                // GUI for Dlogic
};
//*****************************************************************************

#endif

