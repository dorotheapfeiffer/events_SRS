#include "DDL631.h"

  ClassImp(DDL631)

//*****************************************************************************
 DDL631::DDL631(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"Input register",(char*)"DL631",mdesc,addr) {
	       std::cout<<"constructing DDL631\n";
}
//-----------------------------------------------------------------------------
 DDL631::~DDL631() {
	 std::cout<<"destroying DDL631\n";
}
//-----------------------------------------------------------------------------
 void DDL631::ReadVME() {
  Access((char*)"r", 0, 8, &fIntMask);
  fIntMask  = fIntMask  & 0x3f;
  fInter    = fInter    & 0x3f;
  fFlipFlop = fFlipFlop & 0x3f;
  fInput    = fInput    & 0x3f;
}
//-----------------------------------------------------------------------------
 void DDL631::ResetModule() {
  UInt_t addr = 0x80 - (GetAdBase() & 0xff);
  Access((char*)"w", addr, 2, &fDummy);
}
//*****************************************************************************
