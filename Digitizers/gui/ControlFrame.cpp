#include <TApplication.h>
#include <TObjArray.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TGTab.h>
#include <TGResourcePool.h>

#include "ControlFrame.h"
#include "AManager.h"
#include "../digitizer/CAEN5740/CAEN5740Dlg.h"

ClassImp(ControlFrame)


//=================================================================

ControlFrame::ControlFrame(const TGFrame *main) : TGTransientFrame(gClient->GetRoot(), main, 60, 80)
{

    
    aFrame = (MainFrame*) main;
    aManager = &AManager::GetInstance();
    cout << "[ControlFrame::ControlFrame] nr of digitizers = " << aManager->GetNrDigitizers() << endl;
    aNrTabs=1;
/*
    for(int i = 0; i < aManager->GetNrDigitizers(); i++)
       {
        //cout << "[ControlFrame::ControlFrame] " << (aManager->GetDigitizer(i))->GetName() << endl;
	if((aManager->GetDigitizer(i))->IsEnabled()) aNrTabs++;
       }
    cout << "[ControlFrame::ControlFrame] " << aNrTabs << " digitizers are enabled" << endl;
*/
    aNrTabs += aManager->GetNrDigitizers();

    
    SetCleanup(kDeepCleanup);
    Connect("CloseWindow()", "ControlFrame", this, "CloseWindow()");
    SetWindowName("Control Devices");
    SetIconName  ("Control Devices");


    BuildTabs();
    BuildButtonBox();

    MapSubwindows();
    Resize();
    MapWindow();


    aWidth = 950;
    aHeight = 620;

    this->SetWMSize(aWidth, aHeight);
    this->SetWMSizeHints(aWidth, aHeight, aWidth, aHeight, 0, 0);
    this->SetMWMHints(kMWMDecorAll | kMWMDecorResizeH | kMWMDecorMaximize |
                      kMWMDecorMinimize | kMWMDecorMenu,
                      kMWMFuncAll | kMWMFuncResize | kMWMFuncMaximize |
                      kMWMFuncMinimize, kMWMInputModeless);

     this->MapWindow();

    // gClient->WaitFor(this);

}

//========================================================================================

ControlFrame::~ControlFrame() {

 cout << "DEBUG ControlFrame::Destructor" << endl;
 
 SetCleanup(kDeepCleanup);
 Cleanup();
 aManager->ConfigureFrame(kFALSE);
}

//========================================================================================
void ControlFrame::BuildTabs()
{

   aManager = &AManager::GetInstance();
   tCF = new TGCompositeFrame* [aNrTabs];

   fTab = new TGTab(this, 700, 500);
   TGLayoutHints* fTLXY = new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY, 2, 2, 2, 2);

   tCF[0] = fTab->AddTab("General Settings");
   //tCF[0]->SetLayoutBroken(kTRUE);     
   BuildWindow(tCF[0]);


   for(int i = 1; i < aNrTabs; i++) {
        #ifdef DEBUG
        cout << "DEBUG [ControlFrame::BuildTabs() Create TAB] " << aManager->GetDigitizer(i-1)->GetName() << endl;
         #endif
      	tCF[i] = fTab->AddTab(aManager->GetDigitizer(i-1)->GetName()); //i+1
      	//tCF[i]->SetLayoutBroken(kTRUE);                              //i+1

        //dev[i-1] = new CAEN5740Dlg(aManager->GetDigitizer(i-1), tCF[i]);
   
  	aManager->GetDigitizer(i-1)->BuildGui(tCF[i]);              //i+1
   }

   // set active tab to tab 0 (counts form 0)
   fTab->SetTab(0);

   fTab->Resize();

   AddFrame(fTab, fTLXY);

}

//========================================================================================
void ControlFrame::BuildWindow(TGCompositeFrame* fMainFrame) {

   fMainFrame->SetLayoutManager(new TGHorizontalLayout(fMainFrame));

   fTimeOutFrame = new TGGroupFrame(fMainFrame,"Software Timeout", kVerticalFrame);
     fCBTimeOut = new TGCheckButton(fTimeOutFrame,"Timeout [ms]",eTIMEOUTCHECKB);
     //fCBTimeOut->SetTextJustify(36);
     //fCBTimeOut->SetMargins(0,0,0,0);
     //fCBTimeOut->SetWrapLength(-1);
     fCBTimeOut->Connect("Toggled(Bool_t)", "ControlFrame", this, "DoEnable(Bool_t)");
     fTimeOutFrame->AddFrame(fCBTimeOut, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
     fEntryTimeOut = new TGNumberEntry(fTimeOutFrame, aManager->GetTimeout(),11, eTIMEOUTENTRY,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 0);
     if(aManager->GetTimeout()){
        fCBTimeOut->SetState(EButtonState(1));
        fEntryTimeOut->SetState( EButtonState(kTRUE) );
       }
     else {
        fCBTimeOut->SetState(EButtonState(0));
        fEntryTimeOut->SetState( EButtonState(kFALSE) );
       }
     fEntryTimeOut->Connect("ValueSet(Long_t)", "ControlFrame", this, "DoValueSet()");
     (fEntryTimeOut->GetNumberEntry())->Connect("ReturnPressed()", "ControlFrame", this, "DoValueSet()");

   fTimeOutFrame->AddFrame(fEntryTimeOut, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,58));

   //fTimeOutFrame->SetLayoutManager(new TGVerticalLayout(fTimeOutFrame));
   //fTimeOutFrame->MoveResize(5,10,132,80);
   fMainFrame->AddFrame(fTimeOutFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
 

   // "fLoopFrame" group frame
   fLoopFrame = new TGGroupFrame(fMainFrame,"Stop ACQ after", kVerticalFrame);
   fMainFrame->AddFrame(fLoopFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
     
    fCBMaxEvents = new TGCheckButton(fLoopFrame,"Max Events",eEVENTS);
    fLoopFrame->AddFrame(fCBMaxEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fCBMaxEvents->Connect("Toggled(Bool_t)", "ControlFrame", this, "DoEnable(Bool_t)");

    fEntryMaxEvents = new TGNumberEntry(fLoopFrame, aManager->GetMaxEvents(),11,eEVENTS,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);
    if(aManager->GetMaxEvents()){
       fCBMaxEvents->SetState(EButtonState(1));
       fEntryMaxEvents->SetState( EButtonState(kTRUE) );
      }  
    else {
       fCBMaxEvents->SetState(EButtonState(0));
       fEntryMaxEvents->SetState( EButtonState(kFALSE) );
      }  
   fEntryMaxEvents->Connect("ValueSet(Long_t)", "ControlFrame", this, "DoValueSet()");
   (fEntryMaxEvents->GetNumberEntry())->Connect("ReturnPressed()", "ControlFrame", this, "DoValueSet()");
   fLoopFrame->AddFrame(fEntryMaxEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   fCBMaxFiles = new TGCheckButton(fLoopFrame,"Max Files", eFILES);
   fCBMaxFiles->SetTextJustify(36);
   fCBMaxFiles->SetMargins(0,0,0,0);
   fCBMaxFiles->SetWrapLength(-1);
   fCBMaxFiles->Connect("Toggled(Bool_t)", "ControlFrame", this, "DoEnable(Bool_t)");
   fLoopFrame->AddFrame(fCBMaxFiles, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryMaxFiles = new TGNumberEntry(fLoopFrame, aManager->GetFileNr(),11,eFILES,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);

   if(aManager->GetFileNr()){
      fCBMaxFiles->SetState(EButtonState(1));
      fEntryMaxFiles->SetState( EButtonState(kTRUE) );
     } 
   else {
      fCBMaxFiles->SetState(EButtonState(0));
      fEntryMaxFiles->SetState( EButtonState(kFALSE) );
     } 

   fEntryMaxFiles->Connect("ValueSet(Long_t)", "ControlFrame", this, "DoValueSet()");
   (fEntryMaxFiles->GetNumberEntry())->Connect("ReturnPressed()", "ControlFrame", this, "DoValueSet()");
   fLoopFrame->AddFrame(fEntryMaxFiles, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,11));



   // "fFileFrame" group frame
   fFileFrame = new TGGroupFrame(fMainFrame,"New File after", kVerticalFrame);
   fMainFrame->AddFrame(fFileFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   fFileFrame->AddFrame(new TGLabel(fFileFrame, new TGString("File Size [MB] ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));

   fEntryFileSize = new TGNumberEntry(fFileFrame, aManager->GetFileSize(),11,eFILESIZE,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);
   fEntryFileSize->Connect("ValueSet(Long_t)", "ControlFrame", this, "DoValueSet()");
   (fEntryFileSize->GetNumberEntry())->Connect("ReturnPressed()", "ControlFrame", this, "DoValueSet()");
   fFileFrame->AddFrame(fEntryFileSize, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   fFileFrame->AddFrame(new TGLabel(fFileFrame, new TGString("OR")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
   fFileFrame->AddFrame(new TGLabel(fFileFrame, new TGString("after time in [sec] ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
   fEntryFileTime = new TGNumberEntry(fFileFrame, aManager->GetFileTime(),11,eFILETIME,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);
   fEntryFileTime->Connect("ValueSet(Long_t)", "ControlFrame", this, "DoValueSet()");
   (fEntryFileTime->GetNumberEntry())->Connect("ReturnPressed()", "ControlFrame", this, "DoValueSet()");
   fFileFrame->AddFrame(fEntryFileTime, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));




 //  fFileFrame->SetLayoutManager(new TGVerticalLayout(fFileFrame));
 //  fFileFrame->MoveResize(150, 10, 152,184);
 //  fMainFrame->AddFrame(fFileFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


}
//========================================================================================

void ControlFrame::BuildButtonBox()
{

    // Create a frame to hold the buttons.
    // At the moment is only one button, but could be more....
    cout << "[ControlFrame::BuildButtonBox]" << endl;
    TGCompositeFrame *ButtonFrame = new TGCompositeFrame( this, 600, 20, kHorizontalFrame);

    TGLayoutHints* fL2 = new TGLayoutHints(kLHintsBottom | kLHintsRight, 0, 0, 5, 5);
    fButtonClose = new TGTextButton( ButtonFrame, "  &Close  ");
    fButtonClose->Connect("Clicked()", "ControlFrame", this, "DoClose()");
    ButtonFrame->AddFrame( fButtonClose, fL2);

    ButtonFrame->Resize();
    AddFrame(ButtonFrame, new TGLayoutHints( kLHintsExpandX|kLHintsLeft, 2, 2, 2, 2));
    cout << "[ControlFrame::BuildButtonBox] end" << endl;
}

//========================================================================================
void ControlFrame::DoEnable(Bool_t a){

 TGCheckButton *te = (TGCheckButton *) gTQSender;
 Int_t id = te->WidgetId();

 cout << "DEBUG [ControlFrame::DoEnable] widget id = " << id << endl;

 switch (id){
   case eTIMEOUTCHECKB:
     cout << "DEBUG [ControlFrame::DoEnable] eTIMEOUTCHECKB, state =  " << a << endl;
     fCBTimeOut->SetState(EButtonState(a));
     if(!a) {
       fEntryTimeOut->SetNumber(0);  
       aManager->SetTimeout(0);
       }      
     fEntryTimeOut->SetState( EButtonState(a) );
   break;

   case eEVENTS:
     fCBMaxEvents->SetState(EButtonState(a));
     fEntryMaxEvents->SetState( EButtonState(a) );
     if(!a) {
       fEntryMaxEvents->SetNumber(0);
       aManager->SetMaxEvents(0);
       }       
     else {
      fEntryMaxEvents->SetNumber(1);  
      aManager->SetMaxEvents(1);
      fEntryMaxFiles->SetNumber(0); //
      aManager->SetFileNr(0);     //
     fCBMaxFiles->SetState(EButtonState(0));
     fEntryMaxFiles->SetState( EButtonState(0) );
      }
   break;

   case eFILES:
     fCBMaxFiles->SetState(EButtonState(a));
     fEntryMaxFiles->SetState( EButtonState(a) );
     if(!a) {
       fEntryMaxFiles->SetNumber(0);
       aManager->SetFileNr(0);
       }  
     else {
       fEntryMaxFiles->SetNumber(1);  
       aManager->SetFileNr(1);
       fEntryMaxEvents->SetNumber(0); //
       aManager->SetMaxEvents(0);      //
     fCBMaxEvents->SetState(EButtonState(0));
     fEntryMaxEvents->SetState( EButtonState(0) );
       }
   break;

   default:
   break;
  
  }


}

//========================================================================================
void ControlFrame::DoClose()
{
/*
   TCanvas *myc = aManager->GetCanvas();
   Int_t aRange = 100;
   Int_t aSample = 128;


   for(int i = 0; i < aManager->GetNrDigitizers(); i++){
       for(int j = 0; j < aManager->GetDigitizer(i)->GetNrOfChannels(); j++){
           if(aManager->GetDigitizer(i)->GetInputRange(j) > aRange)
              aRange = aManager->GetDigitizer(i)->GetInputRange(j);
           if(aManager->GetDigitizer(i)->GetNrOfSamples() > aSample)
              aSample = aManager->GetDigitizer(i)->GetNrOfSamples();
           }
       }
  
  
   TH1F* fHist = myc->DrawFrame(0, -aRange*1.1, aSample, aRange*1.1, "TRACKS");
   myc->Draw();
*/
/*
   for(int i = 0; i < aManager->GetNrDigitizers(); i++){
      cout << "+====== Name:\t"  << aManager->GetDigitizer(i)->GetName()        << "\t================+" << endl;
      cout << "|       Memory:\t"<< aManager->GetDigitizer(i)->GetNrOfSamples() << "\t\t        |" << endl; 
      for(int j = 0; j < aManager->GetDigitizer(i)->GetNrOfChannels(); j++){ 
          cout << "|       +++Channel:\t"<< j << " " << aManager->GetDigitizer(i)->GetActiveChannel(j) 
                                                                                << "\t        |" << endl; 
          cout << "|          Range:\t"<< j << " " << aManager->GetDigitizer(i)->GetInputRange(j) 
                                                                                << "\t        |" << endl; 
          cout << "|          Offset:\t"<< j << " " << aManager->GetDigitizer(i)->GetOffset(j) 
                                                                                << "\t        |" << endl; 
         }
      cout << "+=======================================+" << endl;

      }
*/

  SendCloseMessage();
}

//========================================================================================
void ControlFrame::Reconfigure(){

Int_t a;
//---
   if((a = aManager->GetTimeout())){
       cout << "[DEBUG ControlFrame::Reconfigure]  a = "<< a << endl;  
       fEntryTimeOut->SetNumber(a);
       fCBTimeOut->SetState(EButtonState(1));
       fEntryTimeOut->SetState( EButtonState(kTRUE) );
     }
   else {
       fCBTimeOut->SetState(EButtonState(0));
       fEntryTimeOut->SetState( EButtonState(kFALSE) );
     }

//---
   if( (a = aManager->GetMaxEvents()) ){
      fEntryMaxEvents->SetNumber(a);
      fCBMaxEvents->SetState(EButtonState(1));
      fEntryMaxEvents->SetState( EButtonState(kTRUE) );
     } 
   else {
      fCBMaxEvents->SetState(EButtonState(0));
      fEntryMaxEvents->SetState( EButtonState(kFALSE) );
     } 
//---
   if( (a = aManager->GetFileNr()) ){
      fEntryMaxFiles->SetNumber(a);
      fCBMaxFiles->SetState(EButtonState(1));
      fEntryMaxFiles->SetState( EButtonState(kTRUE) );
     } 
   else {
      fCBMaxFiles->SetState(EButtonState(0));
      fEntryMaxFiles->SetState( EButtonState(kFALSE) );
     } 

//---
   fEntryFileSize->SetNumber(aManager->GetFileSize());
   fEntryFileTime->SetNumber(aManager->GetFileTime());

//---
/*
   gClient->NeedRedraw(fCBTimeOut);
   gClient->NeedRedraw(fEntryTimeOut);
   gClient->NeedRedraw(fEntryMaxEvents);
   gClient->NeedRedraw(fCBMaxEvents);
   gClient->NeedRedraw(fCBMaxFiles);
   gClient->NeedRedraw(fEntryMaxFiles);
   gClient->NeedRedraw(fEntryFileSize);
*/
}

//========================================================================================
void ControlFrame::DoValueSet(){

TGNumberEntry *te = (TGNumberEntry *) gTQSender;
 Int_t id = te->WidgetId();

   switch (id) {
      case eTIMEOUTENTRY:
         aManager->SetTimeout((Int_t)te->GetNumber());
         cout << "DEBUG [ControlFrame::DoValueSet] SoftTimeout = " << aManager->GetTimeout() << endl;
      break;

      case eEVENTS:
         aManager->SetMaxEvents((Int_t)te->GetNumber());
      break;

      case eFILES:
         aManager->SetMaxFiles((Int_t)te->GetNumber());
      break;

      case eFILESIZE:
         aManager->SetFileSize((Int_t)te->GetNumber());
      break;

      case eFILETIME:
         aManager->SetFileTime((Int_t)te->GetNumber());
      break;

      default:
      break;
      }

}

//========================================================================================
void ControlFrame::DisconnectAll(){

   fCBTimeOut->Disconnect("Toggled(Bool_t)");
   fEntryTimeOut->Disconnect("ValueSet(Long_t)");
   (fEntryTimeOut->GetNumberEntry())->Disconnect("ReturnPressed()");
   fEntryMaxEvents->Disconnect("ValueSet(Long_t)");
   (fEntryMaxEvents->GetNumberEntry())->Disconnect("ReturnPressed()");
   fCBMaxEvents->Disconnect("Toggled(Bool_t)");
   fCBMaxFiles->Disconnect("Toggled(Bool_t)");
   fEntryMaxFiles->Disconnect("ValueSet(Long_t)");
   (fEntryMaxFiles->GetNumberEntry())->Disconnect("ReturnPressed()");
   fEntryFileSize->Disconnect("ValueSet(Long_t)");
   (fEntryFileSize->GetNumberEntry())->Disconnect("ReturnPressed()");
   fEntryFileTime->Disconnect("ValueSet(Long_t)");
   (fEntryFileTime->GetNumberEntry())->Disconnect("ReturnPressed()");
}



