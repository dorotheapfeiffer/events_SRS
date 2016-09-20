#include "DGLogEditor.h"

  ClassImp(DGLogEditor)

//-----------------------------------------------------------------------------
 DGLogEditor::DGLogEditor(const TGWindow *main, DLogic *logic, Int_t sec) : 
  TGTransientFrame(gClient->GetRoot(),main,500,500,kHorizontalFrame) {
  Int_t i;
  Char_t buf[80];

  std::cout << "constructing DGLogEditor" << std::endl;

  fMemList = new TObjArray();
  fLogic = logic;
  fSec = sec;

  fLHLeft = new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 4,0,5,5);
  fLHTop = new TGLayoutHints(kLHintsTop, 5,5,5,5);
  fLHBottom = new TGLayoutHints(kLHintsBottom, 5,5,5,5);
  fMemList->Add(fLHLeft);
  fMemList->Add(fLHTop);
  fMemList->Add(fLHBottom);

  fLabFrame  = new TGVerticalFrame(this, 0, 0);
  fOutDefFrame = new TGVerticalFrame(this,0,0);
  fButtonFrame = new TGVerticalFrame(this, 0, 0);
  AddFrame(fLabFrame, fLHLeft);
  AddFrame(fOutDefFrame, fLHLeft);
  AddFrame(fButtonFrame, fLHLeft);
  fMemList->Add(fLabFrame);
  fMemList->Add(fOutDefFrame);
  fMemList->Add(fButtonFrame);

  // Label frame

  for (i=0; i<8; i++){ 
    sprintf(buf,"%1d",i);
    fLab[i] = new TGLabel(fLabFrame, buf);
    fLabFrame->AddFrame(fLab[i], fLHLeft);
    fMemList->Add(fLab[i]);
  }

  // Output definition frame

  for (i=0; i<8; i++) {
    fTbuf[i] = new TGTextBuffer(80);
    fTbuf[i]->AddText(0,"");
    fTent[i] = new TGTextEntry(fOutDefFrame, fTbuf[i], 15000+i);
    fTent[i]->Resize(150, fTent[i]->GetDefaultHeight());
    fTent[i]->Associate(this);
    fOutDefFrame->AddFrame(fTent[i],fLHTop);
    //    fMemList->Add(fTbuf[i]);
    // TGTextBuffer does not inherit from TObject, and so it cannot be 
    // stored in ObjArray. Don't know what to do. 
    fMemList->Add(fTent[i]);
  }

  // Button frame

  fClearButton = new TGTextButton(fButtonFrame," Clear ",13001);
  fRefreshButton = new TGTextButton(fButtonFrame,"Refresh",13002);
  fDoneButton = new TGTextButton(fButtonFrame," Done ",13003);
  fClearButton->Associate(this);
  fRefreshButton->Associate(this);
  fDoneButton->Associate(this);
  fButtonFrame->AddFrame(fClearButton, fLHTop);
  fButtonFrame->AddFrame(fRefreshButton, fLHTop);
  fButtonFrame->AddFrame(fDoneButton, fLHBottom);
  fMemList->Add(fClearButton);
  fMemList->Add(fRefreshButton);
  fMemList->Add(fDoneButton);

  MapSubwindows();
  this->Resize(GetDefaultSize());
  SetWindowName("Logic Editor");
  MapWindow();
  Refresh();

  //gClient->WaitFor(this);    // Make the invoking application wait 
}

//-----------------------------------------------------------------------------
 DGLogEditor::~DGLogEditor() {
  std::cout <<"destroying DGLogEditor" << std::endl;
  fMemList->Delete();
}
//-----------------------------------------------------------------------------
 void DGLogEditor::Refresh() {
  // Update display such that it represents actual contents of fModule->RAM. 
  Int_t i;
  TString tbuf;

  for (i=0; i<8; i++) {
    fLogic->ExtractLogic(fSec, i, &tbuf);
    fTbuf[i]->Clear();
    fTbuf[i]->AddText(0,tbuf.Data());
    fClient->NeedRedraw(fTent[i]);
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGLogEditor::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t i;

  switch(GET_MSG(msg)) {  
  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_ENTER:
      if (parm1>=15000 && parm1<15008) {
	i = parm1-15000;
	fLogic->DefineLogic(fSec,i,(char *) fTbuf[i]->GetString());
	fLogic->ReadRAM();
	Refresh();
      }
      break;
    default:
      break;
    }
    break;
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      if (parm1 == 13001) {
	fLogic->WriteRAM(fSec, (UShort_t) 0);
	fLogic->ReadRAM();
	Refresh();
      }
      if (parm1 == 13002) {fLogic->ReadRAM(); Refresh();}
      if (parm1 == 13003) delete this;
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

