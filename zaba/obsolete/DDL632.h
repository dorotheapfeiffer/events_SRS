#ifndef DDL632_H
#define DDL632_H

#include "DModule.h"

//*****************************************************************************
class DDL632 : public DModule {

private:
  UShort_t fStatus[4];               // Mode: taster, fix, puls, or extin
  UShort_t fFix[4];                  // Fix status
  UShort_t fExtin[4];                // Extin status
  UShort_t fOut[4];                  // Output status

public:
  DDL632()                    {printf("Default DDL632 constructor called\n");}
  DDL632(Char_t *mdesc, UInt_t addr);
  virtual ~DDL632();

  void SetMode(Int_t i, Int_t j)    {fDummy = j; Access((char*)"w", 8*i+0, 2, &fDummy);}
  void TasterMode(Int_t i)          {SetMode(i,0);}
  void FixMode(Int_t i)             {SetMode(i,1);}
  void PulsMode(Int_t i)            {SetMode(i,2);}
  void ExtinMode(Int_t i)           {SetMode(i,3);}
  void SetFix(Int_t i)              {fDummy = 1; Access((char*)"w", 8*i+2, 2, &fDummy);}
  void ResetFix(Int_t i)            {fDummy = 0; Access((char*)"w", 8*i+2, 2, &fDummy);}
  void GenPuls(Int_t i)             {fDummy = 1; Access((char*)"w", 8*i+4, 2, &fDummy);}

  void ReadVME();
  void ResetModule();

  Int_t GetMode(Int_t i)            {return fStatus[i];}
  Int_t GetFix(Int_t i)             {return fFix[i];}
  Int_t GetExtin(Int_t i)           {return fExtin[i];}
  Int_t GetOut(Int_t i)             {return fOut[i];}

  ClassDef(DDL632,1)  // Heidelberg submodule DL632 - Output Register
};
//*****************************************************************************

#endif
