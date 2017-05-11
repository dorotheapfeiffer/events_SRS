#ifndef DGDL631_H 
#define DGDL631_H

#include "DDL631.h"
#include "DGModule.h"

//*****************************************************************************
class DGDL631 : public DGModule {

private:
  TGLayoutHints     *fL0;
  TGCompositeFrame  *fMatFrame;
  TGTextButton      *fResetFF;
  TGLabel           *fInp[4];
  TGCheckButton     *fMask[4];
  TGCheckButton     *fMaskedFF[4];
  TGLabel           *fFlipFlop[4];
  TGLabel           *fInterrupt[4];

public:
  DGDL631(DModule *module); 
  ~DGDL631();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGDL631,0)                                  // GUI for DDL631
};
//*****************************************************************************

#endif

