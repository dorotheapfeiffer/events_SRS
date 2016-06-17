///////////////////////////////////////////////////////////////////////////////
// DGMultiGrid is a GUI for DMultiGrid
///////////////////////////////////////////////////////////////////////////////
//
//

//

#include "DGMultiGrid.h"
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


  ClassImp(DGMultiGrid)

//-----------------------------------------------------------------------------
 DGMultiGrid::DGMultiGrid(DMultiGrid *aDMultiGrid) : TGMainFrame(gClient->GetRoot(), 0, 0) {
  TGLayoutHints *fL0,*fL1,*fL2,*fL10,*fL11,*fL5;
  //TGLayoutHints *fL3,*fL4,*fL12, fL200;
  TGLayoutHints *fL201,*fL210,*fL211, *fL300;
  //TGLayoutHints *fL301,*fL310,*fL311;
  ULong_t color;

  std::cout << "constructing DGMultiGrid...\n";

  fMultiGrid = aDMultiGrid;;

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

  fMenuFile->AddEntry("&Load setup",       eLoadSetup);  // 11
  fMenuFile->AddEntry("&Save setup",       eSaveSetup);  // 12
  fMenuFile->AddEntry("&Save histograms",  eSaveData);   // 13
  fMenuFile->AddEntry("&Exit",             eExit);       // 14 

  // expert menu 

  fMenuDebug = new TGPopupMenu(gClient->GetRoot());

  //fMenuModules->AddEntry("Initialize setup", eInitializeSetup); // 53
  fMenuModules->AddPopup("modules", fMenuDebug);

  fMenuDebug->AddEntry("Mesytec ADC 32",     eMesytecAdc32);         //39
  fMenuDebug->AddEntry("CAEN Bridge",     eCAENBridgeV1718);         //39
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
  fAcqEventEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry2 );
  fAcqStatusFrame1->AddFrame(fAcqStatusLabel2, fL5);
  fAcqStatusFrame1->AddFrame(fAcqEventEntryField, fL210);
  
  // acq events per sec
  fAcqEventSecEntryField = new TGNumberEntryField(fAcqStatusFrame2, eAcqStatusEntry1, 0, TGNumberFormat::kNESReal, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
  fAcqEventSecEntryField->Resize(80, fAcqEventSecEntryField->GetDefaultHeight() );
  fAcqStatusLabel3 = new TGLabel(fAcqStatusFrame2, new TGString("Events/sec") );
  fAcqEventSecEntryField->SetEnabled(kFALSE);
  //fAcqTimeEntryField->SetAlignment(kTextRight);
  fAcqEventSecEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry3 );
  fAcqStatusFrame2->AddFrame(fAcqStatusLabel3, fL5);
  fAcqStatusFrame2->AddFrame(fAcqEventSecEntryField, fL210);
  
  // data stransfer in MB
  fAcqDataTraEntryField = new TGNumberEntryField(fAcqStatusFrame2, eAcqStatusEntry1, 0, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
  fAcqDataTraEntryField->Resize(80, fAcqDataTraEntryField->GetDefaultHeight() );
  fAcqStatusLabel4 = new TGLabel(fAcqStatusFrame2, new TGString("Data Transfer") );
  fAcqDataTraEntryField->SetEnabled(kFALSE);
  //fAcqTimeEntryField->SetAlignment(kTextRight);
  fAcqDataTraEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry4 );
  fAcqStatusFrame2->AddFrame(fAcqStatusLabel4, fL5);
  fAcqStatusFrame2->AddFrame(fAcqDataTraEntryField, fL210);
  
  // 2. Stop Acquisition after time

    fStopAfterSecCB    = new TGCheckButton(fStopAfterFrame1, "Nr Seconds",    eStopAfterSecCB);    //20902
    fStopAfterFileCB   = new TGCheckButton(fStopAfterFrame1, "Nr Files",  eStopAfterFileCB);  //20902
    fStopAfterEventsCB = new TGCheckButton(fStopAfterFrame2, "Nr Events", eStopAfterEventsCB); //20902
    fStopAfterSecEntry    = new TGNumberEntry(fStopAfterFrame1, 0, 9, eStopAfterSecEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
    fStopAfterFileEntry   = new TGNumberEntry(fStopAfterFrame1, 0, 9, eStopAfterFileEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
    fStopAfterEventsEntry = new TGNumberEntry(fStopAfterFrame2, 0, 9, eStopAfterEventsEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0  ); 
    fStopAfterSecEntry->SetState(kFALSE);    
    fStopAfterFileEntry->SetState(kFALSE);   
    fStopAfterEventsEntry->SetState(kFALSE);

    // I use connect instead Associate(this) because I couldnt understand how the TGNumberEntry works.....
    fStopAfterSecEntry->Connect("ValueSet(Long_t)", "DGMultiGrid", this, "DoValueSet()");
    (fStopAfterSecEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGMultiGrid", this, "DoValueSet()");
    fStopAfterFileEntry->Connect("ValueSet(Long_t)", "DGMultiGrid", this, "DoValueSet()");
    (fStopAfterFileEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGMultiGrid", this, "DoValueSet()");
    fStopAfterEventsEntry->Connect("ValueSet(Long_t)", "DGMultiGrid", this, "DoValueSet()");
    (fStopAfterEventsEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGMultiGrid", this, "DoValueSet()");
 
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
    fSaveSizeEntry    = new TGNumberEntry(fSaveFrame1, 0, 9, eSaveFileSizeEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                            TGNumberFormat::kNELLimitMinMax, 1, 500  ); 
    fSaveLabel        = new TGLabel(fSaveFrame1, new TGString("OR") );
    fSaveTimeCB    = new TGCheckButton(fSaveFrame1, "Nr Seconds",    eSaveFileTimeCB);    //20902
    fSaveTimeEntry = new TGNumberEntry(fSaveFrame1, 0, 9, eSaveFileTimeEntry, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                            TGNumberFormat::kNELLimitMin, 1  ); 
    fSaveSizeEntry->SetNumber( fMultiGrid->m_SaveFileSizeEntry );
    fSaveTimeEntry->SetNumber( fMultiGrid->m_SaveFileTimeEntry );

    fSaveSizeEntry->SetState(kFALSE);
    fSaveTimeEntry->SetState(kFALSE);
    fSaveFrame1->AddFrame(fSaveSizeCB,       fL5);         
    fSaveFrame1->AddFrame(fSaveSizeEntry  ,  fL10); 
    fSaveFrame1->AddFrame(fSaveLabel      ,  fL210); 
    fSaveFrame1->AddFrame(fSaveTimeCB  ,  fL5); 
    fSaveFrame1->AddFrame(fSaveTimeEntry, fL10);  

    // I use connect instead Associate(this) because I couldnt understand how the TGNumberEntry works.....
    fSaveTimeEntry->Connect("ValueSet(Long_t)", "DGMultiGrid", this, "DoValueSet()");
    (fSaveTimeEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGMultiGrid", this, "DoValueSet()");
    fSaveSizeEntry->Connect("ValueSet(Long_t)", "DGMultiGrid", this, "DoValueSet()");
    (fSaveSizeEntry->GetNumberEntry())->Connect("ReturnPressed()", "DGMultiGrid", this, "DoValueSet()");
    fSaveSizeCB->Associate(this);       
    fSaveTimeCB->Associate(this);   

 
  // Canvas frame
  TCanvas *c = fCanvasFrame->GetCanvas();
  
  c->Range(0,0, 100, 100);
  c->SetFillColor(171);
  //c->SetEditable(kFALSE);
  //c->cd();

  fGDisplay = new DGDisplay(fMultiGrid, c);
  fDataSave = new DDataSave(fMultiGrid);

  MapSubwindows();
  this->Resize(this->GetDefaultSize());
  SetWindowName("MultiGrid Readout");
  MapWindow();
  this->Move(100,100);
   
  std::cout << "Set acq loop to " << fMultiGrid->m_Timerout << std::endl;
  fTimer = new TTimer(this, fMultiGrid->m_Timerout);
  fDatime = new TDatime();

  // logfile

  fLog = new std::ofstream("trilog.txt", std::ios::out|std::ios::ate);
  *fLog << fDatime->AsString() << " " << "new DGMultiGrid" << std::endl;
  *fLog << "Set ACQ loop to " << (fMultiGrid->m_Timerout / 1000) << " sec"<< std::endl;

  std::cout << "Creation done!" << std::endl;

  std::cout << "Initialization modules" << std::endl;
  fMultiGrid->InitModules();
  std::cout << " All initialization DONE!" << std::endl;
}

//-----------------------------------------------------------------------------
 DGMultiGrid::~DGMultiGrid() {
  std::cout <<"destroying DGMultiGrid, timer off" << std::endl;

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
  //std::cout <<"destroying DGMultiGrid, timer delete" << std::endl;
  delete fTimer;
  //std::cout <<"destroying DGMultiGrid, flog delete" << std::endl;
  delete fLog;
  //std::cout <<"destroying DGMultiGrid, fDatime delete" << std::endl;
  delete fDatime;
  delete fDataSave;
  delete fGDisplay;

  delete fMultiGrid;
}
//-----------------------------------------------------------------------------
 void DGMultiGrid::Refresh() {
  // Update display so it represents actual contents of fMultiGrid status.
  

  if(fStopAfterSecCB->GetState() == 1){
     //fMultiGrid->m_StopAfterSecEntry = 1;
     fStopAfterSecEntry->SetState(EButtonState(1));
     fStopAfterSecEntry->SetNumber(fMultiGrid->m_StopAfterSecEntry);

     fMultiGrid->m_StopAfterFileEntry = 0;
     fStopAfterFileEntry->SetNumber(0);
     fStopAfterFileEntry->SetState(EButtonState(0));
     fStopAfterFileCB->SetState(EButtonState(0));


     fMultiGrid->m_StopAfterEventsEntry = 0;
     fStopAfterEventsEntry->SetNumber(0);
     fStopAfterEventsEntry->SetState(EButtonState(0));
     fStopAfterEventsCB->SetState(EButtonState(0));
    } 
  else{
     fMultiGrid->m_StopAfterSecEntry = 0;
     fStopAfterSecEntry->SetNumber( fMultiGrid->m_StopAfterSecEntry);
     fStopAfterSecEntry->SetState(EButtonState(0));
    }

  if(fStopAfterFileCB->GetState() == 1){
     //fMultiGrid->m_StopAfterFileEntry = 1;
     fStopAfterFileEntry->SetState(EButtonState(1));
     fStopAfterFileEntry->SetNumber( fMultiGrid->m_StopAfterFileEntry );

     fMultiGrid->m_StopAfterSecEntry = 0;
     fStopAfterSecEntry->SetNumber(0);
     fStopAfterSecEntry->SetState(EButtonState(0));
     fStopAfterSecCB->SetState(EButtonState(0));

     fMultiGrid->m_StopAfterEventsEntry = 0;
     fStopAfterEventsEntry->SetNumber(0);
     fStopAfterEventsEntry->SetState(EButtonState(0));
     fStopAfterEventsCB->SetState(EButtonState(0));
    } 
  else{
     fMultiGrid->m_StopAfterFileEntry = 0;
     fStopAfterFileEntry->SetState(EButtonState(0));
     fStopAfterFileEntry->SetNumber( fMultiGrid->m_StopAfterFileEntry );
    }

  if(fStopAfterEventsCB->GetState() == 1){
     //fMultiGrid->m_StopAfterEventsEntry = 1;
     fStopAfterEventsEntry->SetState(EButtonState(1));
     fStopAfterEventsEntry->SetNumber( fMultiGrid->m_StopAfterEventsEntry );

     fMultiGrid->m_StopAfterSecEntry = 0;
     fStopAfterSecEntry->SetNumber(0);
     fStopAfterSecEntry->SetState(EButtonState(0));
     fStopAfterSecCB->SetState(EButtonState(0));

     fMultiGrid->m_StopAfterFileEntry = 0;
     fStopAfterFileEntry->SetNumber(0);
     fStopAfterFileEntry->SetState(EButtonState(0));
     fStopAfterFileCB->SetState(EButtonState(0));

    } 
  else{
     fMultiGrid->m_StopAfterEventsEntry = 0;
     fStopAfterEventsEntry->SetState(EButtonState(0));
     fStopAfterEventsEntry->SetNumber(fMultiGrid->m_StopAfterEventsEntry );
    }

  if(fMultiGrid->m_Status == 0){ // start program, waiting for user reaction, program is in STOP MODE
    fRunButton->SetEnabled(kTRUE);
    fAcqButton->SetEnabled(kTRUE);
    fRunButton->SetText("  RUN  ");
    fAcqButton->SetText("Start Acq");

    //fAcqTimeEntryField->SetText("00:00:00");
    fAcqEventEntryField->SetNumber(    fMultiGrid->m_AcqStatusEntry2 );
    fAcqEventSecEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry3 );
    fAcqDataTraEntryField->SetNumber(  fMultiGrid->m_AcqStatusEntry4 );

    }
  else if (fMultiGrid->m_Status == 1){ // RUN MODE
    fRunButton->SetEnabled(kTRUE);
    fAcqButton->SetEnabled(kTRUE);
    fRunButton->SetText("STOP");
    fAcqButton->SetText("Start Acq");

    fMultiGrid->ResetModules();
    fMultiGrid->m_AcqStatusEntry2 = 0;
    fMultiGrid->m_AcqStatusEntry3 = 0;
    fMultiGrid->m_AcqStatusEntry4 = 0;

    fAcqEventEntryField->SetNumber(    fMultiGrid->m_AcqStatusEntry2 );
    fAcqEventSecEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry3 );
    fAcqDataTraEntryField->SetNumber(  fMultiGrid->m_AcqStatusEntry4 );
    fMultiGrid->m_ElapsedAcqTime = 0;
    fMultiGrid->m_StartAcqTime.Set();
    }
  else{  				// ACQ MODE
    fRunButton->SetEnabled(kFALSE);
    fAcqButton->SetEnabled(kTRUE);
    fRunButton->SetText("STOP");
    fAcqButton->SetText("Stop Acq");

    fMultiGrid->ResetModules();
    fMultiGrid->m_AcqStatusEntry2 = 0;
    fMultiGrid->m_AcqStatusEntry3 = 0;
    fMultiGrid->m_AcqStatusEntry4 = 0;

    fAcqEventEntryField->SetNumber(    fMultiGrid->m_AcqStatusEntry2 );
    fAcqEventSecEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry3 );
    fAcqDataTraEntryField->SetNumber(  fMultiGrid->m_AcqStatusEntry4 );
    fMultiGrid->m_ElapsedAcqTime = 0;
    fMultiGrid->m_StartAcqTime.Set();
    }

}
//-----------------------------------------------------------------------------
 Bool_t DGMultiGrid::HandleTimer(TTimer *tim) {
  
  fTick++;

  //..................
  // stops acquisition after certain number of events set by the user in GUI
  if(fMultiGrid->m_StopAfterEventsEntry){
     if( fMultiGrid->m_AcqStatusEntry2 >= fMultiGrid->m_StopAfterEventsEntry ){
         ProcessMessage(259, eRunButton, 0); // simulate stop button click...
         RefreshStatus();
         return kFALSE;
       }
     }

  //..................
  // stops acquisition after certain number of seconds set by the user in GUI
  if(fMultiGrid->m_StopAfterSecEntry){
     std::cout << fMultiGrid->m_ElapsedAcqTime << " " << (UInt_t)fMultiGrid->m_StopAfterSecEntry << std::endl;
     if( fMultiGrid->m_ElapsedAcqTime >= (UInt_t)fMultiGrid->m_StopAfterSecEntry ){
         ProcessMessage(259, eRunButton, 0); // simulate stop button click...
         RefreshStatus();
         return kFALSE;
       }
     }


  //..................
  // stops acquisition after certain number of saved files set by the user in GUI
 if(fMultiGrid->m_StopAfterFileEntry){
     if( fMultiGrid->m_NrOfSavedFiles >= fMultiGrid->m_StopAfterFileEntry ){
         ProcessMessage(259, eRunButton, 0); // simulate stop button click...
         RefreshStatus();
         return kFALSE;
       }
     }

/*
  if(fMultiGrid->m_StopAllAcquisition){
     ProcessMessage(259, 7, 0); // simulate stop button click...
     RefreshStatus();
     return kFALSE;
     }
*/

  //..................
  // transfer data from VME modules, one after the other according to the fModuleList, if neessary display and/or save
  
  if( flag == 0 ){
      flag = 1;
      fMultiGrid->ReadVME();
      fMultiGrid->m_AcqStatusEntry2 = fMultiGrid->fDMadc32->GetNrEvents(); 
     
      //fMultiGrid->ShowData(fGDisplay);
 
         
      //std::cout << fMultiGrid->m_DataSave << std::endl;
      if( fMultiGrid->m_DataSave )
          fMultiGrid->DataSave();

      // this is not yet implemented, will be in the next advanced version
      //fMultiGrid->BuildEvent();
      //fMultiGrid->SaveiEvent();
      //fMultiGrid->OnlineEvent();

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

 void DGMultiGrid::RefreshStatus(){
     fMultiGrid->m_EndAcqTime.Set();
     UInt_t m_TimeNow = fMultiGrid->m_EndAcqTime.Convert();
     fMultiGrid->m_ElapsedAcqTime = m_TimeNow - fMultiGrid->m_StartAcqTime.Convert();

     TDatime acqtime(fMultiGrid->m_ElapsedAcqTime);

     // Update acq time
     char time[256];
     sprintf(time, "%02d:%02d:%02d\n", acqtime.GetHour()-1, acqtime.GetMinute(), acqtime.GetSecond());
     fAcqStatusEntry1->SetText(time);

     // update nr of events
     fMultiGrid->m_AcqStatusEntry2 = fMultiGrid->fDMadc32->GetNrEvents();
     fAcqEventEntryField->SetNumber( fMultiGrid->m_AcqStatusEntry2 );

     // update event/sec
     gCurrentTime = gGetLongTime();
     gElapsedTime = gCurrentTime - gPrevRateTime;

     fAcqEventSecEntryField->SetNumber( (float)(fMultiGrid->m_AcqStatusEntry2 - fMultiGrid->m_PrevEvent)*1000.0f / (float)gElapsedTime );
     fMultiGrid->m_PrevEvent = fMultiGrid->m_AcqStatusEntry2;
     fMultiGrid->m_AcqStatusEntry3 = fMultiGrid->fDMadc32->GetDataSize();

     // update transfer rate
     fAcqDataTraEntryField->SetNumber( (float)( fMultiGrid->m_AcqStatusEntry3 - fMultiGrid->m_PrevSize) / (float)gElapsedTime * 1048.57f);
     fMultiGrid->m_PrevSize = fMultiGrid->m_AcqStatusEntry3;
    
     gPrevSize = fMultiGrid->m_AcqStatusEntry3;
     gPrevRateTime = gCurrentTime;

      
     // update all trigger (accepted + rejected)
     // update number of files
     // update nr of acquisition time (in second)
     //

     fGDisplay->Refresh();
}
//-----------------------------------------------------------------------------
 void DGMultiGrid::DoValueSet(){

 TGNumberEntry *te = (TGNumberEntry *) gTQSender;
 Int_t id = te->WidgetId();

   switch (id) {
     case eStopAfterSecEntry:
        fMultiGrid->m_StopAfterSecEntry = (Int_t)te->GetNumber();
     break;

     case eStopAfterFileEntry:
        fMultiGrid->m_StopAfterFileEntry = (Int_t)te->GetNumber();
     break;

     case eStopAfterEventsEntry:
        fMultiGrid->m_StopAfterEventsEntry = (Int_t)te->GetNumber();
     break;

     case eSaveFileTimeEntry:
        fMultiGrid->m_SaveFileTimeEntry = (Int_t)te->GetNumber();
     break;

     case eSaveFileSizeEntry:
        fMultiGrid->m_SaveFileSizeEntry = (Int_t)te->GetNumber();
     break;
     default:
     break;
   }

 }
//-----------------------------------------------------------------------------
 Bool_t DGMultiGrid::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

  std::cout << "msg " << msg << ", parm1 "<< parm1 << ", parm2 "<< parm2 << std::endl;

  TGFileInfo fi;
  TGFileInfo dfi; 
  const char *filetypes[] = { "MultiGrid setup files", "*.tri", 
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
      case eLoadSetup:
	// load setup
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen,&fi);
	if (fi.fFilename) fMultiGrid->Load(fi.fFilename);
	*fLog << "load setup from " << fi.fFilename;
	std::cout << "load setup from " << fi.fFilename;
	break;
      case eSaveSetup:
	// save setup
	new TGFileDialog(fClient->GetRoot(), this, kFDSave,&fi);
	if (fi.fFilename) fMultiGrid->Save(fi.fFilename);
	*fLog << "save setup to   " << fi.fFilename;
	std::cout << "save setup to   " << fi.fFilename;
	break;
      case eSaveData:
	// save histogram data
	//new TGFileDialog(fClient->GetRoot(), this, kFDSave,&fi);
	//if (fi.fFilename) fMultiGrid->SaveData(fi.fFilename);
	*fLog << "save data to   " << fi.fFilename;
	std::cout << "save data to   " << fi.fFilename;
	break;
      case eExit:
	// exit
	*fLog << "exit\n";
	std::cout << "exit\n";
	exit(0);
	break;
      case eInitializeSetup:
	// Initialize setup
	*fLog << "Initialize setup\n";
	std::cout << "Initialize setup\n";
	exit(0);
	break;
      case eMesytecAdc32:
	new DGMadc32(fMultiGrid->fDMadc32);
	*fLog << "debug Mesytec GUI\n";
	std::cout << "debug Mesytec GUI\n";
	break;
      case eCAENBridgeV1718:
	new DGV1718(fMultiGrid->fDV1718);
	*fLog << "debug CAEN Bridge\n";
	std::cout << "debug CAEN Bridge GUI\n";
	break;
      case eHelp:
	new DGHelp((char*)"./help.hlp");
	*fLog << "monitor help";
	std::cout << "monitor help";
	break;
      default:
	break;
      }
    case kCM_CHECKBUTTON:
      switch (parm1) {
        case eStopAfterSecCB:
          fStopAfterFileCB->SetState(EButtonState(0));
          fStopAfterEventsCB->SetState(EButtonState(0));
	  *fLog << "Set stop Acq parameter after [...] sec\n "; 
	  std::cout << "Set stop Acq parameter after [...] sec\n ";
        break;
        case eStopAfterFileCB:
          fStopAfterSecCB->SetState(EButtonState(0));
          fStopAfterEventsCB->SetState(EButtonState(0));
	  *fLog << "Set stop Acq parameter after [...] files\n " ;
	  std::cout << "Set stop Acq parameter after [...] files \n";
        break;
        case eStopAfterEventsCB:
          fStopAfterSecCB->SetState(EButtonState(0));
          fStopAfterFileCB->SetState(EButtonState(0));
	  *fLog << "Set stop Acq parameter after [...] events\n ";
	  std::cout << "Set stop Acq parameter after [...] events \n";
        break;
        case eSaveFileSizeCB:
          fSaveSizeEntry->SetState(fSaveSizeCB->GetState());
	  *fLog << "Set SaveFileSize parameter after [...] file size\n ";
	  std::cout << "Set SaveFileSize parameter after [...] file size\n ";
        break;
        case eSaveFileTimeCB:
          fSaveTimeEntry->SetState(fSaveTimeCB->GetState());
	  *fLog << "Set SaveFileTime parameter after [...] file number\n";
	  std::cout << "Set SaveFileTime parameter after [...] file number\n";
        break;
        default:
        break;

      }
    case kCM_BUTTON:
      std::cout << "kCMBUTTON:" << kCM_BUTTON << " parm1: "<< parm1 << " parm2: " << parm1 << std::endl;
      switch (parm1){
        case eRunButton:
           fMultiGrid->m_DataSave = 0;
           if(fMultiGrid->m_Status == 0){
              fMultiGrid->m_Status = 1;
              fTick = 0;
              fTimer->TurnOn();
	      *fLog << "program RUN ";
	      std::cout << "program RUN " << fMultiGrid->m_Status << std::endl;
              }
           else{              
              fMultiGrid->m_Status = 0;
              fTimer->TurnOff();
              fTick = 0;
	      *fLog << "program STOP ";
	      std::cout << "program STOP " << fMultiGrid->m_Status << std::endl;
             }

        break;

        case eAcqButton:

           if(fMultiGrid->m_Status == 0 || fMultiGrid->m_Status == 1){
	      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &dfi);
	      if (dfi.fFilename) fMultiGrid->SetFileName(dfi.fFilename);
              fMultiGrid->m_NrOfSavedFiles = 0;
              fMultiGrid->m_DataSave = 1;
              fMultiGrid->m_Status = 2;
              fTick = 0;
              fTimer->TurnOn();
	      *fLog << "Acqisition START ";
	      std::cout << "Acqisition START " << std::endl;
              }
           else{ 
              fMultiGrid->m_DataSave = 0;
              fMultiGrid->m_Status = 0;
              fTimer->TurnOff();
              fTick = 0;
	      *fLog << "Acqisition STOP ";
	      std::cout << "Acqisition STOP " << std::endl;
              } 
        break;

        case eDisplayButton:
           if(fMultiGrid->m_Display == 1) { fMultiGrid->m_Display = 0; fDisplayButton->SetText("DISPLAY OFF");}
           else { fMultiGrid->m_Display = 1; fDisplayButton->SetText("DISPLAY ON");}
	   *fLog << "Display ON/OFF";
	   std::cout << "Display ON/OFF" << std::endl;
        break;

        case eResetButton:
            Int_t ret;
            new TGMsgBox(gClient->GetRoot(), this, " WARNING !!! ", 
                                "All histograms will be cleaned, and all data on histograms will be lost", 0, 0x41, &ret);
            if( ret )  
              fGDisplay->Reset();
 
	    *fLog << "Reset all histograms ";
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
    break;
  }
  Refresh();
  *fLog << std::endl;
  return kTRUE;

}
