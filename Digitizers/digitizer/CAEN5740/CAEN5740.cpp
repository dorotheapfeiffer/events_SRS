/***************************************************************************F 
*
* Implementation class Spectrum
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <time.h>

#include <bitset>

#include <TMath.h>

#include "CAEN5740.h"
#include "AManager.h"
#include "AEvent.h"
#include "ATrack.h"

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

ClassImp(CAEN5740)

extern int gDEBUG_CAEN;

using namespace std;
Int_t CAEN5740::gDeviceNr = 0;
CAEN_DGTZ_ErrorCode ErrCode;

ULong_t CurrentTime, PrevRateTime, ElapsedTime;
Int_t nCycles= 0, Ne = 0, Nb = 0;

 Int_t preSize = 0;
static long get_time() {

 
    long time_ms;
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  
    return time_ms;
}



//static Int_t GetBit(Int_t bit, Int_t val){
//  if( (1 << bit) & val ) return 1;
//  else return 0;
//}

static char ErrMsg[100][100] {
 "Operation completed successfully",
 "Communication error",
 "Unspecified error",
 "Invalid parameter",
 "Invalid Link Type",
 "Invalid device handle",
 "Maximum number of devices exceeded",
 "The operation is not allowed on this type of board",
 "The interrupt level is not allowed",
 "The event number is bad",
 "Unable to read the registry",
 "Unable to write into the registry",
 "The channel number is invalid",
 "The Channel is busy",
 "Invalid FPIO Mode",
 "Wrong acquisition mode",
 "This function is not allowed for this module",
 "Communication Timeout",
 "The buffer is invalid",
 "The event is not found",
 "The vent is invalid",
 "Out of memory",
 "Unable to calibrate the board",
 "Unable to open the digitizer",
 "The Digitizer is already open",
 "The Digitizer is not ready to operate",
 "The Digitizer has not the IRQ configured",
 "The digitizer flash memory is corrupted",
 "The digitizer dpp firmware is not supported in this lib version",
 "Invalid Firmware License",
 "The digitizer is found in a corrupted status",
 "The given trace is not supported by the digitizer",
 "The given probe is not supported for the given digitizer's trace",
 "The function is not yet implemented",
};

//==================================================================

CAEN5740::CAEN5740() : testEvent(){
 #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Constructor] " << endl;
  #endif

 mGuiWindow = 0; 
 sprintf(mName, "CAEN5740.%d", gDeviceNr);
 mCardNr		= gDeviceNr;		
 mEnabled		= kTRUE;
 mFirstConfiguration	= kTRUE;
 mReconfig		= kTRUE;
 mStatus		= 0; 		
 mClock			= 0;
 mHandle		= -1;
 mNumEvents		= 10;	
 mBuffer		= NULL;
 mSize			= 0;	
 mBufferConfig		= 0; 
 mRecordLength		= 1024;
 mDecimationFactor 	= 1;
 mPostTrigger		= 50; 

 mSWTriggerMode		= 0;	
 mExtTriggerMode	= 0;
 mRunSyncMode		= 0;	
 mFPIOtype		= 0;
 mChannelPulsPolarity	= 0;
 mAcqMode		= 0;
 mSuppresionMode	= 0;
 mSaveChannel		= 0xFFFF;       


 for(Int_t i = 0; i < 4; i++){
     mTriggerEdge[i]	= 0;
     mThreshold_mV[i] 		= 0;
     mThreshold[i] 		= mV2ADC(mThreshold_mV[i]);
     mDCoffset_mV[i]		= 0;
     mDCoffset[i]		= mV2DAC(mDCoffset_mV[i]);
     mChannelTriggerMode[i]	= 1;
     mGroupEnableMask[i] 	= 0;
     }

for(Int_t i = 0; i < 32; i++) 
     mSelfTriggerMask[i]	= 0;

 Event16		= NULL;

 int LinkNum = gDeviceNr++;
 
 int ConetNode = 0;
 int BaseAddress = 0;


 cout << "OpenDigitizer: " << CAEN_DGTZ_USB << " " << LinkNum << " " <<  ConetNode << " " <<  BaseAddress << endl;
 ErrCode = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, LinkNum, ConetNode, BaseAddress, &mHandle);
 if(ErrCode){
   printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);
   throw  BadDigitizerCreation("communication error");
   }
 
 #ifdef DEBUG
    if(gDEBUG_CAEN > 2)
      cout << "DEBUG [CAEN5740::CAEN5740] mHandle = " << mHandle << endl;
  #endif

}

//==================================================================

CAEN5740::~CAEN5740() {
 #ifdef DEBUG
    if(gDEBUG_CAEN > 2)
      cout << "DEBUG [CAEN5740::Destructor]" << endl;
  #endif

 if(mHandle){
   ErrCode = CAEN_DGTZ_CloseDigitizer(mHandle);
   if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);

   mHandle = -1;
   }

}

//==================================================================

Int_t CAEN5740::Initialize() {
 #ifdef DEBUG
  if(gDEBUG_CAEN > 2){ cout << "DEBUG [CAEN5740::Initialization ]" << mName << endl; }
 #endif

 ErrCode = CAEN_DGTZ_GetInfo(mHandle, &BoardInfo);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);


 printf("DEBUG [Connected to CAEN Digitizer Model: %s\n", BoardInfo.ModelName);
 printf("DEBUG [Model:\t %d\n",		BoardInfo.Model);
 printf("DEBUG [Channels:\t %d\n",	BoardInfo.Channels);
 printf("DEBUG [FormFactor:\t %d\n",	BoardInfo.FormFactor);
 printf("DEBUG [FamilyCode:\t %d\n",	BoardInfo.FamilyCode);
 printf("DEBUG [ROC FPGA Release is: %s\n", BoardInfo.ROC_FirmwareRel);
 printf("DEBUG [AMC FPGA Release is: %s\n", BoardInfo.AMC_FirmwareRel);

 printf("DEBUG [SerialNumber:\t %d\n",	BoardInfo.SerialNumber);
 printf("DEBUG [PCB_Revision:\t %d\n",	BoardInfo.PCB_Revision);
 printf("DEBUG [ADC_NBits:\t %d\n",	BoardInfo.ADC_NBits);
 printf("DEBUG [CommHandle:\t %d\n",	BoardInfo.CommHandle);
 printf("DEBUG [License:\t %s\n",		BoardInfo.License);



 ErrCode = CAEN_DGTZ_Reset(mHandle);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);
 
return 0; 
 }

//==========================================================================================

void CAEN5740::StartAcq(){
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::StartAcq] " << mName << endl;
  #endif
 
 
 ErrCode = CAEN_DGTZ_SWStartAcquisition(mHandle);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);

}

//==========================================================================================

void CAEN5740::StopAcq(){
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::StopAcq] " << mName << endl;
  #endif
 mReconfig = kTRUE;
   ErrCode = CAEN_DGTZ_ClearData(mHandle);
 ErrCode = CAEN_DGTZ_SWStopAcquisition(mHandle);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);

}

//==========================================================================================

void CAEN5740::Close(){
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Close] " << mName << endl;
  #endif


   ErrCode = CAEN_DGTZ_SWStopAcquisition(mHandle);
   ErrCode = CAEN_DGTZ_FreeEvent(mHandle, (void**)&Event16);
   if(ErrCode) printf("ERROR FreeEvent [%s]\n", ErrMsg[0-ErrCode]);


   ErrCode = CAEN_DGTZ_FreeReadoutBuffer(&mBuffer);
   if(ErrCode) printf("ERROR FreeReadoutBuffer [%s]\n", ErrMsg[0-ErrCode]);
   ErrCode = CAEN_DGTZ_ClearData(mHandle);

   mFirstConfiguration = kTRUE;
   mReconfig = kTRUE;


}

//==========================================================================================

void CAEN5740::Reset() {
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Reset] " << mName << endl;
  #endif

 ErrCode = CAEN_DGTZ_Reset(mHandle);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);
}

//==========================================================================================

void CAEN5740::Refresh() {
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Refresh] " << mName << endl;
  #endif
/*
cout << "\t+---- General -----------"<< endl;
cout << "\t| mName\t\t\t= "         	       << mName 		<< endl  ;  
cout << "\t| mCardNr\t\t= "                    << mCardNr               <<  endl;
cout << "\t| mEnabled\t\t= "                   << mEnabled              <<  endl;
cout << "\t| mStatus\t\t= "                    << mStatus               <<  endl;
cout << "\t+---- Channel -----------"<< endl;
cout << "\t| mRecordLength\t\t= "            << mRecordLength         <<  endl;
cout << "\t| mPostTrigger\t\t= "             << mPostTrigger          <<  endl;
cout << "\t| mGroupEnableMask[0..3]= "  << mGroupEnableMask[0] <<", "<< mGroupEnableMask[1] 
                                  <<", "<< mGroupEnableMask[2] <<", "<< mGroupEnableMask[3] << endl;
cout << "\t| mDCoffset[0..3] mV\t= "  	<< mDCoffset_mV[0] <<", "<< mDCoffset_mV[1] <<", "<< mDCoffset_mV[2] <<", "<< mDCoffset_mV[3]  << endl;
cout << "\t| mDCoffset[0..3]\t= "  	<< mDCoffset[0] <<", "<< mDCoffset[1] <<", "<< mDCoffset[2] <<", "<< mDCoffset[3]  << endl;
cout << "\t+---- Trigger -----------"<< endl;
cout << "\t| mSWTriggerMode\t= "      << mSWTriggerMode        << endl;
cout << "\t| mExtTriggerMode\t= "     << mExtTriggerMode       << endl;
cout << "\t| mChnTriggerMode[0..3] = "<< mChannelTriggerMode[0]<<", "<< mChannelTriggerMode[1] 
                                <<", "<< mChannelTriggerMode[2] <<", "<< mChannelTriggerMode[3] << endl;
if(mFPIOtype) cout << "\t| mFPIOtype\t\t= "         << "TTL" <<  endl;
else          cout << "\t| mFPIOtype\t\t= "         << "ECL" <<  endl;

cout << "\t| mThreshold[0..3] mV\t= "    << mThreshold_mV[0] <<", "<< mThreshold_mV[1] <<", "<< mThreshold_mV[2] <<", "<< mThreshold_mV[3] << endl;
cout << "\t| mThreshold[0..3]\t= "    << mThreshold[0] <<", "<< mThreshold[1] <<", "<< mThreshold[2] <<", "<< mThreshold[3] << endl;
cout << "\t| mSelfTriggerMask[0..3]= "<< mSelfTriggerMask[0]   <<", "<< mSelfTriggerMask[1]    
                                <<", "<< mSelfTriggerMask[2]   <<", "<<mSelfTriggerMask[3]     << endl;
cout << "\t| mTriggerEdge\t\t= "           <<  std::bitset<32> (mTriggerEdge)    <<  endl;
cout << "\t| mChannelPulsPolarity\t= "       << bitset<32>(mChannelPulsPolarity)  <<  endl;
cout << "\t+----- Readout ----------"<< endl;
cout << "\t| mAcqMode\t\t= "                 << mAcqMode              <<  endl;
cout << "\t| mNumEvents\t\t= "               << mNumEvents            <<  endl;
cout << "\t| mBuffer\t\t= "            << &mBuffer        <<  endl;
cout << "\t| mSize\t\t\t= "            << mSize        <<  endl;
cout << "\t+----- Advanced ----------"<< endl;
cout << "\t| mClock\t\t= "                   << mClock                <<  endl;
cout << "\t| mRunSyncMode\t\t= "             << mRunSyncMode          <<  endl;
cout << "\t| mSuppresionMode\t= "            << mSuppresionMode       <<  endl;
cout << "\t+---------------"<< endl;
*/
}

//==========================================================================================

void CAEN5740::Configure() {
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Configure] " << mName << endl;
  #endif

  ofstream fout("recordlength.txt", std::ofstream::out | std::ofstream::app);
  fout << mRecordLength << endl;
  fout.close();
 UInt_t address = 0x0;

 
 if( !GetReconfigure() ){ 
/*
 if( MainFrame::sOSCI ){ 
    cout << "clear data in buffer" << endl; 
     ErrCode = CAEN_DGTZ_SWStopAcquisition(mHandle);
     CAEN_DGTZ_ClearData(mHandle);
     ErrCode = CAEN_DGTZ_SWStartAcquisition(mHandle);
    }
*/
    //cout << "return without configuration" << endl; 
    return; // if nothing changed in GUI return immediately
    }
 
 cout << "there will be re-configuration... "<< endl; 
 if( !mFirstConfiguration ){
   cout << "first configuration...  "<< endl; 
   mFirstConfiguration = kFALSE;
  
   ErrCode = CAEN_DGTZ_SWStopAcquisition(mHandle);
   if(ErrCode) printf("ERROR SWStopAcquisition [%s]\n", ErrMsg[0-ErrCode]);

   ErrCode = CAEN_DGTZ_FreeReadoutBuffer(&mBuffer);
   if(ErrCode) printf("ERROR FreeReadoutBuffer [%s]\n", ErrMsg[0-ErrCode]);

   ErrCode = CAEN_DGTZ_FreeEvent(mHandle, (void**)&Event16);
   if(ErrCode) printf("ERROR FreeEvent[%s]\n", ErrMsg[0-ErrCode]);

   ErrCode = CAEN_DGTZ_ClearData(mHandle);
   if(ErrCode) printf("ERROR ClearData [%s]\n", ErrMsg[0-ErrCode]);
   }
 else
   cout << "first configuration skip (skip deleting buffer and event  "<< endl; 

 mReconfig = kFALSE;

 ErrCode = CAEN_DGTZ_Reset(mHandle);
 if(ErrCode) printf("ERROR [%s], Unable to reset digitizer.\nPlease reset digitizer manually then restart the program\n", ErrMsg[0-ErrCode]);


 
//======= Configure channels =================
 
//-A configure Record Length
 UInt_t send = mRecordLength;
 ErrCode = CAEN_DGTZ_SetRecordLength(mHandle, mRecordLength);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetRecordLength [%s]\n", ErrMsg[0-ErrCode]);
 ErrCode = CAEN_DGTZ_GetRecordLength(mHandle, &mRecordLength);
 if(ErrCode) printf("ERROR [CAEN::Configure] GetRecordLength [%s]\n", ErrMsg[0-ErrCode]);
 if(mRecordLength != send) printf("ERROR Configuration, wrong RecordLength, send = %d, read = %d\n", send, mRecordLength); 

 //- A configure DecimationFactor (which is not in the documentation.... 
 send = mDecimationFactor;
 ErrCode = CAEN_DGTZ_SetDecimationFactor(mHandle, mDecimationFactor);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetDecimationFactor [%s]\n", ErrMsg[0-ErrCode]);
 ErrCode = CAEN_DGTZ_GetDecimationFactor(mHandle, &mDecimationFactor);
 if(ErrCode) printf("ERROR [CAEN::Configure] GetDecimationFactor [%s]\n", ErrMsg[0-ErrCode]);
 if(mDecimationFactor != send) printf("ERROR Configuration, wrong DecimationFactor, send = %d, read = %d\n", send, mDecimationFactor);

 //-A configure Post Trigger
 // This for some reason does not work!!! instead I use direct writing using WriteRegister
 // Should work if a decimationFactor is set between RecordLength and PostTrigger
  send = mPostTrigger;
  ErrCode = CAEN_DGTZ_SetPostTriggerSize(mHandle, mPostTrigger);
  if(ErrCode) printf("ERROR [CAEN::Configure] SetPostTriggerSize [%s]\n", ErrMsg[0-ErrCode]);
  ErrCode = CAEN_DGTZ_GetPostTriggerSize(mHandle, &mPostTrigger);
  if(ErrCode) printf("ERROR [CAEN::Configure] GetPostTriggerSize [%s]\n", ErrMsg[0-ErrCode]);
  if(mPostTrigger != send) printf("ERROR Configuration, wrong PostTrigger, send = %d, read = %d\n", send, mPostTrigger);

// writing using register
// send = mPostTrigger;
// address = 0x8114;  // this is address of posttrigger register
// ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send);
// if(ErrCode) printf("ERROR [CAEN::Configure] SetPostTriggerSize [%s]\n", ErrMsg[0-ErrCode]);
// ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &mPostTrigger);
// if(ErrCode) printf("ERROR [CAEN::Configure] GetPostTriggerSize [%s]\n", ErrMsg[0-ErrCode]);
// if(mPostTrigger != send) printf("ERROR Configuration, wrong PostTrigger, send = %d, read = %d\n", send, mPostTrigger);

//============== Configure I/O Panel ====================
 //-t Configure I/O Level (NIM or TTL)
 send = mFPIOtype;
 ErrCode = CAEN_DGTZ_SetIOLevel(mHandle, static_cast<CAEN_DGTZ_IOLevel_t>(mFPIOtype));
 if(ErrCode) printf("ERROR [CAEN::Configure] SetIOLevel [%s]\n", ErrMsg[0-ErrCode]);
 ErrCode = CAEN_DGTZ_GetIOLevel(mHandle, (CAEN_DGTZ_IOLevel_t*)&mFPIOtype);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetIOLevel [%s]\n", ErrMsg[0-ErrCode]);
 if(mFPIOtype != send) printf("ERROR Configuration, wrong FPIOtype\n");


 // Configure Max number of events
 if( MainFrame::sOSCI ) mNumEvents = 1;
 mNumEvents = 1000;
 ErrCode = CAEN_DGTZ_SetMaxNumEventsBLT(mHandle, mNumEvents);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetMaxNumEventsBTL [%s]\n", ErrMsg[0-ErrCode]);

 //-A Configure Acquisition mode
 send = mAcqMode;
 ErrCode = CAEN_DGTZ_SetAcquisitionMode(mHandle, static_cast<CAEN_DGTZ_AcqMode_t>(send));
 if(ErrCode) printf("ERROR [CAEN::Configure] SetAcquisitionMode [%s]\n", ErrMsg[0-ErrCode]);
 ErrCode = CAEN_DGTZ_GetAcquisitionMode(mHandle, (CAEN_DGTZ_AcqMode_t*)&mAcqMode);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetAcquisitionMode [%s]\n", ErrMsg[0-ErrCode]);
 if(send != mAcqMode) printf("ERROR Configuration, wrong AcquisitionMode, send = %d, read = %d\n", send, mAcqMode);
 

 //-t Configure External trigger input mode
 ErrCode = CAEN_DGTZ_SetExtTriggerInputMode(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mExtTriggerMode));
 //ErrCode = CAEN_DGTZ_SetExtTriggerInputMode(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(1));
 if(ErrCode) printf("ERROR [CAEN::Configure] SetExtTriggerInputMode [%s]\n", ErrMsg[0-ErrCode]);


//-A Configure group channels masks
 //UInt_t mask = ((mGroupEnableMask[3] << 3) | (mGroupEnableMask[2] << 2) | (mGroupEnableMask[1] << 1) | mGroupEnableMask[0] ) & 0xF;
 //send = mask;
 //cout << "Configure1: channel mask = " << mask << endl;
 //ErrCode = CAEN_DGTZ_SetGroupEnableMask(mHandle, mask);
 //if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupEnableMask [%s]\n", ErrMsg[0-ErrCode]);
 //if(mask != send) { printf("ERROR Configuration, wrong GroupEnableMask, MUST use direct register writing\n");
 // }
 
 UInt_t mask;
 
   mask = send = ((mGroupEnableMask[3] << 3) | (mGroupEnableMask[2] << 2) | (mGroupEnableMask[1] << 1) | mGroupEnableMask[0] ) & 0xF;
   address = 0x8120; 
   ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send);
   if(ErrCode) printf("ERROR [CAEN::Configure] Group Mask Enable [%s]\n", ErrMsg[0-ErrCode]);
   ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &mask);
   if(ErrCode) printf("ERROR [CAEN::Configure] Group Mask Enable [%s]\n", ErrMsg[0-ErrCode]);
   if(mask != send) printf("ERROR Configuration, wrong Write register to GroupEnalbeMask\n");
   //cout << "value from register GroupEnableMask = "<< bitset<32>(mask) << endl;

   //-t Configure Sowtware trigger input mode
   ErrCode = CAEN_DGTZ_SetSWTriggerMode(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mSWTriggerMode));
   if(ErrCode) printf("ERROR [CAEN::Configure] SetSWTriggerMode [%s]\n", ErrMsg[0-ErrCode]);


 for(Int_t i = 0; i < 4; i++){ 
    //if (mask & (1<<i)) {
    if(mGroupEnableMask[i]){      
       //-A Configure Offset for group channels masks
       ErrCode = CAEN_DGTZ_SetGroupDCOffset(mHandle, i, mDCoffset[i]);
       if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupDCOffset [%s]\n", ErrMsg[0-ErrCode]);
    
       //-t (1) Configure group self trigger Mode (trigger from channel)
       ErrCode = CAEN_DGTZ_SetGroupSelfTrigger(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mChannelTriggerMode[i]), (1<<i));
       if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupSelfTrigger [%s]\n", ErrMsg[0-ErrCode]);

       //-t Configure group trigger threshold
       ErrCode = CAEN_DGTZ_SetGroupTriggerThreshold(mHandle, i, mThreshold[i]);
       if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupTriggerThreshold [%s]\n", ErrMsg[0-ErrCode]);


       //-t Configure channel group mask (register writing)
       send = mSelfTriggerMask[i];
       address = 0x10A8 | (i << 8);  // this is address of posttrigger register
       ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send);
       if(ErrCode) printf("ERROR [CAEN::Configure] SetChannelGroupMask [%s]\n", ErrMsg[0-ErrCode]);
       ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &mSelfTriggerMask[i]);
       if(ErrCode) printf("ERROR [CAEN::Configure] GetChannelGroupMask [%s]\n", ErrMsg[0-ErrCode]);
       if(mSelfTriggerMask[i] != send) printf("ERROR Configuration, wrong ChannelGroupMask, send = %d, read = %d\n", send, mSelfTriggerMask[i]);

       }  
       
   }


 for(Int_t i = 0; i < 4; i++){ 
    //-t Configure trigger polarity
    ErrCode = CAEN_DGTZ_SetTriggerPolarity(mHandle, i, static_cast<CAEN_DGTZ_TriggerPolarity_t>(mTriggerEdge[i]));
    //ErrCode = CAEN_DGTZ_SetTriggerPolarity(mHandle, i, CAEN_DGTZ_TriggerOnFallingEdge);
    if(ErrCode) printf("ERROR [CAEN::Configure] SetTriggerEdge [%s]\n", ErrMsg[0-ErrCode]);
    ErrCode = CAEN_DGTZ_SetChannelPulsePolarity(mHandle, i, CAEN_DGTZ_PulsePolarityNegative);
    if(ErrCode) printf("ERROR [CAEN::Configure] SetChannelPulsePolarity ch = %d, [%s]\n", i, ErrMsg[0-ErrCode]);
    }

 //-t channel puls polarity
/*
    ret |= CAEN_DGTZ_SetRecordLength(mHandle, 1023);
    ret |= CAEN_DGTZ_SetDecimationFactor(mHandle, 1);
    ret |= CAEN_DGTZ_SetPostTriggerSize(mHandle, 20);
    ret |= CAEN_DGTZ_SetIOLevel(mHandle, CAEN_DGTZ_IOLevel_TTL);
    ret |= CAEN_DGTZ_SetMaxNumEventsBLT(mHandle, 1);
    ret |= CAEN_DGTZ_SetAcquisitionMode(mHandle, CAEN_DGTZ_SW_CONTROLLED);
// ************** This works with external trigger ***************************
    ret |= CAEN_DGTZ_SetExtTriggerInputMode(mHandle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
    UInt_t EnableMask = 1;
    ret |= CAEN_DGTZ_SetGroupEnableMask(mHandle, EnableMask);
  
   for(int i=0; i<4; i++) {
       if (EnableMask & (1<<i)) {
        ret |= CAEN_DGTZ_SetGroupDCOffset(mHandle, i, 0x8000);
        ret |= CAEN_DGTZ_SetGroupSelfTrigger(mHandle, CAEN_DGTZ_TRGMODE_DISABLED, i);
        ret |= CAEN_DGTZ_SetGroupTriggerThreshold(mHandle, i, 100);
        ret |= CAEN_DGTZ_SetChannelGroupMask(mHandle, i, 0);
        ret |= CAEN_DGTZ_SetTriggerPolarity(mHandle, i, CAEN_DGTZ_TriggerOnFallingEdge);
        }
      }
 // *****************************************************************************
 // ********* now channel trigger **********************************************
    ret |= CAEN_DGTZ_SetExtTriggerInputMode(mHandle, CAEN_DGTZ_TRGMODE_DISABLED);
    UInt_t EnableMask = 1;
    ret |= CAEN_DGTZ_SetGroupEnableMask(mHandle, EnableMask);
  
   for(int i=0; i<4; i++) {
       if (EnableMask & (1<<i)) {
        ret |= CAEN_DGTZ_SetGroupDCOffset(mHandle, i, 58981);
        ret |= CAEN_DGTZ_SetGroupSelfTrigger(mHandle, CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1<<i));
        ret |= CAEN_DGTZ_SetGroupTriggerThreshold(mHandle, i, 400);
        ret |= CAEN_DGTZ_SetChannelGroupMask(mHandle, i, 1);
        ret |= CAEN_DGTZ_SetTriggerPolarity(mHandle, i, CAEN_DGTZ_TriggerOnRisingEdge);
        }
      }
*/


 CAEN_DGTZ_AllocateEvent(mHandle, (void**)&Event16);
 CAEN_DGTZ_MallocReadoutBuffer(mHandle, &mBuffer, &mSize);


 RegisterDump(); 


 if(mGuiWindow) mGuiWindow->Refresh();

 Refresh();
 ErrCode = CAEN_DGTZ_SWStartAcquisition(mHandle);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);

}

//====================================================================================================

Int_t CAEN5740::ReadConfigFile(ifstream & fin ){
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::ReadConfigFile] " << endl;
  #endif

  string line;
  string name;
  string value;
  string inSection;
  int posEqual;
  int lineNr = 0;
  while (getline(fin,line)) {
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

    if( inSection == string(mName) ) {
       
       if(      name == string("InstrEnabled")		) {mEnabled		= (Bool_t)atoi( value.c_str());	}
       else if( name == string("IOLevel")		) mFPIOtype		= atoi( value.c_str()	); 
       else if( name == string("SuppresionMode")	) mSuppresionMode	= atoi( value.c_str()	); 
       else if( name == string("AcquisitionMode")	) mAcqMode		= atoi( value.c_str()	); 
       else if( name == string("BufferConfiguration")	) mBufferConfig		= atoi( value.c_str()	); 
       else if( name == string("ClockType")		) mClock		= atoi( value.c_str()	);
       else if( name == string("MaxEvents")		) mNumEvents		= atoi( value.c_str()	);
       else if( name == string("SynchronizationMode")	) mRunSyncMode		= atoi( value.c_str()	);
       else if( name == string("RecordLength")		) mRecordLength		= atoi( value.c_str()	); 
       else if( name == string("PostTrigger")		) mPostTrigger		= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr1")		) mGroupEnableMask[0]	= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr2")		) mGroupEnableMask[1]	= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr3")		) mGroupEnableMask[2]	= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr4")		) mGroupEnableMask[3]	= atoi( value.c_str()	);
       else if( name == string("DCOffsetGr1")		) { mDCoffset_mV[0]	= atoi( value.c_str()); mDCoffset[0] = mV2DAC(mDCoffset_mV[0]);}
       else if( name == string("DCOffsetGr2")		) { mDCoffset_mV[1]	= atoi( value.c_str()); mDCoffset[1] = mV2DAC(mDCoffset_mV[1]);}
       else if( name == string("DCOffsetGr3")		) { mDCoffset_mV[2]	= atoi( value.c_str()); mDCoffset[2] = mV2DAC(mDCoffset_mV[2]);}
       else if( name == string("DCOffsetGr4")		) { mDCoffset_mV[3]	= atoi( value.c_str()); mDCoffset[3] = mV2DAC(mDCoffset_mV[3]);}
       else if( name == string("SoftwareTriggerMode")	) mSWTriggerMode	= atoi( value.c_str()	); 
       else if( name == string("ExternalTriggerMode")	) mExtTriggerMode	= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr1") ) mChannelTriggerMode[0]= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr2") ) mChannelTriggerMode[1]= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr3") ) mChannelTriggerMode[2]= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr4") ) mChannelTriggerMode[3]= atoi( value.c_str()	); 
       else if( name == string("ChannelPulsPolarity")	) mChannelPulsPolarity	= atoi( value.c_str()	); 
       else if( name == string("TriggerPolarityGr1")	) mTriggerEdge[0]	= atoi( value.c_str()	); 
       else if( name == string("TriggerPolarityGr2")	) mTriggerEdge[1]	= atoi( value.c_str()	); 
       else if( name == string("TriggerPolarityGr3")	) mTriggerEdge[2]	= atoi( value.c_str()	); 
       else if( name == string("TriggerPolarityGr4")	) mTriggerEdge[3]	= atoi( value.c_str()	); 
       else if( name == string("SelfTriggerMaskGr1")	) mSelfTriggerMask[0]	= atoi( value.c_str()	); 
       else if( name == string("SelfTriggerMaskGr2")	) mSelfTriggerMask[1]	= atoi( value.c_str()	); 
       else if( name == string("SelfTriggerMaskGr3")	) mSelfTriggerMask[2]	= atoi( value.c_str()	); 
       else if( name == string("SelfTriggerMaskGr4")	) mSelfTriggerMask[3]	= atoi( value.c_str()	); 
       else if( name == string("ThresholdGr1")		) { mThreshold_mV[0]	= atoi( value.c_str());mThreshold[0] = mV2ADC(mThreshold_mV[0]);} 
       else if( name == string("ThresholdGr2")		) { mThreshold_mV[1]	= atoi( value.c_str());mThreshold[1] = mV2ADC(mThreshold_mV[1]);} 
       else if( name == string("ThresholdGr3")		) { mThreshold_mV[2]	= atoi( value.c_str());mThreshold[2] = mV2ADC(mThreshold_mV[2]);} 
       else if( name == string("ThresholdGr4")		) { mThreshold_mV[3]	= atoi( value.c_str());mThreshold[3] = mV2ADC(mThreshold_mV[3]);} 
       else cout << "Parameter not recognized, line nr = " << lineNr << "\t[" << inSection << "] " << name << " : " << value << endl;
       
       //cout << "Parameter line nr = " << lineNr << "\t" << inSection << "\tname: " << name << "\tvalue: " << value << endl;
     }  

   }
 
   for(UInt_t i = 0; i < 32; i++){
      if(mGroupEnableMask[i/8])
         mSaveChannel |= 1 << i;           // set bit to 1
      else
         mSaveChannel &= ~( 1 << i);       // set bit to 0
      } 
  cout << mGroupEnableMask[3] << mGroupEnableMask[2] << mGroupEnableMask[1] << mGroupEnableMask[0] << " " << std::bitset<32>(mSaveChannel) << endl; 
 
//cout << "DEBUG [CAEN5740::ReadConfiguration] Show configuration after reading file "<< endl;
//Refresh();

return 0;
}

//====================================================================================================

Int_t CAEN5740::WriteConfigFile(ofstream & fout){
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::WriteConfigFile] " << endl;
  #endif
 
 fout << "\n\n" << endl;
  
 fout << "[" << mName << "]" << endl;            
 fout << "# instrument can be enalbed  = 1" << endl; 
 fout << "#                or disabled = 0" << endl;
 fout << "InstrEnabled "	<< mEnabled		<<endl;          
 fout << "# All digitial inputs on the fron panel can be set as TTL = 1 or NIM = 1" << endl;
 fout << "IOLevel "		<< mFPIOtype       	<<endl; 
 fout << "# DO NOT USE IT" << endl;
 fout << "SuppresionMode " 	<< mSuppresionMode 	<<endl;
 fout << endl; 

 fout << "# Acquisition Mode could be software controled = 0; or by signal from the front panel = 1" << endl;
 fout << "AcquisitionMode "	<< mAcqMode		<<endl;
 fout << "# DO NOT USE IT" << endl;
 fout << "BufferConfiguration "<< mBufferConfig   	<<endl; 
 fout << "# you can change clock setings but I do not know how and what for...." << endl;
 fout << "ClockType "		<< mClock		<<endl;           
 fout << "# Define maximum parameter of how many events can be read from instrument" << endl;
 fout << "MaxEvents "	<< mNumEvents    	<<endl;   
 fout << "# DO not use it" << endl;
 fout << "SynchronizationMode "<< mRunSyncMode   	<<endl; 
 fout << "# " << endl;
 fout << endl; 
 

 fout << "# Channel Configuration"<< endl;
 fout << "# You can difine the track lengt, is the same for all channels" << endl;
 fout << "RecordLength "	<< mRecordLength   	<<endl; 
 fout << "# you can choose how many samples are stored before and after triggeer appear,  the value are between 0, 100 in %" << endl;
 fout << "PostTrigger "	<< mPostTrigger    	<<endl; 
 fout << "# disable = 0 or enable = 1 group channels" << endl;
 fout << "EnableMaskGr1 "	<< mGroupEnableMask[0]	<<endl; 
 fout << "EnableMaskGr2 "	<< mGroupEnableMask[1]	<<endl; 
 fout << "EnableMaskGr3 "	<< mGroupEnableMask[2]	<<endl; 
 fout << "EnableMaskGr4 "	<< mGroupEnableMask[3]	<<endl; 
 fout << "# Set offet for group channels " << endl;
 fout << "# DCOffsetGrN: DC offset adjust (DAC channel setting) in mV." << endl;
 fout << "# -1000: analog input dynamic range = -Vpp to 0 (negative signals)" << endl;
 fout << "# +1000: analog input dynamic range = 0 to +Vpp (positive signals)" << endl;
 fout << "#     0:   analog input dynamic range = -Vpp/2 to +Vpp/2 (bipolar signals)" << endl;
 fout << "DCOffsetGr1 "	<<  mDCoffset_mV[0]   	<< endl; 
 fout << "DCOffsetGr2 "	<<  mDCoffset_mV[1]   	<< endl; 
 fout << "DCOffsetGr3 "	<<  mDCoffset_mV[2]   	<< endl; 
 fout << "DCOffsetGr4 "	<<  mDCoffset_mV[3]   	<< endl; 
 fout << endl; 
 
 fout << "# Trigger Configuration" << endl;
 fout << "# Enable or disable software, external and channel trigger" << endl;
 fout << "# the option could be 0, 1, 2, 3, disable, enable acq only, enable ext only, both "<< endl;
 fout << "SoftwareTriggerMode " << mSWTriggerMode   <<endl; 
 fout << "ExternalTriggerMode "<< mExtTriggerMode 	<<endl; 
 fout << "# " << endl;
 fout << "ChannelTriggerModeGr1 "<< mChannelTriggerMode[0]	<<endl;
 fout << "ChannelTriggerModeGr2 "<< mChannelTriggerMode[1]	<<endl;
 fout << "ChannelTriggerModeGr3 "<< mChannelTriggerMode[2]	<<endl;
 fout << "ChannelTriggerModeGr4 "<< mChannelTriggerMode[3]	<<endl;
 fout << endl; 

 fout << "# Trigger Channel Configuration" << endl;
 fout << "ChannelPulsPolarity "<< mChannelPulsPolarity	<<endl; 
 fout << "TriggerPolarity "	<< mTriggerEdge	<<endl; 
 fout << "SelfTriggerMaskGr1 "<< mSelfTriggerMask[0]	<<endl; 
 fout << "SelfTriggerMaskGr2 "<< mSelfTriggerMask[1]	<<endl; 
 fout << "SelfTriggerMaskGr3 "<< mSelfTriggerMask[2]	<<endl; 
 fout << "SelfTriggerMaskGr4 "<< mSelfTriggerMask[3]	<<endl; 
 fout << "# Adjust the threshold to the coresponding channel in mv" << endl;
 fout << "# Treshold value is an absolut value, consider it when you add offset to the channel" << endl;
 fout << "# Possible values between -2000 and 2000 mv" << endl;
 fout << "ThresholdGr1 "	<<  mThreshold_mV[0] << endl; 
 fout << "ThresholdGr2 "	<<  mThreshold_mV[1] << endl; 
 fout << "ThresholdGr3 "	<<  mThreshold_mV[2] << endl; 
 fout << "ThresholdGr4 "	<<  mThreshold_mV[3] << endl; 
 fout << endl; 

return 0;
}


//====================================================================================================
UInt_t CAEN5740::GetData(){

  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::GetData] " << endl;
  #endif

 UInt_t lstatus1, lstatus2;
 UInt_t aNrEvents = 0;
 Char_t *EventPtr = NULL;
 Bool_t trg = 0;
// Event16 = NULL;

 for(vector<AEvent *>::iterator it = mEvents.begin(); it != mEvents.end(); ++it)
     delete (*it);
 mEvents.clear();

  if(trg){
        //CAEN_DGTZ_SendSWtrigger(mHandle);
        trg = 0;
    }

 mSize = 0;
 
 ErrCode = CAEN_DGTZ_ReadData(mHandle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, mBuffer, &mSize);
 if(ErrCode) printf("ERROR [CAEN::GetData] ReadData [%s]\n", ErrMsg[0-ErrCode]); 

 if (mSize != 0) {
    ErrCode = CAEN_DGTZ_GetNumEvents(mHandle, mBuffer, mSize, &aNrEvents);
    if(ErrCode) printf("ERROR [CAEN::GetData] GetNumEvents [%s]\n", ErrMsg[0-ErrCode]);
    }
 else{
   //cout << " size of readout data in buffer = " << mSize << endl; 
   ErrCode = CAEN_DGTZ_ReadRegister(mHandle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus1);
   if(ErrCode) printf("ERROR [CAEN::GetData] ReadRegister Acquisition Status [%s]\n", ErrMsg[0-ErrCode]);
   //cout << "Acquisition Status = " << bitset<32>(lstatus1) << endl;
   ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8100, &lstatus2);
   if(ErrCode) printf("ERROR [CAEN::GetData] ReadRegister Acquisition Control [%s]\n", ErrMsg[0-ErrCode]);
   //cout << "Acquisition Control = " << bitset<32>(lstatus2) << endl;
 }

  //cout << "---------- Nr of Events from CAEN5740.cpp = " << aNrEvents << " mSize = " << mSize << endl;

  Nb += mSize;
  Ne += aNrEvents;
  CurrentTime = get_time();
  ElapsedTime = CurrentTime - PrevRateTime;

  nCycles++;
  if (ElapsedTime > 1000) {
     if(Nb == 0){
        cout << "No data... Send SW Trigger" << endl;
        trg = 1;
        //cout << "Acquisition Status = " << bitset<32>(lstatus1) << endl;
        //cout << "Acquisition Control = " << bitset<32>(lstatus2) << endl;
        }
     else
        printf("Reading at %.2f MB/s (Trg Rate: %.2f Hz)\n", (float)Nb/((float)ElapsedTime*1048.576f), (float)Ne*1000.0f/(float)ElapsedTime);
  nCycles= 0;
  Nb = 0;
  Ne = 0;
  PrevRateTime = CurrentTime;
  }

/* 
 //cout << "DEBUG [CAEN::GetData] GetNumEvents = " << aNrEvents << endl;
 CAEN_DGTZ_GetEventInfo(mHandle, mBuffer, mSize, aNrEvents-1, &EventInfo, &EventPtr);
 CAEN_DGTZ_DecodeEvent(mHandle, EventPtr, (void**)&Event16);
 AEvent *aSingleEvent = new AEvent(); 
 for(Int_t j2 = 0; j2 < 32; j2++){   
    if( ((mSaveChannel >> j2) & 1) && ((EventInfo.ChannelMask >> j2/8 )& 1)) { 
       aSingleEvent->AddTrack(new ATrack(mCardNr, j2, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[j2], 
                               (Short_t*)Event16->DataChannel[j2], Event16->ChSize[j2]));
       } 
    }
 aSingleEvent->SetEventNr(EventInfo.EventCounter);
 mEvents.push_back(aSingleEvent);
*/   


 for(Int_t i = 0; i < (Int_t)aNrEvents; i++){
    CAEN_DGTZ_GetEventInfo(mHandle, mBuffer, mSize, i, &EventInfo, &EventPtr);
    CAEN_DGTZ_DecodeEvent(mHandle, EventPtr, (void**)&Event16);
     
    AEvent *aSingleEvent = new AEvent(); 
    for(Int_t j2 = 0; j2 < 32; j2++){   
       if( ((mSaveChannel >> j2) & 1) && ((EventInfo.ChannelMask >> j2/8 )& 1)) { 
          //cout << std::bitset<32>(mSaveChannel) << " " << EventInfo.ChannelMask << endl;
          aSingleEvent->AddTrack(new ATrack(mCardNr, j2, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[j2], 
                               (Short_t*)Event16->DataChannel[j2], Event16->ChSize[j2]));
         } 
       }
    aSingleEvent->SetEventNr(EventInfo.EventCounter);
    mEvents.push_back(aSingleEvent);
   }
   


/*
 for(Int_t i = 0; i < (Int_t)aNrEvents; i++){
     //cout << "DEBUG [CAEN::GetData] GetEventInfo = " << i << endl;
     ErrCode = CAEN_DGTZ_GetEventInfo(mHandle, mBuffer, mSize, i, &EventInfo, &EventPtr);
     if(ErrCode) printf("ERROR [CAEN::GetData] GetEventInfo [%s]\n", ErrMsg[0-ErrCode]);

     //cout << "---------- Event Size   ------ = " << EventInfo.EventSize << endl;
     //cout << "---------- Board ID     ------ = " << EventInfo.BoardId   << endl;
     //cout << "---------- Pattern      ------ = " << EventInfo.Pattern   << endl;
     //cout << "---------- Channel Mask ------ = " << EventInfo.ChannelMask << endl;
     //cout << "---------- Event Counter------ = " << EventInfo.EventCounter << endl;
     //cout << "---------- Trigger Time ------ = " << EventInfo.TriggerTimeTag << endl;

     //cout << "DEBUG [CAEN::GetData] DecodeEvent  = " << i << endl;
     ErrCode = CAEN_DGTZ_DecodeEvent(mHandle, EventPtr, (void**)&Event16);
     if(ErrCode) printf("ERROR [CAEN::GetData] DecodeEvent [%s]\n", ErrMsg[0-ErrCode]);
    
     AEvent *aSingleEvent = new AEvent(); 
     for(Int_t j2 = 0; j2 < 32; j2++){    // 32 channels in 4 groups this should be changed and adapted to the settings
        Int_t size = j2 / 8;             // 8 channels per group     --||--          --||--          --||--
        //Int_t group = ( (EventInfo.ChannelMask >> size) & 0x1);
        //cout << "RecordLength = " << mRecordLength << "\tfrom stream = " << Event16->ChSize[j2] << endl;  
        //cout << "EventInfo.ChannelMask = " << EventInfo.ChannelMask << "\tgroup = " << size << "\tchannel = " << j2 << endl;
        //if(group) continue;
           
        //cout << "Obrabiam channel nr " << j2 << ", channel size = " << Event16->ChSize[j2] << endl;
        
        Short_t aValue_mV[Event16->ChSize[j2]];
        memcpy(aValue_mV, Event16->DataChannel[j2], Event16->ChSize[j2] * 2);



        for(UInt_t k = 0; k < Event16->ChSize[j2]; k++){
           if(j2 == 0) cout << j2 << " " << aValue_mV[k] << endl;
           //if(j2 == 0) cout << size << " " << j2 << " " << aValue_mV[k] << " " << EventInfo.EventCounter << endl;
           }
        
        aSingleEvent->AddTrack(new ATrack(mCardNr, j2, 1. / 65e6, 0, 1000, aValue_mV, Event16->ChSize[j2]));
        }
        aSingleEvent->SetEventNr(EventInfo.EventCounter);
        mEvents.push_back(aSingleEvent);

        //cout << "DEBUG [CAEN::GetData] after filling mEvents.size = " << mEvents.size() << " EventNr = " << EventInfo.EventCounter << endl;
    

  //      ErrCode = CAEN_DGTZ_FreeEvent(mHandle, (void**)&Event16);
  //      Event16 = NULL;
   }
*/
/*
 ofstream fout("testCAEN5740.txt", ios::out);

 //for(std::list<AEvent*>::iterator it = mEvents.begin(); it != mEvents.end(); ++it){
 //   (*it)->Write2F(fout);
 //   (*it)->EventShow();
 //   }

 //for(Int_t i = 0; i < 32; i++) {
 //   for(Int_t j = 0; j < Event16->ChSize[i]; j++){
 //      fout << j << " " << Event16->DataChannel[i][j] << endl;
 //      }
 //  }

 fout.close();
*/



return aNrEvents;
}

//===========================================================================
void CAEN5740::SendSWTrigger(){

}

//===========================================================================

void CAEN5740::RegisterDump(){
FILE *fout;
fout = fopen("register_dump.txt", "w");

fprintf(fout, "=== configuration settings ===\n");

 unsigned int read, i3;

 fprintf(fout, "RecordLength       = %d\n", mRecordLength);
 fprintf(fout, "DecimationFactor   = %d\n", mDecimationFactor );
 fprintf(fout, "PostTrigger        = %d\n", mPostTrigger);
 fprintf(fout, "FPIOtype           = %d\n", mFPIOtype);
 fprintf(fout, "NumEvents          = %d\n", mNumEvents);
 fprintf(fout, "Software controled = %d\n",CAEN_DGTZ_SW_CONTROLLED);
 fprintf(fout, "ExtTriggerMode     = %d\n", mExtTriggerMode);
 UInt_t mask = ((mGroupEnableMask[3] << 3) | (mGroupEnableMask[2] << 2) | (mGroupEnableMask[1] << 1) | mGroupEnableMask[0] ) & 0xF;
 fprintf(fout, "EnableMask         = %d\n", mask);
 for(i3=0; i3 < 4; i3++) {
    if (mask & (1<<i3)) {
       fprintf(fout, "...... Group %d\n", i3);
       fprintf(fout, "DCoffset[%d]          = %d\n", i3, mDCoffset[i3]);
       fprintf(fout, "ChannelTriggerMode[%d]= %d\n", i3, mChannelTriggerMode[i3]);
       fprintf(fout, "Threshold[%d]         = %d\n", i3, mThreshold[i3]);
       fprintf(fout, "GroupTrgEnableMask[%d]= %d\n", i3, mSelfTriggerMask[i3]);
       fprintf(fout, "TriggerEdge          = %d\n",      mTriggerEdge[i3]);
       }
    }

  
 fprintf(fout, "\n=== Register readout ===\n");
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x1080, &read); //threshold
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Group 0 Threshold\t= 0x%x (%d)\n", read, read);
 
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x1088, &read); //status
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Group 0 Status\t= 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x1098, &read); //DAC
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Group 0 DAC\t= 0x%x (%d)\n", read, read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x109c, &read); //ADC configuration
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Groupp 0 ADC Configuration = 0x%x (%d)\n", read, read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x10A8, &read); //n channel trigger enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Group 0 Channel Trigger enalbe mask = 0x%x \n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8000, &read); //channel configuration
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Channel configuration = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x800C, &read); //Buffer organization
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Buffer Organization = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8020, &read); //custom size
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "custom size = 0x%x (%d)\n", read, read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8100, &read); //acq control
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "acquisition control = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8104, &read); //acq status
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "acquisition status = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x810C, &read); //trigger source enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Trigger source enalbe mask = 0x%x\n", read);
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8110, &read); //front panel trigger out enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "front panel Trigger out enalbe mask = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8114, &read); //post trigger
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Post trigger = %d\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x811C, &read); //front panel I/O
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Front panel I/O = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8120, &read); //channel enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Channel enalbe mask = 0x%x\n", read);

 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x812C, &read); //event stored
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 fprintf(fout, "Event stored = %d\n", read);


fclose(fout);

}













