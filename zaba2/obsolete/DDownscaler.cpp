
#include "DDownscaler.h"

  ClassImp(DDownscaler)

//*****************************************************************************
 DDownscaler::DDownscaler(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"downscaler",(char*)"dl524",mdesc,addr) {
  std::cout<<"constructing DDownscaler\n";
}
//-----------------------------------------------------------------------------
 DDownscaler::~DDownscaler() {
  std::cout<<"destroying DDownscaler\n";
}
//-----------------------------------------------------------------------------
 void DDownscaler::ReadVME() {
  Access((char*)"r", 0x02,  2, &fDownscale[0]);
  Access((char*)"r", 0x06,  2, &fDownscale[1]);
  Access((char*)"r", 0x0a,  2, &fDownscale[2]);
  Access((char*)"r", 0x0e,  2, &fDownscale[3]);
  Access((char*)"r", 0x12,  2, &fDownscale[4]);
  Access((char*)"r", 0x16,  2, &fDownscale[5]);

  Access((char*)"r", 0x22,  2, &fCounter[0]);
  Access((char*)"r", 0x26,  2, &fCounter[1]);
  Access((char*)"r", 0x2a,  2, &fCounter[2]);
  Access((char*)"r", 0x2e,  2, &fCounter[3]);
  Access((char*)"r", 0x32,  2, &fCounter[4]);
  Access((char*)"r", 0x36,  2, &fCounter[5]);

  Access((char*)"r", 0x42,  2, &fOutputPattern);
  Access((char*)"r", 0x62,  2, &fControlRegister);
}
//-----------------------------------------------------------------------------
 void DDownscaler::ResetModule() {
  WriteControlRegister(0xffff);
  ResetDownscalers();
  ClearCounters();
  ResetOutputLevel();
  for (Int_t i=0; i<4; i++) {
    WriteDownscale(i, 0xffff);
  }
}
//*****************************************************************************

