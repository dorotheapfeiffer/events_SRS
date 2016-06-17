#ifndef DGDOWNSCALER_H 
#define DGDOWNSCALER_H

#include "DGModule.h"
#include "DDownscaler.h"

//*****************************************************************************
class DGDownscaler : public DGModule {

private:
  TGLayoutHints    *fLay;
  TGCompositeFrame *fCounDat;
  TGCheckButton    *fEnable[6];
  TGCheckButton    *fEnableInOr[6];
  TGTextBuffer     *fDownT[6];
  TGTextEntry      *fDownE[6];
  TGLabel          *fOutpat[6];
  TGLabel          *fCount[6];
  TGTextButton     *fResetDownscalersButton;
  TGTextButton     *fResetOutputButton;
  TGTextButton     *fClearCountersButton;

public:
  DGDownscaler(DDownscaler *module);
  ~DGDownscaler();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGDownscaler,0)                          // GUI for DDownscaler
};
//*****************************************************************************

#endif

