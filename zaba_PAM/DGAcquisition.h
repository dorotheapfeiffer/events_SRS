#ifndef DGACQUISITION_H 
#define DGACQUISITION_H

#include <iostream>
#include <TTimer.h>
#include <TObjArray.h>
#include <TGMenu.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TCanvas.h>
#include <TGMsgBox.h>

#include "DGHelp.h"
#include "DAcquisition.h"
#include "DGMadc32.h"
#include "DGCAEN1740.h"
#include "DGV1718.h"
#include "DGDisplay.h"
#include "DDataSave.h"

//*****************************************************************************
class DGAcquisition : public TGMainFrame {
  
private:

enum Widget{
  eLoadSetup = 1,
  eSaveSetup,
  eSaveData,
  eExit,
  eInitializeSetup,
  eMesytecAdc32,
  eCAEN1740,
  eCAENBridgeV1718,
  eRunButton,
  eAcqButton,
  eDisplayButton,
  eResetButton,
  eHelp,
  eAcqStatusEntry1,
  eAcqStatusEntry2,
  eAcqStatusEntry3,
  eAcqStatusEntry4,
  eStopAfterSecCB,
  eStopAfterFileCB,
  eStopAfterEventsCB,
  eStopAfterSecEntry,
  eStopAfterFileEntry,
  eStopAfterEventsEntry,
  eSaveFileSizeCB,
  eSaveFileSizeEntry,
  eSaveFileTimeCB,
  eSaveFileTimeEntry

};


  DAcquisition          *fAcquisition;        // trigger object pointer
  TTimer            	*fTimer;            // timer pointer
  Int_t             	fTick;              // timer downscale-counter
  //TDatime           	*fDatime;           // current time
  //std::ofstream         *fLog;            // log-file stream pointer
  DGDisplay        	*fGDisplay;         // monitor Display( = lower panel)
  DDataSave        	*fDataSave;         // pointer to data save

  TGMenuBar         	*fMenuBar;
  TGPopupMenu       	*fMenuFile;
  TGPopupMenu       	*fMenuModules;
  TGPopupMenu       	*fMenuHelp;
  TGPopupMenu       	*fMenuDebug;
  TRootEmbeddedCanvas	*fCanvasFrame;
  TGVerticalFrame 	*fControlFrame;
  TGHorizontalFrame 	*fControlFrame1;
  TGHorizontalFrame 	*fControlFrame2;
  TGTextButton      	*fLogicPhysEditLogic;
  TGTextButton      	*fLogicPhysTestLogic;

  TGGroupFrame		*fAcqStatusFrame;
  TGVerticalFrame 	*fAcqStatusFrame1;
  TGVerticalFrame 	*fAcqStatusFrame2;

  TGGroupFrame		*fStopAfterFrame;
  TGVerticalFrame 	*fStopAfterFrame1;
  TGVerticalFrame 	*fStopAfterFrame2;

  TGGroupFrame		*fSaveFrame;
  TGHorizontalFrame 	*fSaveFrame1;
  TGHorizontalFrame 	*fSaveFrame2;

  TGTextButton		*fRunButton; 
  TGTextButton		*fAcqButton;        
  TGTextButton		*fDisplayButton;
  TGTextButton		*fResetButton;

  TGNumberEntryField	*fAcqTimeEntryField;
  TGNumberEntryField	*fAcqEventEntryField;
  TGNumberEntryField	*fAcqEventSecEntryField;
  TGNumberEntryField	*fAcqDataTraEntryField;

  TGTextBuffer		*fAcqStatusBuffer1;
  TGTextEntry		*fAcqStatusEntry1;
  TGLabel		*fAcqStatusLabel1;

  TGTextBuffer		*fAcqStatusBuffer2;
  TGTextEntry		*fAcqStatusEntry2;
  TGLabel		*fAcqStatusLabel2;

  TGTextBuffer		*fAcqStatusBuffer3;
  TGTextEntry		*fAcqStatusEntry3;
  TGLabel		*fAcqStatusLabel3;

  TGTextBuffer		*fAcqStatusBuffer4;
  TGTextEntry		*fAcqStatusEntry4;
  TGLabel		*fAcqStatusLabel4;

  TGCheckButton		*fStopAfterSecCB;
  TGCheckButton		*fStopAfterFileCB;
  TGCheckButton		*fStopAfterEventsCB;

  TGNumberEntry		*fStopAfterSecEntry;
  TGNumberEntry		*fStopAfterFileEntry;
  TGNumberEntry		*fStopAfterEventsEntry;

  TGCheckButton		*fSaveSizeCB;      
  TGNumberEntry		*fSaveSizeEntry;  
  TGLabel		*fSaveLabel;     
  TGCheckButton		*fSaveTimeCB;
  TGNumberEntry		*fSaveTimeEntry; 

  


public:
  DGAcquisition(){}
  DGAcquisition( DAcquisition *);
  virtual ~DGAcquisition();

  void RefreshStatus();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void DoValueSet();

  ClassDef(DGAcquisition,0)                                // GUI for DTrigger
};
//*****************************************************************************
#endif

