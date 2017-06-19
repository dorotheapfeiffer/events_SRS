#ifndef DTRIGGER_H 
#define DTRIGGER_H

#include <TObject.h>
#include <TSocket.h>
#include <TObjArray.h>

#include "DLogic.h"
#include "DDownscaler.h"
#include "DDisc.h"
#include "DDL631.h"
#include "DDL632.h"
#include "DDL633.h"
#include "DDL634.h"
#include "DScaler.h"

// define downscaler input channels (FLT), Abort, and Busy sources

#define NFLT    10
#define NABORT   3
#define NBUSY    7

// FLT channels                 beam,minb,cent, SOB, EOB,  MT, UV1, UV2,SDDi,LASE
static const int FLT_DS[NFLT] ={   0,   0,   0,   1,   1,   1,   2,   2,   2,   2};
static const int FLT_Ch[NFLT] ={   0,   1,   2,   0,   1,   2,   0,   1,   2,   3};

//*****************************************************************************
class DTrigger : public TObject {

private:
  TObjArray   *fModuleList;              // list of pointers to VME modules

public:
  DLogic      *fLogic0;                  // logic unit (patgen) 
  DLogic      *fLogic1;                  // logic unit (logic) 
  DDownscaler *fDown[3];                 // FLT downscalers (physics, MT, CAL)
  DDisc       *fDisc;                    // discriminator (SLT SDD)
  DDownscaler *fDown2;                   // downscaler (SLT SDD)
  DDL633      *fFltinp;                  // AND/OR (FLT)
  DDL634      *fFltout;                  // Fan-out (FLT)
  DDL633      *fAboinp;                  // AND/OR (Abort)
  DDL634      *fAboout;                  // Fan-out (Abort)
  DDL633      *fBusy[2];                 // AND/OR (Busy OR, two sections a 4 ch)
  DDL633      *fBusyor;                  // AND/OR (Busy 1 OR 2)
  DDL634      *fBusyout;                 // Fan-out (Busy)
  DDL631      *fBusypat[2];              // Input register (Busy, two sections a 4 ch)
  DScaler     *fScaler0;                 // Scaler (inp + out logic1,1/2)
  DScaler     *fScaler1;                 // Scaler (trigger counts)

public:
  DTrigger();
  virtual ~DTrigger();
  void ResetModules();                   // Call ResetModule() for each VME module
  void Initialize();                     // Load the fixed part of the settings
  void Load(char *filename);             // Load the variable part of settings from file
  void Save(char *filename);             // Save the variable part of settings on file
  void EnableBBP();                      // Enable beam before-protection
  void DisableBBP();                     // Disable beam before-protection
  void EnableIBP();                      // Enable interaction before-protection
  void DisableIBP();                     // Disable interaction before-protection
  void SwitchMTCAL(Int_t, Int_t, Int_t); // Enable/disable channel in MT/CAL logic
  void WriteFLTDownscal(Int_t, Int_t);   // Set downscaling factor
  void EnableFLT(Int_t tri);             // Enable FLT tri
  void DisableFLT(Int_t tri);            // Disable FLT tri
  void ManualFLTReset();                 // Send reset pulse to FLT DS's
  void EnableAbort(Int_t abo)               {fAboinp->Enable(abo);}
  void DisableAbort(Int_t abo)              {fAboinp->Disable(abo);}
  void ManualAbort();                    // Send Abort pulse to detectors
  void EnableSDDMult(Int_t);             // Enable SDD Multiplicity trigger 
  void DisableSDDMult(Int_t);            // Disable SDD Multiplicity trigger 
  void WriteSDDMultThr(Int_t, Int_t);    // Set SDD multiplicity threshold
  void WriteSDDMultDownscal(Int_t, Int_t); // Set SDD multiplicity downscaling factor
  void EnableBusy(Int_t);                // Enable Busy bus
  void DisableBusy(Int_t);               // Disable Busy bus

  void ReadVME();                        // Copy VME data to module objects
  Int_t GetFLTDownscal(Int_t);           // Return downscaling factor
  Int_t GetFLTCount(Int_t);              // Return downscaler count
  Int_t IsEnabledBBP();                  // Return status of beam before-protection
  Int_t IsEnabledIBP();                  // Return status of interaction before-protection
  Int_t IsOnMTCAL(Int_t, Int_t);         // Return status of channel of MT/CAL
  Int_t IsEnabledFLT(Int_t);             // Return status of given FLT 
  Int_t IsEnabledAbort(Int_t);           // Return status of given Abort
  Int_t IsEnabledSDDMult(Int_t);         // Return status of given SDD Multiplicity trigger 
  Int_t GetSDDMultThr(Int_t);            // Return SDD multiplicity threshold
  Int_t GetSDDMultDownscal(Int_t);       // Return SDD multiplicity downscal
  Int_t IsEnabledBusy(Int_t);            // Return status of given Busy
  Int_t IsBusy(Int_t);                   // Return Busy state
  

  ClassDef(DTrigger,1)  // CERES trigger 
};
//*****************************************************************************

#endif

