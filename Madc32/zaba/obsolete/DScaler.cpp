
#include "DScaler.h"

  ClassImp(DScaler)

//*****************************************************************************
 DScaler::DScaler(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"scaler",(char*)"CAEN V560",mdesc,addr) {
  std::cout<<"constructing DScaler\n";
}
//-----------------------------------------------------------------------------
 DScaler::~DScaler() {
  std::cout<<"destroying DScaler\n";
}
//-----------------------------------------------------------------------------
 void DScaler::ReadVME() {
  UChar_t buf[256];

  Access((char*)"r", 0x04,  4, &buf[0x04]);
  Access((char*)"r", 0x0e, 66, &buf[0x0e]);
  Access((char*)"r", 0x58,  2, &buf[0x58]);
  Access((char*)"r", 0xfa,  6, &buf[0xfa]);

  memcpy(&fIntVector, &buf[0x04],1);
  fIntLevel = buf[0x06] & 0x07;
  fVetoStat = (buf[0x06]>>7) & 0x01;
  memcpy(&fRequest, &buf[0x0e], 1);

  // Here I swap adjacent 2-byte words.
  // I think I should redo it in some easier to understand way. 
  for (int i=0; i<16; i++) {
    fCounter[i] = *((UShort_t *) &buf[0x10+i*4] ) * 0xFFFF +
                  *((UShort_t *) &buf[0x10+i*4+2] );
  }

  memcpy(&fScalStat,  &buf[0x58],1);
  memcpy(&fFixCodeF5, &buf[0xfa],1);
  memcpy(&fFixCodeFA, &buf[0xfb],1);

  fManufacturer = buf[0xfd]>>2;
  fModType = (buf[0xfd] & 0x3) * 0x100 + buf[0xfc];
  fVersion      = buf[0xff]>>4;
  fSerial       = (buf[0xff] & 0xf) * 0x100 + buf[0xfe];

}
//-----------------------------------------------------------------------------
 void DScaler::ResetModule() {
  ClearInterrupt();
  DisableInterrupt();
  RemoveVeto();
  ClearScale();
}
//*****************************************************************************

