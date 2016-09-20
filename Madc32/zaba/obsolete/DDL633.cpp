
#include "DDL633.h"

  ClassImp(DDL633)

//*****************************************************************************
 DDL633::DDL633(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"AND/OR Logic",(char*)"DL633",mdesc,addr) {
	       std::cout<<"constructing DDL633\n";
}
//-----------------------------------------------------------------------------
 DDL633::~DDL633() {
	 std::cout<<"destroying DDL633\n";
}
//-----------------------------------------------------------------------------
 void DDL633::ReadVME() {
  Access((char*)"r", 0, 2, &fLogic);
  fLogic = fLogic & 0xff;
}
//-----------------------------------------------------------------------------
 void DDL633::ResetModule() {
  UInt_t addr = 0x80 - (GetAdBase() & 0xff);
  Access((char*)"w", addr, 2, &fDummy);
}
//*****************************************************************************
