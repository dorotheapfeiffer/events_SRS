///////////////////////////////////////////////////////////////////////////////
// DGTrigger is a GUI for DTrigger
///////////////////////////////////////////////////////////////////////////////
//
//

//

#include "DGTrigger.h"
#include "TH1F.h"

  ClassImp(DGTrigger)

//-----------------------------------------------------------------------------
 DGTrigger::DGTrigger(DTrigger *trigger) : TGMainFrame(gClient->GetRoot(), 0, 0) {
  TGLayoutHints *fL0,*fL1,*fL2,*fL3,*fL4,*fL10,*fL11,*fL12;
  TGLayoutHints *fL200,*fL201,*fL210,*fL211;
  TGLayoutHints *fL300,*fL301,*fL310,*fL311;
  ULong_t color;
  Int_t i;  
  TGLabel *xTGLabel;

  std::cout << "constructing DGTrigger...\n";

  fTrigger = trigger;

  gClient->GetColorByName("red", color);
  ChangeBackground(color);
  
  fL0 = new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsCenterX,5,5,5,0);
  fL10 = new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX,5,5,0,5);
  fL11 = new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY | kLHintsCenterX,5,5,5,5);
  fL12 = new TGLayoutHints(kLHintsTop | kLHintsExpandY | kLHintsCenterX,5,5,5,5);
  fL1 = new TGLayoutHints(kLHintsLeft , 5,0,3,3);
  fL2 = new TGLayoutHints(kLHintsRight, 0,0,1,3);
  fL3 = new TGLayoutHints(kLHintsLeft | kLHintsExpandX,0,20,1,1);
  fL4 = new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsCenterX,1,1,1,1);

  fL200 = new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,2,5);
  fL211 = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,5,5,2,5);
  fL201 = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY,5,5,2,5);
  fL210 = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX,5,5,2,5);

  fL300 = new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5);
  fL311 = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsCenterX | kLHintsCenterY,5,5,5,5);
  fL301 = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsCenterY,5,5,5,5);
  fL310 = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsCenterX,5,5,5,5);

  fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  fControlFrame = new TGHorizontalFrame(this, 0, 0, kSunkenFrame);
  fCanvasFrame = new TRootEmbeddedCanvas("CanvasFrame", this, 10, 280, kSunkenFrame);
  AddFrame(fMenuBar, fL0);
  AddFrame(fControlFrame, fL10);
  AddFrame(fCanvasFrame, fL10);

  fLogicFrame = new TGVerticalFrame(fControlFrame, 0, 0, 0);
  fFLTFrame = new TGVerticalFrame(fControlFrame, 0, 0, 0);
  fSLTFrame = new TGVerticalFrame(fControlFrame, 0, 0, 0);
  fBusyFrame = new TGVerticalFrame(fControlFrame, 0, 0, 0);

  fControlFrame->AddFrame(fLogicFrame, fL201);
  fControlFrame->AddFrame(fFLTFrame, fL201);
  fControlFrame->AddFrame(fSLTFrame, fL201);
  fControlFrame->AddFrame(fBusyFrame, fL201);

  // menu

  fMenuFile = new TGPopupMenu(gClient->GetRoot());
  fMenuExpert = new TGPopupMenu(gClient->GetRoot());
  fMenuHelp = new TGPopupMenu(gClient->GetRoot());
  fMenuBar->AddPopup("&File", fMenuFile, fL1);
  fMenuBar->AddPopup("&Expert", fMenuExpert, fL1);
  fMenuBar->AddPopup("&Help", fMenuHelp, fL2);
  fMenuFile->Associate(this);
  fMenuExpert->Associate(this);
  fMenuHelp->Associate(this);

  // file menu 

  fMenuFile->AddEntry("&Load setup", 11);
  fMenuFile->AddEntry("&Save setup", 12);
  fMenuFile->AddEntry("&Exit", 13);

  // expert menu 

  fMenuDebug = new TGPopupMenu(gClient->GetRoot());

  fMenuExpert->AddEntry("Manual Abort",             50);
  fMenuExpert->AddEntry("Manual FLT Reset",         51);
  fMenuExpert->AddEntry("Read logic RAMs",          52);
  fMenuExpert->AddEntry("Initialize setup",         53);
  fMenuExpert->AddPopup("Debug VME module",fMenuDebug);

  fMenuDebug->AddEntry("logic0 (test pattern)",     21);
  fMenuDebug->AddEntry("logic1 (logic)",            22);
  fMenuDebug->AddEntry("downscaler0 (physics)",     23);
  fMenuDebug->AddEntry("downscaler1 (MT)",          24);
  fMenuDebug->AddEntry("downscaler2 (CAL)",         25);
  fMenuDebug->AddEntry("discriminator (SDD)",       26);
  fMenuDebug->AddEntry("downscaler3 (SDD)",         27);
  fMenuDebug->AddEntry("DL633 (FLT inp)",           28);
  fMenuDebug->AddEntry("DL634 (FLT out)",           29);
  fMenuDebug->AddEntry("DL633 (Abort inp)",         30);
  fMenuDebug->AddEntry("DL634 (Abort out)",         31);
  fMenuDebug->AddEntry("DL633 (Busy 1/2)",          32);
  fMenuDebug->AddEntry("DL633 (Busy 2/2)",          33);
  fMenuDebug->AddEntry("DL633 (Busy or)",           34);
  fMenuDebug->AddEntry("DL634 (Busy out)",          35);
  fMenuDebug->AddEntry("DL631 (Busy Pat 1/2)",      36);
  fMenuDebug->AddEntry("DL631 (Busy Pat 2/2)",      37);
  fMenuDebug->AddEntry("scaler0 (logic1 inp/out)",  38);
  fMenuDebug->AddEntry("scaler1 (trigger etc)",     39);
  fMenuDebug->Associate(this);

  // help menu 

  fMenuHelp->AddEntry("logic"  , 61);
  fMenuHelp->AddEntry("FLT"    , 62);
  fMenuHelp->AddEntry("SLT"    , 63);
  fMenuHelp->AddEntry("Busy"   , 64);
  fMenuHelp->AddEntry("monitor", 65);

  // logic frame

  fLogicLab = new TGLabel(fLogicFrame, new TGString("logic"));
  fLogicPhysFrame = new TGGroupFrame(fLogicFrame, "physics");
  fLogicMTCALFrame = new TGGroupFrame(fLogicFrame, "MT/CAL");
  fLogicFrame->AddFrame(fLogicLab, fL210);
  fLogicFrame->AddFrame(fLogicPhysFrame, fL200);
  fLogicFrame->AddFrame(fLogicMTCALFrame, fL211);

  // physics logic frame

  fLogicPhysBbp = new TGCheckButton(fLogicPhysFrame, "beam before-prot.", 20902);
  fLogicPhysIbp = new TGCheckButton(fLogicPhysFrame, "inter. before-prot", 20903);
  fLogicPhysFrame->AddFrame(fLogicPhysBbp, fL1);
  fLogicPhysFrame->AddFrame(fLogicPhysIbp, fL1);
  fLogicPhysBbp->Associate(this);
  fLogicPhysIbp->Associate(this);

  xTGLabel = new TGLabel(fLogicPhysFrame,new TGString(""));
  fLogicPhysFrame->AddFrame(xTGLabel, fL3);

  fLogicPhysEditLogic = new TGTextButton(fLogicPhysFrame, "edit physics logic", 20911);
  fLogicPhysTestLogic = new TGTextButton(fLogicPhysFrame, "test physics logic", 20912);
  fLogicPhysFrame->AddFrame(fLogicPhysEditLogic, fL210);
  fLogicPhysFrame->AddFrame(fLogicPhysTestLogic, fL210);
  fLogicPhysEditLogic->Associate(this);
  fLogicPhysTestLogic->Associate(this);

  // MT/CAL logic frame

  xTGLabel = new TGLabel(fLogicMTCALFrame,new TGString("  inb outb"));
  fLogicMTCALFrame->AddFrame(xTGLabel, fL1);

  for (i=0; i<6; i++) {
    fLogicMTCALFrameSlice[i] = new TGHorizontalFrame(fLogicMTCALFrame,0,0,0);
    fLogicMTCALFrame->AddFrame(fLogicMTCALFrameSlice[i],fL0);
  }

  fLogicMTCALBut[0][0] = new TGCheckButton(fLogicMTCALFrameSlice[0], "", 20200);
  fLogicMTCALBut[0][1] = new TGCheckButton(fLogicMTCALFrameSlice[1], "", 20201);
  fLogicMTCALBut[0][2] = new TGCheckButton(fLogicMTCALFrameSlice[2], "", 20202);
  fLogicMTCALBut[0][3] = new TGCheckButton(fLogicMTCALFrameSlice[3], "", 20203);
  fLogicMTCALBut[0][4] = new TGCheckButton(fLogicMTCALFrameSlice[4], "", 20204);
  fLogicMTCALBut[0][5] = new TGCheckButton(fLogicMTCALFrameSlice[5], "", 20205);

  fLogicMTCALBut[1][0] = new TGCheckButton(fLogicMTCALFrameSlice[0], " MT",      20300);
  fLogicMTCALBut[1][1] = new TGCheckButton(fLogicMTCALFrameSlice[1], "",         20301);
  fLogicMTCALBut[1][2] = new TGCheckButton(fLogicMTCALFrameSlice[2], " SDD inj", 20302);
  fLogicMTCALBut[1][3] = new TGCheckButton(fLogicMTCALFrameSlice[3], " UV1",     20303);
  fLogicMTCALBut[1][4] = new TGCheckButton(fLogicMTCALFrameSlice[4], " UV2",     20304);
  fLogicMTCALBut[1][5] = new TGCheckButton(fLogicMTCALFrameSlice[5], " laser",   20305);

  for (i=0; i<6; i++) {
    fLogicMTCALFrameSlice[i]->AddFrame(fLogicMTCALBut[0][i], fL1);
    fLogicMTCALFrameSlice[i]->AddFrame(fLogicMTCALBut[1][i], fL1);
    fLogicMTCALBut[0][i]->Associate(this);
    fLogicMTCALBut[1][i]->Associate(this);
  }

  // FLT frame

  fFLTLab = new TGLabel(fFLTFrame, new TGString("FLT"));
  fFLTEnableFrame = new TGGroupFrame(fFLTFrame, "enable", kVerticalFrame);
  fFLTFrame->AddFrame(fFLTLab, fL210);
  fFLTFrame->AddFrame(fFLTEnableFrame, fL201);

  fFLTDownLab = new TGLabel(fFLTEnableFrame, new TGString("downsc   "));
  fFLTEnableFrame->AddFrame(fFLTDownLab, fL2);

  // FLT enable frame

  for (i=0; i<NFLT; i++) {
    fFLTEnableFrameSlice[i] = new TGHorizontalFrame(fFLTEnableFrame, 0, 0);
    fFLTEnableFrame->AddFrame(fFLTEnableFrameSlice[i], fL0);
  }

  fFLTEnableBut[0] = new TGCheckButton(fFLTEnableFrameSlice[0], "beam",    20000);
  fFLTEnableBut[1] = new TGCheckButton(fFLTEnableFrameSlice[1], "minb",    20001);
  fFLTEnableBut[2] = new TGCheckButton(fFLTEnableFrameSlice[2], "central", 20002);
  fFLTEnableBut[3] = new TGCheckButton(fFLTEnableFrameSlice[3], "SOB",     20003);
  fFLTEnableBut[4] = new TGCheckButton(fFLTEnableFrameSlice[4], "EOB",     20004);
  fFLTEnableBut[5] = new TGCheckButton(fFLTEnableFrameSlice[5], "MT",      20005);
  fFLTEnableBut[6] = new TGCheckButton(fFLTEnableFrameSlice[6], "SDDinj",  20006);
  fFLTEnableBut[7] = new TGCheckButton(fFLTEnableFrameSlice[7], "UV1",     20007);
  fFLTEnableBut[8] = new TGCheckButton(fFLTEnableFrameSlice[8], "UV2",     20008);
  fFLTEnableBut[9] = new TGCheckButton(fFLTEnableFrameSlice[9], "laser",   20009);

  for (i=0; i<NFLT; i++) {
    fFLTDownB[i] = new TGTextBuffer(80);
    fFLTDownE[i] = new TGTextEntry(fFLTEnableFrameSlice[i], fFLTDownB[i], 20100+i);
    fFLTDownE[i]->Resize(80,fFLTDownE[i]->GetDefaultHeight());
    fFLTEnableFrameSlice[i]->AddFrame(fFLTEnableBut[i], fL0);
    fFLTEnableFrameSlice[i]->AddFrame(fFLTDownE[i], fL0);
    fFLTEnableBut[i]->Associate(this);
    fFLTDownE[i]->Associate(this);
  }

  // SLT frame

  fSLTLab = new TGLabel(fSLTFrame, new TGString("SLT"));
  fSLTRequireFrame = new TGGroupFrame(fSLTFrame, "require", kVerticalFrame);
  fSLTSDDMultFrame = new TGGroupFrame(fSLTFrame, "SDD multiplicity", kVerticalFrame);
  fSLTFrame->AddFrame(fSLTLab, fL210);
  fSLTFrame->AddFrame(fSLTRequireFrame, fL211);
  fSLTFrame->AddFrame(fSLTSDDMultFrame, fL210);

  // SLT require frame

  xTGLabel = new TGLabel(fSLTRequireFrame,new TGString("  "));
  fSLTRequireFrame->AddFrame(xTGLabel, fL0);

  fSLTRequireBut[0] = new TGCheckButton(fSLTRequireFrame, "SDD (for central FLT)",   20400);
  fSLTRequireBut[1] = new TGCheckButton(fSLTRequireFrame, "beam after-protection",   20401);
  fSLTRequireBut[2] = new TGCheckButton(fSLTRequireFrame, "inter. after-protection", 20402);

  for (i=0; i<3; i++) {
    fSLTRequireFrame->AddFrame(fSLTRequireBut[i], fL1);
    fSLTRequireBut[i]->Associate(this);
  }

  // SLT SDD multiplicity frame

  xTGLabel = new TGLabel(fSLTSDDMultFrame,new TGString("        threshold downsc"));
  fSLTSDDMultFrame->AddFrame(xTGLabel, fL10);

  for (i=0; i<4; i++) {
    fSLTSDDMultFrameSlice[i] = new TGHorizontalFrame(fSLTSDDMultFrame, 0, 0);
    fSLTSDDMultFrame->AddFrame(fSLTSDDMultFrameSlice[i], fL0);

    fSLTSDDMultBut[i] = new TGCheckButton(fSLTSDDMultFrameSlice[i], "", 20500+i);
    fSLTSDDMultFrameSlice[i]->AddFrame(fSLTSDDMultBut[i], fL1);
    fSLTSDDMultBut[i]->Associate(this);

    fSDDThrB[i] = new TGTextBuffer(80);
    fSDDThrE[i] = new TGTextEntry(fSLTSDDMultFrameSlice[i], fSDDThrB[i], 20600+i);
    fSDDThrE[i]->Resize(40,fSDDThrE[i]->GetDefaultHeight());
    fSLTSDDMultFrameSlice[i]->AddFrame(fSDDThrE[i], fL1);
    fSDDThrE[i]->Associate(this);

    fSDDDowB[i] = new TGTextBuffer(80);
    fSDDDowE[i] = new TGTextEntry(fSLTSDDMultFrameSlice[i], fSDDDowB[i], 20700+i);
    fSDDDowE[i]->Resize(50,fSDDDowE[i]->GetDefaultHeight());
    fSLTSDDMultFrameSlice[i]->AddFrame(fSDDDowE[i], fL1);
    fSDDDowE[i]->Associate(this);
  }

  // Busy frame

  fBusyLab = new TGLabel(fBusyFrame, new TGString("Busy"));
  fBusyWaitFrame = new TGGroupFrame(fBusyFrame, "wait for", kVerticalFrame);
  fBusyFrame->AddFrame(fBusyLab, fL210);
  fBusyFrame->AddFrame(fBusyWaitFrame, fL201);

  xTGLabel = new TGLabel(fBusyWaitFrame,new TGString("  "));
  fBusyWaitFrame->AddFrame(xTGLabel, fL0);

  // Busy Wait frame

  fBusyWaitBut[0] = new TGCheckButton(fBusyWaitFrame, "DACOX",  20800);
  fBusyWaitBut[1] = new TGCheckButton(fBusyWaitFrame, "SDD",    20801);
  fBusyWaitBut[2] = new TGCheckButton(fBusyWaitFrame, "RICH 1", 20802);
  fBusyWaitBut[3] = new TGCheckButton(fBusyWaitFrame, "RICH 2", 20803);
  fBusyWaitBut[4] = new TGCheckButton(fBusyWaitFrame, "TPC",    20804);
  fBusyWaitBut[5] = new TGCheckButton(fBusyWaitFrame, "DAQ",    20805);
  fBusyWaitBut[6] = new TGCheckButton(fBusyWaitFrame, "Always on",  20806);

  for (i=0; i<NBUSY; i++) {
    fBusyWaitFrame->AddFrame(fBusyWaitBut[i], fL1);
    fBusyWaitBut[i]->Associate(this);
  }

  // Canvas frame
  TCanvas *c = fCanvasFrame->GetCanvas();
  
  c->Range(0,0, 100, 100);
  c->SetFillColor(171);
  //c->SetEditable(kFALSE);
  //c->cd();
  //TH1F h("h","h",100,0,100);
  //h.Fill(50);
  //h.Draw();
  //c->Modified();
  //c->Update();

  fMonitor = new DGMonitor(fTrigger, c);

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  SetWindowName("CERES trigger control");
  MapWindow();

  fTick = 0;
  fTimer = new TTimer(this,500);
  fTimer->TurnOn();

  fDatime = new TDatime();

  // logfile

  fLog = new std::ofstream("trilog.txt", std::ios::out|std::ios::ate);
  *fLog << fDatime->AsString() << " " << "new DGTrigger" << std::endl;

  // scalers data base

  OpenScalerDatabase((char*)"scalers.dat");

  std::cout << "done!" << std::endl;
}

//-----------------------------------------------------------------------------
 DGTrigger::~DGTrigger() {
  std::cout <<"destroying DGTrigger" << std::endl;
  fTimer->TurnOff();
  delete fTimer;
  delete fLog;
  delete fDatime;
  std::cout<<"I do not care about deleting all the stuff..."<<std::endl;
}
//-----------------------------------------------------------------------------
 void DGTrigger::Refresh() {
  // Update display so it represents actual contents of fTrigger.
  Char_t  buf[80];
  Int_t  i;


  // Phys logic

  if (fTrigger->IsEnabledBBP()) fLogicPhysBbp->SetState(kButtonDown);
  else                          fLogicPhysBbp->SetState(kButtonUp);
  if (fTrigger->IsEnabledIBP()) fLogicPhysIbp->SetState(kButtonDown);
  else                          fLogicPhysIbp->SetState(kButtonUp);

  // MT/CAL logic

  for (i=0; i<6; i++) {        
    if (fTrigger->IsOnMTCAL(i+2,1)) fLogicMTCALBut[0][i]->SetState(kButtonDown);
    else                            fLogicMTCALBut[0][i]->SetState(kButtonUp);
    if (fTrigger->IsOnMTCAL(i+2,0)) fLogicMTCALBut[1][i]->SetState(kButtonDown);
    else                            fLogicMTCALBut[1][i]->SetState(kButtonUp);
  }

  // FLT

  for (i=0; i<NFLT; i++) {    
    if (fTrigger->IsEnabledFLT(i)) fFLTEnableBut[i]->SetState(kButtonDown);
    else fFLTEnableBut[i]->SetState(kButtonUp);
    sprintf(buf, "%d", fTrigger->GetFLTDownscal(i));
    fFLTDownB[i]->Clear();
    fFLTDownB[i]->AddText(0,buf);
    fClient->NeedRedraw(fFLTDownE[i]);
  }

  // SLT

  for (i=0; i<NABORT; i++) {
    if (fTrigger->IsEnabledAbort(i)) fSLTRequireBut[i]->SetState(kButtonDown);
    else fSLTRequireBut[i]->SetState(kButtonUp);
  }

  for (i=0; i<4; i++) {
    if (fTrigger->IsEnabledSDDMult(i)) fSLTSDDMultBut[i]->SetState(kButtonDown);
    else fSLTSDDMultBut[i]->SetState(kButtonUp);

    sprintf(buf, "%d", fTrigger->GetSDDMultThr(i));
    fSDDThrB[i]->Clear();
    fSDDThrB[i]->AddText(0,buf);
    fClient->NeedRedraw(fSDDThrE[i]);

    sprintf(buf, "%d", fTrigger->GetSDDMultDownscal(i));
    fSDDDowB[i]->Clear();
    fSDDDowB[i]->AddText(0,buf);
    fClient->NeedRedraw(fSDDDowE[i]);
  }

  // Busy

  for (i=0; i<NBUSY; i++) {
    if (fTrigger->IsEnabledBusy(i)) fBusyWaitBut[i]->SetState(kButtonDown);
    else fBusyWaitBut[i]->SetState(kButtonUp);
  }
}
//-----------------------------------------------------------------------------
 Bool_t DGTrigger::HandleTimer(TTimer *tim) {
  static Int_t noburst_old;
  Int_t noburst;

  //fTrigger->ReadVME();
  fMonitor->Refresh();
  if (fTick/5*5 == fTick) Refresh();

  noburst = fTrigger->fLogic1->GetInputReg() >> 9 & 1;


  // end-of-burst  (more exactly, start-of-noburst) 

  if ((noburst == 1) && (noburst_old == 0)) {

    // save scaler counts in scalers database
    //    std::cout << "fillingn";
    FillScalerDatabase();
  }

  // start-of-burst (more exactly, end-of-noburst) 

  if ((noburst == 0) && (noburst_old == 1)) {

    // clear downscaler counters before sob

    fTrigger->fDown[0]->ClearCounters();
    fTrigger->fDown[1]->ClearCounters();
    fTrigger->fDown[2]->ClearCounters();
    fTrigger->fDown2->ClearCounters();
  }

  noburst_old = noburst;

  fTimer->Reset();
  fTick++;
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGTrigger::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  Int_t i, j;
  Int_t inb, outb;
  int   temp;
  TGFileInfo fi;
  const char *filetypes[] = { "Trigger setup files", "*.tri", 
			      "All files", "*",
			      0,0,
			      0,0};
  fi.fFileTypes = (const char **)filetypes;
  fDatime->Set();
  *fLog << fDatime->AsString() << "........";

  switch(GET_MSG(msg)) {

  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_MENU:
      switch (parm1) {
      case 11:
	// load setup
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen,&fi);
	if (fi.fFilename) fTrigger->Load(fi.fFilename);
	*fLog << "load setup from " << fi.fFilename;
	break;
      case 12:
	// save setup
	new TGFileDialog(fClient->GetRoot(), this, kFDSave,&fi);
	if (fi.fFilename) fTrigger->Save(fi.fFilename);
	*fLog << "save setup on   " << fi.fFilename;
	break;
      case 13:
	// exit
	*fLog << "exitn";
	exit(0);
	break;
      case 21:
	new DGLogic(fTrigger->fLogic0);
	*fLog << "debug logic0n";
	break;
      case 22:
	new DGLogic(fTrigger->fLogic1);
	*fLog << "debug logic1n";
	break;
      case 23:
	new DGDownscaler(fTrigger->fDown[0]);
	*fLog << "debug down0n";
	break;
      case 24:
	new DGDownscaler(fTrigger->fDown[1]);
	*fLog << "debug down1n";
	break;
      case 25:
	new DGDownscaler(fTrigger->fDown[2]);
	*fLog << "debug down2n";
	break;
      case 26:
	new DGDisc(fTrigger->fDisc);
	*fLog << "debug discn";
	break;
      case 27:
	new DGDownscaler(fTrigger->fDown2);
	*fLog << "debug SDD downn";
	break;
      case 28:
	new DGDL633(fTrigger->fFltinp);
	*fLog << "debug fltinpn";
	break;
      case 29:
	new DGDL634(fTrigger->fFltout);
	*fLog << "debug fltoutn";
	break;
      case 30:
	new DGDL633(fTrigger->fAboinp);
	*fLog << "debug aboinpn";
	break;
      case 31:
	new DGDL634(fTrigger->fAboout);
	*fLog << "debug abooutn";
	break;
      case 32:
	new DGDL633(fTrigger->fBusy[0]);
	*fLog << "debug busy0n";
	break;
      case 33:
	new DGDL633(fTrigger->fBusy[1]);
	*fLog << "debug busy1n";
	break;
      case 34:
	new DGDL633(fTrigger->fBusyor);
	*fLog << "debug busyorn";
	break;
      case 35:
	new DGDL634(fTrigger->fBusyout);
	*fLog << "debug busyoutn";
	break;
      case 36:
	new DGDL631(fTrigger->fBusypat[0]);
	*fLog << "debug busypat0n";
	break;
      case 37:
	new DGDL631(fTrigger->fBusypat[2]);
	*fLog << "debug busypat1n";
	break;
      case 38:
	new DGScaler(fTrigger->fScaler0);
	*fLog << "debug scaler0n";
	break;
      case 39:
	new DGScaler(fTrigger->fScaler1);
	*fLog << "debug scaler1n";
	break;
      case 50:
	fTrigger->ManualAbort();
	*fLog << "manual abort";
	break;
      case 51:
	fTrigger->ManualFLTReset();
	*fLog << "manual reset";
	break;
      case 52:
	fTrigger->fLogic0->ReadRAM();
	fTrigger->fLogic1->ReadRAM();
	fTick=0;
	fTimer->Reset();
	*fLog << "read ram";
	break;
      case 53:
	fTrigger->ResetModules();
	fTrigger->Initialize();
	*fLog << "initialize";
	break;
      case 54:
	break;
      case 61:
	new DGHelp((char*)"./help.hlp");
	*fLog << "logic help";
	break;
      case 62:
	new DGHelp((char*)"./help.hlp");
	*fLog << "flt help";
	break;
      case 63:
	new DGHelp((char*)"./help.hlp");
	*fLog << "slt help";
	break;
      case 64:
	new DGHelp((char*)"./help.hlp");
	*fLog << "busy help";
	break;
      case 65:
	new DGHelp((char*)"./help.hlp");
	*fLog << "monitor help";
	break;
      default:
	break;
      }
    case kCM_CHECKBUTTON:
      if (parm1 == 20902) {
	fTrigger->fLogic0->ReadRAM();
	fTrigger->fLogic1->ReadRAM();
	temp = (int) fLogicPhysBbp->GetState();
	if (temp) fTrigger->EnableBBP();
	else      fTrigger->DisableBBP();
	*fLog << "beam before-protection " << temp;
      }
      if (parm1 == 20903) {
	fTrigger->fLogic0->ReadRAM();
	fTrigger->fLogic1->ReadRAM();
	temp = (int) fLogicPhysIbp->GetState();
	if (temp) fTrigger->EnableIBP();
	else      fTrigger->DisableIBP();
	*fLog << "interaction before-protection " << temp;
      }
      if (parm1 >= 20200 && parm1 < 20400) {
	fTrigger->fLogic0->ReadRAM();
	fTrigger->fLogic1->ReadRAM();
	i = parm1 - parm1/100*100;
	inb = fLogicMTCALBut[0][i]->GetState(); 
	outb = fLogicMTCALBut[1][i]->GetState(); 
	fTrigger->SwitchMTCAL(i+2,inb,outb);
	*fLog << "MTCAL " << i << " " << inb << " " << outb;
      }
      if (parm1 >= 20000 && parm1 < 20100) {
	i = parm1 - parm1/100*100;
	temp = (int) fFLTEnableBut[i]->GetState();
	if (temp) fTrigger->EnableFLT(i);
	else      fTrigger->DisableFLT(i);
	*fLog << "FLT " << i << " " << temp;
      }
      if (parm1 >= 20400 && parm1 < 20500) {
	i = parm1 - parm1/100*100;
	temp = (int) fSLTRequireBut[i]->GetState();
	if (temp) fTrigger->EnableAbort(i);
	else      fTrigger->DisableAbort(i);
	*fLog << "SLT " << i << " " << temp;
      }
      if (parm1 >= 20500 && parm1 < 20600) {
	i = parm1 - parm1/100*100;
	temp = (int) fSLTSDDMultBut[i]->GetState();
	if (temp) fTrigger->EnableSDDMult(i);
	else      fTrigger->DisableSDDMult(i);
	*fLog << "SDD mult " << i << " " << temp;
      }
      if (parm1 >= 20800 && parm1 < 20900) {
	i = parm1 - parm1/100*100;
	temp = (int) fBusyWaitBut[i]->GetState();
	if (i == 6) {           // always on
	  fBusyWaitBut[i]->SetState(kButtonDown);
	  new TGMsgBox(gClient->GetRoot(), 0, "dupa", 
		       "Man, do you want to crash the system? ", 
		       kMBIconStop, kMBYes | kMBNo, &j);
	  //	  if (j == kMBYes) std::cout << "dupan";
	} else {
	  if (temp) fTrigger->EnableBusy(i);
	  else      fTrigger->DisableBusy(i);
	}
	*fLog << "busy " << i << " " << temp;
      }
      break;
    case kCM_BUTTON:
      if (parm1 == 20911) {
	new DGLogEditor(this, fTrigger->fLogic1, 0);
	*fLog << "physics log editor ";
      }
      if (parm1 == 20912) {
	fTrigger->ManualFLTReset();
	new DGPatEditor(this, fTrigger->fLogic0, 0, kRestore);
	*fLog << "physics pat editor ";
      }
      break;
    default:
      break;
    }
    fTick = 0;       // refresh
    break;
  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_ENTER:
      if (parm1 >= 20100 && parm1 < 20200) {               // FLT downscaling
	i = parm1 - 20100;
	j = atoi(fFLTDownB[i]->GetString());
	fTrigger->WriteFLTDownscal(i, j);
	*fLog << "FLT " << i << " downscaling " << j;
      }
      if (parm1 >= 20600 && parm1 < 20700) {               // SLT threshold
	i = parm1 - 20600;
	j = atoi(fSDDThrB[i]->GetString());
	fTrigger->WriteSDDMultThr(i, j);
	*fLog << "SDD mult " << i << " threshold " << j;
      }
      if (parm1 >= 20700 && parm1 < 20800) {               // SLT downscaling
	i = parm1 - 20700;
	j = atoi(fSDDDowB[i]->GetString());
	fTrigger->WriteSDDMultDownscal(i, j);
	*fLog << "SDD mult " << i << " downscaling " << j;
      }
      fTick = 0;        // refresh
      fTimer->Reset();
      break;
    case kTE_TEXTCHANGED:

      // postpone refreshing as much as possible, otherwise it could 
      // happen that the field is overwritten by the automatic 
      // refreshment routine while you are editing; not a nice thing

      fTick = 1;        
      fTimer->Reset();
      *fLog << "editing";
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }

  *fLog << std::endl;
  return kTRUE;
}
//-----------------------------------------------------------------------------
/*
void DGTrigger::OpenScalerDatabase_obsolete(Char_t *scalfilename) {
  // open scaler database in update mode and get the tree

  // open file

  TFile *fScalFile = new TFile(scalfilename, "UPDATE","scalers database");
  if (!fScalFile->IsOpen()) {
    std::cout << "DGTrigger: " << scalfilename;
    std::cout << " does not exist. Creating one.n";
    fScalFile = new TFile(scalfilename,"NEW","scalers database");
  }
  
  // get on tree

  fScalTree = (TTree*) fScalFile->Get("scalers_tree");
  std::cout << "fscaltree = " << fScalTree << std::endl;
  if (fScalTree) {
    fScalTree->SetBranchAddress("time", fDatime);
    fScalTree->SetBranchAddress("scaler0", fTrigger->fScaler0->Data());
    fScalTree->SetBranchAddress("scaler1", fTrigger->fScaler1->Data());
  } else {
    std::cout << "scalers_tree does not exist. Creating one.n";
    fScalTree = new TTree("scalers_tree", "scalers_tree", 0);
    fScalTree->Branch("time", fDatime, "time/i", 256); 
    fScalTree->Branch("scaler0", fTrigger->fScaler0->Data(), 
	          "c0/i:c1:c2:c3:c4:c5:c6:c7:c8:c9:ca:cb:cc:cd:ce:cf", 256);
    fScalTree->Branch("scaler1", fTrigger->fScaler1->Data(), 
	          "c0/i:c1:c2:c3:c4:c5:c6:c7:c8:c9:ca:cb:cc:cd:ce:cf", 256);
  }
}
//-----------------------------------------------------------------------------
void DGTrigger::CloseScalerDatabase_obsolete() {
  fScalFile->Close();
  delete fScalFile;
}
//-----------------------------------------------------------------------------
void DGTrigger::FillScalerDatabase_obsolete() {
  // write actual scaler counts on scaler database

  fDatime->Set();
  fScalTree->Fill();
  fScalFile->Purge();
  fScalFile->Write();
}
*/
//-----------------------------------------------------------------------------
 void DGTrigger::OpenScalerDatabase(Char_t *scalfilename) {
  // Open scaler database file. 
  // Sorry guys but for safety reasons I preferred a simple ASCII file
  // than a ROOT file and a ROOT tree. After I managed to corrupt ROOT
  // file by crashing the writing program, I got scared and went back
  // to simple ASCII which I always can edit if something goes wrong. 
  
  fScalFile = new std::ofstream(scalfilename, std::ios::out|std::ios::app);
  *fScalFile << std::hex;
  if (!fScalFile->good()) {
    std::cout << "cannot open " << scalfilename << std::endl;
    return;
  }  
}
//-----------------------------------------------------------------------------
 void DGTrigger::CloseScalerDatabase() {
}
//-----------------------------------------------------------------------------
 void DGTrigger::FillScalerDatabase() {
  // write actual scaler counts on scaler database

  Int_t i;

  fDatime->Set();
  *fScalFile << fDatime->Get();
  for (i=0; i<16; i++) *fScalFile << " " << fTrigger->fScaler0->GetCount(i);
  for (i=0; i<16; i++) *fScalFile << " " << fTrigger->fScaler1->GetCount(i);
  *fScalFile << std::endl;
}
//-----------------------------------------------------------------------------

