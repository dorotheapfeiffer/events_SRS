/************************************************************************MF 
*
* Implementation GUI class for CAEN 5740 
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
#include <TGTab.h>

#include "CAEN5740Dlg.h"
#include "CAEN5740.h"
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

ClassImp(CAEN5740Dlg)

extern int gDEBUG;

using namespace std;

//==================================================================

CAEN5740Dlg::CAEN5740Dlg( CAEN5740 *aDev, TGCompositeFrame* aMainFrame) {
 #ifdef DEBUG
    if(gDEBUG > 2) cout << "DEBUG [CAEN5740Dlg::Constructor]" << endl;
  #endif

   aDevice = aDev;
   Int_t nrGroup = 4;
   Int_t nrChannel = 8; // per group

   char name[30];

    aMainFrame->SetLayoutManager(new TGHorizontalLayout(aMainFrame));
    fFrameChannel = new TGCompositeFrame(aMainFrame, 100, 100, kHorizontalFrame);

    //-----------------------------------------------------------------------------------------------------------------
    fFrame1 = new TGGroupFrame(fFrameChannel, "Samples", kVerticalFrame);
    //-----------
    	fFrame1->AddFrame(new TGLabel(fFrame1, new TGString("Length    ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
    	fRecordLength = new TGNumberEntry(fFrame1, aDevice->mRecordLength, 6, eTRACKLENGTH,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 196*1024);
        fRecordLength->Connect("ValueSet(Long_t)", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
        (fRecordLength->GetNumberEntry())->Connect("ReturnPressed()", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
    
    	if(!aDevice->IsEnabled()) 
		fRecordLength->SetState(kFALSE);
  
    	fFrame1->AddFrame(fRecordLength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
    //-----------
    	fFrame1->AddFrame(new TGLabel(fFrame1, new TGString("PostTrigger[%]")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 8, 0));
    	fPostTrigger = new TGNumberEntry(fFrame1, aDevice->mPostTrigger, 6, ePRETRIGGER,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 100);
        fPostTrigger->Connect("ValueSet(Long_t)", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
        (fPostTrigger->GetNumberEntry())->Connect("ReturnPressed()", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
    
    	if(!aDevice->IsEnabled()) 
		fPostTrigger->SetState(kFALSE);
	
        fFrame1->AddFrame(fPostTrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
    //-----------
    fFrameChannel->AddFrame(fFrame1, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, 1, 1, 1, 1));
    //-----------------------------------------------------------------------------------------------------------------
    

    fFrame2 = new TGGroupFrame(fFrameChannel, "Group ------- Offset", kHorizontalFrame);
    //-----------
        fFrame21 = new TGCompositeFrame(fFrame2, 100, 100, kVerticalFrame);
	for(Int_t i = 0; i < nrGroup; i++){
           sprintf(name, "Gr.%d", i);   
	   fGroupEnableMask[i] = new TGCheckButton(fFrame21, name, eGROUP+i);
	   fGroupEnableMask[i]->Connect("Toggled(Bool_t)", "CAEN5740Dlg", this, "CheckButtonSet(Bool_t)");
	   fGroupEnableMask[i]->Connect("Toggled(Bool_t)", "CAEN5740Dlg", this, "EnableGroup(Bool_t)");
	   fGroupEnableMask[i]->SetState(EButtonState(aDevice->mGroupEnableMask[i]));
	   if(!aDevice->IsEnabled()){ 
	      //fGroupEnableMask[i]->SetState(EButtonState(aDevice->mGroupEnableMask[i]));
	      fGroupEnableMask[i]->SetEnabled(kFALSE);
              }
                                                                                               // LEFT, RIGHT, TOP, BOTTOM
           fFrame21->AddFrame(fGroupEnableMask[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 9, 0));
           }
         fFrame2->AddFrame(fFrame21, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
        
 
        fFrame22 = new TGCompositeFrame(fFrame2, 100, 100, kVerticalFrame);
        for(Int_t i = 0; i < nrGroup; i++){
   	   fDCoffset[i] = new TGNumberEntry(fFrame22, aDevice->mDCoffset_mV[i], 5, eOFFSET + i,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -1000, 1000);
           fDCoffset[i]->Connect("ValueSet(Long_t)", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
           (fDCoffset[i]->GetNumberEntry())->Connect("ReturnPressed()", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
	    if(!aDevice->IsEnabled() || !fGroupEnableMask[i]->GetState()) 
 		fDCoffset[i]->SetState(kFALSE);
       
            fFrame22->AddFrame(fDCoffset[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 5, 0));
            }
  
         fFrame2->AddFrame(fFrame22, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, -7));

    //-----------
    fFrameChannel->AddFrame(fFrame2, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, 1, 1, 1, 1));


    fFrame3 = new TGGroupFrame(fFrameChannel, "Save", kHorizontalFrame);
    //-----------
	fFrame3->SetLayoutManager(new TGMatrixLayout(fFrame3, 4, 8, 10, 0 ));
	for(Int_t i = 0; i < nrGroup * nrChannel; i++){
           sprintf(name, "%d", i);   
           fCheckButtonSave[i] = new TGCheckButton(fFrame3, name, eCHANNEL_SAVE+i);
	   fCheckButtonSave[i]->Connect("Toggled(Bool_t)", "CAEN5740Dlg", this, "CheckButtonSet(Bool_t)");
  	   fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
           if(!aDevice->IsEnabled() || !fGroupEnableMask[i/8]->GetState() ){ 
		//fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) ); 
		fCheckButtonSave[i]->SetEnabled(kFALSE);
             }
           fFrame3->AddFrame(fCheckButtonSave[i]);
          }


    //-----------
    fFrameChannel->AddFrame(fFrame3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));



   //======================================================================================================
    fFrameTrigger = new TGCompositeFrame(aMainFrame, 100, 100, kHorizontalFrame);
          fFrameTg1 = new TGCompositeFrame(fFrameTrigger, 100, 100, kVerticalFrame);
    	  fFrameTg1->AddFrame(new TGLabel(fFrameTg1, new TGString("         ")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 3, 3));
    	  fFrameTg1->AddFrame(new TGLabel(fFrameTg1, new TGString("Gr.1")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 2, 3));
    	  fFrameTg1->AddFrame(new TGLabel(fFrameTg1, new TGString("Gr.2")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 3, 3));
    	  fFrameTg1->AddFrame(new TGLabel(fFrameTg1, new TGString("Gr.3")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 5, 3));
    	  fFrameTg1->AddFrame(new TGLabel(fFrameTg1, new TGString("Gr.4")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 5, 3));
    	  fFrameTg1->AddFrame(new TGLabel(fFrameTg1, new TGString("Ext.")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 5, 3));

         fFrameTrigger->AddFrame(fFrameTg1, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
         //------------
         fFrameTg2 = new TGCompositeFrame(fFrameTrigger, 100, 100, kVerticalFrame);
    	    fFrameTg2->AddFrame(new TGLabel(fFrameTg2, new TGString("Mode")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
            for(Int_t i = 0; i < 4; i++){
               fChannelTriggerMode[i] = new TGComboBox(fFrameTg2, eCHNTRIGGERMODE + i);
               fChannelTriggerMode[i]->Connect("Selected(Int_t)", "CAEN5740Dlg", this, "ComboBoxSet(Int_t)");
               fFrameTg2->AddFrame(fChannelTriggerMode[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
               fChannelTriggerMode[i]->AddEntry("Disabled", 0);
               fChannelTriggerMode[i]->AddEntry("Acq Only", 1);
               fChannelTriggerMode[i]->AddEntry("Ext Only", 2);
               fChannelTriggerMode[i]->AddEntry("Ext & Acq",3);
               fChannelTriggerMode[i]->Select( aDevice->mChannelTriggerMode[i], kFALSE );
               if(!aDevice->IsEnabled()) 
		 fChannelTriggerMode[i]->SetEnabled(kFALSE);
               fChannelTriggerMode[i]->Resize(80,22);
               }
           //-------
             fExtTriggerMode = new TGComboBox(fFrameTg2, eEXTTRIGGERMODE);
             fExtTriggerMode->Connect("Selected(Int_t)", "CAEN5740Dlg", this, "ComboBoxSet(Int_t)");
             fFrameTg2->AddFrame(fExtTriggerMode, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
             fExtTriggerMode->AddEntry("Disabled", 0);
             fExtTriggerMode->AddEntry("Acq Only", 1);
             fExtTriggerMode->AddEntry("Ext Only", 2);
             fExtTriggerMode->AddEntry("Ext & Acq",3);
             fExtTriggerMode->Select(aDevice->mExtTriggerMode, kFALSE);
             fExtTriggerMode->Resize(80,22);
             if(!aDevice->IsEnabled()) 
		fExtTriggerMode->SetEnabled(kFALSE);

         fFrameTrigger->AddFrame(fFrameTg2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
         //------------
         fFrameTg3 = new TGCompositeFrame(fFrameTrigger, 100, 100, kVerticalFrame);
    	    fFrameTg3->AddFrame(new TGLabel(fFrameTg3, new TGString("Threshold[mV]")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
            for(Int_t i = 0; i < 4; i++){
                fThreshold[i] = new TGNumberEntry(fFrameTg3, aDevice->mThreshold_mV[i], 10, eTRIGGER_THRESHOLD+i,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -1000, 1000);
                fThreshold[i]->Connect("ValueSet(Long_t)", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
                (fThreshold[i]->GetNumberEntry())->Connect("ReturnPressed()", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
                fFrameTg3->AddFrame(fThreshold[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
                if(!aDevice->IsEnabled() || !fChannelTriggerMode[i]->GetSelected()) 
		   fThreshold[i]->SetState(kFALSE);
                }

         fFrameTrigger->AddFrame(fFrameTg3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
         //------------
         fFrameTg4 = new TGCompositeFrame(fFrameTrigger, 100, 100, kVerticalFrame);
    	    fFrameTg4->AddFrame(new TGLabel(fFrameTg4, new TGString("Trg Slope")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
            for(Int_t i = 0; i < 4; i++){
               fChannelTriggerEdge[i] = new TGComboBox(fFrameTg4, eTRIGGER_EDGE + i);
               fChannelTriggerEdge[i]->Connect("Selected(Int_t)", "CAEN5740Dlg", this, "ComboBoxSet(Int_t)");
               fFrameTg4->AddFrame(fChannelTriggerEdge[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
               fChannelTriggerEdge[i]->AddEntry("Rising", 0);
               fChannelTriggerEdge[i]->AddEntry("Falling", 1);
               fChannelTriggerEdge[i]->Select( aDevice->mTriggerEdge[i], kFALSE );
               if(!aDevice->IsEnabled() || !fChannelTriggerMode[i]->GetSelected() ) 
                 fChannelTriggerEdge[i]->SetEnabled(kFALSE);
               fChannelTriggerEdge[i]->Resize(70,22);
               }

         fFrameTrigger->AddFrame(fFrameTg4, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
         //------------
         fFrameTg5 = new TGCompositeFrame(fFrameTrigger, 100, 100, kVerticalFrame);
    	    fFrameTg5->AddFrame(new TGLabel(fFrameTg5, new TGString("Channel contribution to build local trigger")),new TGLayoutHints(kLHintsLeft | kLHintsTop,1,1,1,1));
            fFrameTg6 = new TGCompositeFrame(fFrameTg5, 100, 100, kVerticalFrame);
            fFrameTg5->AddFrame(fFrameTg6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
   
            fFrameTg6->SetLayoutManager(new TGMatrixLayout(fFrameTg6, nrGroup, nrChannel, 6, 1 ));
	    for(Int_t i = 0; i < nrGroup * nrChannel; i++){
	        sprintf(name, "%d", i);   
	        fSelfTriggerMask[i] = new TGCheckButton(fFrameTg6, name, eTRIGGER_SELFMASK + i);
	        fSelfTriggerMask[i]->Connect("Toggled(Bool_t)", "CAEN5740Dlg", this, "CheckButtonSet(Bool_t)");
	        fSelfTriggerMask[i]->SetState( EButtonState( aDevice->mSelfTriggerMask[i] ) );
	        //fSelfTriggerMask[i]->SetState( EButtonState( Get_Bit( aDevice->mSelfTriggerMask[i/8], i % 8 ) ) );
	        if(!aDevice->IsEnabled() || !fChannelTriggerMode[i/8]->GetSelected() ){ 
		    fSelfTriggerMask[i]->SetEnabled(kFALSE);
		    }
	        fFrameTg6->AddFrame(fSelfTriggerMask[i]);
	       }
         fFrameTrigger->AddFrame(fFrameTg5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
         //------------
	 fFrameTg7 = new TGCompositeFrame(fFrameTrigger, 100, 100, kVerticalFrame);
    	    fFrameTg7->AddFrame(new TGLabel(fFrameTg7, new TGString("I/O level")),new TGLayoutHints(kLHintsLeft | kLHintsTop,4,1,1,1));
            if(aDevice->mFPIOtype) strcpy(name, "   TTL   ");
	    else strcpy(name, "   NIM   ");
            fFPIOtype  = new TGTextButton(fFrameTg7, name,  eGPIO);

	    fFPIOtype->Connect("Clicked()", "CAEN5740Dlg", this, "ButtonClicked()");
            fFrameTg7->AddFrame(fFPIOtype, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 1, 5, 1));
            fFPIOtype->Resize(87,22);
         fFrameTrigger->AddFrame(fFrameTg7, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
         //------------


         //This MUST be at the end! after creation fThreshold, fChennelTriggerEdge and fSelfTriggerMask !!!
	    for(Int_t i = 0; i < nrGroup; i++){
               fChannelTriggerMode[i]->Connect("Selected(Int_t)", "CAEN5740Dlg", this, "EnableTrigger(Int_t)");
		}

   //======================================================================================================
   fFrameOptions = new TGCompositeFrame(aMainFrame, 100, 100, kHorizontalFrame);

    //--
      fFrameOp1 = new TGGroupFrame(fFrameOptions, "Daisy chain", kVerticalFrame);
      fChBtStart = new TGCheckButton(fFrameOp1, "", eCHBTSTART);
      fChBtStart->Connect("Toggled(Bool_t)", "CAEN5740Dlg", this, "CheckButtonSet(Bool_t)");
      fChBtStart->SetState( EButtonState( aDevice->mAcqMode ) );
      if( !aDevice->IsEnabled() ){ 
	  fChBtStart->SetEnabled(kFALSE);
      }
      fFrameOp1->AddFrame(fChBtStart);
    
    //--
   fFrameOp1->AddFrame(new TGLabel(fFrameOp1, new TGString( "Acq Mode")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
    //--
    fAcqMode = new TGComboBox(fFrameOp1, eACQ_MODE);
    fAcqMode->Connect("Selected(Int_t)", "CAEN5740Dlg", this, "ComboBoxSet(Int_t)");
    fAcqMode->AddEntry("Software", 0);
    fAcqMode->AddEntry("Hardware", 1);
    fAcqMode->Select( aDevice->mAcqMode, kFALSE );
    if(!aDevice->IsEnabled() || !fChBtStart->GetState() )
       fAcqMode->SetEnabled(kFALSE);
    fAcqMode->Resize(70,22);
      
     fFrameOp1->AddFrame(fAcqMode);
    //--
    fFrameOp1->AddFrame(new TGLabel(fFrameOp1, new TGString( "Delay in CLK")), new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 0, 1));
    //--
    fDelay = new TGNumberEntry(fFrameOp1, aDevice->mDelay, 10, eTRIGGEROUT_DELAY,
                          TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 100);
    fDelay->Connect("ValueSet(Long_t)", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
    (fDelay->GetNumberEntry())->Connect("ReturnPressed()", "CAEN5740Dlg", this, "NumberEntrySet(Long_t)");
    fFrameOp1->AddFrame(fDelay, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 1, 1, 1));
    if(!aDevice->IsEnabled() || !fChBtStart->GetState()){ 
      fDelay->SetState(kFALSE);
      }
    fFrameOptions->AddFrame(fFrameOp1, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, 1, 1, 1, 1));
    

   fTab = new TGTab(aMainFrame, 700, 500);
   TGLayoutHints* fTLXY = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);
   fTab->AddTab("Channel", fFrameChannel);
   fTab->AddTab("Trigger", fFrameTrigger);
   fTab->AddTab("Options", fFrameOptions);
   fTab->SetTab(0);
   fTab->Resize();
   aMainFrame->AddFrame(fTab, fTLXY);

}

//==================================================================

CAEN5740Dlg::~CAEN5740Dlg() {
 #ifdef DEBUG
    if(gDEBUG > 2)
      cout << "DEBUG [CAEN5740Dlg::Destructor]" << endl;
  #endif

   Int_t nrGroup = 4;
   Int_t nrChannel = 8;
   aDevice->mGuiWindow = NULL;


   fPostTrigger->Disconnect("ValueSet(Long_t)");
   (fPostTrigger->GetNumberEntry())->Disconnect("ReturnPressed()");
   fRecordLength->Disconnect("ValueSet(Long_t)");
   (fRecordLength->GetNumberEntry())->Disconnect("ReturnPressed()");


 for(Int_t i = 0; i < nrGroup; i++){
    fChannelTriggerMode[i]->Disconnect("Selected(Int_t)");
    fChannelTriggerMode[i]->Disconnect("Selected(Int_t)");
    fChannelTriggerEdge[i]->Disconnect("Selected(Int_t)");

    fDCoffset[i]->Disconnect("ValueSet(Long_t)");
    (fDCoffset[i]->GetNumberEntry())->Disconnect("ReturnPressed()");
    fThreshold[i]->Disconnect("ValueSet(Long_t)");
    (fThreshold[i]->GetNumberEntry())->Disconnect("ReturnPressed()");

    fGroupEnableMask[i]->Disconnect("Toggled(Bool_t)");
    }

 for(Int_t i = 0; i < nrGroup * nrChannel; i++){
    fSelfTriggerMask[i]->Disconnect("Toggled(Bool_t)");
    fCheckButtonSave[i]->Disconnect("Toggled(Bool_t)");
    }


 Cleanup(); //delete all widgets, frames, subframes and layouts
}

//==================================================================

void CAEN5740Dlg::ButtonClicked(){

 TGCheckButton *te = (TGCheckButton *) gTQSender;
 Int_t id = te->WidgetId();

 switch(id){
  case eGPIO:
 	cout << "klikam na button" << endl;
	aDevice->mFPIOtype = (aDevice->mFPIOtype) ? 0 : 1 ; 
	(aDevice->mFPIOtype) ? fFPIOtype->SetText("   TTL   ") : fFPIOtype->SetText("   NIM   ");
  break;

  default:
  break;
 }

  aDevice->SetReconfigure(kTRUE);
  aDevice->Refresh();
}

//==================================================================

void CAEN5740Dlg::EnableGroup(Bool_t a){

 TGCheckButton *te = (TGCheckButton *) gTQSender;
 Int_t id = te->WidgetId();
 cout << "EnableGroup, WidgetID = "<< id << " value = " << a << endl;

    //fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
    //if(!aDevice->IsEnabled() || !fGroupEnableMask[i/8]->GetState() ){
    //    fCheckButtonSave[i]->SetEnabled(kFALSE);
                

 switch (id){
   case eGROUP + 0:
	fDCoffset[0]->SetState(a);
        for(Int_t i = 0; i < 8; i++){
            fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
            if(!aDevice->IsEnabled() || !fGroupEnableMask[0]->GetState() )
               fCheckButtonSave[i]->SetEnabled(kFALSE);
           }
   break;

   case eGROUP + 1:
	fDCoffset[1]->SetState(a);
        for(Int_t i = 8; i < 16; i++){
            fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
            if(!aDevice->IsEnabled() || !fGroupEnableMask[1]->GetState() )
               fCheckButtonSave[i]->SetEnabled(kFALSE);
           }
   break;

   case eGROUP + 2:
	fDCoffset[2]->SetState(a);
        for(Int_t i = 16; i < 24; i++){
            fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
            if(!aDevice->IsEnabled() || !fGroupEnableMask[2]->GetState() )
               fCheckButtonSave[i]->SetEnabled(kFALSE);
           }
   break;

   case eGROUP + 3:
	fDCoffset[3]->SetState(a);
        for(Int_t i = 24; i < 32; i++){
            fCheckButtonSave[i]->SetState( EButtonState( Get_Bit( aDevice->mSaveChannel, i ) ) );
            if(!aDevice->IsEnabled() || !fGroupEnableMask[3]->GetState() )
               fCheckButtonSave[i]->SetEnabled(kFALSE);
           }
   break;

   default:
   break;
   }


}
//==================================================================

void CAEN5740Dlg::EnableTrigger(Int_t a){

 TGComboBox *te = (TGComboBox *) gTQSender;
 Int_t id = te->WidgetId();
 Bool_t aa = Bool_t(a);

  switch (id){
   case eCHNTRIGGERMODE + 0:
	fThreshold[0]->SetState(aa);
	fChannelTriggerEdge[0]->SetEnabled(aa);
        for(Int_t i = 0; i < 8; i++)
	   fSelfTriggerMask[i]->SetEnabled(aa);	
   break;

   case eCHNTRIGGERMODE + 1:
	fThreshold[1]->SetState(aa);
	fChannelTriggerEdge[1]->SetEnabled(aa);
        for(Int_t i = 8; i < 16; i++)
	   fSelfTriggerMask[i]->SetEnabled(aa);	
   break;

   case eCHNTRIGGERMODE + 2:
	fThreshold[2]->SetState(a);
	fChannelTriggerEdge[2]->SetEnabled(a);
        for(Int_t i = 16; i < 24; i++)
	   fSelfTriggerMask[i]->SetEnabled(a);	
   break;

   case eCHNTRIGGERMODE + 3:
	fThreshold[3]->SetState(a);
	fChannelTriggerEdge[3]->SetEnabled(a);
        for(Int_t i = 24; i < 32; i++)
	   fSelfTriggerMask[i]->SetEnabled(a);	
   break;

   default:
   break;
   }


}


//==================================================================

void CAEN5740Dlg::NumberEntrySet(Long_t a){
                 
 TGNumberEntry *te = (TGNumberEntry *) gTQSender;
 Int_t id = te->WidgetId();

 cout << "NumberEntry WidgetID = "<< id << " value = " << a << endl;
 switch (id) {
   case eTRACKLENGTH:
	aDevice->mRecordLength = fRecordLength->GetNumber();
   break;

   case ePRETRIGGER:
	aDevice->mPostTrigger = fPostTrigger->GetNumber();
   break;

   case eTRIGGER_THRESHOLD + 0:
        cout << "@@@@@@ Threshold = " << fThreshold[0]->GetNumber() << endl;
	aDevice->mThreshold_mV[0] =  fThreshold[0]->GetNumber();
        aDevice->mThreshold[0] =  aDevice->mV2ADC(fThreshold[0]->GetNumber());
   break;

   case eTRIGGER_THRESHOLD + 1:
	aDevice->mThreshold_mV[1] =  fThreshold[1]->GetNumber();
        aDevice->mThreshold[1] =  aDevice->mV2ADC(fThreshold[1]->GetNumber());
   break;

   case eTRIGGER_THRESHOLD + 2:
	aDevice->mThreshold_mV[2] =  fThreshold[2]->GetNumber();
        aDevice->mThreshold[2] =  aDevice->mV2ADC(fThreshold[2]->GetNumber());
   break;

   case eTRIGGER_THRESHOLD + 3:
	aDevice->mThreshold_mV[3] =  fThreshold[3]->GetNumber();
        aDevice->mThreshold[3] =  aDevice->mV2ADC(fThreshold[3]->GetNumber());
   break;

   case eOFFSET + 0:
  	aDevice->mDCoffset_mV[0]  =  fDCoffset[0]->GetNumber();
        aDevice->mDCoffset[0]  =  aDevice->mV2DAC(fDCoffset[0]->GetNumber());
   break;

   case eOFFSET + 1:
  	aDevice->mDCoffset_mV[1]  =  fDCoffset[1]->GetNumber();
        aDevice->mDCoffset[1]  =  aDevice->mV2DAC(fDCoffset[1]->GetNumber());
   break;

   case eOFFSET + 2:
  	aDevice->mDCoffset_mV[2]  =  fDCoffset[2]->GetNumber();
        aDevice->mDCoffset[2]  =  aDevice->mV2DAC(fDCoffset[2]->GetNumber());
   break;

   case eOFFSET + 3:
  	aDevice->mDCoffset_mV[3]  =  fDCoffset[3]->GetNumber();
        aDevice->mDCoffset[3]  =  aDevice->mV2DAC(fDCoffset[3]->GetNumber());
   break;

   case eTRIGGEROUT_DELAY:
        aDevice->mDelay  =  fDelay->GetNumber();
   break;
   default:
   break;
   }

  aDevice->SetReconfigure(kTRUE);
  aDevice->Refresh();
}

//==================================================================
void CAEN5740Dlg::ComboBoxSet(Int_t ){

 TGComboBox *te = (TGComboBox *) gTQSender;
 Int_t id = te->WidgetId();
 //cout << "ComboBoxSet WidgetID = "<< id << " value = " << a << endl;

 switch (id){
   case eEXTTRIGGERMODE: 
	aDevice->mExtTriggerMode = te->GetSelected();
   break;

   case eCHNTRIGGERMODE + 0:
	aDevice->mChannelTriggerMode[0] = te->GetSelected();
   break;

   case eCHNTRIGGERMODE + 1:
	aDevice->mChannelTriggerMode[1] = te->GetSelected();
   break;
 
   case eCHNTRIGGERMODE + 2:
	aDevice->mChannelTriggerMode[2] = te->GetSelected();
   break;

   case eCHNTRIGGERMODE + 3:
	aDevice->mChannelTriggerMode[3] = te->GetSelected();
   break;

   case eTRIGGER_EDGE + 0:
	aDevice->mTriggerEdge[0] = te->GetSelected();
   break;

   case eTRIGGER_EDGE + 1:
	aDevice->mTriggerEdge[1] = te->GetSelected();
   break;

   case eTRIGGER_EDGE + 2:
	aDevice->mTriggerEdge[2] = te->GetSelected();
   break;

   case eTRIGGER_EDGE + 3:
	aDevice->mTriggerEdge[3] = te->GetSelected();
   break;

   case eACQ_MODE:
	aDevice->mAcqMode = te->GetSelected();
   break;

   default:
   break;
   }


  aDevice->SetReconfigure(kTRUE);
  aDevice->Refresh();
}

//========================================================================================

void CAEN5740Dlg::CheckButtonSet(Bool_t a){
 TGCheckButton *te = (TGCheckButton *) gTQSender;
 Int_t id = te->WidgetId();
 //cout << "CheckButtonSet WidgetID = "<< id << " value = " << a << endl;

 switch (id){
   case eGROUP + 0:
	aDevice->mGroupEnableMask[0] = a;
        a ? aDevice->mSaveChannel |= 0x000000FF : aDevice->mSaveChannel &= 0xFFFFFF00;
   break;

   case eGROUP + 1:
	aDevice->mGroupEnableMask[1] = a;
        a ? aDevice->mSaveChannel |= 0x0000FF00 : aDevice->mSaveChannel &= 0xFFFF00FF;
   break;

   case eGROUP + 2:
	aDevice->mGroupEnableMask[2] = a;
        a ? aDevice->mSaveChannel |= 0x00FF0000 : aDevice->mSaveChannel &= 0xFF00FFFF;
   break;

   case eGROUP + 3:
	aDevice->mGroupEnableMask[3] = a;
        a ? aDevice->mSaveChannel |= 0xFF000000 : aDevice->mSaveChannel &= 0x00FFFFFF;
   break;

   case  eCHBTSTART:
       aDevice->mAcqMode = !a;
       aDevice->mDelay = 0;
       fDelay->SetState(a);
       fAcqMode->SetEnabled(a);
       fDelay->SetNumber(aDevice->mDelay); 
       fAcqMode->Select(0);
   break;

   default:

    for(Int_t i = eCHANNEL_SAVE; i < eCHANNEL_SAVE+32; i++){
       if( i == id ){
         a ? Set_Bit(aDevice->mSaveChannel, id - eCHANNEL_SAVE) : Reset_Bit(aDevice->mSaveChannel, id - eCHANNEL_SAVE) ; 	
         }
       }

//    if (id >= eCHANNEL_SAVE && id < eCHANNEL_SAVE+32 ){
//          Flip_Bit(aDevice->mSaveChannel, id - eCHANNEL_SAVE);
//         }

    for(Int_t i = eTRIGGER_SELFMASK; i < eTRIGGER_SELFMASK + 32; i++) {
	if( i == id ){
	  a ? aDevice->mSelfTriggerMask[i-eTRIGGER_SELFMASK] = 1 : aDevice->mSelfTriggerMask[i-eTRIGGER_SELFMASK] = 0;
          //Flip_Bit(aDevice->mSelfTriggerMask[i-eTRIGGER_SELFMASK], id - eTRIGGER_SELFMASK);
	  }
       }	
/*
    else  if (id >= eTRIGGER_SELFMASK + 0  && id < eTRIGGER_SELFMASK + 8){
          Flip_Bit(aDevice->mSelfTriggerMask[0], id - eTRIGGER_SELFMASK);
         }
    else  if (id >= eTRIGGER_SELFMASK + 8  && id < eTRIGGER_SELFMASK + 16){
          Flip_Bit(aDevice->mSelfTriggerMask[1], id - eTRIGGER_SELFMASK - 8);
         }
    else  if (id >= eTRIGGER_SELFMASK + 16 && id < eTRIGGER_SELFMASK + 24){
          Flip_Bit(aDevice->mSelfTriggerMask[2], id - eTRIGGER_SELFMASK - 16);
         }
    else  if (id >= eTRIGGER_SELFMASK + 24 && id < eTRIGGER_SELFMASK + 32){
          Flip_Bit(aDevice->mSelfTriggerMask[3], id - eTRIGGER_SELFMASK -24);
         }
*/
   break;
   }

  aDevice->SetReconfigure(kTRUE);
  aDevice->Refresh();
}

//==================================================================

void CAEN5740Dlg::Refresh() {
  #ifdef DEBUG
  cout << "DEBUG [CAEN5470Dlg::Refresh] " << endl;
  #endif

return;
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
Bool_t CAEN5740Dlg::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

//Long_t msg1 = msg;
// if(msg > 1001){
//   cout << "----------------- message: " << msg1 << "\tGET_MSG: " << GET_MSG(msg1) << "\tGET_SUBMSG: " << GET_SUBMSG(msg1) << endl;
//   return kTRUE; // to avoid messages at the beginning.... there are 6 unknown  
//  }

//cout << "DEBUG [CAEN5740Dlg::ProcessMessage] MSG: " << msg1 << "\tGET_MSG: " << GET_MSG(msg1) << "\tGET_SUBMSG: " << GET_SUBMSG(msg1) << endl;
//cout << "DEBUG [CAEN5740Dlg::ProcessMessage] MSG: " << msg << "\tparm1: " << parm1 << "\tparm2: " << parm2 << endl;


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
                 aDevice->mDCoffset[0]  =  aDevice->mV2DAC(-fDCoffset[0]->GetNumber());
                 }
         if (parm1 == eOFFSET + 1)  		{
                 aDevice->mDCoffset_mV[1]  =  fDCoffset[1]->GetNumber();
                 aDevice->mDCoffset[1]  =  aDevice->mV2DAC(fDCoffset[1]->GetNumber());
                 }
         if (parm1 == eOFFSET + 2)  		{
                 aDevice->mDCoffset_mV[2]  =  fDCoffset[2]->GetNumber();
                 aDevice->mDCoffset[2]  =  aDevice->mV2DAC(fDCoffset[2]->GetNumber());
                 }
         if (parm1 == eOFFSET + 3)  		{
                 aDevice->mDCoffset_mV[3]  =  fDCoffset[3]->GetNumber();
                 aDevice->mDCoffset[3]  =  aDevice->mV2DAC(fDCoffset[3]->GetNumber());
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 0)	{
                 aDevice->mThreshold_mV[0] =  fThreshold[0]->GetNumber();
                 aDevice->mThreshold[0] =  aDevice->mV2ADC(fThreshold[0]->GetNumber());
                 cout << "Threshold[0] = " << aDevice->mThreshold[0] << " " << aDevice->mThreshold_mV[0] <<" offset = " << aDevice->mDCoffset[0] << endl;
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 1)	{
                 aDevice->mThreshold_mV[1] =  fThreshold[1]->GetNumber();
                 aDevice->mThreshold[1] =  aDevice->mV2ADC(fThreshold[1]->GetNumber());
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 2)	{
                 aDevice->mThreshold_mV[2] =  fThreshold[2]->GetNumber();
                 aDevice->mThreshold[2] =  aDevice->mV2ADC(fThreshold[2]->GetNumber());
                 }
         if (parm1 == eTRIGGER_THRESHOLD + 3)	{
                 aDevice->mThreshold_mV[3] =  fThreshold[3]->GetNumber();
                 aDevice->mThreshold[3] =  aDevice->mV2ADC(fThreshold[3]->GetNumber());
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


