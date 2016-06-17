#include "DGDL634.h"

  ClassImp(DGDL634)

//-----------------------------------------------------------------------------
 DGDL634::DGDL634(DModule *module) : DGModule((DModule *) module) {
  Int_t j;

  std::cout << "constructing DGDL634" << std::endl;

  fMDat->SetLayoutManager(new TGMatrixLayout(fMDat,5,0,5));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("  ")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("0.")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("1.")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("2.")));
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("3.")));

  fMDat->AddFrame(new TGLabel(fMDat, new TGString("norm")));
  for (j=0; j<4; j++) {
    fNormOut[j] = new TGRadioButton(fMDat,"",300+j);
    fNormOut[j]->Associate(this);
    fMDat->AddFrame(fNormOut[j]);
  }
  
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("inv")));
  for (j=0; j<4; j++) {
    fInvOut[j] = new TGRadioButton(fMDat,"",400+j);
    fInvOut[j]->Associate(this);
    fMDat->AddFrame(fInvOut[j]);
  }

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGDL634::~DGDL634() {
  std::cout <<"destroying DGDL634" << std::endl;
}
//-----------------------------------------------------------------------------
 void DGDL634::Refresh() {
  // Update display so it represents the actual contents of fModule.
  Int_t j;
  DDL634 *dl634;

  dl634 = (DDL634 *) fModule;

  for (j=0; j<4; j++) {
    fNormOut[j]->SetState((EButtonState) dl634->IsNorm(j));
    fInvOut[j]->SetState((EButtonState) dl634->IsInv(j));
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGDL634::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  fModule->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGDL634::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t col,cha;
  DDL634 *dl634;

  DGModule::ProcessMessage(msg, parm1, parm2);
  dl634 = (DDL634 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_RADIOBUTTON:
      col = parm1/100;
      cha = parm1-100*col;
      if (col<3 || col>4) break;
      if (cha<0 || cha>3) break;
      switch (col) {
      case 3:
	fInvOut[cha]->SetState(kButtonUp);
	dl634->Norm(cha);
	break;
      case 4:
	fNormOut[cha]->SetState(kButtonUp);
	dl634->Inv(cha);
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

