/*****************************************************************************
*
*****************************************************************************/

#ifndef ATrack_h
#define ATrack_h
#include <iostream>
#include <string>
#include <exception>

#include <TObject.h>


using namespace std;

class ATrack: public TObject{

private:
 UInt_t		aTrackNr;
 UInt_t		aCardNr;
 UInt_t		aChannelNr; 
 UInt_t		aOffset;
 UInt_t		aRange;
 ULong_t	aTimeStamp;
 UInt_t		aThreshold;

 UInt_t		aDataSize;
 Double_t	aSampling;
 UShort_t*	aData; //[aDataSize]
 

public:
 ATrack();
 ATrack(Int_t card, Int_t channel, Double_t sampling, Int_t offset, Int_t range, ULong_t timestamp, Int_t threshold,
              Short_t* data, Int_t size /*in bytes*/);

 ATrack(const ATrack&);              
 ATrack& operator= (const ATrack&); 


 int AddData(Int_t card, Int_t channel, UShort_t* data, Int_t dataSize);

 UInt_t		GetTrackNr()	const	{ return aTrackNr;  }
 UInt_t		GetCardNr()	const	{ return aCardNr;   }
 UInt_t		GetChannelNr()	const	{ return aChannelNr;}
 UInt_t		GetOffset()	const	{ return aOffset;   }
 UInt_t		GetRange()	const	{ return aRange;   }
 ULong_t	GetTimeStamp()	const	{ return aTimeStamp;   }
 UInt_t		GetThreshold()	const	{ return aThreshold;   }
 UInt_t		GetDataSize()	const	{ return aDataSize; }
 Double_t	GetSampling()	const	{ return aSampling; }
 UShort_t*	GetData()	const	{ return aData;     }
 void           Write2F(ofstream& fout);
virtual ~ATrack();

ClassDef(ATrack, 1) //simple track
};


#endif

