#include "DTrigger.h"

ClassImp(DTrigger)

#include "TCanvas.h"
#include "TPaveText.h"

//*****************************************************************************
 DTrigger::DTrigger() : TObject() {
  // Create objects corresponding to VME modules, and store the pointers 
  // to these objects on the object array *fModuleList.

  std::cout<<"constructing DTrigger\n";
  
  fModuleList = new TObjArray();

  fModuleList->Add(fLogic0  = new DLogic((char*)"MultiGrid",   0xd0000000));

  //fModuleList->Add(fLogic0  = new DLogic((char*)"pattern generator",   0xd0000000));
  //fModuleList->Add(fLogic1  = new DLogic((char*)"lookup table",        0xd0010000));
  //fModuleList->Add(fDown[0] = new DDownscaler((char*)"physics",        0xd0020000));
  //fModuleList->Add(fDown[1] = new DDownscaler((char*)"mt",             0xd0030000));
  //fModuleList->Add(fDown[2] = new DDownscaler((char*)"cal",            0xd0080000));
  //fModuleList->Add(fDisc    = new DDisc((char*)"SLT SDD",              0xd00b0000));
  //fModuleList->Add(fDown2   = new DDownscaler((char*)"SLT SDD",        0xd00c0000));
  //fModuleList->Add(fFltinp  = new DDL633((char*)"FLT input logic",     0xcf000060));
  //fModuleList->Add(fFltout  = new DDL634((char*)"FLT fanout",          0xcf000040));
  //fModuleList->Add(fAboinp  = new DDL633((char*)"Abort input logic",   0xcf000020));
  //fModuleList->Add(fAboout  = new DDL634((char*)"Abort fanout",        0xcf000000));
  //fModuleList->Add(fBusy[0] = new DDL633((char*)"Busy collection 1/2", 0xcf001060));
  //fModuleList->Add(fBusy[1] = new DDL633((char*)"Busy collection 2/2", 0xcf001040));
  //fModuleList->Add(fBusyor  = new DDL633((char*)"Busy 1 or 2",         0xcf001020));
  //fModuleList->Add(fBusyout = new DDL634((char*)"Busy fanout",         0xcf001000));
  //fModuleList->Add(fBusypat[0] = new DDL631((char*)"Busy pattern 1/2", 0xcf002060));
  //fModuleList->Add(fBusypat[1] = new DDL631((char*)"Busy pattern 2/2", 0xcf002040));
  //fModuleList->Add(fScaler0 = new DScaler((char*)"ECL scaler",         0xd0090000));
  //fModuleList->Add(fScaler1 = new DScaler((char*)"NIM scaler",         0xd00a0000));

  printf("%d modulesn",fModuleList->GetLast()+1);
}
//-----------------------------------------------------------------------------
   DTrigger::~DTrigger() {
    std::cout<<"destroying DTrigger\n";
    delete fModuleList;
}
//-----------------------------------------------------------------------------
 void DTrigger::ResetModules() {
  // Call ResetModule for every VME module of fModuleList

  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ResetModule();
}
//-----------------------------------------------------------------------------
 void DTrigger::Initialize() {
  // Setup these parameters of VME modules which are fixed, i.e. not
  // supposed to be changed by the end-user, and thus are not accesible
  // via GUI.
  Int_t i;

  // pattern generator: transparent mode

  fLogic0->ExtClock();
  fLogic0->LogicMode(0);
  fLogic0->LogicMode(1);
  fLogic0->WriteRAM(0,(char*)"i");
  fLogic0->WriteRAM(1,(char*)"i");

// lookup table: standard setup

  fLogic1->LogicMode(0);
  fLogic1->LogicMode(1);

  fLogic1->DefineLogic(0, 0, (char*)"[0] & [1]");
  fLogic1->DefineLogic(0, 3, (char*)"[0] & [1]");
  fLogic1->DefineLogic(0, 1, (char*)"[0] & [1] & [2]");
  fLogic1->DefineLogic(0, 4, (char*)"[0] & [1] & [2]");
  fLogic1->DefineLogic(0, 2, (char*)"[0] & [1] & [2] & [3]");
  fLogic1->DefineLogic(0, 5, (char*)"[0] & [1] & [2] & [3]");
  fLogic1->DefineLogic(0, 6, (char*)"[0]");
  fLogic1->DefineLogic(0, 7, (char*)"[0] & [4]");

  fLogic1->WriteRAM(1, (UShort_t) 0);
  fLogic1->DefineLogic(1, 0, (char*)"![1]");
  fLogic1->DefineLogic(1, 1, (char*)"[1]");

// FLT OR

  fFltinp->Enable(0);
  fFltinp->Enable(1);
  fFltinp->Enable(2);
  fFltinp->Enable(3);

  fFltinp->OrLogic(0);
  fFltinp->OrLogic(1);
  fFltinp->OrLogic(2);
  fFltinp->OrLogic(3);

  fFltout->Inv(0);
  fFltout->Inv(1);
  fFltout->Inv(2);
  fFltout->Norm(3);

// Abort OR

  fAboinp->Disable(3);

  fAboinp->OrLogic(0);
  fAboinp->OrLogic(1);
  fAboinp->OrLogic(2);
  fAboinp->OrLogic(3);

// busy collection

  for (i=0; i<4; i++) {
    fBusyor->Disable(i);
    fBusy[0]->OrLogic(i);
    fBusy[1]->OrLogic(i);
    fBusyor->OrLogic(i);
    fBusyout->Norm(i);
  }
  
  fBusyor->Enable(0);
  fBusyor->Enable(1);
  fBusyout->Inv(0);
  fBusyout->Inv(1);
  
  // busy pattern

  fBusypat[0]->FlipFlopsMasked();
  fBusypat[1]->FlipFlopsMasked();
}

//-----------------------------------------------------------------------------
 void DTrigger::Load(char *filename) {
  // Load setup from file. Setup means here only the dynamic part
  // of setup, i.e. the parameters which can be changed by the end-user.

  Int_t i, j;
  UShort_t ram[256];
  TString buf;
  int old_options;
  Int_t temp;

  std::ifstream inpfile(filename, std::ios::in);
  if (!inpfile) {
    std::cout << "DTrigger: cannot open file " << filename << std::endl;
    return;
  }

  while (buf.ReadLine(inpfile)) {

    if (buf.Contains("physics logic")) {
      old_options = inpfile.flags(std::ios::hex);
      for (i=0; i<16; i++) {
	for (j=0; j<16; j++) {
	  inpfile >> ram[16*i+j];
	}
      }
      //inpfile.flags(old_options);
      fLogic1->WriteRAM(0, ram);
    }

    if (buf.Contains("MTCAL logic")) {
      old_options = inpfile.flags(std::ios::hex);
      for (i=0; i<16; i++) {
	for (j=0; j<16; j++) {
	  inpfile >> ram[16*i+j];
	}
      }
      //inpfile.flags(old_options);
      fLogic1->WriteRAM(1, ram);
    }

    if (buf.Contains("FLT")) {
      buf.ReplaceAll("FLT", "");
      Int_t nflt = atoi(buf.Data());
      for (i=0; i<nflt; i++) {
	inpfile >> temp;
	inpfile >> temp;
	if (temp) EnableFLT(i);
	else DisableFLT(i);
	inpfile >> temp;
	WriteFLTDownscal(i, temp);
      }
    }

    if (buf.Contains("Abort")) {
      buf.ReplaceAll("Abort", "");
      Int_t nabort = atoi(buf.Data());
      for (i=0; i<nabort; i++) {
	inpfile >> temp;
	inpfile >> temp;
	if (temp) EnableAbort(i);
	else DisableAbort(i);
      }
    }

    if (buf.Contains("SDD multiplicity")) {
      for (i=0; i<4; i++) {
	inpfile >> temp;
	inpfile >> temp;
	if (temp) EnableSDDMult(i);
	else DisableSDDMult(i);
	inpfile >> temp;
	WriteSDDMultThr(i, temp);
	inpfile >> temp;
	WriteSDDMultDownscal(i, temp);
      }
    }

    if (buf.Contains("Busy")) {
      buf.ReplaceAll("Busy", "");
      Int_t nbusy = atoi(buf.Data());
      for (i=0; i<nbusy; i++) {
	inpfile >> temp;
	inpfile >> temp;
	if (temp) EnableBusy(i);
	else DisableBusy(i);
      }
    }
  }
  return;
}
//-----------------------------------------------------------------------------
 void DTrigger::Save(char *filename) {
  // Save setup on file. Setup means here only the dynamic part of setup,
  // i.e. the parameters which can be changed by the end-user.

  Int_t i,j;
  UShort_t* ram;

  FILE *outfile;
  outfile = fopen(filename,"w");

  // physics logic

  fprintf(outfile,"physics logicn");
  ram = fLogic1->GetRAM(0);
  for (i=0; i<16; i++) {
    for (j=0; j<16; j++) fprintf(outfile, "%2x  ", ram[16*i+j] & 0xff);
    fprintf(outfile, "n");
  }
  fprintf(outfile, "n");

  // MT/CAL logic

  fprintf(outfile,"MTCAL logicn");
  ram = fLogic1->GetRAM(1);
  for (i=0; i<16; i++) {
    for (j=0; j<16; j++) fprintf(outfile, "%2x  ", ram[16*i+j] & 0xff);
    fprintf(outfile, "n");
  }
  fprintf(outfile, "n");

  // FLT

  fprintf(outfile,"FLT %dn", NFLT);
  for (i=0; i<NFLT; i++) {
    fprintf(outfile, "%2d %2d %6dn", i, IsEnabledFLT(i), GetFLTDownscal(i));
  }
  fprintf(outfile, "n");

  // Abort

  fprintf(outfile,"Abort %dn", NABORT);
  for (i=0; i<NABORT; i++) {
    fprintf(outfile, "%2d %2dn", i, IsEnabledAbort(i));
  }
  fprintf(outfile, "n");

  // SDD multiplicity

  fprintf(outfile,"SDD multiplicityn");
  for (i=0; i<4; i++) {
    fprintf(outfile, "%2d %2d %6d %6dn", i, IsEnabledSDDMult(i), 
    	    GetSDDMultThr(i), GetSDDMultDownscal(i));
  }
  fprintf(outfile, "n");

  // Busy

  fprintf(outfile,"Busy %dn", NBUSY);
  for (i=0; i<NBUSY; i++) {
    fprintf(outfile, "%2d %2dn", i, IsEnabledBusy(i));
  }
  fprintf(outfile, "n");

  fclose(outfile);
}
//-----------------------------------------------------------------------------
 void DTrigger::EnableBBP() {
  // Enable beam before-protection. 
  
  fLogic1->AddToLogic(0, 0, (char*)"&![6]");
  fLogic1->AddToLogic(0, 1, (char*)"&![6]");
  fLogic1->AddToLogic(0, 2, (char*)"&![6]");
}
//-----------------------------------------------------------------------------
 void DTrigger::EnableIBP() {
  // Enable interaction before-protection. 

  fLogic1->AddToLogic(0, 0, (char*)"&![7]");
  fLogic1->AddToLogic(0, 1, (char*)"&![7]");
  fLogic1->AddToLogic(0, 2, (char*)"&![7]");
}
//-----------------------------------------------------------------------------
 void DTrigger::DisableBBP() {
  // Disable beam before-protection.

  fLogic1->RemoveFromLogic(0, 0, (char*)"&![6]");
  fLogic1->RemoveFromLogic(0, 1, (char*)"&![6]");
  fLogic1->RemoveFromLogic(0, 2, (char*)"&![6]");
}
//-----------------------------------------------------------------------------
 void DTrigger::DisableIBP() {
  // Disable interaction before-protection.

  fLogic1->RemoveFromLogic(0, 0, (char*)"&![7]");
  fLogic1->RemoveFromLogic(0, 1, (char*)"&![7]");
  fLogic1->RemoveFromLogic(0, 2, (char*)"&![7]");
}
//-----------------------------------------------------------------------------
 void DTrigger::SwitchMTCAL(Int_t chan, Int_t inb, Int_t outb) {
  // Turn a MT or CAL channel on or off. The arguments inb and outb refer
  // to in-burst and out-of-burst. For example, SwitchMTCAL(3, 1, 0) would
  // enable 3rd MT/CAL channel in burst and disable it out of burst. 

  char tbuf[80];

  if (chan<2 || chan>7) return;

  switch (10*(inb&1) + (outb&1)) {
  case 00:
    sprintf(tbuf,"0");
    break;
  case 01:
    sprintf(tbuf,"[1]&[%d]",chan);
    break;
  case 10:
    sprintf(tbuf,"[0]&[%d]",chan);
    break;
  case 11:
    sprintf(tbuf,"([0]|[1])&[%d]",chan);
    break;
  default:
    break;
  }

  fLogic1->DefineLogic(1, chan, tbuf);
}
//-----------------------------------------------------------------------------
 void DTrigger::WriteFLTDownscal(Int_t tri,Int_t val) {
  // Set downscaling factor for FLT channel tri to val. 
  fDown[FLT_DS[tri]]->WriteDownscale(FLT_Ch[tri],val);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::GetFLTDownscal(Int_t tri) {
  // Get downscaling factor for FLT channel tri. 
  return fDown[FLT_DS[tri]]->GetDownscale(FLT_Ch[tri]);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::GetFLTCount(Int_t tri) {
  // Get count of accepted FLT signals on channel tri. 
  return fDown[FLT_DS[tri]]->GetCounter(FLT_Ch[tri]);
}
//-----------------------------------------------------------------------------
 void DTrigger::EnableFLT(Int_t tri) {
  // Enable FLT channel tri.
  fDown[FLT_DS[tri]]->EnableInOr(FLT_Ch[tri]);
  fDown[FLT_DS[tri]]->Enable(FLT_Ch[tri]);
}
//-----------------------------------------------------------------------------
 void DTrigger::DisableFLT(Int_t tri) {
  // Disable FLT channel tri.
  fDown[FLT_DS[tri]]->Disable(FLT_Ch[tri]);
  fDown[FLT_DS[tri]]->DisableInOr(FLT_Ch[tri]);
}
//-----------------------------------------------------------------------------
 void DTrigger::ManualFLTReset() {
  // Reset FLT downscalers by software reset, and then generate Reset
  // by flipping Busy.
  
  // reset downscalers

  fDown[0]->ResetDownscalers();
  fDown[1]->ResetDownscalers();
  fDown[2]->ResetDownscalers();

  // flip busy

  fBusyout->Norm(0); 
  fBusyout->Inv(0);
}
//-----------------------------------------------------------------------------
 void DTrigger::ManualAbort() {
  // Generate Abort by flipping the Abort line.
  fAboout->Inv(0);
  fAboout->Norm(0); 
}
//-----------------------------------------------------------------------------
 void DTrigger::EnableSDDMult(Int_t chan) {
  // Enable Abort by SDD multiplicity
  fDown2->EnableInOr(chan);
  fDown2->Enable(chan);
}
//-----------------------------------------------------------------------------
 void DTrigger::DisableSDDMult(Int_t chan) {
  // Disable Abort by SDD multiplicity
  fDown2->Disable(chan);
  fDown2->DisableInOr(chan);
}
//-----------------------------------------------------------------------------
 void DTrigger::WriteSDDMultThr(Int_t chan, Int_t thr) {
  // Set SDD multiplicity threshold to thr. One unit is 4 mV. 
  fDisc->WriteThreshold(chan, thr);
}
//-----------------------------------------------------------------------------
 void DTrigger::WriteSDDMultDownscal(Int_t chan, Int_t val) {
  // Set SDD multiplicity downscaling factor. 
  fDown2->WriteDownscale(chan, val);
}
//-----------------------------------------------------------------------------
 void DTrigger::EnableBusy(Int_t bus) {
  // Enable busy bus in Busy-OR. 
  Int_t section = (bus>>2) & 0x1;
  Int_t channel = (bus & 0x3); 
  fBusy[section]->Enable(channel);
}
//-----------------------------------------------------------------------------
 void DTrigger::DisableBusy(Int_t bus) {
  // Disable busy bus in Busy-OR. 
  Int_t section = (bus>>2) & 0x1;
  Int_t channel = (bus & 0x3); 
  fBusy[section]->Disable(channel);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsEnabledBBP() {
  Int_t bbp0, bbp1, bbp2;

  bbp0 = fLogic1->LogicContains(0, 0, (char*)"&![6]");
  bbp1 = fLogic1->LogicContains(0, 1, (char*)"&![6]");
  bbp2 = fLogic1->LogicContains(0, 2, (char*)"&![6]");

  if (bbp0 & bbp1 & bbp2) return 1;
  if (!bbp0 & !bbp1 & !bbp2) return 0;
  std::cout << "DTrigger::IsEnableBBP shit: " << bbp0 << bbp1 << bbp2 << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsEnabledIBP() {
  Int_t bbp0, bbp1, bbp2;

  bbp0 = fLogic1->LogicContains(0, 0, (char*)"&![7]");
  bbp1 = fLogic1->LogicContains(0, 1, (char*)"&![7]");
  bbp2 = fLogic1->LogicContains(0, 2, (char*)"&![7]");

  if (bbp0 & bbp1 & bbp2) return 1;
  if (!bbp0 & !bbp1 & !bbp2) return 0;
  std::cout << "DTrigger::IsEnableBBP shit: " << bbp0 << bbp1 << bbp2 << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsOnMTCAL(Int_t chan, Int_t burst) {
  TString tbuf;
  char inb[80];
  char outb[80];
  char none[80];
  char both[80];

  sprintf(inb,  "[0]&[%1d]", chan);
  sprintf(outb, "[1]&[%1d]", chan);
  sprintf(none, "0");
  sprintf(both, "([0]|[1])&[%1d]", chan); 
  sprintf(both, "[%1d]", chan);

  fLogic1->ExtractLogic(1, chan, &tbuf);

  if (tbuf.CompareTo(none) == 0)              return 0;
  if (tbuf.CompareTo(both) == 0)              return 1;
  if ((tbuf.CompareTo(inb) == 0) && burst)    return 1;
  if ((tbuf.CompareTo(outb) == 0) && !burst)  return 1;
  return 0;
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsEnabledFLT(Int_t tri) {
  Int_t ena;
  Int_t enainor;
  ena = (fDown[FLT_DS[tri]]->GetEnableBit(FLT_Ch[tri]));
  enainor = (fDown[FLT_DS[tri]]->GetEnableInOrBit(FLT_Ch[tri]));
  if (ena == 0 && enainor == 0) return 0;
  else if (ena != 0 && enainor != 0) return 1;
  else {
    std::cout << "Shit! enable and enable-in-or decoupled for FLT " 
	 << (int) tri << std::endl;
    return 0;
  }
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsEnabledAbort(Int_t abo) {
  return fAboinp->IsEnabled(abo);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsEnabledSDDMult(Int_t chan) {
  Int_t ena;
  Int_t enainor;
  ena = fDown2->GetEnableBit(chan);
  enainor = fDown2->GetEnableInOrBit(chan);
  if (ena == 0 && enainor == 0) return 0;
  if (ena && enainor) return 1;
  std::cout << "Shit! enable and enable-in-or decoupled for SDD multiplicity" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::GetSDDMultThr(Int_t chan) {
  return fDisc->GetThreshold(chan);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::GetSDDMultDownscal(Int_t chan) {
  return fDown2->GetDownscale(chan);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsEnabledBusy(Int_t bus) {
  Int_t section = (bus>>2) & 0x1;
  Int_t channel = (bus & 0x3);
  return fBusy[section]->IsEnabled(channel);
}
//-----------------------------------------------------------------------------
 Int_t DTrigger::IsBusy(Int_t bus) {
  Int_t section = (bus>>2) & 0x1;
  Int_t channel = (bus & 0x3);
  return fBusypat[section]->GetInput(channel);
}
//-----------------------------------------------------------------------------
 void DTrigger::ReadVME() {
  TObject   *elem;
  TIterator *iter;
  iter = fModuleList->MakeIterator();
  while ( (elem = iter->Next()) > 0) ((DModule*) elem)->ReadVME();
}
//*****************************************************************************

