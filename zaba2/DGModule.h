#ifndef DGMODULE_H 
#define DGMODULE_H

#include <TObjArray.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGMenu.h>
#include <TGString.h>
#include <TGLabel.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TGSlider.h>
#include "TGNumberEntry.h"
#include <TGButton.h>
#include <TGComboBox.h>
#include <TSystem.h>
#include <TTimer.h>
#include <TFile.h>
#include <TTree.h>
#include <TGFileDialog.h>
#include <TDatime.h>

#include "DModule.h"

//*****************************************************************************
class DGModule : public TGMainFrame {

private:
  TGGroupFrame     *fComm;         // frame for communication with VME
  TGGroupFrame     *fMDes;         // frame for module description
  TGLayoutHints    *fMenuBarLayout, *fL1, *fL2, *fL3;
  TGTextBuffer     *fRefreshTimeBuf;
  TGTextEntry      *fRefreshTimeEnt;
  int               fRefreshTime;
  TGPopupMenu      *fMenuFile;

protected: 
  TObjArray        *fMemList;       // list of all members
  DModule          *fModule;        // pointer to DModule
  TGMenuBar        *fMenuBar;
  TGGroupFrame     *fMDat;          // frame for module specific stuff
  TGCheckButton    *fRefresh;
  TTimer           *fTimer;         // responsible for refreshing

public:
  DGModule(DModule *module);
  virtual ~DGModule();
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  virtual void Refresh() {;}
  void ExecuteEvent(Int_t event, Int_t px, Int_t py) {printf("kuku\n");}

  ClassDef(DGModule,0)                               // GUI for DModule
};
//*****************************************************************************

#endif
