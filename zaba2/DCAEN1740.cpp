#include "DCAEN1740.h"
#include "DGDisplay.h"
#include <bitset>
#include <string>
#include <fstream> 
#include "sys/time.h"
#include "TRandom.h"
#include "gnuplot-iostream.h"
#include "_CAENDigitizer_DPP-QDC.h"

#define MAX_AGGR_NUM_PER_BLOCK_TRANSFER   1023 /* MAX 1023 */

 static ULong_t gCurrentTimeADC = 0;
 static ULong_t gElapsedTimeADC = 0;
 static ULong_t gPrevRateTimeADC = 0;
 //static ULong_t gPrevSizeADC = 0;

unsigned int gEquippedGroups;
_CAEN_DGTZ_DPP_QDC_Event_t     *gEventsGrp[8];

//===========================================================
static ULong_t gGetLongTimeADC(){
 struct timeval t1;
 struct timezone tz;
 gettimeofday(&t1, &tz);

return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}
//===========================================================

static	std::string mode(Int_t a){
	if(a == 0)      return std::string(" [disabled]");
	else if(a == 1) return std::string(" [acq only]");
	else if(a == 2) return std::string(" [ext only]");
	else if(a == 3) return std::string(" [acq &ext]");
	else            return std::string(" [unknown]");
	}
 	
static	std::string edge(Int_t a){
	if(a == 0)      return std::string("rise");
	else if(a == 1) return std::string("fall");
	else            return std::string("unknown");
	}
 	
static	std::string polarity(Int_t a){
	if(a == 0)      return std::string("NEG");
	else if(a == 1) return std::string("POS");
	else            return std::string("unknown");
        }
static	std::string iolevel(Int_t a){
	if(a == 0)      return std::string("    NIM");
	else if(a == 1) return std::string("    TTL");
	else            return std::string("unknown");
	}

static	std::string acqmode(Int_t a){
	if(a == 0)      return std::string(" SOFT ");
	else if(a == 1) return std::string(" HARD ");
	else            return std::string("unknown");
	}

//===========================================================

static std::string Dec2BinStr2(Char_t dec){
   std::string strBin;
  
  for(UInt_t i = 0x80; i > 0; i >>= 1){
      if(i & dec) strBin += '1';
      else        strBin += '0';
      }

return strBin;
}
//===========================================================
static std::string Dec2BinStr(Char_t dec){
  std::string strBin;

  for(UInt_t i = 0x1; i <= 0x80; i <<= 1){
     if(i & dec) strBin += '1';
     else        strBin += '0';
     }

return strBin;
}

//===========================================================


extern int  VME_CRATE; //this is for test only, if you want to change the value, change it in main.cpp
using namespace std;

//===========================================================

  ClassImp(DCAEN1740)

//*****************************************************************************
 DCAEN1740::DCAEN1740(Char_t *mdesc, UInt_t addr) : 
       DModule((char*)"64 channels digitizer 62.5MHz",(char*)"CAEN v1740",mdesc,addr) {

  std::cout<<"\t+ module CAEN 1740...\n";

  m_BaseAddress		= addr;
  m_ModuleID		= 0x6004;
  m_Firmware		= 1;
  m_Enabled		= 0;

  m_localBuffer		= NULL;
  m_AcqMode		= 0;     
  m_Handle		= -1;     
  m_Clock		= 0;     
  m_MaxEventsBLT	= 1; 
  m_RecordLength	= 512; 
  m_Range		= 4096;
  m_Range_mV		= 2000;
  m_NrChannels		= 64;
  m_NrGroups		= 8;
  m_DecimationFactor	= 1;   
  m_PostTrigger		= 50;       
  m_SaveChannel		= 7;      
  m_SWTriggerMode	= 1;   
  m_ExtTriggerMode	= 1; 
  m_FPIOtype		= 0;          
  m_RunSyncMode		= CAEN_DGTZ_RUN_SYNC_Disabled;       
  m_Delay		= 0;            
  m_IRQ 		= 0;

  m_Events		= 0;
  m_dataSizeByte	= 0;
  m_EventsInBuffer	= 0;

 for(UInt_t i = 0; i < m_NrGroups; i++){
     m_TriggerEdge[i]   	 = 0;
     m_Threshold_mV[i]           = 0;
     m_Threshold[i]              = mV2ADC(m_Threshold_mV[i]);
     m_DCoffset_mV[i]            = 0;
     m_DCoffset[i]               = mV2DAC(m_DCoffset_mV[i]);
     m_ChannelTriggerMode[i]     = 1;
     m_GroupEnableMask[i]  = 0;
     m_ChannelPulsPolarity[i] = 0; 
     } 
 m_GroupEnableMask[0]  = 1; 
 
 for(UInt_t i = 0; i < 64; i++)
     m_SelfTriggerMask[i] = 0;
 m_SelfTriggerMask[0] = 1; // this is not in the for loop, set element 0 to 1;

 m_Name = string(mdesc);

 UInt_t LinkNum = 0;
 UInt_t ConetNode = 0;


 if( VME_CRATE ){ 
    std::cout << "try to open digitizer" << std::endl;	 
    ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, 0, 0, (uint32_t)0x32100000, &m_Handle);
    //ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, LinkNum, ConetNode, m_BaseAddress, &m_Handle);
    if(ret != CAEN_DGTZ_Success){
       CheckError(ret);
       std::cout << "[ERROR] Open digitizer, error code = " << ret << "\n";  
       std::cout << "        LinkNum: " <<  LinkNum << ", ConetNode: " << ConetNode << ", m_BaseAddress: 0x" << hex <<  m_BaseAddress  << dec << ret << "\n";  
       exit(0);
    } else {
      std::cout << "digitizer open, handle = " << m_Handle << std::endl;	 
      ret = CAEN_DGTZ_GetInfo(m_Handle, &BoardInfo);                               
      CheckError(ret);
      if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] GetInfo, error code = " << ret << "\n"; return; }
 
      m_NrChannels = BoardInfo.Channels;
      if(m_NrChannels < 32){
         m_NrGroups = m_NrChannels;
         m_NrChannels = 8*m_NrGroups;
      } else{
         m_NrGroups   = BoardInfo.Channels / 8;
      }

      gEquippedGroups = m_NrGroups;
   
      m_Name	+= string(".") + to_string(BoardInfo.SerialNumber); 
      std::cout << "name: " << m_Name << " BoardInfo.Channels: " << BoardInfo.Channels << std::endl;	    
      std::cout << "name: " << m_Name << " NrGroups: " << m_NrGroups << " NrChannels: " << m_NrChannels << std::endl;	    

      std::cout << "\t\t- construction CAEN 1740 done!\n";
    }
 }
}
//-----------------------------------------------------------------------------
 DCAEN1740::~DCAEN1740() {
  std::cout<<"\t+ module " << m_Name << "\n";
if( VME_CRATE ){
  if(m_Handle){
     ret = CAEN_DGTZ_CloseDigitizer(m_Handle);
     CheckError(ret);
  }
  else{ 
     m_Handle = -1;
  }
}

 } 
//-----------------------------------------------------------------------------
 void DCAEN1740::ConfigureModule() {

 }
//-----------------------------------------------------------------------------
// configuration of the module DT5740 and DT5740D (V1740/V1740D) are different
// Some of the settings are the same but
// I decided setup the digitizer in (if else) depends on the model.

void DCAEN1740::InitModule() {
  if( !VME_CRATE ){
    return;
  }

 ret = CAEN_DGTZ_Reset(m_Handle);                                              
 CheckError(ret);
 //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] Reset, error code = " << ret << "\n"; return; }

    // setting general
    ret = CAEN_DGTZ_SetRecordLength(m_Handle, m_RecordLength);                             
    //CheckError(ret);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetRecordLength " << CheckError(ret) ;

    ret = CAEN_DGTZ_SetDecimationFactor(m_Handle, m_DecimationFactor);
    CheckError(ret);
    //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] DecimationFactor, error code = " << ret << "\n"; return; }

    ret = CAEN_DGTZ_SetPostTriggerSize(m_Handle, m_PostTrigger);
    CheckError(ret);
    //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetPostTriggerSize, error code = " << ret << "\n"; return; }

    ret = CAEN_DGTZ_SetIOLevel(m_Handle, static_cast<CAEN_DGTZ_IOLevel_t>(m_FPIOtype) );
    CheckError(ret);
    //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetIOLevel, error code = " << ret << "\n"; return; }

    ret = CAEN_DGTZ_SetMaxNumEventsBLT(m_Handle, m_MaxEventsBLT);                            
    CheckError(ret);
    //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetMaxNumEventsBLT, error code = " << ret << "\n"; return; }

    ret = CAEN_DGTZ_SetAcquisitionMode(m_Handle, static_cast<CAEN_DGTZ_AcqMode_t>(m_AcqMode) );     
    CheckError(ret);
    //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetAcquisitionMode, error code = " << ret << "\n"; return; }

    //================================= channels setting ================================================
    for(UInt_t i = 0; i < m_NrGroups; i++){
        ret = CAEN_DGTZ_SetGroupDCOffset(m_Handle, i, m_DCoffset[i]);
        if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetGroupDCoffset " << CheckError(ret) << "\n"; return; }
    }

    UInt_t mask =0;
    for(UInt_t i = 0; i < m_NrGroups; i++){
        mask |= ((m_GroupEnableMask[i] & 0x1) << i);
        //std::cout << i << " mask: " << mask << endl;  
    }

    ret = CAEN_DGTZ_SetGroupEnableMask(m_Handle, mask);
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetGroupEnableMask " << CheckError(ret) << "\n"; return; }

    // here I implemented channel pulse polarity per group although it is per channel, you can change it manualy
    for(UInt_t i = 0; i < m_NrGroups; i++){
        ret = CAEN_DGTZ_SetChannelPulsePolarity(m_Handle, i, static_cast<CAEN_DGTZ_PulsePolarity_t>(m_ChannelPulsPolarity[i]) );
        if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetChannelPulsePolarity " << CheckError(ret) << "\n"; return; }
    }
    //================================ trigger setting ====================================================
    ret = CAEN_DGTZ_SetSWTriggerMode(m_Handle, static_cast<CAEN_DGTZ_TriggerMode_t>(m_SWTriggerMode) );      
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetExtTriggerMode " << CheckError(ret) << m_SWTriggerMode << "\n"; return; }

    ret = CAEN_DGTZ_SetExtTriggerInputMode(m_Handle, static_cast<CAEN_DGTZ_TriggerMode_t>(m_ExtTriggerMode) );
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetExtTriggerMode " << CheckError(ret) << m_ExtTriggerMode << "\n"; return; }

    for(UInt_t i = 0; i < m_NrGroups; i++){
        ret = CAEN_DGTZ_SetGroupSelfTrigger(m_Handle, static_cast<CAEN_DGTZ_TriggerMode_t>(m_ChannelTriggerMode[i]), i);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetGroupSelfTrigger " << CheckError(ret) << m_ChannelTriggerMode[i] << std::endl;
    }

    mask = 0;
    for(UInt_t i = 0; i < m_NrGroups; i++){
        for(UInt_t j = 0; j< 8; j++){
	    mask |= ((m_SelfTriggerMask[8*i+j] & 0x1) << i);     
            //std::cout << i << " " << j << " mask: " << mask << endl;  
        }	
    ret = CAEN_DGTZ_SetChannelGroupMask(m_Handle, i, mask );
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetChannelGroupMask " << CheckError(ret) << mask << std::endl;
    }

    for(UInt_t i = 0; i < m_NrGroups; i++){
        ret = CAEN_DGTZ_SetGroupTriggerThreshold(m_Handle, i, m_Threshold[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetGroupTriggerThreshold " << CheckError(ret) << m_Threshold[i] << std::endl;
    }

    for(UInt_t i = 0; i < m_NrGroups; i++){
       ret = CAEN_DGTZ_SetTriggerPolarity(m_Handle, i, static_cast<CAEN_DGTZ_TriggerPolarity_t>(m_TriggerEdge[i]));
       CheckError(ret);
    }
    //--------------------------------------------------------------------------------------------------------


 // synchronization setting
 switch (m_RunSyncMode) {
	case CAEN_DGTZ_RUN_SYNC_Disabled:
        break;

	case CAEN_DGTZ_RUN_SYNC_GpioGpioDaisyChain:
        break;

        case CAEN_DGTZ_RUN_SYNC_TrgOutTrgInDaisyChain:
	    if(m_Handle == 0){	
		ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x817C, 1);   // external trigger inhibit
                CheckError(ret);
              }

	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x810C, 0xC0000000);  // accept EXT TRGIN or SW trg 
     	    CheckError(ret);
	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8110, 0xC0000000);  // propagate both EXT and SW TRG to TRGOUT
            CheckError(ret);
	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8100, 0xE);         // Run starts with 1st trigger edge
            CheckError(ret);
	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8170, m_Delay);     // Run Delay decreases with the position (to compensate for run the propagation delay)
            CheckError(ret);

	break;
	
	case CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain:
	    if(m_Handle > 0){
	       ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8100, 0xD);      // Run starts with 1st trigger edge
               CheckError(ret);
	    }

	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x810C, 0x40000000);  // accept EXT TRGIN
     	    CheckError(ret);
	    
 	    /*HERE MUST BE CHANGED!!!!	to set correct group mask*/    
	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x10A8, 0xF);         // group trigger mask
     	    CheckError(ret);
	     	   
	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8110, 0x0);         // out trigger mask
     	    CheckError(ret);
	     	   
	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8170, m_Delay);     // Run Delay decreases with the position (to compensate for run the propagation delay)
            CheckError(ret);

	    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x811C, 0xFFF0FFFF | 0x00010000); // set bit sIN to trigger out
     	    CheckError(ret);
	     	   

         break;

	case CAEN_DGTZ_RUN_SYNC_SinFanout:
	break;
             
	default:
	break;
     }

 std::cout << "Initialization " << m_Name << " module done!\n";

 }
//-----------------------------------------------------------------------------
 void DCAEN1740::ReadVME() {

  ret = CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_localBuffer, &m_Size);
  CheckError(ret);
  //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] ReadData, error code = " << ret << "\n"; return; }

  if (m_Size == 0) return; // no data collected

  int a_ret = CAEN_DGTZ_GetNumEvents(m_Handle, m_localBuffer, m_Size, &m_EventsInBuffer);
  if(a_ret != 0)
     std::cerr << "[ERROR] in GetNumEvents " << std::endl;

  m_Events += m_EventsInBuffer;
  m_dataSizeByte += m_Size;

}
//=============================================================================
void DCAEN1740::GnuplotOnline(Gnuplot &gp){

  // this function could possibly goes to separate thread...

  if( m_Size == 0) return; // no data to display, return
  UInt_t aEvent = 0;		     // otherwise we draw only the first event using gnuplot  

  //if you want to change the graphs on gnuplot, be carefull it is quite tricky.... every space, comma, new line matters...
  //read first the gnuplot documentation

  gp << "set xrange  [" << 0 << ":" << m_RecordLength << "]\n";
  gp << "set xtics nomirror tc lt 0\n";
  gp << "set x2range ["<< 0 << ":" << 16*m_RecordLength << "]\n";
  gp << "set x2tics nomirror tc lt 0\n";
  gp << "set yrange  [0:4300]\n";
  gp << "set ytics nomirror tc lt 0\n";
  gp << "set y2range [-2.2:2.2]\n";
  gp << "set y2tics nomirror tc lt 0\n";
  gp << "set grid ytics lt 0 lw 1 lc rgb \"#880000\"\n";
  gp << "set grid xtics lt 0 lw 1 lc rgb \"#880000\"\n";

  std::string gp_command;
  Char_t	*a_EventPtr;

  ret = CAEN_DGTZ_GetEventInfo(m_Handle, m_localBuffer, m_Size, aEvent, &EventInfo, &a_EventPtr);
  CheckError(ret);
  ret = CAEN_DGTZ_DecodeEvent(m_Handle, a_EventPtr, (void**)&Event16);
  CheckError(ret);

  gp_command = "plot ";

  for(UInt_t j = 0; j < m_NrChannels; j++){
     if( ((m_SaveChannel >> j) & 1) && ((EventInfo.ChannelMask >> j/8 )& 1)) {
	//std::cout << "SaveChannel: " << ((m_SaveChannel >> j) & 1) << " EventInfoChannelMask: " << ((EventInfo.ChannelMask >> j/8 )& 1) << std::endl;     
        char filename1[256];
	sprintf(filename1,"temp/db%d.bin",j);
        std::ofstream gplot1(filename1, std::ofstream::out | std::ofstream::binary | std::ofstream::ate);
        if(!gplot1.is_open()) {
           std::cout << "[ ERROR ] could not open the file... " << filename1 << std::endl;
           }
        else {
           gplot1.write((char*)Event16->DataChannel[j], Event16->ChSize[j]*sizeof(Event16->ChSize[j])); 
           gplot1.close();
	   gp_command += string("'") + string(filename1) + string("'");
	   gp_command += string(" binary record=") + to_string(Event16->ChSize[j]);
	   gp_command += string(" format='%uint16'");
	   gp_command += string(" u 1 w l");
	   gp_command += string(" title 'ch.") + to_string(j) + string("', ");
           }
	}
  }

 // plot thresholds
  
  for(UInt_t j = 0; j < m_NrGroups; j++){
     if(m_ChannelTriggerMode[j]) {
        char filename1[256];
	sprintf(filename1,"temp/th%d.bin",j);
        std::ofstream gplot1(filename1, std::ofstream::out | std::ofstream::binary | std::ofstream::ate);
        if(!gplot1.is_open()) {
           std::cout << "[ ERROR ] could not open the file... " << filename1 << std::endl;
           }
        else {
           uint16_t thr[Event16->ChSize[j]];		 
           for(uint16_t i = 0; i < Event16->ChSize[j]; i++)
              thr[i] = m_Threshold[j];		   
           gplot1.write((char*)thr, Event16->ChSize[j]*sizeof(Event16->ChSize[j])); 
           gplot1.close();
	   gp_command += string("'") + string(filename1) + string("'");
	   gp_command += string(" binary record=") + to_string(Event16->ChSize[j]);
	   gp_command += string(" format='%uint16'");
	   gp_command += string(" u 1 w l");
	   gp_command += string(" notitle, ");
           }
      
     } 

  }

    //cout << gp_command << endl;
    gp << gp_command << endl;

}
//-----------------------------------------------------------------------------
void DCAEN1740::StartAcq(){

	std::cout << "[DEBUG] DCAEN1740::StartAcq\n";       	
 ShowSettings();
 InitModule(); 

 RegisterDump();

 ret = CAEN_DGTZ_AllocateEvent(m_Handle, (void**)&Event16);
 CheckError(ret);
 ret = CAEN_DGTZ_MallocReadoutBuffer(m_Handle, &m_localBuffer, &m_Size);
 CheckError(ret);

 ret = CAEN_DGTZ_SWStartAcquisition(m_Handle);
 CheckError(ret);
 //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SWStartAcquisitionMode, error code = " << ret << "\n"; return; }

}

//-----------------------------------------------------------------------------
void DCAEN1740::StopAcq(){

  ret = CAEN_DGTZ_SWStopAcquisition(m_Handle);
  CheckError(ret);
  //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SWStopAcquisition, error code = " << ret << "\n"; return; }

  ret = CAEN_DGTZ_FreeReadoutBuffer(&m_localBuffer);
  CheckError(ret);
  //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] FreeReadoutBuffer, error code = " << ret << "\n"; return; }

  ret = CAEN_DGTZ_FreeEvent(m_Handle, (void**)&Event16);
  CheckError(ret);
  //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] FreeEvent, error code = " << ret << "\n"; return; }

  ret = CAEN_DGTZ_ClearData(m_Handle);
  CheckError(ret);
  //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] ClearData, error code = " << ret << "\n"; return; }

 // stop the module	
}

//-----------------------------------------------------------------------------
 void DCAEN1740::Log(std::ofstream & logfile) {

  logfile << "\t\t\t ----- CAEN 1740::Settings" << std::endl;
  logfile << "\t\t\t     + m_xxxxxxxxxxxx "<< std::endl;
  logfile << "\t\t\t     + chn:   xxxxxxx "<< std::endl;
//  for(Int_t i = 0; i < 16; i++){
//    logfile << "\t\t\t     + ch["<< i    << "] " << m_ThresholdOn[i]    << ": " << m_ThresholdValue[i] 
//            << "\t["  << i+16 << "] " << m_ThresholdOn[i+16] << ": " << m_ThresholdValue[i+16] << std::endl;
//    }
  logfile << "\t\t\t     + m_xxxxxxxxxxxxxx " << std::endl;

 }

//======================================================================================================

void DCAEN1740::BuildEvent(){

 }

//======================================================================================================

 void DCAEN1740::SaveEvent(){

 }
//======================================================================================================

 void DCAEN1740::ShowEvent(){


 }

//======================================================================================================

 void DCAEN1740::SendEvent(){

 }

//======================================================================================================
std::string DCAEN1740::CheckError(CAEN_DGTZ_ErrorCode err){
 
   std::string s1;

   switch (err) {
	case 0:
	break;
	case -1:  s1 = " Communication error "; 		break;
	case -2:  s1 = " Unspecified error " ; 			break;
	case -3:  s1 = " Invalid parameter " ; 			break;
	case -4:  s1 = " Invalid Link Type "; 			break;
	case -5:  s1 = " Invalid device handle ";		break;
	case -6:  s1 = " Maximum number of devices exceeded ";	break;
	case -8:  s1 = " The interrupt level is not allowed ";	break;
	case -9:  s1 = " The event number is bad "; 		break;
	case -10: s1 = " Unable to read the registry ";		break;
	case -11: s1 = " Unable to write into the registry ";	break;
	case -13: s1 = " The channel number is invalid ";	break;
	case -14: s1 = " The Channel is busy "; 		break;
	case -15: s1 = " Invalid FPIO Mode "; 			break;
	case -16: s1 = " Wrong acquisition mode " ; 		break;
	case -18: s1 = " Communication Timeout " ; 		break;
	case -19: s1 = " The buffer is invalid " ; 		break;
	case -20: s1 = " The event is not found " ; 		break;
	case -21: s1 = " The vent is invalid " ; 		break;
	case -22: s1 = " Out of memory " ; 			break;
	case -23: s1 = " Unable to calibrate the board " ; 	break;
	case -24: s1 = " Unable to open the digitizer " ; 	break;
	case -25: s1 = " The Digitizer is already open " ; 	break;
	case -30: s1 = " Invalid Firmware License " ; 		break;
	case -99: s1 = " The function is not yet implemented "; break;
	case -26: s1 = " The Digitizer is not ready to operate ";		break;
	case -28: s1 = " The digitizer flash memory is corrupted ";		break;
	case -27: s1 = " The Digitizer has not the IRQ configured "; 		break;
	case -17: s1 = " This function is not allowed for this module "; 	break;
	case -7:  s1 = " The operation is not allowed on this type of board "; 	break;
	case -29: s1 = " The digitizer dpp firmware is not supported in this lib version "; 	break;
	case -31: s1 = " The digitizer is found in a corrupted status "; 			break;
	case -32: s1 = " The given trace is not supported by the digitizer " ; 			break;
	case -33: s1 = " The given probe is not supported for the given digitizer's trace "; 	break;
        default:  s1 = " Error unknown, check the library version, this was created for 2.7.5 ";break;	
   }	   
  
 return s1;
 }
//======================================================================================================
 void DCAEN1740::ShowData(DGDisplay *fDisplay, DAcquisition *fAcquisition) {


  static UInt_t Nb, Ne, prevNe, prevNb;

  fAcquisition->m_AcqStatusEntry2 = m_Events;
  Nb += GetDataSize();
  Ne  = GetNrEvents();

  if (!Nb)
     std::cout << "\t+ No data...\n";
  else{
     std::cout << "\t+ Reading: " << setw(5) << (float)(Nb-prevNb) / ((float)fAcquisition->m_ElapsedTimeMS*1.048576f) << " MB/s, " 
               << "Trg Rate: " << (float)(Ne-prevNe) / (float)fAcquisition->m_ElapsedTimeMS << "kHz, " 
	       << "Total events:" << m_Events <<  std::endl; 
  }

  prevNe = Ne;
  prevNb = Nb;
  

  bool DEBUG_SHOW_DATA = 0;

  if(DEBUG_SHOW_DATA) printf("--------\n");
  
  // fill histograms only in GUI version when fDisplay is not NULL
  if(fDisplay)
   { 
   
   }

}

//-----------------------------------------------------------------------------
void DCAEN1740::DataSave(DAcquisition *fAcquisition){
    return;
   // write data to buffer only when the IRQ happend, otherwise return 
   // CHECK IF THIS IS REALLY NECESSARY, there is m_dataSizeByte which shoud be 0 if there is no IRQ
   // If you want to simulate events you have to comment this line, m_IRQ = 1;
   //std::cout << "[DEBUG] 1 DCAEN1740::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
   if( !VME_CRATE ) m_IRQ = 1; 
   if( fAcquisition->m_EmptyBuffer ) m_IRQ = 1; 
   //std::cout << "[DEBUG] 2 DCAEN1740::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
   if(!m_IRQ) return;
      m_IRQ = 0;
   //std::cout << "[DEBUG] 3 DCAEN1740::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;

   Bool_t m_saveAfterSize = kFALSE; // variable to check if the buffer size reach the max size 
   Bool_t m_saveAfterTime = kFALSE; // variable to check if the time elapsed to save file every sec/min/hour

   static UInt_t m_fileEvents = 0; // temporary value to keep the nr of events per file needed for logfile

   gCurrentTimeADC = gGetLongTimeADC() / 1000;              // check the current time
   gElapsedTimeADC = gCurrentTimeADC - gPrevRateTimeADC;    // calcullate elapsed time

   //memcpy((void*)m_Buffer + m_BufferPos, (void*)m_localBuffer, m_dataSizeByte); // copy data from local buffor to main beffor
   memcpy(m_Buffer + m_BufferPos, m_localBuffer, m_dataSizeByte); // copy data from local buffor to main beffor
   m_BufferPos += (m_dataSizeByte);                                 // move the position of the buffer

   // ==========================================================================
   // check if the buffer is full and has to be writen to file
   if( m_BufferPos > UInt_t(fAcquisition->m_SaveFileSizeEntry*1000*1000) ) {
       m_saveAfterSize = kTRUE;
       }

  // ===========================================================================
  // check the elapsed time
  // cout << "time condition" << gElapsedTimeADC << " " <<  (ULong_t)fAcquisition->m_SaveFileTimeEntry << endl;
   if( gElapsedTimeADC > (ULong_t)fAcquisition->m_SaveFileTimeEntry){
       if( gPrevRateTimeADC == 0) {
           m_saveAfterTime = kFALSE;
           gPrevRateTimeADC = gCurrentTimeADC;
           }
       else { 
           m_saveAfterTime = kTRUE;  
          }
     }

 
  // =============================================================================
  // writing bufer to file if one of the three conditions are fulfill
  if( m_saveAfterSize || m_saveAfterTime || fAcquisition->m_EmptyBuffer){
    std::cout << "[DEBUG] 4 DCAEN1740::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
    char nr[256];
    sprintf(nr, "%03d", fAcquisition->m_NrOfSavedFiles);

    //std::time_t t = std::time(NULL);
    //char mbstr[100];
    //if (std::strftime(mbstr, sizeof(mbstr), "%Y_%m_%d_%H%M_", std::localtime(&t))) {
    //    std::cout << mbstr << '\n';
   // }

    string filename = fAcquisition->m_DataPath + string("/") + fAcquisition->GetFileTime() 
	   + string("_") + fAcquisition->GetFileName() + string("_") + string(nr) + string(".bin");    

    // take the time now
    fAcquisition->m_TimeNow = std::time(NULL);
    std::string s1 = string(ctime(&fAcquisition->m_TimeNow));

    std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::binary);
    if(!DataFile.is_open()) {
       std::cout << "[ ERROR ] could not open the file... " << filename << std::endl;
       *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- could not open the file..." << filename<< "\n"; 
       }
    else {
       DataFile.write((char*)m_Buffer, m_BufferPos); 
       DataFile.close();
       *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- " << "file [" << filename<< "] has been saved\n"; 
       *fAcquisition->fLog << "\t\t\t\tAcq time: " << fAcquisition->m_TimeNow - fAcquisition->m_StartAcqTime << "s, " 
                         << "events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
       std::cout << "[MESSAGE] data file [" << filename << "] has been saved " << std::endl;
       std::cout << "          Nr Events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
      }

 // ===============================================================================
 // some cleaning after saving and writing message to the std output
    m_BufferPos = 0;
    m_fileEvents = m_Events;
    fAcquisition->m_NrOfSavedFiles++;
    gPrevRateTimeADC = gCurrentTimeADC;

    }

}

//-----------------------------------------------------------------------------
 void DCAEN1740::ResetModule() {

   CAEN_DGTZ_Reset(m_Handle);

}
//*****************************************************************************
void DCAEN1740::ShowSettings() {

 std::cout << "[MESSAGE] ======= ::" << m_Name << "::ShowSettings =======" << std::endl;
 std::cout << "\t| m_Name\t\t= "          << m_Name                      << std::endl  ;
 std::cout << "\t| m_Handle\t\t= "        << m_Handle                    << std::endl  ;
 std::cout << "\t| m_Module ID\t\t= "     << hex << m_ModuleID    << dec << std::endl;
 std::cout << "\t| m_BaseAddress\t\t= 0x" << hex << m_BaseAddress << dec << std::endl;
 std::cout << "\t| m_AcqMode\t\t= "       << acqmode(m_AcqMode)          << std::endl;
 std::cout << "\t| m_MaxEventsBLT\t= "    << m_MaxEventsBLT              << std::endl;
 std::cout << "\t+ =============================================="<< std::endl;
 std::cout << "\t+- Samples ------------ Group - Offset - Polarity - Save --+"<< std::endl;
 std::cout << "\t| m_RecodrLength = "  << m_RecordLength << "\t" << m_GroupEnableMask[0] << "\t" << m_DCoffset[0] 
	   << "\t " << polarity(m_ChannelPulsPolarity[0]) << "\t   " << Dec2BinStr(m_SaveChannel) << std::endl;
 std::cout << "\t| m_PostTrg\t = " << m_PostTrigger  << "\t" << m_GroupEnableMask[1] << "\t" << m_DCoffset[1] 
	   << "\t " << polarity(m_ChannelPulsPolarity[1]) << "\t   " << Dec2BinStr(Char_t(m_SaveChannel >> 8)) << std::endl;

 for(UInt_t i = 2; i < m_NrGroups; i++){
     cout << "\t| \t\t\t" << m_GroupEnableMask[i] << "\t" << m_DCoffset[i] << "\t "
          << polarity(m_ChannelPulsPolarity[i]) << "\t   " << Dec2BinStr(Char_t(m_SaveChannel >> i*8)) << std::endl;
 }
					      
					 
 std::cout << "\t+ =============================================="<< std::endl;
 std::cout << "\t+----------- Mode - Threshold - Edge -- SelfTrigerMask "<< endl;

 for(UInt_t i = 0; i < m_NrGroups; i++){
    std::cout << "\t| TgGr." << i << " " << mode(m_ChannelTriggerMode[i]) <<"\t"<< m_Threshold[i] <<"\t"<< edge(m_TriggerEdge[i]) <<"\t";
	for(UInt_t j = 0; j < 8; j++){    
		std::cout <<  m_SelfTriggerMask[8*i+j] ; 
	}
    std::cout << std::endl;
 }

 std::cout << "\t| Tg.Ext " << mode(m_ExtTriggerMode) << iolevel(m_FPIOtype)  << std::endl;
 std::cout << "\t| GPIO\t\t" << iolevel(m_FPIOtype)  << std::endl;
 std::cout << "\t+ =============================================="<< std::endl;

}

//==================================================================================
void DCAEN1740::SaveConfig(std::ofstream & fout){

 fout << "[" << m_Name << "]" << endl;
 fout << "# ======= Channel input signal settings ==========" << endl; 
 fout << "RecordLength "        << m_RecordLength        <<endl;
 fout << "# you can choose how many samples are stored before and after triggeer appear,  the value are between 0, 100 in %" << endl;
 fout << "# in case of DPP-QDC PreTrigger must be > Pregate + 7"<< endl;
 fout << "# Offset for group channels " << endl;
 fout << "# The values are in ADC convertion numbers, to see coresponding mV value run acq, look at the right yaxis and addapt accordingly" << endl;
 fout << "# 32768 is a middle, there is no offset" << endl;
 fout << "# syntax: DCoffset <group> <value>" << endl;
 for(unsigned i = 0; i < m_NrGroups; i++)
    fout << "DCoffset " << i << " " << m_DCoffset[i]  << endl;
 fout << endl;
 fout << "PostTrigger " << m_PostTrigger         <<endl;
 fout << endl;
 fout << "# GroupEnableMask: disable = 0 or enable = 1 group channels" << endl;
 fout << "# syntax: GroupEnableMask <group> <value>" << endl;
 for(unsigned i = 0; i < m_NrGroups; i++)
    fout << "GroupEnableMask " << i << " " << m_GroupEnableMask[i]  << endl;
 fout << endl;
 fout << "# Set the pulse polarity, here is per group but you can do it manualy for every channel " << endl;
 fout << "#  decided that for group is enough, The value can be 0 for positive signals and 1 for negative" << endl;
 fout << "# need intervention in the source code..." << endl;
 for(unsigned i = 0; i < m_NrGroups; i++)
    fout << "ChannelPulsPolarity " << i << " " << m_ChannelPulsPolarity[i]  << endl;

 fout << endl;
 fout << "# The save option allows you to save certain channels. The digitizer is designed in such that even only one channel is used"<< endl;
 fout << "# all group must be transfered to PC. Then you can choose by software which channel you want to store and/or display."<< endl;
 fout << "# It is one 32/64 bits word, Each bit correspond to each channel"<< endl;
 fout << "SaveChannels " <<  m_SaveChannel        << endl;
 fout << endl;

 fout << "# ====== Trigger Configuration ==========" << endl;
 fout << "# Enable or disable software, external and channel trigger" << endl;
 fout << "# the options could be 0, 1, 2, 3, disable, enable acq only, enable ext only, both, respectevely "<< endl;
 fout << "# syntax: ChannelTriggerMode <group> <value> "<< endl;
 for(unsigned i = 0; i < m_NrGroups; i++)
    fout << "ChannelTriggerMode " << i << " " << m_ChannelTriggerMode[i]  << endl;
 fout << endl;
 fout << "# " << endl;
 fout << "ExtTriggerMode "<< m_ExtTriggerMode       <<endl;
 fout << "# " << endl;
 fout << "# Adjust the threshold to the coresponding channel" << endl;
 fout << "# Treshold value is an absolut value, consider it when you add offset to the channel" << endl;
 for(unsigned i = 0; i < m_NrGroups; i++)
    fout << "Threshold " << i <<" " <<  m_Threshold[i]  << endl;
 fout << endl;
 fout << "# Trigger slope for the every group of channels, could be rising (0) or falling (1)" << endl;
 for(unsigned i = 0; i < m_NrGroups; i++)
    fout << "TriggerEdge " << i << " " << m_TriggerEdge[i]  << endl;
 fout << endl;

 fout << "# Global trigger can be built from logic OR all indyvidual channel triggers. This option allows you to set which indyvidual "<< endl;
 fout << "# trigger will contribute to build the global one. This is set by indyvidual group. Every group consist of 8 channels" << endl;
 fout << "SelfTriggerMaskGr0 " << m_SelfTriggerMask[0]  << m_SelfTriggerMask[1]  << m_SelfTriggerMask[2]  << m_SelfTriggerMask[3]
                               << m_SelfTriggerMask[4]  << m_SelfTriggerMask[5]  << m_SelfTriggerMask[6]  << m_SelfTriggerMask[7]  << endl;
 fout << "SelfTriggerMaskGr1 " << m_SelfTriggerMask[8]  << m_SelfTriggerMask[9]  << m_SelfTriggerMask[10] << m_SelfTriggerMask[11]
	                       << m_SelfTriggerMask[12] << m_SelfTriggerMask[13] << m_SelfTriggerMask[14] << m_SelfTriggerMask[15] << endl;
 fout << "SelfTriggerMaskGr2 " << m_SelfTriggerMask[16] << m_SelfTriggerMask[17] << m_SelfTriggerMask[18] << m_SelfTriggerMask[19]
	                       << m_SelfTriggerMask[20] << m_SelfTriggerMask[21] << m_SelfTriggerMask[22] << m_SelfTriggerMask[23] << endl;
 fout << "SelfTriggerMaskGr3 " << m_SelfTriggerMask[24] << m_SelfTriggerMask[25] << m_SelfTriggerMask[26] << m_SelfTriggerMask[27]
	                       << m_SelfTriggerMask[28] << m_SelfTriggerMask[29] << m_SelfTriggerMask[30] << m_SelfTriggerMask[31] << endl;
 fout << "SelfTriggerMaskGr4 " << m_SelfTriggerMask[32] << m_SelfTriggerMask[33] << m_SelfTriggerMask[34] << m_SelfTriggerMask[35]
	                       << m_SelfTriggerMask[36] << m_SelfTriggerMask[37] << m_SelfTriggerMask[38] << m_SelfTriggerMask[39] << endl;
 fout << "SelfTriggerMaskGr5 " << m_SelfTriggerMask[40] << m_SelfTriggerMask[41] << m_SelfTriggerMask[42] << m_SelfTriggerMask[43]
	                       << m_SelfTriggerMask[44] << m_SelfTriggerMask[45] << m_SelfTriggerMask[46] << m_SelfTriggerMask[47] << endl;
 fout << "SelfTriggerMaskGr6 " << m_SelfTriggerMask[48] << m_SelfTriggerMask[49] << m_SelfTriggerMask[50] << m_SelfTriggerMask[51]
	                       << m_SelfTriggerMask[52] << m_SelfTriggerMask[53] << m_SelfTriggerMask[54] << m_SelfTriggerMask[55] << endl;
 fout << "SelfTriggerMaskGr7 " << m_SelfTriggerMask[56] << m_SelfTriggerMask[57] << m_SelfTriggerMask[58] << m_SelfTriggerMask[59]
	                       << m_SelfTriggerMask[60] << m_SelfTriggerMask[61] << m_SelfTriggerMask[62] << m_SelfTriggerMask[63] << endl;
 fout << "# " << endl;

 fout << "# All digitial inputs on the fron panel can be set as TTL = 1 or NIM = 0" << endl;
 fout << "IOLevel "             << m_FPIOtype          << endl;
 fout << "# " << endl;

 fout << "# Acquisition Mode could be software controled = 0; or by signal from the front panel = 1" << endl;
 fout << "AcquisitionMode "     << m_AcqMode           << endl;

 fout << endl;

}


//==================================================================================
void DCAEN1740::LoadConfig(std::ifstream & inpfile){
 
  cout << "[MESSAGE] Load configuration file CAEN1740......" << endl;  
 
  string line;
  string name;
  string value;
  string inSection;
  int posEqual;
  int lineNr = 0;
  while (getline(inpfile, line)) {
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
	 vector<string> param;
	 string buf;
	 if( inSection == string(m_Name) ) {
	     if     ( name == string("RecordLength")	) m_RecordLength	= atoi( value.c_str() );
	     else if( name == string("PostTrigger")	) m_PostTrigger		= atoi( value.c_str() );
	     else if( name == string("GroupEnableMask")	) {
	        stringstream ss(value);
		while (ss >> buf)  param.push_back(buf);
		m_GroupEnableMask[ stoi(param.at(0)) ] = stoi( param.at(1) );
	     }
	     else if( name == string("DCoffset")	) {
	        stringstream ss(value);
		while (ss >> buf) param.push_back(buf);
		m_DCoffset[ stoi(param.at(0)) ] = stoi( param.at(1) );
		}
	     else if( name == string("ChannelPulsPolarity")){
	        stringstream ss(value);
		while (ss >> buf) param.push_back(buf);
		m_ChannelPulsPolarity[ stoi(param.at(0)) ] = stoi( param.at(1) );
		}
	     else if( name == string("SaveChannels")	      ) m_SaveChannel 	   	 = atoi( value.c_str()   );
	     else if( name == string("ExtTriggerMode") 	      ) m_ExtTriggerMode         = atoi( value.c_str()   );
	     else if( name == string("ChannelTriggerMode") ){
	        stringstream ss(value);
		while (ss >> buf) param.push_back(buf);
		m_ChannelTriggerMode[ stoi(param.at(0)) ] = stoi( param.at(1) );
	     }
	     else if( name == string("Threshold")	) {
	        stringstream ss(value);
		while (ss >> buf) param.push_back(buf);
		m_Threshold[ stoi(param.at(0)) ] = stoi( param.at(1) );
	     }
	     else if( name == string("TriggerEdge")  ) {
	        stringstream ss(value);
		while (ss >> buf) param.push_back(buf);
		m_TriggerEdge[ stoi(param.at(0)) ] = stoi( param.at(1) );
	     }
             else if( name == string("SelfTriggerMaskGr0")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i] = 1; 
			     else m_SelfTriggerMask[i] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr1")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+8] = 1; 
			     else m_SelfTriggerMask[i+8] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr2")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+16] = 1; 
			     else m_SelfTriggerMask[i+16] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr3")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+24] = 1; 
			     else m_SelfTriggerMask[i+24] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr4")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+32] = 1; 
			     else m_SelfTriggerMask[i+32] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr5")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+40] = 1; 
			     else m_SelfTriggerMask[i+40] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr6")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+48] = 1; 
			     else m_SelfTriggerMask[i+48] = 0; 
		     } 
	     }
             else if( name == string("SelfTriggerMaskGr7")){ 
		     for(Int_t i = 0; i < 8; i++){ 
			     if(value[i] == '1') m_SelfTriggerMask[i+56] = 1; 
			     else m_SelfTriggerMask[i+56] = 0; 
		     } 
	     }
 	     else if( name == string("IOLevel") 	) 	m_FPIOtype            = atoi( value.c_str() );
             else if( name == string("AcquisitionMode") ) 	m_AcqMode             = atoi( value.c_str() );
             else if( name == string("ClockDelay") 	) 	m_Delay                    = atoi( value.c_str() );
             else if( name == string("ClockSource")	) 	m_Clock                    = atoi( value.c_str() );
             else if( name == string("MaxEventsBLT")	) 	m_MaxEventsBLT             = atoi( value.c_str() );
             else if( name == string("SynchronizationMode")) 	m_RunSyncMode      = static_cast<CAEN_DGTZ_RunSyncMode_t>(atoi( value.c_str() ));
             else cout << "[ERROR] Parameter not recognized, line nr = " << lineNr << "\t[" << inSection << "] " << name << " : " << value << endl;
	 }
  }
}

//==================================================================================
void DCAEN1740::RegisterDump(){

   FILE *fout;
   fout = fopen("register_dump.txt", "w");

   fprintf(fout, "=== configuration settings ===\n");

   unsigned int read, i3, address;
   string readStr;

   fprintf(fout, "RecordLength        = %d\n", m_RecordLength);
   fprintf(fout, "DecimationFactor    = %d\n", m_DecimationFactor );
   fprintf(fout, "PostTrigger         = %d\n", m_PostTrigger);
   fprintf(fout, "FPIOtype            = %d\n", m_FPIOtype);
   fprintf(fout, "Max events transfer = %d\n", m_MaxEventsBLT);
   fprintf(fout, "Acquisition control = %d\n", m_AcqMode);
   fprintf(fout, "ExtTriggerMode      = %d\n", m_ExtTriggerMode);
   UInt_t mask = ((m_GroupEnableMask[3] << 3) | (m_GroupEnableMask[2] << 2) | (m_GroupEnableMask[1] << 1) | m_GroupEnableMask[0] ) & 0xF;
   fprintf(fout, "EnableMask          = %d\n", mask);
   for(i3=0; i3 < m_NrGroups; i3++) {
       if (mask & (1<<i3)) {
           fprintf(fout, "...... Group %d\n", i3);
           fprintf(fout, "DCoffset[%d]          = %d\n", i3, m_DCoffset[i3]);
           fprintf(fout, "ChannelTriggerMode[%d]= %d\n", i3, m_ChannelTriggerMode[i3]);
           fprintf(fout, "Threshold[%d]         = %d\n", i3, m_Threshold[i3]);
           fprintf(fout, "GroupTrgEnableMask[%d]= %d\n", i3, m_SelfTriggerMask[i3]);
           fprintf(fout, "TriggerEdge          = %d\n",      m_TriggerEdge[i3]);
        }
    }
   
   fprintf(fout, "\n=== Register readout ===\n");
   address = 0x1080;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //threshold
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group 0 Threshold                   (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x1088;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //status
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group 0 Status                      (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x1098;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //DAC
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group 0 DAC                         (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x109C;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //ADC configuration
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group 0 ADC Configuration           (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x10A8;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //n channel trigger enable mask
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group 0 Channel Trigger Enable Mask (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8000;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //channel configuration
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group Configuration                 (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x800C;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //Buffer organization
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Buffer Organization                 (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8020;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //custom size
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Custom Size                         (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8044;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //Decimation Factor
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Decimation Factor                   (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8100;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //acq control
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Acquisition Control                 (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8104;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //acq status
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Acquisition Status                  (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x810C;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //trigger source enable mask
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Trigger Source Enable Mask          (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8110;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //front panel trigger out enable mask
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Front Panel Trigger Out Enable Mask (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x8114;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //post trigger
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Post Trigger                        (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );

   address = 0x811C;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //front panel I/O
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Front Panel I/O  		    (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );

   address = 0x8120;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //channel enable mask
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Group Enable Mask                   (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );

   address = 0x8124;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //event stored
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "ROC FPGA Firmware rev.              (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );
   
   address = 0x812C;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //event stored
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Events stored                       (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );

   address = 0x8170;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //event stored
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "Run Start/Stop Delay                (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );

   address = 0x817C;
   ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &read); //event stored
   if(ret) fprintf(fout, "ERROR Read Register %d\n", ret);
   readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF));
   fprintf(fout, "External Trigger Inhibit            (0x%X) \t= 0x%08X = %s\n", address, read, readStr.c_str() );

fclose(fout);

}





