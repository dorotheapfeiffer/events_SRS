#include "DV1718.h"
#include "DGDisplay.h"
#include "DMultiGrid.h"
#include <bitset>
#include <string>
#include <fstream>
#include "sys/time.h"
#include "TRandom.h"

extern Int_t VME_CRATE;

ClassImp(DV1718)

//*****************************************************************************
 DV1718::DV1718(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"VME Bridge",(char*)"CAEN V1718",mdesc,addr) {

  std::cout<<"\t+ module V1718...\n";


  //m_VMEBridge   = cvV1718;
  m_Value	= 0;
  m_Input	= 0;
  m_EndCounts	= 0;
  m_CountOutput = 0;
  //m_Handle	= -1;

  //std::cout<<"VME_CRATE: " << VME_CRATE << " m_VMEBridge: "<< m_VMEBridge << " m_Handle: " << m_Handle << "\n";
  if( VME_CRATE ){ // to test puropse only.... (test without crate)
  if (CAENVME_Init( m_VMEBridge, 0, 0, &m_Handle) != cvSuccess) {
     printf("[ERROR] opening the device!!! Crate is down, exit!!!!\n");
     exit(1);
     }
  else {
     std::cout << "\t\t- CAEN bridge V1718 Opened, Crate is OK\n";
     std::cout << "\t\t- Handle = " << m_Handle << std::endl;
     std::cout << "\t\t- construction V1718 done!\n";
    }
  }
}
//-----------------------------------------------------------------------------
 DV1718::~DV1718() {
  std::cout<<"\t+ module V1718...\n";

 } // end of VME_CRATE

//-----------------------------------------------------------------------------
 void DV1718::StartAcq() {
 
if( VME_CRATE ){ // to test puropse only....
  CAENVME_DecodeError( CAENVME_ResetScalerCount(m_VMEBridge) );
  }
 }

//-----------------------------------------------------------------------------
 void DV1718::StopAcq() {
 
if( VME_CRATE ){ // to test puropse only....
  CAENVME_DecodeError( CAENVME_ResetScalerCount(m_VMEBridge) );
  }
 }

//-----------------------------------------------------------------------------
 void DV1718::ConfigureModule() {

 }
//-----------------------------------------------------------------------------
 void DV1718::Log(std::ofstream & logfile) {
 //logfile << "TEST LOG FILE FROM DV1718" << std::endl;
 }
//-----------------------------------------------------------------------------
 void DV1718::InitModule() {
 


if( VME_CRATE ){ // to test puropse only....

  //unsigned int ret;

  CAENVME_SystemReset(m_VMEBridge);

  ///this is for scaler
  //scaler enable
  printf("Initialization Scaler in VME Bridge!\n");
  CAENVME_DecodeError( CAENVME_SetScalerConf(m_VMEBridge, 1023, 0, cvInputSrc0, cvManualSW, cvManualSW) );
  //printf("configuration ret = %d\n", ret);
  CAENVME_DecodeError( CAENVME_EnableScalerGate(m_VMEBridge) );
  //printf("Enalbe scaler ret = %d\n", ret);
 // if(ret) return;
 
 } // end of VME_CRATE

 printf("Initialization DV1718 module done!\n");


 }
//-----------------------------------------------------------------------------
 void DV1718::ReadVME() {

 //Int_t ret;
 
 UInt_t data2;

if( VME_CRATE ){ // to test puropse only....

 CAENVME_DecodeError( CAENVME_ReadRegister(m_VMEBridge, cvScaler1, &data2) );
 m_Value += data2;
 CAENVME_DecodeError( CAENVME_ResetScalerCount(m_VMEBridge) );

  } // end of VME_CRATE

}

//-----------------------------------------------------------------------------
 void DV1718::ShowData(DGDisplay *fDisplay, DMultiGrid *fMultiGrid) {
    
    //printf("Scaler value = %d\n", m_Value);
    //m_Value = 0;
 }

//-----------------------------------------------------------------------------
void DV1718::DataSave(DMultiGrid *fMultiGrid){

}

//-----------------------------------------------------------------------------
 void DV1718::ResetModule() {

  if(VME_CRATE){
    CAENVME_SystemReset(m_VMEBridge);
    m_Value = 0;
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

