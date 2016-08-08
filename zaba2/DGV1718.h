#ifndef DGV1718_H 
#define DGV1718_H

#include "DGModule.h"

//*****************************************************************************
class DGV1718 : public DGModule {

private:
 enum Widget{
  eInput,
  eEndCounts,
  eResetCounter,
  eCountOutput,
  eRead,
  eValue,
 };

  TObjArray        *fWidgetList;

  TGLayoutHints     *fL0;
  TGLayoutHints     *fL1;
  TGLayoutHints     *fL2;
  TGLabel	    *fLabel1;	
  TGLabel	    *fLabel2;	
  TGLabel	    *fLabel3;	
  TGLabel	    *fLabel4;	

  TGCompositeFrame  *fFrame1; // Widget frame 1, 2, 3, 4, 5
  TGCompositeFrame  *fFrame2;
  
  TGComboBox		*fInput;
  TGNumberEntry		*fEndCounts;
  TGTextButton		*fResetCounter;
  TGCheckButton		*fCountOutput;
  TGNumberEntry		*fValue;

public:
  DGV1718(DModule *module); 
  ~DGV1718();
  void Refresh(); 
  Bool_t HandleTimer(TTimer *tim);
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  ClassDef(DGV1718,0)                                  // GUI for DMadc32
};
//*****************************************************************************

#endif

