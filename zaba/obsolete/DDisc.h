#ifndef DDISC_H 
#define DDISC_H


#include "DModule.h"

//*****************************************************************************
class DDisc : public DModule {

private:
  UInt_t   fSwitch;                              // manual switch enabled
  UChar_t  fThreshold[4];                        // discrimination thresholds

public:
  DDisc()                  {printf("Default DDisc constructor called\n");}
  DDisc(Char_t *mdesc, UInt_t addr);
  virtual ~DDisc();
  void EnableManSwitch()           {fDummy = 1; Access((char*)"w", 0, 2, &fDummy);}
  void DisableManSwitch()          {fDummy = 0; Access((char*)"w", 0, 2, &fDummy);}
  void WriteThreshold(Int_t, Int_t);

  void ReadVME();
  void ResetModule();

  Int_t IsManSwitchEnabled()       {return fSwitch;}
  Int_t GetThreshold(UInt_t chan)  {return (Int_t) fThreshold[chan];}

  ClassDef(DDisc,1)  // CES VME discriminator DISC 8151 
};
//*****************************************************************************

#endif
