#ifndef DDL633_H
#define DDL633_H

#include "DModule.h"

//*****************************************************************************
class DDL633 : public DModule {

private:
  UShort_t fLogic;                  // Logic

public:
  DDL633()                    {printf("Default DDL633 constructor called\n");}
  DDL633(Char_t *mdesc, UInt_t addr);
  virtual ~DDL633();

  void Enable(Int_t i)              {BitAccess(0x0, i, 1);}
  void Disable(Int_t i)             {BitAccess(0x0, i, 0);}
  void OrLogic(Int_t i)             {BitAccess(0x0, i+4, 1);}
  void AndLogic(Int_t i)            {BitAccess(0x0, i+4, 0);}

  void ReadVME();
  void ResetModule();

  Int_t IsEnabled(Int_t i)          {return (fLogic>>i & 1);}
  Int_t IsDisabled(Int_t i)         {return ((fLogic>>i & 1) ^ 1);}
  Int_t IsAnd(Int_t i)              {return ((fLogic>>(i+4) & 1) ^ 1);}
  Int_t IsOr(Int_t i)               {return ((fLogic>>(i+4)) & 1);}

  ClassDef(DDL633,1)  // Heidelberg submodule DL633 - NIM AND/OR Logic
};
//*****************************************************************************

#endif
