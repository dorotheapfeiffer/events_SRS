#include "DV1718.h"
#include "DGDisplay.h"
#include <bitset>
#include <string>
#include <fstream>
#include "sys/time.h"
#include "TRandom.h"


 static Int_t VME_CRATE = 0;

ClassImp(DV1718)

//*****************************************************************************
 DV1718::DV1718(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"Pulse sensitive digitizer",(char*)"Mesytec Madc-32",mdesc,addr) {
	       std::cout<<"constructing DV1718\n";


  m_VMEBridge   = cvV1718;
  m_Value	= 0;
  m_Input	= 0;
  m_EndCounts	= 0;
  m_CountOutput = 0;

  if( VME_CRATE ){ // to test puropse only.... (test without crate)
      CAENVME_SystemReset(m_VMEBridge);

 } // end of VME_CRATE



}
//-----------------------------------------------------------------------------
 DV1718::~DV1718() {
  std::cout<<"destroying DV1718\n";

 } // end of VME_CRATE
//-----------------------------------------------------------------------------
 void DV1718::ConfigureModule() {

 }
//-----------------------------------------------------------------------------
 void DV1718::InitModule() {
 


if( VME_CRATE ){ // to test puropse only....
 CAENVME_SystemReset(m_VMEBridge);

 CAENVME_SetScalerConf(m_VMEBridge, 1023, 0, cvInputSrc0, cvManualSW, cvManualSW) ;
 CAENVME_EnableScalerGate(m_VMEBridge) ;
 
 } // end of VME_CRATE

 printf("Initialization DV1718 module done!\n");


 }
//-----------------------------------------------------------------------------
 void DV1718::ReadVME() {

 Int_t ret;
 
 UInt_t data2;

if( VME_CRATE ){ // to test puropse only....

 CAENVME_ReadRegister(m_VMEBridge, cvScaler1, &data2) ;
 m_Value += data2;
 CAENVME_ResetScalerCount(m_VMEBridge) ; 
 //if ( !(i%1000)){ 
 //   printf("scaler counts = %d\n", total);
 //   total = 0;
 //   }


  } // end of VME_CRATE

}

//-----------------------------------------------------------------------------
 void DV1718::ShowData(DGDisplay *fDisplay) {

 }

//-----------------------------------------------------------------------------
void DV1718::DataSave(DMultiGrid *fMultiGrid){

}

//-----------------------------------------------------------------------------
 void DV1718::ResetModule() {

 m_Value = 0;
 if(VME_CRATE){
    CAENVME_ResetScalerCount(m_VMEBridge) ; 
   }
}
//*****************************************************************************
void DV1718::ShowSettings() {
  std::cout << "======= DV1718::ShowSettings =======" << std::endl; 

  std::cout << "m_Input:       " << m_Input << std::endl; 
  std::cout << "m_EndCounts:   " << m_EndCounts << std::endl; 
  std::cout << "m_CountOutput: " << m_CountOutput << std::endl; 
  std::cout << "m_Value:      " << m_Value<< std::endl; 
  std::cout << "=====================================\n"<< std::endl; 
}

