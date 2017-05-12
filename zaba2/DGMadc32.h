#ifndef DGMADC32_H 
#define DGMADC32_H

#include "DGModule.h"
#include "TGNumberEntry.h"

//*****************************************************************************
class DGMadc32 : public DGModule {

private:
 enum Widget{
    eIgnoreThreshold,
    eGateGenerator,
    eGateDelay,
    eGateWidth,
    eGateOutput,
    eADCResolution,
    eThresholdOn = 100,
    eThresholdValue = 200
 };

  TObjArray        *fWidgetList;

  TGLayoutHints     *fL0;
  TGLayoutHints     *fL1;
  TGLayoutHints     *fL2;
  TGLabel	    *fLabel1;	
  TGLabel	    *fLabel2;	
  TGLabel	    *fLabel3;	
  TGLabel	    *fLabel4;	

  TGCompositeFrame  *fW1Frame; // Widget frame 1, 2, 3, 4, 5
  TGCompositeFrame  *fW2Frame;
   TGCompositeFrame  *fW2_1Frame;
   TGCompositeFrame  *fW2_2Frame;
   TGCompositeFrame  *fW2_3Frame;
   TGCompositeFrame  *fW2_4Frame;
  TGCompositeFrame  *fW3Frame;
   TGCompositeFrame  *fW3_1Frame;
   TGCompositeFrame  *fW3_2Frame;
  TGCompositeFrame  *fW4Frame;
  //TGTextButton      *fResetFF;
  TGCheckButton		*fIgnoreThreshold;
  TGCheckButton		*fThresholdOn[32];
  TGNumberEntry		*fThresholdValue[32];
  TGCheckButton		*fGateGenerator;
  TGNumberEntry		*fGateDelay;
  TGNumberEntry		*fGateWidth;
  TGCheckButton		*fGateOutput;
  TGComboBox		*fADCResolution;

public:
  DGMadc32(DModule *module); 
  ~DGMadc32();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGMadc32,0)                                  // GUI for DMadc32
};
//*****************************************************************************

#endif

