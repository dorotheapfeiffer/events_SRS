#include "DGLogic.h"

  ClassImp(DGLogic)

//-----------------------------------------------------------------------------
 DGLogic::DGLogic(DLogic *module) : DGModule((DModule *) module) {
  Int_t i,j;  
  Char_t buf[80];

  //  std::cout << "constructing DGLogic" << std::endl;

  fLVer0   = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 0,0,0,0);
  fLVer5   = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 5,5,5,5);
  fLVerLab = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 4,4,3,3);
  fLVerChe = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 4,4,3,2);
  fLVerEnt = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 4,4,0,0);
  fLHor0 = new TGLayoutHints(kLHintsLeft, 0,0,0,0);
  fLHor5 = new TGLayoutHints(kLHintsLeft, 5,5,5,5);
  fMemList->Add(fLVer0);
  fMemList->Add(fLVer5);
  fMemList->Add(fLVerLab);
  fMemList->Add(fLVerChe);
  fMemList->Add(fLVerEnt);
  fMemList->Add(fLHor0);
  fMemList->Add(fLHor5);

  fMenuARAM = new TGPopupMenu(fClient->GetRoot());
  fMenuARAM->AddEntry("Dump", 81);
  fMenuARAM->AddEntry("Load", 82);
  fMenuARAM->AddEntry("Invoke pattern editor", 83);
  fMenuARAM->AddEntry("Invoke logic editor", 84);
  fMenuARAM->Associate(this);

  fMenuBRAM = new TGPopupMenu(fClient->GetRoot());
  fMenuBRAM->AddEntry("Dump", 91);
  fMenuBRAM->AddEntry("Load", 92);
  fMenuBRAM->AddEntry("Invoke pattern editor", 93);
  fMenuBRAM->AddEntry("Invoke logic editor", 94);
  fMenuBRAM->Associate(this);

  fMenuRAM = new TGPopupMenu(fClient->GetRoot());
  fMenuBar->AddPopup("&RAM", fMenuRAM, fLHor5);
  fMenuRAM->AddPopup("&A RAM", fMenuARAM);
  fMenuRAM->AddPopup("&B RAM", fMenuBRAM);
  fMenuRAM->Associate(this);

  fMemList->Add(fMenuARAM);
  fMemList->Add(fMenuBRAM);
  fMemList->Add(fMenuRAM);

  // two sections per module

  for (i=0; i<2; i++) {
    fSecFrame[i] = new TGCompositeFrame(fMDat, 60, 20, kHorizontalFrame);
    fMDat->AddFrame(fSecFrame[i], fLVer5);
    fMemList->Add(fSecFrame[i]);

    // 5 frames per section

    fNrFrame[i]  = new TGCompositeFrame(fSecFrame[i], 60, 20, kVerticalFrame);
    fInFrame[i]  = new TGCompositeFrame(fSecFrame[i], 60, 20, kVerticalFrame);
    fOutFrame[i]  = new TGCompositeFrame(fSecFrame[i], 60, 20, kVerticalFrame);

    fSecFrame[i]->AddFrame(fNrFrame[i], 
			   new TGLayoutHints(kLHintsLeft, 5,5,20,5));
    fSecFrame[i]->AddFrame(fInFrame[i], fLHor0);
    fSecFrame[i]->AddFrame(fOutFrame[i], fLHor0);

    fMemList->Add(fNrFrame[i]);
    fMemList->Add(fInFrame[i]);
    fMemList->Add(fOutFrame[i]);

    // frame with channel numbers

    for (j=0; j<8; j++){ 
      sprintf(buf,"%1d.",j);
      fNrLab[i][j] = new TGLabel(fNrFrame[i], new TGString(buf));
      fNrFrame[i]->AddFrame(fNrLab[i][j],fLVerLab);
      fMemList->Add(fNrLab[i][j]);
    }

    // input frame

    fInRegFrame[i] = new TGGroupFrame(fInFrame[i], "in", kVerticalFrame);
    fInpExt[i] = new TGRadioButton(fInFrame[i], "ext", 1000*i+101);
    fInpVME[i] = new TGRadioButton(fInFrame[i], "VME", 1000*i+102);
    fInpPat[i] = new TGRadioButton(fInFrame[i], "pattern", 1000*i+103);
    fInpExt[i]->Associate(this);
    fInpVME[i]->Associate(this);
    fInpPat[i]->Associate(this);
    fInFrame[i]->AddFrame(fInRegFrame[i], fLVer5);
    fInFrame[i]->AddFrame(fInpExt[i],     fLVer5);
    fInFrame[i]->AddFrame(fInpVME[i],     fLVer5);
    fInFrame[i]->AddFrame(fInpPat[i],     fLVer5);
    fMemList->Add(fInRegFrame[i]);
    fMemList->Add(fInpExt[i]);
    fMemList->Add(fInpVME[i]);
    fMemList->Add(fInpPat[i]);

    for (j=0; j<8; j++) {
      fInRegBit[i][j] = new TGCheckButton(fInRegFrame[i],"",1000*i+110+j);
      fInRegBit[i][j]->Associate(this);
      fInRegFrame[i]->AddFrame(fInRegBit[i][j], fLVerChe);
      fMemList->Add(fInRegBit[i][j]);
    }

    // output frame

    fOutRegFrame[i] = new TGGroupFrame(fOutFrame[i], "out", kVerticalFrame);
    fOutRAM[i] = new TGRadioButton(fOutFrame[i], "RAM", 1000*i+201);
    fOutVME[i] = new TGRadioButton(fOutFrame[i], "VME", 1000*i+202);
    fOutRAM[i]->Associate(this);
    fOutVME[i]->Associate(this);
    fOutFrame[i]->AddFrame(fOutRegFrame[i], fLVer5);
    fOutFrame[i]->AddFrame(fOutRAM[i],      fLVer5);
    fOutFrame[i]->AddFrame(fOutVME[i],      fLVer5);
    fMemList->Add(fOutRegFrame[i]);
    fMemList->Add(fOutRAM[i]);
    fMemList->Add(fOutVME[i]);

    for (j=0; j<8; j++) {
      fOutRegBit[i][j] = new TGCheckButton(fOutRegFrame[i],"",1000*i+210+j);
      fOutRegBit[i][j]->Associate(this);
      fOutRegFrame[i]->AddFrame(fOutRegBit[i][j], fLVerChe);
      fMemList->Add(fOutRegBit[i][j]);
    }

  }
  /*
    printf("entry def hei %dn",fOutDefEntr[0][0]->GetDefaultHeight());
    printf("TGLab def hei %dn",fNrLab[0][0]->GetDefaultHeight());
    printf("Check but def hei %dn",fInRegBit[0][0]->GetDefaultHeight());
  */

  /* What was this supposed to be??? 
  fInpExt[0]->SetState(kButtonDown);
  fInpExt[1]->SetState(kButtonDown);
  fOutRAM[0]->SetState(kButtonDown);
  fOutRAM[1]->SetState(kButtonDown);
  */

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGLogic::~DGLogic() {
  std::cout <<"destroying DGLogic" << std::endl;
}
//-----------------------------------------------------------------------------
 void DGLogic::Refresh() {
  // Update display such that it represents actual contents of fModule. 
  Int_t  i,j;
  UShort_t regist;
  DLogic *logic;

  logic = (DLogic *) fModule;

  // configuration register

  regist = logic->GetConfReg();
  for (i=0; i<2; i++) {
    if (regist >> (i+0) & 1) {
      fInpExt[i]->SetState(kButtonUp);
      fInpVME[i]->SetState(kButtonDown);
    } else {
      fInpExt[i]->SetState(kButtonDown);
      fInpVME[i]->SetState(kButtonUp);
    }
    if (regist >> (i+2) & 1) {
      fOutRAM[i]->SetState(kButtonDown);
      fOutVME[i]->SetState(kButtonUp);
    } else {
      fOutRAM[i]->SetState(kButtonUp);
      fOutVME[i]->SetState(kButtonDown);
    }
    if ( regist>>(i+6) & 1) {
      fInpExt[i]->SetState(kButtonUp);
      fInpVME[i]->SetState(kButtonUp);
      fInpPat[i]->SetState(kButtonDown);
    } else {
      fInpPat[i]->SetState(kButtonUp);
    }
  }

  for (i=0; i<2; i++) {

    // input register

    regist = logic->GetInputReg(i);
    for (j=0; j<8; j++) {
      fInRegBit[i][j]->SetState((EButtonState) ((regist>>j)&1));
    }

    // output register

    regist = logic->GetOutputReg(i);
    for (j=0; j<8; j++) {
      fOutRegBit[i][j]->SetState((EButtonState) ((regist>>j)&1));
    }
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGLogic::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  ((DLogic *) fModule)->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGLogic::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  DLogic *logic;
  UShort_t regist;
  Int_t i;
  Int_t sec;
  UShort_t usbuf[256];
  Char_t   tbuf[8][80];
  const char *filetypes[] = { "Look-up table files", "*.lut", 
			      "Pattern files", "*.pat",
			      "All files", "*",
			      0,0};
  TGFileInfo fi;
  FILE *lutfile;

  logic = (DLogic *) fModule;
  DGModule::ProcessMessage(msg, parm1, parm2);

  //printf("gdlogic  process message called %ld %ld %ldn",msg,parm1,parm2);

  switch(GET_MSG(msg)) {

  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_MENU:
      if (parm1 >= 81 && parm1 <= 99) {
	if (parm1 < 90) sec = 0;
	else sec = 1;
	if (parm1 == 81 || parm1 == 91) {
	  logic->ReadRAM();
	  if (parm1 == 81) logic->DumpARAM();
	  if (parm1 == 91) logic->DumpBRAM();
	}
	if (parm1 == 82 || parm1 == 92) {
	  fi.fFileTypes = (const char **)filetypes;
	  fi.fIniDir = (char*)"ROOT";
	  new TGFileDialog(fClient->GetRoot(), this, kFDOpen,&fi);

	  if (fi.fFilename) {
	    lutfile = fopen(fi.fFilename,"r");
	    for (i=0; i<256; fscanf(lutfile,"%hx",&usbuf[i++]));
	    fgets(tbuf[0],80,lutfile);
	    for (i=0; i<8; fgets(tbuf[i++],80,lutfile));
	    fclose(lutfile);
	    logic->WriteRAM(sec, usbuf);
	    Resize(GetDefaultSize());
	  }
	}
	if (parm1 == 83 || parm1 == 93) {         // invoke pattern editor
	  new DGPatEditor(this, logic, sec, kRestore);
	}
	if (parm1 == 84 || parm1 == 94) {         // invoke logic editor
	  new DGLogEditor(this,logic,sec);
	}
      }
      break;
    case kCM_CHECKBUTTON:
      if (parm1>= 110 && parm1< 118) {
	i = parm1-110;
	regist = logic->GetInputReg(0);
	setbit(&regist,i,fInRegBit[0][i]->GetState());  
	logic->WriteAddrReg(0,regist);
      }
      if (parm1>=1110 && parm1<1118) {
	i = parm1-1110;
	regist = logic->GetInputReg(1);
	setbit(&regist,i,fInRegBit[1][i]->GetState());   
	logic->WriteAddrReg(1,regist);
      }
      if (parm1>= 210 && parm1< 218) {
	i = parm1-210;
	regist = logic->GetOutputReg(0);
	setbit(&regist,i,fOutRegBit[0][i]->GetState());	 
	logic->WriteOutputReg(0,regist);
      }
      if (parm1>=1210 && parm1<1218) {
	i = parm1-1210;
	regist = logic->GetOutputReg(1);
	setbit(&regist,i,fOutRegBit[1][i]->GetState());	  
	logic->WriteOutputReg(1,regist);
      }
      break;
    case kCM_RADIOBUTTON:
      switch (parm1) {
      case 101:
	fInpVME[0]->SetState(kButtonUp);
	fInpPat[0]->SetState(kButtonUp);
	logic->StopPatGen(0);
	logic->SelectExtInput(0);
	break;
      case 102:
	fInpExt[0]->SetState(kButtonUp);
	fInpPat[0]->SetState(kButtonUp);
	logic->StopPatGen(0);
	logic->SelectVMEInput(0);
	break;
      case 103:
	fInpExt[0]->SetState(kButtonUp);
	fInpVME[0]->SetState(kButtonUp);
	logic->SelectVMEInput(0);
	logic->StartPatGen(0);
	break;
      case 1101:
	fInpVME[1]->SetState(kButtonUp);
	fInpPat[1]->SetState(kButtonUp);
	logic->StopPatGen(1);
	logic->SelectExtInput(1);
	break;
      case 1102:
	fInpExt[1]->SetState(kButtonUp);
	fInpPat[1]->SetState(kButtonUp);
	logic->StopPatGen(1);
	logic->SelectVMEInput(1);
	break;
      case 1103:
	fInpExt[1]->SetState(kButtonUp);
	fInpVME[1]->SetState(kButtonUp);
	logic->SelectVMEInput(1);
	logic->StartPatGen(1);
	break;
      case 201:
	fOutVME[0]->SetState(kButtonUp);
	logic->SelectRAMOutput(0);
	break;
      case 202:
	fOutRAM[0]->SetState(kButtonUp);
	logic->SelectVMEOutput(0);
	break;
      case 1201:
	fOutVME[1]->SetState(kButtonUp);
	logic->SelectRAMOutput(1);
	break;
      case 1202:
	fOutRAM[1]->SetState(kButtonUp);
	logic->SelectVMEOutput(1);
	break;
      default:
	break;
      }
    case kCM_BUTTON:
      /*
      if (parm1 == 101) logic->IncrementScale();
      if (parm1 == 102) logic->ClearScale();
      */
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return kTRUE;
}
//-----------------------------------------------------------------------------

