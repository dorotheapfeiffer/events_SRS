#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "TFile.h"




// Root Includes
#include "DData.h"

using namespace std;

ClassImp(DData)

//============================================================


DData::DData( TString file )  : TObject() {
 
     char tab[ file.Length()+1 ];     
     strcpy( tab, file.Data() );
     string filename (tab);
    

     size_t pos = filename.find_last_of('.');
     string extension = filename.substr(pos);

     cout << "DEBUG [DData::DData] extension file" << extension << endl;
  
     allEvents.clear();

     Int_t nrEvents = 0;
 
//==============================================================================================
     if( extension.compare(".bin") == 0){ //reading file with bin extension (Anton Madc32)



	typedef struct {
   		UInt_t       n_words    : 12;
   		UInt_t       adc_res    : 3;
   		UInt_t       out_format : 1;
   		UInt_t       module_id  : 8;
   		UInt_t       sub_header : 6;
   		UInt_t       header_sig : 2;
		} EventHeader;

	typedef struct {
   		UInt_t       adc_data   : 14;
   		UInt_t       overflow   : 1;
   		UInt_t       nop        : 1;
   		UInt_t       channel    : 5;
   		UInt_t       fix        : 9;
   		UInt_t       data_sig   : 2;
	} DataWord;

	typedef struct {
   		UInt_t       counter_tts: 30;
   		UInt_t       end_event  : 2;
	} EndOfEvent;

	static union {
   		UInt_t       m_Header;
   		EventHeader  m_EventHeader;
	};

	static union {
   		UInt_t       m_Word;
   		DataWord     m_DataWord;
	};

	static union {
   		UInt_t       m_Footer;
   		EndOfEvent   m_EndOfEvent;
	};



      int DEBUG = 0; 
      //cout << "[DEBUG] bede czytal plik bin " << endl;


      FILE *fin = fopen(Form("%s", filename.c_str()), "r");
      if (fin == NULL) {
         cout << "cannot open file" << endl;
         }

     UInt_t i = 0;
     while( fread(&m_Header, sizeof(m_Header), 1, fin) > 0){
	 AEvent *aEvent = new AEvent();	
         if(DEBUG) printf("read data 0x%X in the data stream, wordnr: %d, header_sig: %d\n", m_Header, i, m_EventHeader.header_sig);
         if(m_EventHeader.header_sig == 0x1){
            if(DEBUG){
               printf(" + Found data 0x%X in the data stream, nr %d\n", m_Header, i);
               printf("     header_sig: %d\n", m_EventHeader.header_sig );
               printf("     sub_header: %d\n", m_EventHeader.sub_header );
               printf("     module_id : %d\n", m_EventHeader.module_id  );
               printf("     out_format: %d\n", m_EventHeader.out_format );
               printf("     adc_res   : %d\n", m_EventHeader.adc_res    );
               printf("     n_words   : %d\n", m_EventHeader.n_words    );
               }

        Short_t m_value_adc[32]; //keep all adc value from the digitizer it has always 11-13 bits, short has 16bits, OK
        
        for(Int_t n = 0; n < m_EventHeader.n_words-1; n++){
            fread(&m_Word, sizeof(m_Word), 1, fin);
            //if(m_DataWord.channel == n)  value[n] = m_DataWord.adc_data;
            m_value_adc[ m_DataWord.channel ] = m_DataWord.adc_data;

            if(DEBUG) cout << "from file: " << m_value_adc[n] << endl;
            if(DEBUG) printf("  dsig: %d, chn: %d, over: %d, adc: %d\n",m_DataWord.data_sig, m_DataWord.channel,
                                                                        m_DataWord.overflow, m_DataWord.adc_data);
           }
        fread(&m_Footer, sizeof(m_Footer), 1, fin);
        UInt_t m_value_tts = m_EndOfEvent.counter_tts;
        if(DEBUG) printf("  dsig: %d, tts: %d\n",m_EndOfEvent.end_event, m_EndOfEvent.counter_tts); 

        for(Int_t n = 0; n < m_EventHeader.n_words-1; n++){
            Short_t adc[1];
            adc[0] = m_value_adc[n];
	    aEvent->AddTrack( new ATrack((Int_t)m_EventHeader.module_id, 
                                         (Int_t)n,           // ch nr 
                                                15e6,        // VME sampling 
                                                0,           // offset
                                                0,           // range
                                                m_value_tts, // tts
                                                0,           // threskold
		                                adc,         // adc conversion
                                        (UInt_t) 1)) ;       // data size

            if(DEBUG){
               cout << "aEvent->write: " << m_value_adc[n] << endl;
               cout <<"nr event: "<< i <<"\tid: "<< m_EventHeader.module_id <<"\tch: "<< n <<"\ttts: "<< m_value_tts <<"\tadc: "<< adc[0] << endl;  
               UShort_t *ptr = aEvent->GetTrack(n)->GetData();
               UShort_t adcread = ptr[0];
               cout << "aEvent->GetTrack(n)->GetData(): "<< adcread << endl;
               }
	    allEvents.push_back( aEvent );	
            nrEvents++;
            }
         if(DEBUG) cout << endl; 
         }
      i++;
      }
     fclose(fin);

     cout << "DEBUG [DData] loaded succesfull from .bin files "<< endl;
     } // end of reading bin files

//=================================================================================================================

   else if ( extension.compare(".dat") == 0) { //reading file with dat extension (Francecso CAEB digitizer)
 
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

 //  	fileHEADER   fHEADER;		// file  header at the moment not used...
   	eventHEADER  eHEADER;		// event header
   	trackHEADER  tHEADER;		// track header




 
 
   //---------------------- check the size of file 
   //struct stat st;
   //stat(filename.c_str(), &st);
   //size_t size = st.st_size;

   FILE *fin = fopen(Form("%s", filename.c_str()), "r");
   if (fin == NULL) {
         cout << "cannot open file" << endl;
      
      }

   //Char_t aName[30];
   //Char_t aTitle[30];
   //

      char txtname[500];
      std::string str = filename;
      std::size_t found = str.find_last_of("/");
      strcpy(txtname, (char*)"output/");
      strcpy(txtname + strlen(txtname), str.substr(found+1).c_str());
      strcpy(txtname + strlen(txtname) - 4, (char*)".txt");   
      cout << "txtname: "<< txtname << endl;

   while( fread(&eHEADER, sizeof(eHEADER), 1, fin) ){
        if(eHEADER.extraData > 0){ 		//printf("There is extra data, must be read...\n"); 
          Short_t extraDataEvent[eHEADER.extraData];
          fread(extraDataEvent, eHEADER.extraData, 1, fin);
          }
	AEvent *aEvent = new AEvent();	
	//cout << "DEBUG [DData::DData]::aEvent.nrAllTracks = " << eHEADER.nrAllTracks << endl;

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

       ofstream fout(txtname, std::ofstream::out | std::ofstream::app);
       aEvent->Write2F( fout);
       fout.close(); 
    nrEvents++;
    }

    fclose(fin);
   } // emd of dat extension



   cout << "DEBUG [DData::DData] exit, " << filename <<" allEvents.size = " << allEvents.size() << endl;


}


//============================================================


DData::~DData() {

cout << "DEBUG [Destructor DData ]"<< endl; 
UInt_t i = 0;

cout << "DEBUG [Destructor DData ] allEventSize = " << allEvents.size() << endl; 
   for( i = 0; i < allEvents.size(); i++) {
        //allEvents.at(i)->EventShow();
        //delete  allEvents.at(i); 

        //allEvents.at(i) = 0;
         } 



//  for (AEvent* aE : allEvents){   
//       cout << "bede casowal event nr " << i << endl;
//       aE->EventShow();
//       delete *it;
//      }

 // allEvents.clear();

cout << "DEBUG [Destructor DData ] skasowalem wszystko z bufora i wyczyscilem"<< endl;


}

//============================================================ 
