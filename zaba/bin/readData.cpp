#include <string>
#include <iostream>
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TObjArray.h"
#include "TObject.h"
#include "TIterator.h"

#include "AEvent.h"
#include "ATrack.h"
#include <TApplication.h>
#include "TSystem.h"


using namespace std;

Int_t ReadFile(const char * filename, TObjArray *alist);


int main(Int_t argc, Char_t **argv ){

 TObjArray *aBufferDataList = new TObjArray();

 char filename[200];
 if(argc == 2){
   strcpy(filename, argv[1]);
   }
 else{
  printf("usage: readData [file]\n");
  exit(1);
  }

printf("I have read %d events\n", ReadFile(argv[1], aBufferDataList));



TApplication theApp("App",&argc,argv);
TCanvas *c = new TCanvas("c","c",800, 600);

TObject *e; 
TIterator *it;
it = aBufferDataList->MakeIterator();
while( (e = it->Next() ) > 0 ){
    ((TH1F *)e)->Draw(); 
    c->Modified();
    c->Update();
    gSystem->Sleep(100);
    c->Clear();
    }
  

 theApp.Run();
 
delete aBufferDataList;

return 0;
}


Int_t ReadFile(const char * filename, TObjArray *alist){

   Int_t nrEvents = 0;   

   struct fileHEADER{
      char header[4096];
   };

   struct eventHEADER{

	UInt_t		eventHeader;  // normaly should be 0xFFFFFFFF
	UInt_t		eventNr;      // event nr per file, if you have more than one files you have to add them all together
	ULong_t		timeStamp;    // general timestamp (if there is more tracks and each has different timestamp)
	UShort_t	nrAllTracks;  // nr tracks per event
	UShort_t	nrAllInstrs;  // nr of instrument in acquisition 
	UInt_t		extraData;    // if this is different than 0 then read nr of bytes equal in this variable
   };

   struct trackHEADER{

	UInt_t		trackheader;     // normaly should be 0xEEEEEEEE
	UInt_t		tracknr;	 // 
	ULong_t		tracktimestamp;  //
	Double_t	sampling;        //
	UInt_t		offset;	         // 
	UInt_t		range;	         // 
	UInt_t		threshold;	 // 
	UShort_t	instrnr;         //   
	UShort_t	channelnr;	 //			
	UInt_t		extradatasize;	 // 
	UInt_t		datasize;	 // 
   };

   fileHEADER	fHEADER;
   eventHEADER 	eHEADER;
   trackHEADER	tHEADER;

  
  FILE *fin = fopen(Form("%s", filename), "r");
  if (fin == NULL) {
      printf("Cannot open file \'%s\'\n", filename);
      return 0;
  }

  //read the file header
  //fread(&fHEADER, sizeof(fHEADER), 1, fin);
  //printf("");

  //printf("read event header, sizeof eHEADER  = %lu\n", sizeof(eHEADER));
  //printf("read event header, sizeof tHEADER  = %lu\n", sizeof(tHEADER));
  //printf("read event header, sizeof ULong_t  = %lu\n", sizeof(ULong_t));
  //printf("read event header, sizeof Long_t   = %lu\n", sizeof(Long_t));
  //printf("read event header, sizeof Double_t = %lu\n", sizeof(Double_t));
  //printf("read event header, sizeof UShort_t = %lu\n", sizeof(UShort_t));
  //printf("read event header, sizeof Short_t  = %lu\n", sizeof(Short_t));
  //printf("read event header, sizeof UInt_t   = %lu\n", sizeof(UInt_t));
  //printf("read event header, sizeof Int_t    = %lu\n", sizeof(Int_t));
  // read event header

  //AEvent  *aEvent;
  TH1F	  *h;

  while( fread(&eHEADER, sizeof(eHEADER), 1, fin) ){
     
     //aEvent = new AEvent();
     //aEvent->SetEventNr(eHEADER.eventNr);
 
     //aEvent->SetTimeStamp(eHEADER.timeStamp);
     
     printf("Event header: 0x%X\n",    eHEADER.eventHeader);
     printf("Event number: %d\n",      eHEADER.eventNr    );
     printf("Timestamp: %lu\n",        eHEADER.timeStamp  );
     printf("Nr of all instr:\t%d\n",  eHEADER.nrAllInstrs);
     printf("Nr of all tracks:\t%d\n", eHEADER.nrAllTracks);
     printf("Extra data:\t%d\n",       eHEADER.extraData);

     if(eHEADER.extraData > 0){
       printf("There is extra data, must be read...\n"); 
       Short_t extraDataEvent[eHEADER.extraData];
       fread(extraDataEvent, eHEADER.extraData, 1, fin);
       }

     for(int i = 0; i < eHEADER.nrAllTracks; i++ ){
           char name[100];
           sprintf(name,"%d",eHEADER.eventNr);
           printf("---read %d track header\n", i);
           fread(&tHEADER, sizeof(tHEADER), 1, fin);
           Short_t data[tHEADER.datasize];
           Short_t extraDataTrack[tHEADER.extradatasize];
           fread(extraDataTrack, tHEADER.extradatasize, 1, fin);

           fread(data, sizeof(data), 1, fin);
           h = new TH1F(name, name, tHEADER.datasize, 0, tHEADER.datasize);
           for(UInt_t j = 1; j < tHEADER.datasize; j++)
              h->SetBinContent(j, data[j]);

           //aEvent->AddTrack(new ATrack(tHEADER.instrnr, tHEADER.channelnr, tHEADER.sampling, tHEADER.offset, tHEADER.range, tHEADER.tracktimestamp, 
           //             tHEADER.threshold, data, tHEADER.datasize ));

           alist->Add(h); 
           printf("   Track header:\t0x%X\n",    tHEADER.trackheader);
           printf("   Track number:\t%u\n",      tHEADER.tracknr    );
           printf("   Timestamp:\t%lu\n",      tHEADER.tracktimestamp  );
           printf("   Sampling:\t%e\n",	    tHEADER.sampling);
           printf("   Offset:\t%d\n",	    tHEADER.offset);
           printf("   Range:\t%d\n",	    tHEADER.range);
           printf("   Threshold:\t%d\n",	    tHEADER.threshold);
           printf("   Instrument Nr:\t%d\n",    tHEADER.instrnr);
           printf("   Channel:\t%8d\n",	    tHEADER.channelnr);
           printf("   Extra dataSize:\t%8d\n",   tHEADER.extradatasize);
           printf("   DataSize:\t%8d\n",         tHEADER.datasize);

           }
    nrEvents++;
    }

    fclose(fin);

return nrEvents;
}



/*
//open the root file
TFile *outfile = new TFile(Form("%s.root", filename), "RECREATE");

define the rec tree
TTree *rec = new TTree("rec","rec");
rec->Branch("t", &t   ,"t[1024]/D");  
rec->Branch("chn1", &chn1 ,"chn1[1024]/D");
rec->Branch("chn2", &chn2 ,"chn2[1024]/D");
rec->Branch("chn3", &chn3 ,"chn3[1024]/D");
rec->Branch("chn4", &chn4 ,"chn4[1024]/D");

// loop over all events in data file
for (n=0 ; fread(&header, sizeof(header), 1, f) > 0; n++) {

// decode time      
for (Int_t i=0; i<1024; i++)
t[i] = (Double_t) header.time[i];

fread(&waveform, sizeof(waveform), 1, f);

// decode amplitudes in mV
for (Int_t i=0; i<1024; i++) {
chn1[i] = (Double_t) ((waveform.chn1[i]) / 65535. - 0.5) * 1000;   
chn2[i] = (Double_t) ((waveform.chn2[i]) / 65535. - 0.5) * 1000;   
chn3[i] = (Double_t) ((waveform.chn3[i]) / 65535. - 0.5) * 1000;   
chn4[i] = (Double_t) ((waveform.chn4[i]) / 65535. - 0.5) * 1000;   
}
rec->Fill();
}

// draw channel #4

  // print number of events
cout<<n<<" events processed"<<endl;
cout<<"\""<<Form("%s.root", filename)<<"\" written"<<endl;
// save and close root file
rec->Write();
outfile->Close();
}
*/

