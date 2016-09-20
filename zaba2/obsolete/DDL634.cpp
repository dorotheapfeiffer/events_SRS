#include "DDL634.h"

  ClassImp(DDL634)

//*****************************************************************************
 DDL634::DDL634(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"Fan Out",(char*)"DL634",mdesc,addr) {
	       std::cout<<"constructing DDL634\n";
}
//-----------------------------------------------------------------------------
 DDL634::~DDL634() {
	 std::cout<<"destroying DDL634\n";
}
//-----------------------------------------------------------------------------
 void DDL634::ReadVME() {
  Access((char*)"r", 0, 2, &fFanout);
  fFanout = fFanout & 0xf;
}
//-----------------------------------------------------------------------------
 void DDL634::ResetModule() {
  UInt_t addr = 0x80 - (GetAdBase() & 0xff);
  Access((char*)"w", addr, 2, &fDummy);
}
//*****************************************************************************
