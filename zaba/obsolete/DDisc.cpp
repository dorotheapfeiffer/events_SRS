#include "DDisc.h"

  ClassImp(DDisc)

//*****************************************************************************
 DDisc::DDisc(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"discriminator",(char*)"CES DISC 8151",mdesc,addr) {
  std::cout<<"constructing DDisc\n";
}
//-----------------------------------------------------------------------------
 DDisc::~DDisc() {
  std::cout<<"destroying DDisc\n";
}
//-----------------------------------------------------------------------------
 void DDisc::WriteThreshold(Int_t chan, Int_t thre) {
  UShort_t buf;
  Int_t i;
  Int_t j;

  chan = chan & 0x3;
  if (thre > 0xff) thre = 0xff;

  // All this mess is because two channels are described by one 2-byte word. 
  // Congratulations, CES, for saving 4 bytes this way.

  i = chan > 1;    
  j = chan & 1;

  Access((char*)"r", 0x4 + 2*i, 2, &buf);

  if (j == 0) {
    buf = buf & 0xff;
    buf = buf | (thre << 8);
  } else {
    buf = buf & 0xff00;
    buf = buf | thre;
  }

  Access((char*)"w", 0x4 + 2*i, 2, &buf);
}
//-----------------------------------------------------------------------------
 void DDisc::ReadVME() {
  UShort_t buf[2];
  Access((char*)"r", 0x0, 2, &fSwitch);
  Access((char*)"r", 0x4, 4, buf);
  fSwitch = fSwitch & 0x1;
  swab(buf, fThreshold, 4);
}
//-----------------------------------------------------------------------------
 void DDisc::ResetModule() {
  EnableManSwitch();
  WriteThreshold(0,0);
  WriteThreshold(1,0);
  WriteThreshold(2,0);
  WriteThreshold(3,0);
}
//*****************************************************************************


