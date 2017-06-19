#ifndef DGSCALER_H 
#define DGSCALER_H

#include "DGModule.h"
#include "DScaler.h"

//*****************************************************************************
class DGScaler : public DGModule {

private:
  TGLayoutHints    *fLay;
  TGCompositeFrame *fCounDat;
  TGTextBuffer     *fTbuf[16];
  TGTextEntry      *fTent[16];
  TGTextButton     *fIncrButton;
  TGTextButton     *fClearButton;

public:
  DGScaler(DScaler *module);
  ~DGScaler();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGScaler,0)                                // GUI for DScaler
};
//*****************************************************************************

#endif

