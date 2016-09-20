///////////////////////////////////////////////////////////////////////////////
// DLogic handles the VME dual logic unit V495 (CAEN). This unit can work as
// a logic unit, input/output register, or pattern generator. 
///////////////////////////////////////////////////////////////////////////////

#include "DLogic.h"

  ClassImp(DLogic)

//*****************************************************************************
 DLogic::DLogic(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"logic unit",(char*)"CAEN V495",mdesc,addr) {
  std::cout<<"constructing DLogic\n";
}
//-----------------------------------------------------------------------------
 DLogic::~DLogic() {
  std::cout<<"destroying DLogic\n";
}
//-----------------------------------------------------------------------------
 void DLogic::WriteAddrReg(Int_t sec, Int_t addr) {
  Access((char*)"w", 0x600 + (sec&1)*0x200 + (UChar_t) addr*2, 2, &fDummy);
}
//-----------------------------------------------------------------------------
 void DLogic::WriteRAM(Int_t sec, UShort_t *buf) { 
  // basic RAM-writing method

  UShort_t regist;
  ReadVME();
  regist = GetConfReg();
  EditMode(sec);
  Access((char*)"w", 0x200 + (sec&1)*0x200, 512, buf);
  ReadRAM();
  WriteConfReg(regist);
}
//-----------------------------------------------------------------------------
 void DLogic::WriteRAM(Int_t sec, UShort_t dat) { 
  // write same number dat into all RAM locations
  UShort_t buf[256];
  int i;
  for (i=0; i<256; i++) {
    buf[i] = dat;
  }
  WriteRAM(sec, buf);
}
//-----------------------------------------------------------------------------
 void DLogic::WriteRAM(Int_t sec, Char_t* flag) { 
  // Write a predefined sequence into RAM. 

  UShort_t buf[256];
  Int_t i;

  // diagonal matrix

  if (strcmp(flag,"i")==0) {
    for (i=0; i<256; i++) {
      buf[i] = (UShort_t) i;
    }
  WriteRAM(sec, buf);
  }
}
//-----------------------------------------------------------------------------
 void DLogic::ReadVME() {

  UChar_t buf[256];

  Access((char*)"r", 0x004,   2, &fOutputReg);
  Access((char*)"r", 0x006,   2, &fInputReg);
  Access((char*)"r", 0x002,   2, &fConfReg);
  Access((char*)"r", 0x0fa,   6, &buf[0xfa]);
  memcpy(&fFixCodeF5, &buf[0xfa],1);
  memcpy(&fFixCodeFA, &buf[0xfb],1);

  fManufacturer = buf[0xfd]>>2;
  fModType = (buf[0xfd] & 0x3) * 0x100 + buf[0xfc];
  fVersion      = buf[0xff]>>4;
  fSerial       = (buf[0xff] & 0xf) * 0x100 + buf[0xfe];
}
//-----------------------------------------------------------------------------
 void DLogic::ReadRAM() {
  // Basic RAM-reading method. It is separated from ReadVME because it
  // affects the operation of the unit and should not be done unless
  // really needed. 

  UShort_t regist;
  UShort_t tempbuf[256];

  ReadVME();
  regist = GetConfReg();
  EditMode(0);
  EditMode(1);
  Access((char*)"r", 0x200, 512, fARAM);
  Access((char*)"r", 0x400, 512, tempbuf);

  // I do not understand why but I need to swap bytes for section B

  swab(tempbuf,fBRAM,512);
  WriteConfReg(regist);
}
//-----------------------------------------------------------------------------
 void DLogic::ResetModule() {
  WriteConfReg(0x0);
  WriteRAM(0, (UShort_t) 3); 
  WriteRAM(0, (UShort_t) 7); 
}
//-----------------------------------------------------------------------------
 void DLogic::LogicMode(Int_t sec) { 
  DisableStrobe(sec);
  StopPatGen(sec);
  SelectExtInput(sec);
  SelectRAMOutput(sec);
}
//-----------------------------------------------------------------------------
 void DLogic::PatternMode(Int_t sec) {
  DisableStrobe(sec);
  SelectVMEInput(sec);
  SelectRAMOutput(sec);
  StartPatGen(0);
}
//-----------------------------------------------------------------------------
 void DLogic::IORegisterMode(Int_t sec) {
  DisableStrobe(sec);
  StopPatGen(sec);
  SelectExtInput(sec);
  SelectVMEOutput(sec);
}
//-----------------------------------------------------------------------------
 void DLogic::EditMode(Int_t sec) {
  DisableStrobe(sec);
  StopPatGen(sec);
  SelectVMEInput(sec);
  SelectRAMOutput(sec);
}
//-----------------------------------------------------------------------------
 void DLogic::DumpConfReg() {
  int i;
  for (i=0; i<=10; i++) {
    printf("%2d  ",i);
    printf("%1dn", (fConfReg>>i)&1);
  }
}
//-----------------------------------------------------------------------------
 void DLogic::DumpARAM() {
  int i,j;
  printf("n");
  for (i=0; i<16; i++) {
    printf("%3d:    ", 16*i);
    for (j=0; j<16; j++) {
      printf("%2x  ", fARAM[16*i+j] & 0xff);
    }
    printf("n");
  }
}
//-----------------------------------------------------------------------------
 void DLogic::DumpBRAM() {
  int i,j;
  printf("n");
  for (i=0; i<16; i++) {
    printf("%3d:    ", 16*i);
    for (j=0; j<16; j++) {
      printf("%2x  ", fBRAM[16*i+j] & 0xff);
    }
    printf("n");
  }
}
//-----------------------------------------------------------------------------
 void DLogic::ExtractLogic(Int_t sec, Int_t outchan, TString *tbuf) {
  // Scan the data matrix and try to figure out the logic underlying the 
  // outchan-th output bit.  The routine is intelligent enough to recognize 
  // coincidences and anticoincidences. The result will be stored in 
  // tbuf in the format like [0]&^[1]. 

  // or[i] (and[i]) contains an OR (AND) of all such input bit configurations 
  // that the i-th output bit is 1. 

  Int_t j;
  UShort_t on,off;
  Char_t temp[80];
  UShort_t *fData;
  Int_t always0;
  Int_t always1;

  on = 0xff;
  off = 0xff;
  always0 = 1;
  always1 = 1;

  if (sec == 0) fData = fARAM;
  else if (sec == 1) fData = fBRAM;
  else {
    std::cout << "DLogic::ExtractLogic: What?n";
    return;
  }
  
  // scan the input bit configurations, determine or[outchan] and and[outchan]

  for (j=0; j<256; j++) {
    if (fData[j]>>outchan & 1) {
      always0 = 0;
      on = on & j;
      off = off & (0xff^j);
    } else {
      always1 = 0;
    }
  }

  // extract logic from or, and, always0, and always1

  tbuf->Remove(0);

  if (always0) {
    sprintf(temp,"0");
    tbuf->Append(temp);
  }

  if (always1) {
    sprintf(temp,"1");
    tbuf->Append(temp);
  }

  if ((on & off) == 0 && (on | off) != 0) {   // reasonable combination
    for (j=0; j<8; j++) {
      if (on>>j & 1) {
	sprintf(temp,"[%1d]&",j);
	tbuf->Append(temp);
      }
      if (off>>j & 1) {
	sprintf(temp,"![%1d]&",j);
	tbuf->Append(temp);
      }
    }

  // some cosmetics

  if (tbuf->Length()>0) 
    tbuf->Remove(tbuf->Length()-1,1);

  }

    //  std::cout << outchan << "on=" << on << "off=" << off << std::endl;

}
//-----------------------------------------------------------------------------
 void DLogic::DefineLogic(Int_t sec, Int_t outchan, char *definition) {
  // Redefine the i-th bit of the array fData cells to be equal to 
  // definition, where definition represents a logic function of
  // the input bits. 
  // outchan - which output bit (0-7)
  // definition - logic expression, like "([0] & [1]) | [7]"

  TFormula *outdef;
  Int_t j,k;
  UShort_t usbuf[256];

  memcpy(usbuf, GetRAM(sec), 512);
  outdef = new TFormula("outdef",definition);

  for (j=0; j<256; j++) {
    for (k=0; k<8; k++) {
      outdef->SetParameter(k, (j>>k) & 0x1);
    }
    setbit(&usbuf[j], outchan, (Int_t) (outdef->Eval(0)) & 0x1);
  }

  delete outdef;

  WriteRAM(sec, usbuf);
}
//-----------------------------------------------------------------------------
 void DLogic::AddToLogic(Int_t sec, Int_t outchan, char *definition) {
  // Append string definition to the current logic definition of chan outchan.

  TString tbuf;
  ExtractLogic(sec, outchan, &tbuf);
  if (tbuf.Length() == 0) {
    std::cout << "DLogic::AddToLogic: attempt to add " << definition << "to empty definition" << std::endl;
    return;
  }
  tbuf.Append(definition);
  DefineLogic(sec, outchan, (char *) tbuf.Data());
}
//-----------------------------------------------------------------------------
 Int_t DLogic::LogicContains(Int_t sec, Int_t outchan, char *definition) {
  // Check if logic definition of channel outchan contains string definition.
  TString tbuf;
  ExtractLogic(sec, outchan, &tbuf);
  return tbuf.Contains(definition);
}
//-----------------------------------------------------------------------------
 void DLogic::RemoveFromLogic(Int_t sec, Int_t outchan, char *definition) {
  // Remove string definition from current logic definition of channel outchan.
  TString tbuf;
  ExtractLogic(sec, outchan, &tbuf);
  if (tbuf.Contains(definition)) {
    tbuf.ReplaceAll(definition,"");
    DefineLogic(sec, outchan, (char *) tbuf.Data());
  } else {
    std::cout << "DLogic::RemoveFromLogic warning: attempt to remove " << definition << "from " << tbuf.Data() << std::endl;
  }
}
//*****************************************************************************

