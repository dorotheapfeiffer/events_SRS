/*****************************************************************************
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "AEvent.h"

using namespace std;
extern int gDEBUG_EVENT; 
UInt_t gEvent = 0;
ClassImp(AEvent)


//===============================================================================
AEvent::AEvent(Int_t a){
  aTrackVector.reserve(a);
  aEventNr = -1;
  aExtraDataSize = 0;
}

//===============================================================================
AEvent::~AEvent(){
#ifdef DEBUG
    if(gDEBUG_EVENT > 3) cout << "DEBUG [AEvent:Destructor] Delete AEvent aEventNr = " << aEventNr << " + tracks " << endl;
#endif    
  //cout << "[AEvent DES] Clear vector, " << aTrackVector.size() << " elements" << endl;

  aEventNr = -1;
  for(it = aTrackVector.begin(); it != aTrackVector.end(); ++it) {
      delete *it;
      *it = 0;
      }
  aTrackVector.clear( );
}

//===============================================================================
AEvent& AEvent::operator= (const AEvent& from){

#ifdef DEBUG
    if(gDEBUG_EVENT > 3) cout << "DEBUG [AEvent::operator =] " << aEventNr << endl;
#endif    
if(&from == this) return *this;

//cout << "DEBUG [AEvent::operator =] aTrackVector.size (PRZED) = " << aTrackVector.size() << endl;
  for(it = aTrackVector.begin(); it != aTrackVector.end(); ++it) {
      delete *it;
      *it = 0;
      }
aTrackVector.clear();
//cout << "DEBUG [AEvent::operator =] aTrackVector.size (PO) = " << aTrackVector.size() << endl;
aTrackVector.reserve(from.GetNrTracks());
aEventNr = from.GetEventNr();

for(unsigned int i = 0; i < from.GetNrTracks(); i++){
   aTrackVector.push_back( new ATrack( (ATrack&)*from.GetTrack(i) ) );
   }

return *this;
}
//===============================================================================
AEvent::AEvent(const AEvent& from){
#ifdef DEBUG
    if(gDEBUG_EVENT > 3) cout << "DEBUG [AEvent::copy constructor ] " << from.GetEventNr() << endl;
#endif    
aEventNr = from.GetEventNr();
aTrackVector.reserve(from.GetNrTracks());

for(unsigned int i = 0; i < from.GetNrTracks(); i++){
   aTrackVector.push_back( new ATrack( (ATrack&)*from.GetTrack(i) ) );
   }

}
//===============================================================================
void AEvent::AddTrack(ATrack* aTrack){
#ifdef DEBUG
    if(gDEBUG_EVENT > 3) cout << "DEBUG [AEvent::AddTrack ] " << endl;
#endif    
    aTrackVector.push_back(aTrack); 
}
//===============================================================================
void AEvent::Write2F(ofstream& fout){

/********* zapisywanie binarne	
*/
Int_t aEventHeader = 0xFFFFFFFF;

aExtraDataSize = 0;

//fout.write( reinterpret_cast<const char*>(header), sizeof(Char_t));
fout.write( reinterpret_cast<const char*>(&aEventHeader), sizeof(Int_t));
fout.write( reinterpret_cast<const char*>(&aEventNr), sizeof(Int_t));
fout.write( reinterpret_cast<const char*>(&aTimeStamp), sizeof(ULong_t));
Int_t cards = 0;
Int_t cards_tracks = ((cards << 16) | aTrackVector.size());
fout.write( reinterpret_cast<const char*>(&cards_tracks), sizeof(Int_t) );
fout.write( reinterpret_cast<const char*>(&aExtraDataSize), sizeof(Int_t) );

   for(it = aTrackVector.begin(); it != aTrackVector.end(); ++it){
     (*it)->Write2F(fout);
     }
/*koniec zapisu binarnego*******/

//zapis txt
/*
Int_t aEventStamp = 0xFFFFFFFF;
fout << " -- Event Stamp:  0x" << hex << aEventStamp << dec << endl;
fout << "  - Event number: " << aEventNr << endl;
fout << "  - Nr of Track: " << aTrackVector.size() << endl;
   for(it = aTrackVector.begin(); it != aTrackVector.end(); ++it){
     (*it)->Write2F(fout);
     }
*/

}


//===============================================================================

UInt_t AEvent::SaveInBuffer(Char_t *aBuffer){


Int_t aEventHeader = 0xFFFFFFFF;
aExtraDataSize = 0;
Int_t size = 0;

memcpy(aBuffer,        &aEventHeader,  sizeof(Int_t) );
size += sizeof(Int_t);

memcpy(aBuffer + size, &aEventNr,      sizeof(Int_t) );
size += sizeof(Int_t);

memcpy(aBuffer + size, &aTimeStamp,    sizeof(ULong_t) );
size += sizeof(ULong_t);

Int_t cards = 0;
Int_t cards_tracks = ((cards << 16) | aTrackVector.size());

memcpy(aBuffer + size, &cards_tracks,  sizeof(Int_t) );
size += sizeof(Int_t);

memcpy(aBuffer + size, &aExtraDataSize,sizeof(Int_t) );
size += sizeof(Int_t);


   for(it = aTrackVector.begin(); it != aTrackVector.end(); ++it){
     size += (*it)->SaveInBuffer(aBuffer + size);
     }

return size; 
}
//===============================================================================

void AEvent::RemoveTrack(UInt_t id){
    for(unsigned i = 0; i < aTrackVector.size(); i++)
        if(aTrackVector.at(i)->GetTrackNr() == id)
           aTrackVector.erase(aTrackVector.begin()+i); 

}
//===============================================================================
void AEvent::ClearEvent(){
//    cout << "[AEvent::Clear] Clear event" << endl;
    aEventNr = -1;
//    cout << "[AEvent::Clear] vector size (before) = " << aTrackVector.size() << endl;
   // for(it = aTrackVector.begin(); it != aTrackVector.end(); ++it) {
   //    delete *it;
   //    *it = 0;
   //    }

  //  cout << "[AEvent::Clear] vector size (before) = " << aTrackVector.size() << endl;
    aTrackVector.clear();
  //  cout << "[AEvent::Clear] vector size (after) = " << aTrackVector.size() << endl;
}

//===============================================================================
 ULong_t AEvent::GetEventSize() const{
       ULong_t sum = 0;
       for(UInt_t i = 0; i < aTrackVector.size(); i++)
          sum += aTrackVector.at(i)->GetDataSize();
       
 return sum ;
 }

//===============================================================================

void AEvent::EventShow(){
 cout << "Event ID = " << aEventNr << endl;
 for(UInt_t i = 0; i < aTrackVector.size(); i++){ 
    cout << "-+ Track nr = " << i << endl;
       UShort_t* ptr = aTrackVector.at(i)->GetData();
       cout << ptr[0]  << " " << ptr[1]  << " " << ptr[2]  << " " << ptr[3]  << " " 
            << ptr[4]  << " " << ptr[5]  << " " << ptr[6]  << " " << ptr[7]  << " " << endl;
       cout << ptr[8]  << " " << ptr[9]  << " " << ptr[10] << " " << ptr[11] << " " 
            << ptr[12] << " " << ptr[13] << " " << ptr[14] << " " << ptr[15] << " " << endl;
    }
}
