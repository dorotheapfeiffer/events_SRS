
#include "DDL632.h"

  ClassImp(DDL632)

//*****************************************************************************
 DDL632::DDL632(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"Output register",(char*)"DL632",mdesc,addr) {
	       std::cout<<"constructing DDL632\n";
}
//-----------------------------------------------------------------------------
 DDL632::~DDL632() {
	 std::cout<<"destroying DDL632\n";
}
//-----------------------------------------------------------------------------
 void DDL632::ReadVME() {
  Int_t i;
  for (i=0; i<4; i++) {
    Access((char*)"r", 8*i+0, 2, &fStatus[i]);
    Access((char*)"r", 8*i+2, 2, &fFix[i]);
    Access((char*)"r", 8*i+4, 2, &fExtin[i]);
    Access((char*)"r", 8*i+6, 2, &fOut[i]);

    fStatus[i] = fStatus[i] & 0x3;
    fFix[i]    = fFix[i]    & 0x1;
    fExtin[i]  = fExtin[i]  & 0x1;
    fOut[i]    = fOut[i]    & 0x1;
  }
}
//-----------------------------------------------------------------------------
 void DDL632::ResetModule() {
  UInt_t addr = 0x80 - (GetAdBase() & 0xff);
  Access((char*)"w", addr, 2, &fDummy);
}
//*****************************************************************************
