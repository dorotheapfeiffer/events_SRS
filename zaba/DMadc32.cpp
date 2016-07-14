#include "DMadc32.h"
#include "DGDisplay.h"
#include <bitset>
#include <string>
#include <fstream> 
#include "sys/time.h"
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


 static ULong_t gCurrentTimeADC = 0;
 static ULong_t gElapsedTimeADC = 0;
 static ULong_t gPrevRateTimeADC = 0;
 //static ULong_t gPrevSizeADC = 0;

//===========================================================
static ULong_t gGetLongTimeADC(){
 struct timeval t1;
 struct timezone tz;
 gettimeofday(&t1, &tz);

return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}
//===========================================================

extern int  VME_CRATE; //this is for test only, if you want to change the value, change it in main.cpp
TRandom m_random;

using namespace std;

//===========================================================


  ClassImp(DMadc32)

//===========================================================

static  Int_t CheckError(CVErrorCodes ret){
   if     (ret == cvSuccess ) { /* printf(" Cycle(s) completed normally\n"); */ return 0;}
   else if(ret == cvBusError) {    printf(" Bus Error !!!\n");                  return 1;} 
   else if(ret == cvCommError){    printf(" Communication Error !!!\n");        return 2;}
   else                       {    printf(" Unknown Error !!!\n");              return 3;} 
 }

//===========================================================
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


  m_BaseAddress = 0x0;
  m_ModuleID	= 0x6004;

  m_BufferPos	= 0;
  m_dataSizeByte= 0;
  m_Events	= 0;
  m_IRQ		= 0;


  //clear the buffer
  for(Int_t i = 0; i < 810*1024*1024; i++)
     m_Buffer[i] = 0;


  for(Int_t i = 0; i < 32; i++){
      m_ThresholdValue[i] = 0x1FFF;
      m_ThresholdOn[i] = 0;
      m_ThresholdCache[i] = 0;
      }

  m_IgnoreThreshold	= 0;
  m_ADCResolution	= 2;
  m_GateGenerator	= 0;
  m_GateDelay		= 0;
  m_GateWidth		= 0;
  m_GateOutput		= 0;

  VME_CRATE = 1;
 std::cout << "VME_CRATE (0)- simulation (1) - real instrument, VME_CRATE = " << VME_CRATE << endl; 
 std::cout << "CONSTR m_GateGenerator: " << m_GateGenerator << " m_GateDelay: " << m_GateDelay 
           << " m_GateWidth: " << m_GateWidth << " m_GateOutput: " << m_GateOutput <<"\n";

  if( VME_CRATE ){ // to test puropse only....
     CAENVME_SystemReset(m_VMEBridge);
     CAENVME_IRQEnable(m_VMEBridge, 7);
  

     // Set the module ID
     Int_t address = m_BaseAddress + m_ModuleID;
     Int_t data = 1;
     printf("CAENVME_WriteCycle: Set Module Id = 1\n");
     Int_t ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
     if(ret) return;
   }


std::cout << "end of constructor\n";
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
 data = 0x0;
 printf("CAENVME_WriteCycle: Stop acquisition\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 // Timestamp
 address = m_BaseAddress + 0x6038;
 data = 0x1;
 if(data == 1) printf("CAENVME_WriteCycle: Set Timestamp%d\n", data);
 else if(data == 0) printf("CAENVME_WriteCycle: Set Counter%d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
 
 // set multi event register
 address = m_BaseAddress + 0x6036;
 data = 0x3;
 printf("CAENVME_WriteCycle: Set Multi event = %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // Initialize IRQ - emit irq when more than 20 words in FIFO
 address = m_BaseAddress + 0x6018;
 data = 10;
 printf("CAENVME_WriteCycle: Initialize IRQ, emit irq when more than %d words in FIFO\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
 // max transfer data
 address = m_BaseAddress + 0x601A;
 data = 1000;
 printf("CAENVME_WriteCycle: Max transfer data to %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // Initialize IRQ - irq vector
 address = m_BaseAddress + 0x6012;
 data = 0x0;
 printf("CAENVME_WriteCycle: Initialize IRQ vector = %d\n", data);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // Initialize IRQ - irq set when event converted
 address = m_BaseAddress + 0x6010;
 data = 0x1;
 printf("CAENVME_WriteCycle: Initialize IRQ, set when event converted\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

/*
 // pulser
 address = m_BaseAddress + 0x6070;
 data = 7;
 printf("CAENVME_WriteCycle: Set pulser to amplitude: 0->low->high->0...\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/

 // ignore threshold
 address = m_BaseAddress + 0x604C;
 if (m_IgnoreThreshold) printf("CAENVME_WriteCycle: Ignore thresholds\n");
 else                   printf("CAENVME_WriteCycle: Thresholds will be set individualy\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &m_IgnoreThreshold, cvA32_U_DATA, cvD16) );
 if(ret) return;

 // ADC resolution
 address = m_BaseAddress + 0x6042;
 printf("CAENVME_WriteCycle: Set ADC resolution to %d\n", m_ADCResolution);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &m_ADCResolution, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
/*
 // Gate generator input NIM_gat1_osc
 address = m_BaseAddress + 0x606A;
 data = 0;
 printf("CAENVME_WriteCycle: Gate generator input at NIM_gat1\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/

 //std::cout<< "INIT m_GateGenerator: " << m_GateGenerator << " m_GateDelay: " << m_GateDelay << " m_GateWidth: " << m_GateWidth << " m_GateOutput: " << m_GateOutput <<"\n";
 // Gate generator output at busy output
 address = m_BaseAddress + 0x606E;
 printf("CAENVME_WriteCycle: Gate generator output monitoring at the NIM_busy %d\n", m_GateOutput);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &m_GateOutput, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // internal gate generator delay0
 address = m_BaseAddress + 0x6050;
 printf("CAENVME_WriteCycle: Gate generator delay = %d\n", m_GateDelay);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &m_GateDelay, cvA32_U_DATA, cvD16) );
 if(ret) return;
 

 // internal gate generator width 
 address = m_BaseAddress + 0x6054;
 printf("CAENVME_WriteCycle: Gate generator width %d\n", m_GateWidth);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &m_GateWidth, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
 // internal gate generator 
 address = m_BaseAddress + 0x6058;
 printf("CAENVME_WriteCycle: Internal gate generator %d\n", m_GateGenerator);
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &m_GateGenerator, cvA32_U_DATA, cvD16) );
 if(ret) return;
 
// reset tts
 address = m_BaseAddress + 0x606C;
 data = 0x1;
 printf("CAENVME_WriteCycle: Reset Trigger Time Stamp, (NIM_fc_reset)\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

/*
// ts_source
 address = m_BaseAddress + 0x6096;
 data = 0x2; // frequency source VME, enable reset
 printf("CAENVME_WriteCycle: Enable ts_source from VME and enable reset\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

// ts_divisor
 address = m_BaseAddress + 0x6098;
 data = 0x0; // 0 means division by 65536 
 printf("CAENVME_WriteCycle: Set Divisor to the TTS\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;
*/

  // set threshold 
address = m_BaseAddress + 0x4000;
 for(Int_t i = 0; i < 32; i++){
     ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address + 2*i, &m_ThresholdValue[i], cvA32_U_DATA, cvD16) );
    }

 // buffer initialization, reset
 address = m_BaseAddress + 0x603C;
 data = 0x0;
 printf("CAENVME_WriteCycle: Reset FIFO buffer\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;


 // readout reset register
 address = m_BaseAddress + 0x6034;
 data = 0x0;
 printf("CAENVME_WriteCycle: Reset readout register\n");
 ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
 if(ret) return;

 
 } // end of VME_CRATE

 printf("Initialization Madc32 module done!\n");


 }
//-----------------------------------------------------------------------------
 void DMadc32::ReadVME() {

 Int_t bytesToRead = 0; 
 Int_t address = 0;
 Int_t data = 0;
 Int_t ret;

 static UInt_t nn_IRQ = 0;
 static UInt_t n1_IRQ = 0;
 static UInt_t n0_IRQ = 0;


 static int i = 0;
 i++;

 CAEN_BYTE mask;

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
       m_IRQ = 1; 
       //printf("is IRQ n1\n"); 
       }
    else {
       printf("ERROR in IRQ\n"); // some other IRQ
       nn_IRQ++;
       return;
       }
    }

  bytesToRead = 256; 
  bool DEBUG_READ_DATA = 0;

  UInt_t offset = 0;
  UInt_t nn = 0;

  address = m_BaseAddress + 0x0; // 0x0 - begining of FIFO buffer;

  //UInt_t buffer_length = 0;
  //ret = CAENVME_ReadCycle(m_VMEBridge, 0x6030, &buffer_length, cvA32_U_DATA, cvD32) ;
  //ret = CAENVME_BLTReadCycle(m_VMEBridge, address, &m_localBuffer, bytesToRead, cvA32_U_DATA, cvD32, &m_dataSizeByte) ;
  //printf("buffer length = %u, check ret value after BLTReadCycle = %d, read: %d bytes\n", 0x0000FFFF & buffer_length, ret, m_dataSizeByte);
  
  UInt_t *a = m_localBuffer;
  UInt_t buffer_length = 0;
  ret = CAENVME_ReadCycle(m_VMEBridge, 0x6030, &buffer_length, cvA32_U_DATA, cvD32) ;
  while(nn < 25 && ret == 0){
      ret = CAENVME_BLTReadCycle(m_VMEBridge, address, &m_localBuffer[ offset ], bytesToRead, cvA32_U_DATA, cvD32, &m_dataSizeByte) ;
      //ret = CAENVME_BLTReadCycle(m_VMEBridge, address, a+offset, bytesToRead, cvA32_U_DATA, cvD32, &m_dataSizeByte) ;
      offset += m_dataSizeByte;
      //printf("buffer length = %u, check ret value after BLTReadCycle = %d, read: %d bytes, nn = %d, offset = %d\n", 
      //       0x0000FFFF & buffer_length, ret, m_dataSizeByte, nn, offset);
      nn++;
    }
  


  if(DEBUG_READ_DATA) printf("check ret value after BLTReadCycle = %d, read: %d bytes, in hex = 0x%X\n", ret, m_dataSizeByte, m_dataSizeByte);


  m_dataSizeByte = offset;
  for(Int_t i = 0; i < m_dataSizeByte/4; i++){
     UInt_t id = (m_localBuffer[i] >> 30) & 0x3;
     switch(id)
           {
           case 0: 
             if(DEBUG_READ_DATA) printf("DATA,   m_localbuffer[%d], chn: %u, value: %u\n", i, (m_localBuffer[i]>>16) & 0x1F, m_localBuffer[i] & 0x1FFF);
           break; 

           case 1:
             if(DEBUG_READ_DATA) printf("HEADER, m_localbuffer[%d], nr_of_words: %u\n", i, m_localBuffer[i] & 0xFF);
             m_Events++;
           break;

           case 3:
               if(DEBUG_READ_DATA) printf("FOOTER, m_localbuffer[%d], tts: %u\n", i, m_localBuffer[i] & 0xCFFFFFFF);
           break;

           default:
             if(DEBUG_READ_DATA) printf("ERROR buffer[%d]\n", i);
           break;
           }
     }

  //printf("------------------------check ret value after BLTReadCycle = %d, read: %d bytes, nn = %d, Events: %d\n", ret, m_dataSizeByte, nn, m_Events);
  //reset IRQ and Berr
  address = m_BaseAddress + 0x6034; // 0x0 - begining of FIFO buffer;
  data = 0;
  ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );

  } // end of VME_CRATE

 if( !VME_CRATE ){
  
 // simulation of read data from VME
 // Int_t nr = (Int_t)(100*m_random.Rndm());
 //std::cout << "hello 1" << std::endl; 

  Int_t m_BufferPos1 = 0;
  m_dataSizeByte = 0;
 for(Int_t inr = 0; inr < 1000; inr++){
     m_EventHeader.header_sig = 0x1;
     m_EventHeader.sub_header = 0;
     m_EventHeader.module_id  = 1;
     m_EventHeader.out_format = 0;
     m_EventHeader.adc_res    = 1;
     //m_EventHeader.n_words    = (Int_t)(4 * m_random.Rndm()+2) ;
     m_EventHeader.n_words    = 9; // 8ch + 1tts

     m_localBuffer[ m_BufferPos1 + 0] = m_Header;

     for(Int_t i = 0; i < m_EventHeader.n_words-1 ; i++){
         m_DataWord.data_sig = 0;    
         m_DataWord.fix      = 0x20;    
         m_DataWord.channel  = i;
         m_DataWord.nop      = 0;
         m_DataWord.overflow = 0;
         m_DataWord.adc_data = 100*m_random.Gaus(3);
         m_localBuffer[ m_BufferPos1 + i + 1] = m_Word;
     } 

     static int tts = 0;
     m_EndOfEvent.end_event = 0x3;
     m_EndOfEvent.counter_tts = tts++;

     m_localBuffer[ m_BufferPos1 + m_EventHeader.n_words] = m_Footer; 

     m_Events++;
     m_BufferPos1 += (m_EventHeader.n_words + 1);
     m_dataSizeByte += 4 + m_EventHeader.n_words * 4; // header 4bytes, nwords each 4bytes + tts also 4bytes
     //cout << "m_BufferPos1 " << m_BufferPos1 << ",  m_dataSizeByte " << m_dataSizeByte << endl;
  }
  //cout << "Simulation data: m_dataSizeByte = " << m_dataSizeByte << endl;
 }//end of VME_CRATE


}

//-----------------------------------------------------------------------------
void DMadc32::StartAcq(){

 
 cout << "DMadc32::StartAcq()" << endl;
 ShowSettings();
 InitModule(); 

 unsigned int address = m_BaseAddress + 0x603A;
 unsigned int data = 0x1;
 if( VME_CRATE ){
   //printf("CAENVME_WriteCycle: Start ACQ\n");
   int ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
   if(ret) return;
   }
}

//-----------------------------------------------------------------------------
void DMadc32::StopAcq(){
 cout << "DMadc32::StopAcq()" << endl;
 unsigned int address = m_BaseAddress + 0x603A;
 unsigned data = 0x0;
 if( VME_CRATE ){
   //printf("CAENVME_WriteCycle: Stop acquisition\n");
   int ret = CheckError( CAENVME_WriteCycle(m_VMEBridge, address, &data, cvA32_U_DATA, cvD16) );
   if(ret) return;
   }
}
//-----------------------------------------------------------------------------
 void DMadc32::ShowData(DGDisplay *fDisplay) {

  bool DEBUG_SHOW_DATA = 0;

  Int_t value[33];
  for(Int_t i = 0; i < 32; i++) value[32] = 0;

  Int_t tts = 0l;

  for(Int_t i = 0; i < m_dataSizeByte/4; i++){
      m_Header = m_localBuffer[i];
      if(m_EventHeader.header_sig == 0x1){ // looking for a header word, return if not found
         if(DEBUG_SHOW_DATA) printf("is data header:\n  dsig: %d, m_EventHeader.n_words = %d \n", m_EventHeader.header_sig, m_EventHeader.n_words);
        }
      else{
         if(DEBUG_SHOW_DATA) printf("no data header: \n");
       continue; 
        }


      Int_t n ;
     
      for(n = 0; n < m_EventHeader.n_words-1; n++){
          m_Word = m_localBuffer[i+n+1];
          value[m_DataWord.channel] = m_DataWord.adc_data;
          if(DEBUG_SHOW_DATA) printf("  dsig: %d, chn: %d, over: %d, adc: %d\n",m_DataWord.data_sig, m_DataWord.channel, m_DataWord.overflow, m_DataWord.adc_data);
          }

      m_Footer = m_localBuffer[i+n+1]; // here check! if this is OK

      if(m_EndOfEvent.end_event == 0x3){  
         tts = m_EndOfEvent.counter_tts;
         if(DEBUG_SHOW_DATA) printf("  dsig: %d, tts: %u, n = %d, i = %d, (n+i+1) = %d\n",m_EndOfEvent.end_event, tts, n, i, n+i+1);
         if(DEBUG_SHOW_DATA) printf("tts %u\n", tts);
         }   
      else{
         //printf("  dsig2: %d, tts: %d, n = %d, i = %d, (i+n+1) = %d\n",m_EndOfEvent.end_event, m_EndOfEvent.counter_tts, n, i, i+n+1);
      } 

   if(DEBUG_SHOW_DATA) printf("--------\n");

  
   // fill histograms
   
   fDisplay->hPh[0]->Fill(value[0]);
   fDisplay->hPosition[0]->Fill(value[1]);

   fDisplay->hPh[1]->Fill(value[2]);
   fDisplay->hPosition[1]->Fill(value[3]);
   
   fDisplay->hPh[2]->Fill(value[4]);
   fDisplay->hPosition[2]->Fill(value[5]);

   fDisplay->hPh[3]->Fill(value[6]);
   fDisplay->hPosition[3]->Fill(value[7]);

   fDisplay->hTTS->Fill(tts);
   }

 }

//-----------------------------------------------------------------------------
void DMadc32::DataSave(DMultiGrid *fMultiGrid){

   // write data to buffer only wnhen the IRQ happend, otherwise return 
   // CHECK IF THIS IS REALLY NECESSARY, there is m_dataSizeByte which shoud be 0 if there is no IRQ
   // If you want to simulate events you have to comment this line, m_IRQ = 1;
   if( !VME_CRATE ) m_IRQ = 1; 
   if(!m_IRQ) return;
      m_IRQ = 0;

   Bool_t m_saveAfterSize = kFALSE; // variable to check if the buffer size reach the max size 
   Bool_t m_saveAfterTime = kFALSE; // variable to check if the time elapsed to save file every sec/min/hour

   gCurrentTimeADC = gGetLongTimeADC() / 1000;              // check the current time
   gElapsedTimeADC = gCurrentTimeADC - gPrevRateTimeADC;    // calcullate elapsed time

   memcpy((void*)m_Buffer + m_BufferPos, (void*)m_localBuffer, m_dataSizeByte); // copy data from local buffor to main beffor
   m_BufferPos += (m_dataSizeByte);                                 // move the position of the buffer

   // ==========================================================================
   // check if the buffer is full and need to be writen to file
   if( m_BufferPos > UInt_t(fMultiGrid->m_SaveFileSizeEntry*1024*1024) ) {
       m_saveAfterSize = kTRUE;
       }

  // ===========================================================================
  // check the elapsed time
  // cout << "time condition" << gElapsedTimeADC << " " <<  (ULong_t)fMultiGrid->m_SaveFileTimeEntry << endl;
   if( gElapsedTimeADC > (ULong_t)fMultiGrid->m_SaveFileTimeEntry){
       if( gPrevRateTimeADC == 0) {
           m_saveAfterTime = kFALSE;
           gPrevRateTimeADC = gCurrentTimeADC;
           }
       else { 
           m_saveAfterTime = kTRUE;  
          }
     }
  // =============================================================================
  // writing bufer to file if one of the two condition are fulfill
  if( m_saveAfterSize || m_saveAfterTime ){
    char nr[256];
    sprintf(nr, "%03d", fMultiGrid->m_NrOfSavedFiles);



    //std::time_t t = std::time(NULL);
    //char mbstr[100];
    //if (std::strftime(mbstr, sizeof(mbstr), "%Y_%m_%d_%H%M_", std::localtime(&t))) {
    //    std::cout << mbstr << '\n';
   // }

    string filename = fMultiGrid->m_DataPath + string("/") + fMultiGrid->GetFileTime() 
	   + string("_") + fMultiGrid->GetFileName() + string("_") + string(nr) + string(".bin");    


    std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::binary);
    if(!DataFile.is_open()) {
       std::cout << "ERROR could not open the file... " << filename << std::endl;
       }
    else {
       DataFile.write((char*)m_Buffer, m_BufferPos); 
       DataFile.close();
      }
 
 // ===============================================================================
 // some cleaning after saving and writing message to the std output
    m_BufferPos = 0;
    fMultiGrid->m_NrOfSavedFiles++;
    gPrevRateTimeADC = gCurrentTimeADC;
    std::cout << "[MESSAGE] data file [" << filename << "] has saved " << std::endl;
    std::cout << "          Nr Events= " << m_Events << std::endl;
    }

}

//-----------------------------------------------------------------------------
 void DMadc32::ResetModule() {

  m_dataSizeByte = 0;
  m_BufferPos = 0;
  m_Events   = 0;;

}
//*****************************************************************************
void DMadc32::ShowSettings() {
  std::cout << "======= DMadc32::ShowSettings =======" << std::endl; 

  std::cout << "m_IgnoreThreshold: "<< m_IgnoreThreshold << std::endl; 
  std::cout << " chn:   value    chn:   value"<< std::endl; 
  for(Int_t i = 0; i < 16; i++){
    std::cout << "ch["<< i << "] " << m_ThresholdOn[i] << ": " << m_ThresholdValue[i] << "\t[" << i+16 << "] " << m_ThresholdOn[i+16] << ": " << m_ThresholdValue[i+16] << std::endl; 
    //std::cout << i << ": "<< m_ThresholdValue[i] << "   " << i+16 << " " << m_ThresholdValue[i+16] << std::endl; 
  }
  std::cout << "m_GateGenerator: " << m_GateGenerator   << std::endl; 
  std::cout << "m_GateDelay    : " << m_GateDelay       << std::endl; 
  std::cout << "m_GateWidth    : " << m_GateWidth       << std::endl; 
  std::cout << "m_GateOutput   : " << m_GateOutput      << std::endl; 
  std::cout << "m_ADCResolution: " << m_ADCResolution   << std::endl; 
  std::cout << "m_: " << m_ADCResolution   << std::endl; 
  std::cout << "=====================================\n"<< std::endl; 
}

//==================================================================================
 void DMadc32::SaveConfig(std::ofstream & fout){

 fout << "# Instrument properties: " << std::endl;
 fout << "# Keep in mind that there are only few parameters which user can change or modified." << std::endl;
 fout << "# There are much more but for the standard using they are not neccesary to set." << std::endl;
 fout << "# The parameters below are only these which you can change in the GUI." << std::endl;
 fout << "# Settings another parameters require adding some code and recompile. The simplest way is to edit DMadc.32.cpp" << std::endl;
 fout << "# and in the function InitModule either uncomment or write which register and value you wan to set. Similar to what is already there." << std::endl;
 fout << "# For details of other settings see the documentation." << std::endl;
 fout << "[" <<  GetActTypeDescription()  << "]" << std::endl;
 fout << "BaseAddress "  << m_BaseAddress << std::endl;
 fout << "ModuleID "     << m_ModuleID << std::endl;

 fout << std::endl;

 fout << "#By setting this to 1 you can ignore thresholds in all channels" << std::endl;
 fout << "IgnoreThresholds "<< m_IgnoreThreshold << std::endl; 
 fout << std::endl;
 fout << "#Channel configuration. Turn channel on/off, can be only 0 - 1, value of threshold from 0 - 0x1FFF (8191)" << std::endl;
  for(Int_t i = 0; i < 32; i++){
    fout << "ch["<< i << "] " << m_ThresholdOn[i] << " " << m_ThresholdValue[i] << std::endl; 
  }
 fout << std::endl;
 fout << "#Activate gate generator, values 0 or 1" << std::endl;
 fout << "GateGenerator " << m_GateGenerator   << std::endl; 
 fout << std::endl;

 fout << "#Set the Gate Delay, values between 0 - 255, 0 = 25ns, 1 = 50ns, then multiple of 50ns" << std::endl;
 fout << "GateDelay "          << m_GateDelay       << std::endl; 
 fout << std::endl;
 fout << "#Set the Gate Width, values between 0 - 255, multiple of 50ns" << std::endl; 
 fout << "GateWidth "          << m_GateWidth       << std::endl; 
 fout << std::endl; 
 fout << "#Set the Gate Output"                     << std::endl; 
 fout << "GateOutput "         << m_GateOutput      << std::endl; 
 fout << std::endl;
 fout << "#Set the ADC Resolution"                  << std::endl; 
 fout << "ADCResolution "      << m_ADCResolution   << std::endl; 
 fout << std::endl;
 

}


//==================================================================================
 void DMadc32::LoadConfig(std::ifstream & inpfile){
 
  cout << "Load configuration file MADC-32......" << endl;  

  Int_t temp;
  string line;
  string name;
  string value;
  string inSection;
  int posEqual;
  int lineNr = 0;

   while (getline(inpfile,line)) {
    lineNr++;
    if ( !line.length()) continue;

    if (line[0] == '#') continue;
    if (line[0] == ';') continue;

    if (line[0] == '[') {
      inSection = Trim(line.substr(1,line.find(']')-1));
      continue;
      }

    posEqual=line.find(' ');
    name  = Trim(line.substr(0,posEqual));
    value = Trim(line.substr(posEqual+1));

     if( inSection == string("Mesytec Madc-32")){
       if( name == string("BaseAddress") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_BaseAddress = 0;
         else m_BaseAddress = temp;
         }
       else if( name == string("ModuleID") ){
         temp = atoi( value.c_str() );
         if(temp < 0) m_ModuleID = 0;
         else m_ModuleID = temp;
         }
       else if( name == string("IgnoreThresholds") ){
         temp = atoi( value.c_str() );
         if(temp < 0 || temp > 1) m_IgnoreThreshold = 0;
         else  m_IgnoreThreshold = temp;
               
         }

       else if( name == string("GateGenerator") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_GateGenerator = 0;
         else m_GateGenerator = 1;
         }
       else if( name == string("GateDelay") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_GateDelay = 0;
         else if (temp > 255) m_GateDelay = 255; 
         else m_GateDelay = temp;
         }
       else if( name == string("GateWidth") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_GateWidth = 0;
         else if(temp > 255) m_GateWidth = 255;  
         else m_GateWidth = temp;
         }
       else if( name == string("GateOutput") ){
         temp = atoi( value.c_str() );
         if(temp <= 0) m_GateOutput = 0;
         else m_GateOutput = temp;
         }
       else if( name == string("ADCResolution") ){
         temp = atoi( value.c_str() );
         if(temp < 0 || temp > 5) m_ADCResolution = 2;
         else m_ADCResolution = temp;
         }


       else if( name.substr(0,3) == string("ch[")){
         line.erase(0,3);
         line.erase( line.find("]", 0), 1);
         int chNr = 0, chOn = 0, chVal = 0; // channel number; channel on/off; channel threshold value;
         sscanf(line.c_str(), "%d %d %d", &chNr, &chOn, &chVal);  
         if( chOn) { 
             if(m_IgnoreThreshold){
                m_ThresholdOn[chNr] = 1;
                m_ThresholdValue[chNr] = 0;
                m_ThresholdCache[chNr] = chVal;
                }
             else{
                m_ThresholdOn[chNr] = 1;
                m_ThresholdValue[chNr] = chVal;
               }      
          }
        }
     }

    } // end while loop


 }
