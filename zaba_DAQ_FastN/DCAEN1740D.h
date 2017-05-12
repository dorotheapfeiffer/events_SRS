#ifndef CAEN1740_H
#define CAEN1740_H
#include <ostream>
#include <queue>
#include <vector>

#include "DModule.h"
#include "CAENVMElib.h"
#include "CAENDigitizer.h"
#include "CAENDigitizerType.h"
#include "_CAENDigitizer_DPP-QDC.h"

#define MAX_BLT_SIZE (256*1024)

//*****************************************************************************
class DGDisplay;

class DCAEN1740D : public DModule {


private:
public:

 UInt_t			m_BaseAddress;
 Int_t			m_ModuleID;

 Int_t			m_Enabled;
 std::string		m_Name;
 UInt_t			m_Firmware;

 UInt_t                 m_AcqMode;               // acq controlled on hardware(1) signal-IN or software mode (0)
 CAEN_DGTZ_RunSyncMode_t m_RunSyncMode;           // synchronization mode for the digitizer

 Int_t                  m_Handle;                // handle of the instrument
 Int_t                  m_Clock;                 // clock in the system, could be internal, external or ref ext 
 UInt_t                 m_NrChannels;
 UInt_t                 m_NrGroups;
 UShort_t               m_DecimationFactor;      // 
 ULong_t                m_SaveChannel;           // Save channel, each bit correspond to one channel (64)
 UInt_t                 m_SWTriggerMode;         // SW Trigger should only generate internal trigger or/and output trigger
 UInt_t                 m_ExtTriggerMode;        // EXT Trigger input should generate internal trigger or/and also output trigger 
 /*
 Int_t                  m_MaxEventsBLT;          // Maximum number of events to transfer in a BlockRead 
 UInt_t                 m_RecordLength;          // record length
 UInt_t                 m_Range;  
 UInt_t                 m_Range_mV;
 UInt_t                 m_PostTrigger;           // nr of post trigger samples
 UInt_t                 m_DCoffset[8];           // channel offset (for group)
 Double_t               m_DCoffset_mV[8];        // channel offset (for group) in mV
 UInt_t                 m_GroupEnableMask[8];    // each group of channel can be enabled or disabled  
 UInt_t                 m_ChannelTriggerMode[8]; // group self trigger mode, each trigger from the group channel can generate also output trigger
 UInt_t                 m_Threshold[8];          // threshold for trigger, each for one  group 
 Double_t               m_Threshold_mV[8];       // threshold for trigger, each for one  group in mV
*/
 UInt_t                 m_FPIOtype;              // I/O level of the digitizers (NIM=0 or TTL=1)
 //UInt_t                 m_ChannelPulsPolarity[8];// Channel Pulse polarity (1 - POS or 0 - NEG) in the digitizer is individual per channel but I implemented it per group...
 //UInt_t                 m_TriggerEdge[8];        // per group, not clear from the documentaion
 
 UInt_t                 m_Delay;                 // Delay in the daisy chain, for one digitizer = 0, for many the last one should be also 0.
 //UInt_t                 m_SelfTriggerMask[64];   // decide which indyvidual trigger take part in generating global trigger, 

 // settings for DPP-QDC
 UInt_t			m_MaxEventsAggBLT;
 UInt_t                 m_AcqModeDPP;           // data can be saved in different formats, LIST or MIXED
 UInt_t                 m_RecordLengthDPP;      // record length
 UInt_t 		m_FixedBaseLineDPP;
 UInt_t                 m_GroupEnableMaskDPP[8];    // each group of channel can be enabled or disabled  
 UInt_t                 m_SelfTriggerMaskDPP[64];   // decide which indyvidual trigger take part in generating global trigger, 
 UInt_t                 m_DCoffsetDPP[8];       // channel offset (for group)
 UInt_t 		m_GateWidthDPP[8];      // in steps (1step = 16ns) 
 UInt_t 		m_PreGateDPP[8];        // 
 UInt_t			m_BaseLineDPP[8]; 
 UInt_t 		m_PreTriggerDPP[8];     // pretrigger ins steps (1step = 16ns)
 UInt_t 		m_TriggerHoldOffDPP[8];    // in steps, (1step = 16ns)
 UInt_t 		m_TriggerModeDPP;    // shared trigger width in step of 16ns, used to propagate the trigger
 UInt_t 		m_PulsePolarityDPP[8];  // pulse polarity, can be positive or negative
 UInt_t 		m_EnableChargePedestalDPP;
 UInt_t 		m_ChargeSensitivityDPP[8];
 UInt_t			m_TriggerSmoothingDPP[8];
 UInt_t 		m_ThresholdDPP[64];
 UInt_t			m_DisTrigHist;
 UInt_t			m_DisSelfTrigger ;
 UInt_t			m_TestPulsesRate ;
 UInt_t			m_EnTestPulses   ;

  // indyvidual for each group
  // UInt_t                       mSuppresionMode;
  // UInt_t                       mCoincidenceWindow;
  // UInt_t                       mTriggerCoincidenceLevel;
  // UInt_t                       mTriggerOutLogic;
  // UInt_t                       mMajorityLevel;


 CAEN_DGTZ_BoardInfo_t        BoardInfo;
 CAEN_DGTZ_EventInfo_t        EventInfo;
 CAEN_DGTZ_UINT16_EVENT_t    *Event16;
 CAEN_DGTZ_ErrorCode 	      ret;
 uint32_t     		      NumEvents[64];

 _CAEN_DGTZ_DPP_QDC_Event_t     *gEvent[64];
 _CAEN_DGTZ_DPP_QDC_Waveforms_t *gWaveforms;
 _CAEN_DGTZ_DPP_QDC_Event_t     *gEventsGrp[8];
 uint32_t                       gAllocatedSize;

 Char_t			*m_localBuffer; 
 std::string		m_StringBuffer1; 
 std::string		m_StringBuffer2; 
 std::string		m_StringBufferSize1; 
 std::string		m_StringBufferSize2; 
 Char_t			m_Buffer[500*1024*1024]; 
 
 UInt_t			m_EventsInBuffer;
 UInt_t			m_Size;
 UInt_t			m_BufferPos;
 UInt_t			m_BufferSize;
 UInt_t			m_dataSizeByte;
 UInt_t			m_IRQ;
 UInt_t			m_Events;
 UInt_t			m_savingformat; 
 //std::queue<AEvent *>   m_QueEvents;                // List of all events saved by the digitizer during one dataread
 std::queue< int >   	  m_QueEvents;                // List of all events saved by the digitizer during one dataread

public:
  DCAEN1740D()     {printf("Default CAEN 1740 constructor called\n");}
  DCAEN1740D(Char_t *mdesc, UInt_t addr);
  virtual 	~DCAEN1740D();

  void		InitModule();
  void		SaveConfig(std::ofstream &);
  void		LoadConfig(std::ifstream &);
  void		StartAcq();
  void		StopAcq();
  void          Log(std::ofstream &);
  void		ConfigureModule();
  void		ReadVME();
  void		BuildEvent();
  void		SaveEvent();
  void		ShowEvent();
  void		SendEvent();
  void		ShowData(DGDisplay*, DAcquisition *);
  void		DataSave(DAcquisition *);
  void		ResetModule();
  UInt_t	GetNrEvents() { return m_Events;}
  UInt_t	GetDataSize() { return m_dataSizeByte;}
  void		ShowSettings();
  void 		GnuplotOnline(Gnuplot &);

  //void          PopEvent()    { m_QueEvents.pop();}
  //AEvent*       GetEvent()    { return m_QueEvents.front();}

  Int_t         GetTrackLength()                        const   { return m_RecordLengthDPP; }
  //Int_t         GetRange(Int_t a, Bool_t b = kTRUE )    const   { if(b) return m_Range_mV; else return m_Range;} // return in mVolts
  Int_t         GetOffset(Int_t a, Bool_t b = kTRUE)    const   { return m_DCoffsetDPP[a];} // return in mVolts or adc conversion
  Int_t         GetThreshold(Int_t a, Bool_t b = kTRUE) const   { return m_ThresholdDPP[a];}
  Int_t         GetNrOfGroups()                         const   { return m_NrGroups;}
  Int_t         GetNrOfChannels()                       const   { return m_NrChannels;}
  Int_t         GetActiveGroup(Int_t a)                 const   { return m_GroupEnableMaskDPP[a];}
  //Int_t         GetThresholdMode(Int_t a)               const   { return m_ChannelTriggerMode[a];}


  void 		RegisterDump();
  std::string	CheckError(CAEN_DGTZ_ErrorCode);
  //ADC is 12-bits
  Double_t      ADC2mV(UInt_t counts){ return (counts - 2048) / 2.048 ; }
  UInt_t        mV2ADC(Double_t mV){ return mV * 2.048 + 2048; }
  
  //DAC is 16-bits
  Double_t      DAC2mV(UInt_t counts){ return (counts - 32768) / 32.768; }
  UInt_t        mV2DAC(Double_t mV){ return mV * 32.768 + 32768;  }
  


  ClassDef(DCAEN1740D,1)       // Mesytec Pulse Sensitive Digitizer MADC-32
};
//*****************************************************************************

#endif
