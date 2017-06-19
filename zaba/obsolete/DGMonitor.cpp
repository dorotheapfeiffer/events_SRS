#include "DGMonitor.h"
#include "TH1F.h"

  ClassImp(DGMonitor)

//-----------------------------------------------------------------------------
DGCable::DGCable(Int_t x0, Int_t x1, Int_t y, Char_t *label) {
  // Line with static label and changing counts. Line can be
  // excited (red and thick) or relaxed (black and thin). 
  fLine_x0 = x0;
  fLine_x1 = x1;
  fLine_y = y;
  fLabel_x = x0 + 10;
  fLabel_y = y + 10;
  fCount_x = x0 + 100;
  fCount_y = y + 10;

  fLine = new TGraph(2);
  fLine->SetFillColor(19);
  fLine->SetPoint(0, fLine_x0, fLine_y);
  fLine->SetPoint(1, fLine_x1, fLine_y);
  fLine->Draw("L");

  fLabel = new TText(fLabel_x, fLabel_y, label);
  fLabel->SetTextFont(42);
  fLabel->SetTextSize(TEXT_SIZE);
  fLabel->SetTextAlign(11);
  fLabel->Draw();

  fCount = new TText(fCount_x, fCount_y, "");
  fCount->SetTextFont(12);
  fCount->SetTextSize(TEXT_SIZE);
  fCount->SetTextAlign(31);
  fCount->Draw();

}
DGCable::~DGCable() {
  std::cout << "DGCable destroyedn" <<std::endl;
  delete fLine;
  delete fLabel;
  delete fCount;
}
void DGCable::SetCount(Int_t n) {
  Char_t buf[80];
  sprintf(buf, "%12d", n);
  fCount->SetText(fCount_x, fCount_y, buf);
}
void DGCable::SetLabel(const char * label) {
  fLabel->SetText(fLabel_x, fLabel_y, label);
}
void DGCable::Excite() {
  fLine->SetLineColor(RED);
  fLine->SetLineWidth(THICK);
}
void DGCable::Relax() {
  fLine->SetLineColor(1);
  fLine->SetLineWidth(1);
}
//-----------------------------------------------------------------------------
DGSwitch::DGSwitch(Int_t x, Int_t y, Int_t opt ) {
  // Represents a switch by a short line which either disappears (opt = 0)
  // or rises its right end (opt = 1) when the switch is off. 

  fX0 = x;
  fY0 = y;
  fX1 = x + 20;
  fY1 = y;

  switch (opt) {
  case 1:
    fX2 = fX0 + 14; 
    fY2 = fY0 + 30;
    break;
  default:
    fX2 = fX0; 
    fY2 = fY0;
    break;
  }

  fLine = new TGraph(2);
  fLine->SetFillColor(19);
  fLine->SetPoint(0, fX0, fY0);
  fLine->SetPoint(1, fX2, fY2);
  fLine->Draw("L");
}
DGSwitch::~DGSwitch() {
  delete fLine;
}
void DGSwitch::SetState(Int_t i) {
  if (i) fLine->SetPoint(1, fX1, fY1);
  else   fLine->SetPoint(1, fX2, fY2);
}
//-----------------------------------------------------------------------------
DGBox::DGBox(Int_t x0, Int_t x1, Int_t y, Int_t height, char * label, Int_t color) {
  // Box with label. 

  fBox = new TPaveText(x0, y-height/2, x1, y+height/2,"br");
  fBox->SetBorderSize(1);
  fBox->SetFillColor(color);
  fBox->SetLineColor(1);
  //  fBox->SetTextFont(52);
  fBox->SetTextFont(42);       // needs to be fixed
  fBox->SetTextSize(TEXT_SIZE);
  fBox->SetTextAlign(22);
  fBox->AddText(label);
  fBox->Draw();
}
DGBox::~DGBox() {
  delete fBox;
}
//-----------------------------------------------------------------------------
 DGMonitor::DGMonitor(DTrigger *trigger, TCanvas *canvas) {
  // Represents simplified CERES trigger scheme. 
  std::cout << "Constructing DGMonitor" << std::endl; //ja

  Int_t i;

  fTrigger = trigger;
  fCanvas = canvas;

  // logic inputs

  for (i=0; i<6; i++) fPhysLogInpCable[i] = new DGCable(20, 120, 900-60*i,(char*)"");
  for (i=0; i<6; i++) fPhysLogInpSwitch[i] = new DGSwitch(  120, 900-60*i, 0);
  for (i=0; i<8; i++) fMTCALLogInpCable[i] = new DGCable(20, 140, 450-60*i,(char*)"");

  // logic boxes
  
  new DGBox(140, 190, 750, 380, (char*)" physics", BOXCOLOR);
  new DGBox(140, 190, 240, 480, (char*)"MT/CAL", BOXCOLOR);
  
  // logic outputs
  
  for (i=0; i<3; i++) fPhysLogOutCable[i] =  new DGCable(190, 310, 900-60*i,(char*)"");
  for (i=0; i<8; i++) fMTCALLogOutCable[i] = new DGCable(190, 310, 450-60*i,(char*)"");
  
  // FLT box
  
  new DGBox(300, 430, 500, 980, (char*)"FLTn\nn\n", BOXCOLOR);
  
  // FLT intermediate
  
  for (i=0; i<3; i++) fPhysFLTInpSwitch[i] =  new DGSwitch(        300, 900-60*i, 1);
  for (i=0; i<3; i++) fPhysFLTOutCable[i] =   new DGCable(320, 420, 900-60*i,(char*)"");
  for (i=0; i<8; i++) fMTCALFLTInpSwitch[i] = new DGSwitch(        300, 450-60*i, 1);
  for (i=0; i<8; i++) fMTCALFLTOutCable[i] =  new DGCable(320, 420, 450-60*i,(char*)"");
  
  // FLT output, SLT
  
  fFLTOutCable = new DGCable(430, 850, 600, (char*)"FLT");
  fSLTCable    = new DGCable(850, 950, 600, (char*)"SLT");
  
  // Abort
  
  for (i=0; i<3; i++) fAbortInpCable[i] =   new DGCable(500, 650, 900-60*i,(char*)"");
  for (i=0; i<3; i++) fAbortInpSwitch[i] = new DGSwitch(     650, 900-60*i, 0);
  new DGBox(670, 720, 840, 200, (char*)"Abort", BOXCOLOR);
  fAbortOutCable = new DGCable(720, 850, 840, (char*)"Abort");
  fAbortArrow = new TArrow(850, 840, 850, 610, 0.03, (char*)"|>");
  fAbortArrow->Draw();
  
  // Busy
  
  for (i=0; i<7; i++) fBusyInpCable[i] = new DGCable(550, 700, 480-60*i,(char*)"");
  for (i=0; i<7; i++) fBusyInpSwitch[i] = new DGSwitch(   700, 480-60*i, 0);
  new DGBox(720, 780, 300, 400, (char*)"Busy", BOXCOLOR);
  fBusyOutCable = new DGCable(780, 846, 300,(char*)"Busy");
  
  fBusyInvert = new TEllipse(848, 300, 3, 10);
  fBusyInvert->Draw();
  fResetCable = new DGCable(850, 950, 300,(char*)"Reset");
  
  // labels
  
  fPhysLogInpCable[0]->SetLabel("BC1");
  fPhysLogInpCable[1]->SetLabel("BC2");
  fPhysLogInpCable[2]->SetLabel("BC3");
  fPhysLogInpCable[3]->SetLabel("MD");
  fPhysLogInpCable[4]->SetLabel("MD low");
  fPhysLogInpCable[5]->SetLabel("");
  
  fMTCALLogInpCable[0]->SetLabel("burst");
  fMTCALLogInpCable[1]->SetLabel("noburst");
  fMTCALLogInpCable[2]->SetLabel("pulser");
  fMTCALLogInpCable[3]->SetLabel("");
  fMTCALLogInpCable[4]->SetLabel("SDD inj");
  fMTCALLogInpCable[5]->SetLabel("UV1");
  fMTCALLogInpCable[6]->SetLabel("UV2");
  fMTCALLogInpCable[7]->SetLabel("laser");
  
  fPhysLogOutCable[0]->SetLabel("beam");
  fPhysLogOutCable[1]->SetLabel("minb");
  fPhysLogOutCable[2]->SetLabel("central");
  
  fMTCALLogOutCable[0]->SetLabel("SOB");
  fMTCALLogOutCable[1]->SetLabel("EOB");
  fMTCALLogOutCable[2]->SetLabel("MT");
  fMTCALLogOutCable[3]->SetLabel("");
  fMTCALLogOutCable[4]->SetLabel("SDD inj");
  fMTCALLogOutCable[5]->SetLabel("UV1");
  fMTCALLogOutCable[6]->SetLabel("UV2");
  fMTCALLogOutCable[7]->SetLabel("laser");
  
  fAbortInpCable[0]->SetLabel("SDD");
  fAbortInpCable[1]->SetLabel("BAP");
  fAbortInpCable[2]->SetLabel("IAP");
  
  fBusyInpCable[0]->SetLabel("DACOX");
  fBusyInpCable[1]->SetLabel("SDD");
  fBusyInpCable[2]->SetLabel("RICH 1");
  fBusyInpCable[3]->SetLabel("RICH 2");
  fBusyInpCable[4]->SetLabel("TPC");
  fBusyInpCable[5]->SetLabel("DAQ");
  fBusyInpCable[6]->SetLabel("Always on");

  fCanvas->Modified();
  fCanvas->Update();
  fCanvas->cd();
}
//-----------------------------------------------------------------------------
 DGMonitor::~DGMonitor() {
}
//-----------------------------------------------------------------------------
 void DGMonitor::Refresh() {
  Int_t i;

  // logic 

  if (fTrigger->fLogic0->GetInputReg() >> 8 & 1) fMTCALLogInpCable[0]->Excite();
  else  fMTCALLogInpCable[0]->Relax();
  if (fTrigger->fLogic0->GetInputReg() >> 9 & 1) fMTCALLogInpCable[1]->Excite();
  else  fMTCALLogInpCable[1]->Relax();

  if (fTrigger->fLogic0->IsInLogicMode(0)) {
    for (i=0; i<6; i++) fPhysLogInpSwitch[i]->SetState(1);
  } else {
    for (i=0; i<6; i++) fPhysLogInpSwitch[i]->SetState(0);
  }
  for (i=0; i<6; i++) fPhysLogInpCable[i]->SetCount(fTrigger->fScaler0->GetCount(i));
  for (i=0; i<3; i++) fPhysLogOutCable[i]->SetCount(fTrigger->fScaler0->GetCount(i+8));
  
  

  // FLT

  for (i=0; i<3; i++) fPhysFLTInpSwitch[i]->SetState(fTrigger->IsEnabledFLT(i));
  for (i=0; i<3; i++) fMTCALFLTInpSwitch[i]->SetState(fTrigger->IsEnabledFLT(i+3));
  for (i=4; i<8; i++) fMTCALFLTInpSwitch[i]->SetState(fTrigger->IsEnabledFLT(i+2));

  for (i=0; i<3; i++) fPhysFLTOutCable[i]->SetCount(fTrigger->GetFLTCount(i));
  for (i=0; i<3; i++) fMTCALFLTOutCable[i]->SetCount(fTrigger->GetFLTCount(i+3));
  for (i=4; i<8; i++) fMTCALFLTOutCable[i]->SetCount(fTrigger->GetFLTCount(i+2));

  fFLTOutCable->SetCount(fTrigger->fScaler1->GetCount(0));
  fSLTCable->SetCount(fTrigger->fScaler1->GetCount(6));

  // Abort

  for (i=0; i<3; i++) fAbortInpCable[i]->SetCount(fTrigger->fScaler1->GetCount(2+i));
  for (i=0; i<3; i++) fAbortInpSwitch[i]->SetState(fTrigger->IsEnabledAbort(i));

  fAbortOutCable->SetCount(fTrigger->fScaler1->GetCount(5));
  fResetCable->SetCount(fTrigger->fScaler1->GetCount(7));

  // Busies

  for (i=0; i<7; i++) fBusyInpSwitch[i]->SetState(fTrigger->IsEnabledBusy(i));

  for (i=0; i<7; i++) {
    if (fTrigger->IsBusy(i)) fBusyInpCable[i]->Excite();
    else fBusyInpCable[i]->Relax();
  }

  fCanvas->Update();
}
//-----------------------------------------------------------------------------

