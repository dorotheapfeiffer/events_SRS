#include "DGDisc.h"

  ClassImp(DGDisc)

//-----------------------------------------------------------------------------
 DGDisc::DGDisc(DDisc *module) : DGModule((DModule *) module) {
  int i;

  std::cout << "constructing DGDisc" << std::endl;

  fL0 = new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 5, 5, 5,5);

  fMDat->ChangeOptions((fMDat->GetOptions() & ~kVerticalFrame) | kHorizontalFrame);

  for (i=0; i<4; i++) {
    fVertSlice[i] = new TGVerticalFrame(fMDat, 0, 0, 0);
    fMDat->AddFrame(fVertSlice[i], fL0);
  }

  fVertSlice[0]->AddFrame(new TGLabel(fVertSlice[0], new TGString("chan 0")),fL0);
  fVertSlice[1]->AddFrame(new TGLabel(fVertSlice[1], new TGString("chan 1")),fL0);
  fVertSlice[2]->AddFrame(new TGLabel(fVertSlice[2], new TGString("chan 2")),fL0);
  fVertSlice[3]->AddFrame(new TGLabel(fVertSlice[3], new TGString("chan 3")),fL0);

  for (i=0; i<4; i++) {
    fTbuf[i] = new TGTextBuffer(32);
    fTbuf[i]->AddText(0,"0");
    fTent[i] = new TGTextEntry(fVertSlice[i], fTbuf[i], 120+i);
    fTent[i]->Resize(40, fTent[i]->GetDefaultHeight());
    fTent[i]->Associate(this);
    fVertSlice[i]->AddFrame(fTent[i],fL0);

    fVsli[i] = new TGVSlider(fVertSlice[i], 100, kSlider2 | kScaleBoth, 130+i);
    fVsli[i]->SetRange(0,255);
    fVsli[i]->SetPosition(255);
    fVsli[i]->Associate(this);
    fVertSlice[i]->AddFrame(fVsli[i],fL0);
  }

  MapSubwindows();
  Resize(GetDefaultSize());

  Layout();
  MapWindow();
  Refresh();
}

//-----------------------------------------------------------------------------
 DGDisc::~DGDisc() {
  std::cout <<"destroying DGDisc" << std::endl;
  for (Int_t i=0; i<4; i++) {
    delete fVsli[i];
    delete fTent[i];
    delete fVertSlice[i];
  }
}
//-----------------------------------------------------------------------------
 void DGDisc::Refresh() {
  // Update display such that is represents the actual contents of fModule.

  Int_t   i,thr;
  Char_t  buf[80];
  DDisc   *disc;

  disc = (DDisc *) fModule;

  for (i=0; i<4; i++) {
    thr = disc->GetThreshold(i);
    fVsli[i]->SetPosition(255-thr);
    sprintf(buf, "%d", thr);
    fTbuf[i]->Clear();
    fTbuf[i]->AddText(0,buf);
    fClient->NeedRedraw(fTent[i]);
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGDisc::HandleTimer(TTimer *tim)
{
  DGModule::HandleTimer(tim);
  ((DDisc *) fModule)->ReadVME();
  Refresh();
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGDisc::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int i;
  int j;
  char buf[80];
  DDisc   *disc;

  disc = (DDisc *) fModule;

  DGModule::ProcessMessage(msg, parm1, parm2);

  switch (GET_MSG(msg)) {
  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_ENTER:
      for (i=0; i<4; i++) {
	if (120+i == parm1) {
	  if (fRefresh->GetState()) fTimer->TurnOn();
	  j = atoi(fTbuf[i]->GetString());
	  fVsli[i]->SetPosition(255-j);
	  disc->WriteThreshold(i,j);
	  disc->ReadVME();
	  Refresh();
	}
      }
      break;
    case kTE_TEXTCHANGED:
      if (parm1>=120 && parm1<124) fTimer->TurnOff();
      break;
    default:
      break;
    }
    break;
  case kC_VSLIDER:
    switch (GET_SUBMSG(msg)) {
    case kSL_POS:
      j=255 - (int) parm2;
      sprintf(buf, "%d", j);
      for (i=0; i<4; i++) {
	if (130+i == parm1) {
	  fTbuf[i]->Clear();
	  fTbuf[i]->AddText(0,buf);
	  fClient->NeedRedraw(fTent[i]);
	  disc->WriteThreshold(i,j);
	}
      }
      break;
    }
    break;
  default:
    break;
  }
  return kTRUE;
}
//-----------------------------------------------------------------------------

