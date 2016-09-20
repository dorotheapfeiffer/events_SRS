#include "DGDL633.h"

  ClassImp(DGDL633)

//-----------------------------------------------------------------------------
 DGDL633::DGDL633(DModule *module) : DGModule((DModule *) module) {
  Int_t j;

  std::cout << "constructing DGDL633" << std::endl;

  fMDat->SetLayoutManager(new TGMatrixLayout(fMDat,5,0,5 ));

  fMDat->AddFrame(new TGLabel(fMDat, new TGString("  ")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("0.")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("1.")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("2.")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("3.")));

  fMDat->AddFrame(new TGLabel(fMDat, new TGString("enable  ")));
  for (j=0; j<4; j++) {
    fEnaInp[j] = new TGCheckButton(fMDat,"",100+j);
    fEnaInp[j]->Associate(this);
    fMDat->AddFrame(fEnaInp[j]);
    }
    
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("AND")));
  for (j=0; j<4; j++) {
    fAndInp[j] = new TGRadioButton(fMDat,"",200+j);
    fAndInp[j]->Associate(this);
    fMDat->AddFrame(fAndInp[j]);
  }

  fMDat->AddFrame(new TGLabel(fMDat, new TGString("OR")));
  for (j=0; j<4; j++) {
    fOrInp[j] = new TGRadioButton(fMDat,"",300+j);
    fOrInp[j]->Associate(this);
    fMDat->AddFrame(fOrInp[j]);
  }

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGDL633::~DGDL633() {
  std::cout <<"destroying DGDL633" << std::endl;
}
//-----------------------------------------------------------------------------
 void DGDL633::Refresh() {
  // Update display so it represents the actual contents of fModule.
  Int_t j;
  DDL633 *dl633;

  dl633 = (DDL633 *) fModule;

  for (j=0; j<4; j++) {
    fEnaInp[j]->SetState((EButtonState) dl633->IsEnabled(j));
    fOrInp[j]->SetState((EButtonState) dl633->IsOr(j));
    fAndInp[j]->SetState((EButtonState) dl633->IsAnd(j));
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGDL633::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  fModule->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGDL633::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t col,cha;
  DDL633 *dl633;

  DGModule::ProcessMessage(msg, parm1, parm2);
  dl633 = (DDL633 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_CHECKBUTTON:
      col = parm1/100;
      cha = parm1-100*col;
      if (col<1 || col>1) break;
      if (cha<0 || cha>3) break;
      if (fEnaInp[cha]->GetState()) dl633->Enable(cha);
      else dl633->Disable(cha);
      break;
    case kCM_RADIOBUTTON:
      col = parm1/100;
      cha = parm1-100*col;
      if (col<2 || col>3) break;
      if (cha<0 || cha>3) break;

      switch (col) {
      case 2:
	fOrInp[cha]->SetState(kButtonUp);
	dl633->AndLogic(cha);
	break;
      case 3:
	fAndInp[cha]->SetState(kButtonUp);
	dl633->OrLogic(cha);
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

