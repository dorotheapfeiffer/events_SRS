#ifndef mySpy_H
#define mySpy_H

#include "TGFrame.h"
#include "TObject.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TGLayout.h"
#include "TSocket.h"
#include "TMessage.h"
#include "../core/AEvent.h"
#include "../core/ATrack.h"


using namespace std;

class TH1F;
class TGraph;
class TMultiGraph;

class mySpy {

private:
   TGMainFrame         *fMain;
   TRootEmbeddedCanvas *fCanvas;
   TGHorizontalFrame   *fHorz;
   TGHorizontalFrame   *fHorz2;
   TGLayoutHints       *fLbut;
   TGLayoutHints       *fLhorz;
   TGLayoutHints       *fLcan;
   TGTextButton        *fGet;
   TGButton            *fConnect;
   TGButton            *fQuit;
   TSocket             *fSock;
   AEvent	       *aEvent;


   TMultiGraph		*mMultiGraph;
   TGraph		**mGraphs;
   UInt_t		aNrGraphs;
   UInt_t		aGetData;
   TH1F			*h;
   Int_t		preTracks;
   Int_t		preSamples;

   void 		InitGraphs();
   void 		WindowsUpdate();
public:
   mySpy();
   ~mySpy();

   void Connect();
   void DoButton();
   ClassDef(mySpy, 0) //online client
};

#endif
