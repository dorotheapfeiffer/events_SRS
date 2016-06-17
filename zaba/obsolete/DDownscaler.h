#ifndef DDOWNSCALER_H 
#define DDOWNSCALER_H

#include "DModule.h"

//*****************************************************************************
class DDownscaler : public DModule {

private:
  UShort_t fDownscale[6];                    // downscale factors
  UShort_t fCounter[6];                      // counters 
  UShort_t fOutputPattern;                   // output pattern
  UShort_t fControlRegister;                 // control register

public:
  DDownscaler() {printf("Default DDownscaler constructor called\n");}
  DDownscaler(Char_t *mdesc, UInt_t addr);
  virtual ~DDownscaler();
  void     WriteDownscale(Int_t i,Int_t val) {fDummy = (UShort_t) val; 
                                              Access((char*)"w", 4*i+2, 2, &fDummy);}
  void     WriteControlRegister(UShort_t i)  {Access((char*)"w", 0x62, 2, &i);}
  void     ResetDownscalers()                {Access((char*)"r", 0x1e, 2, &fDummy);}
  void     ClearCounters()                   {Access((char*)"r", 0x3e, 2, &fDummy);}
  void     ResetOutputLevel()                {Access((char*)"r", 0x7e, 2, &fDummy);}
  void     Enable(Int_t i)                   {BitAccess(0x62, i, 1);}
  void     Disable(Int_t i)                  {BitAccess(0x62, i, 0);}
  void     EnableInOr(Int_t i)               {BitAccess(0x62, i+8, 1);}
  void     DisableInOr(Int_t i)              {BitAccess(0x62, i+8, 0);}

  void     ReadVME();
  void     ResetModule();

  Int_t    GetDownscale(Int_t i)             {return fDownscale[i];}
  Int_t    GetCounter(Int_t i)               {return fCounter[i];}
  Int_t    GetOutputPattern()                {return fOutputPattern;}
  Int_t    GetControlRegister()              {return fControlRegister;}
  Int_t    GetOutputBit(Int_t i)             {return fOutputPattern>>i & 1;}
  Int_t    GetEnableBit(Int_t i)             {return fControlRegister>>i & 1;}
  Int_t    GetEnableInOrBit(Int_t i)         {return (fControlRegister>>i)+(8&1);} // ???????????????????

  ClassDef(DDownscaler,1) // Heidelberg downscaler
};
//*****************************************************************************

#endif
