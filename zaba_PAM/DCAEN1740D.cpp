#include "DCAEN1740D.h"
#include "DGDisplay.h"
#include <bitset>
#include <string>
#include <fstream>
#include "sys/time.h"
#include "TRandom.h"
#include "gnuplot-iostream.h"
#include "_CAENDigitizer_DPP-QDC.h"

#define MAX_AGGR_NUM_PER_BLOCK_TRANSFER   1023 /* MAX 1023 */

int pluto = 1;
long InitValue = 99.9;

static ULong_t gCurrentTimeADC = 0;
static ULong_t gElapsedTimeADC = 0;
static ULong_t gPrevRateTimeADC = 0;
//static ULong_t gPrevSizeADC = 0;
extern unsigned int gEquippedGroups;

uint64_t     		gExtendedTimeTag[64];
uint64_t     		gETT[64];
uint64_t     		gPrevTimeTag[64];
#include <stdio.h>
#include <chrono>
#include <thread>
#include "DKeyboard.h"
//===========================================================
static ULong_t gGetLongTimeADC(){
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    
    return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}
//===========================================================
/*
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
 */
static	std::string polarity(Int_t a){
    if(a == 0)      return std::string("POS");
    else if(a == 1) return std::string("NEG");
    else            return std::string("unknown");
}
/*
 static	std::string iolevel(Int_t a){
	if(a == 0)      return std::string("    NIM");
	else if(a == 1) return std::string("    TTL");
	else            return std::string("unknown");
	}
 */
static	std::string acqmode(Int_t a){
    if(a == 0)      return std::string(" SOFT ");
    else if(a == 1) return std::string(" HARD ");
    else            return std::string("unknown");
}

static	std::string acqmodedpp(Int_t a){
    if(a == 0)      return std::string(" MIXED ");
    else if(a == 1) return std::string("  LIST ");
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
/*
 static std::string Dec2BinStr(Char_t dec){
 std::string strBin;
 
 for(UInt_t i = 0x1; i <= 0x80; i <<= 1){
 if(i & dec) strBin += '1';
 else        strBin += '0';
 }
 
 return strBin;
 }
 */
//===========================================================


typedef tuple< uint16_t, uint16_t, uint64_t> singleEvent;
/*
 class Compare{
 public:
	bool operator()(const triple_t& a, const triple_t& b) const{
	if(get<2>(a) > get<2>(b))
	return true;
	else
	return false;
	}
 };
 */
//auto compare = [](const singleEvent& a, const singleEvent& b) { return 0; };
auto compare = [](const singleEvent& a, const singleEvent& b) { return get<2>(a) > get<2>(b); };
typedef std::priority_queue < singleEvent, std::deque< singleEvent >, decltype(compare) > p_queue; // piority queue

extern int  VME_CRATE; //this is for test only, if you want to change the value, change it in main.cpp
using namespace std;

//===========================================================

ClassImp(DCAEN1740D)

//*****************************************************************************
DCAEN1740D::DCAEN1740D(Char_t *mdesc, UInt_t addr) :
DModule((char*)"64 channels digitizer 62.5MHz",(char*)"CAEN v1740D",mdesc,addr) {
    
    /** DA CAPIRE: 
     ** cosa definisce if(VME_CRATE)
     ** gruppi e canali
     ** gEvent
     ** gEventGRP **/
    
    std::cout<<"\t+ module CAEN 1740...\n";
    
    m_BaseAddress		= addr;
    m_ModuleID		= 0x6004;
    m_Firmware		= 1;
    m_Enabled         = 0;
    
    UInt_t LinkNum = 0;
    UInt_t ConetNode = 0;
    
    m_localBuffer		= NULL;
    m_AcqMode		= 0;
    m_Handle		= -1;
    m_Clock		= 0;
    //m_MaxEventsBLT	= 1;
    //m_RecordLength	= 512;
    //m_Range		= 4096;
    //m_Range_mV		= 2000;
    m_NrChannels		= 64;
    m_NrGroups		= 8;
    m_DecimationFactor	= 1;
    //m_PostTrigger		= 50;
    m_SaveChannel		= 7;
    m_SWTriggerMode	= 1;
    m_ExtTriggerMode	= 1;
    //m_FPIOtype		= 0;
    //m_RunSyncMode		= CAEN_DGTZ_RUN_SYNC_Disabled;
    m_RunSyncMode = CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain;  // questa riga lha messa FRAPI
    // dovrebbe prenderla da zabarc  
    m_Delay		= 0;
    //m_IRQ 		= 0;
    
    m_Events		= 0;
    m_dataSizeByte	= 0;
    m_EventsInBuffer	= 0;
    m_BufferPos		= 0;
    /*
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
     */
    
    /** dovrebbe essere la scrittura della matrice SelfTriggerMaskGrx line 106 zabarc**/
    
    for(UInt_t i = 0; i < 64; i++)  m_SelfTriggerMaskDPP[i] = 1;
    
    //m_SelfTriggerMaskDPP[0] = 1; // this is not in the for loop, set element 0 to 1;
    
    m_AcqModeDPP 		= 1;
    m_RecordLengthDPP	= 600;
    
    m_EnableChargePedestalDPP = 0;
    
    for(UInt_t i = 0; i < m_NrGroups; i++){
        m_GroupEnableMaskDPP[i]	= 1;
        m_DCoffsetDPP[i]            = 32768; // this is in the middle, no offset
        m_BaseLineDPP[i] 		= 16;
        m_PreGateDPP[i] 		= 20;
        m_GateWidthDPP[i] 		= 200;
        m_PulsePolarityDPP[i]	= 0;
        m_TriggerHoldOffDPP[i] 	= 0;
        m_PreTriggerDPP[i]		= 50;
        m_ChargeSensitivityDPP[i]	= 5;
        m_TriggerSmoothingDPP[i]	= 0;
    }
    
    m_DisSelfTrigger		= 0;
    m_TestPulsesRate		= 0;
    m_EnTestPulses		= 0;
    m_DisTrigHist		= 0;  // default is 1
    m_TriggerModeDPP		= 0;
    m_FixedBaseLineDPP		= 3;
    m_MaxEventsAggBLT 		= 1;
    m_savingformat		= 0;
    m_Name = string(mdesc);
    
    
    for(UInt_t i = 0; i < m_NrChannels; i++){
        m_ThresholdDPP[i] 		= 100;
    }
    
    /** perchè è un nuovo loop??? **/
    for(UInt_t i = 0; i < m_NrChannels; i++)
        gEvent[i] = NULL;
    
    //** da capire memset(NumEvents, 0, MAX_CHANNELS*sizeof(NumEvents[0])) cosa fa... **/
    memset(NumEvents, 0, MAX_CHANNELS*sizeof(NumEvents[0]));
    

    
    if( VME_CRATE ){
        /** CAEN_DGTZ_OpenDigitizer() qui cambiare per USB,Opt, etc...) **/
       // ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, 0, 0, (uint32_t)0x32100000, &m_Handle);
        ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, LinkNum, ConetNode, (uint32_t)m_BaseAddress, &m_Handle);
        if(ret != CAEN_DGTZ_Success){
            CheckError(ret);
            std::cout << "[ERROR] Open digitizer, error code = " << ret << "\n";
            std::cout << "        LinkNum: " <<  LinkNum << ", ConetNode: " << ConetNode << ", m_BaseAddress: 0x" << hex <<  m_BaseAddress  << dec << ret << "\n";
            exit(0);
        } else {
            ret = CAEN_DGTZ_GetInfo(m_Handle, &BoardInfo);
            CheckError(ret);
            if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] GetInfo, error code = " << ret << "\n"; return; }
            
            /** controllare nGroup e nChannel... non un problema per 1740D **/
            m_NrChannels = BoardInfo.Channels;
            if(m_NrChannels < 32){
                m_NrGroups = m_NrChannels;
                m_NrChannels = 8*m_NrGroups;
            } else{
                m_NrGroups   = BoardInfo.Channels / 8;
            }
            
            /** controllo stupido da cancellare... **/
            if(m_NrChannels == 64) std::cout << "ok, trovati 64 canali " << std::endl;
            if(m_NrGroups == 8) std::cout << "ok, trovati 8 gruppi " << std::endl;
            
            gEquippedGroups = m_NrGroups; // this line needed by external library _CAENDigitizer_DPP-QDC.c
            for(UInt_t i = 0; i < m_NrGroups; i++)
                gEventsGrp[i] = NULL;
            
            m_Name	+= string(".") + to_string(BoardInfo.SerialNumber);
            std::cout << "name: " << m_Name << " BoardInfo.Channels: " << BoardInfo.Channels << std::endl;
            std::cout << "name: " << m_Name << " NrGroups: " << m_NrGroups << " NrChannels: " << m_NrChannels << std::endl;
            
            std::cout << "\t\t- construction CAEN 1740D done!\n";
        }
    }
    
}
//-----------------------------------------------------------------------------
DCAEN1740D::~DCAEN1740D() {
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
void DCAEN1740D::ConfigureModule() {
    std::cout << "questa funzione non fa una ceppa" << std::endl;
    
}
//-----------------------------------------------------------------------------
// configuration of the module DT5740 and DT5740D (V1740/V1740D) are different
// Some of the settings are the same but
// I decided setup the digitizer in (if else) depends on the model.

void DCAEN1740D::InitModule() {
    if( !VME_CRATE ){
        return;
    }
    
/** 1) reset module **/
    ret = CAEN_DGTZ_Reset(m_Handle);
    CheckError(ret);
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] Reset Module, error code = " << ret << "\n"; return; }
    uint32_t mask = 0;
    
/** set trigger mask DPP **/
    for(unsigned i = 0; i < m_NrGroups; i++) {
        mask = 0;
        for(UInt_t j = 0; j< 8; j++){
            mask <<= 1;
            mask |= m_SelfTriggerMaskDPP[8*i+j] & 0x1;
        }
        uint32_t address = 0x10A8 + 0x100*i;
        ret = CAEN_DGTZ_WriteRegister(m_Handle, address, mask);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Set individual trigger for group and channel " << CheckError(ret) << address << " " << mask << std::endl;
    }
    
/** set readout enable mask **/
    for(UInt_t i = 0; i < m_NrGroups; i++){
        mask |= ((m_GroupEnableMaskDPP[i] & 0x1) << i);
        //std::cout << i << " mask: " << mask << endl;
    }
    ret = CAEN_DGTZ_SetGroupEnableMask(m_Handle, mask);
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetGroupEnableMask " << CheckError(ret) << "\n"; return; }
    
    //  Set selfTrigger threshold
    for(unsigned i = 0; i < m_NrChannels; i++){
        uint32_t a_gr = i / 8;
        uint32_t a_ch = i % 8;
        uint32_t address = 0x1000 + 0x100*a_gr + 0xD0 + 4*a_ch;
        ret = CAEN_DGTZ_WriteRegister(m_Handle, address, m_ThresholdDPP[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetChannelTriggerThreshold " << CheckError(ret) << m_ThresholdDPP[i] << std::endl;
    }
    
    /* Disable Group self trigger for the acquisition (mask = 0) */
/** ??? **/
    ret = CAEN_DGTZ_SetGroupSelfTrigger(m_Handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY, 0x00);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetGroupSelfTrigger " << CheckError(ret) << std::endl;
    
    /* Set the behaviour when a SW tirgger arrives */
    ret = CAEN_DGTZ_SetSWTriggerMode(m_Handle, static_cast<CAEN_DGTZ_TriggerMode_t>(m_SWTriggerMode) );
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetSWTriggerMode " << CheckError(ret) << std::endl;
    
    /* Set the max number of events/aggregates to transfer in a sigle readout */
    //ret = CAEN_DGTZ_SetMaxNumAggregatesBLT(m_Handle, MAX_AGGR_NUM_PER_BLOCK_TRANSFER);
    ret = CAEN_DGTZ_SetMaxNumAggregatesBLT(m_Handle, 500);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetMaxNumAggregatesBLT " << CheckError(ret) << std::endl;
    
    /* Set the start/stop acquisition control */
    ret = CAEN_DGTZ_SetAcquisitionMode(m_Handle, CAEN_DGTZ_SW_CONTROLLED);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetAcquisitionMode " << CheckError(ret) << std::endl;
    
    /* Trigger Hold Off */
    for(unsigned i = 0; i < m_NrGroups; i++) {
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8074, m_TriggerHoldOffDPP[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Trigger Hold Off " << CheckError(ret) << std::endl;
    }
    
    /* Pulse polarity */
    /*    for(unsigned i = 0; i < m_NrGroups; i++) {
     uint32_t address = 0x1040 + 0x100*i;
     ret = CAEN_DGTZ_WriteRegister(m_Handle, address, m_PulsePolarityDPP[i]);
     cout << i << " address:" << hex << address << dec << " pulspolarity:" << m_PulsePolarityDPP[i] << endl;
     if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] PulsePolarityDPP " << CheckError(ret) << std::endl;
     }
     */
    
    for(unsigned i = 0; i < m_NrGroups; i++) {
        uint32_t DppCtrl1 = 0;
        DppCtrl1 = ( ((m_ChargeSensitivityDPP[i] & 0x7) <<  0)   |  // charge sensitivity 0x7 = 20.48pC
                    ((m_EnTestPulses            & 0x0) <<  4)   |  // 0 - disable, 1 - enable
                    ((m_TestPulsesRate          & 0x3) <<  5)   |  // 0x3 = 1MHz
                    ((m_EnableChargePedestalDPP & 0x1) <<  8)   |  // if 1 1024 is added to the charge (useful when energy close to zero)
                    ((m_TriggerSmoothingDPP[i]  & 0x0) << 12)   |  // smoothing signal, 0x0 - no smoothing
                    ((m_PulsePolarityDPP[i]     & 0x1) << 16)   |  // 0 - positive, 1 negatize
                    ((m_TriggerModeDPP          & 0x3) << 18)   |  // 0x00 normal self trigger, 0x01 paired mode, read the documentation
                    ((m_FixedBaseLineDPP        & 0x7) << 20)   |  // 0-fixed, 1-4smp, 2-16smps, 3-64samples
                    ((m_DisSelfTrigger          & 0x1) << 24)   |  //
                    ((m_DisTrigHist             & 0x1) << 30)   ); // trigger histeresis, disable
        
        uint32_t address = 0x1040 + 0x100*i;
        ret = CAEN_DGTZ_WriteRegister(m_Handle, address, DppCtrl1);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] DppCtrl1 " << CheckError(ret) << std::endl;
    }
    
    /* Set Pre Trigger (in samples) */
    // this is if you want to set all channels with one value
    //ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x803C, m_PreTriggerDPP[0]);
    //if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR]  Pre Trigger " << CheckError(ret) << std::endl;
    //this is per group
    for(unsigned i = 0; i < m_NrGroups; i++) {
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x103C + 0x100*i, m_PreTriggerDPP[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Gate Width " << CheckError(ret) << std::endl;
    }
    
    /* Set Gate Offset (in samples) */
    // this is if you want to set all channels with one value
    //ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8034, m_PreGateDPP);
    //if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Gate Offset " << CheckError(ret) << std::endl;
    for(unsigned i = 0; i < m_NrGroups; i++) {
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x1034 + 0x100*i, m_PreGateDPP[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Gate Width " << CheckError(ret) << std::endl;
    }
    
    /* Set Baseline (used in fixed baseline mode only) */
    // this is if you want to set all channels with one value
    //ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8038, m_FixedBaseLineDPP);
    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8038, 64);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR]  Baseline " << CheckError(ret) << std::endl;
    
    //for(unsigned i = 0; i < m_NrGroups; i++) {
        //ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x1038 + 0x100*i, m_BaseLineDPP[i]);
        //  if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Gate Width " << CheckError(ret) << std::endl;
    //}
    
    /* Set Gate Width (in samples) */
    for(unsigned i = 0; i < m_NrGroups; i++) {
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x1030 + 0x100*i, m_GateWidthDPP[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Gate Width " << CheckError(ret) << std::endl;
    }
    
    /* Set the waveform lenght (in samples) */
    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8024, m_RecordLengthDPP/8); // i do not know why but must be divided by 8....
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] SetRecordLength " << CheckError(ret) << std::endl;
    
    /* Set DC offset */
    for(unsigned i = 0; i < m_NrGroups; i++){
        ret = CAEN_DGTZ_SetGroupDCOffset(m_Handle, i, m_DCoffsetDPP[i]);
        if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] Set DC offset " << CheckError(ret) << std::endl;
    }
    
    /* enable Charge mode */
    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8004, 0x00080000);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] enable Charge mode " << CheckError(ret) << std::endl;
    
    /* enable Timestamp */
    ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8004, 0x00040000);
    
    /* set Scope mode */
    if ( m_AcqModeDPP == 0)
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8004, 0x00010000);
    else
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8008, 0x00010000);
    if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] m_AcqModeDPP " << CheckError(ret) << std::endl;
    
    /* Set number of events per memory buffer */
    _CAEN_DGTZ_DPP_QDC_SetNumEvAggregate(m_Handle, m_MaxEventsAggBLT); // 0 - automatic
    //_CAEN_DGTZ_DPP_QDC_SetNumEvAggregate(m_Handle, 0); // 0 - automatic
    
    /* enable test pulses on TRGOUT/GPO */
    //if (ENABLE_TEST_PULSE) {
    if (1) {
        uint32_t d32;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, 0x811C, &d32);
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x811C, d32 | (1<<15));
        ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8168, 2);
    }
    
    /* Set Extended Time Stamp if enabled*/
    //if (params->EnableExtendedTimeStamp)
    //ret |= CAEN_DGTZ_WriteRegister(m_Handle, 0x8004, 1 << 17);

/** Set pannel IOLevel **/
    ret = CAEN_DGTZ_SetIOLevel(m_Handle, static_cast<CAEN_DGTZ_IOLevel_t>(m_FPIOtype) );
    CheckError(ret);
    //if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SetIOLevel, error code = " << ret << "\n"; return; }
    
    
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
		cout << "ci sono " << endl;
            if(m_Handle > 0){
                ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8100, 0xD);      // Run starts with 1st trigger edge
                CheckError(ret);
   		 if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain 1 " << CheckError(ret) << std::endl;
            }
            
            ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x810C, 0x40000000);  // accept EXT TRGIN
            CheckError(ret);
   		 if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain 2 " << CheckError(ret) << std::endl;
            
            /*HERE MUST BE CHANGED!!!!	to set correct group mask*/
            ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x10A8, 0xF);         // group trigger mask
            CheckError(ret);
   		 if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain 3 " << CheckError(ret) << std::endl;
            
            ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8110, 0x0);         // out trigger mask
            CheckError(ret);
   		 if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain 4 " << CheckError(ret) << std::endl;
            
            ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x8170, m_Delay);     // Run Delay decreases with the position (to compensate for run the propagation delay)
            CheckError(ret);
   		 if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain 5 " << CheckError(ret) << std::endl;
            
            ret = CAEN_DGTZ_WriteRegister(m_Handle, 0x811C, 0xFFF0FFFF | 0x00010000); // set bit sIN to trigger out
            CheckError(ret);
   		 if(ret != CAEN_DGTZ_Success) std::cout << "[ERROR] CAEN_DGTZ_RUN_SYNC_TrgOutSinDaisyChain 6 " << CheckError(ret) << std::endl;
            
            
            break;
            
        case CAEN_DGTZ_RUN_SYNC_SinFanout:
            break;
            
        default:
            break;
    }
    
    RegisterDump();
    std::cout << "SynchronisationMode: " << m_RunSyncMode << std::endl;
    std::cout << "Initialization " << m_Name << " module done!\n";
    
}
//-----------------------------------------------------------------------------
void DCAEN1740D::ReadVME() {
    
    m_Size = 0;
    ret = CAEN_DGTZ_ReadData(m_Handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_localBuffer, &m_Size);
    CheckError(ret);
    
    //cout << "1) ReadVME() m_Size:  " << m_Size << endl;
    
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] ReadData, error code = " << ret << "\n"; return; }
    //  cout << " --------- mSize:"  << m_Size << " ---------\n";
    if (m_Size == 0) return; // no data collected
    
    //cout << "2) ReadVME() m_Size:  " << m_Size << endl;
    
    // this 2 for loops are necessary because the values of chare and the value of subchannel stay from the prevous readout...
    //for(uint32_t i = 0; i < NumEvents[i] && i < 64; i++) {
    for(uint32_t i = 0; i < m_NrChannels; i++) {
        for (uint32_t j = 0; j < NumEvents[i]; j++) {
            gEvent[i][j].Charge = InitValue;
            gEvent[i][j].SubChannel = InitValue;
            gEvent[i][j].TimeTag = InitValue;
        }
    }
    
    int a_ret = _CAEN_DGTZ_GetDPPEvents(m_Handle, m_localBuffer, m_Size, (void **)gEvent, NumEvents);
    
    // PROVA !!!!
    //    for (int i=0; i< 64; i++)
    //		cout << "i: " << i << "   NumEvent:  " << NumEvents[i] << endl;

    //******************************************
    // This might be unnecessary and slowing down DAQ
/*
    std::ofstream tts("data/timestamp", std::ios::out | std::ios::app);
    for(uint32_t i = 0; i < m_NrChannels; i++) {
        for (uint32_t j = 0; j < NumEvents[i]; j++) {
            
            if (gEvent[i][j].TimeTag < gPrevTimeTag[i])
                gETT[i]++;
            
            gExtendedTimeTag[i] = (gETT[i] << 32) + (uint64_t)(gEvent[i][j].TimeTag);
            
            gPrevTimeTag[i]   = gEvent[i][j].TimeTag;
            tts << gExtendedTimeTag[i] << endl;
        }
    }
    tts.close();
*/
    //*******************************************
    
    
    /*
     printf("----- events ----- \n");
     for(int i = 0; i < 32; i++)
     cout << NumEvents[i] << " ";
     cout << endl;
     for(int i = 32; i < 64; i++)
     cout << NumEvents[i] << " ";
     cout << endl;
     */
    
    if(a_ret != 0) std::cerr << "[ERROR] in GetDPPEvents " << std::endl;
    else {
        //=============================================================================================================================
        /*
         uint32_t i2 = 0;
         uint32_t k = 0, j = 0;
         for(j=0; j<NumEvents[k] && k < 8; j++, ++k) {
         printf("----- charge j=%d k=%d\n", j, k);
         for (i2 = 0; i2 < 8; i2++) {
         printf("Gr.%d %5d\t%5d\t%5d\t%5d\t%5d\t%5d\t%5d\t%5d\n", i2,
         gEvent[8*i2+0][j].Charge & 0xFFFF, gEvent[8*i2+1][j].Charge & 0xFFFF, gEvent[8*i2+2][j].Charge & 0xFFFF, gEvent[8*i2+3][j].Charge & 0xFFFF,
         gEvent[8*i2+4][j].Charge & 0xFFFF, gEvent[8*i2+5][j].Charge & 0xFFFF, gEvent[8*i2+6][j].Charge & 0xFFFF, gEvent[8*i2+7][j].Charge & 0xFFFF );
         }
         printf("----- TimeTag j=%d k=%d\n", j, k);
         for (i2 = 0; i2 < 8; i2++) {
         printf("Gr.%u %8u\t%8u\t%8u\t%8u\t%8u\t%8u\t%8u\t%8u\n", i2,
         //gEvent[8*i2+0][j].SubChannel & 0xFFFF, gEvent[8*i2+1][j].SubChannel & 0xFFFF, gEvent[8*i2+2][j].SubChannel & 0xFFFF, gEvent[8*i2+3][j].SubChannel & 0xFFFF,
         //gEvent[8*i2+4][j].SubChannel & 0xFFFF, gEvent[8*i2+5][j].SubChannel & 0xFFFF, gEvent[8*i2+6][j].SubChannel & 0xFFFF, gEvent[8*i2+7][j].SubChannel & 0xFFFF );
         gEvent[8*i2+0][j].TimeTag , gEvent[8*i2+1][j].TimeTag , gEvent[8*i2+2][j].TimeTag, gEvent[8*i2+3][j].TimeTag,
         gEvent[8*i2+4][j].TimeTag , gEvent[8*i2+5][j].TimeTag , gEvent[8*i2+6][j].TimeTag, gEvent[8*i2+7][j].TimeTag);
         }
         }
         
         */
        
        uint32_t totalEvents = 0;
        for (uint32_t i = 0; i < m_NrChannels; ++i)
            totalEvents += NumEvents[i];
        
        m_Events += totalEvents;
        m_dataSizeByte += m_Size;
        
    }
}
//=============================================================================
void DCAEN1740D::GnuplotOnline(Gnuplot &gp){
    
    // this function could possibly goes to separate thread...
    //static flag = 0;
    if( m_Size == 0 ) return; // no data to display, return
    //UInt_t aEvent = 0;		     // otherwise we draw only the first event using gnuplot
    
    DKeyboard *dKeyboard = &DKeyboard::GetInstance();
    
    //if you want to change the graphs on gnuplot, be carefull it is quite tricky.... every space, comma, new line matters...
    //read first the gnuplot documentation
    static int first_time = 1;
    if(first_time){
        gp << "set xrange  [" << 0 << ":" << m_RecordLengthDPP << "]\n";
        gp << "set xtics nomirror tc lt 0\n";
        gp << "set x2range ["<< 0 << ":" << 16*m_RecordLengthDPP << "]\n";
        gp << "set x2tics nomirror tc lt 0\n";
        gp << "set yrange  [0:4095] \n"; //65535] \n";
        gp << "set ytics nomirror tc lt 0\n";
        gp << "set y2range [-2.2:2.2] \n";
        gp << "set y2tics nomirror tc lt 0\n";
        gp << "set grid ytics lt 0 lw 1 lc rgb \"#880000\"\n";
        gp << "set grid xtics lt 0 lw 1 lc rgb \"#880000\"\n";
        gp << "set style line 1 lt 1 lw 1 \n";
        gp << "set style line 2 lt 2 lw 1 \n";
        gp << "set style line 3 lt 3 lw 1 \n";
        gp << "set style line 4 lt 4 lw 1 \n";
        first_time = 0;
    }
    
    std::string gp_command = "";
    
    gp_command += string("plot ");
    /*
     cout << endl;
     for(int i = 0; i < 32; i++)
     cout << dKeyboard->m_ch[i] << " ";
     cout << endl;
     for(int i = 32; i < 64; i++)
     cout << dKeyboard->m_ch[i] << " ";
     cout << endl;
     */
    for(UInt_t i = 0; i < m_NrChannels; ++i){
        //for(int j=0; j<NumEvents[i]; ++j) {
        //    cout << "event i: "<< i << "\tj: " << j << endl;
        //uint32_t Charge     = gEvent[i][0].Charge & 0xFFFF;
        uint32_t SubChannel = gEvent[i][0].SubChannel & 0xFFFF;
        cout << "SubChannel: "<< SubChannel << "\tdKeyboard->m_ch["<<i<<"]: " <<  dKeyboard->m_ch[i] << " i: " << i << endl;
        //if(dKeyboard->m_ch[i] && SubChannel == i){
        if(dKeyboard->m_ch[i]){
            cout << "in, ch: "<< SubChannel << "\tdK" <<  dKeyboard->m_ch[i] << endl;
            
            _CAEN_DGTZ_DecodeDPPWaveforms(&gEvent[i][0], gWaveforms);
            
            std::string fname = string("temp/ch") + to_string(i);
            std::ofstream gpfile(fname, std::ofstream::out | std::ofstream::ate);
    /*        for(uint32_t k = 0; k < gWaveforms->Ns; k++){
                gpfile << m_DCoffsetDPP[i] + gWaveforms->Trace1[k] << " ";                  // signal           column 1
                gpfile << m_DCoffsetDPP[i] + 800 * gWaveforms->DTrace1[k] << " ";            // gate             column 2
                gpfile << m_DCoffsetDPP[i] - 200 - 500 * gWaveforms->DTrace2[k] << " ";            // trigger          column 2
                gpfile << m_DCoffsetDPP[i] - 300 - 500 * gWaveforms->DTrace3[k] << " ";    // trigger hold-off column 3
                gpfile << m_DCoffsetDPP[i] - 400 - 500 * gWaveforms->DTrace4[k] << " ";    // trigger hold-off column 3
                //gpfile << gWaveforms->Trace1[0] + m_ThresholdDPP[i] << " "; // trigger          column 4
                gpfile << endl;
            }
      */
            float ScalingFactor = 0.0625;
            float pippo;
            for(uint32_t k = 0; k < gWaveforms->Ns; k++){
                pippo = (65535 - m_DCoffsetDPP[i]) * ScalingFactor;
                
                gpfile << gWaveforms->Trace1[k] << " ";                  // signal           column 1
                gpfile << 100 + 3900 * gWaveforms->DTrace1[k] << " ";            // gate             column 2
                gpfile << pippo + 500 * gWaveforms->DTrace2[k] << " ";            // trigger + DCOffset(scaled)         column 3
                gpfile << endl;
            }
            gpfile.close();
            gp_command += string("'") + fname + string("' u 1 w l ls 1 t 'ch") + to_string(i) + string("', ");
            gp_command += string("'") + fname + string("' u 2 w l ls 2 t 'gt") + to_string(i) + string("', ");
            gp_command += string("'") + fname + string("' u 3 w l ls 3 t 'ho") + to_string(i) + string("', ");
            gp_command += string("'") + fname + string("' u 4 w l ls 4 t 'th") + to_string(i) + string("', ");
            gp << gp_command << endl;
        }
        //if (gp_command.length() > 5) gp << gp_command << endl;
        //}
    }
    
}
//-----------------------------------------------------------------------------
void DCAEN1740D::StartAcq(){
    
    //std::cout << "[DEBUG] DCAEN1740D::StartAcq\n";
    ShowSettings();
    InitModule();
    
    
    //std::cout << "Alocate Buffer.....";
    ret = static_cast<CAEN_DGTZ_ErrorCode>(_CAEN_DGTZ_MallocReadoutBuffer(m_Handle, &m_localBuffer, &m_Size));
    if (ret){
        std::cout << "Cannot allocate " << &m_Size << " bytes for the acquisition buffer! Exiting..." << std::endl;
        exit(0);
    }
    else     std::cout << "Allocated " << &m_Size << " bytes for readout buffer" << std::endl;
    
    //std::cout << "Alocate gEvents.....";
    for (unsigned i= 0; i < m_NrChannels; ++i) {
        int allocated_size = MAX_AGGR_NUM_PER_BLOCK_TRANSFER * MAX_EVENT_QUEUE_DEPTH * sizeof(_CAEN_DGTZ_DPP_QDC_Event_t);
        gEvent[i] = (_CAEN_DGTZ_DPP_QDC_Event_t *)malloc(allocated_size);
        
        if (gEvent[i] == NULL) {
            std::cout << "Cannot allocate memory for event list for channel " << i << "\n";
            exit(0);
        }
    }
    
    //std::cout << "Alocate DPPWaveforms (gWaveforms).....\n";
    ret = static_cast<CAEN_DGTZ_ErrorCode>(_CAEN_DGTZ_MallocDPPWaveforms(m_Handle, &gWaveforms, &gAllocatedSize));
    if (ret != CAEN_DGTZ_Success) std::cout << "Cannot allocate DPPWaveFormss " << ret << std::endl;
    
    
    std::cout << "[DEBUG] Send software trigger.....\n";
    ret = CAEN_DGTZ_SWStartAcquisition(m_Handle);
    if (ret != CAEN_DGTZ_Success) {
        std::cout << "Cannot start the acquisition " << ret << std::endl;
        exit(0);
    }
    
}

//-----------------------------------------------------------------------------
void DCAEN1740D::StopAcq(){
    
    ret = CAEN_DGTZ_SWStopAcquisition(m_Handle);
    CheckError(ret);
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] SWStopAcquisition, error code = " << ret << "\n"; return; }
    
    for(int i = 0; i < MAX_CHANNELS; ++i) {
        if (gEvent[i] != NULL)
            free(gEvent[i]);
    }
    for(int i=0; i<8; i++) {
        if (gEventsGrp[i] != NULL)
            free(gEventsGrp[i]);
    }
    
    
    ret = CAEN_DGTZ_ClearData(m_Handle);
    CheckError(ret);
    if(ret != CAEN_DGTZ_Success) { std::cout << "[ERROR] ClearData, error code = " << ret << "\n"; return; }
    
    ret = CAEN_DGTZ_Reset(m_Handle);
    CheckError(ret);
}

//-----------------------------------------------------------------------------
void DCAEN1740D::Log(std::ofstream & logfile) {
    
    logfile << "\t\t\t ----- CAEN 1740D::Settings" << std::endl;
    logfile << "\t\t\t |     Mask    DCoffset Polarity Width PreGate Baseline ---"<< std::endl;
    for(unsigned i = 0; i < m_NrGroups; i++){
        logfile << "\t\t\t | Gr." << i << "\t  " << m_GroupEnableMaskDPP[i] << "\t" << m_DCoffsetDPP[i] << "\t" <<  polarity(m_PulsePolarityDPP[i])
        << "\t  " << m_GateWidthDPP[i] << "\t" << m_PreGateDPP[i] << "\t" << m_BaseLineDPP[i] << std::endl;
    }
    
    logfile << "\t\t\t | --------- Trg Mask\t               Trg threshold (1...7) \tTrg HoldOff \tPreTrigger ---"<< std::endl;
    for(unsigned i = 0; i < m_NrGroups; i++){
        logfile << "\t\t\t | Gr." << i << "\t  ";
        for(unsigned j = 0; j < 8; j++)
            logfile << m_SelfTriggerMaskDPP[8*i+j] << " ";
        
        logfile << "\t";
        
        for(unsigned j = 0; j < 8; j++)
            logfile << m_ThresholdDPP[8*i+j] << " ";
        
        logfile << "\t";
        logfile << m_TriggerHoldOffDPP[i] << "\t" << m_PreTriggerDPP[i] ;
        logfile << std::endl;
    }
    
}


//======================================================================================================

void DCAEN1740D::BuildEvent(){
    if(m_Size == 0) return;
    //cout << "BuildEvent m_Size = " << m_Size << endl;
    
    // 0 - no saving,
    // 1-saving LIST short, forma1 < col1 = channel, col2 = charge, col3 = timestamp >
    // 2-saving LIST (long format), ready for Francesco's analysis in Matlab < col0...coln charge for each channel even if is 0, coln+1 timestamp >
    // e.g 0 0 0 ..... 64 times .... timestamp
    //
    // 4-saving LIST binary short, the same like short but writen in binary format not a text
    // 8-saving MIX2 full track+short format, only binary
    
    //**********************************
    //** ATTENZIONE: m_savingformat  =1;
    m_savingformat  =1; //for test only
    if( m_savingformat == 0) return;
    //---------------------------------------------------------------------------------------------------------------------------
    if( m_savingformat == 1){// 3 columns, 1-channel, 2-charge, 3-timestamp
        p_queue pEventQueue(compare);
        
        for (uint32_t i = 0; i < m_NrChannels; i++) {
            for(uint32_t j = 0; j < NumEvents[i]; j++) {
                uint16_t Charge     = gEvent[i][j].Charge;
                uint16_t SubChannel = uint16_t(8 * (i / 8)) + gEvent[i][j].SubChannel;
                uint64_t TimeStamp  = gEvent[i][j].TimeTag;
                pEventQueue.push( std::make_tuple(SubChannel, Charge, TimeStamp) );
            }
        }
        
        while(pEventQueue.size() != 0) {
            //int i = 0; cout << "while(pEventQueue.size() != 0) iteration n.   " << i << endl; i++;
            //** sembra sia sempre e solo una iterazione... da giocare con i buffer del digitiser...
            
            
//            m_StringBuffer1 +=  to_string(get<0>(pEventQueue.top())) + " "  + to_string(get<1>(pEventQueue.top()) ) + " " +to_string(get<2>(pEventQueue.top())) + "\n";
            /** Format of output string: <TimeStamp><DigitiserNumber><ChannelNumber><Energy> **/
            m_StringBuffer1 +=  to_string(get<2>(pEventQueue.top())) + " "  + to_string(BoardInfo.SerialNumber) + " " + to_string(get<0>(pEventQueue.top()) ) + " " +to_string(get<1>(pEventQueue.top())) + "\n";
            pEventQueue.pop(); // remove the oldest element of the queue --> reduce the size of the queue by 1
        }
       /* 
        if(BoardInfo.SerialNumber == 34)
            cout <<
        //"BuildEvent: " <<
            m_StringBuffer1 << endl; //*/
        
    }
    //--------- end of m_savingformat == 1   ------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------------
    if( m_savingformat == 2){
        static uint32_t		flag = 1;
        
        if(flag == 1){
            flag = 0;
            memset(gExtendedTimeTag, 0, 64*sizeof(uint64_t));
            memset(gETT,0, 64*sizeof(uint64_t));
            memset(gPrevTimeTag, 0, 64*sizeof(uint64_t));
        }
        
        
        for (uint32_t i = 0; i < 64; i++) {
            for(uint32_t j = 0; j < NumEvents[i]; j++) {
                for(uint32_t k = 0; k < m_NrChannels; k++) {
                    m_StringBuffer2 += to_string( gEvent[k][j].Charge ) + " " ;
                }
                
                if (gEvent[i][j].TimeTag < gPrevTimeTag[i])
                    gETT[i]++;
                
                gExtendedTimeTag[i] = (gETT[i] << 32) + (uint64_t)(gEvent[i][j].TimeTag);
                
                m_StringBuffer2 += to_string(gEvent[i][j].TimeTag) + " ";
                m_StringBuffer2 += to_string(gExtendedTimeTag[i]) + "\n";
                
                gPrevTimeTag[i]   = gEvent[i][j].TimeTag;
            }
        }
        
        m_StringBufferSize2 = m_StringBuffer2.length();
    }
    //--------- end of m_savingformat == 2   ------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------------
    if( m_savingformat == 4){  // like list2 but binary
        for (uint32_t i = 0; i < m_NrChannels; i++) {
            //cout << "====== i: NumEvents["<<i<<"] " << NumEvents[i] << endl;;
            for(uint32_t j = 0; j < NumEvents[i]; j++) {
                uint16_t Charge     = gEvent[i][j].Charge;
                uint16_t SubChannel = uint16_t(8 * (i / 8)) + gEvent[i][j].SubChannel;
                uint64_t TimeStamp  = gEvent[i][j].TimeTag;
                //cout << "i= " << i << " j= " << j << "\t" << Charge << " " << SubChannel << " " << TimeStamp << endl;
                memcpy(m_Buffer + m_BufferPos, &Charge,     sizeof(uint16_t));
                m_BufferPos += sizeof(uint16_t);
                memcpy(m_Buffer + m_BufferPos, &SubChannel, sizeof(uint16_t));
                m_BufferPos += sizeof(uint16_t);
                memcpy(m_Buffer + m_BufferPos, &TimeStamp,  sizeof(uint64_t));
                m_BufferPos += sizeof(uint64_t);
            }
        }
    }
    //--------- end of m_savingformat == 4   ------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------------
    //if( m_savingformat == 8) // need to be implemented
    
    
}

//======================================================================================================

void DCAEN1740D::SaveEvent(){
    
}
//======================================================================================================

void DCAEN1740D::ShowEvent(){
    
    
}

//======================================================================================================

void DCAEN1740D::SendEvent(){
    
}

//======================================================================================================
std::string DCAEN1740D::CheckError(CAEN_DGTZ_ErrorCode err){
    
    std::string s1;
    
    switch (err) {
        case 0:
            break;
        case -1:  s1 = " Communication error "; 						break;
        case -2:  s1 = " Unspecified error " ; 							break;
        case -3:  s1 = " Invalid parameter " ; 							break;
        case -4:  s1 = " Invalid Link Type "; 							break;
        case -5:  s1 = " Invalid device m_Handle ";						break;
        case -6:  s1 = " Maximum number of devices exceeded ";					break;
        case -7:  s1 = " The operation is not allowed on this type of board "; 			break;
        case -8:  s1 = " The interrupt level is not allowed ";					break;
        case -9:  s1 = " The event number is bad "; 						break;
        case -10: s1 = " Unable to read the registry ";						break;
        case -11: s1 = " Unable to write into the registry ";					break;
        case -13: s1 = " The channel number is invalid ";					break;
        case -14: s1 = " The Channel is busy "; 						break;
        case -15: s1 = " Invalid FPIO Mode "; 							break;
        case -16: s1 = " Wrong acquisition mode " ; 						break;
        case -17: s1 = " This function is not allowed for this module "; 			break;
        case -18: s1 = " Communication Timeout " ; 						break;
        case -19: s1 = " The buffer is invalid " ; 						break;
        case -20: s1 = " The event is not found " ; 						break;
        case -21: s1 = " The event is invalid " ; 						break;
        case -22: s1 = " Out of memory " ; 							break;
        case -23: s1 = " Unable to calibrate the board " ; 					break;
        case -24: s1 = " Unable to open the digitizer " ; 					break;
        case -25: s1 = " The Digitizer is already open " ; 					break;
        case -26: s1 = " The Digitizer is not ready to operate ";				break;
        case -28: s1 = " The digitizer flash memory is corrupted ";				break;
        case -27: s1 = " The Digitizer has not the IRQ configured "; 				break;
        case -29: s1 = " The digitizer dpp firmware is not supported in this lib version "; 	break;
        case -30: s1 = " Invalid Firmware License " ; 						break;
        case -31: s1 = " The digitizer is found in a corrupted status "; 			break;
        case -32: s1 = " The given trace is not supported by the digitizer " ; 			break;
        case -33: s1 = " The given probe is not supported for the given digitizer's trace "; 	break;
        case -99: s1 = " The function is not yet implemented ";					break;
        default:  s1 = " Error unknown, check the library version, this was created for 2.7.5 ";break;
    }
    
    return s1;
}
//======================================================================================================
void DCAEN1740D::ShowData(DGDisplay *fDisplay, DAcquisition *fAcquisition) {
    
    
    
    fAcquisition->m_AcqStatusEntry2 = m_Events;
    UInt_t Nb = 0, Ne, prevNb = 0;
    Nb  = GetDataSize();
    Ne  = GetNrEvents();
    //uint32_t a_TotEvCnt = 0;
    
    std::cout << "\tReading bytes =\t" << ( (float)(Nb-prevNb) / 1024 / 1024 / fAcquisition->m_ElapsedAcqTime*1.048576f )  << " [MB/s]" << std::endl;
    std::cout << "\tEvents =\t"      << Ne  << std::endl;
    std::cout << "\tRate =\t\t"        <<  ((float)Ne / ((fAcquisition->m_ElapsedAcqTime*1.048576f))/1000 ) << "[Kevt/s]"<< std::endl;
    
    prevNb = Nb;
    
    
    // fill histograms only in GUI version when fDisplay is not NULL
    if(fDisplay)
    {
        
    }
    
}

//-----------------------------------------------------------------------------
void DCAEN1740D::DataSave(DAcquisition *fAcquisition){
    // write data to buffer only when the IRQ happend, otherwise return
    // CHECK IF THIS IS REALLY NECESSARY, there is m_dataSizeByte which shoud be 0 if there is no IRQ
    // If you want to simulate events you have to comment this line, m_IRQ = 1;
    //std::cout << "[DEBUG] 1  BoardInfo: " << to_string(BoardInfo.SerialNumber) << "  DCAEN1740D::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
    //**if( !VME_CRATE ) m_IRQ = 1;
    //**if( fAcquisition->m_EmptyBuffer ) m_IRQ = 1;
    //std::cout << "[DEBUG] 2 DCAEN1740D::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
    //if(!m_IRQ) return;
    //**m_IRQ = 0;
    //std::cout << "[DEBUG] 3 DCAEN1740D::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
    
    Bool_t m_saveAfterSize = kFALSE; // variable to check if the buffer size reach the max size
    Bool_t m_saveAfterTime = kFALSE; // variable to check if the time elapsed to save file every sec/min/hour
    
    static UInt_t m_fileEvents = 0; // temporary value to keep the nr of events per file needed for logfile
    
    gCurrentTimeADC = gGetLongTimeADC() / 1000;              // check the current time
    gElapsedTimeADC = gCurrentTimeADC - gPrevRateTimeADC;    // calcullate elapsed time
    
    //memcpy((void*)m_Buffer + m_BufferPos, (void*)m_localBuffer, m_dataSizeByte); // copy data from local buffor to main beffor
    //memcpy(m_Buffer + m_BufferPos, m_localBuffer, m_dataSizeByte); // copy data from local buffor to main beffor
    //m_BufferPos += (m_dataSizeByte);                                 // move the position of the buffer
    
    // ==========================================================================
    // check if the buffer is full and has to be writen to file
    //cout << "m_BufferPos " << m_BufferPos << ",\tsize "<<  UInt_t(fAcquisition->m_SaveFileSizeEntry*1000*1000) << endl;
    if( m_BufferPos  > UInt_t(fAcquisition->m_SaveFileSizeEntry*1024*1024) ) {
        m_saveAfterSize = kTRUE;
    }
    
    if( m_StringBuffer1.length()  > UInt_t(fAcquisition->m_SaveFileSizeEntry*1024*1024) ) {
        m_saveAfterSize = kTRUE;
    }
    
    //m_StringBuffer2 è usato in savingformat2
    if( m_StringBuffer2.length()  > UInt_t(fAcquisition->m_SaveFileSizeEntry*1024*1024) ) {
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
        //std::cout << "[DEBUG] 4  BoardInfo: "  << to_string(BoardInfo.SerialNumber) << "  DCAEN1740D::DataSave m_EmptyBuffer = "<< fAcquisition->m_EmptyBuffer << std::endl;
        char nr[256];
        sprintf(nr, "%03d", fAcquisition->m_NrOfSavedFiles);
        
        // take the time now
        fAcquisition->m_TimeNow = std::time(NULL);
        std::string s1 = string(ctime(&fAcquisition->m_TimeNow));
        
        if( m_savingformat == 1){
	    string filename = fAcquisition->m_DataPath + string("/") + fAcquisition->GetFileTime() + string("_DigNo") + to_string(BoardInfo.SerialNumber) + string("_") + fAcquisition->GetFileName() + string("_") + string(nr) + string(".lst1");
            
            cout << "DataSave___afterCondition    " << filename << endl;
            std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::binary);
            if(!DataFile.is_open()) {
                std::cerr << "[ ERROR ] could not open the file... " << filename << std::endl;
                *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- could not open the file..." << filename<< "\n";
                return;
            }
            DataFile << m_StringBuffer1;
            DataFile.close();
            *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- " << "file [" << filename<< "] has been saved\n";
            *fAcquisition->fLog << "\t\t\t\tAcq time: " << fAcquisition->m_TimeNow - fAcquisition->m_StartAcqTime << "s, "
            << "events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
            std::cout << "[MESSAGE] data file [" << filename << "] has been saved " << std::endl;
            std::cout << "          Nr Events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
        }
        
        if( m_savingformat == 2){
            //if( true ){
            string filename = fAcquisition->m_DataPath + string("/") + fAcquisition->GetFileTime()
            + string("_") + fAcquisition->GetFileName() + string("_") + string(nr) + string(".lst2");
            std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::binary);
            if(!DataFile.is_open()) {
                std::cerr << "[ ERROR ] could not open the file... " << filename << std::endl;
                *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- could not open the file..." << filename<< "\n";
                return;
            }
            DataFile << m_StringBuffer2;
            DataFile.close();
            *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- " << "file [" << filename<< "] has been saved\n";
            *fAcquisition->fLog << "\t\t\t\tAcq time: " << fAcquisition->m_TimeNow - fAcquisition->m_StartAcqTime << "s, "
            << "events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
            std::cout << "[MESSAGE] data file [" << filename << "] has been saved " << std::endl;
            std::cout << "          Nr Events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
        }
        
        if( m_savingformat == 4){
            //if( false){
            string filename = fAcquisition->m_DataPath + string("/") + fAcquisition->GetFileTime()
            + string("_") + fAcquisition->GetFileName() + string("_") + string(nr) + string(".bin");
            std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::binary);
            if(!DataFile.is_open()) {
                std::cerr << "[ ERROR ] could not open the file... " << filename << std::endl;
                *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- could not open the file..." << filename<< "\n";
                return;
            }
            DataFile.write((char*)m_Buffer, m_BufferPos);
            DataFile.close();
            *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- " << "file [" << filename<< "] has been saved\n";
            *fAcquisition->fLog << "\t\t\t\tAcq time: " << fAcquisition->m_TimeNow - fAcquisition->m_StartAcqTime << "s, "
            << "events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
            std::cout << "[MESSAGE] data file [" << filename << "] has been saved " << std::endl;
            std::cout << "          Nr Events total: " << m_Events << " / in file: " << m_Events - m_fileEvents << std::endl;
        }
        
        if( m_savingformat == 8){
            string filename = fAcquisition->m_DataPath + string("/") + fAcquisition->GetFileTime()
            + string("_") + fAcquisition->GetFileName() + string("_") + string(nr) + string(".mix");
            std::ofstream DataFile(filename, std::ofstream::out | std::ofstream::binary);
            if(!DataFile.is_open()) {
                std::cerr << "[ ERROR ] could not open the file... " << filename << std::endl;
                *fAcquisition->fLog << s1.substr(0, s1.length()-1) << " ----- could not open the file..." << filename<< "\n";
                return;
            }
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
        m_StringBuffer1.clear();
        m_StringBuffer2.clear();  // buffer usato in m_savingformat2
        m_BufferPos = 0;
        //** FUNZIONA SOLO CON DUE DIGIT>...
	if(pluto) pluto = 0;
        else
        {
            pluto=1;
        m_fileEvents = m_Events;
        fAcquisition->m_NrOfSavedFiles++;
        gPrevRateTimeADC = gCurrentTimeADC;
        }
        
    }
    
}

//-----------------------------------------------------------------------------
void DCAEN1740D::ResetModule() {
    
    CAEN_DGTZ_Reset(m_Handle);
    
}
//*****************************************************************************
void DCAEN1740D::ShowSettings() {
    
    std::cout << "[MESSAGE] ======= ::"      << m_Name << "::ShowSettings =======" << std::endl;
    std::cout << "\t| m_Name\t\t= "          << m_Name                      << std::endl  ;
    std::cout << "\t| m_Handle\t\t= "        << m_Handle                    << std::endl  ;
    std::cout << "\t| m_Module ID\t\t= "     << hex << m_ModuleID    << dec << std::endl;
    std::cout << "\t| m_BaseAddress\t\t= 0x" << hex << m_BaseAddress << dec << std::endl;
    std::cout << "\t| m_AcqMode\t\t= "       << acqmode(m_AcqMode)          << std::endl;
    std::cout << "\t| m_MaxEventsAggBLT\t= " << m_MaxEventsAggBLT           << std::endl;
    std::cout << "\t+ =============================================="<< std::endl;
    std::cout << "\t| m_AcqModeDPP\t= "              << acqmodedpp(m_AcqModeDPP) << std::endl;
    std::cout << "\t| m_EnableChargePedestalDPP\t= " << m_EnableChargePedestalDPP << std::endl;
    std::cout << "\t| m_TriggerModeDPP\t= "          << m_TriggerModeDPP << std::endl;
    std::cout << "\t| --- channel settings --------- gate settings ----------------------------"<< std::endl;
    std::cout << "\t|     Mask    DCoffset Polarity Width PreGate Baseline smooth charge.sen---"<< std::endl;
    for(unsigned i = 0; i < m_NrGroups; i++){
        std::cout << "\t| Gr." << i << "\t" << m_GroupEnableMaskDPP[i] << "\t" << m_DCoffsetDPP[i] << "\t" <<  polarity(m_PulsePolarityDPP[i])
        << "\t" << m_GateWidthDPP[i] << "\t" << m_PreGateDPP[i] << "\t" << m_BaseLineDPP[i]
        << "\t" << m_TriggerSmoothingDPP[i] << "\t" << m_ChargeSensitivityDPP[i] << std::endl;
    }
    
    std::cout << "\t| --- trigger settings ---"<< std::endl;
    std::cout << "\t|     PreTrigg  th0\tth1\tth2\tth3\tth4\tth5\tth6\tth7  ---"<< std::endl;
    for(unsigned i = 0; i < m_NrGroups; i++){
        std::cout << "\t| Gr." << i << "\t" << m_PreTriggerDPP[i] << "\t";
        for(unsigned j = 0; j < 8; j++)
            std::cout << m_ThresholdDPP[8*i+j] << "\t";
        std::cout << std::endl;
    }
    
    std::cout << "\t| --- self trigger mask ---"<< std::endl;
    for(unsigned i = 0; i < m_NrGroups; i++){
        std::cout << "\t| Gr." << i << "\t";
        for(unsigned j = 0; j < 8; j++)
            std::cout << m_SelfTriggerMaskDPP[8*i+j] << " ";
        std::cout << std::endl;
    }
    
    /*
     std::cout << "\t| --- group/channel gatewidth/threshold ---"<< std::endl;
     std::cout << "\t+\t\t gr0\tgr1\tgr2\tgr3\tgr4\tgr5\tgr5\tgr7"<< std::endl;
     std::cout << "\t| GateWidth\t" << m_GateWidthDPP[0] <<"\t"<< m_GateWidthDPP[1] <<"\t"<< m_GateWidthDPP[2] <<"\t"<< m_GateWidthDPP[3] <<"\t"
     << m_GateWidthDPP[4] <<"\t"<< m_GateWidthDPP[5] <<"\t"<< m_GateWidthDPP[6] <<"\t"<< m_GateWidthDPP[7] << std::endl;
     
     for(unsigned i = 0; i < m_NrGroups; i++){
     std::cout << "\t| Thr ch: " << 8*i << "-"<< 8*i+7 << "\t";
     for(unsigned j = 0; j < 8; j++)
     std::cout << m_ThresholdDPP[8*i+j] << "\t";
     std::cout << std::endl;
     }
     std::cout << "\t+ =============================================="<< std::endl;
     */
    
}

//==================================================================================
void DCAEN1740D::SaveConfig(std::ofstream & fout){
    
    fout << "[" << m_Name << "]" << endl;
    fout << "# ======= Channel input signal settings ==========" << endl;
    fout << "RecordLengthDPP "        << m_RecordLengthDPP        <<endl;
    fout << "# you can choose how many samples are stored before and after triggeer appear,  the value are between 0, 100 in %" << endl;
    fout << "# in case of DPP-QDC PreTrigger must be > Pregate + 7"<< endl;
    fout << "# Offset for group channels " << endl;
    fout << "# The values are in ADC convertion numbers, to see coresponding mV value run acq, look at the right yaxis and addapt accordingly" << endl;
    fout << "# 32768 is a middle, there is no offset" << endl;
    fout << "# syntax: DCoffset <group> <value>" << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "DCoffsetDPP " << i << " " << m_DCoffsetDPP[i]  << endl;
    fout << endl;
    //fout << "PostTrigger " << m_PostTrigger         <<endl;
    //fout << endl;
    fout << "# GroupEnableMask: disable = 0 or enable = 1 group channels" << endl;
    fout << "# syntax: GroupEnableMask <group> <value>" << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "GroupEnableMaskDPP " << i << " " << m_GroupEnableMaskDPP[i]  << endl;
    fout << endl;
    //fout << "# Set the pulse polarity, here is per group but you can do it manualy for every channel " << endl;
    //fout << "#  decided that for group is enough, The value can be 0 for positive signals and 1 for negative" << endl;
    //fout << "# need intervention in the source code..." << endl;
    //for(unsigned i = 0; i < m_NrGroups; i++)
    //   fout << "ChannelPulsPolarityDPP " << i << " " << m_ChannelPulsPolarity[i]  << endl;
    
    //fout << endl;
    fout << "# The save option allows you to save certain channels. The digitizer is designed in such that even only one channel is used"<< endl;
    fout << "# all group must be transfered to PC. Then you can choose by software which channel you want to store and/or display."<< endl;
    fout << "# It is one 32/64 bits word, Each bit correspond to each channel"<< endl;
    fout << "SaveChannels " <<  m_SaveChannel        << endl;
    fout << endl;
    
    fout << "# ====== Trigger Configuration ==========" << endl;
    // fout << "# Enable or disable software, external and channel trigger" << endl;
    // fout << "# the options could be 0, 1, 2, 3, disable, enable acq only, enable ext only, both, respectevely "<< endl;
    // fout << "# syntax: ChannelTriggerMode <group> <value> "<< endl;
    // for(unsigned i = 0; i < m_NrGroups; i++)
    //    fout << "ChannelTriggerMode " << i << " " << m_ChannelTriggerMode[i]  << endl;
    // fout << endl;
    // fout << "# " << endl;
    // fout << "ExtTriggerMode "<< m_ExtTriggerMode       <<endl;
    // fout << "# " << endl;
    // fout << "# Adjust the threshold to the coresponding channel" << endl;
    // fout << "# Treshold value is an absolut value, consider it when you add offset to the channel" << endl;
    // for(unsigned i = 0; i < m_NrGroups; i++)
    //    fout << "Threshold " << i <<" " <<  m_Threshold[i]  << endl;
    // fout << endl;
    // fout << "# Trigger slope for the every group of channels, could be rising (0) or falling (1)" << endl;
    // for(unsigned i = 0; i < m_NrGroups; i++)
    //    fout << "TriggerEdge " << i << " " << m_TriggerEdge[i]  << endl;
    // fout << endl;
    
    fout << "# Global trigger can be built from logic OR all indyvidual channel triggers. This option allows you to set which indyvidual "<< endl;
    fout << "# trigger will contribute to build the global one. This is set by indyvidual group. Every group consist of 8 channels" << endl;
    fout << "SelfTriggerMaskGr0 " << m_SelfTriggerMaskDPP[0]  << m_SelfTriggerMaskDPP[1]  << m_SelfTriggerMaskDPP[2]  << m_SelfTriggerMaskDPP[3]
    << m_SelfTriggerMaskDPP[4]  << m_SelfTriggerMaskDPP[5]  << m_SelfTriggerMaskDPP[6]  << m_SelfTriggerMaskDPP[7]  << endl;
    fout << "SelfTriggerMaskGr1 " << m_SelfTriggerMaskDPP[8]  << m_SelfTriggerMaskDPP[9]  << m_SelfTriggerMaskDPP[10] << m_SelfTriggerMaskDPP[11]
    << m_SelfTriggerMaskDPP[12] << m_SelfTriggerMaskDPP[13] << m_SelfTriggerMaskDPP[14] << m_SelfTriggerMaskDPP[15] << endl;
    fout << "SelfTriggerMaskGr2 " << m_SelfTriggerMaskDPP[16] << m_SelfTriggerMaskDPP[17] << m_SelfTriggerMaskDPP[18] << m_SelfTriggerMaskDPP[19]
    << m_SelfTriggerMaskDPP[20] << m_SelfTriggerMaskDPP[21] << m_SelfTriggerMaskDPP[22] << m_SelfTriggerMaskDPP[23] << endl;
    fout << "SelfTriggerMaskGr3 " << m_SelfTriggerMaskDPP[24] << m_SelfTriggerMaskDPP[25] << m_SelfTriggerMaskDPP[26] << m_SelfTriggerMaskDPP[27]
    << m_SelfTriggerMaskDPP[28] << m_SelfTriggerMaskDPP[29] << m_SelfTriggerMaskDPP[30] << m_SelfTriggerMaskDPP[31] << endl;
    fout << "SelfTriggerMaskGr4 " << m_SelfTriggerMaskDPP[32] << m_SelfTriggerMaskDPP[33] << m_SelfTriggerMaskDPP[34] << m_SelfTriggerMaskDPP[35]
    << m_SelfTriggerMaskDPP[36] << m_SelfTriggerMaskDPP[37] << m_SelfTriggerMaskDPP[38] << m_SelfTriggerMaskDPP[39] << endl;
    fout << "SelfTriggerMaskGr5 " << m_SelfTriggerMaskDPP[40] << m_SelfTriggerMaskDPP[41] << m_SelfTriggerMaskDPP[42] << m_SelfTriggerMaskDPP[43]
    << m_SelfTriggerMaskDPP[44] << m_SelfTriggerMaskDPP[45] << m_SelfTriggerMaskDPP[46] << m_SelfTriggerMaskDPP[47] << endl;
    fout << "SelfTriggerMaskGr6 " << m_SelfTriggerMaskDPP[48] << m_SelfTriggerMaskDPP[49] << m_SelfTriggerMaskDPP[50] << m_SelfTriggerMaskDPP[51]
    << m_SelfTriggerMaskDPP[52] << m_SelfTriggerMaskDPP[53] << m_SelfTriggerMaskDPP[54] << m_SelfTriggerMaskDPP[55] << endl;
    fout << "SelfTriggerMaskGr7 " << m_SelfTriggerMaskDPP[56] << m_SelfTriggerMaskDPP[57] << m_SelfTriggerMaskDPP[58] << m_SelfTriggerMaskDPP[59]
    << m_SelfTriggerMaskDPP[60] << m_SelfTriggerMaskDPP[61] << m_SelfTriggerMaskDPP[62] << m_SelfTriggerMaskDPP[63] << endl;
    fout << "# " << endl;
    
    fout << "# All digitial inputs on the fron panel can be set as TTL = 1 or NIM = 0" << endl;
    fout << "IOLevel "             << m_FPIOtype          << endl;
    fout << "# " << endl;
    
    fout << "# Acquisition Mode could be software controled = 0; or by signal from the front panel = 1" << endl;
    fout << "AcquisitionMode "     << m_AcqMode           << endl;
    
    // these option are only for DPP-QDC firmware
    fout << "# " << endl;
    fout << "# ===== These options are only for DPP-QDC firmware =====" << endl;
    fout << "# " << endl;
    fout << "# Acquisition DPP Mode could be MIXED = 0; or LIST = 1" << endl;
    fout << "AcquisitionModeDPP "     << m_AcqModeDPP           << endl;
    fout << "# PulsePolarityDPP per group, 1 negative, 0 positive" << endl;
    fout << "# syntax: PulsePolarityDPP <group> <value>" << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "PulsePolarityDPP " << i << " " << m_PulsePolarityDPP[i] << endl;
    
    fout << "# ======= BaseLine settings ==========" << endl;
    fout << "# if FixedBaseLine equal 0 = fixed valu value, 1 = 4 samples, 2 = 16 samples, 3 = 64 samples, 4 = 256 samples" << endl;
    fout << "# BaseLine: baseline value per group " << endl;
    fout << "# syntax: BaseLineDPP <group> <fixed value>" << endl;
    fout << "FixedBaseLineDPP " <<  m_FixedBaseLineDPP << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "BaseLineDPP " << i << " " << m_BaseLineDPP[i] << endl;
    
    fout << "# ======= Gate settings ==========" << endl;
    fout << "# syntax: GateWidth            <group> <value>" << endl;
    fout << "#         Pregate              <group> <value>" << endl;
    fout << "#         EnableChargePedestal <value>" << endl;
    fout << "#         ChargeSensitivity    <value> " << endl;
    fout << "# gate width (in steps of 16 ns) " << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "GateWidthDPP " << i << " " << m_GateWidthDPP[i]  << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "PreGateDPP " << i << " " << m_PreGateDPP[i]  << endl;
    
    
    fout << "# EnableChargePedestal: 0 = disabled; 1 = enabled (add 1024 to the charge) " << endl;
    fout << "EnableChargePedestalDPP " <<  m_EnableChargePedestalDPP << endl;
    fout << "#  " << endl;
    fout << "# ========== TriggerSettings " << endl;
    fout << "# TriggerSettings in case of DPP-QDC is different than the standard firmware." << endl;
    fout << "# In the DPP-QDC is per channel, in the standard firmware per group." << endl;
    fout << "# expressed in ADC LSB (1 LSB = 0.5mV) " << endl;
    fout << "# syntax: ThresholdDPP     <group> <value>" << endl;
    for(unsigned i = 0; i < m_NrChannels; i++)
        fout << "ThresholdDPP " << i << " " << m_ThresholdDPP[i]  << endl;
    fout << "# TriggerMode: 0 = Normal; 1 = Paired" << endl;
    fout << "TriggerModeDPP " << m_TriggerModeDPP << endl;
    fout << "# TriggerHoldOff: Trigger hold off (in steps of 16ns) " << endl;
    fout << "# syntax: TriggerHoldOffDPP     <group> <value>" << endl;
    
    
    fout << "# Charge sensitivity:  0=0.16pC, 1=0.32pC, 2=0.64pC, 3=1.28pC, 4=2.56pC, 5=5.12pC, 6=10.24pC, 7=20.48pC  " << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "ChargeSensitivityDPP " << i << " " << m_ChargeSensitivityDPP[i] << endl;
    
    fout << "# TriggerSmoothing: 0 = no smoothing, otherwise mean over 2^n samples, with n <= 6 " << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "TriggerSmoothingDPP " << i << " " << m_TriggerSmoothingDPP[i] << endl;
    
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "TriggerHoldOffDPP "<< i << " " << m_TriggerHoldOffDPP[i] << endl;
    fout << "# PreTriggerDPP: must be bigger than pregate +7 " << endl;
    fout << "# syntax: PreTriggerDPP     <group> <value>" << endl;
    for(unsigned i = 0; i < m_NrGroups; i++)
        fout << "PreTriggerDPP "<< i<< " " <<  m_PreTriggerDPP[i] << endl;
    fout << endl;
    
    fout << "# Delay in the daisy chain, value in CLK of digitizer. The last digitizer in the chain should be equal 0 and value increase backward" << endl;
    fout << "ClockDelay "     << m_Delay             << endl;
    fout << "# if you want to change clock setings you have to first update firmware" << endl;
    fout << "ClockSource "           << m_Clock             << endl;
    fout << "# Define maximum parameter of how many events can be read from instrument" << endl;
    fout << "MaxEventsAggBLT "   << m_MaxEventsAggBLT           << endl;
    fout << "# DO not use it" << endl;
    fout << "SynchronizationMode "<< m_RunSyncMode         << endl;
    fout << "# " << endl;
    fout << endl;
    
}


//==================================================================================
void DCAEN1740D::LoadConfig(std::ifstream & inpfile){
    
    cout << "[MESSAGE] Load configuration file CAEN1740D......" << endl;
    
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
            if     ( name == string("RecordLengthDPP")	) m_RecordLengthDPP	= atoi( value.c_str() );
            //else if( name == string("PostTrigger")	) m_PostTrigger		= atoi( value.c_str() );
            else if( name == string("GroupEnableMaskDPP")	) {
                stringstream ss(value);
                while (ss >> buf)  param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8)
                    m_GroupEnableMaskDPP[ gr ] = val;
            }
            else if( name == string("DCoffsetDPP")	) {
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8)
                    m_DCoffsetDPP[ gr ] = val;
            }
            //else if( name == string("ChannelPulsPolarityDPP")){
            //   stringstream ss(value);
            //while (ss >> buf) param.push_back(buf);
            //m_ChannelPulsPolarity[ stoi(param.at(0)) ] = stoi( param.at(1) );
            //}
            else if( name == string("SaveChannels")	      ) m_SaveChannel 	   	 = atoi( value.c_str()   );
            //else if( name == string("ExtTriggerMode") 	      ) m_ExtTriggerMode         = atoi( value.c_str()   );
            //else if( name == string("ChannelTriggerMode") ){
            // stringstream ss(value);
            //while (ss >> buf) param.push_back(buf);
            //m_ChannelTriggerMode[ stoi(param.at(0)) ] = stoi( param.at(1) );
            //}
            //else if( name == string("Threshold")	) {
            //   stringstream ss(value);
            //while (ss >> buf) param.push_back(buf);
            //m_Threshold[ stoi(param.at(0)) ] = stoi( param.at(1) );
            //}
            //else if( name == string("TriggerEdge")  ) {
            // stringstream ss(value);
            //while (ss >> buf) param.push_back(buf);
            //m_TriggerEdge[ stoi(param.at(0)) ] = stoi( param.at(1) );
            //}
            else if( name == string("SelfTriggerMaskGr0")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+0]  = 1; else m_SelfTriggerMaskDPP[i+0]  = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr1")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+8]  = 1; else m_SelfTriggerMaskDPP[i+8]  = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr2")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+16] = 1; else m_SelfTriggerMaskDPP[i+16] = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr3")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+24] = 1; else m_SelfTriggerMaskDPP[i+24] = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr4")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+32] = 1; else m_SelfTriggerMaskDPP[i+32] = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr5")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+40] = 1; else m_SelfTriggerMaskDPP[i+40] = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr6")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+48] = 1; else m_SelfTriggerMaskDPP[i+48] = 0; }
            }
            
            else if( name == string("SelfTriggerMaskGr7")){
                for(Int_t i = 0; i < 8; i++){ if(value[i] == '1') m_SelfTriggerMaskDPP[i+56] = 1; else m_SelfTriggerMaskDPP[i+56] = 0; }
            }
            
            else if( name == string("IOLevel") 	){
                m_FPIOtype = atoi( value.c_str() );
		cout << "IOLevel: " << m_FPIOtype << endl;}
            
            else if( name == string("AcquisitionMode") )
                m_AcqMode = atoi( value.c_str() );
            
            else if( name == string("AcquisitionModeDPP") )
                m_AcqModeDPP = atoi( value.c_str() );
            
            else if( name == string("BaseLineDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8 && m_FixedBaseLineDPP == 0) m_BaseLineDPP[ gr ] = val;
            }
            
            else if( name == string("FixedBaseLineDPP") ){
                m_FixedBaseLineDPP = atoi( value.c_str() );
                if(m_FixedBaseLineDPP == 1)      for(UInt_t i = 0; i < m_NrGroups; i++) m_BaseLineDPP[i] = 4;
                else if(m_FixedBaseLineDPP == 2) for(UInt_t i = 0; i < m_NrGroups; i++) m_BaseLineDPP[i] = 16;
                else if(m_FixedBaseLineDPP == 3) for(UInt_t i = 0; i < m_NrGroups; i++) m_BaseLineDPP[i] = 64;
                else if(m_FixedBaseLineDPP == 4) for(UInt_t i = 0; i < m_NrGroups; i++) m_BaseLineDPP[i] = 256;
            }
            
            else if( name == string("GateWidthDPP")  ) {
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8)m_GateWidthDPP[ gr ] = val;
            }
            
            else if( name == string("ThresholdDPP")  ) {
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int ch = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if( ch < 64) m_ThresholdDPP[ ch ] = val;
            }
            
            else if( name == string("PulsePolarityDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8) m_PulsePolarityDPP[ gr ] = val;
            }
            
            else if( name == string("PreGateDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8) m_PreGateDPP[ gr ] = val;
            }
            
            else if( name == string("EnableChargePedestalDPP"))
                m_EnableChargePedestalDPP = atoi( value.c_str() );
            
            else if( name == string("ChargeSensitivityDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8) m_ChargeSensitivityDPP[ gr ] = val;
            }
            
            else if( name == string("PreTriggerDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8) m_PreTriggerDPP[ gr ] = val;
            }
            
            else if( name == string("TriggerModeDPP") )
                m_TriggerModeDPP = atoi( value.c_str() );
            
            else if( name == string("TriggerSmoothingDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8) m_TriggerSmoothingDPP[ gr ] = val;
            }
            
            else if( name == string("TriggerHoldOffDPP") ){
                stringstream ss(value);
                while (ss >> buf) param.push_back(buf);
                int gr = stoi( param.at(0) );
                int val= stoi( param.at(1) );
                if(gr < 8) m_TriggerHoldOffDPP[ gr ] = val;
            }
            
            else if( name == string("ClockDelay") 	)
                m_Delay = atoi( value.c_str() );
            
            else if( name == string("ClockSource")	)
                m_Clock = atoi( value.c_str() );
            
            else if( name == string("MaxEventsAggBLT")	)
                m_MaxEventsAggBLT = atoi( value.c_str() );
            
            else if( name == string("SynchronizationMode"))
                //FraMe disabled for test 
		m_RunSyncMode      = static_cast<CAEN_DGTZ_RunSyncMode_t>(atoi( value.c_str() ));
//                cout << "SynchronisationMode HardCoded !!!" << endl;
            
            else cout << "[ERROR] Parameter not recognized, line nr = " << lineNr << "\t[" << inSection << "] " << name << " : " << value << endl;
        }
    }
}

//==================================================================================
void DCAEN1740D::RegisterDump(){
    
    FILE *fout;
    fout = fopen("register_dump.txt", "w");
    
    //unsigned int read, i3;
    string readStr;
    
    uint32_t out, i, address;// ret;
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x1030 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle,  0x1030 + 0x100*i, &out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
        else fprintf(fout, "(0x%X) GateWidth\t0x%X\n", address, out);
    }
    
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x1034 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X)  GateOffset\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    }
    
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x1038 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X) GateBaseline\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    }
    
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x103C + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X)  PreTrigger\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout,"Errors during register dump.\n");
    }
    
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x1074 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X) TriggerHoldOff\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    }
    /*
     for(i=0; i< m_NrGroups; i++){
     uint32_t address = 0x1040 + 0x100*i;
     ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
     fprintf(fout, "(0x%X) PulsePolarity\t0x%X\n", address, out);
     if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
     }
     */
    
    /*
     for(i=0; i<8; i++){
     uint32_t address = 0x1078 + 0x100*i;
     ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
     fprintf(fout, "(0x%X)  ShapedTrigger\t0x%X\n", address, out);
     if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
     }
     */
    
    
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x1098 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X)  DCoffset\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    }
    
    for(i=0; i< m_NrGroups; i++){
        uint32_t address = 0x10A8 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X)  ChannelEnableMask\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    }
    
    
    for(i=0; i<m_NrChannels; i++){
        uint32_t a_gr = i / 8;
        uint32_t a_ch = i % 8;
        uint32_t address = 0x1000 + 0x100*a_gr + 0xD0 + 4*a_ch;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        fprintf(fout, "(0x%X) TriggerThreshold\t0x%X\n", address, out);
        if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    }
    
    address = 0x8000;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) Board Configuration\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0x800C;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) AggregateOrganization\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0x8020;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) EventsPerAggregate\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    
    /* Error in the firmware, this register cannot be red. check the new firmware if exist
     address = 0x8024;
     ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
     if(ret != CAEN_DGTZ_Success) {
     fprintf(fout, "(0x%X) Errors during register dump --------- ret(%d)\n", address, ret);
     }
     else fprintf(fout, "(0x%X) RecordLength\t0x%X\n", address, out);
     */
    
    for(i = 0; i < m_NrGroups; i++){
        address = 0x1040 + 0x100*i;
        ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
        if(ret != CAEN_DGTZ_Success)  fprintf(fout, "(0x%X) Errors during register dump --------- ret(%d)\n", address, ret);
        else {
            std::string s = Dec2BinStr2( (out & 0xFF000000) >>  24 ) + "'" +
            Dec2BinStr2( (out & 0x00FF0000) >>  16 ) + "'" +
            Dec2BinStr2( (out & 0x0000FF00) >>   8 ) + "'" +
            Dec2BinStr2( (out & 0x000000FF) >>   0 );
            fprintf(fout, "(0x%X) DPPControl gr%d\t0x%X %s\n",address, i, out, s.c_str());
        }
    }
    
    
    address = 0x8100;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) AcqControl\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0x810C;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) GlobalTriggerMask\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0x8120;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) GroupEnableMask\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0x814C;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) EventSize\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0xEF00;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) ReadoutControl\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    address = 0xEF1C;
    ret = CAEN_DGTZ_ReadRegister(m_Handle, address, &out);
    fprintf(fout, "(0x%X) AggregateNumber per BLT\t0x%X\n", address, out);
    if(ret != CAEN_DGTZ_Success) fprintf(fout, "Errors during register dump.\n");
    
    
    
    
    /*
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
     
     */
    fclose(fout);
    
}





