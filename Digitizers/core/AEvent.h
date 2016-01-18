/*****************************************************************************
*
*****************************************************************************/

#ifndef AEvent_h
#define AEvent_h


#include <iostream>
#include <string>
#include <exception>
#include <TObject.h>
#include "vecTrack.h"


class ATrack;

using namespace std;

class AEvent: public TObject{
private:
	std::vector<ATrack*> aTrackVector;
	std::vector<ATrack*>::iterator it;

  UInt_t	aEventNr;
  ULong_t	aTimeStamp;
  UInt_t	aExtraDataSize;

public:
  AEvent(){}
  AEvent(Int_t a);
  virtual ~AEvent();

  AEvent& operator= (const AEvent&);
  AEvent(const AEvent&);

 void		AddTrack(ATrack* aTrack);
 void		AddEvent(AEvent* aEvent);
 void		RemoveTrack(UInt_t id);

 void		ClearEvent();
 void		SetEventNr(Int_t a)	{ aEventNr = a;} 
 UInt_t		GetEventNr()	  const	{ return aEventNr;} 
 void		EventShow();
 UInt_t		GetNrTracks()	  const	{ return aTrackVector.size(); }
 ULong_t	GetEventSize()	  const;
 ATrack*	GetTrack(Int_t a) const	{ return aTrackVector.at(a); }
 void 		Write2F(ofstream& fout);
 void		SetTimeStamp(ULong_t a)	{ aTimeStamp = a; }
 ULong_t	GetTimeStamp()	const	{ return aTimeStamp; }

 UInt_t           SaveInBuffer(Char_t *aBuffer);

ClassDef(AEvent, 1) //simple event

};

#endif

