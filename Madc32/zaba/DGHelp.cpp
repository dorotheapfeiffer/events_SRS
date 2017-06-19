#include "DGHelp.h"

  ClassImp(DGHelp)

#define ENTHEI 20
#define ENTWID 10

//-----------------------------------------------------------------------------
 DGHelp::DGHelp(char * filename) :
  TGTransientFrame(gClient->GetRoot(), 0, 0, 0,kVerticalFrame) {

  fMemList = new TObjArray();
  fL0 = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2);
  fL1 = new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,
			  2, 2, 2, 2);
  fTextView = new TGTextView(this, 550, 240, kChildFrame, 3);
  fCloseButton = new TGTextButton(this,"Close",71);

  AddFrame(fTextView,fL1);
  AddFrame(fCloseButton,fL0);

  fMemList->Add(fL0);
  fMemList->Add(fL1);
  fMemList->Add(fTextView);
  fMemList->Add(fCloseButton);

  fTextView->LoadFile(filename);

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
  SetWindowName("CERES trigger help");
}
//-----------------------------------------------------------------------------
 DGHelp::~DGHelp() {
  fMemList->Delete();
}
//-----------------------------------------------------------------------------
 Bool_t DGHelp::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

  fTextView->ProcessMessage(msg, parm1, parm2);

  switch(GET_MSG(msg)) {  
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      if (parm1 == 71) delete this;
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

