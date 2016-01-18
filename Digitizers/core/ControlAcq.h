#ifndef ControlAcq_H
#define ControlAcq_H 

#include <iostream>

#include "TObject.h"
#include "TTimer.h"

#include "Digitizer.h"

class AManager;
class AEvent;
class ARingBuffer;

class ControlAcq : public TQObject{
 private:
  
	Bool_t 		mSimOnly;
	Int_t		mSegSize;
	UInt_t		numInst;
	Int_t		mChannels;
	Int_t		mTracks;
	Int_t		mLoopNr;
	Int_t		mMaxSegment;
	Int_t		*mCardReady;
	Int_t		mAllCardsReady;
	UInt_t		mNrAllEvents;

	std::vector<AEvent>	mAggregateEvents;


	ARingBuffer	*aSaveBuffer;
	ARingBuffer	*aDisplayBuffer;
	ARingBuffer	*aOnlineBuffer;
  
	AManager	*aManager;
	TTimer		*timer2;
	Long_t 		mTimeout2;
	
	Int_t		IsBadEvent();
	Int_t		Aggregate();
	bool 		LoopCondition(Int_t mode = 0);
	bool		IsForceTrigger();

 	UInt_t 		numEvents[ 100 ];		// number of events per instrument it support 100 instruments....

	ControlAcq( const ControlAcq& );		// disable copy contructor
 	ControlAcq operator=( ControlAcq );		// disaple = operator
 public:
  
	ControlAcq( ) {}				// default constructor required by ROOT
	ControlAcq( bool mSimOnly );
	virtual ~ControlAcq();

	void		DoTimeout(Int_t);
	void		Run();
	void		StartAllCards();
	void		StopAllCards();

ClassDef(ControlAcq,0)
};

#endif
