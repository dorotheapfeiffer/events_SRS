#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>


/// Root Includes

#include "ReadDataFile.h"

using namespace std;

ClassImp(ReadDataFile)

//============================================================

ReadDataFile::ReadDataFile() {

}
//============================================================

ReadDataFile::ReadDataFile(const std::string& filename) {

   // you have to delete ALL OBJECT EVENTS!!! not only pointers!!!
   //for(it = allEvents.begin(); it != allEvents.end(); ++it){
   //  delete *it;
   //  *it = 0;
   //  }
   allEvents.clear();

   Int_t nrEvents = 0;
 
   struct fileHEADER{
      char header[4096];
   };

   struct eventHEADER{

        UInt_t          eventHeader;  // normaly should be 0xFFFFFFFF
        UInt_t          eventNr;      // event nr per file, if you have more than one files you have to add them all together
        ULong_t         timeStamp;    // general timestamp (if there is more tracks and each has different timestamp)
        UShort_t        nrAllTracks;  // nr tracks per event
        UShort_t        nrAllInstrs;  // nr of instrument in acquisition 
        UInt_t          extraData;    // if this is different than 0 then read nr of bytes equal in this variable
   };

   struct trackHEADER{

        UInt_t          trackheader;     // normaly should be 0xEEEEEEEE
        UInt_t          tracknr;         // 
        ULong_t         tracktimestamp;  //
        Double_t        sampling;        //
        UInt_t          offset;          // 
        UInt_t          range;           // 
        UInt_t          threshold;       // 
        UShort_t        channelnr;       //                     
        UShort_t        instrnr;         //   
        UInt_t          extradatasize;   // 
        UInt_t          datasize;        // 
   };

   fileHEADER   fHEADER;		// file  header
   eventHEADER  eHEADER;		// event header
   trackHEADER  tHEADER;		// track header

   //---------------------- check the size of file 
   //struct stat st;
   //stat(filename.c_str(), &st);
   //size_t size = st.st_size;

   FILE *fin = fopen(Form("%s", filename.c_str()), "r");
   if (fin == NULL) {
      throw  ReadData::BadDigitizerCreation("cannot open file");
      }

   //Char_t aName[30];
   //Char_t aTitle[30];
   //

   while( fread(&eHEADER, sizeof(eHEADER), 1, fin) ){
        if(eHEADER.extraData > 0){ 		//printf("There is extra data, must be read...\n"); 
          Short_t extraDataEvent[eHEADER.extraData];
          fread(extraDataEvent, eHEADER.extraData, 1, fin);
          }
	AEvent *aEvent = new AEvent();	
	//cout << "DEBUG [ReadDataFile::ReadDataFile]::aEvent.nrAllTracks = " << eHEADER.nrAllTracks << endl;

        for(int i = 0; i < eHEADER.nrAllTracks; i++ ){
            fread(&tHEADER, sizeof(tHEADER), 1, fin);

            //sprintf(aName,  "Event_%d:ch_%d", eHEADER.eventNr, i);

            Short_t data[tHEADER.datasize];

            Short_t extraDataTrack[tHEADER.extradatasize];
            fread(extraDataTrack, tHEADER.extradatasize, 1, fin);
            fread(data, sizeof(data), 1, fin);

	    aEvent->AddTrack( new ATrack((Int_t)tHEADER.instrnr, (Int_t)tHEADER.channelnr, tHEADER.sampling, 
                                  tHEADER.offset, tHEADER.range, tHEADER.tracktimestamp, tHEADER.threshold, 
				   data , tHEADER.datasize));
            }
	allEvents.push_back( aEvent );	

    nrEvents++;
    }

    fclose(fin);

   cout << "DEBUG DUPA [ReadDataFile::ReadDataFile] " << filename <<" allEvents.size = " << allEvents.size() << endl;


}


//============================================================


ReadDataFile::~ReadDataFile() {
//cout << "DEBUG [Destructor ReadDataFile ]"<< endl; 
}

//============================================================
 
