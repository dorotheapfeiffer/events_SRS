
#ifndef SimCard_h
#define SimCard_h

#include <iostream>
#include <TThread.h>

#include "Digitizer.h"
#include "SimCardDlg.h"
#include "AEvent.h"
#include "ATrack.h"


class TGFrame;
class TGLabel;
class TGComboBox;
class TGCheckButton;
class TGTextEntry;
class TGVSlider;
class TGTextBuffer;
class TGNumberEntry;
class ATrack;
class TThread;

class SimCardDlg;


class SimCard : public Digitizer {
private:

   TThread			aThread;
   static void* 		ThreadFunc(void*);
   Bool_t			aRun;


   static Int_t                 gDeviceNr;              // needed for counting cards in case there are more than one

   char                         mName[80];              // name of the card
   Int_t                        mCardNr;                // a card number in case there are more than one instrument in the system
   bool                         mEnabled;               // a card could be enabled (switched on) or disabled (switched off)
   Int_t                        mStatus;                // a card status could be connected or not (not implemented...)
   Bool_t                       mReconfig;
   Bool_t                       mFirstConfiguration;

   UInt_t                       mAcqMode;
   Int_t                        mClock;                 // clock in the system, could be internal, external or ref ext 
   Int_t                        mHandle;                // handle of the instrument
   Int_t                        mNumEvents;             // Maximum number of events to transfer in a BlockRead 
   Char_t*                      mBuffer;                // Pointer to the buffer allocated
   UInt_t                       mSize;                  // The size in byte of the buffer allocated
   UInt_t                       mBufferConfig;          // configuration number and size of the buffer
   UInt_t                       mRecordLength;          // record length
   UShort_t                     mDecimationFactor;      // 
   UInt_t                       mPostTrigger;           // nr of post trigger samples
   UInt_t                       mDCoffset[4];           // channel offset (for group)
   Double_t                     mDCoffset_mV[4];        // channel offset (for group) in mV
   UInt_t                       mGroupEnableMask[4];    // each group of channel can be enabled or disabled  
   UInt_t                       mSaveChannel;           // Save channel, each bit correspond to one channel

   UInt_t                       mSWTriggerMode;         // SW Trigger should only generate internal trigger or/and output trigger
   UInt_t                       mExtTriggerMode;        // EXT Trigger input should generate internal trigger or/and also output trigger 
   UInt_t                       mChannelTriggerMode[4]; // group self trigger mode, each trigger from the group channel can generate also output trigger
   UInt_t                       mThreshold[4];          // threshold for trigger, each for one  group 
   Double_t                     mThreshold_mV[4];       // threshold for trigger, each for one  group in mV

   UInt_t                       mFPIOtype;              // I/O level of the digitizers (NIM or TTL)
   UInt_t                       mChannelPulsPolarity;   // Channel Pulse polarity (1 - POS or 0 - NEG) one 32bit word for all channels
   UInt_t                       mTriggerEdge[4];        // per group, not clear from the documentaion

   UInt_t                       mRunSyncMode;           // synchronization mode for the digitizer
   UInt_t                       mSelfTriggerMask[32];   // decide which indyvidual trigger take part in generating global trigger, 
                                                        // indyvidual for each group
                                                        //    UInt_t                       mSuppresionMode;


   std::vector<AEvent *>        mEvents;                // List of all events saved by the digitizer during one dataread

   AEvent                       testEvent;

   SimCard(){}
   SimCard(Int_t);
   SimCard(const SimCard&);
   SimCard operator=( SimCard );

   friend class Digitizer;
   friend class SimCardDlg;

   SimCardDlg                  *mGuiWindow;


public:

  virtual	~SimCard();
 
   virtual void          Refresh();
  virtual void          DoEnable(bool a)        { cout << "Button in state : "<< a << endl; mEnabled = a; }
  virtual Bool_t        IsEnabled() const       { return mEnabled;}

  //virtual void                PrintInfo(){}
  virtual void          Reset();
  virtual Int_t         Initialize();
  virtual Int_t          Configure();
  virtual void          StartAcq();
  virtual void          StopAcq();
  virtual void          Close();
  
  virtual AEvent*       GetEvent()                              {return mEvents[0];}
  //virtual AEvent*     GetEvent(Int_t nr)                              {return &testEvent; }

  
  virtual const char*   GetName()                               const   { return mName; }
  virtual UInt_t        GetData();
  virtual void          SendSWTrigger();
  virtual void          BuildGui(TGCompositeFrame *aFrame)              { mGuiWindow = new SimCardDlg(this, aFrame); }
  
  virtual Int_t         ReadConfigFile(ifstream &);
  virtual Int_t         WriteConfigFile(ofstream &);
  virtual Int_t         GetTrackLength()                        const   { return mRecordLength; }
  virtual Int_t         GetRange(Int_t )                        const   { return 2000;} // return in mVolts
  virtual Int_t         GetNrOfGroups()                         const   { return 4;}
  virtual Int_t         GetNrOfChannels()                       const   { return 32;}
  virtual Int_t         GetActiveGroup(Int_t a)                 const   { return mGroupEnableMask[a];}
  virtual Int_t         GetThreshold(Int_t a, Bool_t b = kTRUE) const   { if(b) return mThreshold_mV[a]; else return mThreshold[a];}
  virtual Int_t         GetThresholdMode(Int_t a)               const   { return mChannelTriggerMode[a];}

	void		GenerateEvent(Short_t *, Int_t); 
	 void          SetReconfigure(Bool_t a){mReconfig = a;}
          Bool_t        GetReconfigure(){return mReconfig;}

ClassDef(SimCard, 0)
};


#endif

