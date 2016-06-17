
#include <iostream>
#include <stdlib.h>
#include "DGDownscaler.h"

  ClassImp(DGDownscaler)

//-----------------------------------------------------------------------------
 DGDownscaler::DGDownscaler(DDownscaler *module) 
  : DGModule((DModule *) module) {
  Int_t i;
  TGLayoutHints *fL1,*fL2;

  std::cout << "constructing DGDownscaler" << std::endl;

  fCounDat    = new TGCompositeFrame(fMDat, 60, 20, kVerticalFrame);
  fCounDat->SetLayoutManager(new TGMatrixLayout(fCounDat, 7, 0, 5 ));

  fLay = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5,5,10,5);
  fL1 = new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 5,5,10,5);
  fL2 = new TGLayoutHints(kLHintsLeft, 2,2,5,0);
  fMDat->AddFrame(fCounDat, fLay);

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("chan")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 0.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 1.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 2.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 3.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 4.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 5.")));

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("enab")));
  for (i=0; i<6; i++) {
    fCounDat->AddFrame(fEnable[i] = new TGCheckButton(fCounDat,"",120+i), fL2);
    fEnable[i]->SetToolTipText("Enable channel");
    fEnable[i]->Associate(this);
  }

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("enor")));
  for (i=0; i<6; i++) {
    fCounDat->AddFrame
      (fEnableInOr[i] = new TGCheckButton(fCounDat,"",130+i), fL2);
    fEnableInOr[i]->SetToolTipText("Enable channel in OR");
    fEnableInOr[i]->Associate(this);
  }

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("ds")));
  for (i=0; i<6; i++) {
    fDownT[i] = new TGTextBuffer(6);
    fDownE[i] = new TGTextEntry(fCounDat, fDownT[i], 140+i);
    fDownE[i]->Resize(50, fDownE[i]->GetDefaultHeight());
    fDownE[i]->Associate(this);
    fCounDat->AddFrame(fDownE[i]);
  }

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("opat")));
  for (i=0; i<6; i++) {
    fOutpat[i] = new TGLabel(fCounDat, new TGString("?"));
    fCounDat->AddFrame(fOutpat[i]);
  }

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("count")));
  for (i=0; i<6; i++) {
    fCount[i] = new TGLabel(fCounDat, new TGString("?????"));
    fCounDat->AddFrame(fCount[i]);
  }

  fResetDownscalersButton  = new TGTextButton(fMDat, "Reset downscalers", 101);
  fResetOutputButton   = new TGTextButton(fMDat, "Reset output pattern", 102);
  fClearCountersButton = new TGTextButton(fMDat, "Clear counters", 103);
  fResetDownscalersButton ->Associate(this);
  fResetOutputButton      ->Associate(this);
  fClearCountersButton    ->Associate(this);
  fMDat->AddFrame(fResetDownscalersButton, fLay);
  fMDat->AddFrame(fResetOutputButton,      fLay);
  fMDat->AddFrame(fClearCountersButton,    fLay);

  fCounDat->Resize(fCounDat->GetDefaultSize());
  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGDownscaler::~DGDownscaler() {
  std::cout <<"destroying DGDownscaler" << std::endl;
}
//-----------------------------------------------------------------------------
 void DGDownscaler::Refresh() {
  // Update display so it represents the actual contents of fModule.
  Int_t  i;
  Char_t buf[80];
  DDownscaler *downscaler;

  downscaler = (DDownscaler *) fModule;

  for (i=0; i<6; i++) {
    if (downscaler->GetEnableBit(i)) fEnable[i]->SetState(kButtonDown);
    else                             fEnable[i]->SetState(kButtonUp);
    if (downscaler->GetEnableInOrBit(i)) fEnableInOr[i]->SetState(kButtonDown);
    else                                 fEnableInOr[i]->SetState(kButtonUp);
    sprintf(buf,"%d",downscaler->GetDownscale(i));
    fDownT[i]->Clear();
    fDownT[i]->AddText(0,buf);
    fClient->NeedRedraw(fDownE[i]);
    sprintf(buf,"%d",downscaler->GetOutputBit(i));
    fOutpat[i]->SetText(new TGString(buf));
    sprintf(buf,"%d",downscaler->GetCounter(i) );
    fCount[i]->SetText(new TGString(buf));
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGDownscaler::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  ((DDownscaler *) fModule)->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGDownscaler::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t i,j;
  DDownscaler *downscaler;
  downscaler = (DDownscaler *) fModule;
  DGModule::ProcessMessage(msg, parm1, parm2);

  //  printf("DGDownscaler process message %lx %lx %lxn",msg,parm1,parm2);
  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      if (parm1 == 101) downscaler->ResetDownscalers();
      if (parm1 == 102) downscaler->ResetOutputLevel();
      if (parm1 == 103) downscaler->ClearCounters();
      break;
    case kCM_CHECKBUTTON:
      if (parm1>=120 && parm1<=126) {
	i = parm1-120;
	if (fEnable[i]->GetState() == kButtonDown) downscaler->Enable(i);
	else downscaler->Disable(i);
      }
      if (parm1>=130 && parm1<=136) {
	i = parm1-130;
	if (fEnableInOr[i]->GetState() == kButtonDown) downscaler->EnableInOr(i);
	else downscaler->DisableInOr(i);
      }
      break;
    }
    break;
  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_ENTER:
      for (i=0; i<6; i++) {
	if (140+i == parm1) {
	  if (fRefresh->GetState()) fTimer->TurnOn();
	  j = atoi(fDownT[i]->GetString());
	  downscaler->WriteDownscale(i,j);
	  downscaler->ReadVME();
	  Refresh();
	}
      }
      break;
    case kTE_TEXTCHANGED:
      if (parm1>=140 && parm1<146) fTimer->TurnOff();
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

