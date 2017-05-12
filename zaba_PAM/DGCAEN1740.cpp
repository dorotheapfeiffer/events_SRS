#include <bitset>
#include "DGCAEN1740.h"
#include "DCAEN1740.h"

//===========================================================
static void Flip_Bit(ULong_t &val, int bit){
	  val ^= (1LL << bit);
}
//===========================================================
static Int_t Get_Bit(ULong_t val, Int_t bit){
   if( (1LL << bit) & val ) return 1;
   else return 0;
}
//===========================================================


 ClassImp(DGCAEN1740)

//-----------------------------------------------------------------------------
 DGCAEN1740::DGCAEN1740(DModule *module) : DGModule((DModule *) module) {

  std::cout << "constructing DGCAEN1740" << std::endl;

  DCAEN1740 *dCAEN1740 = (DCAEN1740 *) fModule;

                                                      // left - right - top - bottom
  fL00  = new TGLayoutHints( kLHintsTop     | kLHintsLeft | kLHintsExpandY,    0,0,10,10);
  fL0   = new TGLayoutHints( kLHintsTop     | kLHintsLeft,                     2,2,2,2);
  fL1   = new TGLayoutHints( kLHintsTop     | kLHintsLeft | kLHintsExpandY,    2,2,2,2);
  fL2   = new TGLayoutHints( kLHintsTop     | kLHintsExpandX,                  2,2,2,2);
  fL3   = new TGLayoutHints( kLHintsTop     | kLHintsExpandY | kLHintsExpandX, 2,2,2,2);
  fL4   = new TGLayoutHints( kLHintsCenterX | kLHintsExpandX,                  2,2,2,2);
  fL5   = new TGLayoutHints( kLHintsCenterX | kLHintsExpandY,                  2,2,2,2);
  fL6   = new TGLayoutHints( kLHintsCenterX | kLHintsExpandX | kLHintsExpandY, 2,2,2,2);


  fTab = new TGTab(fMDat, 30, 30);
  fMDat->AddFrame(fTab, fL6);
  fTab->SetTab(0);

  fNrTab[0] = fTab->AddTab("channels");
  fNrTab[0]->SetLayoutManager(new TGHorizontalLayout(fNrTab[0]));

  fNrTab[1] = fTab->AddTab("triggers");
  fNrTab[1]->SetLayoutManager(new TGHorizontalLayout(fNrTab[1]));

  fNrTab[2] = fTab->AddTab("options");
  fNrTab[2]->SetLayoutManager(new TGHorizontalLayout(fNrTab[2]));

  fNrTab[3] = fTab->AddTab("display");
  fNrTab[3]->SetLayoutManager(new TGHorizontalLayout(fNrTab[3]));

  //------------------------------------------------------------------
  fGF0_0 = new TGGroupFrame(fNrTab[0], "Samples", kVerticalFrame);
  fNrTab[0]->AddFrame(fGF0_0, fL1);

  fGF0_1 = new TGGroupFrame(fNrTab[0], "Groups" , kVerticalFrame);
  fNrTab[0]->AddFrame(fGF0_1, fL1);

  fGF0_2 = new TGGroupFrame(fNrTab[0], "Offset" , kVerticalFrame);
  fNrTab[0]->AddFrame(fGF0_2, fL1);

  fGF0_4 = new TGGroupFrame(fNrTab[0], "Polarity" , kVerticalFrame);
  fNrTab[0]->AddFrame(fGF0_4, fL1);

  fGF0_3 = new TGGroupFrame(fNrTab[0], "Save");
  fNrTab[0]->AddFrame(fGF0_3, fL6);
  //------------------------------------------------------------------
  fGF1_0 = new TGGroupFrame(fNrTab[1], "Trigger mode", kHorizontalFrame);
  fNrTab[1]->AddFrame(fGF1_0, fL1);

  fGF1_01 = new TGCompositeFrame(fGF1_0, 0, 0, kVerticalFrame);
  fGF1_02 = new TGCompositeFrame(fGF1_0, 0, 0, kVerticalFrame);
  fGF1_0->AddFrame(fGF1_01, fL00);
  fGF1_0->AddFrame(fGF1_02, fL00);
  fGF1_01->Resize();
  fGF1_02->Resize();

  fGF1_1 = new TGGroupFrame(fNrTab[1], "Threshold" , kVerticalFrame);
  fNrTab[1]->AddFrame(fGF1_1, fL1);

  //fGF1_2 = new TGGroupFrame(fNrTab[1], "Mode" , kVerticalFrame);
  //fNrTab[1]->AddFrame(fGF1_2, fL1);

  fGF1_4 = new TGGroupFrame(fNrTab[1], "Edge" , kVerticalFrame);
  fNrTab[1]->AddFrame(fGF1_4, fL1);

  fGF1_3 = new TGGroupFrame(fNrTab[1], "Channel contribution to trigger");
  fNrTab[1]->AddFrame(fGF1_3, fL6);
   //------------------------------------------------------------------
  fGF2_0 = new TGGroupFrame(fNrTab[2], "Synchronization", kVerticalFrame);
  fNrTab[2]->AddFrame(fGF2_0, fL1);

  fGF2_1 = new TGGroupFrame(fNrTab[2], "Input/Output" , kVerticalFrame);
  fNrTab[2]->AddFrame(fGF2_1, fL1);
    //------------------------------------------------------------------
  fGF3_0 = new TGGroupFrame(fNrTab[3], "General", kVerticalFrame);
  fNrTab[3]->AddFrame(fGF3_0, fL1);

  fGF3_1 = new TGGroupFrame(fNrTab[3], "individual channel" , kVerticalFrame);
  fNrTab[3]->AddFrame(fGF3_1, fL1);
  

  //===================================================================================================================
  //first tab (channels) first group (samples), counting from 0
  
  fGF0_0->AddFrame(new TGLabel(fGF0_0, new TGString("Record Length [smp]") ), fL0);
  
  fRecordLength  = new TGNumberEntry(fGF0_0, dCAEN1740->m_RecordLength, 5, eRecordLength, TGNumberFormat::kNESInteger, 
		        TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 1, 8191 );
  fRecordLength->Resize(80, fRecordLength->GetDefaultHeight());
  fRecordLength->Associate(this);
  fGF0_0->AddFrame(fRecordLength, fL0);

  fGF0_0->AddFrame(new TGLabel(fGF0_0, new TGString("Post Trigger [smp]") ), fL0);
  fPostTrigger  = new TGNumberEntry(fGF0_0, dCAEN1740->m_PostTrigger, 5, ePostTrigger, TGNumberFormat::kNESInteger, 
		        TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 1, 8191 );
  fPostTrigger->Resize(80, fPostTrigger->GetDefaultHeight());
  fPostTrigger->Associate(this);
  fGF0_0->AddFrame(fPostTrigger, fL0);

  //first tab (channels) second group (Group), counting from 0
  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
     char name[256];	  
     sprintf(name, "Gr.%d", i);
     fGroupEnableMask[i] = new TGCheckButton(fGF0_1, name, eGroupEnableMask+i);
     fGF0_1->AddFrame(fGroupEnableMask[i], fL1);
     fGroupEnableMask[i]->SetState(static_cast<EButtonState>(dCAEN1740->m_GroupEnableMask[i]));
     fGroupEnableMask[i]->Resize();
     fGroupEnableMask[i]->Associate(this);
  }      

  //first tab (channels) third group (Offset), counting from 0
  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
      fDCoffset[i] = new TGNumberEntry(fGF0_2, dCAEN1740->m_DCoffset[i], 5, eDCoffset + i, TGNumberFormat::kNESInteger, 
		        TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, (dCAEN1740->m_DCoffset[i] * 2) - 1 );
      fGF0_2->AddFrame(fDCoffset[i], fL1);
      fDCoffset[i]->Resize();
      fDCoffset[i]->Associate(this);
  } 

  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
     fChannelPulsePolarity[i] = new TGComboBox(fGF0_4, eChannelPulsePolarity + i);
     fGF0_4->AddFrame(fChannelPulsePolarity[i], fL1);
     fChannelPulsePolarity[i]->AddEntry("Neg", 0); 
     fChannelPulsePolarity[i]->AddEntry("Pos", 1); 
     fChannelPulsePolarity[i]->Select(dCAEN1740->m_ChannelPulsPolarity[i], kFALSE); 
     fChannelPulsePolarity[i]->Resize(50,22);
     fChannelPulsePolarity[i]->Associate(this);
  }

  //first tab (channels) forth group (Save), counting from 0
  fGF0_3->SetLayoutManager(new TGMatrixLayout(fGF0_3, dCAEN1740->m_NrGroups, dCAEN1740->m_NrChannels/8, 2, 15 ));
  for(UInt_t i = 0; i < dCAEN1740->m_NrChannels; i++){
      char name[256];	  
      sprintf(name, "%d", i);
      fSaveChannel[i] = new TGCheckButton(fGF0_3, name, eSaveChannel+i);
      fGF0_3->AddFrame(fSaveChannel[i], fL2);
      fSaveChannel[i]->SetState(static_cast<EButtonState>((dCAEN1740->m_SaveChannel >> i) & 0x1));
      fSaveChannel[i]->Resize();
      fSaveChannel[i]->Associate(this);
  }      

  //===================================================================================================================
  //second tab (triggers) first group (Group), counting from 0
  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
     char name[256];	  
     sprintf(name, "TrGr.%d", i);
     fGF1_01->AddFrame(new TGLabel(fGF1_01, new TGString(name) ), fL1);
  }      
  fGF1_01->AddFrame(new TGLabel(fGF1_01, new TGString("TgExt") ), fL1);

  //second tab (triggers) first group (Threshold), counting from 0
  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
      fThreshold[i] = new TGNumberEntry(fGF1_1, dCAEN1740->m_Threshold[i], 5, eThreshold + i, TGNumberFormat::kNESInteger, 
		        TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, (dCAEN1740->m_Threshold[i] * 2) - 1 );
      fGF1_1->AddFrame(fThreshold[i], fL1);
      fThreshold[i]->Resize(50,22);
      fThreshold[i]->Associate(this);
  } 
  fTriggerLevel = new TGTextEntry(fGF1_1, new TGTextBuffer(80), eTriggerLevel);
  fGF1_1->AddFrame(fTriggerLevel, fL1);
  fTriggerLevel->Resize(50, 22);
  fTriggerLevel->SetEnabled(kFALSE);
  fTriggerLevel->SetAlignment(kTextCenterX);
  dCAEN1740->m_FPIOtype ? fTriggerLevel->SetText(" TTL ") : fTriggerLevel->SetText(" NIM "); 


  //second tab (triggers) third group (Mode), counting from 0
  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
      fChannelTriggerMode[i] = new TGComboBox(fGF1_02, eChannelTriggerMode + i);
      fGF1_02->AddFrame(fChannelTriggerMode[i], fL1);
      fChannelTriggerMode[i]->AddEntry("Disabled", 0);
      fChannelTriggerMode[i]->AddEntry("Acq Only", 1);
      fChannelTriggerMode[i]->AddEntry("Ext Only", 2);
      fChannelTriggerMode[i]->AddEntry("Ext & Acq",3);
      fChannelTriggerMode[i]->Select( dCAEN1740->m_ChannelTriggerMode[i], kFALSE );

      fChannelTriggerMode[i]->Resize(80, 22);
      fChannelTriggerMode[i]->Associate(this);
  } 
  fExtTriggerMode = new TGComboBox(fGF1_02, eExtTriggerMode);
  fGF1_02->AddFrame(fExtTriggerMode, fL1);
  fExtTriggerMode->AddEntry("Disabled", 0);
  fExtTriggerMode->AddEntry("Acq Only", 1);
  fExtTriggerMode->AddEntry("Ext Only", 2);
  fExtTriggerMode->AddEntry("Ext & Acq",3);
  fExtTriggerMode->Select( dCAEN1740->m_ExtTriggerMode, kFALSE );
  fExtTriggerMode->Resize(80, 22);
  fExtTriggerMode->Associate(this);

  for(UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++){
     fTriggerEdge[i] = new TGComboBox(fGF1_4, eTriggerEdge + i);
     fGF1_4->AddFrame(fTriggerEdge[i], fL1);
     fTriggerEdge[i]->AddEntry("rising", 0); 
     fTriggerEdge[i]->AddEntry("falling", 1); 
     fTriggerEdge[i]->Select(dCAEN1740->m_TriggerEdge[i], kFALSE); 
     fTriggerEdge[i]->Resize(70,20);
     fTriggerEdge[i]->Associate(this);
  }


  //second tab (trigger) forth group (Channel contribution to build self trigger), counting from 0
  fGF1_3->SetLayoutManager(new TGMatrixLayout(fGF1_3, dCAEN1740->m_NrGroups, dCAEN1740->m_NrChannels/8, 2, 2 ));
  for(UInt_t i = 0; i < dCAEN1740->m_NrChannels; i++){
      char name[256];	  
      sprintf(name, "%d", i);
      fSelfTriggerMask[i] = new TGCheckButton(fGF1_3, name, eSelfTriggerMask+i);
      fGF1_3->AddFrame(fSelfTriggerMask[i], fL2);
      fSelfTriggerMask[i]->SetState(static_cast<EButtonState>(dCAEN1740->m_SelfTriggerMask[i]));
      fSelfTriggerMask[i]->Resize();
      fSelfTriggerMask[i]->Associate(this);
  }      

  //third tab (option) first group (synchronization), counting from 0
  fGF2_0->AddFrame(new TGLabel(fGF2_0, new TGString("Synch mode") ), fL0);

  fRunSyncMode = new TGComboBox(fGF2_0, eRunSyncMode);
  fGF2_0->AddFrame(fRunSyncMode, fL0);
  fRunSyncMode->AddEntry("Disabled", 0);
  fRunSyncMode->AddEntry("TrgOutTriInDaisyChain", 1);
  fRunSyncMode->AddEntry("GpioDaisyChain", 2);
  fRunSyncMode->AddEntry("All trigger ", 3);
  fRunSyncMode->Select(0, kFALSE);
  fRunSyncMode->Resize(120, 22);
  fRunSyncMode->Associate(this);

  fGF2_0->AddFrame(new TGLabel(fGF2_0, new TGString("Clock source") ), fL0);
  
  fClock = new TGComboBox(fGF2_0, eClock);
  fClock->AddEntry("Internal",0);
  fClock->AddEntry("External",1);
  fClock->Select(0, kFALSE);
  fClock->Resize(120, 22);
  fClock->Associate(this);
  fGF2_0->AddFrame(fClock, fL0);
  
  fGF2_0->AddFrame(new TGLabel(fGF2_0, new TGString("Clock delay") ), fL0);
  
  fDelay = new TGNumberEntry(fGF2_0, dCAEN1740->m_Delay, 5, eDelay, TGNumberFormat::kNESInteger, 
		        TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 100 );
  fGF2_0->AddFrame(fDelay, fL0);
  
  //third tab (option) first group (input/output), counting from 0
  std::string gpio;
  if(dCAEN1740->m_FPIOtype) gpio = ":TTL:"; else gpio = ":NIM:";
  fFPIOButton = new TGTextButton(fGF2_1, gpio.c_str(),  eGPIO);
  fGF2_1->AddFrame(fFPIOButton);
  fClock->Resize(120, 22);
  fClock->Associate(this);
  

 MapSubwindows();
 this->Resize(this->GetDefaultSize());
 MapWindow();
 Refresh();
 std::cout << "constructing DGCAEN1740 done!" << std::endl;
 dCAEN1740->ShowSettings(); 

 RaiseWindow();
}

//-----------------------------------------------------------------------------
 DGCAEN1740::~DGCAEN1740() {
  
  //DGCAEN1740 *dCAEN1740 = (DGCAEN1740 *) fModule;
  //dCAEN1740->dGMadc32 = 0;
  std::cout <<"destroying DGCAEN1740D" << std::endl;
  //delete fL0;

}
//-----------------------------------------------------------------------------
 void DGCAEN1740::Refresh() {
  // Update GUI so it represents the actual contents of fModule.
  // 
  DCAEN1740 *dCAEN1740 = (DCAEN1740 *) fModule;

  std::cout << " DGCAEN1740 refresh button status " << std::endl;

  for (UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++) {
    if(dCAEN1740->m_GroupEnableMask[i] == 0){
//       for(Int_t j = 0; j < 8; j++){	    
// 	   fSaveChannel[i*8+j]->SetState( EButtonState( Get_Bit( dCAEN1740->m_SaveChannel, i*8+j ) ) );
//           if(!fGroupEnableMask[i*8+j]->GetState() )
//	     fSaveChannel[i*8+j]->SetEnabled(1);
//           }

       fDCoffset[i]->SetState( EButtonState(0) );
       fChannelPulsePolarity[i]->SetEnabled(0);
    }else{
//       for(UInt_t j = 0; j < 8; j++){	    
//          fSaveChannel[i*8+j]->SetState(EButtonState(Get_Bit( dCAEN1740->m_SaveChannel, i*8+j )));
//          if(!fGroupEnableMask[i*8+j]->GetState() )
//	     fSaveChannel[i*8+j]->SetEnabled(0);
//           }
       fDCoffset[i]->SetState( EButtonState(1) );
       fChannelPulsePolarity[i]->SetEnabled(1);
    }
  }

 //     dCAEN1740->m_GroupEnableMask[i] ? fSaveChannel[i]->SetState(EButtonState(1)) : fSaveChannel[i]->SetState(EButtonState(0));

  for (UInt_t i = 0; i < dCAEN1740->m_NrGroups; i++) {
      if(dCAEN1740->m_ChannelTriggerMode[i] == 0){
	fTriggerEdge[i]->SetEnabled(0);      
	fThreshold[i]->SetState(EButtonState(0));
//        for(UInt_t j = 0; j < 8; j++){	    
//           fSelfTriggerMask[i*8+j]->SetState(EButtonState(dCAEN1740->m_SelfTriggerMask[i*8+j]));
//           if(!fSelfTriggerMask[i*8+j]->GetState() )
//	     fSelfTriggerMask[i*8+j]->SetEnabled(0);
//        }
      }
      else{
	fTriggerEdge[i]->SetEnabled(1);      
	fThreshold[i]->SetState(EButtonState(1));
//        for(UInt_t j = 0; j < 8; j++){	    
//           fSelfTriggerMask[i*8+j]->SetState(EButtonState(dCAEN1740->m_SelfTriggerMask[i*8+j]));
//           if(!fSelfTriggerMask[i*8+j]->GetState() )
//	     fSelfTriggerMask[i*8+j]->SetEnabled(1);
//        }
      }
  }   

  DoRedraw();
}
//-----------------------------------------------------------------------------
 Bool_t DGCAEN1740::HandleTimer(TTimer *tim) {
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGCAEN1740::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2){

 std::cout << "msg " << msg << ", parm1 "<< parm1 << ", parm2 "<< parm2 << std::endl;
 std::cout << "GET_MSG: " << GET_MSG(msg) << " GET_SUBMSG(msg): " << GET_SUBMSG(msg) << std::endl; 
   
 DCAEN1740 *dCAEN1740 = (DCAEN1740 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
       case kCM_BUTTON:
	  if(parm1 == eGPIO){
             dCAEN1740->m_FPIOtype = (dCAEN1740->m_FPIOtype) ? 0 : 1 ;
	     (dCAEN1740->m_FPIOtype) ? fFPIOButton->SetText(":TTL:") : fFPIOButton->SetText(":NIM:");
	  }
       break;	       
       case kCM_COMBOBOX:
          if(parm1 == eRunSyncMode)
	     dCAEN1740->m_RunSyncMode = static_cast<CAEN_DGTZ_RunSyncMode_t>(parm2);
	  else if(parm1 == eClock)
	     dCAEN1740->m_Clock = parm2;
	  else if(parm1 >= eChannelPulsePolarity && parm1 < eChannelPulsePolarity + 8)
	     dCAEN1740->m_ChannelPulsPolarity[parm1 - eChannelPulsePolarity] = parm2;
          else if(parm1 >= eChannelTriggerMode && parm1 < eChannelTriggerMode + 8){
	     dCAEN1740->m_ChannelTriggerMode[parm1 - eChannelTriggerMode] = parm2;
	     dCAEN1740->m_SelfTriggerMask[parm1 - eChannelTriggerMode] = parm2;
	  }  
          else if(parm1 >= eTriggerEdge && parm1 < eTriggerEdge + 8)
	     dCAEN1740->m_TriggerEdge[parm1 - eTriggerEdge] = parm2;	  
	  else if(parm1 == eExtTriggerMode)
	     dCAEN1740->m_ExtTriggerMode = static_cast<UInt_t>(parm2);	  
          else
            std::cerr << "ERROR [DGCAEN1740::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
		  
       break;
       case kCM_CHECKBUTTON:
         if(parm1 >= eSaveChannel && parm1 < eSaveChannel + 64)
            Flip_Bit(dCAEN1740->m_SaveChannel, parm1 - eSaveChannel); 
	 else if(parm1 >= eGroupEnableMask && parm1 < eGroupEnableMask + 8)
            dCAEN1740->m_GroupEnableMask[parm1 - eGroupEnableMask] = fGroupEnableMask[parm1 - eGroupEnableMask]->GetState(); 
	 else if(parm1 >= eSelfTriggerMask && parm1 < eSelfTriggerMask + 64)
            dCAEN1740->m_SelfTriggerMask[parm1 - eSelfTriggerMask] = fSelfTriggerMask[parm1 - eSelfTriggerMask]->GetState(); 
	 else 
            std::cerr << "ERROR [DGCAEN1740::ProcessMessage] in CheckButton parm1: " << parm1 << std::endl;
       break;
       default:
       break;
      }
  break;
  case kC_TEXTENTRY:
      switch(GET_SUBMSG(msg)) {
          case kTE_TEXTCHANGED:
         //std::cout << "text changed parm1: " << parm1 << " value: " << fThresholdValue[parm1-200]->GetNumber() << std::endl;
         if(parm1 >= eDCoffset && parm1 < eDCoffset + 8)
            dCAEN1740->m_DCoffset[parm1 - eDCoffset] = fDCoffset[parm1 - eDCoffset]->GetNumber(); 
	 else if(parm1 >= eThreshold && parm1 < eThreshold + 8)
            dCAEN1740->m_Threshold[parm1 - eThreshold] = fThreshold[parm1 - eThreshold]->GetNumber(); 
         else if(parm1 == eRecordLength )
            dCAEN1740->m_RecordLength = fRecordLength->GetNumber(); 
         else if(parm1 == ePostTrigger )
            dCAEN1740->m_PostTrigger = fPostTrigger->GetNumber(); 
         else if(parm1 == eDelay )
            dCAEN1740->m_Delay = fDelay->GetNumber(); 
         else
            std::cerr << "ERROR [DGCAEN1740::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
       break;
       case kTE_ENTER:
         if(parm1 >= eDCoffset && parm1 < eDCoffset + 8)
            dCAEN1740->m_DCoffset[parm1 - eDCoffset] = fDCoffset[parm1 - eDCoffset]->GetNumber(); 
	 else if(parm1 >= eThreshold && parm1 < eThreshold + 8)
            dCAEN1740->m_Threshold[parm1 - eThreshold] = fThreshold[parm1 - eThreshold]->GetNumber(); 
         else if(parm1 == eRecordLength )
            dCAEN1740->m_RecordLength = fRecordLength->GetNumber(); 
         else if(parm1 == ePostTrigger )
            dCAEN1740->m_PostTrigger = fPostTrigger->GetNumber(); 
         else if(parm1 == eDelay )
            dCAEN1740->m_Delay = fDelay->GetNumber(); 
         else
            std::cerr << "ERROR [DGCAEN1740::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
       break;

       default:
       break;
     }
  break;
  default:
  break;
  }
 Refresh();
 //dCAEN1740->InitModule();
 dCAEN1740->ShowSettings(); 
 
  return kTRUE;
}
//-----------------------------------------------------------------------------

