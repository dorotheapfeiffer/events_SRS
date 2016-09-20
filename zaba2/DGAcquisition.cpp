///////////////////////////////////////////////////////////////////////////////
// DGAcquisition is a GUI for DAcquisition
///////////////////////////////////////////////////////////////////////////////
//
//

//

#include "DGAcquisition.h"
#include "TH1F.h"
//#include "TGFileInfo.h"
#include "TGFileDialog.h"
#include "sys/time.h"

  static UInt_t flag = 0;

// this global time variable and function only temporary!!!!
 static ULong_t gCurrentTime = 0;
 static ULong_t gElapsedTime = 0;
 static ULong_t gPrevRateTime = 0;
 static ULong_t gPrevSize = 0;

 static ULong_t gGetLongTime(){

 struct timeval t1;
 struct timezone tz;
 gettimeofday(&t1, &tz);

return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}


  ClassImp(DGAcquisition)

//-----------------------------------------------------------------------------
 DGAcquisition::DGAcquisition(DAcquisition *_fAcquisition) : TGMainFrame(gClient->GetRoot(), 0, 0) {
	
  fAcquisition = _fAcquisition;

  TGLayoutHints *fL0,*fL1,*fL2,*fL10,*fL11,*fL5;
  //TGLayoutHints *fL3,*fL4,*fL12, fL200;
  TGLayoutHints *fL201,*fL210,*fL211, *fL300;
  //TGLayoutHints *fL301,*fL310,*fL311;
  ULong_t color;

  std::cout << "[MESSAGE] Constructing GUI window\n";

  //gClient->GetColorByName("green", color);
  gClient->GetColorByName("#00CC00", color);
  ChangeBackground(color);
  
                                                                                   // left - right - top - bottom
  fL0   = new TGLayoutHints( kLHintsTop | kLHintsExpandX | kLHintsCenterX		  ,5,5,5,0);
  fL10  = new TGLayoutHints( kLHintsTop | kLHintsCenterX | kLHintsExpandX		  ,5,5,0,5);
  fL11  = new TGLayoutHints( kLHintsTop | kLHintsExpandX | kLHintsExpandY | kLHintsCenterX,5,5,5,5);
  //fL12  = new TGLayoutHints( kLHintsTop | kLHintsExpandY | kLHintsCenterX		  ,5,5,5,5);
  fL1   = new TGLayoutHints( kLHintsLeft 						  ,5,0,3,3);
  fL2   = new TGLayoutHints( kLHintsRight						  ,0,0,1,3);
  //fL3   = new TGLayoutHints( kLHintsLeft | kLHintsExpandX				  ,0,20,1,1);
  //fL4   = new TGLayoutHints( kLHintsBottom | kLHintsExpandX | kLHintsCenterX		  ,1,1,1,1);
  fL5   = new TGLayoutHints( kLHintsTop | kLHintsLeft					  ,5,5,5,0);

  //fL200 = new TGLayoutHints( kLHintsLeft | kLHintsTop					  ,5,5,2,5);
  fL211 = new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY	  ,5,5,2,5);
  fL201 = new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandY			  ,5,5,2,5);
  fL210 = new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX			  ,5,5,2,5);

  fL300 = new TGLayoutHints( kLHintsLeft | kLHintsTop				      	  ,5,5,5,5);
  //fL311 = new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsCenterX | kLHintsCenterY	  ,5,5,5,5);
  //fL301 = new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsCenterY			  ,5,5,5,5);
  //fL310 = new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsCenterX			  ,5,5,5,5);

  fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  fControlFrame = new TGVerticalFrame(this, 0, 0, kSunkenFrame);
  fCanvasFrame = new TRootEmbeddedCanvas("CanvasFrame", this, 10, 280, kSunkenFrame);
  AddFrame(fMenuBar, fL0);
  AddFrame(fControlFrame, fL10);
  AddFrame(fCanvasFrame, fL11);

  fControlFrame1 =	new TGHorizontalFrame(fControlFrame, 0, 0, 0); // upper one wtih buttons
  fControlFrame2 =	new TGHorizontalFrame(fControlFrame, 0, 0, 0); // lower one with group frames and widges

  fAcqStatusFrame =	new TGGroupFrame(fControlFrame2, "Acquisition Status", kHorizontalFrame);
  fAcqStatusFrame1 =	new TGVerticalFrame(fAcqStatusFrame, 0,0,0);
  fAcqStatusFrame2 =	new TGVerticalFrame(fAcqStatusFrame, 0,0,0);
  
  fStopAfterFrame =	new TGGroupFrame(fControlFrame2, "Stop Acq After", kHorizontalFrame);
  fStopAfterFrame1 =	new TGVerticalFrame(fStopAfterFrame, 0,0,0);
  fStopAfterFrame2 =	new TGVerticalFrame(fStopAfterFrame, 0,0,0);
  
  fSaveFrame = 		new TGGroupFrame(fControlFrame2, "Save Data Files", kVerticalFrame);
  fSaveFrame1 =		new TGHorizontalFrame(fSaveFrame, 0,0,0);
  fSaveFrame2 =		new TGHorizontalFrame(fSaveFrame, 0,0,0);

  fControlFrame->AddFrame(fControlFrame1,   fL210);
  fControlFrame->AddFrame(fControlFrame2,   fL211);
  
  fAcqStatusFrame->AddFrame(fAcqStatusFrame1, fL210);
  fAcqStatusFrame->AddFrame(fAcqStatusFrame2, fL210);

  fStopAfterFrame->AddFrame(fStopAfterFrame1, fL210);
  fStopAfterFrame->AddFrame(fStopAfterFrame2, fL210);

  fSaveFrame->AddFrame(fSaveFrame1, fL210);
  fSaveFrame->AddFrame(fSaveFrame2, fL210);

  fControlFrame2->AddFrame(fAcqStatusFrame, fL201);
  fControlFrame2->AddFrame(fStopAfterFrame, fL201);
  fControlFrame2->AddFrame(fSaveFrame,      fL201);


  // menu

  fMenuFile = new TGPopupMenu(gClient->GetRoot());
  fMenuModules = new TGPopupMenu(gClient->GetRoot());
  fMenuHelp = new TGPopupMenu(gClient->GetRoot());
  fMenuBar->AddPopup("&File", fMenuFile, fL1);
  fMenuBar->AddPopup("&Modules", fMenuModules, fL1);
  fMenuBar->AddPopup("&Help", fMenuHelp, fL2);
  fMenuFile->Associate(this);
  fMenuModules->Associate(this);
  fMenuHelp->Associate(this);

  // file menu 

  fMenuFile->AddEntry("&Load config",       eLoadSetup);  // 11
  fMenuFile->AddEntry("&Save config",       eSaveSetup);  // 12
  fMenuFile->AddEntry("&Save histograms",  eSaveData);   // 13
  fMenuFile->AddEntry("&Exit",             eExit);       // 14 

  // expert menu 

  fMenuDebug = new TGPopupMenu(gClient->GetRoot());

  //fMenuModules->AddEntry("Initialize setup", eInitializeSetup); // 53
  fMenuModules->AddPopup("modules", fMenuDebug);

  //fMenuDebug->AddEntry("Mesytec ADC 32",     eMesytecAdc32);         //39
  //fMenuDebug->AddEntry("CAEN Bridge",     eCAENBridgeV1718);         //39
  fMenuDebug->AddEntry("CAEN 1740",     eCAEN1740);         //39
  fMenuDebug->Associate(this);

  // help menu 

  fMenuHelp->AddEntry("help", eHelp);             // 65  

  // button frame

  fRunButton     = new TGTextButton(fControlFrame1, "  RUN  ",    eRunButton);
  fAcqButton     = new TGTextButton(fControlFrame1, "Start Acq",  eAcqButton);
  fDisplayButton = new TGTextButton(fControlFrame1, "DISPLAY ON", eDisplayButton);
  fResetButton   = new TGTextButton(fControlFrame1, "Reset",      eResetButton);
  fControlFrame1->AddFrame(fRunButton,    fL300);
  fControlFrame1->AddFrame(fAcqButton,    fL300);
  fControlFrame1->AddFrame(fDisplayButton,fL300);
  fControlFrame1->AddFrame(fResetButton,  fL300);
  fRunButton->Associate(this);   
  fAcqButton->Associate(this);         
  fDisplayButton->Associate(this);  
  fResetButton->Associate(this);  

  // acq time 
  fAcqStatusBuffer1 = new TGTextBuffer(80);
  fAcqStatusEntry1 = new TGTextEntry(fAcqStatusFrame1, fAcqStatusBuffer1, eAcqStatusEntry1); // 20100 +i
  fAcqStatusEntry1->Resize(80, fAcqStatusEntry1->GetDefaultHeight() );
  fAcqStatusLabel1 = new TGLabel(fAcqStatusFrame1, new TGString("Acq Time") );
  fAcqStatusEntry1->SetEnabled(kFALSE);
  fAcqStatusEntry1->SetAlignment(kTextRight);
  fAcqStatusEntry1->SetText("00:00:00");
  fAcqStatusFrame1->AddFrame(fAcqStatusLabel1, fL5);
  fAcqStatusFrame1->AddFrame(fAcqStatusEntry1, fL210);

  // acq nr events
  fAcqEventEntryField = new TGNumberEntryField(fAcqStatusFrame1, eAcqStatusEntry1, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
  fAcqEventEntryField->Resize(80, fAcqEventEntryField->GetDefaultHeight() );
  fAcqStatusLabel2 = new TGLabel(fAcqStatusFrame1, new TGString("Nr events") );
  fAcqEventEntryField->SetEnabled(kFALSE);
  //fAcqTimeEntryField->SetAlignment(kTextRight);
  fAcqEventEntryField->SetNumber( fAcquisition->m_AcqStatusEntry2 );
  fAcqStatusFrame1->AddFrame(fAcqStatusLabel2, fL5);
  fAcqStatusFrame1->AddFrame(fAcqEventEntryField, fL210);
  
  // acq events per sec
  fAcqEventSecEntryField = new TGNumberEntryField(fAcqStatusFrame2, eAcqStatusEntry1, 0, TGNumberFormat::kNESReal, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
  fAcqEventSecEntryField->Resize(80, fAcqEventSecEntryField->GetDefaultHeight() );
  fAcqStatusLabel3 = new TGLabel(fAcqStatusFrame2, new TGString("Events/sec") );
  fAcqEventSecEntryField->SetEnabled(kFALSE);
  //fAcqTimeEntryField->SetAlignment(kTextRight);
  fAcqEventSecEntryField->SetNumber( fAcquisition->m_AcqStatusEntry3 );
  fAcqStatusFrame2->AddFrame(fAcqStatusLabel3, fL5);
  fAcqStatusFrame2->AddFrame(fAcqEventSecEntryField, fL210);
  
  // data stransfer in MB
  fAcqDataTraEntryField = new TGNumberEntryField(fAcqStatusFrame2, eAcqStatusEntry1, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
  fAcqDataTraEntryField->Resize(80, fAcqDataTraEntryField->GetDefaultHeight() );
  fAcqStatusLabel4 = new TGLabel(fAcqStatusFrame2, new TGString("Data Transfer") );
  fAcqDataTraEntryField->SetEnabled(kFALSE);
  //fAcqTimeEntryField->SetAlignment(kTextRight);
  fAcqDataTraEntryField->SetNumber( fAcquisition->m_AcqStatusEntry4 );
  fAcqStatusFrame2->AddFrame(fAcqStatusLabel4, fL5);
  fAcqStatusFrame2->AddFrame(fAcqDataTraEntryField, fL210);
  
  // 2. Stop Acquisition after time

    fStopAfterSecCB    = new TGCheckButton(fStopAfterFrame1, "Nr Seconds",    eStopAfterSecCB);
    fStopAfterFileCB   = new TGCheckButton(fStopAfterFrame1, "Nr Files",  eStopAfterFileCB);  
    fStopAfterEventsCB = new TGCheckButton(fStopAfterFrame2, "Nr Events", eStopAfterEventsCB); 
    fStopAfterSecEntry    = new TGNumberEntry(fStopAfterFrame1, fAcquisition->m_StopAfterSecEntry, 9, eStopAfterSecEntry, 
                                TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
    fStopAfterFileEntry   = new TGNumberEntry(fStopAfterFrame1, fAcquisition->m_StopAfterFileEntry, 9, eStopAfterFileEntry, 
                                TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
    fStopAfterEventsEntry = new TGNumberEntry(fStopAfterFrame2, fAcquisition->m_StopAfterEventsEntry, 9, eStopAfterEventsEntry, 
                                TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
    (fAcquisition->m_StopAfterSecEntry)    ? fStopAfterSecEntry->SetState(kTRUE)     : fStopAfterSecEntry->SetState(kFALSE);    
    (fAcquisition->m_StopAfterFileEntry)   ? fStopAfterFileEntry->SetState(kTRUE)    : fStopAfterFileEntry->SetState(kFALSE);   
    (fAcquisition->m_StopAfterEventsEntry) ? fStopAfterEventsEntry->SetState(kTRUE) : fStopAfterEventsEntry->SetState(kFALSE);

    // I use connect instead Associate(this) because I couldnt understand how the TGNumberEntry works.....
    fStopAfterSecEntry->Connect("ValueSet(Long_t)", "DGAcquisition", this, "DoValueSet()");
    (fStopAfterSecEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGAcquisition", this, "DoValueSet()");
    fStopAfterFileEntry->Connect("ValueSet(Long_t)", "DGAcquisition", this, "DoValueSet()");
    (fStopAfterFileEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGAcquisition", this, "DoValueSet()");
    fStopAfterEventsEntry->Connect("ValueSet(Long_t)", "DGAcquisition", this, "DoValueSet()");
    (fStopAfterEventsEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGAcquisition", this, "DoValueSet()");
 
    fStopAfterSecEntry->Resize(80, fStopAfterSecEntry->GetDefaultHeight() );
    fStopAfterFileEntry->Resize(80, fStopAfterFileEntry->GetDefaultHeight() );
    fStopAfterEventsEntry->Resize(80, fStopAfterEventsEntry->GetDefaultHeight() );
    fStopAfterFrame1->AddFrame(fStopAfterSecCB, fL5);
    fStopAfterFrame1->AddFrame(fStopAfterSecEntry, fL210);
    fStopAfterFrame1->AddFrame(fStopAfterFileCB, fL5);
    fStopAfterFrame1->AddFrame(fStopAfterFileEntry, fL210);
    fStopAfterFrame2->AddFrame(fStopAfterEventsCB, fL5);
    fStopAfterFrame2->AddFrame(fStopAfterEventsEntry, fL210);
    fStopAfterSecCB->Associate(this);   
    fStopAfterFileCB->Associate(this); 
    fStopAfterEventsCB->Associate(this);

  // 3. Save file after/or
 
    fSaveSizeCB       = new TGCheckButton(fSaveFrame1, "Size [MB]",    eSaveFileSizeCB);    //20902
    fSaveSizeEntry    = new TGNumberEntry(fSaveFrame1, fAcquisition->m_SaveFileSizeEntry, 9, eSaveFileSizeEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                            TGNumberFormat::kNELLimitMinMax, 1, 500  ); 
    fSaveLabel        = new TGLabel(fSaveFrame1, new TGString("OR") );
    fSaveTimeCB    = new TGCheckButton(fSaveFrame1, "Nr Seconds",    eSaveFileTimeCB);    //20902
    fSaveTimeEntry = new TGNumberEntry(fSaveFrame1, fAcquisition->m_SaveFileTimeEntry, 9, eSaveFileTimeEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                            TGNumberFormat::kNELLimitMin, 1  ); 
    fSaveSizeEntry->SetNumber( fAcquisition->m_SaveFileSizeEntry );
    fSaveTimeEntry->SetNumber( fAcquisition->m_SaveFileTimeEntry );

    fSaveSizeEntry->SetState(kFALSE);
    fSaveTimeEntry->SetState(kFALSE);

    fSaveFrame1->AddFrame(fSaveSizeCB,       fL5);         
    fSaveFrame1->AddFrame(fSaveSizeEntry  ,  fL10); 
    fSaveFrame1->AddFrame(fSaveLabel      ,  fL210); 
    fSaveFrame1->AddFrame(fSaveTimeCB  ,  fL5); 
    fSaveFrame1->AddFrame(fSaveTimeEntry, fL10);  

    // I use connect instead Associate(this) because I couldnt understand how the TGNumberEntry works.....
    fSaveTimeEntry->Connect("ValueSet(Long_t)", "DGAcquisition", this, "DoValueSet()");
    (fSaveTimeEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGAcquisition", this, "DoValueSet()");
    fSaveSizeEntry->Connect("ValueSet(Long_t)", "DGAcquisition", this, "DoValueSet()");
    (fSaveSizeEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGAcquisition", this, "DoValueSet()");
    fSaveSizeCB->Associate(this);       
    fSaveTimeCB->Associate(this);   

 
  // Canvas frame
  TCanvas *c = fCanvasFrame->GetCanvas();
  
  c->Range(0,0, 100, 100);
  c->SetFillColor(171);
  //c->SetEditable(kFALSE);
  //c->cd();

  fGDisplay = new DGDisplay(fAcquisition, c);
  fDataSave = new DDataSave(fAcquisition);

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  SetWindowName(fAcquisition->m_AcqName.c_str());
  MapWindow();
  this->Move(100,100);
   
  fTimer = new TTimer(this, fAcquisition->m_Timerout);

  std::cout << "[MESSAGE] All initialization DONE!" << std::endl;
  std::cout << "        + System ready to start" << std::endl;
}

//-----------------------------------------------------------------------------
 DGAcquisition::~DGAcquisition() {
  std::cout <<"destroying DGAcquisition, timer off" << std::endl;

    fStopAfterSecEntry->Disconnect("ValueSet(Long_t)");
    (fStopAfterSecEntry->GetNumberEntry())->Disconnect("ReturnPressed()");
    fStopAfterFileEntry->Disconnect("ValueSet(Long_t)");
    (fStopAfterFileEntry->GetNumberEntry())->Disconnect("ReturnPressed()");
    fStopAfterEventsEntry->Disconnect("ValueSet(Long_t)");
    (fStopAfterEventsEntry->GetNumberEntry())->Disconnect("ReturnPressed()");

    fSaveTimeEntry->Disconnect("ValueSet(Long_t)");
    (fSaveTimeEntry->GetNumberEntry())->Disconnect("ReturnPressed()");
    fSaveSizeEntry->Disconnect("ValueSet(Long_t)");
    (fSaveSizeEntry->GetNumberEntry())->Disconnect("ReturnPressed()");

  fTimer->TurnOff();
  delete fTimer;
  delete fDataSave;
  delete fGDisplay;
  Cleanup();
  //delete fAcquisition;
  //Connect("CloseWindow()", "DAcquisition", fAcquisition, "Exit()");
}
//-----------------------------------------------------------------------------
 void DGAcquisition::Refresh() {
  // Update display so it represents actual contents of fAcquisition status.
  

  if(fStopAfterSecCB->GetState() == 1){
     //fAcquisition->m_StopAfterSecEntry = 1;
     fStopAfterSecEntry->SetState(EButtonState(1));
     fStopAfterSecEntry->SetNumber(fAcquisition->m_StopAfterSecEntry);

     fAcquisition->m_StopAfterFileEntry = 0;
     fStopAfterFileEntry->SetNumber(0);
     fStopAfterFileEntry->SetState(EButtonState(0));
     fStopAfterFileCB->SetState(EButtonState(0));


     fAcquisition->m_StopAfterEventsEntry = 0;
     fStopAfterEventsEntry->SetNumber(0);
     fStopAfterEventsEntry->SetState(EButtonState(0));
     fStopAfterEventsCB->SetState(EButtonState(0));
    } 
  else{
     fAcquisition->m_StopAfterSecEntry = 0;
     fStopAfterSecEntry->SetNumber( fAcquisition->m_StopAfterSecEntry);
     fStopAfterSecEntry->SetState(EButtonState(0));
    }

  if(fStopAfterFileCB->GetState() == 1){
     //fAcquisition->m_StopAfterFileEntry = 1;
     fStopAfterFileEntry->SetState(EButtonState(1));
     fStopAfterFileEntry->SetNumber( fAcquisition->m_StopAfterFileEntry );

     fAcquisition->m_StopAfterSecEntry = 0;
     fStopAfterSecEntry->SetNumber(0);
     fStopAfterSecEntry->SetState(EButtonState(0));
     fStopAfterSecCB->SetState(EButtonState(0));

     fAcquisition->m_StopAfterEventsEntry = 0;
     fStopAfterEventsEntry->SetNumber(0);
     fStopAfterEventsEntry->SetState(EButtonState(0));
     fStopAfterEventsCB->SetState(EButtonState(0));
    } 
  else{
     fAcquisition->m_StopAfterFileEntry = 0;
     fStopAfterFileEntry->SetState(EButtonState(0));
     fStopAfterFileEntry->SetNumber( fAcquisition->m_StopAfterFileEntry );
    }

  if(fStopAfterEventsCB->GetState() == 1){
     //fAcquisition->m_StopAfterEventsEntry = 1;
     fStopAfterEventsEntry->SetState(EButtonState(1));
     fStopAfterEventsEntry->SetNumber( fAcquisition->m_StopAfterEventsEntry );

     fAcquisition->m_StopAfterSecEntry = 0;
     fStopAfterSecEntry->SetNumber(0);
     fStopAfterSecEntry->SetState(EButtonState(0));
     fStopAfterSecCB->SetState(EButtonState(0));

     fAcquisition->m_StopAfterFileEntry = 0;
     fStopAfterFileEntry->SetNumber(0);
     fStopAfterFileEntry->SetState(EButtonState(0));
     fStopAfterFileCB->SetState(EButtonState(0));

    } 
  else{
     fAcquisition->m_StopAfterEventsEntry = 0;
     fStopAfterEventsEntry->SetState(EButtonState(0));
     fStopAfterEventsEntry->SetNumber(fAcquisition->m_StopAfterEventsEntry );
    }

  if(fAcquisition->m_Status == 0){ // start program, waiting for user reaction, program is in STOP MODE
    fRunButton->SetEnabled(kTRUE);
    fAcqButton->SetEnabled(kTRUE);
    fRunButton->SetText("  RUN  ");
    fAcqButton->SetText("Start Acq");

    //fAcqTimeEntryField->SetText("00:00:00");
    fAcqEventEntryField->SetNumber(    fAcquisition->m_AcqStatusEntry2 );
    fAcqEventSecEntryField->SetNumber( fAcquisition->m_AcqStatusEntry3 );
    fAcqDataTraEntryField->SetNumber(  fAcquisition->m_AcqStatusEntry4 );

    }
  else if (fAcquisition->m_Status == 1){ // RUN MODE
    fRunButton->SetEnabled(kTRUE);
    fAcqButton->SetEnabled(kTRUE);
    fRunButton->SetText("STOP");
    fAcqButton->SetText("Start Acq");

    fAcquisition->ResetModules();
    fAcquisition->m_AcqStatusEntry2 = 0;
    fAcquisition->m_AcqStatusEntry3 = 0;
    fAcquisition->m_AcqStatusEntry4 = 0;

    fAcqEventEntryField->SetNumber(    fAcquisition->m_AcqStatusEntry2 );
    fAcqEventSecEntryField->SetNumber( fAcquisition->m_AcqStatusEntry3 );
    fAcqDataTraEntryField->SetNumber(  fAcquisition->m_AcqStatusEntry4 );
    fAcquisition->m_ElapsedAcqTime = 0;
    fAcquisition->m_StartAcqTime = std::time(NULL);
    }
  else{  				// ACQ MODE
    fRunButton->SetEnabled(kFALSE);
    fAcqButton->SetEnabled(kTRUE);
    fRunButton->SetText("STOP");
    fAcqButton->SetText("Stop Acq");

    fAcquisition->ResetModules();
    fAcquisition->m_AcqStatusEntry2 = 0;
    fAcquisition->m_AcqStatusEntry3 = 0;
    fAcquisition->m_AcqStatusEntry4 = 0;

    fAcqEventEntryField->SetNumber(    fAcquisition->m_AcqStatusEntry2 );
    fAcqEventSecEntryField->SetNumber( fAcquisition->m_AcqStatusEntry3 );
    fAcqDataTraEntryField->SetNumber(  fAcquisition->m_AcqStatusEntry4 );
    fAcquisition->m_ElapsedAcqTime = 0;
    fAcquisition->m_StartAcqTime = std::time(NULL);
    }

}
//-----------------------------------------------------------------------------
 Bool_t DGAcquisition::HandleTimer(TTimer *tim) {
  
  fTick++;

  //..................
  // stops acquisition after certain number of events set by the user in GUI
  if(fAcquisition->m_StopAfterEventsEntry){
     if( fAcquisition->m_AcqStatusEntry2 >= fAcquisition->m_StopAfterEventsEntry ){
         ProcessMessage(259, eRunButton, 0); // simulate stop button click...
         RefreshStatus();
         return kFALSE;
       }
     }

  //..................
  // stops acquisition after certain number of seconds set by the user in GUI
  if(fAcquisition->m_StopAfterSecEntry){
     std::cout << fAcquisition->m_ElapsedAcqTime << " " << (UInt_t)fAcquisition->m_StopAfterSecEntry << std::endl;
     if( fAcquisition->m_ElapsedAcqTime >= (UInt_t)fAcquisition->m_StopAfterSecEntry ){
         ProcessMessage(259, eRunButton, 0); // simulate stop button click...
         RefreshStatus();
         return kFALSE;
       }
     }


  //..................
  // stops acquisition after certain number of saved files set by the user in GUI
 if(fAcquisition->m_StopAfterFileEntry){
     if( fAcquisition->m_NrOfSavedFiles >= fAcquisition->m_StopAfterFileEntry ){
         ProcessMessage(259, eRunButton, 0); // simulate stop button click...
         RefreshStatus();
         return kFALSE;
       }
     }

  //..................
  // transfer data from VME modules, one after the other according to the fModuleList, if neessary display and/or save
  
  if( flag == 0 ){
      flag = 1;
      fAcquisition->ReadVME();
      fAcquisition->m_AcqStatusEntry2 = fAcquisition->fDMadc32->GetNrEvents(); 
      fAcquisition->m_AcqStatusEntry4 = fAcquisition->fDMadc32->GetDataSize(); 
     
      fAcquisition->ShowData(fGDisplay);
 
         
      //std::cout << fAcquisition->m_DataSave << std::endl;
      if( fAcquisition->m_DataSave )
          fAcquisition->DataSave();

      // this is not yet implemented, will be in the next advanced version
      //fAcquisition->BuildEvent();
      //fAcquisition->SaveiEvent();
      //fAcquisition->OnlineEvent();

      flag = 0;
      }   

  //..................
  // refresh acquisition status in the main GUI
  if(fTick == 990){
     RefreshStatus();
     fTimer->Reset();

     fTick = 0;
   } 

  return kTRUE;
}
//-----------------------------------------------------------------------------

 void DGAcquisition::RefreshStatus(){
     fAcquisition->m_TimeNow = std::time(NULL);
     fAcquisition->m_ElapsedAcqTime = (fAcquisition->m_TimeNow) - (fAcquisition->m_StartAcqTime);

     TDatime acqtime(fAcquisition->m_ElapsedAcqTime);

     // Update acq time
     char time[256];
     sprintf(time, "%02d:%02d:%02d\n", acqtime.GetHour()-1, acqtime.GetMinute(), acqtime.GetSecond());
     fAcqStatusEntry1->SetText(time);

     // update nr of events
     fAcquisition->m_AcqStatusEntry2 = fAcquisition->fDMadc32->GetNrEvents();
     fAcqEventEntryField->SetNumber( fAcquisition->m_AcqStatusEntry2 );

     // update event/sec
     gCurrentTime = gGetLongTime();
     gElapsedTime = gCurrentTime - gPrevRateTime;

     fAcqEventSecEntryField->SetNumber( (float)(fAcquisition->m_AcqStatusEntry2 - fAcquisition->m_PrevEvent)*1000.0f / (float)gElapsedTime );
     fAcquisition->m_PrevEvent = fAcquisition->m_AcqStatusEntry2;
     fAcquisition->m_AcqStatusEntry3 = fAcquisition->fDMadc32->GetDataSize();

     // update transfer rate
     fAcqDataTraEntryField->SetNumber( (float)( fAcquisition->m_AcqStatusEntry3 - fAcquisition->m_PrevSize) / (float)gElapsedTime * 1048.57f);
     fAcquisition->m_PrevSize = fAcquisition->m_AcqStatusEntry3;
    
     gPrevSize = fAcquisition->m_AcqStatusEntry3;
     gPrevRateTime = gCurrentTime;

      
     // update all trigger (accepted + rejected)
     // update number of files
     // update nr of acquisition time (in second)
     //

     fGDisplay->Refresh();
}
//-----------------------------------------------------------------------------
 void DGAcquisition::DoValueSet(){

 TGNumberEntry *te = (TGNumberEntry *) gTQSender;
 Int_t id = te->WidgetId();

   switch (id) {
     case eStopAfterSecEntry:
        fAcquisition->m_StopAfterSecEntry = (Int_t)te->GetNumber();
     break;
   
     case eStopAfterFileEntry:
        fAcquisition->m_StopAfterFileEntry = (Int_t)te->GetNumber();
     break;

     case eStopAfterEventsEntry:
        fAcquisition->m_StopAfterEventsEntry = (Int_t)te->GetNumber();
     break;

     case eSaveFileTimeEntry:
        fAcquisition->m_SaveFileTimeEntry = (Int_t)te->GetNumber();
     break;

     case eSaveFileSizeEntry:
        fAcquisition->m_SaveFileSizeEntry = (Int_t)te->GetNumber();
     break;
     default:
     break;
   }

 }
//-----------------------------------------------------------------------------
 Bool_t DGAcquisition::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

  std::cout << "msg " << msg << ", parm1 "<< parm1 << ", parm2 "<< parm2 << std::endl;

  TGFileInfo fi;
  TGFileInfo dfi; 
  const char *filetypes[] = { "MultiGrid setup files", "*.tri", 
			      "All files", "*",
			      0,0,
			      0,0};
  fi.fFileTypes = (const char **)filetypes;


  fAcquisition->m_TimeNow = std::time(NULL);
  std::string s1 = std::string(ctime(&fAcquisition->m_TimeNow));
  *fAcquisition->fLog << s1 << " ----- ";

  switch(GET_MSG(msg)) {

  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_MENU:
      switch (parm1) {
      case eLoadSetup:
	// load setup
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen,&fi);
	if (fi.fFilename) fAcquisition->LoadConfig(fi.fFilename);
	*fAcquisition->fLog << "load setup from " << fi.fFilename;
	std::cout << "load setup from " << fi.fFilename;
	break;
      case eSaveSetup:
	// save setup
	new TGFileDialog(fClient->GetRoot(), this, kFDSave,&fi);
	if (fi.fFilename) fAcquisition->SaveConfig(fi.fFilename);
	*fAcquisition->fLog << "save setup to   " << fi.fFilename;
	std::cout << "save setup to   " << fi.fFilename;
	break;
      case eSaveData:
	// save histogram data
	//new TGFileDialog(fClient->GetRoot(), this, kFDSave,&fi);
	//if (fi.fFilename) fAcquisition->SaveData(fi.fFilename);
	*fAcquisition->fLog << "save data to   " << fi.fFilename;
	std::cout << "save data to   " << fi.fFilename;
	break;
      case eExit:
	// exit
	*fAcquisition->fLog << "exit\n";
	std::cout << "exit\n";
	exit(0);
	break;
      case eInitializeSetup:
	// Initialize setup
	*fAcquisition->fLog << "Initialize setup\n";
	std::cout << "Initialize setup\n";
	exit(0);
	break;
      case eMesytecAdc32:
	new DGMadc32(fAcquisition->fDMadc32);
	*fAcquisition->fLog << "open GUI for Mesytec\n";
	std::cout << "open GUI for Mesytec\n";
	break;
      case eCAEN1740:
	    std::cout << "hello 3 `...." << std::endl;
	new DGCAEN1740(fAcquisition->fDCAEN1740);
	*fAcquisition->fLog << "open GUI for CAEN 1740\n";
	std::cout << "open GUI for CAEN 1740\n";
	break;
      case eCAENBridgeV1718:
	new DGV1718(fAcquisition->fDV1718);
	*fAcquisition->fLog << "open GUI for CAEN Bridge\n";
	std::cout << "open GUI for CAEN Bridge\n";
	break;
      case eHelp:
	new DGHelp((char*)"./help.hlp");
	*fAcquisition->fLog << "monitor help";
	std::cout << "monitor help";
	break;
      default:
	    std::cout << "hello 4 `...." << std::endl;
	break;
      }
    case kCM_CHECKBUTTON:
      switch (parm1) {
        case eStopAfterSecCB:
          fStopAfterFileCB->SetState(EButtonState(0));
          fStopAfterEventsCB->SetState(EButtonState(0));
	  *fAcquisition->fLog << "\tSet stop Acq parameter after [...] sec\n "; 
	  std::cout << "Set stop Acq parameter after [...] sec\n ";
        break;
        case eStopAfterFileCB:
          fStopAfterSecCB->SetState(EButtonState(0));
          fStopAfterEventsCB->SetState(EButtonState(0));
	  *fAcquisition->fLog << "\tSet stop Acq parameter after [...] files\n " ;
	  std::cout << "Set stop Acq parameter after [...] files \n";
        break;
        case eStopAfterEventsCB:
          fStopAfterSecCB->SetState(EButtonState(0));
          fStopAfterFileCB->SetState(EButtonState(0));
	  *fAcquisition->fLog << "\tSet stop Acq parameter after [...] events\n ";
	  std::cout << "Set stop Acq parameter after [...] events \n";
        break;
        case eSaveFileSizeCB:
          fSaveSizeEntry->SetState(fSaveSizeCB->GetState());
	  *fAcquisition->fLog << "\tSet SaveFileSize parameter after [...] file size\n ";
	  std::cout << "Set SaveFileSize parameter after [...] file size\n ";
        break;
        case eSaveFileTimeCB:
          fSaveTimeEntry->SetState(fSaveTimeCB->GetState());
	  *fAcquisition->fLog << "\tSet SaveFileTime parameter after [...] file number\n";
	  std::cout << "Set SaveFileTime parameter after [...] file number\n";
        break;
        default:
        break;

      }
    case kCM_BUTTON:
      //std::cout << "kCMBUTTON:" << kCM_BUTTON << " parm1: "<< parm1 << " parm2: " << parm1 << std::endl;
      switch (parm1){
        case eRunButton:
           fAcquisition->m_DataSave = 0;
           if(fAcquisition->m_Status == 0){
              fAcquisition->m_Status = 1;
              fTick = 0;
              fAcquisition->StartAcq();
              fTimer->TurnOn();
	      *fAcquisition->fLog << "\tClick button RUN, program in mode: " << fAcquisition->m_Status << std::endl;
	      std::cout << "Click button RUN, program in mode: " << fAcquisition->m_Status << std::endl;
              }
           else{              
              fAcquisition->m_Status = 0;
              fAcquisition->StopAcq();
              fTimer->TurnOff();
              fTick = 0;
	      *fAcquisition->fLog << "\tClick button STOP, program in mode: " << fAcquisition->m_Status << std::endl;
	      std::cout << "Click button STOP, program in mode: " << fAcquisition->m_Status << std::endl;
             }

        break;

        case eAcqButton:

           if(fAcquisition->m_Status == 0 || fAcquisition->m_Status == 1){
	      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &dfi);
              std::string s1(dfi.fFilename);
	      if (s1.length()) {
                  std::cout << " ======== s1: " << s1 << std::endl;
                  std::size_t found  = s1.find_last_of("/\\");
                  fAcquisition->SetFileName(s1.substr(found+1) );
                  fAcquisition->SetDataPath(s1.substr(0,found) );
                  std::cout << s1.substr(0, found) << std::endl;
                  std::cout << s1.substr(found+1) << std::endl;
                  }
              else return 0; 
              fAcquisition->m_NrOfSavedFiles = 0;
              fAcquisition->m_DataSave = 1;
              fAcquisition->m_Status = 2;
              fTick = 0;
              fAcquisition->StartAcq();
              fTimer->TurnOn();
	      *fAcquisition->fLog << "\tClick button ACQ/Start, program in mode: " << fAcquisition->m_Status << std::endl;
	      std::cout << "Click button ACQ/Start, program in mode: " << fAcquisition->m_Status << std::endl;
              }
           else{ 
              fAcquisition->m_DataSave = 0;
              fAcquisition->m_Status = 0;
              fAcquisition->StopAcq();
              fTimer->TurnOff();
              fTick = 0;
	      *fAcquisition->fLog << "\tClick button ACQ/Stop, program in mode: " << fAcquisition->m_Status << std::endl;
	      std::cout << "Click button ACQ/Stop, program in mode: " << fAcquisition->m_Status << std::endl;
              } 
        break;

        case eDisplayButton:
           if(fAcquisition->m_Display == 1) { fAcquisition->m_Display = 0; fDisplayButton->SetText("DISPLAY OFF");}
           else { fAcquisition->m_Display = 1; fDisplayButton->SetText("DISPLAY ON");}
	   *fAcquisition->fLog << "\tClick button DISPLAY ON/OFF: " << fAcquisition->m_Display << std::endl;
	   std::cout << "Click button DISPLAY ON/OFF: " << fAcquisition->m_Display << std::endl;
        break;

        case eResetButton:
            Int_t ret;
            new TGMsgBox(gClient->GetRoot(), this, " WARNING !!! ", 
                                "All histograms will be cleaned, and all data on histograms will be lost", 0, 0x41, &ret);
            if( ret )  
              fGDisplay->Reset();
 
	    *fAcquisition->fLog << "\tReset all histograms ";
	    std::cout << "Reset all histograms " << std::endl;
        break;

        default:
        break;
       }

    break;

    default:
    break;
      
    }
    fTick = 0;       // refresh
    break;
    
  default:
	    std::cout << "Message not recognized " << std::endl;
    break;
  }
  Refresh();
  *fAcquisition->fLog << std::endl;
  return kTRUE;

}
