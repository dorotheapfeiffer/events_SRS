
#include "DGDL631.h"

  ClassImp(DGDL631)

//-----------------------------------------------------------------------------
 DGDL631::DGDL631(DModule *module) : DGModule((DModule *) module) {
  Int_t i;

  std::cout << "constructing DGDL631" << std::endl;

  fL0   = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 0,0,5,5);

  fMatFrame = new TGCompositeFrame(fMDat, 0, 0);
  fResetFF = new TGTextButton(fMDat,"reset flip-flops",300);
  fResetFF->Associate(this);
  fMDat->AddFrame(fMatFrame, fL0);
  fMDat->AddFrame(fResetFF, fL0);

  // Matrix frame 

  fMatFrame->SetLayoutManager(new TGMatrixLayout(fMatFrame,5,0,5 ));

  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("  ")));
  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("0.")));
  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("1.")));
  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("2.")));
  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("3.")));

  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("inp")));
  for (i=0; i<4; i++) {
    fInp[i] = new TGLabel(fMatFrame, new TGString("?"));
    fMatFrame->AddFrame(fInp[i]);
  }

  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("mask")));
  for (i=0; i<4; i++) {
    fMask[i] = new TGCheckButton(fMatFrame,"",100+i);
    fMask[i]->Associate(this);
    fMatFrame->AddFrame(fMask[i]);
  }

  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("FFmd")));
  for (i=0; i<4; i++) {
    fMaskedFF[i] = new TGCheckButton(fMatFrame,"",200+i);
    fMaskedFF[i]->Associate(this);
    fMatFrame->AddFrame(fMaskedFF[i]);
  }

  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("FF")));
  for (i=0; i<4; i++) {
    fFlipFlop[i] = new TGLabel(fMatFrame, new TGString("?"));
    fMatFrame->AddFrame(fFlipFlop[i]);
  }

  fMatFrame->AddFrame(new TGLabel(fMatFrame, new TGString("int")));
  for (i=0; i<4; i++) {
    fInterrupt[i] = new TGLabel(fMatFrame, new TGString("?"));
    fMatFrame->AddFrame(fInterrupt[i]);
  }

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGDL631::~DGDL631() {
  std::cout <<"destroying DGDL631" << std::endl;
  for (Int_t i=0; i<4; i++) {
    delete fInp[i];
    delete fMask[i];
    delete fMaskedFF[i];
    delete fFlipFlop[i];
    delete fInterrupt[i];
  }
  delete fL0;
  delete fMatFrame;
  delete fResetFF;
}
//-----------------------------------------------------------------------------
 void DGDL631::Refresh() {
  // Update display so it represents the actual contents of fModule.
  Char_t buf[80];
  Int_t i;
  DDL631 *dl631;

  dl631 = (DDL631 *) fModule;

  for (i=0; i<4; i++) {
    sprintf(buf,"%d",dl631->GetInput(i));
    fInp[i]->SetText(new TGString(buf));

    fMask[i]->SetState((EButtonState) dl631->IsIntEnabled(i));
    fMaskedFF[i]->SetState((EButtonState) dl631->IsFlipFlopMasked(i));

    sprintf(buf,"%d",dl631->GetFlipFlop(i));
    fFlipFlop[i]->SetText(new TGString(buf));

    sprintf(buf,"%d",dl631->GetInterrupt(i));
    fInterrupt[i]->SetText(new TGString(buf));
  }
  DoRedraw();
}
//-----------------------------------------------------------------------------
 Bool_t DGDL631::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  fModule->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGDL631::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t col,cha;
  DDL631 *dl631;

  DGModule::ProcessMessage(msg, parm1, parm2);
  dl631 = (DDL631 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      if (parm1 == 300) dl631->ResetFlipFlops();
      break;
    case kCM_CHECKBUTTON:
      col = parm1/100;
      cha = parm1-100*col;
      if (cha<0 || cha>3) break;
      switch (col) {
      case 1:
	if (fMask[cha]->GetState()) dl631->EnableInt(cha);
	else dl631->DisableInt(cha);
	break;
      case 2:
	if (fMaskedFF[cha]->GetState()) dl631->FlipFlopsMasked();
	else dl631->FlipFlopsFree();
	break;
      default:
	break;
      }
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

