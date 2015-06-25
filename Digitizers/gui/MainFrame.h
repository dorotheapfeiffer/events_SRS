
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


class MainFrame : public TGMainFrame {

public:
enum mSTATE {
     sSTOP = 0,
     sOSCI,
     sACQUISITION
   };

enum CommandIdentifiers{ 
     M_FILE_EXIT,
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
     B_EXIT
   };


private:
   AHelp		*aHelpFrame;
   
   ADisplay		*aDisplay;
   
   AManager 		*aManager;

   mSTATE 		mState;

   TRootEmbeddedCanvas  *fEcan;
   TGStatusBar          *fStatusBar;
 
   TGMenuBar		*fMenuBar;

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

   TGPopupMenu          *fMenuFile;
   TGPopupMenu          *fMenuAcq;
   TGPopupMenu          *fMenuDisplay;
   TGPopupMenu          *fMenuAnalysis;
   TGPopupMenu          *fMenuHelp;

   void CreateMenuBar();
   void CreateToolBox();
   void CreateEmbeddedCanvas();

public:
   MainFrame(const TGWindow *p, UInt_t, UInt_t);
   virtual ~MainFrame();

   void SetAcquisitionState(mSTATE aState) { mState = aState;}
   MainFrame::mSTATE GetAcquisitionState() {return MainFrame::mState;}
   
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
