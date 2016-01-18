/*****************************************************************************
*
*****************************************************************************/

#ifndef ARingBuffer_h
#define ARingBuffer_h


#include <iostream>
#include <string>
#include <exception>
#include <list>
#include <queue>

#include <TH1F.h>
#include <TObject.h>
#include <TMutex.h>
#include <TCondition.h>

#include "AEvent.h"
#include "ATrack.h"
//class AEvent;

using namespace std;
class ARingBuffer: public TObject{
private:

    const UInt_t aCapacity;
    TMutex aMutex;
    TCondition aBufferNotEmpty;
    TCondition aBufferNotFull;

    list<AEvent> aListBuffer;

public:
    ARingBuffer(UInt_t capacity = 5) : 
        aCapacity(capacity),
        aMutex(),
        aBufferNotEmpty( &aMutex ),
        aBufferNotFull( &aMutex )
        {}

    virtual ~ARingBuffer(){
    cout << "DEBUG [ARingBuffer::~ARingBuffer]" << endl;
    aListBuffer.clear();
    }

// static ARingBuffer& GetInstance(){
//        static ARingBuffer aRingBuffer;        
//        return aRingBuffer;
//        }


//----------------------------------------------------------------
    void push_front(AEvent& aEvent) {
        Int_t retval = 0; // to dodalem
        aMutex.Lock();
        while( is_not_full() && retval == 0){
	   //cout << "BUFFER czekam " << endl;
           retval = aBufferNotFull.TimedWaitRelative(100); // to zmienilem bylo wait
	   //cout << "BUFFER przyszedl timeout retval = " << retval << endl;
           }
	if(retval == 0 ){  // to doalem
            aListBuffer.push_front( aEvent ); 
            //cout << "[BUFFER zapisalem] event Nr = " << aEvent.GetEventNr() << endl;
            //cout << "[BUFFER wr] +++ Track Nr = " << aEvent.GetTrack(0)->GetTrackNr() << endl;
            //cout << "[BUFFER wr] +++ Track Nr = " << aEvent.GetTrack(1)->GetTrackNr() << endl;
            //cout << "[BUFFER wr] +++ Track Nr = " << aEvent.GetTrack(2)->GetTrackNr() << endl;
            //cout << "[BUFFER wr] +++ Track Nr = " << aEvent.GetTrack(3)->GetTrackNr() << endl;
	    }
	//else cout << "BUFFER byl timeout " << endl;
	aMutex.UnLock();
        aBufferNotEmpty.Broadcast();
    }

//----------------------------------------------------------------
    //void pop_back(AEvent* aEvent) { zmienilem na bool bo wymaga tego ADisplay
    bool pop_back(AEvent* aEvent) {
        aMutex.Lock();
        while( is_not_empty() ){
           aBufferNotEmpty.Wait();
           }
        //cout << "[BUFFER 1] bede czytal  event" << endl;
        *aEvent = aListBuffer.back();
        //cout << "[BUFFER 2] event Nr = " << aEvent->GetEventNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(0)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(1)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(2)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(3)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] size = " << aListBuffer.size() << endl;
        aListBuffer.pop_back();
        //cout << "[BUFFER 3] event Nr = " << endl;
        aMutex.UnLock();
        aBufferNotFull.Broadcast();
   return kTRUE;   
  }

//----------------------------------------------------------------
    //void pop_back(AEvent* aEvent) { zmienilem na bool bo wymaga tego ADisplay
    bool pop_back_time(AEvent* aEvent) {
        //cout << "[BUFFER 0] jestem w pop_back_time" << endl;
	Int_t retval = 0;    
        aMutex.Lock();
        while( is_not_empty() && retval == 0){
           retval = aBufferNotEmpty.TimedWaitRelative(100);
           }
	if(retval == 0){
        //cout << "[BUFFER 1] bede czytal  event" << endl;
        *aEvent = aListBuffer.back();
        //cout << "[BUFFER 2] event Nr = " << aEvent->GetEventNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(0)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(1)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(2)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] +++ Track Nr = " << aEvent->GetTrack(3)->GetTrackNr() << endl;
        //cout << "[BUFFER 2] size = " << aListBuffer.size() << endl;
        aListBuffer.pop_back();
	}
        //cout << "[BUFFER 3] nie ma danych" << endl;
        aMutex.UnLock();
        aBufferNotFull.Broadcast();
   return bool(retval);   
  }


//----------------------------------------------------------------
    void front(AEvent* aEvent) {
        aMutex.Lock();
        while( is_not_empty() ){
           aBufferNotEmpty.Wait();
           }
        *aEvent = aListBuffer.front();
        aMutex.UnLock();
        aBufferNotFull.Broadcast();

   }

//----------------------------------------------------------------


private:
    ARingBuffer(const ARingBuffer&);              // Disabled copy constructor
    ARingBuffer& operator = (const ARingBuffer&); // Disabled assign operator

    bool is_not_empty() const { return aListBuffer.size() <= 0; }
    bool is_not_full() const { return aListBuffer.size() >= aCapacity; }

ClassDef(ARingBuffer, 0)
};




#endif

