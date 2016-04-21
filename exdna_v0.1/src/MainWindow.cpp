/*******************************************************************************************
*
* Declaration of the class MainWindow, it contains the main window and all widgets
* 
*
*
********************************************************************************************/


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/// Root Includes
#include <TROOT.h>
#include <TCanvas.h>
#include <TRootEmbeddedCanvas.h>
#include <TApplication.h>
#include <TGFrame.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TGButton.h>
#include <TObjString.h>
#include <TGFileDialog.h>
#include <TGStatusBar.h>
#include <TGMenu.h>
#include <TGLabel.h>
#include <TH1F.h>
#include <TGProgressBar.h>
#include <TGNumberEntry.h>
#include <TGListBox.h>
#include <TG3DLine.h> 
#include <TGLabel.h>
#include <TGButton.h>

#include <TMultiGraph.h>
#include <TGButtonGroup.h>
#include <TGComboBox.h>
#include <TPaveText.h>
#include <TGMsgBox.h>
#include <TPRegexp.h>

#include "MainWindow.h"
#include "UserClass.h"
#include "Files.h"
#include "ReadData.h"

using namespace std;

ClassImp(MainWindow)

ReadData *aReadData = 0;

MainWindow::MainWindow(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame( p, w, h) { 
  
  SetCleanup(kDeepCleanup);
   
  TStyle *style = new TStyle("Plain","Plain");
  style->cd();
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatBorderSize(1);
  gStyle->SetFrameFillColor(kWhite);

//-----------------------------------------------

  iCurrentEvent	= 0;
  iNrEvents	= 0;
  iCurrentFile	= "";
  iNrOfFiles	= 0;
  iNrFile	= 0;
  iNrTracks	= 0;
  iEventRefresh	= 100;
  iTimeRefresh	= 2000;
  bLoopCon	= kFALSE;
  mLoopCondition= kTRUE;
  mResetDisplay = kFALSE;
  mUserClass	= 0;
  mAlgorythm    = 0;

  SetCleanup(kDeepCleanup);

  mGraphList = new TObjArray();

//-----------------------------------------------

  ReadConfigFile();

//-----------------------------------------------
  AddMenuBar();
  AddButtonsPanel();
  AddCommonFrame( );
  AddStatusBar();

//-----------------------------------------------

  //Connect("CloseWindow()", "MainWindow()" , this, "CloseWindow()");
  //DontCallClose();

  SetWindowName("Event Display and Analysis");
  MapSubwindows();
  Resize( GetDefaultSize() );
  MapWindow();
  fCommonFrame->HideFrame(fOptionMenu);

}

//============================================================

MainWindow::~MainWindow() {

  if (mUserClass) { 
      delete mUserClass;
      mUserClass = 0;
     }

  delete mGraphList;

  SetCleanup(kDeepCleanup);
  Cleanup(); // deleting all widgets, layouthints and subframes...
  DeleteWindow();
  gApplication->Terminate(0);

}

//============================================================

void MainWindow::ReadData(){
     UpdateProgressBar((char*)"reading");
     try{
	if(aReadData) {
           delete aReadData; 
           aReadData=0; 
           }     
        aReadData = ReadData::Factory("FILE", iCurrentFile.Data());
        fEventNumber->SetNumber(0);
        fEventNumber->SetLimits(TGNumberEntry::kNELLimitMax, 0, aReadData->GetNrEvents() );
        }
     catch(...){ cout << "Error during reading file" << endl; return; }

     UpdateProgressBar((char*)"read");

     fButtonStart->SetEnabled( kTRUE  );   
     fButtonStop->SetEnabled(  kFALSE );   
     fButtonReset->SetEnabled( kFALSE );   
     fEventNumber->SetState(   kTRUE  );
 
     fStatusBar->SetText(iCurrentFile, 0);

     if( !mUserClass ) mUserClass = new UserClass(iCurrentFile, mAlgorythm);

    
     DisplayEvent(0);    
 
}

//============================================================

void MainWindow::AnalyseData(){

   //cout << "iNrFile: "<< iNrFile << "\tnr of files:" <<  Files::getInstance().GetSize() << endl;
   Int_t i = 0;

      while( GetLoopCondition() ){
             
           fEventNumber->SetNumber(0);
           for(i = 0; i < aReadData->GetNrEvents(); i++){
               //AEvent &aEvent = aReadData->GetEvent(fEventNumber->GetNumber());  
               AEvent *aEvent= aReadData->GetEvent(i);  
               mUserClass->DoAnalysis(*aEvent, mMode);
               gSystem->ProcessEvents();
           
               if(!(i % 500)){ 
                 //cout << "file nr: " << iNrFile << "\ti: " << i << endl;
                 fEventNumber->SetNumber(i);
                 DisplayEvent(i);
                 UpdateProgressBar((char*)"analysing");
                 }               

               if( !GetLoopCondition() ) break;

              }
            iNrFile++;
            if( iNrFile < Files::getInstance().GetSize() ){
               SetCurrentFile(Files::getInstance().GetFile( iNrFile ).Data());
               ReadData();
               }
            else break;

            UpdateProgressBar((char*)"analysing");
           }
 
     iNrFile--;
     fEventNumber->SetNumber(i);
     UpdateProgressBar((char*)"analysing");
     DisplayEvent(i-1);
    gApplication->Terminate(); 

} 
//============================================================

void MainWindow::AddMenuBar() {
   fMenuFile = new TGPopupMenu(gClient->GetRoot());

   fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
   fMenuFile->AddEntry("&Connect...", M_FILE_CONNECT);
   fMenuFile->AddEntry("&Disconnet", M_FILE_DISCONNECT);
   fMenuFile->AddEntry("E&xit", M_FILE_EXIT);

   //fMenuFile->AddEntry("O&ption", M_VIEW_OPTION);
   fMenuFile->Connect("Activated(Int_t)", "MainWindow", this, "HandleMenu(Int_t)");
   fMenuView = new TGPopupMenu(gClient->GetRoot());
   fMenuView->AddEntry("O&ption", M_VIEW_OPTION);
   fMenuView->Connect("Activated(Int_t)", "MainWindow", this, "HandleMenu(Int_t)");
   
   fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
   fMenuBar->AddPopup("&File", fMenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
   fMenuBar->AddPopup("&View", fMenuView, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
   AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

   //cout<<"out of add menu bar"<<endl;
}

//============================================================

void MainWindow::AddCommonFrame() {
  // fCommonFrame    horizontal frame containing embedded canvas and option menu
  fCommonFrame = new TGCompositeFrame(this,300,300,kHorizontalFrame);
  fCommonFrame->SetName("MainPanel");
  //
  // fEcan embedded canvas---------------------------------------
  //
  fEcan = new TRootEmbeddedCanvas("fEcan",fCommonFrame,500,300);
  fEcan->GetCanvas()->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject *)","MainWindow",this,"DrawEventStatus(Int_t, Int_t, Int_t, TObject *)");
  
  fCommonFrame->AddFrame(fEcan, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,1,1,1,1));
  
  fOptionMenu = new TGCompositeFrame(fCommonFrame,300,300,kVerticalFrame);
//     fDisplayGroup = new TGGroupFrame(fOptionMenu, "Display mode");
   
     fAlgorythmGroup = new TGVButtonGroup(fOptionMenu, "Algorythm");
	fButtonAlgorythm = new TGTextButton(fAlgorythmGroup, "INDYVIDUAL");
        if(mAlgorythm == 1){      
           fButtonAlgorythm->SetText("CHARGE DIV");
           }
	fButtonAlgorythm->MoveResize(0,0,20,20);
	fButtonAlgorythm->Connect("Clicked()", "MainWindow", this, "DoAlgorythm()");
	fAlgorythmGroup->AddFrame(fButtonAlgorythm, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, -20, 0));
     fOptionMenu->AddFrame(fAlgorythmGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,0,0));
     
     fButtonGroup = new TGVButtonGroup(fOptionMenu, "Display Mode");
     
     fCBDisplayMode = new TGComboBox(fButtonGroup, eDISPLAYMODE);
        fCBDisplayMode->AddEntry("signals", 0);
        fCBDisplayMode->AddEntry("corrected", 1);
	fCBDisplayMode->Resize(80,20);
	fCBDisplayMode->Select(0);
     fButtonGroup->AddFrame(fCBDisplayMode, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,0,0));
   
     fRadiob[0] = new TGRadioButton(fButtonGroup, new TGHotString("Together"), eRBTOGETHER);
     fRadiob[1] = new TGRadioButton(fButtonGroup, new TGHotString("Separate"), eSEPARATE);
     fRadiob[2] = new TGRadioButton(fButtonGroup, new TGHotString("Group"),    eRBGROUP);
     fRadiob[3] = new TGRadioButton(fButtonGroup, new TGHotString("Casette"),  eRBCASETTE);
     fOptionMenu->AddFrame(fButtonGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));

     fCBCasetteNr = new TGComboBox(fButtonGroup, eCASSETENR);
        fCBCasetteNr->AddEntry("all", 0);
        for(Int_t i = 0; i < 9; i++){
            char nr[10];
            sprintf(nr, "%d", i+1);
            fCBCasetteNr->AddEntry(nr, i+1);
            }
	fCBCasetteNr->Resize(50,20);
	fCBCasetteNr->Select(0);
        if(fRadiob[3]->IsOn()) fCBCasetteNr->SetEnabled(1);
        else fCBCasetteNr->SetEnabled(0);
     fButtonGroup->AddFrame(fCBCasetteNr, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,0,0));
        
   
     fButtonGroup->SetRadioButtonExclusive(kTRUE);
     fRadiob[0]->SetOn();

     fRadiob[0]->Connect("Toggled(Bool_t)", "MainWindow", this, "DoRadioButton()");
     fRadiob[1]->Connect("Toggled(Bool_t)", "MainWindow", this, "DoRadioButton()");
     fRadiob[2]->Connect("Toggled(Bool_t)", "MainWindow", this, "DoRadioButton()");
     fRadiob[3]->Connect("Toggled(Bool_t)", "MainWindow", this, "DoRadioButton()");

     //fHistogramGroup = new TGGroupFrame(fOptionMenu, "Histograms");
     //fOptionMenu->AddFrame(fHistogramGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        
  fCommonFrame->AddFrame(fOptionMenu, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandY,1,1,1,1));


 AddFrame(fCommonFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,3,3,3,3));
 
}

//============================================================

void MainWindow::AddButtonsPanel() {

	fButtonsFrame = new TGHorizontalFrame(this,200,40,kHorizontalFrame);

	//------------text entry and button to go to event number n
	fEventNumber = new TGNumberEntry( fButtonsFrame, 0, 10, eEVENTNUMBER,
                           TGNumberEntry::kNESInteger,TGNumberEntry::kNEANonNegative,TGNumberEntry::kNELLimitMin,0);
	fEventNumber->MoveResize(0,0,70,20);
	fEventNumber->SetMinWidth(10);
	fEventNumber->Connect("ValueSet(Long_t)","MainWindow",this,"NumberEntrySet(Long_t)");
        (fEventNumber->GetNumberEntry())->Connect("ReturnPressed()", "MainWindow", this, "NumberEntrySet(Long_t)");
	fButtonsFrame->AddFrame(fEventNumber, new TGLayoutHints(kLHintsLeft |kLHintsTop|kLHintsExpandY,1,1,1,1));

	//----------------ProgressBar
	progressBar = new TGHProgressBar(fButtonsFrame,TGHProgressBar:: kFancy | TGHProgressBar::kSolidFill,20);
	progressBar->SetName("ProgressBar");
	progressBar->SetFillType(TGProgressBar::kSolidFill);
       	progressBar->SetBarColor("lightblue");
	progressBar->ChangeOptions(kSunkenFrame | kDoubleBorder | kOwnBackground);
	progressBar->SetPosition(0.01);
	progressBar->ShowPosition(kTRUE,kFALSE,"Select a file to start");
	//progressBar->MoveResize(0,0,20,20);
	fButtonsFrame->AddFrame(progressBar, new TGLayoutHints(kLHintsLeft | kLHintsTop |kLHintsExpandX |kLHintsExpandY,1,1,1,1));

	//--------------- button start analysis
	fButtonStart = new TGTextButton(fButtonsFrame, "START");
	fButtonStart->MoveResize(0,0,20,20);
	fButtonStart->Connect("Clicked()", "MainWindow", this, "DoStart()");
	fButtonsFrame->AddFrame(fButtonStart, new TGLayoutHints(kLHintsRight | kLHintsTop,1,1,1,1));

	//--------------- button stop analysis
	fButtonStop = new TGTextButton(fButtonsFrame, " STOP ");
	fButtonStop->MoveResize(0,0,20,20);
	fButtonStop->Connect("Clicked()", "MainWindow", this, "DoStop()");
	fButtonsFrame->AddFrame(fButtonStop, new TGLayoutHints(kLHintsRight | kLHintsTop,1,1,1,1));

	//--------------- button reset histograms
	fButtonReset = new TGTextButton(fButtonsFrame, "RESET");
	fButtonReset->MoveResize(0,0,20,20);
	fButtonReset->Connect("Clicked()", "MainWindow", this, "DoReset()");
	fButtonsFrame->AddFrame(fButtonReset, new TGLayoutHints(kLHintsRight | kLHintsTop,1,1,1,1));


	fButtonStart->SetEnabled(0);
	fButtonStop->SetEnabled(0);
	fButtonReset->SetEnabled(0);
	fEventNumber->SetState( EButtonState(kFALSE));
	
	AddFrame(fButtonsFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));
	
}

//============================================================

void MainWindow::AddStatusBar(){
  //add status bar, info are displayed in MainWindow::DrawEventStatus(,,,)
  
  fStatusBar = new TGStatusBar(this,488,20);
  fStatusBar->SetName("fStatusBar");
  int parts[] = {33,10,10,47};
  fStatusBar->SetParts(parts,4);
 

  AddFrame(fStatusBar, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
}

//============================================================


void MainWindow::HandleMenu(Int_t id)
{
    switch (id) 
    {
    case M_FILE_EXIT:
	CloseWindow();
	break;

    case M_FILE_OPEN:
        {
	  TGFileInfo fi;
	  fi.fIniDir    = StrDup(gApplication->WorkingDirectory());
	  new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fi);

	  if(fi.fFilename==0 && fi.fFileNamesList==0){
	    break;
	  }
	  
	  if(fi.fMultipleSelection && fi.fFileNamesList) {
	    TObjString *el;
	    TIter next(fi.fFileNamesList);
	    while((el = (TObjString *) next())) {
	      Files::getInstance().AddFile(el->GetString());
	    }
	  } else if (fi.fFilename) {
	    Files::getInstance().AddFile(fi.fFilename);
	  }

	  if(fi.fFilename != 0 || Files::getInstance().GetFile(0) != 0){
            SetCurrentFile(Files::getInstance().GetFile(0).Data());
	    ReadData();
	 
	    }
	  else{
	    iNrOfFiles = 0;
	  } 
        }
   	break;

    case M_FILE_CONNECT:
    break;

    case M_FILE_DISCONNECT:
    break;

    case M_VIEW_OPTION:
      {
	if(fCommonFrame->IsVisible(fOptionMenu))fCommonFrame->HideFrame(fOptionMenu);
	else fCommonFrame->ShowFrame(fOptionMenu);
      }
      break;
	
    default:
	printf("Menu item %d selected\n", id);
	break;
   }
}

//============================================================

void MainWindow::NumberEntrySet(Long_t nrEvent){

 TGNumberEntry *te = (TGNumberEntry *) gTQSender;
 Int_t id = te->WidgetId();


 switch (id) {
   case eEVENTNUMBER:
        if ( fEventNumber->GetNumber() >  0 &&  fEventNumber->GetNumber() < aReadData->GetNrEvents() ){ // normal case
             //aReadData->DrawEvent( fEcan, fEventNumber->GetNumber(), 0);
	     DisplayEvent(fEventNumber->GetNumber());
           }
	else if( (fEventNumber->GetNumber() >= aReadData->GetNrEvents()) && ((iNrFile+1) < Files::getInstance().GetSize())){ 
                                               // reach the max event, load next file
                                               // if exist
            iNrFile++; 
            SetCurrentFile(Files::getInstance().GetFile( iNrFile ).Data());
            ReadData();

          }
        else if ( fEventNumber->GetNumber() == 0 && iNrFile > 0 ){ // reach min event load prevoius file (if exist)

            iNrFile--;
            SetCurrentFile(Files::getInstance().GetFile( iNrFile ).Data());
            ReadData();
            fEventNumber->SetNumber(aReadData->GetNrEvents());

          }
   break;

   default:
   break;
  }

}

//============================================================
void MainWindow::DoStop() {

   mLoopCondition = 0;
 
}

//============================================================
void MainWindow::DoStart() {
  
   mLoopCondition = 1; 
   fButtonStart->SetEnabled(0);
   fButtonStop->SetEnabled(1);
   fButtonReset->SetEnabled(1);
   fEventNumber->SetState( EButtonState(kFALSE));
   UpdateProgressBar((char*)"analysing");
   //fEventNumber->SetNumber(0);

   //--------------------------------------------

   AnalyseData();

   //--------------------------------------------

   fButtonStop->SetEnabled(0);
   fButtonStart->SetEnabled(1);
   fButtonReset->SetEnabled(0);
   fEventNumber->SetState( EButtonState(kTRUE));
   //fEventNumber->SetNumber(0);
   
}
//============================================================
void MainWindow::DoReset() {

   DoStop();
   Int_t ret = 0;
   new TGMsgBox(gClient->GetRoot(), this, " WARNING !!! ", "All histograms will be cleaned, and all data on histograms will be lost", 0, 0x41, &ret); 
   cout << "retval = "<< ret << endl;
   if (ret == 1 ){
      mResetDisplay = kTRUE;
      }
}

//============================================================
void MainWindow::DoAlgorythm() {

   if(mAlgorythm == 0) {
      fButtonAlgorythm->SetText("CHARGE DIV");
      mAlgorythm = 1;
      WriteConfigFile();
      if(mUserClass) mUserClass->DrawCdivCanvas();
      }
   else{ 
      fButtonAlgorythm->SetText("INDYVIDUAL");
      mAlgorythm = 0;
      WriteConfigFile();
      if(mUserClass) mUserClass->DrawIndCanvas();
      }

}
//============================================================
void MainWindow::ResetDisplay(){

      if( iNrFile == 0){   
        cout << "DEBUG [MainWindow::DoReset] iNrFile = " << iNrFile << endl;
        fEventNumber->SetNumber(0);
        DisplayEvent(fEventNumber->GetNumber());
        //aReadData->DrawEvent( fEcan, fEventNumber->GetNumber(), 0);
        }
      else{
        cout << "DEBUG [MainWindow::DoReset] iNrFile = " << iNrFile << endl;
        iNrFile = 0;
        fEventNumber->SetNumber(0);
        SetCurrentFile(Files::getInstance().GetFile( iNrFile ).Data());
        ReadData();
        DisplayEvent(fEventNumber->GetNumber());
        //aReadData->DrawEvent( fEcan, fEventNumber->GetNumber(), 0);
        } 
   UpdateProgressBar((char*)"analysing"); 
}

//============================================================
void MainWindow::DoRadioButton(){
 
    if(fRadiob[3]->IsOn()) fCBCasetteNr->SetEnabled(1);
    else fCBCasetteNr->SetEnabled(0);

    DisplayEvent( fEventNumber->GetNumber() );

}

//============================================================

//void MainWindow::CloseWindow() {
//   this->DeleteWindow();
//   gApplication->Terminate();
//}

//============================================================

void MainWindow::UpdateEventNumber(Int_t a) {
     fEventNumber->SetNumber(a);
}

//============================================================

void MainWindow::Reading(Int_t status){
  cout<<"reading\t"<<status<<endl;
  TString strText;
//  strText.Form( "Reading ( %d / %d )", iCurrentFile.SubString().AtoI() , iNrOfFiles );
  progressBar->SetMax(100);
  progressBar->SetPosition(status);
  progressBar->ShowPosition(kTRUE,kFALSE,strText.Data());
}

//==================================================================================================

void MainWindow::UpdateProgressBar(char* operation){
  TString tsOper = operation;
  tsOper.ToLower();
  TString strText;
/*
  TPRegexp e("[0-9]");
  Int_t begining = iCurrentFile.Last('_');
  TString temp = iCurrentFile;
  temp.Remove(0, begining+1);
  temp.Remove(temp.Length()-4);
   
  Int_t iFile = temp.Atoi();
*/   
  if(tsOper == "analysing"){
    //analysing
    strText.Form(" Analysing ( %d / %d )", iNrFile + 1, Files::getInstance().GetSize() );
    progressBar->SetMax( aReadData->GetNrEvents() );
    progressBar->ShowPosition( kTRUE, kFALSE, strText.Data() );
    progressBar->SetPosition( fEventNumber->GetNumber()  );
  }
  else if(tsOper == "reading"){
    //Reading
    strText.Form( "Reading file ( %d / %d )", iNrFile + 1 , Files::getInstance().GetSize() );
    progressBar->ShowPosition( kTRUE, kFALSE, strText.Data() );
    progressBar->SetPosition( 0.01 );
  }
  else if(tsOper == "read"){ 
    //Read
    strText.Form(" File ( %d / %d ) has loaded, %d events ", iNrFile + 1, Files::getInstance().GetSize(), aReadData->GetNrEvents());
    progressBar->ShowPosition( kTRUE, kFALSE, strText.Data() );
  } 
  else if(tsOper == "seek"){
    strText.Form(" Seeking ( %d / %d ), %d events ", iNrFile, iNrOfFiles, iNrEvents);
    progressBar->SetMax( iNrEvents );
    progressBar->ShowPosition( kTRUE, kFALSE, strText.Data() );
    progressBar->SetPosition( iCurrentEvent );
    //Update the textEntry to the good value
    fEventNumber->SetIntNumber( iCurrentEvent );
  }
  else if(tsOper == "clear"){
	progressBar->SetPosition( 0.01 );
	progressBar->ShowPosition( kTRUE, kFALSE, "Select a file to start" );
  }
  else cout<<"[MainWindow::UpdateProgressBar] error control string not reconized"<<endl;
	 
}

//==================================================================================================

void MainWindow::MousePosition(Int_t event, Int_t x, Int_t y, TObject *selected){
  TString tsClassName(selected->ClassName(),10);
  cout << "tsClassName: " << tsClassName << endl;
  //  TCanvas* fcan = fEcan->GetCanvas();
  //if(tsClassName=="TFra"){
  // cout<<event<<"\t"<<fcan->XtoPad(x)<<"\t"<<fcan->YtoPad(y)<<"\t"<<selected<<endl;
  //
}

//==================================================================================================
/*
void MainWindow::UpdateStatusBar(Char_t text, Int_t px){

fStatusBar->SetTExt(text, 0);

}
*/

//==================================================================================================

void MainWindow::DrawEventStatus(Int_t event, Int_t px, Int_t py, TObject *selected) {
   // Report name and title of primitive below the cursor.
   //
   //    This function is called when the option "Event Status"
   //    in the canvas menu "Options" is selected.

   const Int_t kTMAX=256;
   static char atext[kTMAX];

   TCanvas* fcan = fEcan->GetCanvas();
   TVirtualPad* savepad;
   savepad = gPad;

   gPad=fcan->GetSelectedPad();

   fStatusBar->SetText( iCurrentFile, 0 );
   fStatusBar->SetText( selected->GetName(), 1 );
   if (event == kKeyPress)
      sprintf(atext, "%c", (char) px);
   else
      sprintf(atext, "%d,%d", px, py);
   fStatusBar->SetText(atext,2);
   fStatusBar->SetText(selected->GetObjectInfo(px,py),3);
   gPad = savepad;
}

//==================================================================================================

void MainWindow::DivideCanvas(){
return;
  // cout<<"bCustomDisplay"<<bCustomDisplay<<endl;
  switch(iBGDisplay){
  case 1:
    //cout<<"iNrTracks"<<iNrTracks<<endl;
    if(iNrTracks<=16&&iNrTracks>0)iNbToDisplay= iNrTracks;
    else iNbToDisplay = 16;
    break;
  case 2: 
    iNbToDisplay = 1;
    break;
  case 3: 
    iNbToDisplay = 1;
    break;
  case 4://will set the canvas according to the custom display window
     for(Int_t pad = 0; pad<iNrTracks; pad++){//scan trough pads to find wich track to display
      iNbTrackByPad[pad]=0;

      for(Int_t tracks = 0; tracks<iNrTracks;tracks++){
	if(gRBG1[tracks]->GetButton(pad+1)->IsDown()){
	
	  iTracktoBeShown[pad][iNbTrackByPad[pad]]=tracks;
	  iNbTrackByPad[pad]++;
	}
      }
    }

    iNbToDisplay=0;
    //scan through IPadToBeShown to calculate the number of pad (higher pad nuber called;
    for(Int_t pad = iNrTracks-1 ; pad >= 0; pad--){
      if(iNbTrackByPad[pad] != 0){
	iNbToDisplay = pad+1;
	break;
      }
    }
    //cout<<"iNbToDisplay custom "<<iNbToDisplay<<endl;
    // for(Int_t ppad = 0; ppad < iNbToDisplay; ppad++){
      //  cout<<"Pad "<<ppad+1;
      //for(Int_t ttra = 0; ttra<iNbTrackByPad[ppad];ttra++)cout<<"\tTrack\t"<<iTracktoBeShown[ppad][ttra];
      //cout<<endl;
    //}
	
    break;
  default://else use info in the ListBox instead
    // cout<<"else"<<endl;
    for(int tt = 0; tt<iNrTracks; tt++){
      //   bToBeDisplayed[tt]=fListBox->GetSelection(tt);
      // cout<<bToBeDisplayed[tt]<<endl;
      //if(fListBox->GetSelection(tt))iNbToDisplay++;
    }
    //  cout<<"nb to display\t"<<iNbToDisplay<<endl;
    iNbToDisplay=4;
    if(iNbToDisplay>16){
      cout<<"warning: only 16 Tracks can be shown"<<endl;
      iNbToDisplay=16;
    }
    break;
  
}
   TCanvas *canvas = fEcan->GetCanvas();
   canvas->Clear();
   //cout<< "[MainWindow::ReadData]:iNrTracks\t"<<iNrTracks<<endl;
   // cout<<"iNbToDisplay"<<iNbToDisplay<<endl;
   if(iNbToDisplay == 1)       canvas->cd();
   else if (iNbToDisplay == 2) canvas->Divide(1,2);
   else if (iNbToDisplay == 3) canvas->Divide(1,3);
   else if (iNbToDisplay <  5) canvas->Divide(2,2);
   else if (iNbToDisplay <  7) canvas->Divide(2,3);
   else if (iNbToDisplay <  9) canvas->Divide(2,4);
   else if (iNbToDisplay < 10) canvas->Divide(3,3);
   else if (iNbToDisplay < 13) canvas->Divide(3,4);
   else canvas->Divide(4,4);
   //cout<<"Going to display event"<<endl;
     DisplayEvent(iCurrentEvent);

}

//==================================================================================================
void MainWindow::DivideCanvasCustom(){
  /**create the window used for setting the tracks layout.
Actual canvas division is done by DivideCanvas() that is call when the draw button is called
  */ 
  bCustomDisplay = kTRUE;
  //block list box used for setting canvas
  //    fTextButtonCust->SetEnabled(0);
  //  fTextButton662->SetEnabled(0);
  //cout<<"[MainWindow::DivideCanvasCustom]: new window"<<endl;
  fCustWindow = new TGTransientFrame(gClient->GetRoot(),this,400,200);
  //fCustWindow->MoveResize(10,10,400,400);
  fCustWindow->SetWindowName("Dispaly Manager");
  fCustWindow->Connect("CloseWindow()","MainWindow",this,"CloseCustom()");
  //for desing only
  //iNrTracks = 4;
  // TGHorizontalFrame* gRadioDisplay;
  //  TGButtonGroup *gRBG1;
  TString g;
  TGHorizontalFrame* gCheckDisplay;
  //title (pad names)
  TGHorizontalFrame* gPadName =  new TGHorizontalFrame(fCustWindow);
  gPadName->AddFrame(new TGLabel(gPadName,"tracks"), new TGLayoutHints( kLHintsCenterY |kLHintsLeft,1,1,1,1));
  //  
  //Header with pad numbers
  //
  for(Int_t iTr = 0; iTr < iNrTracks; iTr++){
    g.Form("Pad%d",iTr+1);
    gPadName->AddFrame(new TGLabel(gPadName,g.Data()), new TGLayoutHints(kLHintsExpandX | kLHintsCenterY |kLHintsLeft,1,1,1,1));
    
  }
  fCustWindow->AddFrame(gPadName, new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));
  //
  //for each track a label + Button group holding  check buttons, 1 for each pad
  //
  gRBG1 =  new TGButtonGroup*[iNrTracks];
  for(Int_t iTr = 0; iTr < iNrTracks; iTr++){
    gCheckDisplay =  new TGHorizontalFrame(fCustWindow);
    g.Form("Track%d",iTr);
    gCheckDisplay->AddFrame(new TGLabel(gCheckDisplay,g.Data()), new TGLayoutHints(kLHintsCenterY |kLHintsLeft,1,1,1,1));
    //  gRBG1 = new  TGButtonGroup(fCustWindow,"",kHorizontalFrame);
    gRBG1[iTr] = new  TGButtonGroup(gCheckDisplay,"",kHorizontalFrame);
    gRBG1[iTr]->Connect("Clicked(Int_t)", "MainWindow", this, "DivideCanvas()");
    // gRBG1 = new  TGButtonGroup(fCustWindow,"",kHorizontalFrame);
    for(Int_t iPad=0;iPad <iNrTracks; iPad++){
      TGCheckButton* tgb = new TGCheckButton(gRBG1[iTr]);
      tgb->SetTextJustify(kTextCenterX);
     if(iPad==iTr)tgb->SetState(kButtonDown);
    }
    gRBG1[iTr]->SetLayoutHints(new  TGLayoutHints( kLHintsExpandX | kLHintsLeft,1,1,1,1),0);
    gRBG1[iTr]->Show();
    gCheckDisplay->AddFrame(gRBG1[iTr], new TGLayoutHints(/*kLHintsCenterX | kLHintsCenterY |*/ kLHintsExpandX,1,1,1,1));
    fCustWindow->AddFrame(gCheckDisplay, new TGLayoutHints(kLHintsTop | kLHintsExpandX,1,1,1,1));
    
  }
  //
  //button to call DivideCanvas
  //
  /*  fButtonRedraw =  new TGTextButton(fCustWindow,new TGHotString("&Redraw"));
  fButtonRedraw->Connect("Clicked()", "MainWindow", this, "DivideCanvas()");
  fCustWindow->AddFrame(fButtonRedraw, new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,1,1,1,1));*/


  //   fCustWindow->MapSubWindow(); 
  fCustWindow->MapSubwindows();
  fCustWindow->MapWindow();
  fCustWindow->Resize(iNrTracks*50+50,iNrTracks*54+70);
  DivideCanvas();
}

//==================================================================================================

void MainWindow::SetFirstEvent(Long_t){
  //  cout<<"set first event "<<iFirstEvent<<endl;
  //iFirstEvent=(Int_t)fNEFirstEvent->GetNumber();
  //cout<<"set first event "<<iFirstEvent<<endl;
  //fNELastEvent->SetLimitValues(iFirstEvent+1,iNrEvents); 

}

//==================================================================================================

void MainWindow::SetLastEvent(Long_t){
  //cout<<"set last event"<<iLastEvent<<endl;
  //iLastEvent=(Int_t) fNELastEvent->GetNumber();
  //cout<<"set last event"<<iLastEvent<<endl;
  //fNEFirstEvent->SetLimitValues(0,iLastEvent-1);
}

//==================================================================================================

void MainWindow::DisplayEvent(Int_t iEvent) {

   mGraphList->Clear();

   TCanvas *c = fEcan->GetCanvas();
   c->cd();
   c->Clear();
 
   if( !aReadData ) {
     cout << "Please load the data first...." << endl;
     return;
     }

   AEvent* aEvent= aReadData->GetEvent(iEvent);
 
   Int_t mNrTracks = aEvent->GetNrTracks();

   //------------==================---------
   if (fRadiob[3]->IsOn()) {
      //cout << "draw signals per casette" << endl;
       //-----------------------------------------------------------

       if(mMultiGraph){ delete mMultiGraph; mMultiGraph = 0; }
       mMultiGraph = new TMultiGraph();

       //-----------------------------------------------------------

       TGraph *gr;
       char grName[100];
       //char grTitle[100];
       char multiGrName[100];
       sprintf(multiGrName, "Event:%d-", iEvent);

       for(Int_t i = 0; i < mNrTracks; i++){
           sprintf(grName,"Inst:%d-ch:%d.", (Int_t)aEvent->GetTrack(i)->GetCardNr(), (Int_t)aEvent->GetTrack(i)->GetChannelNr());
           strcpy( multiGrName + strlen(multiGrName), grName);
           gr = new TGraph( (Int_t)aEvent->GetTrack(i)->GetDataSize() );
           //sprintf(grTitle, "ch_%d", (Int_t)aEvent.GetTrack(i)->GetChannelNr());  
           //gr->SetTitle(grTitle);

           UShort_t *data = aEvent->GetTrack(i)->GetData();
           for(UInt_t j = 0; j < aEvent->GetTrack(i)->GetDataSize(); j++){
               gr->SetPoint(j, j, data[j]);
              }

           gr->SetName(grName);
           gr->SetMarkerColor(i+2);
           gr->SetLineColor(i+2);
           gr->SetMarkerStyle(21);
           gr->SetMarkerSize(0.3);

           mMultiGraph->Add(gr,"lp");
           }

       mMultiGraph->SetTitle(multiGrName);
       mMultiGraph->Draw("a");
       mMultiGraph->SetMinimum(0);
       mMultiGraph->SetMaximum(5000);
       c->Modified();
       c->Update();
      }

   //------------==================---------
   if (fRadiob[0]->IsOn()) {
      //cout << "draw all signals together" << endl;

       //-----------------------------------------------------------

       if(mMultiGraph){ delete mMultiGraph; mMultiGraph = 0; }
       mMultiGraph = new TMultiGraph();

       //-----------------------------------------------------------

       TGraph *gr;
       char grName[100];
       char multiGrName[100];
       sprintf(multiGrName, "Event:%d-", iEvent);

       for(Int_t i = 0; i < mNrTracks; i++){
           sprintf(grName,"Inst:%d-ch:%d.", (Int_t)aEvent->GetTrack(i)->GetCardNr(), (Int_t)aEvent->GetTrack(i)->GetChannelNr());
           strcpy( multiGrName + strlen(multiGrName), grName);
           gr = new TGraph( (Int_t)aEvent->GetTrack(i)->GetDataSize() );

           UShort_t *data = aEvent->GetTrack(i)->GetData();
           for(UInt_t j = 0; j < aEvent->GetTrack(i)->GetDataSize(); j++){
               gr->SetPoint(j, j, data[j]);
              }

           gr->SetName(grName);
           gr->SetMarkerColor(i+2);
           gr->SetLineColor(i+2);
           gr->SetMarkerStyle(21);
           gr->SetMarkerSize(0.3);

           mMultiGraph->Add(gr,"lp");
           }

       mMultiGraph->SetTitle(multiGrName);
       mMultiGraph->Draw("a");
       mMultiGraph->SetMinimum(0);
       mMultiGraph->SetMaximum(5000);
       c->Modified();
       c->Update();
      }


   //------------==================---------
   if (fRadiob[2]->IsOn()) {
      //cout << "draw signals per group od digitizers" << endl;

      }
   //------------==================---------
   if (fRadiob[1]->IsOn()) {
      //cout << "draw all signals separate on each pad" << endl;
      if( mNrTracks == 1) {;}
      else if( mNrTracks == 2) {c->Divide(1,2);} 
      else if( mNrTracks < 5)  {c->Divide(2,2);} 
      else if( mNrTracks < 7)  {c->Divide(3,2);} 
      else if( mNrTracks < 9)  {c->Divide(4,2);} 
      else if( mNrTracks == 9) {c->Divide(3,3);}
      else if( mNrTracks < 13) {c->Divide(4,3);}
      else if( mNrTracks < 17) {c->Divide(4,4);}
      else if( mNrTracks < 21) {c->Divide(5,4);}
      else if( mNrTracks < 26) {c->Divide(5,5);}
      else if( mNrTracks < 31) {c->Divide(6,5);}
      else c->Divide(8,4);
     
       TGraph *gr[mNrTracks];
       char grName[100];
       char grTitle[100];
       char multiGrName[100];
       sprintf(multiGrName, "Event:%d-", iEvent);

       for(Int_t i = 0; i < mNrTracks; i++){
           sprintf(grName,"Inst:%d-ch:%d.", (Int_t)aEvent->GetTrack(i)->GetCardNr(), (Int_t)aEvent->GetTrack(i)->GetChannelNr());
           strcpy( multiGrName + strlen(multiGrName), grName);
           gr[i] = new TGraph( (Int_t)aEvent->GetTrack(i)->GetDataSize() );
           sprintf(grTitle, "ch_%d", (Int_t)aEvent->GetTrack(i)->GetChannelNr());  
           gr[i]->SetTitle(grTitle);
	   
           mGraphList->Add(gr[i]);

           UShort_t *data = aEvent->GetTrack(i)->GetData();
           for(UInt_t j = 0; j < aEvent->GetTrack(i)->GetDataSize(); j++){
               gr[i]->SetPoint(j, j, data[j]);
              }

           c->cd(i+1);
           gr[i]->Draw("alp"); 
           gr[i]->SetName(grName);
           gr[i]->SetMarkerColor(i+2);
           gr[i]->SetLineColor(i+2);
           gr[i]->SetMarkerStyle(21);
           gr[i]->SetMarkerSize(0.3);
           }

       c->cd(); 
       c->Modified();
       c->Update();

      }

}

//==================================================================================================

void MainWindow::HandelBGDisplay(Int_t id){
  iBGDisplay=id;
  if(id==4&&!bCustomDisplay)DivideCanvasCustom();
  else DivideCanvas();
  
}

//==================================================================================================

void MainWindow::CloseCustom(){
  //cout<<"Closing window"<<endl;
  bCustomDisplay = kFALSE;
  //  fTextButtonCust->SetEnabled(1);
  //  fTextButton662->SetEnabled(1);
}

//==================================================================================================

void MainWindow::SetTimeRefresh(Long_t time){
  //iTimeRefresh = (Int_t)fNETimeRefresh->GetNumber();
  //cout<<time<<"\t"<<iTimeRefresh<<endl;
  //if(mUserClass)mUserClass->TimeRefresh(iTimeRefresh);
}

//==================================================================================================

void MainWindow::SetEventRefresh(Long_t event){
  //iEventRefresh = (Int_t)fNEEventRefresh->GetNumber();
  //if(mUserClass)mUserClass->EventRefresh(iEventRefresh);
}

//==================================================================================================

void MainWindow::SetOutText(char* ch){
  // cout<<ch<<endl;
  TString str = ch;
  TString path(str(0,str.Last('/')+1));
  if(path=="")path="./";


  TString filename = str.Remove(0, str.Last('/')+1);
  //TString path(str(0,str.Last('/')));
  cout<<"filename\t"<<filename<<"\t"<<"path\t"<<path<<endl;

}
//==============================================================================

void MainWindow::WriteConfigFile(){

   ofstream fout(".rybarc", ios::out);
   if(!fout){
     cerr << "Error opening file .rybarc " << endl;
     }
   fout << "Algorythm " << mAlgorythm << endl;
   fout.close(); 

}

//==============================================================================
void MainWindow::ReadConfigFile(){
   ifstream fin(".rybarc", ios::in);
   if(!fin){
      return;
      }
   TString aString;
   Int_t   aInt = -1;

   fin >> aString >> aInt ;

   if(aString == "Algorythm")
     mAlgorythm = aInt;
 
   fin.close();
}
//==============================================================================
