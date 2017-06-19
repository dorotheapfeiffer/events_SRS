///////////////////////////////////////////////////////////////////////////////
// DGScaler is a GUI for DScaler
///////////////////////////////////////////////////////////////////////////////
//
//

//

#include "DGScaler.h"

  ClassImp(DGScaler)

//-----------------------------------------------------------------------------
 DGScaler::DGScaler(DScaler *module) : DGModule((DModule *) module) {
  Int_t i;
  char buf[80];
  Long_t count;

  std::cout << "constructing DGScaler" << std::endl;

  fCounDat    = new TGCompositeFrame(fMDat, 60, 20, kVerticalFrame);
  fCounDat->SetLayoutManager(new TGMatrixLayout(fCounDat, 16, 0, 5 ));

  fLay = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5,5,10,5);
  fMDat->AddFrame(fCounDat, fLay);

  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 0.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 1.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 2.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 3.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 4.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 5.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 6.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 7.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 8.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString(" 9.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("10.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("11.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("12.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("13.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("14.")));
  fCounDat->AddFrame(new TGLabel(fCounDat, new TGString("15.")));

  for (i=0; i<16; i++) {
    count = ((DScaler*) module)->GetCount(i);
    sprintf(buf,"%ld",count);
    fTbuf[i] = new TGTextBuffer(15);
    fTbuf[i]->AddText(0,buf);
    fTent[i] = new TGTextEntry(fCounDat, fTbuf[i], 0);
    fTent[i]->Resize(80, fTent[i]->GetDefaultHeight());
    fTent[i]->Associate(this);
    //    fCounDat->AddFrame(fTent[i],fLay);
    fCounDat->AddFrame(fTent[i]);
  }

  fIncrButton  = new TGTextButton(fMDat, "&Increment counters",  101);
  fClearButton = new TGTextButton(fMDat, "&Clear counters", 102);
  fIncrButton->Associate(this);
  fClearButton->Associate(this);
  fMDat->AddFrame(fIncrButton, fLay);
  fMDat->AddFrame(fClearButton, fLay);

  fCounDat->Resize(fCounDat->GetDefaultSize());
  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
}

//-----------------------------------------------------------------------------
 DGScaler::~DGScaler() {
  std::cout <<"destroying DGScaler" << std::endl;
}
//-----------------------------------------------------------------------------
 void DGScaler::Refresh() {
  // Update display so it represents the actual contents of fModule. 
  Int_t  i;
  Char_t buf[80];
  Long_t count;
  DScaler *scaler;

  scaler = (DScaler *) fModule;

  for (i=0; i<16; i++) {
    count = scaler->GetCount(i);
    sprintf(buf, "%ld", count);
    fTbuf[i]->Clear();
    fTbuf[i]->AddText(0,buf);
    fClient->NeedRedraw(fTent[i]);
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGScaler::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  ((DScaler *) fModule)->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGScaler::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  DScaler   *scaler;
  scaler = (DScaler *) fModule;
  DGModule::ProcessMessage(msg, parm1, parm2);

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      if (parm1 == 101) scaler->IncrementScale();
      if (parm1 == 102) scaler->ClearScale();
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

