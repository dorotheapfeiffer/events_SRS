#ifndef CAEN5740Dlg_h
#define CAEN5740Dlg_h

#include <iostream>
#include <string>
#include <TQObject.h>
#include <TGFrame.h>

class TGCompositeFrame;
class TGGroupFrame;
class TGLayoutHints;
class TGTextButton;
class TGCheckButton;
class TGNumberEntry;
class TGTextEntry;
class TGTextBuffer;
class TGComboBox;
class Digitizer;
class CAEN5740;
class TGTab;

//class CAEN5740Dlg : public TQObject {
class CAEN5740Dlg : public TGMainFrame {
 
private:
  CAEN5740 *aDevice; 	
   enum WD {
     eCARD = 10,
     eSTATUS,
     eTRACKLENGTH,
     ePRETRIGGER,
     eBUFFERS,
     eGROUP = 50,	
     eOFFSET = 100,	
     eCHANNEL_SAVE = 150,
     eGPIO = 200,
     eCLK,		
     eTRIGGER_EXT,
     eTRIGGER_SW,
     eTRIGGER_CHN,
     eEXTTRIGGERMODE,
     eSWTRIGGERMODE,
     eCHNTRIGGERMODE,
     eCOINCIDENCE = 290,
     eTRIGGERGROUP = 300,
     eTRIGGER_SELFMASK = 350,
     eTRIGGER_ENABLE = 400,
     eTRIGGER_EDGE = 450,
     eCHANNEL_POLARITY = 500,
     eTRIGGER_THRESHOLD = 550,
     eTRIGGER_SYNCHRONIZATION = 600,
     eCHBTSTART,
     eTRIGGEROUT_DELAY,
     eACQ_MODE  		
     };

   //----------------------------------
   TGCompositeFrame* 	fFrameCard;
   TGTextButton*	fButtonEnable;
//   TGTextEntry*		fStatusEntry;

   //----------------------------------
   TGGroupFrame* 	fFrameCommon;
   TGCompositeFrame*	fFrameChannel;
    TGGroupFrame*	fFrame1;
    TGGroupFrame*	fFrame2;
     TGCompositeFrame*	fFrame21;
     TGCompositeFrame*	fFrame22;
     TGCompositeFrame*	fFrame23;
    TGGroupFrame*	fFrame3;

   TGCompositeFrame* 	fFrameTrigger;
     TGCompositeFrame*	fFrameTg1;
     TGCompositeFrame*	fFrameTg2;
     TGCompositeFrame*	fFrameTg3;
     TGCompositeFrame*	fFrameTg4;
     TGCompositeFrame*	fFrameTg5;
     TGCompositeFrame*	fFrameTg6;
     TGCompositeFrame*	fFrameTg7;
     TGNumberEntry*	fRecordLength;
     TGNumberEntry*	fPostTrigger;
    //----------------------------------
    TGGroupFrame* 	fFrameGroup;
     TGCheckButton*	fGroupEnableMask[4];
    TGGroupFrame* 	fFrameOffset;
     TGNumberEntry*	fDCoffset[4];
    TGGroupFrame* 	fFrameSave;
     TGCheckButton*	fCheckButtonSave[32];
 
   //----------------------------------
    TGCompositeFrame*	fFrameTrigger1;
     TGCompositeFrame*	fFrameTrigger11;
     TGGroupFrame*	fFrameTrigger12;
     //TGCompositeFrame*	fFrameTrigger12;
     TGComboBox*	fChannelTriggerMode[4];
     TGNumberEntry*	fThreshold[4];
     TGComboBox*	fChannelTriggerEdge[4];


    TGCompositeFrame*	fFrameTrigger2;
     TGGroupFrame*	fFrameTrigger21;
      TGComboBox*	fExtTriggerMode;
      TGTextButton*	fFPIOtype;

     TGGroupFrame*	fFrameTrigger22;
     TGCheckButton*	fCheckButtonTrigger[32];
     TGTextButton*	fChannelPolarity[32];
     TGTextButton*	fTriggerEdge[32];

   TGCheckButton*	fSelfTriggerMask[32];

     TGGroupFrame*	fFrameTrigger23;
      TGNumberEntry*	fCoincidence;	

   TGCompositeFrame* 	fFrameOptions;
     TGGroupFrame* 	fFrameOp1;
     TGCheckButton*	fChBtStart;	
     TGNumberEntry*	fDelay;
     TGComboBox*	fAcqMode;

   TGTab* 		fTab;
   TGCompositeFrame*    fFrameTab[3]; // there are always three tabs

public:
   
  CAEN5740Dlg(){}; 
  CAEN5740Dlg(CAEN5740 *, TGCompositeFrame* );
  virtual 		~CAEN5740Dlg();

  void			Refresh(); 
  virtual Bool_t	ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void 			NumberEntrySet(Long_t); 
  void 			ComboBoxSet(Int_t); 
  void 			CheckButtonSet(Bool_t); 
  void 			EnableGroup(Bool_t a);
  void 			EnableTrigger(Int_t a);
  void 			ButtonClicked();


ClassDef(CAEN5740Dlg,0);
};


#endif

