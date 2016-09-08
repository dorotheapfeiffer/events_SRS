#ifndef DGCAEN1740_H 
#define DGCAEN1740_H

#include "DGModule.h"
#include <TGNumberEntry.h>
#include <TGTab.h>

//*****************************************************************************
class DGCAEN1740 : public DGModule {

private:
 enum Widget{
	 eRecordLength = 1,
	 ePostTrigger,
         eGroupEnableMask = 10,
         eDCoffset = 20,	 
	 eSaveChannel = 30,
	 eTriggerLevel = 100,
	 eThreshold = 101,
         eExtTriggerMode = 200,
         eChannelTriggerMode = 201,
	 eSelfTriggerMask = 230,
	 eRunSyncMode = 300,
	 eClock,
	 eDelay, 
	 eTriggerEdge = 310,
         eChannelPulsePolarity = 320	 
 };

  TObjArray		*fWidgetList;

  TGLayoutHints		*fL0;
  TGLayoutHints		*fL1;
  TGLayoutHints		*fL2;
  TGLayoutHints		*fL3;
  TGLayoutHints		*fL4;
  TGLayoutHints		*fL5;
  TGLayoutHints		*fL6;
  TGLabel		*fLabel1;	
  TGLabel		*fLabel2;	
  TGLabel		*fLabel3;	
  TGLabel		*fLabel4;	
 
  TGTab			*fTab; 
  TGCompositeFrame	*fNrTab[4];
  TGCompositeFrame	*fFrame;
  TGGroupFrame		*fGF0_0, *fGF0_1, *fGF0_2, *fGF0_3, *fGF0_4;	
  TGGroupFrame		*fGF1_0, *fGF1_1, *fGF1_2, *fGF1_3, *fGF1_4;	
  TGGroupFrame		*fGF2_0, *fGF2_1;
  TGGroupFrame		*fGF3_0, *fGF3_1;

  TGNumberEntry		*fRecordLength;
  TGNumberEntry		*fPostTrigger;

  TGCheckButton		*fGroupEnableMask[8];
  TGNumberEntry		*fDCoffset[8];
  TGComboBox		*fChannelPulsePolarity[8];
  TGCheckButton		*fSaveChannel[64];

  TGComboBox		*fChannelTriggerMode[8];
  TGComboBox		*fExtTriggerMode;
  TGComboBox		*fTriggerEdge[8];
  TGNumberEntry		*fThreshold[8];
  TGTextEntry		*fTriggerLevel;
  TGCheckButton		*fSelfTriggerMask[64];

  TGComboBox		*fRunSyncMode;
  TGComboBox		*fClock;
  TGNumberEntry		*fDelay;

public:
  DGCAEN1740(DModule *module); 
  ~DGCAEN1740();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGCAEN1740,0)                                  // GUI for DCAEN1740
};
//*****************************************************************************

#endif

