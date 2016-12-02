
#include "DGMadc32.h"
#include "DMadc32.h"

  ClassImp(DGMadc32)

//-----------------------------------------------------------------------------
 DGMadc32::DGMadc32(DModule *module) : DGModule((DModule *) module) {

  std::cout << "constructing DGMadc32" << std::endl;

  DMadc32 *dMadc32 = (DMadc32 *) fModule;

                                                      // left - right - top - bottom
  fL0   = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 0,0,5,5);
  fL1   = new TGLayoutHints(kLHintsTop  | kLHintsLeft, 3,10,1,1);
  fL2   = new TGLayoutHints(kLHintsTop  | kLHintsLeft, 3,0,2,2);

  fW1Frame = new TGCompositeFrame(fMDat, 0, 0);

  fW2Frame = new TGCompositeFrame(fMDat, 0, 0, kHorizontalFrame);
   fW2_1Frame = new TGCompositeFrame(fW2Frame, 0, 0);
   fW2_2Frame = new TGCompositeFrame(fW2Frame, 0, 0);
   fW2_3Frame = new TGCompositeFrame(fW2Frame, 0, 0);
   fW2_4Frame = new TGCompositeFrame(fW2Frame, 0, 0);
   fW2Frame->AddFrame(fW2_1Frame, fL0);
   fW2Frame->AddFrame(fW2_2Frame, fL0);
   fW2Frame->AddFrame(fW2_3Frame, fL0);
   fW2Frame->AddFrame(fW2_4Frame, fL0);

  fW3Frame = new TGCompositeFrame(fMDat, 0, 0, kHorizontalFrame);
   fW3_1Frame = new TGCompositeFrame(fW3Frame, 0, 0);
   fW3_2Frame = new TGCompositeFrame(fW3Frame, 0, 0);
   fW3Frame->AddFrame(fW3_1Frame, fL0); 
   fW3Frame->AddFrame(fW3_2Frame, fL0); 

  fW4Frame = new TGCompositeFrame(fMDat, 0, 0);
 
  fMDat->AddFrame(fW1Frame, fL0);
  fMDat->AddFrame(fW2Frame, fL0);
  fMDat->AddFrame(fW3Frame, fL0);
  fMDat->AddFrame(fW4Frame, fL0);


  fIgnoreThreshold = new TGCheckButton(fW1Frame, "Ignore all Threshold", eIgnoreThreshold);
  fIgnoreThreshold->Associate(this);
 
  for(Int_t i = 0; i < 16; i++){
      char name[10];
      sprintf(name, "Ch%2d", i);  

      fThresholdOn[i]     = new TGCheckButton(fW2_1Frame, name, eThresholdOn + i);
      fThresholdOn[i]->Resize(80, fThresholdOn[i]->GetDefaultHeight() );
      fThresholdOn[i]->Associate(this);

      fThresholdValue[i]  = new TGNumberEntry(fW2_2Frame, dMadc32->m_ThresholdValue[i], 5, eThresholdValue + i, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 8191 );
      fThresholdValue[i]->Resize(60, 20);
      fThresholdValue[i]->Associate(this);

      sprintf(name, "Ch%2d", i+16);  
      fThresholdOn[i+16]     = new TGCheckButton(fW2_3Frame, name, eThresholdOn + i + 16);
      fThresholdOn[i+16]->Resize(80, fThresholdOn[i+16]->GetDefaultHeight() );
      fThresholdOn[i+16]->Associate(this);
      fThresholdValue[i+16]  = new TGNumberEntry(fW2_4Frame, dMadc32->m_ThresholdValue[i+16], 5, eThresholdValue + i + 16, TGNumberFormat::kNESInteger, 
                                                 TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 8191 );
      fThresholdValue[i+16]->Resize(60, 20 );
      fThresholdValue[i+16]->Associate(this);
      }

  fGateGenerator   = new TGCheckButton(fW3_1Frame, "Gate Generator", eGateGenerator);
  fGateGenerator->Associate(this);
  fLabel1	   = new TGLabel(fW3_1Frame, new TGString("Gate Delay [ns]??") );
  fLabel2	   = new TGLabel(fW3_1Frame, new TGString("Gate Width [ns]??") );
  fGateOutput      = new TGCheckButton(fW3_2Frame, "Gate Output", eGateOutput);
  fGateOutput->Associate(this);
  fGateDelay       = new TGNumberEntry(fW3_2Frame, dMadc32->m_GateDelay, 5, eGateDelay, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 255 );
  fGateWidth       = new TGNumberEntry(fW3_2Frame, dMadc32->m_GateWidth, 5, eGateWidth, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 255 );
  fGateWidth->Associate(this);
  fGateDelay->Associate(this);
  
  fADCResolution   = new TGComboBox(fW4Frame, eADCResolution);
  fADCResolution->AddEntry("2k       (800ns)", 0);                   
  fADCResolution->AddEntry("4k       (1.6us)", 1);                   
  fADCResolution->AddEntry("4k hires (3.2us)", 2);                   
  fADCResolution->AddEntry("8k       (6.4us)", 3);                   
  fADCResolution->AddEntry("8k hires (12.5us)",4);                   
  fADCResolution->Select(dMadc32->m_ADCResolution, kFALSE);               
  fADCResolution->Resize(120,22);
  fADCResolution->Associate(this);
  fLabel4	   = new TGLabel(fW4Frame, new TGString("ADC resolution") );

 
  fW1Frame->AddFrame(fIgnoreThreshold, fL1);
  for(Int_t i = 0; i < 16; i++){
      fW2_1Frame->AddFrame(fThresholdOn[i],       fL2);
      fW2_2Frame->AddFrame(fThresholdValue[i],    fL1);
      fW2_3Frame->AddFrame(fThresholdOn[i+16],    fL2);
      fW2_4Frame->AddFrame(fThresholdValue[i+16], fL1);
      }

  fW3_1Frame->AddFrame(fGateGenerator, fL0);
  fW3_1Frame->AddFrame(fLabel1, fL0);
  fW3_1Frame->AddFrame(fLabel2, fL0);
  fW3_2Frame->AddFrame(fGateOutput, fL0);
  fW3_2Frame->AddFrame(fGateDelay, fL0);
  fW3_2Frame->AddFrame(fGateWidth, fL0);
  fW4Frame->AddFrame(fLabel4, fL1);
  fW4Frame->AddFrame(fADCResolution, fL1);
 
 MapSubwindows();
 this->Resize(this->GetDefaultSize());
 MapWindow();
 Refresh();
 std::cout << "constructing DGMadc32 done!" << std::endl;
 dMadc32->ShowSettings(); 

 LowerWindow();
}

//-----------------------------------------------------------------------------
 DGMadc32::~DGMadc32() {
  
  //DMadc32 *dMadc32 = (DMadc32 *) fModule;
  //dMadc32->dGMadc32 = 0;
  std::cout <<"destroying DGMhexadc32" << std::endl;
  delete fL0;

}
//-----------------------------------------------------------------------------
 void DGMadc32::Refresh() {
  // Update display so it represents the actual contents of fModule.
  DMadc32 *dMadc32 = (DMadc32 *) fModule;

 std::cout << " DGMadc32 refresh button status " << std::endl;
  for (Int_t i = 0; i < 32; i++) {
    if(dMadc32-> m_ThresholdOn[i]){ 
       fThresholdValue[i]->SetState(EButtonState(1));
       fThresholdValue[i]->SetHexNumber(dMadc32->m_ThresholdValue[i]);
       }
    else{ 
       fThresholdValue[i]->SetState(EButtonState(0));
       fThresholdValue[i]->SetHexNumber(0x1FFF);
       }

   // if(dMadc32->m_IgnoreThreshold){
   //   fThresholdValue[i]->SetHexNumber(0L);
   //   }
   // else{
   //   fThresholdValue[i]->SetHexNumber(dMadc32->m_ThresholdValue[i]);
   //   }
  
}


  DoRedraw();

}
//-----------------------------------------------------------------------------
 Bool_t DGMadc32::HandleTimer(TTimer *tim) {
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGMadc32::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2){

 std::cout << "msg " << msg << ", parm1 "<< parm1 << ", parm2 "<< parm2 << std::endl;
 std::cout << "GET_MSG: " << GET_MSG(msg) << " GET_SUBMSG(msg): " << GET_SUBMSG(msg) << std::endl; 
   
 DMadc32 *dMadc32 = (DMadc32 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
    case kCM_COMBOBOX:
        switch (parm1){
             case eADCResolution: 
                  dMadc32->m_ADCResolution = parm2;
             break;
             default:
             break; 
        }
    break;
    case kCM_CHECKBUTTON:
      switch (parm1) {
      case eIgnoreThreshold:
        dMadc32->m_IgnoreThreshold = (Int_t)fIgnoreThreshold->GetState(); 
         
        std::cout << "m_IgnoreThreshold; " << dMadc32->m_IgnoreThreshold << std::endl;   
        if(dMadc32->m_IgnoreThreshold){ 
          for(Int_t i = 0; i < 32; i++){
             if(dMadc32->m_ThresholdOn[i]){
                dMadc32->m_ThresholdCache[i] = dMadc32->m_ThresholdValue[i]; 
                dMadc32->m_ThresholdValue[i] = 0;
                } 
              }
          }
        else{
          for(Int_t i = 0; i < 32; i++){
             if(dMadc32->m_ThresholdOn[i]){
                dMadc32->m_ThresholdValue[i] = dMadc32->m_ThresholdCache[i];
                }  
             //dMadc32->m_ThresholdOn[i] = 1;
           }   
         }
      break;
      case eGateGenerator:
        dMadc32->m_GateGenerator = (Int_t)fGateGenerator->GetState(); 
      break;

      case eGateOutput:
        dMadc32->m_GateOutput = (Int_t)fGateOutput->GetState(); 
      break;

      default:
        if(parm1 >= 100 && parm1 < 132){
           //std::cout << " (Int_t)fThresholdOn[i]->GetState() " << (Int_t)fThresholdOn[parm1 - 100]->GetState()  << std::endl;   
           dMadc32-> m_ThresholdOn[parm1 - 100] = (Int_t)fThresholdOn[parm1-100]->GetState();
           if(dMadc32-> m_ThresholdOn[parm1 - 100]){ 
              dMadc32-> m_ThresholdValue[parm1 - 100] = dMadc32-> m_ThresholdCache[parm1 - 100];
              std::cout <<"z cache" << std::endl;
              }
           else{ 
              dMadc32-> m_ThresholdValue[parm1 - 100] =  0x1FFF;
              std::cout << "0x1FFF" << std::endl;
              }    
           }

        //else if (parm1 == eGateDelay )
        //    dMadc32->m_GateDelay = fGateDelay->GetNumber();                               
        //else if(parm1 == eGateWidth )
        //    dMadc32->m_GateWidth = fGateWidth->GetNumber();                     
        //else
            std::cout << "ERROR [DGMadc32::ProcessMessage] in text enter parm1: " << parm1 << std::endl;   

	break;
      }
   
    default:
      break;
    }
    break;
  case kC_TEXTENTRY:
     switch(GET_SUBMSG(msg)) {
       case kTE_TEXTCHANGED:
         //std::cout << "text changed parm1: " << parm1 << " value: " << fThresholdValue[parm1-200]->GetNumber() << std::endl;
         if(parm1 >= eThresholdValue && parm1 < eThresholdValue + 32)
            dMadc32->m_ThresholdValue[parm1-200] = fThresholdValue[parm1-200]->GetNumber(); 
         else if (parm1 == eGateDelay)
            dMadc32->m_GateDelay = fGateDelay->GetNumber(); 
         else if ( parm1 == eGateWidth )
            dMadc32->m_GateWidth = fGateWidth->GetNumber(); 
         else
            std::cout << "ERROR [DGMadc32::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
       break;
       case kTE_ENTER:
         if(parm1 >= eThresholdValue && parm1 < eThresholdValue + 32)
            dMadc32->m_ThresholdValue[parm1 - eThresholdValue] = fThresholdValue[parm1 - eThresholdValue]->GetNumber(); 
         else if(parm1 == eGateDelay )
            dMadc32->m_GateDelay = fGateDelay->GetNumber(); 
         else if(parm1 == eGateWidth )
            dMadc32->m_GateWidth = fGateWidth->GetNumber(); 
         else
            std::cout << "ERROR [DGMadc32::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
       break;

       default:
       break;
     }
  break;
  default:
    break;
  }
 Refresh();
 dMadc32->InitModule();
 dMadc32->ShowSettings(); 
  return kTRUE;
}
//-----------------------------------------------------------------------------

