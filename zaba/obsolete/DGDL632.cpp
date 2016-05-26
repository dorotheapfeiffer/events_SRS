#include "DGDL632.h"

  ClassImp(DGDL632)

//-----------------------------------------------------------------------------
 DGDL632::DGDL632(DModule *module) : DGModule((DModule *) module) {
  Int_t i;
  TGLayoutHints *fL0,*fL1,*fL2;

  std::cout << "constructing DGDL632" << std::endl;

  //  fMDat->SetLayoutManager(new TGMatrixLayout(fMDat,5,0,5));

  fMDat->ChangeOptions((fMDat->GetOptions() & ~kHorizontalFrame) | kVerticalFrame);

  fL0 = new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,5,5);
  fMDat->AddFrame(new TGLabel(fMDat, new TGString("fix   puls extin   source   out")), fL0);

  fL1 = new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,1,1,3,3);
  for (i=0; i<4; i++) {
    fSlice[i] = new TGHorizontalFrame(fMDat,0,0,0);
    fMDat->AddFrame(fSlice[i],fL1);
  }

  fL2 = new TGLayoutHints(kLHintsLeft,5,5,1,1);
  fSlice[0]->AddFrame(new TGLabel(fSlice[0], new TGString("0.")),fL2);
  fSlice[1]->AddFrame(new TGLabel(fSlice[1], new TGString("1.")),fL2);
  fSlice[2]->AddFrame(new TGLabel(fSlice[2], new TGString("2.")),fL2);
  fSlice[3]->AddFrame(new TGLabel(fSlice[3], new TGString("3.")),fL2);

  for (i=0; i<4; i++) {
    fFix[i] = new TGCheckButton(fSlice[i],"",100+i);
    fSlice[i]->AddFrame(fFix[i],fL2);
    fFix[i]->Associate(this);

    fPuls[i] = new TGTextButton(fSlice[i],"gen",200+i);
    fSlice[i]->AddFrame(fPuls[i],fL2);
    fPuls[i]->Associate(this);
    
    fExtin[i] = new TGLabel(fSlice[i], new TGString("?"));
    fSlice[i]->AddFrame(fExtin[i],fL2);

    fSource[i] = new TGComboBox(fSlice[i], 300+i);
    fSlice[i]->AddFrame(fSource[i],fL2);
    fSource[i]->AddEntry("taster", 0);
    fSource[i]->AddEntry("fix",    1);
    fSource[i]->AddEntry("puls",   2);
    fSource[i]->AddEntry("extin",  3);
    fSource[i]->Associate(this);

    fOut[i] = new TGLabel(fSlice[i], new TGString("?"));
    fSlice[i]->AddFrame(fOut[i],fL2);

    fSource[i]->Resize(60, 1.2*fExtin[i]->GetDefaultHeight());
    fPuls[i]->Resize(fPuls[i]->GetDefaultWidth(), 10);
  }

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGDL632::~DGDL632() {
  std::cout <<"destroying DGDL632" << std::endl;
}
//-----------------------------------------------------------------------------
 void DGDL632::Refresh() {
  // Update display so it represents the actual contents of fModule.

  Char_t buf[80];
  Int_t i;
  DDL632 *dl632;

  dl632 = (DDL632 *) fModule;

  for (i=0; i<4; i++) {
    fFix[i]->SetState((EButtonState) dl632->GetFix(i));

    sprintf(buf,"%d",dl632->GetExtin(i));
    fExtin[i]->SetText(new TGString(buf));

    fSource[i]->Select(dl632->GetMode(i));

    sprintf(buf,"%d",dl632->GetOut(i));
    fOut[i]->SetText(new TGString(buf));
  }
  DoRedraw();
}
//-----------------------------------------------------------------------------
 Bool_t DGDL632::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  fModule->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGDL632::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t col,cha;
  DDL632 *dl632;

  DGModule::ProcessMessage(msg, parm1, parm2);
  dl632 = (DDL632 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_CHECKBUTTON:
      col = parm1/100;
      cha = parm1-100*col;
      if (col != 1) break;
      if (cha<0 || cha>3) break;
      if (fFix[cha]->GetState()) dl632->SetFix(cha);
      else dl632->ResetFix(cha);
    case kCM_BUTTON:
      col = parm1/100;
      cha = parm1-100*col;
      if (col != 2) break;
      if (cha<0 || cha>3) break;
      dl632->GenPuls(cha);
    case kCM_COMBOBOX:
      col = parm1/100;
      cha = parm1-100*col;
      if (col != 3) break;
      if (cha<0 || cha>3) break;
      dl632->SetMode(cha, (Int_t) parm2);
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

