#ifndef DGDL634_H 
#define DGDL634_H

#include "DDL634.h"
#include "DGModule.h"

//*****************************************************************************
class DGDL634 : public DGModule {

private:
  TGRadioButton    *fNormOut[4];
  TGRadioButton    *fInvOut[4];

public:
  DGDL634(DModule *module); 
  ~DGDL634();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGDL634,0)                                  // GUI for DDL634
};
//*****************************************************************************

#endif

