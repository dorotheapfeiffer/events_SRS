#ifndef DGDL632_H 
#define DGDL632_H

#include "DDL632.h"
#include "DGModule.h"

//*****************************************************************************
class DGDL632 : public DGModule {

private:
  TGHorizontalFrame  *fSlice[4];
  TGCheckButton      *fFix[4];
  TGTextButton       *fPuls[4];
  TGLabel            *fExtin[4];
  TGComboBox         *fSource[4];
  TGLabel            *fOut[4];

public:
  DGDL632(DModule *module); 
  ~DGDL632();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGDL632,0)                                // GUI for DDL632
};
//*****************************************************************************

#endif

