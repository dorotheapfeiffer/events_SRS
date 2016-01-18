#ifndef SimCardDlg_h
#define SimCardDlg_h

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
class SimCard;

//class SimCardDlg : public TQObject {
class SimCardDlg : public TGMainFrame {
 
private:
  SimCard *aDevice; 	
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
     eCOINCIDENCE,
     eTRIGGERGROUP = 300,
     eTRIGGER_SELFMASK = 400,
     eTRIGGER_ENABLE = 500,
     eTRIGGER_EDGE = 600,
     eCHANNEL_POLARITY = 700,
     eTRIGGER_THRESHOLD = 800,
     eTRIGGER_SYNCHRONIZATION = 900
     		
     };

   //----------------------------------
   TGCompositeFrame* 	fFrameCard;
   TGTextButton*	fButtonEnable;
//   TGTextEntry*		fStatusEntry;

   //----------------------------------
   TGGroupFrame* 	fFrameCommon;
    TGCompositeFrame*	fFrame1;
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
   TGGroupFrame* 	fFrameTrigger;
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
   
public:
   
  SimCardDlg(){}; 
  SimCardDlg(SimCard *, TGCompositeFrame* );
  virtual 		~SimCardDlg();

  void			Refresh(); 
  virtual Bool_t	ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  

ClassDef(SimCardDlg,0);
};


#endif

