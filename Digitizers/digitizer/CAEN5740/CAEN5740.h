#ifndef CAEN5740_h
#define CAEN5740_h

#include <iostream>
#include <vector>
#include <queue>

#include "Digitizer.h"
#include "CAEN5740Dlg.h"
#include "AEvent.h"
#include "ATrack.h"


#include "include/CAENDigitizer.h"
#include "include/CAENDigitizerType.h"

class CAEN5740 : public Digitizer {
 
private:
   static Int_t 		gDeviceNr;		// needed for counting cards in case there are more than one
  
   char				mName[80];		// name of the card
   Int_t			mCardNr;		// a card number in case there are more than one instrument in the system
   bool				mEnabled;		// a card could be enabled (switched on) or disabled (switched off)
   Int_t 			mStatus; 		// a card status could be connected or not (not implemented...)
   Bool_t			mReconfig;
   Bool_t			mFirstConfiguration;

   CAEN_DGTZ_BoardInfo_t	BoardInfo;
   CAEN_DGTZ_EventInfo_t	EventInfo;
   CAEN_DGTZ_UINT16_EVENT_t    *Event16;

   UInt_t			mAcqMode;		// acq controlled on hardware(1) or software mode (0)
   Int_t			mClock;			// clock in the system, could be internal, external or ref ext 
   Int_t			mHandle;		// handle of the instrument
   Int_t			mNumEvents;		// Maximum number of events to transfer in a BlockRead 
   Char_t*			mBuffer;		// Pointer to the buffer allocated
   UInt_t			mSize;			// The size in byte of the buffer allocated
   UInt_t			mBufferConfig;          // configuration number and size of the buffer
   UInt_t			mRecordLength;		// record length
   UShort_t			mDecimationFactor;	// 
   UInt_t			mPostTrigger;		// nr of post trigger samples
   UInt_t			mDCoffset[4];		// channel offset (for group)
   Double_t			mDCoffset_mV[4];	// channel offset (for group) in mV
   UInt_t			mGroupEnableMask[4];	// each group of channel can be enabled or disabled  
   UInt_t			mSaveChannel;		// Save channel, each bit correspond to one channel
 
   UInt_t			mSWTriggerMode;		// SW Trigger should only generate internal trigger or/and output trigger
   UInt_t			mExtTriggerMode;	// EXT Trigger input should generate internal trigger or/and also output trigger 
   UInt_t			mChannelTriggerMode[4]; // group self trigger mode, each trigger from the group channel can generate also output trigger
   UInt_t			mThreshold[4];		// threshold for trigger, each for one  group 
   Double_t			mThreshold_mV[4];	// threshold for trigger, each for one  group in mV

   UInt_t			mFPIOtype;		// I/O level of the digitizers (NIM or TTL)
   UInt_t			mChannelPulsPolarity;	// Channel Pulse polarity (1 - POS or 0 - NEG) one 32bit word for all channels
   UInt_t			mTriggerEdge[4];	// per group, not clear from the documentaion

   UInt_t			mRunSyncMode;		// synchronization mode for the digitizer
   UInt_t			mDelay;			// Delay in the daisy chain, for one digitizer = 0, for many the last one should be also 0.
   UInt_t			mSelfTriggerMask[32];	// decide which indyvidual trigger take part in generating global trigger, 
							// indyvidual for each group
   UInt_t			mSuppresionMode;
   UInt_t			mCoincidenceWindow; 
   UInt_t 			mTriggerCoincidenceLevel;
   UInt_t 			mTriggerOutLogic;
   UInt_t 			mMajorityLevel;

   std::queue<AEvent *>		mEvents;		// List of all events saved by the digitizer during one dataread
   
   AEvent			testEvent;

   CAEN5740(); 
   CAEN5740(Int_t); 
   friend class Digitizer;
   friend class CAEN5740Dlg;

   CAEN5740Dlg			*mGuiWindow;
   void				RegisterDump();
  
public:
 
  virtual 		~CAEN5740();

  virtual void		Refresh(); 
  virtual void		DoEnable(bool a)	{ cout << "Button in state : "<< a << endl; mEnabled = a; }
  virtual Bool_t	IsEnabled() const	{ return mEnabled;}

  //virtual void		PrintInfo(){}
  virtual void		Reset();
  virtual Int_t		Initialize();
  virtual Int_t		Configure();
  virtual void		StartAcq();
  virtual void		StopAcq();
  virtual void		Close();

  virtual void		PopEvent()					{mEvents.pop();}
  virtual AEvent*	GetEvent()					{return mEvents.front();}
  //virtual AEvent*	GetEvent(Int_t nr)				{return &testEvent; }

  virtual const char*  	GetName()				const 	{ return mName; }
  virtual UInt_t	GetData();
  virtual void		SendSWTrigger();
  virtual void		BuildGui(TGCompositeFrame *aFrame)		{ mGuiWindow = new CAEN5740Dlg(this, aFrame); }

  virtual Int_t 	ReadConfigFile(ifstream &);
  virtual Int_t 	WriteConfigFile(ofstream &);
  virtual Int_t 	GetTrackLength()			const 	{ return mRecordLength; }
  virtual Int_t 	GetRange(Int_t )			const 	{ return 2000;} // return in mVolts
  virtual Int_t         GetNrOfGroups()				const	{ return 4;}
  virtual Int_t         GetNrOfChannels()                       const   { return 32;}  
  virtual Int_t         GetActiveGroup(Int_t a)                 const   { return mGroupEnableMask[a];}  
  virtual Int_t         GetThreshold(Int_t a, Bool_t b = kTRUE) const   { if(b) return mThreshold_mV[a]; else return mThreshold[a];}  
  virtual Int_t         GetThresholdMode(Int_t a)               const   { return mChannelTriggerMode[a];}  
 
  	  void		SetReconfigure(Bool_t a){mReconfig = a;}
	  Bool_t	GetReconfigure(){return mReconfig;}

          //ADC is 12-bits
	  Double_t	ADC2mV(UInt_t counts){ return (counts - 2048) / 2.048 ; }
	  UInt_t	mV2ADC(Double_t mV){ return mV * 2.048 + 2048; }

          //DAC is 16-bits
	  Double_t	DAC2mV(UInt_t counts){ return (counts - 32768) / 32.768; }
	  UInt_t	mV2DAC(Double_t mV){ return mV * 32.768 + 32768;  }

	  UInt_t	GetDataSize()	{return mSize;}

	  void 		NormalTriggerBuild(     AEvent &);   // for normal work
          void 		MultiBladeTriggerBuild( AEvent &);   // for Francesco detector
          void 		MultiGridTriggerBuild ( AEvent &);   // for Anton detector



/*
  virtual void		BuildWindow(TGCompositeFrame* );

  virtual void		SetTimeout(int n_timeout) {timeout=n_timeout;}

  virtual Short_t*  GetTrack(Int_t seg = 0, Int_t chn = 0)     const { return mDataY[seg][chn]; }

  virtual Double_t GetSamplingRate()		const { return iSampling; }
  virtual Int_t GetActiveChannel(Int_t a = 0)	const { return iChannelEnable[a]; }
  virtual Double_t GetOffset(Int_t a = 0)	const { return 0;}
  virtual Double_t GetTriggerLevel(Int_t a = 0)	const { return iTriggerLevel[a]; }

  virtual Int_t ReadConfigFile(ifstream &);
  virtual Int_t WriteConfigFile(ofstream &);
  virtual Int_t AcqReady();
  virtual void GetDataFromCard();

  virtual Int_t GetSegmentSize() const {return nbrSegments;}
  virtual unsigned long long GetTimeStamp(Int_t seg ) const { return 0; }
  virtual void SetSegmentSize(int a) { nbrSegments = a; } 
  virtual Double_t GetMultiplicationFactor(int channel);
 */


ClassDef(CAEN5740,0);
};


#endif

