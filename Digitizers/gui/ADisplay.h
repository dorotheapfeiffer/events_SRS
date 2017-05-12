/*****************************************************************************
*
*****************************************************************************/

#ifndef ADisplay_h
#define ADisplay_h

#include <iostream>
#include <string>

#include <TQObject.h>
#include <TRootEmbeddedCanvas.h>
#include "AEvent.h"

//#include "ARingBuffer.h"

class TPad;
class TObjArray;
class TPaveText;
class TH1F;
class TCanvas;
class TGraph;
class TMultiGraph;
class AEvent;
class AManager;
class TTimer;
class TLine;
class TText;

const Int_t 	nrFrame = 4;

using namespace std;

class ADisplay: public TGMainFrame{

 AManager	 *aManager;
// ARingBuffer* 	aRingBuffer;

 //AEvent		  aEvent;
 
 UInt_t		  aNrGraphs;
 UInt_t		  aMaxSample; 

 UInt_t		  preTracks;
 UInt_t		  preSamples;
 Int_t		  preRange;
 UInt_t		  preThreshold;

 TTimer		 *fTimer;
 UInt_t 	  mRefreshGraph;
 UInt_t 	  mRefreshData;

 ULong_t   	  mElapsedTime; 
 ULong_t	  mCurrentTime; 
 ULong_t	  mPrevRateTime;
 
 ULong_t	  mPrevEvents;
 ULong_t	  mPrevSize;
 AEvent		  mEvent;


 TMultiGraph	 *mMultiGraph;
 TGraph		**mGraphs;
 TCanvas*	  mCanvas;
 TObjArray	 *mMemList;		// list containing all TLine the TText
 TLine		**mThresholdLine; 
 TText		**mThresholdText; 

 TPad		 *upperPad;
 TPad		 *lowerPad;
 TPaveText	 *aPaveText[nrFrame];

 TH1F		 *h;

 
public:
 		 ADisplay(){}
 		 ADisplay(TCanvas* );
 virtual	~ADisplay();

 void		 UpdateGraph(AEvent *);
 UInt_t		 InitGraphs(AEvent *);
 UInt_t 	 InitThreshold();

 AEvent* 	 GenerateEvent();
 TPad		*GetPad()		{ return upperPad; } 
 Bool_t		 HandleTimer(TTimer *);
 
 ULong_t	 GetLongTime();

 ClassDef(ADisplay, 0)
};

#endif

