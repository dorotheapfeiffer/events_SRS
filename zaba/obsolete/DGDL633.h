#ifndef DGDL633_H 
#define DGDL633_H

#include "DDL633.h"
#include "DGModule.h"

//*****************************************************************************
class DGDL633 : public DGModule {

private:
  TGCheckButton    *fEnaInp[4];
  TGRadioButton    *fAndInp[4];
  TGRadioButton    *fOrInp[4];

public:
  DGDL633(DModule *module); 
  ~DGDL633();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGDL633,0)                                  // GUI for DDL633
};
//*****************************************************************************

#endif

