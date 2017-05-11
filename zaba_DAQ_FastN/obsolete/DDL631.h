#ifndef DDL631_H
#define DDL631_H

#include "DModule.h"

//*****************************************************************************
class DDL631 : public DModule {

private:
  UShort_t fIntMask;                // Interrupt mode + interrupt mask
  UShort_t fInter;                  // Interrupt mode + interrupt status
  UShort_t fFlipFlop;               // Interrupt mode + flip-flop status
  UShort_t fInput;                  // Interrupt mode + input status

public:
  DDL631()                    {printf("Default DDL631 constructor called\n");}
  DDL631(Char_t *mdesc, UInt_t addr);
  virtual ~DDL631();

  void EnableInt(Int_t i)           {BitAccess(0x0, i, 1);}
  void DisableInt(Int_t i)          {BitAccess(0x0, i, 0);}
  void ResetFlipFlops()             {fDummy = 0xf; Access((char*)"w", 0x2, 2, &fDummy);}
  void FlipFlopsFree()              {fDummy = 0x0; Access((char*)"w", 0x4, 2, &fDummy);}
  void FlipFlopsMasked()            {fDummy = 0xf; Access((char*)"w", 0x4, 2, &fDummy);}

  void ReadVME();
  void ResetModule();

  Int_t IsIntEnabled(Int_t i)       {return (fIntMask>>i & 1);}
  Int_t IsIntDisabled(Int_t i)      {return ((fIntMask>>i & 1) ^ 1);}
  Int_t IsFlipFlopMasked(Int_t i)   {return ((fIntMask>>(4+i/2)) & 1);}
  Int_t IsFlipFlopFree(Int_t i)     {return ((fIntMask>>(4+i/2) & 1) ^ 1);}
  Int_t GetInterrupt(Int_t i)       {return ((fInter>>i) & 1);}
  Int_t GetFlipFlop(Int_t i)        {return ((fFlipFlop>>i) & 1);}
  Int_t GetInput(Int_t i)           {return ((fInput>>i) & 1);}

  ClassDef(DDL631,1)       // Heidelberg submodule DL631 - Input Register
};
//*****************************************************************************

#endif
