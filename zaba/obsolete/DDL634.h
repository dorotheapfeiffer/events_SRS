#ifndef DDL634_H
#define DDL634_H

#include "DModule.h"

//*****************************************************************************
class DDL634 : public DModule {

private:
  UShort_t fFanout;                 // Fan-out

public:
  DDL634()                    {printf("Default DDL634 constructor called\n");}
  DDL634(Char_t *mdesc, UInt_t addr);
  virtual ~DDL634();

  void Norm(Int_t i)             {BitAccess(0x0, i, 0);}
  void Inv(Int_t i)              {BitAccess(0x0, i, 1);}

  void ReadVME();
  void ResetModule();

  Int_t IsNorm(Int_t i)           {return (((fFanout >> i) & 1) ^ 1);}
  Int_t IsInv(Int_t i)            {return ((fFanout >> i) & 1);}

  ClassDef(DDL634,1)   // Heidelberg submodule DL634 - Fan Out
};
//*****************************************************************************

#endif
