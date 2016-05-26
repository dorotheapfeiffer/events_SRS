
#ifndef MainFrame_H
#define MainFrame_H

#include <TGFrame.h>
#include <TCanvas.h>
#include <TGStatusBar.h>
#include <TGMenu.h>
#include <TRootEmbeddedCanvas.h>
#include <TGButton.h>

#include "AEvent.h"

class ControlFrame;

class TGLabel;
class TGHorizontal3DLine;
class AManager;
class AEvent;
class AHelp;
class ADisplay;
class TGTab;
class TGNumberEntry;


class MainFrame : public TGMainFrame {

public:
enum mSTATE {
     sSTOP = 0,
     sOSCI,
     sACQUISITION
   };

enum CommandIdentifiers{ 
     M_FILE_EXIT,
     M_VIEW_DISPLAY,
     M_VIEW_DEVICE,	
     M_ACQ_READCONFIG,
     M_ACQ_SAVECONFIG,
     M_ACQ_CONFIGURE,
     M_ACQ_STARTOSCI,
     M_ACQ_STOPOSCI,
     M_ACQ_STARTACQ,
     M_ACQ_STOPACQ,
     M_DISP_ON,
     M_DISP_OFF,
     M_ONLINE_ON,
     M_ONLINE_OFF,
     M_HELP_CONTENTS,
     B_OSCI,
     B_CONFIGURE,
     B_ACQ,
     B_ONLINE,
     B_DISPLAY,
     B_EXIT,

     eTIMEOUTCHECKB  = 210,
     eTIMEOUTENTRY   = 220,
     eEVENTSCHECKB   = 230,   
     eEVENTSENTRY    = 240,
     eACQTIMEENTRY   = 250,
     eACQTIMECHB     = 260,
     eFILESCHECKB    = 270,
     eFILESENTRY     = 280,
     eFILESIZE       = 290,   
     eFILETIME       = 300

   };


private:
   Int_t		mNrTabs;
   AHelp		*aHelpFrame;
   
   ADisplay		*aDisplay;
   
   AManager 		*aManager;

   mSTATE 		mState;

   TRootEmbeddedCanvas  *fEcan;
   TGStatusBar          *fStatusBar;
 
   TGMenuBar		*fMenuBar;
   TGCompositeFrame	*fFrame1;
   TGCompositeFrame	*fFrame2;
   TGCompositeFrame	*fCommonFrame;
   TGCompositeFrame	*fControlDisplay;
   TGCompositeFrame	*fControlDevice;
  

   TGHorizontalFrame	*fButtons;
   TGPictureButton 	*fButtonOnline;
   TGPictureButton 	*fButtonOsci;
   TGPictureButton 	*fButtonAcq;
   TGPictureButton 	*fButtonExit;
   TGPictureButton 	*fButtonConfigureAcq;
   TGPictureButton 	*fButtonDisplay;

   TGLabel		*fAcqLabel;
   
   TGHorizontal3DLine   *l1; 
   TGHorizontal3DLine   *l2; 
   TGHorizontal3DLine   *l3; 

   TGPopupMenu          *fMenuFile;
   TGPopupMenu          *fMenuView;
   TGPopupMenu          *fMenuAcq;
   TGPopupMenu          *fMenuDisplay;
   TGPopupMenu          *fMenuAnalysis;
   TGPopupMenu          *fMenuHelp;

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
  	TGCheckButton *fCBAcqTime;
        TGNumberEntry *fEntryAcqTime;


        TGGroupFrame    *fFileFrame;
        TGNumberEntry   *fEntryFileSize ;
        TGNumberEntry   *fEntryFileTime ;


   void CreateMenuBar();
   void CreateToolBox();
   void CreateControlDevice();
   void CreateCommonFrame();   // for embedded canvas and control display
   void BuildTabs();
   void BuildWindow(TGCompositeFrame* );

public:
   MainFrame(const TGWindow *p, UInt_t, UInt_t);
   virtual ~MainFrame();

   void SetAcquisitionState(mSTATE aState) { mState = aState;}
   MainFrame::mSTATE GetAcquisitionState() {return MainFrame::mState;}
 
   //void NumberEntrySet(Long_t a){cout << "NumberEntrySet  a = "<<  endl;} 
 
   void DoExit();
   void DoReadConfig();
   void DoSaveConfig();
   void CreateConfigureFrame();
  // void StartOsciMode();
  // void StopOsciMode();
  // void StartAcqMode();
  // void StopAcqMode();
   void DoDisplayOn();
   void DoDisplayOff();
   void DoOnlineOff();
   void DoOnlineOn();
   void DoEnable(Bool_t );
   void DoValueSet();

   ADisplay		*GetDisplay() { return aDisplay;} 

   //Int_t StartAcquisition();
   //Int_t StartDataSave();
   //Int_t StartOnlineServer();
   //Int_t StopAcquisition();
   //Int_t StopDataSave();
   //Int_t StopOnlineServer();

   void SetStatusText(const char *txt, Int_t pi);

   //void EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected);
   //void HandleCanvas(Int_t event, Int_t x, Int_t y, TObject *selected);
   //virtual Int_t DistancetoPrimitive(Int_t px, Int_t py){return 0;}
   void SetLabelAcq(bool a);
   void DisconnectSlots();
   //virtual void CloseWindow();
   //void Activated(Int_t id); 
   //void HandleMenu(Int_t id);
   //void HandleButtons(Int_t id);
   //void HandlePopup() { }
   //void HandlePopdown() { }
   TCanvas* GetCanvas() const {return fEcan->GetCanvas();}
   void Refresh();

private:
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
   bool aIsUpdate;
   void Style_ACQ();
   ClassDef(MainFrame, 0)
};




#endif  
