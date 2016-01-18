#include <vector>
#include <iostream>
#include <fstream>

#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGFileDialog.h>
#include <TROOT.h>
#include <TMath.h>
#include <TRandom.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TGButton.h>
#include <TG3DLine.h>
#include <TGFrame.h>
#include <TGFont.h>
#include <TGMsgBox.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTab.h>
#include <TGNumberEntry.h>

#include <TGraph.h>
#include <TMultiGraph.h>

#include "MainFrame.h"
#include "ADataSave.h"
#include "AManager.h"
#include "Digitizer.h"
#include "ADisplay.h"
#include "ARingBuffer.h"
#include "AHelp.h"


extern int gDEBUG_MAIN;

using namespace std;


ClassImp(MainFrame)


//===================================================================================================
MainFrame::MainFrame(const TGWindow *p, UInt_t width, UInt_t height) : TGMainFrame(p, width, height) {
/* Contructor of the Main Frame
 */

  #ifdef DEBUG
    if(gDEBUG_MAIN > 0) cout << "DEBUG [MainFrame::MainFrame] Create Main Window" << endl;
  #endif

   aIsUpdate	= kFALSE;
   aManager	= &AManager::GetInstance();
   mState	= sSTOP;
   aHelpFrame	= 0;
   aDisplay	= 0;
   
   mNrTabs	= 1;
   SetCleanup(kDeepCleanup);

 /* Set the style of the Main Frame, Colors, Lines, Borders, etc
  */

 gStyle->SetOptDate(0);
 gStyle->SetOptStat(0);
 gStyle->SetOptTitle(kFALSE);
 gStyle->SetTitleFillColor(4);
 gStyle->SetTitleBorderSize(1);
 gStyle->SetTitleBorderSize(1);
 gStyle->SetOptFit(0);
 gStyle->SetFrameFillColor(1);
 gStyle->SetFrameBorderSize(1);
 gStyle->SetFrameBorderMode(0);
 gStyle->SetCanvasColor(1);
 gStyle->SetCanvasBorderSize(1);
 gStyle->SetCanvasBorderMode(0);
 gStyle->SetPadColor(1);
 gStyle->SetPadBorderSize(2);
 gStyle->SetPadBorderMode(0);
 gStyle->SetLabelSize(0.04,"X");
 gStyle->SetLabelSize(0.04,"Y");
 gStyle->SetAxisColor(32, "x");
 gStyle->SetAxisColor(32, "y");
 gStyle->SetLabelColor(32, "x");
 gStyle->SetLabelColor(32, "y");
 gStyle->SetPadGridX(1);
 gStyle->SetPadGridY(1);
 gStyle->SetPadTickX(1);
 gStyle->SetPadTickY(1);
 gStyle->SetPalette(1);
 gStyle->SetNdivisions(-808);
 gStyle->cd();





   //Style_ACQ();
   CreateMenuBar();
   CreateToolBox();
   CreateCommonFrame();
   CreateControlDevice();
   Refresh();  
 
   //Connect("CloseWindow()", "MainFrame" , this, "CloseWindow()");
   DontCallClose();
   MapSubwindows();
   HideFrame(fControlDevice);
   fCommonFrame->HideFrame(fControlDisplay);
   Resize(GetDefaultSize());
   SetWindowName("Data Acquisition System");
   MapWindow();
   Move(600,50);

}

//=========================================================================

MainFrame::~MainFrame() {

  #ifdef DEBUG
   if(gDEBUG_MAIN > 0) cout << endl << "DEBUG Destructor MainFrame "<< endl;
  #endif

  aManager->ClearAll();
  if(aHelpFrame) delete aHelpFrame;

  delete aDisplay;

  DeleteWindow();
  gApplication->Terminate(0);

  SetCleanup(kDeepCleanup);
  Cleanup(); // deleting all widgets, layouthints and subframes...
}

//=========================================================================
/*
void MainFrame::CloseWindow() {

  cout << endl;
  cout << "DEBUG ====== Cleaning =======" << endl;
  cout << "DEBUG MainFrame::CloseWindow" << endl;

  aManager->ClearAll();

  Disconnect("CloseWindow()");
  DeleteWindow();
  gApplication->Terminate(0);
}
*/
//=========================================================================

void MainFrame::DisconnectSlots(){
/* Disconnect all slots
 */

   //this->Disconnect("CloseWindow()");
   //fMenuFile->Disconnect("Activated(Int_t)");
   //fMenuAcq->Disconnect("Activated(Int_t)");
   //fMenuDisplay->Disconnect("Activated(Int_t)");
   //fMenuAnalysis->Disconnect("Activated(Int_t)");
   //fMenuHelp->Disconnect("Activated(Int_t)");
   //fButtonOsci->Disconnect("Clicked()");
   //fButtonConfigureAcq->Disconnect("Clicked()");
   //fButtonAcq->Disconnect("Clicked()");
   //fButtonDisplay->Disconnect("Clicked()");
   //fButtonOnline->Disconnect("Clicked()");
   //fButtonExit->Disconnect("Clicked()");
   //fEcan->GetCanvas()->Disconnect("ProcessedEvent(Int_t, Int_t, Int_t, TObject *)");
}
//=========================================================================

void MainFrame::CreateMenuBar() {

  #ifdef DEBUG
   if(gDEBUG_MAIN > 2) cout << endl << "DEBUG [MainFrame::CreateMenuBar] "<< endl;
  #endif

   fMenuFile = new TGPopupMenu(gClient->GetRoot());
   fMenuFile->AddEntry("E&xit", M_FILE_EXIT);
   fMenuFile->Associate(this);
  
   fMenuView = new TGPopupMenu(gClient->GetRoot());
   fMenuView->AddEntry("View Display", M_VIEW_DISPLAY);
   fMenuView->AddEntry("View Device", M_VIEW_DEVICE);
   fMenuView->Associate(this); 

   fMenuAcq = new TGPopupMenu(gClient->GetRoot());
   fMenuAcq->AddEntry("Read Config File", M_ACQ_READCONFIG);
   fMenuAcq->AddEntry("Save Config File", M_ACQ_SAVECONFIG);
   fMenuAcq->AddSeparator();
   fMenuAcq->AddEntry("Configuration", M_ACQ_CONFIGURE);
   fMenuAcq->AddSeparator();
   fMenuAcq->AddEntry("Start Osci", M_ACQ_STARTOSCI);
   fMenuAcq->AddEntry("Stop Osci", M_ACQ_STOPOSCI);
   fMenuAcq->AddSeparator();
   fMenuAcq->AddEntry("Start Acq", M_ACQ_STARTACQ);
   fMenuAcq->AddEntry("Stop Acq", M_ACQ_STOPACQ);
   fMenuAcq->Associate(this);
   
   fMenuDisplay = new TGPopupMenu(gClient->GetRoot());
   fMenuDisplay->AddEntry("Display ON", M_DISP_ON);
   fMenuDisplay->AddEntry("Display OFF", M_DISP_OFF);
   fMenuDisplay->Associate(this);

   
   fMenuAnalysis = new TGPopupMenu(gClient->GetRoot());
   fMenuAnalysis->AddEntry("Online On", M_ONLINE_ON);
   fMenuAnalysis->AddEntry("Online Off", M_ONLINE_OFF);
   fMenuAnalysis->Associate(this);
   

   fMenuHelp = new TGPopupMenu(gClient->GetRoot());
   fMenuHelp->AddEntry("Help", M_HELP_CONTENTS);
   fMenuHelp->Associate(this);
 
   fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
   fMenuBar->AddPopup("&File", fMenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
   fMenuBar->AddPopup("&View", fMenuView, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
   fMenuBar->AddPopup("&Acquisition", fMenuAcq, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
   fMenuBar->AddPopup("&Display", fMenuDisplay, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
   //fMenuBar->AddPopup("A&nalysis", fMenuAnalysis, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
   fMenuBar->AddPopup("&Help", fMenuHelp, new TGLayoutHints(kLHintsTop | kLHintsRight));
   AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

}
   
//=========================================================================
void MainFrame::CreateToolBox() {
 
  #ifdef DEBUG
   if(gDEBUG_MAIN > 2) cout << endl << "DEBUG [MainFrame::CreateToolBox] "<< endl;
  #endif

  l1 = new TGHorizontal3DLine(this);
  AddFrame(l1, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

  fButtons = new TGHorizontalFrame(this,1,1,kHorizontalFrame);
      fButtonOsci=new TGPictureButton(fButtons,gClient->GetPicture(aManager->GetWorkingDir()+"/icons/start_osci.xpm"),B_OSCI);
      fButtonOsci->MoveResize(0,0,32,32);   
      fButtonOsci->Associate(this);
      fButtons->AddFrame(fButtonOsci, new TGLayoutHints(kLHintsLeft | kLHintsTop,10,1,1,1));

      fButtonConfigureAcq = new TGPictureButton(fButtons, gClient->GetPicture(aManager->GetWorkingDir()+"/icons/equipment.xpm"), B_CONFIGURE);
      fButtonConfigureAcq->MoveResize(0,0,32,32);   
      fButtonConfigureAcq->Associate(this);
      fButtons->AddFrame(fButtonConfigureAcq, new TGLayoutHints(kLHintsLeft | kLHintsTop,3,1,1,1));


      fButtonAcq = new TGPictureButton(fButtons, gClient->GetPicture(aManager->GetWorkingDir()+"/icons/start_acq.xpm"), B_ACQ);
      fButtonAcq->Associate(this);
      fButtonAcq->MoveResize(0,0,32,32);   
      fButtons->AddFrame(fButtonAcq, new TGLayoutHints(kLHintsLeft | kLHintsTop,3,1,1,1));

      fButtonDisplay = new TGPictureButton(fButtons, gClient->GetPicture(aManager->GetWorkingDir()+"/icons/d_on.xpm"), B_DISPLAY);
      fButtonDisplay->Associate(this);
      fButtonDisplay->MoveResize(0,0,32,32);   
      fButtonDisplay->SetEnabled(kFALSE);
      fButtons->AddFrame(fButtonDisplay, new TGLayoutHints(kLHintsLeft | kLHintsTop,3,1,1,1));

      
      fButtonOnline = new TGPictureButton(fButtons, gClient->GetPicture(aManager->GetWorkingDir()+"/icons/o_on.xpm"), B_ONLINE);
      fButtonOnline->Associate(this);
      fButtonOnline->MoveResize(0,0,32,32);   
      fButtons->AddFrame(fButtonOnline, new TGLayoutHints(kLHintsLeft | kLHintsTop,3,1,1,1));
      

      TGFont *ufont = gClient->GetFont("-*-utopia-bold-r-*-*-24-*-*-*-*-*-*-*");
      TGGC   *uGC; 
      GCValues_t vall949;
      vall949.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
      gClient->GetColorByName("#ff0000",vall949.fForeground);
      gClient->GetColorByName("#c0c0c0",vall949.fBackground);
      vall949.fFillStyle = kFillSolid;
      vall949.fFont = ufont->GetFontHandle();
      vall949.fGraphicsExposures = kFALSE;
      uGC = gClient->GetGC(&vall949, kTRUE);

      fAcqLabel = new TGLabel(fButtons,"\t\t\t\t" ,uGC->GetGC(),ufont->GetFontStruct());
      fButtons->AddFrame(fAcqLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,20,2,7,2));
      fButtonExit = new TGPictureButton(fButtons, gClient->GetPicture(aManager->GetWorkingDir()+"/icons/exit.xpm"), B_EXIT);
      fButtonExit->Associate(this);
      fButtonExit->MoveResize(0,0,32,32);   
      fButtons->AddFrame(fButtonExit, new TGLayoutHints(kLHintsRight | kLHintsTop,1,1,1,1));

   AddFrame(fButtons, new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));

  l2 = new TGHorizontal3DLine(this);
  AddFrame(l2, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

}
 
//=========================================================================

void MainFrame::SetLabelAcq(bool a){
  #ifdef DEBUG
   if(gDEBUG_MAIN > 4) cout << endl << "DEBUG [MainFrame::SetLabelAcq] =  " << a << endl;
  #endif

if(a) fAcqLabel->SetText("+++ Acq running +++");
else  fAcqLabel->SetText("");

}
//=========================================================================

void MainFrame::CreateCommonFrame(){

   #ifdef DEBUG
      if(gDEBUG_MAIN > 2) cout << "DEBUG [MainFrame::CreateCommonFrame] "<< endl;
   #endif

   fCommonFrame = new TGCompositeFrame(this,500,400, kHorizontalFrame);
   fCommonFrame->SetName("MainPanel");

   fEcan = new TRootEmbeddedCanvas("MainCanvas", fCommonFrame, 500, 300);
   fCommonFrame->AddFrame(fEcan, new TGLayoutHints(kLHintsTop | kLHintsLeft |  kLHintsExpandY | kLHintsExpandX, 0, 0, 1, 1) );

   aDisplay = new ADisplay(fEcan->GetCanvas());
   AEvent *aEvent = NULL;
   aDisplay->InitGraphs(aEvent);
   aDisplay->InitThreshold();

   fControlDisplay = new TGCompositeFrame(fCommonFrame, 500, 100, kVerticalFrame);  
   TGTextButton *hhButton = new TGTextButton(fControlDisplay, "DUPAH");
   fControlDisplay->AddFrame(hhButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2) );

   fCommonFrame->AddFrame(fControlDisplay, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandY,1,1,1,1)); 

   l3 = new TGHorizontal3DLine(fControlDisplay);
   fControlDisplay->AddFrame(l3, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));

   AddFrame(fCommonFrame, new  TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY ,1,1,1,1));

} 

//=========================================================================

void MainFrame::CreateControlDevice(){

   #ifdef DEBUG
      if(gDEBUG_MAIN > 2) cout << "DEBUG [MainFrame::CreateControlDevice] "<< endl;
   #endif

   mNrTabs += aManager->GetNrDigitizers();

   fControlDevice = new TGCompositeFrame(this, 500, 100, kVerticalFrame); 
  

   fTab = new TGTab(fControlDevice, 700, 500);
   TGLayoutHints* fTLXY = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0);

   tCF = new TGCompositeFrame* [mNrTabs];
   tCF[0] = fTab->AddTab("General Settings");
   BuildWindow(tCF[0]);


   for(int i = 1; i < mNrTabs; i++) {
     #ifdef DEBUG
        cout << "DEBUG [MainFrame::BuildTabs() Create TAB] " << aManager->GetDigitizer(i-1)->GetName() << endl;
     #endif
     tCF[i] = fTab->AddTab(aManager->GetDigitizer(i-1)->GetName()); //i+1
     aManager->GetDigitizer(i-1)->BuildGui(tCF[i]);              //i+1
     }

   // set active tab to tab 0 (counts form 0)
   fTab->SetTab(0);

   fTab->Resize();

   fControlDevice->AddFrame(fTab, fTLXY);



   //fControlDevice->AddFrame(fDeviceFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2) );
 
   AddFrame(fControlDevice, new  TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2));
}

//========================================================================================
void MainFrame::BuildTabs(){
}

//========================================================================================
void MainFrame::BuildWindow(TGCompositeFrame* fMainFrame) {

   fMainFrame->SetLayoutManager(new TGHorizontalLayout(fMainFrame));

    fTimeOutFrame = new TGGroupFrame(fMainFrame,"Software Timeout", kVerticalFrame);
     fCBTimeOut = new TGCheckButton(fTimeOutFrame,"Timeout [ms]",eTIMEOUTCHECKB);
     fCBTimeOut->Connect("Toggled(Bool_t)", "MainFrame", this, "DoEnable(Bool_t)");
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
     fEntryTimeOut->Connect("ValueSet(Long_t)", "MainFrame", this, "DoValueSet()");
     (fEntryTimeOut->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "DoValueSet()");

    fTimeOutFrame->AddFrame(fEntryTimeOut, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,58));

    fMainFrame->AddFrame(fTimeOutFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    fLoopFrame = new TGGroupFrame(fMainFrame,"Stop ACQ after", kVerticalFrame);
    fMainFrame->AddFrame(fLoopFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    fCBMaxEvents = new TGCheckButton(fLoopFrame,"Max Events",eEVENTSCHECKB);
    fLoopFrame->AddFrame(fCBMaxEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fCBMaxEvents->Connect("Toggled(Bool_t)", "MainFrame", this, "DoEnable(Bool_t)");

    fEntryMaxEvents = new TGNumberEntry(fLoopFrame, aManager->GetMaxEvents(),11,eEVENTSENTRY,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);
    if(aManager->GetMaxEvents()){
       fCBMaxEvents->SetState(EButtonState(1));
       fEntryMaxEvents->SetState( EButtonState(kTRUE) );
      }
    else {
       fCBMaxEvents->SetState(EButtonState(0));
       fEntryMaxEvents->SetState( EButtonState(kFALSE) );
      }
    fEntryMaxEvents->Connect("ValueSet(Long_t)", "MainFrame", this, "DoValueSet()");
    (fEntryMaxEvents->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "DoValueSet()");
    fLoopFrame->AddFrame(fEntryMaxEvents, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    fCBMaxFiles = new TGCheckButton(fLoopFrame,"Max Files", eFILESCHECKB);
    fCBMaxFiles->SetTextJustify(36);
    fCBMaxFiles->SetMargins(0,0,0,0);
    fCBMaxFiles->SetWrapLength(-1);
    fCBMaxFiles->Connect("Toggled(Bool_t)", "MainFrame", this, "DoEnable(Bool_t)");
    fLoopFrame->AddFrame(fCBMaxFiles, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    fEntryMaxFiles = new TGNumberEntry(fLoopFrame, aManager->GetMaxFiles(),11,eFILESENTRY,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);

    if(aManager->GetMaxFiles()){
      fCBMaxFiles->SetState(EButtonState(1));
      fEntryMaxFiles->SetState( EButtonState(kTRUE) );
     }
    else {
      fCBMaxFiles->SetState(EButtonState(0));
      fEntryMaxFiles->SetState( EButtonState(kFALSE) );
     }

    fEntryMaxFiles->Connect("ValueSet(Long_t)", "MainFrame", this, "DoValueSet()");
    (fEntryMaxFiles->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "DoValueSet()");
    fLoopFrame->AddFrame(fEntryMaxFiles, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,11));


    fFileFrame = new TGGroupFrame(fMainFrame,"New File after", kVerticalFrame);
    fMainFrame->AddFrame(fFileFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    fFileFrame->AddFrame(new TGLabel(fFileFrame, new TGString("File Size [MB] ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));

    fEntryFileSize = new TGNumberEntry(fFileFrame, aManager->GetFileSize(),11,eFILESIZE,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);
    fEntryFileSize->Connect("ValueSet(Long_t)", "MainFrame", this, "DoValueSet()");
    (fEntryFileSize->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "DoValueSet()");
    fFileFrame->AddFrame(fEntryFileSize, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

    fFileFrame->AddFrame(new TGLabel(fFileFrame, new TGString("OR")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
    fFileFrame->AddFrame(new TGLabel(fFileFrame, new TGString("after time in [sec] ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
    fEntryFileTime = new TGNumberEntry(fFileFrame, aManager->GetFileTime(),11,eFILETIME,
                   TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMin, 1);
    fEntryFileTime->Connect("ValueSet(Long_t)", "MainFrame", this, "DoValueSet()");
    (fEntryFileTime->GetNumberEntry())->Connect("ReturnPressed()", "MainFrame", this, "DoValueSet()");
    fFileFrame->AddFrame(fEntryFileTime, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


}

//==================================================================
void MainFrame::Refresh() {
   #ifdef DEBUG
      if(gDEBUG_MAIN > 2) cout << "DEBUG [MainFrame::Refresh] state " << mState << endl;
   #endif

  switch( mState ){
	case sSTOP:
                //------ Menu ----------
   		fMenuAcq->EnableEntry(M_ACQ_READCONFIG);
   		fMenuAcq->EnableEntry(M_ACQ_SAVECONFIG);
   		fMenuAcq->EnableEntry(M_ACQ_CONFIGURE);
   		fMenuAcq->EnableEntry(M_ACQ_STARTOSCI);
   		fMenuAcq->DisableEntry(M_ACQ_STOPOSCI);
   		fMenuAcq->EnableEntry(M_ACQ_STARTACQ);
   		fMenuAcq->DisableEntry(M_ACQ_STOPACQ);

   		fMenuDisplay->DisableEntry(M_DISP_OFF);
   		fMenuDisplay->DisableEntry(M_DISP_ON);

   		fMenuAnalysis->DisableEntry(M_ONLINE_OFF);
   		fMenuAnalysis->DisableEntry(M_ONLINE_ON);
                
		//------ Buttons ----------
		fButtonOsci->SetPicture(gClient->GetPicture(aManager->GetWorkingDir()+"/icons/start_osci.xpm"));
         	fButtonOsci->SetEnabled(kTRUE);

         	fButtonConfigureAcq->SetEnabled(kTRUE);
         	fButtonAcq->SetPicture(gClient->GetPicture(aManager->GetWorkingDir()+"/icons/start_acq.xpm"));
         	fButtonAcq->SetEnabled(kTRUE);

         	fButtonDisplay->SetEnabled(kFALSE);
        
         	fButtonOnline->SetEnabled(kFALSE);

	break;
	
	case sOSCI:
                //------ Menu ----------
	   	fMenuAcq->DisableEntry(M_ACQ_READCONFIG);
   		fMenuAcq->DisableEntry(M_ACQ_SAVECONFIG);
   		fMenuAcq->EnableEntry(M_ACQ_CONFIGURE);
   		fMenuAcq->DisableEntry(M_ACQ_STARTOSCI);
   		fMenuAcq->EnableEntry(M_ACQ_STOPACQ);
   		fMenuAcq->EnableEntry(M_ACQ_STARTACQ);
   		fMenuAcq->DisableEntry(M_ACQ_STOPOSCI);

   		fMenuDisplay->DisableEntry(M_DISP_OFF);
   		fMenuDisplay->EnableEntry(M_DISP_ON);

   		fMenuAnalysis->DisableEntry(M_ONLINE_OFF);
   		fMenuAnalysis->DisableEntry(M_ONLINE_ON);
                //------ Buttons ----------
		fButtonOsci->SetPicture(gClient->GetPicture(aManager->GetWorkingDir()+"/icons/stop_osci.xpm"));
         	fButtonOsci->SetEnabled(kTRUE);

         	fButtonConfigureAcq->SetEnabled(kTRUE);
         	fButtonAcq->SetPicture(gClient->GetPicture(aManager->GetWorkingDir()+"/icons/start_acq.xpm"));
         	fButtonAcq->SetEnabled(kTRUE);

         	fButtonDisplay->SetEnabled(kFALSE);
        
         	fButtonOnline->SetEnabled(kTRUE);


	break;

	case sACQUISITION:
                //------ Menu ----------
   		fMenuAcq->DisableEntry(M_ACQ_READCONFIG);
   		fMenuAcq->DisableEntry(M_ACQ_SAVECONFIG);
   		fMenuAcq->DisableEntry(M_ACQ_CONFIGURE);
   		fMenuAcq->DisableEntry(M_ACQ_STARTOSCI);
   		fMenuAcq->DisableEntry(M_ACQ_STOPACQ);
   		fMenuAcq->DisableEntry(M_ACQ_STARTACQ);
   		fMenuAcq->EnableEntry(M_ACQ_STOPOSCI);

   		fMenuDisplay->EnableEntry(M_DISP_OFF);
   		fMenuDisplay->DisableEntry(M_DISP_ON);

   		fMenuAnalysis->DisableEntry(M_ONLINE_OFF);
   		fMenuAnalysis->EnableEntry(M_ONLINE_ON);
                //------ Buttons ----------
		fButtonOsci->SetPicture(gClient->GetPicture(aManager->GetWorkingDir()+"/icons/stop_osci.xpm"));
         	fButtonOsci->SetEnabled(kFALSE);

         	fButtonConfigureAcq->SetEnabled(kFALSE);
         	fButtonAcq->SetPicture(gClient->GetPicture(aManager->GetWorkingDir()+"/icons/stop_acq.xpm"));
         	fButtonAcq->SetEnabled(kTRUE);

         	fButtonDisplay->SetEnabled(kTRUE);
                if( aManager->GetDisplay() ) fButtonDisplay->SetPicture( gClient->GetPicture( aManager->GetWorkingDir() + "/icons/d_on.xpm" ) );
                else 			     fButtonDisplay->SetPicture( gClient->GetPicture( aManager->GetWorkingDir() + "/icons/d_off.xpm" ) );
        
         	fButtonOnline->SetEnabled(kTRUE);


	break;

	default:
	break;

      }

}

//==================================================================
Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {

//cout << "MSG: " << msg << "\tparm1: " << parm1 << "\tparm2: " << parm2 << endl;
 
  switch(GET_MSG(msg)) {
  //-
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
	case kCM_MENU:
	switch(parm1){
	   case M_FILE_EXIT:
		cout << "M_FILE_EXIT "<< endl;
		CloseWindow();
	   break;

	   case M_VIEW_DISPLAY:
		if( fMenuView->IsEntryChecked(M_VIEW_DISPLAY) ){
                    fMenuView->UnCheckEntry(M_VIEW_DISPLAY);
                    fCommonFrame->HideFrame(fControlDisplay);   
		}else{
                    fMenuView->CheckEntry(M_VIEW_DISPLAY);
                    fCommonFrame->ShowFrame(fControlDisplay);
		}

	   break;

	   case M_VIEW_DEVICE:
		if( fMenuView->IsEntryChecked(M_VIEW_DEVICE) ){
                    fMenuView->UnCheckEntry(M_VIEW_DEVICE);
                    this->HideFrame(fControlDevice);   
		}else{
                    fMenuView->CheckEntry(M_VIEW_DEVICE);
                    this->ShowFrame(fControlDevice);
		}
	   break;

	   case M_ACQ_READCONFIG:
		aManager->LoadConfiguration();    // dopisac dialog file!!!
		cout << "M_ACQ_READCONFIG "<< endl;
	   break;

	   case M_ACQ_SAVECONFIG:
		aManager->SaveConfiguration();     // dopisac dialog file!!!
		cout << "M_ACQ_SAVECONFIG"<< endl;
	   break;

	   case M_ACQ_CONFIGURE:
  		(fControlDevice->IsVisible(fControlDevice)) ? this->HideFrame(fControlDevice) : this->ShowFrame(fControlDevice) ;
	//	aManager->ConfigureFrame();
	   break;

	   case M_ACQ_STARTOSCI:
                mState = sOSCI;
		cout << "M_ACQ_STARTOSCI " << mState << endl;
                aManager->StartAcquisition( mState );
	   break;

	   case M_ACQ_STOPOSCI:
                mState = sSTOP;
		cout << "M_ACQ_STOPOSCI " << mState << endl;
                //aManager->StopAcquisition( mState );
	   break;

	   case M_ACQ_STARTACQ:
                if(mState == sSTOP){
                  aManager->StartAcquisition( mState );
                  }
                mState = sACQUISITION;
		cout << "M_ACQ_STOPACQ " << mState << endl;
	   break;

	   case M_ACQ_STOPACQ:
                mState = sSTOP;
		cout << "M_ACQ_STOPACQ " << mState << endl;
                //aManager->StopAcquisition( mState );
	   break;
	   
	   case M_DISP_ON:
		aManager->SetDisplay(kFALSE);
	   break;

	   case M_DISP_OFF:
		aManager->SetDisplay(kTRUE);
	   break;

	   case M_HELP_CONTENTS:
                if(aHelpFrame) aHelpFrame->Pop();
		else aHelpFrame = new AHelp(const_cast<char*>("/Users/tomaszbrys/root/root_v5.34.18.source/README/INSTALL") );   
	   break;
           default:
	   break;
           }

        case kCM_CHECKBUTTON:
        break;
        
        case kCM_BUTTON:
	   switch(parm1){	

	   case B_OSCI:
		if(mState == sSTOP){
                   mState = sOSCI;
		   aManager->StartAcquisition(mState);
                   }
                else{
                   mState = sSTOP;
                   //aManager->StopAcquisition();
                  }
	   break;

	   case B_CONFIGURE:
                //cout << "IsVisible  = " << this->IsVisible(fControlDevice) << endl;
                //cout << "IsArranged = " << this->IsArranged(fControlDevice) << endl;
                //cout << "GetState   = " << this->GetState(fControlDevice) << endl;
                
  		if(this->IsVisible(fControlDevice)){ 
                     fMenuView->UnCheckEntry(M_VIEW_DEVICE);
                     this->HideFrame(fControlDevice) ;
                     }
                else{ 
                     fMenuView->CheckEntry(M_VIEW_DEVICE);
                     this->ShowFrame(fControlDevice) ;
                    }
		//aManager->ConfigureFrame();

	   break;

	   case B_ACQ:
		if(mState == sSTOP){
		  mState = sACQUISITION;
                  aManager->StartAcquisition(mState);
		  }
		else if(mState == sACQUISITION){
                  mState = sSTOP;
                  //aManager->StopAcquisition();
		  }
                else{
		  mState = sACQUISITION;
                  }
	   break;

	   case B_DISPLAY:
		if (aManager->GetDisplay()) aManager->SetDisplay(kFALSE);
                else                        aManager->SetDisplay(kTRUE); 
                cout << "================ GetDisplay =  "<< aManager->GetDisplay() << "=============================================" << endl;
	   break;

	   case B_ONLINE:
		//if (aManager->GetOnline() == kTRUE ) aManager->SetOnline(kFALSE);
                //else                                 aManager->SetOnline(kTRUE); 

	   break;

	   case B_EXIT:
		CloseWindow();
	   break;

           default:
	   break;
	   }

        break;

	  
        //-- 
        default:
        //cout << "Default kCM_CHECKBUTTON " << kCM_CHECKBUTTON << endl;
        break;
    //--
    }
    break;
  //-
  
  default:
    cout << "Default kC_COMMAND " << kC_COMMAND << endl;
    break;
  }

  Refresh();
 
  return kTRUE;
}

//=========================================================================
void MainFrame::DoEnable(Bool_t a){

 TGCheckButton *te = (TGCheckButton *) gTQSender;
 Int_t id = te->WidgetId();

 cout << "DEBUG [ControlFrame::DoEnable] widget id = " << id << endl;

 switch (id){
   case eTIMEOUTCHECKB:
     //fCBTimeOut->SetState(EButtonState(a));
     if(!a) {
       fEntryTimeOut->SetNumber(0);
       aManager->SetTimeout(0);
       }
     fEntryTimeOut->SetState( EButtonState(a) );
   break;

   case eEVENTSCHECKB:
     //fCBMaxEvents->SetState(EButtonState(a));
     fEntryMaxEvents->SetState( EButtonState(a) );
     if(!a) {
       fEntryMaxEvents->SetNumber(0);
       aManager->SetMaxEvents(0);
       }
     else {
      fEntryMaxEvents->SetNumber(1);
      aManager->SetMaxEvents(1);
      fEntryMaxFiles->SetNumber(0); //
      aManager->SetMaxFiles(0);     //
      fCBMaxFiles->SetState(EButtonState(0));
      fEntryMaxFiles->SetState( EButtonState(0) );
      }
   break;
 
   case eFILESCHECKB:
     //fCBMaxFiles->SetState(EButtonState(a));
     fEntryMaxFiles->SetState( EButtonState(a) );
     if(!a) {
       fEntryMaxFiles->SetNumber(0);
       aManager->SetMaxFiles(0);
       }
     else {
       fEntryMaxFiles->SetNumber(1);
       aManager->SetMaxFiles(1);
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

void MainFrame::DoValueSet(){

 TGNumberEntry *te = (TGNumberEntry *) gTQSender;
 Int_t id = te->WidgetId();

   switch (id) {
      case eTIMEOUTENTRY:
         aManager->SetTimeout((Int_t)te->GetNumber());
         cout << "DEBUG [MainFrame::DoValueSet] SoftTimeout = " << aManager->GetTimeout() << endl;
      break;

      case eEVENTSENTRY:
         aManager->SetMaxEvents((Int_t)te->GetNumber());
         cout << "DEBUG [MainFrame::DoValueSet] MAxEvents = " << aManager->GetMaxEvents() << endl;
      break;

      case eFILESENTRY:
         aManager->SetMaxFiles((Int_t)te->GetNumber());
         cout << "DEBUG [MainFrame::DoValueSet] MaxFiles = " << aManager->GetMaxFiles() << endl;
      break;

      case eFILESIZE:
         aManager->SetFileSize((Int_t)te->GetNumber());
         cout << "DEBUG [MainFrame::DoValueSet] FileSize = " << aManager->GetFileSize() << endl;
      break;

      case eFILETIME:
         aManager->SetFileTime((Int_t)te->GetNumber());
         cout << "DEBUG [MainFrame::DoValueSet] FileTime = " << aManager->GetFileTime() << endl;
      break;

      default:
      break;
      }

}


void MainFrame::DoExit() {
/*   
cout << "DoExit" << endl;	
   aManager->WriteConfigFile(); 
   aManager->ClearAll();
   for(int i = 0; i < aManager->GetNrDigitizers(); i++){
       aManager->GetDigitizer(i)->Close(); 
       }
   CloseWindow();
*/
}

//=========================================================================
void MainFrame::DoSaveConfig() {
 static TString dir(".");
	  TGFileInfo fi;
          fi.fIniDir    = StrDup(dir);
          new TGFileDialog(gClient->GetRoot(), this, kFDSave, &fi);
          dir = fi.fIniDir;
          if(fi.fFilename)
             aManager->WriteConfigFile(fi.fFilename);

}
//=========================================================================
void MainFrame::DoReadConfig() {

 static TString dir(".");
 TGFileInfo fi;
 fi.fIniDir    = StrDup(dir);
 new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fi);
 dir = fi.fIniDir;

 if(fi.fFilename)
   aManager->ReadConfigFile(fi.fFilename);


}

//=========================================================================

void MainFrame::CreateConfigureFrame(){

}

//=========================================================================

void MainFrame::DoDisplayOn(){
  #ifdef DEBUG
    if(gDEBUG_MAIN > 0)
      cout << "DEBUG [MainFrame::DoDisplayOn] M_ACQ_DISPLAY ON" << endl;
  #endif
         
}

//=========================================================================
void MainFrame::DoDisplayOff(){
  #ifdef DEBUG
    if(gDEBUG_MAIN > 0)
      cout << "DEBUG [MainFrame::DoDisplayOFF] M_ACQ_DISPLAY OFF" << endl;
  #endif
 
}

//=========================================================================
void MainFrame::DoOnlineOff(){
  #ifdef DEBUG
    if(gDEBUG_MAIN > 0)
      cout << "DEBUG [MainFrame::DoOnlineOff] M_ACQ_ONLINE OFF" << endl;
  #endif
    /*
     aManager->StopOnlineServer();
     aManager->SetOnlineLoop(kFALSE);
     aManager->DeleteBufferOnline();
    */
}

//=========================================================================

void MainFrame::DoOnlineOn(){
  #ifdef DEBUG
    if(gDEBUG_MAIN > 0)
      cout << "DEBUG [MainFrame::DoOnlineOn] M_ACQ_ONLINE ON" << endl;
  #endif
/*
     aManager->InitBufferOnline();
     aManager->StartOnlineServer();
     aManager->SetOnlineLoop(kTRUE);
*/
}

//=========================================================================

void MainFrame::SetStatusText(const char *txt, Int_t pi)
{
   // Set text in status bar.
   fStatusBar->SetText(txt,pi);
   cout << "MainFrame::SetStatusText(" << *txt << ")" << endl;
}

//=========================================================================

//MainFrame::StartAcqMode(){}

//MainFrame::StopOsciMode(){}
//MainFrame::StartOsciMode(){}
//MainFrame::StopAcqMode(){}
