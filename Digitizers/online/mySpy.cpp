#include <iostream>   
#include <TGButton.h>
#include <TSystem.h>
#include <TObject.h>
#include <TRootEmbeddedCanvas.h>
#include <TGLayout.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TSocket.h>
#include <TMessage.h>
#include "../core/AEvent.h"
#include "../core/vecTrack.h"
#include "mySpy.h"



using namespace std;

int gDEBUG_EVENT;

ClassImp(mySpy)

//==========================================================	
void mySpy::DoButton() {

   if(aGetData){
     aGetData = 0;
     fGet->SetText("GET");
   }
   else{
     aGetData = 1;
     fGet->SetText("STOP");
     WindowsUpdate();	   
   }

 }

//==========================================================	
void mySpy::WindowsUpdate(){
   
   if (!fSock->IsValid()){
      cout << "Connection not valid" << endl;	   
      return;
   }

   while(aGetData){
      fSock->Send("get");
      TMessage *mess;
      if(fSock->Recv(mess) <= 0) {
         printf("mySpy::DoButton error receiving message");
         return;
        }

      if (aEvent) delete aEvent;

      aEvent = (AEvent*) mess->ReadObject(mess->GetClass());
   
      InitGraphs();

      for(UInt_t i = 0; i < aEvent->GetNrTracks(); i++){
          Short_t *data = aEvent->GetTrack(i)->GetData();
          for(Int_t j = 0; j <  aEvent->GetTrack(i)->GetDataSize(); j++){
              mGraphs[i]->SetPoint(j, j, data[j]);
              }
          }

           
      fCanvas->GetCanvas()->Modified();
      fCanvas->GetCanvas()->Update();

      delete mess;
      
      gSystem->ProcessEvents(); 
    }


}

//==========================================================	
void mySpy::InitGraphs(){
 #ifdef DEBUG
   cout << "DEBUG [MainFrame::InitGraphs] " << endl;
  #endif
 Int_t aTracks = aEvent->GetNrTracks();
 Int_t aSamples = 0;


 for(Int_t i = 0; i < aTracks; i++){
     if( aSamples < aEvent->GetTrack(i)->GetDataSize() ){ 
         aSamples = aEvent->GetTrack(i)->GetDataSize(); 
       }
    }		 

 if(aTracks == preTracks && aSamples == preSamples){
   //cout << "DEBUG [MainFrame::InitGraph] number of preTracks = " << preTracks  << "\tnumber of tracks in Event  = " << aTracks  <<"\t No Initialization, just draw" << endl;
   //cout << "DEBUG [MainFrame::InitGraph]          preSamples = " << preSamples << "\tnumber of samples in Event = " << aSamples <<"\t" << endl;
   return;	  
   } 
 else{
   //cout << "DEBUG [MainFrame::InitGraph] number of preTracks = " << preTracks  << "\tnumber of tracks in Event  = "  << aTracks <<"\t MUST be (re)Initialization" << endl;
   //cout << "DEBUG [MainFrame::InitGraph]          preSamples = " << preSamples << "\tnumber of samples in Event = " << aSamples <<"\t MUST be (re)Initialization" << endl;
   }


 
 if(mMultiGraph){
   delete mMultiGraph;
   if(mGraphs) { delete [] mGraphs; mGraphs = 0;}
   }

 
 mMultiGraph = new TMultiGraph();
      
 mGraphs = new TGraph* [ aTracks ];
 char name[20];
 for(Int_t i = 0; i < aTracks; i++){
    sprintf(name,"g_%d",i);
    mGraphs[i] = new TGraph( aEvent->GetTrack(i)->GetDataSize() );
    //cout << " DEBUG [MainFrame::InitGraph] create graphs  [" << name << "]\t" << "nr points = " << aEvent.GetTrack(i)->GetDataSize() << endl;;
    for(Int_t j = 0; j < aEvent->GetTrack(i)->GetDataSize(); j++){
        mGraphs[i]->SetPoint(j, j, 0);
       }
    mGraphs[i]->SetName(name);
    mGraphs[i]->SetMarkerColor(i+2);
    mGraphs[i]->SetLineColor(i+2);
    mGraphs[i]->SetMarkerStyle(21);
    mGraphs[i]->SetMarkerSize(0.3);

    mMultiGraph->Add(mGraphs[i],"lp");
    }

   delete h;
   h = new TH1F("h","h",aSamples, 0, aSamples);
   h->Draw(" ");
   h->SetAxisRange(-5000, 5000, "y");
   
   mMultiGraph->Draw("");

   fCanvas->GetCanvas()->Modified();
   fCanvas->GetCanvas()->Update();


   preTracks  = aTracks;
   preSamples = aSamples;
}


//==========================================================	
void mySpy::Connect() {
   // Connect to mySpyServ
   fSock = new TSocket("localhost", 9090);
   fConnect->SetState(kButtonDisabled);
   fGet->SetState(kButtonUp);
}

//==========================================================	
mySpy::mySpy()
{
   // Create a main frame
   fMain = new TGMainFrame(0, 100, 100);
   fMain->SetCleanup(kDeepCleanup);

   // Create an embedded canvas and add to the main frame, centered in x and y
   // and with 30 pixel margins all around
   fCanvas = new TRootEmbeddedCanvas("Canvas", fMain, 600, 400);
   fLcan = new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,30,30,30,30);
   fMain->AddFrame(fCanvas, fLcan);
   cout << "Hello 10 "<< endl;
/*
*/

   // Create a horizonal frame containing three text buttons
   fLhorz = new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 30);
   fHorz = new TGHorizontalFrame(fMain, 100, 100);
   fMain->AddFrame(fHorz, fLhorz);

   fLbut = new TGLayoutHints(kLHintsCenterX, 10, 10, 0, 0);
   fGet = new TGTextButton(fHorz, "Get Event", 3);
   fGet->SetState(kButtonDisabled);
   fGet->Connect("Clicked()", "mySpy", this, "DoButton()");
   fHorz->AddFrame(fGet, fLbut);

   // Create a horizonal frame containing two text buttons
   fHorz2 = new TGHorizontalFrame(fMain, 100, 100);
   fMain->AddFrame(fHorz2, fLhorz);

   // Create "Connect" and "Quit" buttons
   // Add to horizontal frame
   fConnect = new TGTextButton(fHorz2, "Connect");
   fConnect->Connect("Clicked()", "mySpy", this, "Connect()");
   fHorz2->AddFrame(fConnect, fLbut);
   fQuit = new TGTextButton(fHorz2, "Quit");
   fQuit->SetCommand("gApplication->Terminate()");
   fHorz2->AddFrame(fQuit, fLbut);

   cout << "Hello 20 "<< endl;
   // Set main frame name, map sub windows (buttons), initialize layout
   // algorithm via Resize() and map main frame
   fMain->SetWindowName("mySpy on mySpyServ");
   fMain->MapSubwindows();
   fMain->Resize(fMain->GetDefaultSize());
   fMain->MapWindow();

   h = new TH1F("h","h",1024, 0, 1024);
   h->Draw(" ");
   h->SetAxisRange(5000, 5000, "y");

   fCanvas->GetCanvas()->Modified();
   fCanvas->GetCanvas()->Update();
   cout << "Hello 30 "<< endl;
   aEvent	= 0;
   mMultiGraph	= 0;
   mGraphs	= 0;
   aNrGraphs	= 0;
   aGetData	= 0;

   cout << "Hello 40 "<< endl;

}

//==========================================================	
mySpy::~mySpy()
{
   // Clean up

   delete aEvent;
   delete fSock;
   delete fLbut;
   delete fLhorz;
   delete fLcan;
   delete fGet;
   delete fConnect;
   delete fQuit;
   delete fHorz;
   delete fHorz2;
   delete fCanvas;
   delete fMain;
}

