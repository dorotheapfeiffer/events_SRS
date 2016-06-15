#ifndef DGDISC_H 
#define DGDISC_H

#include "DGModule.h"
#include "DDisc.h"

//*****************************************************************************
class DGDisc : public DGModule {

 private:
  TGLayoutHints    *fL0;
  TGVerticalFrame  *fVertSlice[4];
  TGTextBuffer     *fTbuf[4];
  TGTextEntry      *fTent[4];
  TGVSlider        *fVsli[4];

public:
  DGDisc(DDisc *module); 
  ~DGDisc();             
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGDisc,0)                             // GUI for DDisc
};
//*****************************************************************************

#endif

