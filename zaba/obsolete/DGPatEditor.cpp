#include "DGPatEditor.h"

  ClassImp(DGPatEditor)

#define ENTHEI 20
#define ENTWID 10

//-----------------------------------------------------------------------------
 DGPatEditor::DGPatEditor(const TGWindow *main, DLogic *logic, Int_t sec, Int_t opt): 
  TGTransientFrame(gClient->GetRoot(),main,0,0,kHorizontalFrame) {
  Char_t buf[80];
  Int_t i, j;
  Int_t defheight;

  fMemList = new TObjArray();
  fLogic = logic;
  fSec = sec;
  fOptions = opt;

  if (fOptions & kRestore) {
    fConfreg = fLogic->GetConfReg();
    memcpy(fRam, fLogic->GetRAM(sec), 512);
  }

  std::cout << "constructing DGPatEditor" << std::endl;

  fLHLeft = new TGLayoutHints(kLHintsLeft, 5,5,8,0);
  fLHRight = new TGLayoutHints(kLHintsRight | kLHintsExpandY, 5,5,8,0);
  fLHTop = new TGLayoutHints(kLHintsTop, 0,0,0,0);
  fLHBottom = new TGLayoutHints(kLHintsBottom, 0,0,0,0);
  fLHCanvas = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 
				0, 0, 5, 0);

  fMemList->Add(fLHLeft);
  fMemList->Add(fLHRight);
  fMemList->Add(fLHTop);
  fMemList->Add(fLHBottom);
  fMemList->Add(fLHCanvas);

  fLeftLabFrame  = new TGVerticalFrame(this, 0, 0);
  fRightLabFrame = new TGVerticalFrame(this, 0, 0);
  fDataCanvas = new TGCanvas(this, 400, 180);
  fDataFrame = new TGCompositeFrame(fDataCanvas->GetViewPort(), 0, 0);
  fDataFrame->SetLayoutManager(new TGMatrixLayout(fDataFrame,11,0,0));
  fDataCanvas->SetContainer(fDataFrame);
  fButtonFrame = new TGVerticalFrame(this, 0, 0);

  AddFrame(fLeftLabFrame,fLHLeft);
  AddFrame(fDataCanvas, fLHCanvas);
  AddFrame(fRightLabFrame, fLHLeft);
  AddFrame(fButtonFrame, fLHRight);

  fMemList->Add(fLeftLabFrame);
  fMemList->Add(fDataCanvas);
  fMemList->Add(fRightLabFrame);
  fMemList->Add(fButtonFrame);

  // Label frames

  for (i=0; i<8; i++){ 
    sprintf(buf,"%1d",i);
    fLeftLab[i] = new TGLabel(fLeftLabFrame, new TGString(buf));
    fRightLab[i] = new TGLabel(fRightLabFrame, new TGString(buf));
    fLeftLabFrame->AddFrame(fLeftLab[i], fLHTop);
    fRightLabFrame->AddFrame(fRightLab[i], fLHTop);
    fMemList->Add(fLeftLab[i]);
    fMemList->Add(fRightLab[i]);
  }

  defheight = fLeftLab[0]->GetHeight();

  // Data frame

  for (i=0; i<256; i++) {
    for (j=0; j<8; j++) {
      fDataBit[i][j] = new TGButton(fDataFrame,10000+(i<<3)+j);
      fDataBit[i][j]->Resize(10,defheight);
      fDataBit[i][j]->AllowStayDown(1);
      fDataBit[i][j]->Associate(this);
      fDataFrame->AddFrame(fDataBit[i][j]);
      fMemList->Add(fDataBit[i][j]);
    }
    sprintf(buf,"%1d",i/100);
    fBottomLab[i][0] = new TGLabel(fDataFrame, new TGString(buf));
    sprintf(buf,"%1d",(i-i/100*100)/10);
    fBottomLab[i][1] = new TGLabel(fDataFrame, new TGString(buf));
    sprintf(buf,"%1d",i-i/10*10);
    fBottomLab[i][2] = new TGLabel(fDataFrame, new TGString(buf));
    fDataFrame->AddFrame(fBottomLab[i][0]);
    fDataFrame->AddFrame(fBottomLab[i][1]);
    fDataFrame->AddFrame(fBottomLab[i][2]);
    fMemList->Add(fBottomLab[i][0]);
    fMemList->Add(fBottomLab[i][1]);
    fMemList->Add(fBottomLab[i][2]);
  }

  // Button frame

  fClearButton = new TGTextButton(fButtonFrame," Clear ",13001);
  fRefreshButton = new TGTextButton(fButtonFrame,"Refresh",13002);
  fDoneButton = new TGTextButton(fButtonFrame," Done  ",13003);
  fClearButton->Associate(this);
  fRefreshButton->Associate(this);
  fDoneButton->Associate(this);
  fButtonFrame->AddFrame(fClearButton, fLHTop);
  fButtonFrame->AddFrame(fRefreshButton, fLHTop);
  fButtonFrame->AddFrame(fDoneButton, fLHBottom);
  fMemList->Add(fRefreshButton);
  fMemList->Add(fClearButton);
  fMemList->Add(fDoneButton);

  fDataCanvas->Resize(400,fDataFrame->GetDefaultHeight()+1.5*defheight);
  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  MapWindow();
  SetWindowName("Pattern Editor");
  Refresh();

  fLogic->PatternMode(fSec);
  if (fOptions & kBlock) gClient->WaitFor(this);    
}
//-----------------------------------------------------------------------------
 DGPatEditor::~DGPatEditor() {
  if (fOptions & kRestore) {
    fLogic->WriteConfReg(fConfreg);
    fLogic->WriteRAM(fSec, fRam);
  }

  std::cout <<"destroying DGPatEditor" << std::endl;

  fMemList->Delete();
}
//-----------------------------------------------------------------------------
 void DGPatEditor::Refresh() {
  // Update display such that it represents actual contents of fModule->RAM. 

  Int_t i,j;
  UShort_t ram;

  for (i=0; i<256; i++) {
    ram = fLogic->GetRAM(fSec, i);
    for (j=0; j<8; j++) {
      //      fDataBit[i][j]->SetState((EButtonState) (fData[i]>>j & 1));
      fDataBit[i][j]->SetState((EButtonState) (ram>>j & 1));
    }
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGPatEditor::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t i,j;
  UShort_t *ram;

  switch(GET_MSG(msg)) {  
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      if (parm1 >= 10000 && parm1 < 10000 + 256*8) {
	i=(parm1-10000)>>3;
	j=(parm1-10000) & 7;
	ram = fLogic->GetRAM(fSec) + i;
	if ((*ram)>>j & 1) {
	  setbit(ram, j, 0);
	  fLogic->WriteRAM(fSec, fLogic->GetRAM(fSec));
	  fDataBit[i][j]->SetState(kButtonUp);	
	} else { 
	  setbit(ram, j, 1);
	  fLogic->WriteRAM(fSec, fLogic->GetRAM(fSec));
	  fDataBit[i][j]->SetState(kButtonDown);	
	}
	fLogic->ReadRAM();
	Refresh();
      }
      if (parm1 == 13001) {
	fLogic->WriteRAM(fSec, (UShort_t) 0);
	fLogic->ReadRAM();
	Refresh();
      }
      if (parm1 == 13002) {
	fLogic->ReadRAM();
	Refresh();
      }
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






