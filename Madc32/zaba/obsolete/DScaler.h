#ifndef DSCALER_H 
#define DSCALER_H

#include "DModule.h"

//*****************************************************************************
class DScaler : public DModule {

private:
  UShort_t fIntVector;                       // interrupt vector 
  UShort_t fIntLevel;                        // interrupt level
  UShort_t fVetoStat;                        // veto state last read (1)
  UShort_t fRequest;                         // VME interrupt enabled
  ULong_t  fCounter[16];                     // counters
  UShort_t fScalStat;                        // 0 for 2*32, 1 for 64
  UShort_t fFixCodeFA;                       // fixed code   (0xfa = 250)
  UShort_t fFixCodeF5;                       // fixed code   (0xf5 = 245)
  UShort_t fManufacturer;                    // manufacturer (0x02 = 2)
  UShort_t fModType;                         // type         (0x1f = 24)
  UShort_t fVersion;                         // version
  UShort_t fSerial;                          // serial number

public:
  DScaler()                 {printf("Default DScaler constructor called\n");}
  DScaler(Char_t *mdesc, UInt_t addr);
  virtual ~DScaler();
  Long_t   GetCount(Int_t i)                 {return fCounter[i];}
  ULong_t *Data()                            {return fCounter;}
  void     IncrementScale()                  {Access((char*)"r", 0x56, 2, &fDummy);}
  void     ClearScale()                      {Access((char*)"r", 0x50, 2, &fDummy);}
  void     SetVeto()                         {Access((char*)"r", 0x52, 2, &fDummy);}
  void     RemoveVeto()                      {Access((char*)"r", 0x54, 2, &fDummy);}
  void     EnableInterrupt()                 {Access((char*)"r", 0x08, 2, &fDummy);}
  void     DisableInterrupt()                {Access((char*)"r", 0x0a, 2, &fDummy);}
  void     ClearInterrupt()                  {Access((char*)"r", 0x0c, 2, &fDummy);}
  void     ReadVME();
  void     ResetModule();

  ClassDef(DScaler,1) // CAEN VME scaler V560
};
//*****************************************************************************

#endif
