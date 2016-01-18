/***************************************************************************F 
*
* Implementation class Spectrum
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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

static string Dec2BinStr(Char_t dec){
string strBin;
 //for(UInt_t i = 0x80; i > 0; i >>= 1){
 for(UInt_t i = 0x1; i <= 0x80; i <<= 1){
    if(i & dec) strBin += '1';
    else        strBin += '0';
    }

return strBin;
}

static string Dec2BinStr2(Char_t dec){
string strBin;
 for(UInt_t i = 0x80; i > 0; i >>= 1){
    if(i & dec) strBin += '1';
    else        strBin += '0';
    }

return strBin;
}


//static Int_t GetBit(Int_t bit, Int_t val){
//  if( (1 << bit) & val ) return 1;
//  else return 0;
//}

static char ErrMsg[100][100] = {
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

CAEN5740::CAEN5740(Int_t) : testEvent(){
 #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Constructor] " << endl;
  #endif

 mGuiWindow = 0; 
 //sprintf(mName, "CAEN5740.%d", gDeviceNr);
 strcpy(mName, "CAEN5740");
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
 mFPIOtype		= 1;
 mChannelPulsPolarity	= 0;
 mAcqMode		= 0;
 mSuppresionMode	= 0;
 mSaveChannel		= 0x1;       
 mDelay			= 0;       

 mCoincidenceWindow	= 0;
 mTriggerCoincidenceLevel = 0;
 mTriggerOutLogic	= 0;
 mMajorityLevel		= 0;


 for(Int_t i = 0; i < 4; i++){
     mTriggerEdge[i]	= 0;
     mThreshold_mV[i] 		= 0;
     mThreshold[i] 		= mV2ADC(mThreshold_mV[i]);
     mDCoffset_mV[i]		= 0;
     mDCoffset[i]		= mV2DAC(mDCoffset_mV[i]);
     mChannelTriggerMode[i]	= 1;
     if(i) mGroupEnableMask[i] 	= 0;
     else  mGroupEnableMask[i] 	= 1;
     }

for(Int_t i = 0; i < 32; i++) 
     mSelfTriggerMask[i]	= 0;

     mSelfTriggerMask[0]	= 1;

 Event16		= NULL;

 int LinkNum = gDeviceNr++;
 
 int ConetNode = 0;
 int BaseAddress = 0;


 cout << "OpenDigitizer: " << CAEN_DGTZ_USB << " " << LinkNum << " " <<  ConetNode << " " <<  BaseAddress << endl;
 ErrCode = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, LinkNum, ConetNode, BaseAddress, &mHandle);
 if(ErrCode){
   printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);
   string str = "CAEN5740 LinkNum:" + std::to_string(LinkNum) + " ConnetNode:" + std::to_string(ConetNode) + " BaseAddress:" + std::to_string(BaseAddress);
   throw  BadDigitizerCreation(str);
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

 
 sprintf(mName + strlen(mName), ".%d", BoardInfo.SerialNumber);

 ErrCode = CAEN_DGTZ_Reset(mHandle);
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);
 
return 0; 
 }

//==========================================================================================

void CAEN5740::StartAcq(){
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::StartAcq] " << mName << endl;
  #endif

 // if (mAcqMode == 0){
 //    CAEN_DGTZ_WriteRegister(mHandle, 0x8100 , 0xE); 
     ErrCode = CAEN_DGTZ_SWStartAcquisition(mHandle);
 //    CAEN_DGTZ_WriteRegister(mHandle, 0x817C , 0); // Enable TRGIN of the first board
 //   }
  RegisterDump(); 

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
  Reset();
  cout << "DEBUG [CAEN5740::StopAcq] " << mName << endl;
  if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);

}

//===========================================================================
void CAEN5740::SendSWTrigger(){

  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::StartAcq] " << mName << endl;
  #endif
 
  ErrCode = CAEN_DGTZ_SendSWtrigger(mHandle);
  //ErrCode = CAEN_DGTZ_SWStartAcquisition(mHandle);
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

//for(Int_t i = 0; i < 100; i++) cout << endl;

AManager *aManager = &AManager::GetInstance();

cout << "\t+---- General -----------"<< endl;
cout << "\t| Name\t\t\t= "        << mName 		      << endl  ;  
cout << "\t| CardNr\t\t= "        << mCardNr                  <<  endl;
cout << "\t| Enabled\t\t= "       << mEnabled                 <<  endl;
cout << "\t| Status\t\t= "        << mStatus                  <<  endl;
cout << "\t| SoftTimeOut\t\t= "   << aManager->GetTimeout()   <<  endl;
cout << "\t| MaxEvent\t\t= "      << aManager->GetMaxEvents() <<  endl;
cout << "\t| MaxFiles\t\t= "      << aManager->GetMaxFiles()  <<  endl;
cout << "\t| FileSize\t\t= "      << aManager->GetFileSize()  <<  endl;
cout << "\t| FileTime\t\t= "      << aManager->GetFileTime()  <<  endl;
cout << "\t+---- Channels -----------"<< endl;
cout << "\t+- Samples ---------- Group --- Offset/Offset[mV] ------ Save ----------"<< endl;
cout << "\t| mLength =\t"  << mRecordLength << "\t" << mGroupEnableMask[0] << "\t" << mDCoffset[0] << " / " << mDCoffset_mV[0] << "\t\t"   
	<< Dec2BinStr(mSaveChannel) << endl;
                                //                                << bitset<8>(Char_t(mSaveChannel >> 0)) << endl;
cout << "\t| mPostTrg =\t" << mPostTrigger  << "\t" << mGroupEnableMask[1] << "\t" << mDCoffset[1] << " / " << mDCoffset_mV[1] << "\t\t"   
	<< Dec2BinStr(Char_t(mSaveChannel >> 8)) << endl;
                                              //                  << bitset<8>(Char_t(mSaveChannel >> 8)) << endl;
cout << "\t| \t\t\t" << mGroupEnableMask[2] << "\t" << mDCoffset[2] << " / " << mDCoffset_mV[2] << "\t\t"  
	<< Dec2BinStr(Char_t(mSaveChannel >> 16)) << endl;
                                                //                << bitset<8>(Char_t(mSaveChannel >> 16)) << endl;
cout << "\t| \t\t\t" << mGroupEnableMask[3] << "\t" << mDCoffset[3] << " / " << mDCoffset_mV[3] << "\t\t"   
	<< Dec2BinStr(Char_t(mSaveChannel >> 24)) << endl;
                                                  //              << bitset<8>(Char_t(mSaveChannel >> 24)) << endl;
cout << "\t+---- Triggers -----------"<< endl;
cout << "\t+- Mode -- Threshold / Threshold[mV] - Slope --- TrigerMask -"<< endl;
cout << "\t| Gr1 " << mChannelTriggerMode[0] <<"\t\t"<< mThreshold[0] <<" / "<< mThreshold_mV[0] <<"\t\t"<< std::bitset<1> (mTriggerEdge[0]) <<"\t"
		<< mSelfTriggerMask[0] << mSelfTriggerMask[1] << mSelfTriggerMask[2] << mSelfTriggerMask[3] 
		<< mSelfTriggerMask[4] << mSelfTriggerMask[5] << mSelfTriggerMask[6] << mSelfTriggerMask[7] << endl;
cout << "\t| Gr2 " << mChannelTriggerMode[1] <<"\t\t"<< mThreshold[1] <<" / "<< mThreshold_mV[1] <<"\t\t"<< std::bitset<1> (mTriggerEdge[1]) <<"\t"
		<< mSelfTriggerMask[8] << mSelfTriggerMask[9] << mSelfTriggerMask[10] << mSelfTriggerMask[11] 
		<< mSelfTriggerMask[12] << mSelfTriggerMask[13] << mSelfTriggerMask[14] << mSelfTriggerMask[15] << endl;
cout << "\t| Gr3 " << mChannelTriggerMode[2] <<"\t\t"<< mThreshold[2] <<" / "<< mThreshold_mV[2] <<"\t\t"<< std::bitset<1> (mTriggerEdge[2]) <<"\t"
		<< mSelfTriggerMask[16] << mSelfTriggerMask[17] << mSelfTriggerMask[18] << mSelfTriggerMask[19] 
		<< mSelfTriggerMask[20] << mSelfTriggerMask[21] << mSelfTriggerMask[22] << mSelfTriggerMask[23] << endl;
cout << "\t| Gr4 " << mChannelTriggerMode[3] <<"\t\t"<< mThreshold[3] <<" / "<< mThreshold_mV[3] <<"\t\t"<< std::bitset<1> (mTriggerEdge[3]) <<"\t"
		<< mSelfTriggerMask[24] << mSelfTriggerMask[25] << mSelfTriggerMask[26] << mSelfTriggerMask[27] 
		<< mSelfTriggerMask[28] << mSelfTriggerMask[29] << mSelfTriggerMask[30] << mSelfTriggerMask[31] << endl;
cout << "\t| Ext " << mExtTriggerMode << endl;
if(mFPIOtype) cout << "\t| mFPIOtype = "         << "TTL" ;
else          cout << "\t| mFPIOtype = "         << "NIM" ;
if(mAcqMode) cout << "\tAcqMode = Hardware" << "\tDelay = " << mDelay;
else         cout << "\tAcqMode = Software" << "\tDelay = " << mDelay;
cout << endl;
/*
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

Int_t CAEN5740::Configure() {
  #ifdef DEBUG
    if(gDEBUG_CAEN > 2) cout << "DEBUG [CAEN5740::Configure] " << mName << endl;
  #endif

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
    return 0; // if nothing changed in GUI return immediately
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
 mNumEvents = 2;
 ErrCode = CAEN_DGTZ_SetMaxNumEventsBLT(mHandle, mNumEvents);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetMaxNumEventsBTL [%s]\n", ErrMsg[0-ErrCode]);

 //-A Configure Acquisition mode
 send = mAcqMode;
 ErrCode = CAEN_DGTZ_SetAcquisitionMode(mHandle, static_cast<CAEN_DGTZ_AcqMode_t>(send));
 if(ErrCode) printf("ERROR [CAEN::Configure] SetAcquisitionMode [%s]\n", ErrMsg[0-ErrCode]);
 ErrCode = CAEN_DGTZ_GetAcquisitionMode(mHandle, (CAEN_DGTZ_AcqMode_t*)&mAcqMode);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetAcquisitionMode [%s]\n", ErrMsg[0-ErrCode]);
 if(send != mAcqMode) printf("ERROR Configuration, wrong AcquisitionMode, send = %d, read = %d\n", send, mAcqMode);
 
 //-A Configure Delay in daisy chain
 send = mDelay;
 ErrCode = CAEN_DGTZ_WriteRegister(mHandle, 0x8170, send);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetAcquisitionMode [%s]\n", ErrMsg[0-ErrCode]);
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8170, &mDelay);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetAcquisitionMode [%s]\n", ErrMsg[0-ErrCode]);
 if(send != mDelay) printf("ERROR Configuration, wrong AcquisitionMode, send = %d, read = %d\n", send, mDelay);
 cout << "mDelay = "<< mDelay << endl; 


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

 for(Int_t i = 0; i < 4; i++){ 
    if(mGroupEnableMask[i]){      
       //-A Configure Offset for group channels masks
       ErrCode = CAEN_DGTZ_SetGroupDCOffset(mHandle, i, mDCoffset[i]);
       if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupDCOffset [%s]\n", ErrMsg[0-ErrCode]);
      }
    }

/*
 //-t Configure Sowtware trigger input mode
 ErrCode = CAEN_DGTZ_SetSWTriggerMode(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mSWTriggerMode));
 if(ErrCode) printf("ERROR [CAEN::Configure] SetSWTriggerMode [%s]\n", ErrMsg[0-ErrCode]);

 //-t Configure External trigger input mode
 ErrCode = CAEN_DGTZ_SetExtTriggerInputMode(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mExtTriggerMode));
 //ErrCode = CAEN_DGTZ_SetExtTriggerInputMode(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(1));
 if(ErrCode) printf("ERROR [CAEN::Configure] SetExtTriggerInputMode [%s]\n", ErrMsg[0-ErrCode]);

 //-t (1) Configure group self trigger Mode (trigger from channel)
 ErrCode = CAEN_DGTZ_SetGroupSelfTrigger(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mChannelTriggerMode[0]), 0);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupSelfTrigger [%s]\n", ErrMsg[0-ErrCode]);

 //-t (1) Configure group self trigger Mode (trigger from channel)
 ErrCode = CAEN_DGTZ_SetGroupSelfTrigger(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mChannelTriggerMode[1]), 1);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupSelfTrigger [%s]\n", ErrMsg[0-ErrCode]);

 //-t (1) Configure group self trigger Mode (trigger from channel)
 ErrCode = CAEN_DGTZ_SetGroupSelfTrigger(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mChannelTriggerMode[2]), 2);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupSelfTrigger [%s]\n", ErrMsg[0-ErrCode]);

 //-t (1) Configure group self trigger Mode (trigger from channel)
 ErrCode = CAEN_DGTZ_SetGroupSelfTrigger(mHandle, static_cast<CAEN_DGTZ_TriggerMode_t>(mChannelTriggerMode[3]), 3);
 if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupSelfTrigger [%s]\n", ErrMsg[0-ErrCode]);
*/
 UInt_t mTriggerInEnableMask = 0, mTriggerOutEnableMask = 0;
 
  // Configuring trigger is a bit tricky.... The CAEN library has errors, maybe it is fixed in the new version
  // anthow I decided to use direct register writing instead of using library.
  // One has to consider 3 different registers: 
  // 1. Trigger Source Enable Mask Register (0x810C)
  // 2. Setting Trigger Output Enable Mask Register (0x8110)
  // 3. Setting n Channel Trigger Mask Register for every n!  (0x1nA8) where n is a number of group

   UInt_t send1, send2, send3[4];
   UInt_t read;
   // Setting Trigger Source Enable Mask Register
   address = 0x810C; 
   
   if( mSWTriggerMode  == 1 || mSWTriggerMode  == 3 ) send1 = 1; else send1 = 0;
   if( mExtTriggerMode == 1 || mExtTriggerMode == 3 ) send2 = 1; else send2 = 0;
   for(Int_t i = 0; i < 4; i++){
      if( mChannelTriggerMode[i] == 1 || mChannelTriggerMode[i] == 3 ) send3[i] = 1; else send3[i] = 0;
      }
   mCoincidenceWindow &= 0xF;
   mTriggerCoincidenceLevel &= 0x7;

   send = ((send1 << 31) | (send2 << 30) | (mTriggerCoincidenceLevel << 24) | (mCoincidenceWindow << 20) |
          (send3[3] << 3) | (send3[2] << 2) | (send3[1] << 1) | send3[0] );

   ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send);
   if(ErrCode) printf("ERROR [CAEN::Configure] Trigger Source Enable Mask Register [%s]\n", ErrMsg[0-ErrCode]);
   ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read);
   if(ErrCode) printf("ERROR [CAEN::Configure] Trigger Source Enable Mask Register [%s]\n", ErrMsg[0-ErrCode]);
   if(read != send){ 
      string readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + string("'") + 
                       Dec2BinStr2((read & 0x00FF0000) >> 16) + string("'") + 
                       Dec2BinStr2((read & 0x0000FF00) >> 8 ) + string("'") + 
                       Dec2BinStr2((read & 0x000000FF)      ); 

      string sendStr = Dec2BinStr2((send & 0xFF000000) >> 24) + string("'") + 
                       Dec2BinStr2((send & 0x00FF0000) >> 16) + string("'") + 
                       Dec2BinStr2((send & 0x0000FF00) >> 8 ) + string("'") + 
                       Dec2BinStr2((send & 0x000000FF)      ); 
      printf("ERROR Configuration, wrong Write Trigger Source Enable Mask Register\n");
      printf("send %s, read %s\n", sendStr.c_str(), readStr.c_str());    
      }
                 
   // Setting Trigger Output Enable Mask Register
   address = 0x8110; 

   if( mSWTriggerMode  == 1 || mSWTriggerMode  == 3 ) send1 = 1; else send1 = 0;
   if( mExtTriggerMode == 1 || mExtTriggerMode == 3 ) send2 = 1; else send2 = 0;
   for(Int_t i = 0; i < 4; i++){
      if( mChannelTriggerMode[i] == 1 || mChannelTriggerMode[i] == 3 ) send3[i] = 1; else send3[i] = 0;
      }


   //if( mSWTriggerMode  & 0x3 ) send1 = 1; else send1 = 0;
   //if( mExtTriggerMode & 0x3 ) send2 = 1; else send2 = 0;
   //for(UInt_t i1 = 0; i1 < 4; i1++){
   //   if( mChannelTriggerMode[i1] & 0x3 ) send3[i1] = 1; else send3[i1] = 0;
   //   }
   mTriggerOutLogic &= 0x3;
   mMajorityLevel   &= 0x7;

   send = ((send1 << 31) | (send2 << 30) | (mTriggerOutLogic << 8) | (mMajorityLevel << 10) |
           (send3[3] << 3) | (send3[2] << 2) | (send3[1] << 1) | send3[0] );

   ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send);
   if(ErrCode) printf("ERROR [CAEN::Configure] Trigger Source Enable Mask Register [%s]\n", ErrMsg[0-ErrCode]);
   ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read);
   if(ErrCode) printf("ERROR [CAEN::Configure] Trigger Source Enable Mask Register [%s]\n", ErrMsg[0-ErrCode]);
   if(read != send){ 
      string readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + string("'") + 
                       Dec2BinStr2((read & 0x00FF0000) >> 16) + string("'") + 
                       Dec2BinStr2((read & 0x0000FF00) >> 8 ) + string("'") + 
                       Dec2BinStr2((read & 0x000000FF)      ); 

      string sendStr = Dec2BinStr2((send & 0xFF000000) >> 24) + string("'") + 
                       Dec2BinStr2((send & 0x00FF0000) >> 16) + string("'") + 
                       Dec2BinStr2((send & 0x0000FF00) >> 8 ) + string("'") + 
                       Dec2BinStr2((send & 0x000000FF)      ); 
      printf("ERROR Configuration, wrong Write Trigger Source Enable Mask Register\n");
      printf("send %s, read %s\n", sendStr.c_str(), readStr.c_str());    
      }
  
   // Setting n Channel Trigger Mask Register
   // address 0x1nA8 - n is a group number
   //

   for(UInt_t gr = 0; gr < 4; gr++){
      send = 0;
      for(UInt_t ch = 0; ch < 8; ch++){
         if(mChannelTriggerMode[gr] == 0) {mSelfTriggerMask[8*gr + ch] = 0; }

         mSelfTriggerMask[8*gr + ch] &= 1;
         send |= (mSelfTriggerMask[8*gr + ch] << ch );

         }
      address = 0x10A8 | (gr << 8);

      ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send & 0xFF);
      if(ErrCode) printf("ERROR [CAEN::Configure] N Channel Trigger Mask [%s]\n", ErrMsg[0-ErrCode]);
      ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read);
      if(ErrCode) printf("ERROR [CAEN::Configure] N Channel Trigger Mask [%s]\n", ErrMsg[0-ErrCode]);
      if(mSelfTriggerMask[gr] != send){ 
         string readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + string("'") + 
                          Dec2BinStr2((read & 0x00FF0000) >> 16) + string("'") + 
                          Dec2BinStr2((read & 0x0000FF00) >> 8 ) + string("'") + 
                          Dec2BinStr2((read & 0x000000FF)      ); 

         string sendStr = Dec2BinStr2((send & 0xFF000000) >> 24) + string("'") + 
                          Dec2BinStr2((send & 0x00FF0000) >> 16) + string("'") + 
                          Dec2BinStr2((send & 0x0000FF00) >> 8 ) + string("'") + 
                          Dec2BinStr2((send & 0x000000FF)      ); 
         printf("ERROR Configuration, wrong Write N Channel Trigger Mask Register\n");
         printf("send %s, read %s\n", sendStr.c_str(), readStr.c_str());    
        }

      }


       //-t Configure group trigger threshold
 for(Int_t i = 0; i < 4; i++){ 
       ErrCode = CAEN_DGTZ_SetGroupTriggerThreshold(mHandle, i, mThreshold[i]);
       if(ErrCode) printf("ERROR [CAEN::Configure] SetGroupTriggerThreshold [%s]\n", ErrMsg[0-ErrCode]);
    }


 if (mAcqMode == 0){  
    // inhibit TRGIN on board 0 in order to avoid start of run with external triggers
    //CAEN_DGTZ_WriteRegister(mHandle, 0x817C, 1);
    }

    // Acq mode, start with 1-st trigger edge
//    CAEN_DGTZ_WriteRegister(mHandle, 0x8100, 0xE);  
    // set delay in clk values
    //CAEN_DGTZ_WriteRegister(mHandle, 0x8170, 4*(1-mHandle));   // Run Delay 


/*
       //-t Configure channel group mask (register writing)
       send = mSelfTriggerMask[i];
       address = 0x10A8 | (i << 8);  // this is address of posttrigger register
       ErrCode = CAEN_DGTZ_WriteRegister(mHandle, address, send);
       if(ErrCode) printf("ERROR [CAEN::Configure] SetChannelGroupMask [%s]\n", ErrMsg[0-ErrCode]);
       ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &mSelfTriggerMask[i]);
       if(ErrCode) printf("ERROR [CAEN::Configure] GetChannelGroupMask [%s]\n", ErrMsg[0-ErrCode]);
       if(mSelfTriggerMask[i] != send) printf("ERROR Configuration, wrong ChannelGroupMask, send = %d, read = %d\n", send, mSelfTriggerMask[i]);
*/
       


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


 //RegisterDump(); 


 if(mGuiWindow) mGuiWindow->Refresh();

 Refresh();
 if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);
 //if(ErrCode) printf("ERROR [%s]\n", ErrMsg[0-ErrCode]);

return 1;
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
       else if( name == string("RecordLength")		) mRecordLength		= atoi( value.c_str()	); 
       else if( name == string("PostTrigger")		) mPostTrigger		= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr1")		) mGroupEnableMask[0]	= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr2")		) mGroupEnableMask[1]	= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr3")		) mGroupEnableMask[2]	= atoi( value.c_str()	);
       else if( name == string("EnableMaskGr4")		) mGroupEnableMask[3]	= atoi( value.c_str()	);
       else if( name == string("DCOffsetGr1")		) { 
		mDCoffset_mV[0]	= atoi( value.c_str()); 
		if(mDCoffset_mV[0] > 1001 || mDCoffset_mV[0] < -1001){ 
		   mDCoffset_mV[0] = 0;
		   cout << "WARNING! Value of offset is not correct! Set to 0." << endl;
                   }
		mDCoffset[0] = mV2DAC(mDCoffset_mV[0]);
		}
       else if( name == string("DCOffsetGr2")		) { 
		mDCoffset_mV[1]	= atoi( value.c_str()); 
		if(mDCoffset_mV[1] > 1001 || mDCoffset_mV[1] < -1001){ 
		   mDCoffset_mV[1] = 0;
		   cout << "WARNING! Value of offset is not correct! Set to 0." << endl;
                   }
		mDCoffset[1] = mV2DAC(mDCoffset_mV[1]);
		}
       else if( name == string("DCOffsetGr3")		) { 
		mDCoffset_mV[2]	= atoi( value.c_str()); 
		if(mDCoffset_mV[2] > 1001 || mDCoffset_mV[2] < -1001){ 
		   mDCoffset_mV[2] = 0;
		   cout << "WARNING! Value of offset is not correct! Set to 0." << endl;
                   }
		mDCoffset[2] = mV2DAC(mDCoffset_mV[2]);
		}
       else if( name == string("DCOffsetGr4")		) { 
		mDCoffset_mV[3]	= atoi( value.c_str()); 
		if(mDCoffset_mV[3] > 1001 || mDCoffset_mV[3] < -1001){ 
		   mDCoffset_mV[3] = 0;
		   cout << "WARNING! Value of offset is not correct! Set to 0." << endl;
                   }
		mDCoffset[3] = mV2DAC(mDCoffset_mV[3]);
		}
       else if( name == string("SaveChannel")		) mSaveChannel		= atoi( value.c_str()   );
       else if( name == string("SoftwareTriggerMode")	) mSWTriggerMode	= atoi( value.c_str()	); 
       else if( name == string("ExternalTriggerMode")	) mExtTriggerMode	= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr1") ) mChannelTriggerMode[0]= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr2") ) mChannelTriggerMode[1]= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr3") ) mChannelTriggerMode[2]= atoi( value.c_str()	); 
       else if( name == string("ChannelTriggerModeGr4") ) mChannelTriggerMode[3]= atoi( value.c_str()	); 
       else if( name == string("ChannelPulsPolarity")	) mChannelPulsPolarity	= atoi( value.c_str()	); 
       else if( name == string("ThresholdGr1")		) { mThreshold_mV[0]	= atoi( value.c_str());mThreshold[0] = mV2ADC(mThreshold_mV[0]);} 
       else if( name == string("ThresholdGr2")		) { mThreshold_mV[1]	= atoi( value.c_str());mThreshold[1] = mV2ADC(mThreshold_mV[1]);} 
       else if( name == string("ThresholdGr3")		) { mThreshold_mV[2]	= atoi( value.c_str());mThreshold[2] = mV2ADC(mThreshold_mV[2]);} 
       else if( name == string("ThresholdGr4")		) { mThreshold_mV[3]	= atoi( value.c_str());mThreshold[3] = mV2ADC(mThreshold_mV[3]);} 
       else if( name == string("TriggerSlopeGr1")	) mTriggerEdge[0]	= atoi( value.c_str()	); 
       else if( name == string("TriggerSlopeGr2")	) mTriggerEdge[1]	= atoi( value.c_str()	); 
       else if( name == string("TriggerSlopeGr3")	) mTriggerEdge[2]	= atoi( value.c_str()	); 
       else if( name == string("TriggerSlopeGr4")	) mTriggerEdge[3]	= atoi( value.c_str()	); 
       else if( name == string("SelfTriggerMaskGr1")	){ 
            for(Int_t i = 0; i < 8; i++){
                if(value[i] == '1') mSelfTriggerMask[i] = 1;
                else mSelfTriggerMask[i] = 0;
               }
              }

       else if( name == string("SelfTriggerMaskGr2")	){ 
            for(Int_t i = 0; i < 8; i++){
                if(value[i] == '1') mSelfTriggerMask[i+8] = 1;
                else mSelfTriggerMask[i+8] = 0;
               }
              }

       else if( name == string("SelfTriggerMaskGr3")	){ 
            for(Int_t i = 0; i < 8; i++){
                if(value[i] == '1') mSelfTriggerMask[i+16] = 1;
                else mSelfTriggerMask[i+16] = 0;
               }
              }

       else if( name == string("SelfTriggerMaskGr4")	){ 
            for(Int_t i = 0; i < 8; i++){
                if(value[i] == '1') mSelfTriggerMask[i+24] = 1;
                else mSelfTriggerMask[i+24] = 0;
               }
              }

       else if( name == string("IOLevel")		) mFPIOtype		= atoi( value.c_str()	); 
       else if( name == string("ChannelPulsPolarity")	) mChannelPulsPolarity	= atoi( value.c_str()	); 
       else if( name == string("SuppresionMode")	) mSuppresionMode	= atoi( value.c_str()	); 
       else if( name == string("AcquisitionMode")	) mAcqMode		= atoi( value.c_str()	); 
       else if( name == string("DelayChainDaisy")	) mDelay		= atoi( value.c_str()	); 
       else if( name == string("BufferConfiguration")	) mBufferConfig		= atoi( value.c_str()	); 
       else if( name == string("ClockType")		) mClock		= atoi( value.c_str()	);
       else if( name == string("MaxEvents")		) mNumEvents		= atoi( value.c_str()	);
       else if( name == string("SynchronizationMode")	) mRunSyncMode		= atoi( value.c_str()	);
       else cout << "Parameter not recognized, line nr = " << lineNr << "\t[" << inSection << "] " << name << " : " << value << endl;
       
     }  

   }

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
 fout << "# not fully tested, do not use this option, set it always 1" << endl;
 fout << "InstrEnabled "	<< mEnabled		<<endl;          
 fout << endl; 

 fout << "# Channel Configuration"<< endl;
 fout << "# You can difine the track lengt (RecordLength), is the same for all channels" << endl;
 fout << "RecordLength "	<< mRecordLength   	<<endl; 
 fout << "# you can choose how many samples are stored before and after triggeer appear,  the value are between 0, 100 in %" << endl;
 fout << "PostTrigger "	<< mPostTrigger    	<<endl; 
 fout << "# disable = 0 or enable = 1 group channels" << endl;
 fout << "EnableMaskGr1 "	<< mGroupEnableMask[0]	<<endl; 
 fout << "EnableMaskGr2 "	<< mGroupEnableMask[1]	<<endl; 
 fout << "EnableMaskGr3 "	<< mGroupEnableMask[2]	<<endl; 
 fout << "EnableMaskGr4 "	<< mGroupEnableMask[3]	<<endl; 
 fout << "# Set offet for group channels " << endl;
 fout << "# DCOffsetGrN: DC offset adjust (DAC channel setting) in mV. It will automatically recalculated to the correct value of DAC" << endl;
 fout << "# -1000: analog input dynamic range = -Vpp to 0 (negative signals)" << endl;
 fout << "# +1000: analog input dynamic range = 0 to +Vpp (positive signals)" << endl;
 fout << "#     0:   analog input dynamic range = -Vpp/2 to +Vpp/2 (bipolar signals)" << endl;
 fout << "DCOffsetGr1 "	<<  mDCoffset_mV[0]   	<< endl; 
 fout << "DCOffsetGr2 "	<<  mDCoffset_mV[1]   	<< endl; 
 fout << "DCOffsetGr3 "	<<  mDCoffset_mV[2]   	<< endl; 
 fout << "DCOffsetGr4 "	<<  mDCoffset_mV[3]   	<< endl; 

 fout << endl; 
 fout << "# The save option allows you to save certain channels. The digitizer is designed in such that even only one channel is used"<< endl; 
 fout << "# all group must be transfered to PC. Then you can choose by software which channel you want to store and/or display."<< endl; 
 fout << "# It is one 32 bits word, Each bit correspond to each channel"<< endl; 
 fout << "SaveChannel "	<<  mSaveChannel   	<< endl; 
 fout << endl; 
 
 fout << "# Trigger Configuration" << endl;
 fout << "# Enable or disable software, external and channel trigger" << endl;
 fout << "# the options could be 0, 1, 2, 3, disable, enable acq only, enable ext only, both, respectevely "<< endl;
 fout << "SoftwareTriggerMode " << mSWTriggerMode   <<endl; 
 fout << "ExternalTriggerMode "<< mExtTriggerMode 	<<endl; 
 fout << "# " << endl;
 fout << "ChannelTriggerModeGr1 "<< mChannelTriggerMode[0]	<<endl;
 fout << "ChannelTriggerModeGr2 "<< mChannelTriggerMode[1]	<<endl;
 fout << "ChannelTriggerModeGr3 "<< mChannelTriggerMode[2]	<<endl;
 fout << "ChannelTriggerModeGr4 "<< mChannelTriggerMode[3]	<<endl;
 fout << "# " << endl;
 fout << "# Adjust the threshold to the coresponding channel in mv" << endl;
 fout << "# Treshold value is an absolut value, consider it when you add offset to the channel" << endl;
 fout << "# Possible values between -2000 and 2000 mv" << endl;
 fout << "ThresholdGr1 "	<<  mThreshold_mV[0] << endl; 
 fout << "ThresholdGr2 "	<<  mThreshold_mV[1] << endl; 
 fout << "ThresholdGr3 "	<<  mThreshold_mV[2] << endl; 
 fout << "ThresholdGr4 "	<<  mThreshold_mV[3] << endl; 
 fout << "# " << endl;
 fout << "# Trigger slope for the every group of channels, could be positive or negative" << endl;
 fout << "TriggerSlopeGr1 "<< mTriggerEdge[0]	<<endl;
 fout << "TriggerSlopeGr2 "<< mTriggerEdge[1]	<<endl;
 fout << "TriggerSlopeGr3 "<< mTriggerEdge[2]	<<endl;
 fout << "TriggerSlopeGr4 "<< mTriggerEdge[3]	<<endl;
 fout << "# " << endl;
 fout << "# Global trigger can be built from logic OR all indyvidual channel triggers. This option allows you to set which indyvidual "<< endl;
 fout << "# trigger will contribute to build the global one. This is set by indyvidual group. Every group consist of 8 channels" << endl;
 fout << "SelfTriggerMaskGr1 " << mSelfTriggerMask[0]  << mSelfTriggerMask[1]  << mSelfTriggerMask[2]  << mSelfTriggerMask[3]  
                               << mSelfTriggerMask[4]  << mSelfTriggerMask[5]  << mSelfTriggerMask[6]  << mSelfTriggerMask[7]  << endl; 
 fout << "SelfTriggerMaskGr2 " << mSelfTriggerMask[8]  << mSelfTriggerMask[9]  << mSelfTriggerMask[10] << mSelfTriggerMask[11]
                               << mSelfTriggerMask[12] << mSelfTriggerMask[13] << mSelfTriggerMask[14] << mSelfTriggerMask[15] << endl; 
 fout << "SelfTriggerMaskGr3 " << mSelfTriggerMask[16] << mSelfTriggerMask[17] << mSelfTriggerMask[18] << mSelfTriggerMask[19]
                               << mSelfTriggerMask[20] << mSelfTriggerMask[21] << mSelfTriggerMask[22] << mSelfTriggerMask[23] << endl; 
 fout << "SelfTriggerMaskGr4 " << mSelfTriggerMask[24] << mSelfTriggerMask[25] << mSelfTriggerMask[26] << mSelfTriggerMask[27]
                               << mSelfTriggerMask[28] << mSelfTriggerMask[29] << mSelfTriggerMask[30] << mSelfTriggerMask[31] << endl; 
 fout << "# " << endl;
 fout << "# All digitial inputs on the fron panel can be set as TTL = 1 or NIM = 1" << endl;
 fout << "IOLevel "		<< mFPIOtype       	<<endl; 
 fout << "# " << endl;
 fout << "# These options are not fully supported, some of them are hard codded or will be added soon.... " << endl;

 
 fout << "# Trigger Channel Configuration" << endl;
 fout << "ChannelPulsPolarity "<< mChannelPulsPolarity	<<endl; 
 fout << "# Suppresion Mode not implemented, DO NOT USE IT" << endl;
 fout << "SuppresionMode " 	<< mSuppresionMode 	<<endl;
 fout << "# Acquisition Mode could be software controled = 0; or by signal from the front panel = 1" << endl;
 fout << "AcquisitionMode "	<< mAcqMode		<<endl;
 fout << "# Delay in the daisy chain, value in CLK of digitizer. The last digitizer in the chain should be equal 0 and value increase backward" << endl;
 fout << "DelayChainDaisy "     << mDelay		<< endl;
 fout << "# Buffer Configuration is set automatically, DO NOT USE THIS OPTION" << endl;
 fout << "BufferConfiguration "<< mBufferConfig   	<<endl; 
 fout << "# if you want to change clock setings you have to first update firmware" << endl;
 fout << "ClockType "		<< mClock		<<endl;           
 fout << "# Define maximum parameter of how many events can be read from instrument" << endl;
 fout << "MaxEvents "	<< mNumEvents    	<<endl;   
 fout << "# DO not use it" << endl;
 fout << "SynchronizationMode "<< mRunSyncMode   	<<endl; 
 fout << "# " << endl;
 fout << endl; 
 



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
 ULong_t TTT =0l, prevTTT =0l, Nroll = 0l;
// Event16 = NULL;

 //for(Int_t i = 0; i < mEvents.size(); i++)
 //    delete (*it);
 //mEvents.clear();

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
   if(ErrCode) {
       printf("ERROR [CAEN::GetData] ReadRegister Acquisition Status [%s]\n", ErrMsg[0-ErrCode]);
       cout << "Acquisition Status = " << bitset<32>(lstatus1) << endl;
       }
   ErrCode = CAEN_DGTZ_ReadRegister(mHandle, 0x8100, &lstatus2);
   if(ErrCode) {
       printf("ERROR [CAEN::GetData] ReadRegister Acquisition Control [%s]\n", ErrMsg[0-ErrCode]);
       cout << "Acquisition Control = " << bitset<32>(lstatus2) << endl;
       }
 }


  //cout << "---------- Nr of Events from " << mName << ", mHandle = " << mHandle << " " << aNrEvents << " mSize = " << mSize << endl;

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

 for(Int_t i = 0; i < (Int_t)aNrEvents; i++){
    CAEN_DGTZ_GetEventInfo(mHandle, mBuffer, mSize, i, &EventInfo, &EventPtr);
    CAEN_DGTZ_DecodeEvent(mHandle, EventPtr, (void**)&Event16);
     
    AEvent *aLocalEvent = new AEvent(); 
    NormalTriggerBuild( *aLocalEvent);
/*
       if( ((mSaveChannel >> j2) & 1) && ((EventInfo.ChannelMask >> j2/8 )& 1)) { 
          //cout << std::bitset<32>(mSaveChannel) << " " << EventInfo.ChannelMask << endl;
          aLocalEvent->AddTrack(new ATrack(mCardNr, j2, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[j2], 
                               (Short_t*)Event16->DataChannel[j2], Event16->ChSize[j2]));
         }
*/
 
    aLocalEvent->SetEventNr(EventInfo.EventCounter);
    aLocalEvent->SetTimeStamp(EventInfo.TriggerTimeTag);
    mEvents.push( aLocalEvent );
 
    // roll over....
    TTT = ((Nroll<<31) + (EventInfo.TriggerTimeTag & 0x7FFFFFFF));
    if (TTT < prevTTT) {
       Nroll++;
       TTT += (1<<31);
       cout << "---------------------------------------------------------------------------------------------------------" << endl;
       }
    prevTTT = TTT;

    EventInfo.TriggerTimeTag = TTT;
    //cout << "DEBUG [CAEN5740::GetData] Name = " << mName << " TimeStamp = " << EventInfo.TriggerTimeTag << endl;
   }

   //cout << "DEBUG[CAEN5740::GetData] size of mEvents = " << mEvents.size() << endl;
   
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

void CAEN5740::NormalTriggerBuild(AEvent &aEvent){   // for normal work
    for(Int_t i = 0; i < 32; i++){   
       if( ((mSaveChannel >> i) & 1) && ((EventInfo.ChannelMask >> i/8 )& 1)) { 
          //cout << std::bitset<32>(mSaveChannel) << " " << EventInfo.ChannelMask << endl;
          aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                               (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
         }
 
       }
}

//===========================================================================

void CAEN5740::MultiBladeTriggerBuild( AEvent &aEvent){   // for Francesco detector

    if( EventInfo.ChannelMask && 0x0000000F){    
        for(Int_t i = 0; i < 5; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
           }
      }

    if( EventInfo.ChannelMask && 0x000000F0){    
        for(Int_t i = 5; i < 8; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
           }
      }

    if( EventInfo.ChannelMask && 0x00000F00){    
        for(Int_t i = 8; i < 12; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
           }
      }

    if( EventInfo.ChannelMask && 0x0000F000){    
        for(Int_t i = 12; i < 36; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
           }
      }

    if( EventInfo.ChannelMask && 0x000F0000){    
        for(Int_t i = 16; i < 19; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
            }
      }

    if( EventInfo.ChannelMask && 0x00F00000){    
        for(Int_t i = 19; i < 23; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
           }
      }

    if( EventInfo.ChannelMask && 0x0F000000){    
        for(Int_t i = 23; i < 28; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
            }
      }

    if( EventInfo.ChannelMask && 0xF0000000){    
        for(Int_t i = 28; i < 32; i++){
            aEvent.AddTrack(new ATrack(mCardNr, i, 1. / 65e6, 0, 1000, EventInfo.TriggerTimeTag, mThreshold_mV[i], 
                           (Short_t*)Event16->DataChannel[i], Event16->ChSize[i]));
            }        
      }

}

//===========================================================================
void CAEN5740::MultiGridTriggerBuild ( AEvent & /*aEvent*/){   // for Anton detector

}

//===========================================================================

void CAEN5740::RegisterDump(){
FILE *fout;
fout = fopen("register_dump.txt", "w");

fprintf(fout, "=== configuration settings ===\n");

 unsigned int read, i3, address;
 string readStr;

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
 address = 0x1080;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //threshold
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group 0 Threshold                   (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x1088; 
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //status
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group 0 Status                      (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x1098; 
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //DAC
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group 0 DAC                         (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x109C;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //ADC configuration
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group 0 ADC Configuration           (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x10A8;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //n channel trigger enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group 0 Channel Trigger Enable Mask (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );


 address = 0x8000;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //channel configuration
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group Configuration                 (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x800C;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //Buffer organization
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Buffer Organization                 (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8020;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //custom size
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Custom Size                         (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8044;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //Decimation Factor
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Decimation Factor                   (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8100;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //acq control
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Acquisition Control                 (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8104;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //acq status
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Acquisition Status                  (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x810C;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //trigger source enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Trigger Source Enable Mask          (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8110;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //front panel trigger out enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Front Panel Trigger Out Enable Mask (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8114;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //post trigger
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Post Trigger                        (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x811C;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //front panel I/O
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Front Panel I/O                     (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8120;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //channel enable mask
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Group Enable Mask                   (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8124;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //event stored
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "ROC FPGA Firmware rev.              (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );
 
 address = 0x812C;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //event stored
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Events stored                       (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x8170;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //event stored
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "Run Start/Stop Delay                (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );

 address = 0x817C;
 ErrCode = CAEN_DGTZ_ReadRegister(mHandle, address, &read); //event stored
 if(ErrCode) fprintf(fout, "ERROR Read Register %d\n", ErrCode);
 readStr = Dec2BinStr2((read & 0xFF000000) >> 24) + "'" + Dec2BinStr2((read & 0xFF0000) >> 16) + string("'") + Dec2BinStr2((read & 0xFF00) >> 8) + string("'") + Dec2BinStr2((read & 0xFF)); 
 fprintf(fout, "External Trigger Inhibit            (0x%X) \t= 0x%8X = %s\n", address, read, readStr.c_str() );


fclose(fout);

}













