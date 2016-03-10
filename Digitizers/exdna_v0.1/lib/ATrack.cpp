/*****************************************************************************
*
*****************************************************************************/

#include <iostream>
#include <string>
#include <exception>
#include "ATrack.h"
#include <fstream>


using namespace std;
extern int gDEBUG_EVENT;


UInt_t gTrackNr = 0;

ClassImp(ATrack)


//===============================================================================
ATrack::ATrack(){
     //cout << "Konstruktor default " << endl;
     aData	= NULL;
     aTrackNr	= 0;
     aCardNr	= 0;
     aChannelNr	= 0;
     aOffset	= 0;
     aRange	= 0;
     aTimeStamp = 0;
     aThreshold = 0;
     aDataSize	= 0;
     aSampling	= 0;

     }

//===============================================================================

ATrack::ATrack(Int_t card, Int_t channel, Double_t sampling, Int_t offset, Int_t range, ULong_t timestamp, Int_t threshold, 
              Short_t* data, UInt_t size /*in bytes*/){
     aData = new UShort_t[ size ];
     aTrackNr = gTrackNr++;
     aCardNr = card;
     aChannelNr = channel;
     aOffset = offset;
     aRange  = range;
     aTimeStamp = timestamp;
     aThreshold = threshold;
     aSampling = sampling;
     aDataSize = size;
     memcpy(aData, data, size*sizeof(Short_t));
     //cout << "Konstruktor Track, id = " << aTrackNr << endl;
     }

//===============================================================================
ATrack::ATrack(const ATrack& from){
//cout << " ATrack [constructor kopiujacy] " << aTrackNr << endl;

 aTrackNr	= from.GetTrackNr();
 aCardNr	= from.GetCardNr();
 aOffset	= from.GetOffset();
 aChannelNr	= from.GetChannelNr();
 aSampling	= from.GetSampling();
 aDataSize	= from.GetDataSize();
 aRange		= from.GetRange();
 aTimeStamp	= from.GetTimeStamp();
 aThreshold	= from.GetThreshold();
 aData = new UShort_t[aDataSize];
 memcpy(aData, from.aData, aDataSize * sizeof(Short_t)); 
 
}
//===============================================================================
 ATrack& ATrack::operator= (const ATrack& from){
 //cout << " ATrack [operator =] " << aTrackNr << endl;
 if(&from == this) return *this;

 delete [] aData;

 aTrackNr	= from.GetTrackNr();
 aCardNr	= from.GetCardNr();
 aChannelNr	= from.GetChannelNr();
 aSampling	= from.GetSampling();
 aDataSize	= from.GetDataSize();
 aOffset	= from.GetOffset();
 aRange		= from.GetRange();
 aTimeStamp	= from.GetTimeStamp();
 aThreshold	= from.GetThreshold();

 aData = new UShort_t[aDataSize];
 memcpy(aData, from.aData, aDataSize*sizeof(Short_t)); 
 
 return *this;
 }
//===============================================================================
void ATrack::Write2F(ofstream& fout){

 //****************************  zapisywanie binarne       
 UInt_t aHeader = 0xEEEEEEEE;
 UInt_t extraDataSize = 0x0;

 fout.write( reinterpret_cast<const char*>(&aHeader),      sizeof(UInt_t) );

 fout.write( reinterpret_cast<const char*>(&aTrackNr),     sizeof(UInt_t) );
 fout.write( reinterpret_cast<const char*>(&aTimeStamp),   sizeof(ULong_t) );
 fout.write( reinterpret_cast<const char*>(&aSampling),    sizeof(Double_t) );
 fout.write( reinterpret_cast<const char*>(&aOffset),      sizeof(UInt_t) );
 fout.write( reinterpret_cast<const char*>(&aRange),       sizeof(UInt_t) );
 fout.write( reinterpret_cast<const char*>(&aThreshold),   sizeof(UInt_t) );

 UInt_t card_channel = ((aCardNr << 16) | aChannelNr );
 fout.write( reinterpret_cast<const char*>(&card_channel), sizeof(UInt_t) );

 fout.write( reinterpret_cast<const char*>(&extraDataSize), sizeof(UInt_t)  ); 
 
 fout.write( reinterpret_cast<const char*>(&aDataSize),     sizeof(UInt_t)  ); 

 fout.write( reinterpret_cast<const char*>(aData), aDataSize*sizeof(UShort_t));

 /************************* koniec zapisywania binarnego */

 // zapisywanie txt
  /*
 Int_t aTrackStamp = 0x00000000;
 //fout << " --- Track stamp : 0x" << hex << aTrackStamp << dec << endl;
 //fout << "  -- Card number : " << aCardNr <<"\t Channel number: " << aChannelNr << endl;
 //fout << "  -- Sampling    : " << aSampling << endl;
 //fout << "  -- Data: " << endl;
 for(Int_t i = 0; i < aDataSize; i++){
    //if( (i < 10) || (i > aDataSize - 10) ) 
       fout << "    " << i << " " << aData[i] << endl;
    }
*/

 //koniec zapisu txt     
}

//===============================================================================

UInt_t ATrack::SaveInBuffer(Char_t *aBuffer){


UInt_t aHeader = 0xEEEEEEEE;
UInt_t extraDataSize = 0x0;

UInt_t size = 0;

memcpy(aBuffer,        &aHeader,      sizeof(UInt_t)   );
size += sizeof(UInt_t);

memcpy(aBuffer + size, &aTrackNr,     sizeof(UInt_t)   );
size += sizeof(UInt_t);

memcpy(aBuffer + size, &aTimeStamp,   sizeof(ULong_t) );
size += sizeof(ULong_t);

memcpy(aBuffer + size, &aSampling,    sizeof(Double_t) );
size += sizeof(Double_t);

memcpy(aBuffer + size, &aOffset,      sizeof(UInt_t) );
size += sizeof(UInt_t);

memcpy(aBuffer + size, &aRange,       sizeof(UInt_t) );
size += sizeof(UInt_t);

memcpy(aBuffer + size, &aThreshold,   sizeof(UInt_t) );
size += sizeof(UInt_t);
                
 UInt_t card_channel = ((aCardNr << 16) | aChannelNr );

memcpy(aBuffer + size, &card_channel,             sizeof(UInt_t) );
size += sizeof(UInt_t);

memcpy(aBuffer + size, &extraDataSize,            sizeof(UInt_t) );
size += sizeof(UInt_t);

memcpy(aBuffer + size, &aDataSize,                sizeof(UInt_t) );
size += sizeof(UInt_t);

memcpy(aBuffer + size, aData,         aDataSize * sizeof(UShort_t) );
size += (aDataSize * sizeof(UShort_t));

return size;
}
//===============================================================================
/*
Int_t ATrack::AddData(Int_t card, Int_t channel, Double_t sampling, Int_t offset, Int_t range, ULong_t timestamp, Int_t threshold, 
              Short_t* data, Int_t size ){ // size in bytes

     if(dataSize != aDataSize){
        delete [] aData;
        aData = new Short_t [ dataSize ];
       }
     aDataSize = dataSize;
      
     memcpy(aData, data, dataSize*sizeof(Short_t));
 
}
*/
//===============================================================================
Int_t ATrack::AddData(Int_t card, Int_t channel, UShort_t* data, UInt_t dataSize /*in bytes*/){
     card = channel = 0;
     if(dataSize != aDataSize){
        delete [] aData;
        aData = new UShort_t [ dataSize ];
       }
     aDataSize = dataSize;
     memcpy(aData, data, dataSize*sizeof(Short_t));
 return 1;
 }

//===============================================================================
/* 
Double_t ATrack::BaseLineCorrection(UInt_t n){

Double_t avg = 0;
if(n > 0 && n < aDataSize){
    for(UInt_t i = 0; i < n; i++) {
       avg += aData[i];
       }
    avg /= Float_t(n);

  for(UInt_t i=0; i < aDataSize; i++){
       aData[i] -= avg;
       }
    }
//cout << "avg = " << avg << endl;
return avg;

}
*/
//===============================================================================

ATrack::~ATrack(){
  #ifdef DEBUG
    if(gDEBUG_EVENT > 3) cout << "Destruktor Track, id = " << aTrackNr << endl;
  #endif 
    delete [] aData;
    gTrackNr--;
    }


//===============================================================================

