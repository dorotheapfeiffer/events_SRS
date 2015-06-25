#ifndef ControlFrame_H
#define ControlFrame_H

#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TGResourcePool.h>
#include <TGFrame.h>
#include <TGComboBox.h>   
#include <TGLabel.h>      

//#include "AManager.h"

class AManager;
class MainFrame;
class TGGroupFrame;
class TGTab;
class TGCheckButton;
class TGNumberEntry;
class TGLabel;

class ControlFrame : public TGTransientFrame {

private:

        enum WD{
		eTIMEOUT  = 10,
		eEVENTS   = 20,
		eFILES   = 30,
		eFILESIZE = 40,
		eFILETIME = 50
        };       
  
        AManager*	aManager;  // pointer to Manager
        MainFrame*	aFrame;    // pointer to main window 
        Int_t		aNrTabs;   // nr of tabs necessary to create control window, 
                                   // here also the activation of digitizer will be done 
				   // also for simulation (for test only)  
				   //

	Int_t		aWidth;    // size of the control window (is fixed) user should 
                                   // not change the size of this window
	Int_t		aHeight;
 

        TGCompositeFrame** tCF;    // pointers to frames of tabs, each frame has own tab connected to a digitizer
	TGTab* fTab; 	
 
	TGGroupFrame *fTimeOutFrame;
	TGCheckButton *fCBTimeOut;
	TGNumberEntry *fEntryTimeOut; 

	TGGroupFrame *fLoopFrame;
	TGCheckButton *fCBMaxEvents;
	TGNumberEntry *fEntryMaxEvents;
	TGCheckButton *fCBMaxFiles ;
	TGNumberEntry *fEntryMaxFiles;

 
	TGGroupFrame	*fFileFrame;	
	TGNumberEntry	*fEntryFileSize ;
	TGNumberEntry	*fEntryFileTime ;
	 

	TGTextButton	*fButtonClose; //  button  to close the window


public:
   ControlFrame(const TGFrame*);
   virtual ~ControlFrame();


   void BuildTabs();                  // function which take all information about each digitizer and build a tabs
   void BuildWindow(TGCompositeFrame* );
   void BuildButtonBox();             // function build a button box in the bottom of the window
   void DoClose();                    // function is called when the button fButtonClose is clicked

   void DoValueSet();
   void DoEnable(Bool_t);
   void DisconnectAll();

   void Reconfigure();

   ClassDef(ControlFrame, 0)
};

#endif
