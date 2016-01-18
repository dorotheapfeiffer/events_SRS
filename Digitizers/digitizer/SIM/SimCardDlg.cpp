/***************************************************************************F 
*
* Implementation GUI class for SimCard
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <bitset>

#include <TObjArray.h>

#include <TGFrame.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGNumberEntry.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>

#include "SimCardDlg.h"
#include "SimCard.h"
#include "Digitizer.h"
#include "AManager.h"

static Int_t Get_Bit(UInt_t val, Int_t bit){
  if( (1 << bit) & val ) return 1;
  else return 0;
}

static void Reset_Bit(UInt_t &val, int bit){
  val &= ~(1 << bit);
}

static void Set_Bit(UInt_t &val, int bit){
  val |= (1 << bit); 
}

static void Flip_Bit(UInt_t &val, int bit){
  val ^= (1 << bit); 
}

static void PrintBin(Int_t dec){
 for(UInt_t i = 0x80000000; i > 0; i >>= 1){
    if(i & dec) printf("1");
    else        printf("0");
    }
 printf("\n");
}

ClassImp(SimCardDlg)

extern int gDEBUG;

using namespace std;

//==================================================================

SimCardDlg::SimCardDlg( SimCard *aDev, TGCompositeFrame* aMainFrame) {
 #ifdef DEBUG
    if(gDEBUG > 2) cout << "DEBUG [SimCardDlg::Constructor]" << endl;
  #endif

   aDevice = aDev;
   Int_t nrGroup = 4;
   Int_t nrChannel = 8; // per group

   char name[30];
  //----------------------------------------------------------
   // CARD FRAME
   //----------------------------------------------------------
   
   fFrameCard = new TGCompositeFrame(aMainFrame, 100, 100, kHorizontalFrame);

      fButtonEnable = new TGTextButton(fFrameCard, "CARD IS ON", eCARD);
      fButtonEnable->Associate(this);
      if(!aDevice->IsEnabled()) fButtonEnable->SetText("CARD IS OFF");
      fButtonEnable->MoveResize(3,3,80,20);
      fFrameCard->AddFrame(fButtonEnable, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));

   aMainFrame->AddFrame(fFrameCard, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 2,2,2,2));


   //----------------------------------------------------------
   // COMMON FRAME
   //----------------------------------------------------------
   fFrameCommon = new TGGroupFrame(aMainFrame, "Common", kHorizontalFrame);
  	fFrame1 = new TGCompositeFrame(fFrameCommon, 100, 100, kVerticalFrame);

	//---------------------------------------------
	fFrame1->AddFrame(new TGLabel(fFrame1, new TGString("RecordLength ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
	fRecordLength = new TGNumberEntry(fFrame1, aDevice->mRecordLength, 6, eTRACKLENGTH,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 196*1024);
	fRecordLength->Associate(this);
	if(!aDevice->IsEnabled()) fRecordLength->SetState(kFALSE);
	fFrame1->AddFrame(fRecordLength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
	//---------------------------------------------
	fFrame1->AddFrame(new TGLabel(fFrame1, new TGString("PostTrigger in [%]")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 8, 0));
	fPostTrigger = new TGNumberEntry(fFrame1, aDevice->mPostTrigger, 6, ePRETRIGGER,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 100);
	fPostTrigger->Associate(this);
	if(!aDevice->IsEnabled()) fPostTrigger->SetState(kFALSE);
	fFrame1->AddFrame(fPostTrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
	//---------------------------------------------
		
       fFrameCommon->AddFrame(fFrame1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2,2,2,2));
	//-----------------------------------------------------
	
	fFrameGroup = new TGGroupFrame(fFrameCommon, "Group", kVerticalFrame);
	for(Int_t i = 0; i < nrGroup; i++){
           sprintf(name, "Gr.%d", i);   
	   fGroupEnableMask[i] = new TGCheckButton(fFrameGroup, name, eGROUP+i);
	   fGroupEnableMask[i]->Associate(this);
	   fGroupEnableMask[i]->SetState(EButtonState(aDevice->mGroupEnableMask[i]));
	   if(!aDevice->IsEnabled()){ 
	      //fGroupEnableMask[i]->SetState(EButtonState(aDevice->mGroupEnableMask[i]));
	      fGroupEnableMask[i]->SetEnabled(kFALSE);
              }
           fFrameGroup->AddFrame(fGroupEnableMask[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 4, 5, 5));
           }
        fFrameCommon->AddFrame(fFrameGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2) ) ;	

	//-----------------------------------------------------
	fFrameOffset = new TGGroupFrame(fFrameCommon, "Offset [mV]", kVerticalFrame); 
		for(Int_t i = 0; i < nrGroup; i++){
   		   fDCoffset[i] = new TGNumberEntry(fFrameOffset, aDevice->mDCoffset_mV[i], 5, eOFFSET + i,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -1000, 1000);
		   fDCoffset[i]->Associate(this);
		   if(!aDevice->IsEnabled() || !fGroupEnableMask[i]->GetState()) fDCoffset[i]->SetState(kFALSE);
                   fFrameOffset->AddFrame(fDCoffset[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 4, 3, 2));
                   }
  
	fFrameCommon->AddFrame(fFrameOffset, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2) ) ;	

	//-----------------------------------------------------
	
	fFrameSave = new TGGroupFrame(fFrameCommon, "Save Channels", kVerticalFrame);   
		fFrameSave->SetLayoutManager(new TGMatrixLayout(fFrameSave, 4, 8, 10, 0 ));
		for(Int_t i = 0; i < nrGroup * nrChannel; i++){
                   sprintf(name, "%d", i);   
		   fCheckButtonSave[i] = new TGCheckButton(fFrameSave, name, eCHANNEL_SAVE+i);
		   fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
		   if(!aDevice->IsEnabled() || !fGroupEnableMask[i/8]->GetState() ){ 
			//fCheckButtonSave[i]->SetState( EButtonState( 0) ); 
			fCheckButtonSave[i]->SetEnabled(kFALSE);
                   	}
		   fCheckButtonSave[i]->Associate(this);
                   fFrameSave->AddFrame(fCheckButtonSave[i]);
                   }

	fFrameCommon->AddFrame(fFrameSave, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2) ) ;	
	
	//-----------------------------------------------------

   aMainFrame->AddFrame(fFrameCommon, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 2, 2, 2, 2) );
   
   //----------------------------------------------------------
   // TRIGGER FRAME
   //----------------------------------------------------------

   fFrameTrigger = new TGGroupFrame(aMainFrame, "Trigger Settings", kVerticalFrame);
   //-----------------------------------------------------
      fFrameTrigger1 = new TGCompositeFrame(fFrameTrigger, 100, 100, kHorizontalFrame);
      fFrameTrigger->AddFrame(fFrameTrigger1, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 2,2,2,2));

      fFrameTrigger11 = new TGGroupFrame(fFrameTrigger1, "Channels Group Trigger", kVerticalFrame);
      fFrameTrigger1->AddFrame(fFrameTrigger11, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1) ) ;	

      fFrameTrigger11->SetLayoutManager(new TGMatrixLayout(fFrameTrigger11, 5, 4, 10, 0 ));

      fFrameTrigger11->AddFrame(new TGLabel(fFrameTrigger11, new TGString(" ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
      fFrameTrigger11->AddFrame(new TGLabel(fFrameTrigger11, new TGString("Mode" )), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
      fFrameTrigger11->AddFrame(new TGLabel(fFrameTrigger11, new TGString("Threshold [mV]")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
      fFrameTrigger11->AddFrame(new TGLabel(fFrameTrigger11, new TGString("Trigger Edge")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));

      for(Int_t i = 0; i < 4; i++){
          sprintf(name, "Group Trg.%d", i);   
          fFrameTrigger11->AddFrame(new TGLabel(fFrameTrigger11, new TGString( name)), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));

          fChannelTriggerMode[i] = new TGComboBox(fFrameTrigger11, eCHNTRIGGERMODE + i);
          fFrameTrigger11->AddFrame(fChannelTriggerMode[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 4, 10, 4));
          fChannelTriggerMode[i]->AddEntry("Disabled", 0);
          fChannelTriggerMode[i]->AddEntry("Acq Only", 1);
          fChannelTriggerMode[i]->AddEntry("Ext Only", 2);
          fChannelTriggerMode[i]->AddEntry("Ext & Acq",3);
          fChannelTriggerMode[i]->Select( aDevice->mChannelTriggerMode[i] );
          if(!aDevice->IsEnabled()) fChannelTriggerMode[i]->SetEnabled(kFALSE);
          fChannelTriggerMode[i]->Resize(75,22);
          fChannelTriggerMode[i]->Associate(this);

          fThreshold[i] = new TGNumberEntry(fFrameTrigger11, aDevice->mThreshold_mV[i], 6, eTRIGGER_THRESHOLD+i,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -1000, 1000);
          fThreshold[i]->Associate(this);
          fFrameTrigger11->AddFrame(fThreshold[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 4, 10, 4));
          if(!aDevice->IsEnabled() || !fChannelTriggerMode[i]->GetSelected()) fThreshold[i]->SetState(kFALSE);

          fChannelTriggerEdge[i] = new TGComboBox(fFrameTrigger11, eTRIGGER_EDGE);
          fFrameTrigger11->AddFrame(fChannelTriggerEdge[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 4, 10, 4));
          fChannelTriggerEdge[i]->AddEntry("Rising", 0);
          fChannelTriggerEdge[i]->AddEntry("Falling", 1);
          fChannelTriggerEdge[i]->Select( aDevice->mTriggerEdge[i] );
          if(!aDevice->IsEnabled() || !fChannelTriggerMode[i]->GetSelected() ) fChannelTriggerEdge[i]->SetEnabled(kFALSE);
          fChannelTriggerEdge[i]->Resize(75,22);
          fChannelTriggerEdge[i]->Associate(this);


          }
        
       //---------------------------------------------------------

       
       fFrameTrigger12 = new TGGroupFrame(fFrameTrigger1, "Channel contribution to build local trigger", kVerticalFrame);
       //fFrameTrigger12 = new TGCompositeFrame(fFrameTrigger1, 100,100, kVerticalFrame);
       fFrameTrigger1->AddFrame(fFrameTrigger12, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 1, 1, 0, 1) ) ;

       fFrameTrigger12->SetLayoutManager(new TGMatrixLayout(fFrameTrigger12, 5, 8, 14, 0 ));
       for(Int_t i = 0; i < 8; i++)
	 fFrameTrigger12->AddFrame(new TGLabel(fFrameTrigger12, new TGString(" ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));

	for(Int_t i = 0; i < nrGroup * nrChannel; i++){
	   sprintf(name, "%d", i);   
	   fSelfTriggerMask[i] = new TGCheckButton(fFrameTrigger12, name, eTRIGGER_SELFMASK + i);
	   fSelfTriggerMask[i]->SetState( EButtonState( Get_Bit( aDevice->mSelfTriggerMask[i/8], i/8 ) ) );
	   if(!aDevice->IsEnabled() || !fChannelTriggerMode[i/8]->GetSelected() ){ 
		//fCheckButtonSave[i]->SetState( EButtonState( 0) ); 
		fSelfTriggerMask[i]->SetEnabled(kFALSE);
		}
	   fSelfTriggerMask[i]->Associate(this);
	   fFrameTrigger12->AddFrame(fSelfTriggerMask[i]);
	   }

	


/* 
       fFrameTrigger12->SetLayoutBroken(kTRUE);
       Int_t row = 4, col = 8; 
       //---------------------------------------------------------
       for(Int_t i = 0; i < row; i++) {
           for(Int_t j = 0; j < col; j++) {
              sprintf(name,"ch.%2d",i*col+j); 
              fCheckButtonTrigger[i*col+j] = new TGCheckButton(fFrameTrigger12, name, eTRIGGER_ENABLE+i*col+j);
              fChannelPolarity[i*col+j]    = new TGTextButton(fFrameTrigger12, "NEG", eCHANNEL_POLARITY+i*col+j);
              fTriggerEdge[i*col+j]        = new TGTextButton(fFrameTrigger12, "Fall", eTRIGGER_EDGE+i*col+j);
              fCheckButtonTrigger[i*col+j]->Associate(this); 
              fChannelPolarity[i*col+j]   ->Associate(this);
              fTriggerEdge[i*col+j]       ->Associate(this); 
 
              fCheckButtonTrigger[i*col+j]->MoveResize(j*70+10, i*39 + 25, 50, 20); 
              fChannelPolarity[i*col+j]   ->MoveResize(j*70+10, i*39 + 44, 30, 20); 
              fTriggerEdge[i*col+j]       ->MoveResize(j*70+40, i*39 + 44, 30, 20); 
              fFrameTrigger12->AddFrame(fCheckButtonTrigger[i*col+j], new TGLayoutHints(kLHintsLeft | kLHintsBottom, 0, 0, 0, 0));
              fFrameTrigger12->AddFrame(fChannelPolarity[i*col+j],    new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0)); 
              fFrameTrigger12->AddFrame(fTriggerEdge[i*col+j],        new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
              }
           }
*/
       
        //---------------------------------------------------------
       fFrameTrigger2 = new TGCompositeFrame(fFrameTrigger, 600, 240, kHorizontalFrame);
       fFrameTrigger->AddFrame(fFrameTrigger2, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 2,2,2,2));

       fFrameTrigger21 = new TGGroupFrame(fFrameTrigger2, "External Trigger", kHorizontalFrame);
       fFrameTrigger2->AddFrame(fFrameTrigger21, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1) ) ;	

       fExtTriggerMode = new TGComboBox(fFrameTrigger21, eEXTTRIGGERMODE);
       fFrameTrigger21->AddFrame(fExtTriggerMode, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 3, 3, 3));
       fExtTriggerMode->AddEntry("Disabled", 0);
       fExtTriggerMode->AddEntry("Acq Only", 1);
       fExtTriggerMode->AddEntry("Ext Only", 2);
       fExtTriggerMode->AddEntry("Ext & Acq",3);
       fExtTriggerMode->Select(0);
       fExtTriggerMode->Resize(75,22);
       fExtTriggerMode->Associate(this);
       if(!aDevice->IsEnabled()) fExtTriggerMode->SetEnabled(kFALSE);

       fFrameTrigger22 = new TGGroupFrame(fFrameTrigger2, "I/O Panel", kHorizontalFrame);
       fFrameTrigger2->AddFrame(fFrameTrigger22, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1) ) ;	

       fFPIOtype  = new TGTextButton(fFrameTrigger22, "   TTL   ",  eGPIO);
       fFPIOtype->Associate(this);
       fFPIOtype->Resize(87,22);
       fFrameTrigger22->AddFrame(fFPIOtype, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 1, 5, 1));
/*
       fFrameTrigger23 = new TGGroupFrame(fFrameTrigger2, "Coincidence", kHorizontalFrame);
       fFrameTrigger2->AddFrame(fFrameTrigger23, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1) ) ;	
  
       fCoincidence = new TGNumberEntry(fFrameTrigger23, 0, 5, eCOINCIDENCE,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 32);
       fFrameTrigger23->AddFrame(fCoincidence, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 5, 1) ) ;
*/	
        //-----------------------------------------------------
   aMainFrame->AddFrame(fFrameTrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2) );

 
//   Refresh();

}

//==================================================================

SimCardDlg::~SimCardDlg() {
 #ifdef DEBUG
    if(gDEBUG > 2)
      cout << "DEBUG [SimCardDlg::Destructor]" << endl;
  #endif
 aDevice->mGuiWindow = NULL;
 Cleanup(); //delete all widgets, frames, subframes and layouts
}

//==================================================================

void SimCardDlg::Refresh() {
  #ifdef DEBUG
  cout << "DEBUG [SimCard5470Dlg::Refresh] " << endl;
  #endif

   if(aDevice->IsEnabled()){
        fButtonEnable->SetText("Card is ON");
        fRecordLength->SetState(kTRUE);
        fPostTrigger->SetState(kTRUE);
	fFPIOtype->SetEnabled(kTRUE);
	fExtTriggerMode->SetEnabled(kTRUE);
        for(Int_t i = 0; i < 4; i++){
	   fGroupEnableMask[i]->SetEnabled(kTRUE);
	   fDCoffset[i]->SetState(kTRUE);
	   fThreshold[i]->SetState(kTRUE);
           fChannelTriggerEdge[i]->SetEnabled(kTRUE);
	   //fSelfTriggerMask[i]->SetEnabled(kTRUE);
	   fChannelTriggerMode[i]->SetEnabled(kTRUE);  
	   }

        for(Int_t i = 0; i < 32; i++){
	   fCheckButtonSave[i]->SetState(EButtonState( Get_Bit(aDevice->mSaveChannel, i)  ));
           if(fGroupEnableMask[i/8]->GetState()){ 
	      fCheckButtonSave[i]->SetEnabled( kFALSE);
              }

           fSelfTriggerMask[i]->SetState(EButtonState( Get_Bit(aDevice->mSelfTriggerMask[i/8], i-(i/8)*8)  ));
 //          cout << "fSelfTriggerMask "<< i << "\tmSelfTriggerMask " << i/8 << "\tbit " << i - (i/8)*8 << endl;
           if(!fChannelTriggerMode[i/8]->GetSelected()){ 
	      fSelfTriggerMask[i]->SetEnabled( kFALSE);
              }
  	   
           }	
     
     }
   else { 
        fButtonEnable->SetText("Card is OFF");
        fRecordLength->SetState(kFALSE);
        fPostTrigger->SetState(kFALSE);
	fExtTriggerMode->SetEnabled(kFALSE);
	fFPIOtype->SetEnabled(kFALSE);
        for(Int_t i = 0; i < 4; i++){
	   fGroupEnableMask[i]->SetEnabled(kFALSE);
	   fDCoffset[i]->SetState(kFALSE);
	   fThreshold[i]->SetState(kFALSE);
           fChannelTriggerEdge[i]->SetEnabled(kFALSE);
	  // fSelfTriggerMask[i]->SetEnabled(kFALSE);
	   fChannelTriggerMode[i]->SetEnabled(kFALSE);  
	   aDevice->mSelfTriggerMask[i] = 0;;
	   }
        for(Int_t i = 0; i < 32; i++){
           Reset_Bit(aDevice->mSaveChannel, i);
	   fCheckButtonSave[i]->SetState(EButtonState( Get_Bit(aDevice->mSaveChannel, i)  ));
	   fCheckButtonSave[i]->SetEnabled(kFALSE);

           fSelfTriggerMask[i]->SetState(EButtonState( Get_Bit(aDevice->mSelfTriggerMask[i/8], i/8 )));
	   fSelfTriggerMask[i]->SetEnabled(kFALSE);
           }	
     return;
     } 

  //------------------------------------------------------
 
   if(aDevice->mFPIOtype) fFPIOtype->SetText("   TTL   ");
   else                   fFPIOtype->SetText("   NIM   ");

   fRecordLength->SetNumber(aDevice->mRecordLength);
   fPostTrigger->SetNumber(aDevice->mPostTrigger);
   
   for(UInt_t i = 0; i < 4; i++){
       fGroupEnableMask[i]->SetState( EButtonState( aDevice->mGroupEnableMask[i]  ));
       if(fGroupEnableMask[i]->GetState()){
         fDCoffset[i]->SetState(kTRUE);
         }
       else{
         fDCoffset[i]->SetState(kFALSE);
         }
       //fDCoffset[i]->SetNumber( aDevice->mDCoffset[i]);
       }
   
   for(UInt_t i = 0; i < 4; i++){
       if(!fChannelTriggerMode[i]->GetSelected()){ 
          fThreshold[i]->SetState(kFALSE);
          fChannelTriggerEdge[i]->SetEnabled(kFALSE);
          }
       else{  
          fThreshold[i]->SetState(kTRUE);
          fChannelTriggerEdge[i]->SetEnabled(kTRUE);
         }
       }


   for(UInt_t i = 0; i < 32; i++){
	   fCheckButtonSave[i]->SetState(EButtonState( Get_Bit(aDevice->mSaveChannel, i)  ));
           if(!fGroupEnableMask[i/8]->GetState()){ 
	      fCheckButtonSave[i]->SetEnabled( kFALSE);
              }
      }


//   for(UInt_t i = 0; i < 32; i++){
//      if(!fChannelTriggerMode[i]->GetSelected()){ 
//	fSelfTriggerMask[i]->SetState(EButtonState( kFALSE));
//        }
//       else{  
//	fSelfTriggerMask[i]->SetState(EButtonState( kTRUE ));
//         }
//      }

}

//==================================================================
Bool_t SimCardDlg::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

// Long_t msg1 = msg;
// if(msg > 1001){
//   cout << "----------------- message: " << msg1 << "\tGET_MSG: " << GET_MSG(msg1) << "\tGET_SUBMSG: " << GET_SUBMSG(msg1) << endl;
//   return kTRUE; // to avoid messages at the beginning.... there are 6 unknown  
//  }

//cout << "DEBUG [SimCardDlg::ProcessMessage] MSG: " << msg1 << "\tGET_MSG: " << GET_MSG(msg1) << "\tGET_SUBMSG: " << GET_SUBMSG(msg1) << endl;
//cout << "DEBUG [SimCardDlg::ProcessMessage] MSG: " << msg << "\tparm1: " << parm1 << "\tparm2: " << parm2 << endl;


  switch(GET_MSG(msg)) {
  //-
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    //-- 
    case kCM_BUTTON:
      if (parm1 == eCARD){ if( aDevice->IsEnabled() ) aDevice->DoEnable(kFALSE); else aDevice->DoEnable(kTRUE); }
      if (parm1 == eGPIO){ aDevice->mFPIOtype = (aDevice->mFPIOtype) ? 0 : 1 ; }                                 
    break;

    case kCM_CHECKBUTTON:
      if (parm1 == eGROUP + 0) {
         if(!fGroupEnableMask[0]->GetState()){
           aDevice->mGroupEnableMask[0] = 0;
           aDevice->mSaveChannel &= 0xFFFFFF00;
           }
         else{
           aDevice->mGroupEnableMask[0] = 1;
           aDevice->mSaveChannel |= 0x000000FF;
           }   
      } 
      if (parm1 == eGROUP + 1) {
         if(!fGroupEnableMask[1]->GetState()){
           aDevice->mGroupEnableMask[1] = 0;
           aDevice->mSaveChannel &= 0xFFFF00FF;
           }
         else{
           aDevice->mGroupEnableMask[1] = 1;
           aDevice->mSaveChannel |= 0x0000FF00;
           }   
      }
      if (parm1 == eGROUP + 2) {
         if(!fGroupEnableMask[2]->GetState()){
           aDevice->mGroupEnableMask[2] = 0;
           aDevice->mSaveChannel &= 0xFF00FFFF;
           }
         else{
           aDevice->mGroupEnableMask[2] = 1;
           aDevice->mSaveChannel |= 0x00FF0000;
           }   
      }
      if (parm1 == eGROUP + 3) {
         if(!fGroupEnableMask[3]->GetState()){
           aDevice->mGroupEnableMask[3] = 0;
           aDevice->mSaveChannel &= 0x00FFFFFF;
           }
         else{
           aDevice->mGroupEnableMask[3] = 1;
           aDevice->mSaveChannel |= 0xFF000000;
           }   
      }


      if (parm1 >= eTRIGGER_SELFMASK + 0  && parm1 < eTRIGGER_SELFMASK + 8){
          Flip_Bit(aDevice->mSelfTriggerMask[0], parm1 - eTRIGGER_SELFMASK);
         }
      if (parm1 >= eTRIGGER_SELFMASK + 8  && parm1 < eTRIGGER_SELFMASK + 16){
          Flip_Bit(aDevice->mSelfTriggerMask[1], parm1 - eTRIGGER_SELFMASK - 8);
         }
      if (parm1 >= eTRIGGER_SELFMASK + 16 && parm1 < eTRIGGER_SELFMASK + 24){
          Flip_Bit(aDevice->mSelfTriggerMask[2], parm1 - eTRIGGER_SELFMASK - 16);
         }
      if (parm1 >= eTRIGGER_SELFMASK + 24 && parm1 < eTRIGGER_SELFMASK + 32){
          Flip_Bit(aDevice->mSelfTriggerMask[3], parm1 - eTRIGGER_SELFMASK -24);
         }

	
      if (parm1 >= eCHANNEL_SAVE   && parm1 < eCHANNEL_SAVE+32) {
          Flip_Bit(aDevice->mSaveChannel, parm1 - eCHANNEL_SAVE);
          //cout << "mSaveChannel= " << std::bitset<32> ( aDevice->mSaveChannel )<< " parm1= " << parm1 << " parm1-eCHANNEL_SAVE = " << parm1 - eCHANNEL_SAVE << endl;
          }
    break; 
      
    case kCM_COMBOBOX:
      if (parm1 == eEXTTRIGGERMODE)	aDevice->mExtTriggerMode = parm2;

      if (parm1 == eCHNTRIGGERMODE+0)	{
          aDevice->mChannelTriggerMode[0] = parm2; 
          //cout << "mChannelTriggerMode[0] = " << aDevice->mChannelTriggerMode[0] << endl;
          }
      if (parm1 == eCHNTRIGGERMODE+1)	{
          aDevice->mChannelTriggerMode[1] = parm2; 
          //cout << "mChannelTriggerMode[1] = " << aDevice->mChannelTriggerMode[1] << endl;
          }
      if (parm1 == eCHNTRIGGERMODE+2)	{
          aDevice->mChannelTriggerMode[2] = parm2; 
          //cout << "mChannelTriggerMode[2] = " << aDevice->mChannelTriggerMode[2] << endl;
          }
      if (parm1 == eCHNTRIGGERMODE+3)	{
          aDevice->mChannelTriggerMode[3] = parm2; 
          //cout << "mChannelTriggerMode[3] = " << aDevice->mChannelTriggerMode[3] << endl;
          }

    break;

    default:
      cout << "Default kCM_CHECKBUTTON " << kCM_CHECKBUTTON << endl;
    break;
    //--
    }
  break;
  //-
  case kC_TEXTENTRY:
    switch(GET_SUBMSG(msg)) {
       case kTE_TEXTCHANGED:
	 if (parm1 == eTRACKLENGTH)		{aDevice->mRecordLength = fRecordLength->GetNumber(); cout << "kTE_TEXTCHANGED eTRACKLENGTH"<< endl;}
         if (parm1 == ePRETRIGGER)  		{aDevice->mPostTrigger  = fPostTrigger->GetNumber();  cout << "kTE_TEXTCHANGED ePRETRIGGER"<< endl;}
         if (parm1 == eOFFSET + 0)  		{
                 aDevice->mDCoffset_mV[0]  =  -fDCoffset[0]->GetNumber(); 
                 //aDevice->mDCoffset[0]  =  aDevice->mV2DAC(-fDCoffset[0]->GetNumber());
                 }
         if (parm1 == eOFFSET + 1)  		{
                 aDevice->mDCoffset_mV[1]  =  fDCoffset[1]->GetNumber();
                 //aDevice->mDCoffset[1]  =  aDevice->mV2DAC(fDCoffset[1]->GetNumber());
                 }
         if (parm1 == eOFFSET + 2)  		{
                 aDevice->mDCoffset_mV[2]  =  fDCoffset[2]->GetNumber();
                 //aDevice->mDCoffset[2]  =  aDevice->mV2DAC(fDCoffset[2]->GetNumber());
                 }
         if (parm1 == eOFFSET + 3)  		{
                 aDevice->mDCoffset_mV[3]  =  fDCoffset[3]->GetNumber();
                 //aDevice->mDCoffset[3]  =  aDevice->mV2DAC(fDCoffset[3]->GetNumber());
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 0)	{
                 aDevice->mThreshold_mV[0] =  fThreshold[0]->GetNumber();
                 //aDevice->mThreshold[0] =  aDevice->mV2ADC(fThreshold[0]->GetNumber());
                 cout << "Threshold[0] = " << aDevice->mThreshold[0] << " " << aDevice->mThreshold_mV[0] <<" offset = " << aDevice->mDCoffset[0] << endl;
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 1)	{
                 aDevice->mThreshold_mV[1] =  fThreshold[1]->GetNumber();
                 //aDevice->mThreshold[1] =  aDevice->mV2ADC(fThreshold[1]->GetNumber());
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 2)	{
                 aDevice->mThreshold_mV[2] =  fThreshold[2]->GetNumber();
                 //aDevice->mThreshold[2] =  aDevice->mV2ADC(fThreshold[2]->GetNumber());
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 3)	{
                 aDevice->mThreshold_mV[3] =  fThreshold[3]->GetNumber();
                 //aDevice->mThreshold[3] =  aDevice->mV2ADC(fThreshold[3]->GetNumber());
                 }
         // cout << "Default kTE_TEXTCHANGED " << kTE_TEXTCHANGED << endl;
       break;
       case kTE_ENTER:
         if (parm1 == eTRACKLENGTH) 		cout << "TEXT_ENTER eTRACKLENGTH" << endl; // aDevice->DoEnable(kTRUE);
         if (parm1 == ePRETRIGGER)		cout << "TEXT_ENTER ePRETRIGGER" << endl; 
         if (parm1 == eOFFSET + 0)		cout << "TEXT_ENTER eOFFSET+0" << endl; 
         if (parm1 == eOFFSET + 1)		cout << "TEXT_ENTER eOFFSET+1" << endl; 
         if (parm1 == eOFFSET + 2)		cout << "TEXT_ENTER eOFFSET+2" << endl; 
         if (parm1 == eOFFSET + 3)		cout << "TEXT_ENTER eOFFSET+3" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 0)	cout << "TEXT_ENTER eTRIGGER_THRESHOLD+0" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 1)	cout << "TEXT_ENTER eTRIGGER_THRESHOLD+1" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 2)	cout << "TEXT_ENTER eTRIGGER_THRESHOLD+2" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 3)	cout << "TEXT_ENTER eTRIGGER_THRESHOLD+3" << endl;   	
       break;
       case kTE_TAB:
        // if (parm1 == eTRACKLENGTH) 		cout << "TEXT_TAB eTRACKLENGTH" << endl; // aDevice->DoEnable(kTRUE);
        // if (parm1 == ePRETRIGGER)		cout << "TEXT_TAB ePRETRIGGER" << endl; 
        // if (parm1 == eOFFSET + 0)		cout << "TEXT_TAB eOFFSET+0" << endl; 
        // if (parm1 == eOFFSET + 1)		cout << "TEXT_TAB eOFFSET+1" << endl; 
        // if (parm1 == eOFFSET + 2)		cout << "TEXT_TAB eOFFSET+2" << endl; 
        // if (parm1 == eOFFSET + 3)		cout << "TEXT_TAB eOFFSET+3" << endl; 
        // if (parm1 == eTRIGGER_THRESHOLD + 0)	cout << "TEXT_TAB eTRIGGER_THRESHOLD+0" << endl; 
        // if (parm1 == eTRIGGER_THRESHOLD + 1)	cout << "TEXT_TAB eTRIGGER_THRESHOLD+1" << endl; 
        // if (parm1 == eTRIGGER_THRESHOLD + 2)	cout << "TEXT_TAB eTRIGGER_THRESHOLD+2" << endl; 
        // if (parm1 == eTRIGGER_THRESHOLD + 3)	cout << "TEXT_TAB eTRIGGER_THRESHOLD+3" << endl;   	
       break;
       case kTE_KEY:
         if (parm1 == eTRACKLENGTH) 		cout << "TEXT_KEY eTRACKLENGTH" << endl; // aDevice->DoEnable(kTRUE);
         if (parm1 == ePRETRIGGER)		cout << "TEXT_KEY ePRETRIGGER" << endl; 
         if (parm1 == eOFFSET + 0)		cout << "TEXT_KEY eOFFSET+0" << endl; 
         if (parm1 == eOFFSET + 1)		cout << "TEXT_KEY eOFFSET+1" << endl; 
         if (parm1 == eOFFSET + 2)		cout << "TEXT_KEY eOFFSET+2" << endl; 
         if (parm1 == eOFFSET + 3)		cout << "TEXT_KEY eOFFSET+3" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 0)	cout << "TEXT_KEY eTRIGGER_THRESHOLD+0" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 1)	cout << "TEXT_KEY eTRIGGER_THRESHOLD+1" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 2)	cout << "TEXT_KEY eTRIGGER_THRESHOLD+2" << endl; 
         if (parm1 == eTRIGGER_THRESHOLD + 3)	cout << "TEXT_KEY eTRIGGER_THRESHOLD+3" << endl;   	
       break;
       default:
          cout << "Default kC_TEXTENTRY " << kC_TEXTENTRY << endl;
       break;
       }
  break;
  
  default:
    cout << "Default kC_COMMAND " << kC_COMMAND << endl;
  break;
  }


  Refresh();
  cout << "======================= ustawiam setConfigure ===========" << endl; 
  aDevice->SetReconfigure(kTRUE);
 
  aDevice->Refresh();
 
  return kTRUE;
}
//-----------------------------------------------------------------------------


