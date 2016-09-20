#include "DMadc32.h"
#include "DGDisplay.h"
#include <bitset>
#include <string>
#include <fstream>
#include "TRandom.h"

typedef struct {
   UInt_t       n_words    : 12;
   UInt_t       adc_res    : 3;
   UInt_t       out_format : 1;
   UInt_t       module_id  : 8;
   UInt_t       sub_header : 6;
   UInt_t       header_sig : 2;
} EventHeader;

typedef struct {
   UInt_t       adc_data   : 14;
   UInt_t       overflow   : 1;
   UInt_t       nop        : 1;
   UInt_t       channel    : 5;
   UInt_t       fix        : 9;
   UInt_t       data_sig   : 2;
} DataWord;

typedef struct {
   UInt_t       counter_tts: 30;
   UInt_t       end_event  : 2;
} EndOfEvent;

static union {
   UInt_t       m_Header;
   EventHeader  m_EventHeader;
};

static union {
   UInt_t       m_Word;
   DataWord     m_DataWord;
};

static union {
   UInt_t       m_Footer;
   EndOfEvent   m_EndOfEvent;
};



static Int_t VME_CRATE = 1;
TRandom m_random;

  ClassImp(DMadc32)

 Int_t CheckError(CVErrorCodes ret){
   if     (ret == cvSuccess ) { //printf(" Cycle(s) completed normally\n"); 
                                return 0;}
   else if(ret == cvBusError) { printf(" Bus Error !!!\n");              return 1;} 
   else if(ret == cvCommError){ printf(" Communication Error !!!\n");     return 2;}
   else                       { printf(" Unknown Error !!!\n");           return 3;} 
 }

 static std::string Dec2BinStr(Char_t dec){
    std::string strBin;

    for(UInt_t i = 0x80; i > 0; i >>= 1){
       if(i & dec) strBin += '1';
       else        strBin += '0';
       }
 
return strBin;
}

//*****************************************************************************
 DMadc32::DMadc32(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"Pulse sensitive digitizer",(char*)"Mesytec Madc-32",mdesc,addr) {
	       std::cout<<"constructing DMadc32\n";


  m_VMEBridge	= cvV1718;
  m_Link	= 0;       
  m_Device	= 0;     
  m_Handle	= -1;

  m_BaseAddress = 0x0;
  m_ModuleID	= 0x6004;

  m_BufferPos	= 0;
  m_dataSize	= 0;
  m_Events	= 0;

  //clear the buffer
  for(Int_t i = 0; i < 810*1024*1024; i++)
     m_Buffer[i] = 0;


if( VME_CRATE ){ // to test puropse only.... (test without crate)
  if (CAENVME_Init( m_VMEBridge, 0, 0, &m_Handle) != cvSuccess) {
     printf("Error opening the device!!! Crate is down\n");
     exit(1);
     }
  else {
     printf("CAEN V1718 Opened, Crate is OK\n");
     printf("           Handle = %d\n", m_Handle);
    }

 CAENVME_SystemReset(m_VMEBridge);
 CAENVME_IRQEnable(m_VMEBridge, 7);
 // Set the module ID
 Int_t address = m_BaseAddress + m_ModuleID;
 Int_t data = 1;
 printf("CAENVME_WriteCycle: Set Module Id = 1\n");
 Int_t ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 } // end of VME_CRATE



}
//-----------------------------------------------------------------------------
 DMadc32::~DMadc32() {
  std::cout<<"destroying DMadc32\n";

  if( VME_CRATE ){ // to test puropse only....
      CAENVME_End(m_Handle);
    }
 
 } // end of VME_CRATE
//-----------------------------------------------------------------------------
 void DMadc32::ConfigureModule() {

 }
//-----------------------------------------------------------------------------
 void DMadc32::InitModule() {
 Int_t ret = 0; 
 Int_t address = 0;
 Int_t data = 0;



if( VME_CRATE ){ // to test puropse only....
 CAENVME_SystemReset(m_VMEBridge);


  // stop acquisition
 address = m_BaseAddress + 0x603A;
 data = 0;
 printf("CAENVME_WriteCycle: Stop acquisition\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 // Timestamp
 address = m_BaseAddress + 0x6038;
 data = 1;
 printf("CAENVME_WriteCycle: Set Timestamp not event counter %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
 
 // set multi event register
 address = m_BaseAddress + 0x6036;
 data = 3;
 printf("CAENVME_WriteCycle: Set Multi event = %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // Initialize IRQ - emit irq when more than 200 words in FIFO
 address = m_BaseAddress + 0x6018;
 data = 200;
 printf("CAENVME_WriteCycle: Initialize IRQ, emit irq when more than %d words in FIFO\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
 // max transfer data
 address = m_BaseAddress + 0x601A;
 data = 222;
 printf("CAENVME_WriteCycle: Max transfer data to %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // Initialize IRQ - irq vector
 address = m_BaseAddress + 0x6012;
 data = 0;
 printf("CAENVME_WriteCycle: Initialize IRQ vector = %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // Initialize IRQ - irq set when event converted
 address = m_BaseAddress + 0x6010;
 data = 1;
 printf("CAENVME_WriteCycle: Initialize IRQ, set when event converted\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
/*
 // Initialize IRQ - irq set when event converted
 address = m_BaseAddress + 0x6018;
 data = 100;
 printf("CAENVME_WriteCycle: Emited IRQ when more than 100 words in FIFO\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/

/*
 // pulser
 address = m_BaseAddress + 0x6070;
 data = 7;
 printf("CAENVME_WriteCycle: Set pulser to amplitude: 0->low->high->0...\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/
/*
 // ignore threshold
 address = m_BaseAddress + 0x604C;
 data = 1;
 printf("CAENVME_WriteCycle: Ignore threshol\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/
 // ADC resolution
 address = m_BaseAddress + 0x6042;
 data = 2;
 printf("CAENVME_WriteCycle: Set ADC resolution to %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
/*
 // Gate generator input NIM_gat1_osc
 address = m_BaseAddress + 0x606A;
 data = 0;
 printf("CAENVME_WriteCycle: Gate generator input at NIM_gat1\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/

 // Gate generator output at busy output
 address = m_BaseAddress + 0x606E;
 data = 1;
 printf("CAENVME_WriteCycle: Gate generator output monitoring at the NIM_busy\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // internal gate generator delay0
 address = m_BaseAddress + 0x6050;
 data = 0;
 printf("CAENVME_WriteCycle: Gate generator delay = %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
 

 // internal gate generator width 
 address = m_BaseAddress + 0x6054;
 data = 30;
 printf("CAENVME_WriteCycle: Gate generator width %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
 // internal gate generator 
 address = m_BaseAddress + 0x6058;
 data = 1;
 printf("CAENVME_WriteCycle: Internal gate generator\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
 
// reset couters
 //address = m_BaseAddress + 0x6090;
 //data = 1;
 //printf("CAENVME_WriteCycle: Set Multi event\n");
 //ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 //if(ret) return;

  // set threshold 
 address = m_BaseAddress + 0x4000;
 data = 40;
 for(Int_t i = 0; i < 32; i++){
    if(i == 0 || i == 1){ 
       printf("CAENVME_WriteCycle: Set threshold ch %d,  to: %d\n", i, data);
       ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address + 2*i, &data, cvA32_U_DATA, cvD16) );
       }
    else{
       data = 0x1FFF;
       printf("CAENVME_WriteCycle: Set threshold ch %d,  to: 0x%X\n", i, data);
       ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address + 2*i, &data, cvA32_U_DATA, cvD16) );
    }
    //printf("ret = %d\n", ret);
    if(ret) return;
    }


 // buffer initialization, reset
 address = m_BaseAddress + 0x603C;
 data = 0;
 printf("CAENVME_WriteCycle: Reset FIFO buffer\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // readout reset register
 address = m_BaseAddress + 0x6034;
 data = 0;
 printf("CAENVME_WriteCycle: Reset readout register\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // start acq
 address = m_BaseAddress + 0x603A;
 data = 1;
 printf("CAENVME_WriteCycle: Start ACQ\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 } // end of VME_CRATE


 printf("Initialization Madc32 module done!\n");


 }
//-----------------------------------------------------------------------------
 void DMadc32::ReadVME() {

 Int_t DEBUG = 1;

 Int_t ret = 0;
 Int_t bytesToRead = 0; 
 //Int_t bytesRead = 0; 
 Int_t address = 0;
 Int_t data = 0;
 CAEN_BYTE mask;

 UInt_t vector;
 CVDataWidth dtsize;
 static UInt_t nn_IRQ = 0;
 static UInt_t n1_IRQ = 0;
 static UInt_t n0_IRQ = 0;

if( VME_CRATE ){ // to test puropse only....
 // printf("%s\n", CAENVME_DecodeError( CAENVME_IRQWait(m_VMEBridge, 1, 5)));
 // printf("%s\n", CAENVME_DecodeError( CAENVME_IRQCheck(m_VMEBridge, &mask)));
 
  if( !CAENVME_IRQCheck(m_VMEBridge, &mask) ){
    if(mask == 0) { // no IRQ, exit
       n0_IRQ++; 
      //printf("no IRQ\n");
      return;
      }
    else if(mask == 1)  { //is IRQ = 1, need readout
       n1_IRQ++; 
       //printf("is IRQ\n"); 
       }
    else {
       //printf("ERROR in IRQ\n"); // some other IRQ
       nn_IRQ++;
       }
    }

 
  // check how many events are converted in buffer_data_length 0x6030 
  //UChar_t data1;
  //ret = CAENVME_ReadCycle(m_VMEBridge, 0x6030, &data1, cvA32_U_DATA, cvD16);
  //printf("read form buffer_data_length = %d, ret = %d\n", data1, ret);  
 
  // 65536, 67000
  bytesToRead = 2000;

  address = m_BaseAddress + 0x0; // 0x0 - begining of FIFO buffer;
  ret = CAENVME_BLTReadCycle(m_VMEBridge, address, &m_Buffer, bytesToRead, cvA32_U_DATA, cvD32, &data) ;
  //CAENVME_BLTReadCycle(m_VMEBridge, address, &m_Buffer, bytesToRead, cvA32_U_DATA, cvD32, &data) ;
  printf("check ret value after BLTReadCycle = %d \n", ret);
  //if(DEBUG) 
  if(DEBUG) printf("CAENVME_BLTReadCycle: Read buffer, has read = %d bytes\n",data);
  printf("n0_IRQ = %d, n1_IRQ = %d, nn_IRQ = %d, mask = %d\n", n0_IRQ, n1_IRQ, nn_IRQ, mask);

 m_BufferSize = data; 

 //reset IRQ and Berr
 if(DEBUG) printf("CAENVME_WriteCycle: Reset register readout_reset\n");
 address = m_BaseAddress + 0x6034; // 0x0 - begining of FIFO buffer;
 data = 0;
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(DEBUG) printf("ReadVME done!\n"); 


 } // end of VME_CRATE
/*
 if( !VME_CRATE ){
  
 // simulation of read data from VME
 m_random.Rndm();

 m_EventHeader.header_sig = 0x1;
 m_EventHeader.sub_header = 0;
 m_EventHeader.module_id  = 1;
 m_EventHeader.out_format = 0;
 m_EventHeader.adc_res    = 1;
 m_EventHeader.n_words    = (Int_t)(4 * m_random.Rndm()+2) ;

 m_Buffer[0] = m_Header;

 for(Int_t i = 1; i < m_EventHeader.n_words ; i++){
    m_DataWord.data_sig = 0;    
    m_DataWord.fix      = 0x20;    
    m_DataWord.channel  = i-1;
    m_DataWord.nop      = 0;
    m_DataWord.overflow = 0;
    m_DataWord.adc_data = 100*m_random.Gaus(3);
    m_Buffer[i] = m_Word;
    } 

    static int tts = 0;
    m_EndOfEvent.end_event = 0x3;
    m_EndOfEvent.counter_tts = tts++;
    m_Buffer[ m_EventHeader.n_words ] = m_Footer; 

 }
*/
   m_dataSize = m_BufferSize;
   m_Events++;
}

//-----------------------------------------------------------------------------
 void DMadc32::ShowData(DGDisplay *fDisplay) {
  Int_t value[33];
  Int_t tts;

  m_Header = m_Buffer[0];
  if(m_EventHeader.header_sig == 0x1){ // looking for a header word, return if not found
    //printf("is data header:\n  dsig: %d, m_EventHeader.n_words = %d \n", m_EventHeader.header_sig, m_EventHeader.n_words);
   }
  else{
    //printf("no data header: \n");
    return; 
   }

  Int_t n; 
   for(n = 0; n < m_EventHeader.n_words-1; n++){
       m_Word = m_Buffer[n+1];
       //printf("  dsig: %d, chn: %d, over: %d, adc: %d\n",m_DataWord.data_sig, m_DataWord.channel, m_DataWord.overflow, m_DataWord.adc_data);
       if(m_DataWord.channel == 0x0){   
          value[n] = m_DataWord.adc_data;
          //printf("n = %d, value[%d] = %d\n", n, n, value[n]);
          }
       else if(m_DataWord.channel == 0x1){   
          value[n] = m_DataWord.adc_data;
          //printf("n = %d, value[%d] = %d\n", n, n, value[n]);
          }
       //else 
          //printf("dupa\n");
       }

   m_Footer = m_Buffer[n+1]; // here check! if this is OK
   if(m_EndOfEvent.end_event == 0x3){  
      tts = m_EndOfEvent.counter_tts;
      //printf("  dsig1: %d, tts: %d, n = %d\n",m_EndOfEvent.end_event, tts, n);
      }   
   else{
      //printf("  dsig2: %d, tts: %d, n = %d\n",m_EndOfEvent.end_event, m_EndOfEvent.counter_tts, n);
   } 

   //printf("--------\n");

  
   // fill histograms
   
   fDisplay->hPh->Fill(value[0]);
   fDisplay->hPosition->Fill(value[1]);



  std::string readStr; 
  TString name("data4_07Apr2016");

  
/*
  TString aFileTxt = name + ".txt";
  std::ofstream DataFileTxt(aFileTxt, std::ofstream::out | std::ofstream::app);
  if(!DataFileTxt.is_open()) {
     std::cout << "ERROR could not open the file... " << aFileTxt << std::endl;
    }
  else{
    DataFileTxt << m_Buffer[1] << " " << m_Buffer[2] << " " << m_Buffer[33] << std::endl;
    DataFileTxt.close();
    }
*/
/*
  //printf("%d %d %d\n", 0xCFF & m_Buffer[1], 0xCFF & m_Buffer[2], 0x3FFFFFFF & m_Buffer[33]);
  for(Int_t i = 0; i < m_BufferSize/4; i++){
     //if(i == 33 ) printf("%d %d\n", i, (0x00FFFFFF & m_Buffer[i])); 
     //else printf("%d %d\n", i, m_Buffer[i]); 
     //printf("%d 0x%X\n", i, m_Buffer[0]); 
     //if(i == 0){
     readStr = Dec2BinStr((m_Buffer[i] & 0xFF000000) >> 24) + std::string("'") + Dec2BinStr((m_Buffer[i] & 0xFF0000) >> 16) + std::string("'") 
             + Dec2BinStr((m_Buffer[i] & 0x0000FF00) >> 8 ) + std::string("'") + Dec2BinStr((m_Buffer[i] & 0x0000FF));
     std::cout << i << " " << readStr << std::endl;    
     //}
     }
*/

/*
  TString aFile = name + ".bin";
  std::ofstream DataFile(aFile, std::ofstream::out | std::ofstream::app);
  if(!DataFile.is_open()) {
     std::cout << "ERROR could not open the file... " << aFile << std::endl;
    }
  else{
    DataFile.write((char *) m_Buffer, m_BufferSize);
    DataFile.close();
    }
*/

 }

//-----------------------------------------------------------------------------
void DMadc32::DataSave(DMultiGrid *fMultiGrid){
/*
  if( m_dataSize > fMultiGrid->m_SaveDataSize || fMultiGrid->m_ElspsedTime > fMultiGrid->m_SaveDataTime){
    static int nrFile = 0;
    char filename[256];
    sprintf(filename, "%s_%3d.bin", fMultiGrid->m_FileName, nrFile++);
    std::cout << "DMadc32 " << filename << std::endl;

    std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::app);
    if(!DataFile.is_open()) {
       std::cout << "ERROR could not open the file... " << filename << std::endl;
       }
    else {
       DataFile.write((char *) m_Buffer, m_BufferSize);
       DataFile.close();
      }

    }
 */
}

//-----------------------------------------------------------------------------
 void DMadc32::ResetModule() {

  m_dataSize = 0;
  m_Events   = 0;;

}
//*****************************************************************************
