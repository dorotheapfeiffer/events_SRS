#ifndef DGTRIGGER_H 
#define DGTRIGGER_H

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
#include <TCanvas.h>
#include <TGMsgBox.h>

#include "DGLogic.h"
#include "DGDownscaler.h"
#include "DGDisc.h"
#include "DGDL631.h"
#include "DGDL632.h"
#include "DGDL633.h"
#include "DGDL634.h"
#include "DGScaler.h"
#include "DGMonitor.h"
#include "DGHelp.h"

#include "DTrigger.h"

//*****************************************************************************
class DGTrigger : public TGMainFrame {

private:
  DTrigger          *fTrigger;          // trigger object pointer
  TTimer            *fTimer;            // timer pointer
  Int_t             fTick;              // timer downscale-counter
  TDatime           *fDatime;           // current time
  std::ofstream          *fLog;              // log-file stream pointer
  std::ofstream          *fScalFile;         // scaler database pointer
  TTree             *fScalTree;         // scaler database pointer
  DGMonitor         *fMonitor;          // monitor ( = lower panel)
  TGMenuBar         *fMenuBar;
  TGPopupMenu       *fMenuFile;
  TGPopupMenu       *fMenuExpert;
  TGPopupMenu       *fMenuHelp;
  TGPopupMenu       *fMenuDebug;
  TRootEmbeddedCanvas *fCanvasFrame;
  TGHorizontalFrame *fControlFrame;
  TGHorizontalFrame *fLogicMTCALFrameSlice[8];
  TGHorizontalFrame *fFLTEnableFrameSlice[18];
  TGHorizontalFrame *fSLTSDDMultFrameSlice[18];
  TGVerticalFrame   *fLogicFrame;
  TGVerticalFrame   *fFLTFrame;
  TGVerticalFrame   *fSLTFrame;
  TGVerticalFrame   *fBusyFrame;
  TGLabel           *fLogicLab;
  TGLabel           *fFLTLab;
  TGLabel           *fSLTLab;
  TGLabel           *fBusyLab;
  TGLabel           *fFLTDownLab;
  TGGroupFrame      *fLogicPhysFrame;
  TGGroupFrame      *fLogicMTCALFrame;
  TGGroupFrame      *fFLTEnableFrame;
  TGGroupFrame      *fSLTRequireFrame;
  TGGroupFrame      *fSLTSDDMultFrame;
  TGGroupFrame      *fBusyWaitFrame;
  TGTextButton      *fLogicPhysEditLogic;
  TGTextButton      *fLogicPhysTestLogic;
  TGCheckButton     *fLogicPhysBbp;
  TGCheckButton     *fLogicPhysIbp;
  TGCheckButton     *fLogicMTCALBut[2][8];
  TGCheckButton     *fFLTEnableBut[18];
  TGCheckButton     *fSLTRequireBut[18];
  TGCheckButton     *fSLTSDDMultBut[18];
  TGCheckButton     *fBusyWaitBut[18];
  TGTextBuffer      *fFLTDownB[18];
  TGTextBuffer      *fSDDThrB[4];
  TGTextBuffer      *fSDDDowB[4];
  TGTextEntry       *fFLTDownE[18];
  TGTextEntry       *fSDDThrE[4];
  TGTextEntry       *fSDDDowE[4];

public:
  DGTrigger(){}
  DGTrigger(DTrigger *trigger);
  virtual ~DGTrigger();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void OpenScalerDatabase(Char_t *);     // open scaler database
  void CloseScalerDatabase();            // close scaler database
  void FillScalerDatabase();             // write actual scaler counts into db

  ClassDef(DGTrigger,0)                                // GUI for DTrigger
};
//*****************************************************************************
#endif

