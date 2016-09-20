#ifndef DLOGIC_H 
#define DLOGIC_H

#include "DModule.h"

//*****************************************************************************
class DLogic : public DModule {

private:
  UShort_t fARAM[256];                       // Section A RAM
  UShort_t fBRAM[256];                       // Section B RAM
  UShort_t fOutputReg;                       // Output register
  UShort_t fInputReg;                        // Input register
  UShort_t fConfReg;                         // Configuration register
  UShort_t fFixCodeFA;                       // fixed code   (0xfa = 250)
  UShort_t fFixCodeF5;                       // fixed code   (0xf5 = 245)
  UShort_t fManufacturer;                    // manufacturer (0x02 = 2)
  UShort_t fModType;                         // type         (0x19 = 25)
  UShort_t fVersion;                         // version
  UShort_t fSerial;                          // serial number

public:
  DLogic()                 {printf("Default DLogic constructor called\n");}
  DLogic(Char_t *mdesc, UInt_t addr);
  virtual ~DLogic();
  void WriteAddrReg(Int_t, Int_t);           // Write address register
  void WriteRAM(Int_t, UShort_t*);           // Fill RAM - basic method
  void WriteRAM(Int_t, UShort_t);            // Fill RAM with one number
  void WriteRAM(Int_t, Char_t* );            // Fill RAM with special sequence

  void GenStrobe(Int_t sec)   {Access((char*)"w", 0x00c - (sec&1)*0x002, 2, &fDummy);}
  void WriteOutputReg(Int_t sec, UShort_t dat) {
    Access((char*)"w", 0x006 - (sec&1)*0x002, 2, &dat);
  }
  void WriteConfReg(UShort_t dat)  {Access((char*)"w", 0x002, 2, &dat);}
  void ReadVME();
  void ReadRAM();
  void ResetModule();

  void SelectVMEInput(Int_t sec)   {BitAccess(0x002,     (sec&1), 1);}
  void SelectExtInput(Int_t sec)   {BitAccess(0x002,     (sec&1), 0);}
  void SelectRAMOutput(Int_t sec)  {BitAccess(0x002, 2 + (sec&1), 1);} 
  void SelectVMEOutput(Int_t sec)  {BitAccess(0x002, 2 + (sec&1), 0);} 
  void EnableStop(Int_t sec)       {BitAccess(0x002, 5 - (sec&1), 1);}
  void DisableStop(Int_t sec)      {BitAccess(0x002, 5 - (sec&1), 0);}
  void StartPatGen(Int_t sec)      {BitAccess(0x002, 6 + (sec&1), 1);}
  void StopPatGen(Int_t sec)       {BitAccess(0x002, 6 + (sec&1), 0);}
  void DisableStrobe(Int_t sec)    {BitAccess(0x002, 8 + (sec&1), 1);}
  void EnableStrobe(Int_t sec)     {BitAccess(0x002, 8 + (sec&1), 0);}
  void ExtClock()                  {BitAccess(0x002, 10, 1);}
  void IntClock()                  {BitAccess(0x002, 10, 0);}

  void LogicMode(Int_t sec);
  void PatternMode(Int_t sec);
  void IORegisterMode(Int_t sec);
  void EditMode(Int_t sec);

  UShort_t GetConfReg()            {return fConfReg;}
  UShort_t GetInputReg()           {return fInputReg;}
  UShort_t GetOutputReg()          {return fOutputReg;}
  UShort_t GetInputReg(Int_t sec)  {return (fInputReg >> ((sec&1)*8)) & 0xff;}
  UShort_t GetOutputReg(Int_t sec) {return (fOutputReg >> ((sec&1)*8)) & 0xff;}
  UShort_t* GetRAM(Int_t sec)       {if (sec==0) return fARAM; else return fBRAM;}
  UShort_t GetRAM(Int_t sec, Int_t i) {if (sec==0) return fARAM[i]; else return fBRAM[i];}
  void     DumpConfReg();
  void     DumpARAM();
  void     DumpBRAM();

  Int_t    IsInputVME(Int_t sec)       {return ( (GetConfReg() >> 0) + (sec&1) ) & 1;}
  Int_t    IsInputExt(Int_t sec)       {return ( ( (GetConfReg() >> 0) + ((sec&1) & 1))) ^ 1;}
  Int_t    IsOutputRAM(Int_t sec)      {return ( (GetConfReg() >> 2) + (sec&1)) & 1;}
  Int_t    IsOutputVME(Int_t sec)      {return ( ( (GetConfReg() >> 2) + ((sec&1) & 1))) ^ 1;}
  Int_t    IsPatGenStarted(Int_t sec)  {return ( (GetConfReg() >> 6) + (sec&1)) & 1;}
  Int_t    IsInLogicMode(Int_t sec)    {return IsInputExt(sec) & IsOutputRAM(sec) & 
					  !IsPatGenStarted(sec);}
  Int_t    IsForcedNormal(Int_t sec)   {return (GetConfReg() >> 8) + ((sec&1) & 1);}

  void     ExtractLogic(Int_t, Int_t, TString *);
  void     DefineLogic(Int_t, Int_t, char *);
  void     AddToLogic(Int_t, Int_t, char *);
  Int_t    LogicContains(Int_t, Int_t, char *);
  void     RemoveFromLogic(Int_t, Int_t, char *);

  ClassDef(DLogic,1)   // CAEN VME dual logic unit V495
};
//*****************************************************************************

#endif
