
#include "TGNumberEntry.h"
#include "DGV1718.h"
#include "DV1718.h"


  ClassImp(DGV1718)

//-----------------------------------------------------------------------------
 DGV1718::DGV1718(DModule *module) : DGModule((DModule *) module) {

  std::cout << "constructing DGV1718" << std::endl;

  DV1718 *dV1718 = (DV1718 *) fModule;

                                                      // left - right - top - bottom
  fL0   = new TGLayoutHints(kLHintsTop  | kLHintsExpandX, 0,0,5,5);
  fL1   = new TGLayoutHints(kLHintsTop  | kLHintsLeft, 3,10,1,1);
  fL2   = new TGLayoutHints(kLHintsTop  | kLHintsLeft, 3,0,2,2);

  fMDat->ChangeOptions(kHorizontalFrame);
  fFrame1 = new TGCompositeFrame(fMDat, 0, 0);
  fFrame2 = new TGCompositeFrame(fMDat, 0, 0);

  fMDat->AddFrame(fFrame1, fL0);
  fMDat->AddFrame(fFrame2, fL0);

  fLabel1 = new TGLabel(fFrame1, new TGString("Input Hit") );
  fLabel2 = new TGLabel(fFrame1, new TGString("End Counts") );
  fLabel3 = new TGLabel(fFrame1, new TGString("Value") );
  fLabel4 = new TGLabel(fFrame1, new TGString("End counts on out4") );

  fFrame1->AddFrame(fLabel1, fL2);
  fFrame1->AddFrame(fLabel2, fL2);
  fFrame1->AddFrame(fLabel3, fL2);
  fFrame1->AddFrame(fLabel4, fL2);

  //-----------------------------------------------
  fInput = new TGComboBox(fFrame2, eInput);
  fInput->AddEntry("Input 1", 0);                   
  fInput->AddEntry("Input 2", 1);                   
  fInput->Select(dV1718->m_Input, kFALSE);               
  fInput->Resize(80, 20 );
  fInput->Associate(this);

  //-----------------------------------------------
  fEndCounts = new TGNumberEntry(fFrame2, dV1718->m_EndCounts, 5, eEndCounts, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, 
                                       TGNumberFormat::kNELLimitMinMax, 0, 1023 );
  fEndCounts->Associate(this);
  fEndCounts->Resize(80, 20 );

  //----------------------------------------------
  fValue = new TGNumberEntry(fFrame2, dV1718->m_Value, 5, eValue, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative);
  fValue->Resize(80, 20 );
  fValue->SetState(0);
               
  //-----------------------------------------------
  fCountOutput     = new TGCheckButton(fFrame2, "", eCountOutput);
  fCountOutput->Resize(80, 22 );
  fCountOutput->Associate(this);
  //-----------------------------------------------

  fResetCounter  = new TGTextButton(fFrame2, "Reset counter",    eResetCounter);
  fResetCounter->Resize(80, 20 );
  fResetCounter->Associate(this);
  //-----------------------------------------------

  fFrame2->AddFrame(fInput, fL1);
  fFrame2->AddFrame(fEndCounts, fL1);
  fFrame2->AddFrame(fValue, fL1);
  fFrame2->AddFrame(fCountOutput, fL0);
  fFrame2->AddFrame(fResetCounter, fL0);

 MapSubwindows();
 this->Resize(this->GetDefaultSize());
 MapWindow();
 Refresh();
 std::cout << "constructing DGV1718 done!" << std::endl;
 dV1718->ShowSettings(); 

}

//-----------------------------------------------------------------------------
 DGV1718::~DGV1718() {
  
  //DMadc32 *dMadc32 = (DMadc32 *) fModule;
  //dMadc32->dGMadc32 = 0;
  std::cout <<"destroying DGV1718" << std::endl;
  delete fL0;

}
//-----------------------------------------------------------------------------
 void DGV1718::Refresh() {
  // Update display so it represents the actual contents of fModule.
  DV1718 *dV1718 = (DV1718 *) fModule;
/*
 std::cout << " DGV1718 refresh button status " << std::endl;
  for (Int_t i = 0; i < 32; i++) {
    if(dMadc32->m_IgnoreThreshold == 0 && dMadc32->m_ThresholdOn[i] == 1){
      //std::cout << " dMadc32->m_IgnoreThreshold:" << dMadc32->m_IgnoreThreshold << " dMadc32->m_ThresholdOn["<<i<<"]: " << dMadc32->m_ThresholdOn[i] << " state(1) "<< std::endl;
      fThresholdValue[i]->SetState(EButtonState(1));
      fThresholdOn[i]->SetState(EButtonState(1));
      }
    else{
      //std::cout << " dMadc32->m_IgnoreThreshold:" << dMadc32->m_IgnoreThreshold << " dMadc32->m_ThresholdOn["<<i<<"]: " << dMadc32->m_ThresholdOn[i] << " state(0) "<< std::endl;
      fThresholdValue[i]->SetState(EButtonState(0));
      fThresholdOn[i]->SetState(EButtonState(0));
      }
  }


  DoRedraw();
*/
}
//-----------------------------------------------------------------------------
 Bool_t DGV1718::HandleTimer(TTimer *tim) {
  return kTRUE;
}
//-----------------------------------------------------------------------------
 Bool_t DGV1718::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2){

 std::cout << "msg " << msg << ", parm1 "<< parm1 << ", parm2 "<< parm2 << std::endl;
 std::cout << "GET_MSG: " << GET_MSG(msg) << " GET_SUBMSG(msg): " << GET_SUBMSG(msg) << std::endl; 
   
 DV1718 *dV1718 = (DV1718 *) fModule;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
        if(parm1 == eResetCounter){
          dV1718->ResetModule();
          }
    case kCM_COMBOBOX:
        switch (parm1){
             case eInput: 
                  dV1718->m_Input = parm2;
             break;
             default:
             break; 
        }
    break;
    case kCM_CHECKBUTTON:
      switch (parm1) {
      case eCountOutput:
        dV1718->m_CountOutput = (Int_t)fCountOutput->GetState(); 
      break;
      default:
      break;
      }
   
    default:
      break;
    }
    break;
  case kC_TEXTENTRY:
     switch(GET_SUBMSG(msg)) {
       case kTE_TEXTCHANGED:
         if(eEndCounts){
           dV1718->m_EndCounts = fEndCounts->GetNumber();  
           }
         else
            std::cout << "ERROR [DGV1718::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
       break;
       case kTE_ENTER:
         if(eEndCounts){
           dV1718->m_EndCounts = fEndCounts->GetNumber();  
           }
         else
            std::cout << "ERROR [DGV1718::ProcessMessage] in text enter parm1: " << parm1 << std::endl;
       break;

       default:
       break;
     }
  break;
  default:
    break;
  }
 Refresh();
 dV1718->ShowSettings(); 

  return kTRUE;
}
//-----------------------------------------------------------------------------

