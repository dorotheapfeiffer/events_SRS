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
    bool aEmpty; 
    TMutex aMutex;
    TCondition aBufferNotEmpty;
    TCondition aBufferNotFull;

    list<AEvent> aListBuffer;

public:
    ARingBuffer(UInt_t capacity = 5) : 
        aCapacity(capacity),
	aEmpty(0),
        aMutex(),
        aBufferNotEmpty( &aMutex ),
        aBufferNotFull( &aMutex )
        {}

    virtual ~ARingBuffer(){
    aListBuffer.clear();
    }



//----------------------------------------------------------------
// Write event to the buffer if is not full, if the buffer is full WAIT until is not. 

    void push_front(AEvent& aEvent) {  
        aMutex.Lock();
        while( !is_not_full() ){
	   //cout << "Debug [BUFFER::push_front] czekam " << endl;
           aBufferNotFull.Wait(); 
	   if(aEmpty){ 
	      aMutex.UnLock();
	      //cout << "DEBUG [Buffer::push_front] czyszcze buffer i spadam " << endl;
	      return;
	      }
           }
        aListBuffer.push_front( aEvent ); 
	    
	aMutex.UnLock();
        aBufferNotEmpty.Broadcast();
    }

//----------------------------------------------------------------
// Write to the front of the buffer if is not full, if is full return and DO NOT WAIT!

    void push_front_if_not_full(AEvent& aEvent) {  
        aMutex.Lock();
        if( is_not_full() )
          aListBuffer.push_front( aEvent ); 
	    
	aMutex.UnLock();
        aBufferNotEmpty.Broadcast();
    }




//----------------------------------------------------------------
// read and remove last element from the buffer
//
    bool pop_back(AEvent* aEvent) {
	    
        aMutex.Lock();
        while( is_not_empty() || is_empty()){
           aBufferNotEmpty.Wait();
           }
        *aEvent = aListBuffer.back();
        aListBuffer.pop_back();
        aMutex.UnLock();
        aBufferNotFull.Broadcast();
   return kTRUE;   
  }

//----------------------------------------------------------------
// read and remove last element from the buffer
// _time needed for UpdateWindow in Display, the main thread cannot wait forever if the buffor is empty

    bool pop_back_time(AEvent* aEvent) {
        //cout << "Debug [BUFFER::pop_back_time] I am in  pop_back_time" << endl;
	Int_t retval = 0;    
        aMutex.Lock();
        while( is_not_empty()){
           retval = aBufferNotEmpty.TimedWaitRelative(100);  // 0 - normal signal, 1 - timeout
	   if(retval){
              //cout << "Debug [BUFFER::pop_back_time] timeout occure, break" << endl;
	      aMutex.UnLock(); 
	      return bool(retval);
	      }
           }
           //cout << "Debug [BUFFER::pop_back_time] reading event, retval = " << retval << endl;
        *aEvent = aListBuffer.back();
        aListBuffer.pop_back();
        
	aMutex.UnLock();
        aBufferNotFull.Broadcast();
   return bool(retval);   
  }



//----------------------------------------------------------------
// this function is needed in a situation when save buffer is empty and is waiting for data and the user click end of acquisition
   void clear(){

        aMutex.Lock();
	aEmpty = 1;
	aListBuffer.clear();
        aMutex.UnLock();

        aBufferNotEmpty.Signal();
        aBufferNotFull.Signal();
   }

//----------------------------------------------------------------


private:
    ARingBuffer(const ARingBuffer&);              // Disabled copy constructor
    ARingBuffer& operator = (const ARingBuffer&); // Disabled assign operator

    bool is_not_empty()	const { return aListBuffer.size() == 0; }
    bool is_not_full()	const { return aListBuffer.size() <= aCapacity; }
    bool is_empty()	const { return aEmpty; }

ClassDef(ARingBuffer, 0)
};




#endif

