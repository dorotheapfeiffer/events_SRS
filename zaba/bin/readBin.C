
#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TMath.h"

typedef struct {
   UInt_t	n_words	   : 12;
   UInt_t	adc_res	   : 3;
   UInt_t	out_format : 1;
   UInt_t	module_id  : 8;	
   UInt_t	sub_header : 6;
   UInt_t	header_sig : 2;
} EventHeader;

typedef struct {
   UInt_t	adc_data   : 14;
   UInt_t	overflow   : 1;
   UInt_t	nop        : 1;
   UInt_t	channel	   : 5;	
   UInt_t	fix	   : 9;
   UInt_t	data_sig   : 2;
} DataWord;

typedef struct {
   UInt_t	counter_tts: 30;
   UInt_t	end_event  : 2;
} EndOfEvent;

static union {
   UInt_t	m_Header;
   EventHeader  m_EventHeader;
};

static union {
   UInt_t	m_Word;
   DataWord	m_DataWord;
};

static union {
   UInt_t	m_Footer;
   EndOfEvent	m_EndOfEvent;
};



Int_t value2Pos(float *table, Int_t value){
  
   //Long64_t a = TMath::BinarySearch((Long64_t)65, (const float*)tab, (Int_t)value); 
  
   for(Int_t i = 0; i < 65; i++){
       if( value < table[i] )
         return i;
      }

return -1;
}

//=====================================================================================================
Int_t adc2pos(Int_t bin){
// pos = 22mV/bin + 460(fixed_offset_adc) + 355*address;
// values 0.46 - 3.3 V

    Int_t fixed_offset_adc = 400;
    Int_t address = 0;
    
    //printf("bimf:%d \t posf: %d \t |", bin, (( Double_t(bin) - 400. ) / 22. )+0.5); 
    return ((bin - fixed_offset_adc -  355*address ) / 22) + 0.5;
}

//=====================================================================================================
 /*
    if(value < tab[0]) return pos;
    else if(value > tab[size-1]) return size;
    else return pos = size * (int)(value - tab[0]) / (tab[size-1] - tab[0]);  
*/


//=====================================================================================================


void readBin(const char* filename) {

//====================================================================================================
   const Int_t size = 65;
   float tabx[ size ] = { 535.,  545.,  565.,  585.,  605.,  625.,  645.,  665.,  685.,  711.,  733.,  753.,  775.,  797.,  816.,  838.,  
                          866.,  884.,  905.,  923.,  944.,  967.,  989., 1011., 1032., 1054., 1072., 1095., 1118., 1140., 1162., 1182., 
                         1204., 1229., 1247., 1268., 1289., 1307., 1328., 1350., 1373., 1393., 1412., 1434., 1456., 1480., 1500., 1520., 
                         1547., 1570., 1591., 1632., 1655., 1675., 1695., 1720., 1745., 1765., 1785., 1810., 1832., 1854., 1878., 1900., 
                         1921.};
   float taby[ size ];

   for (Int_t nri = 0; nri < 65; nri++){ 
       taby[nri] = 100.;  
       }

   TH1F *adc1 = new TH1F("adc1", "adc1", 2000, 0, 4000);
   TH1F *adc2 = new TH1F("adc2", "adc2", 2000, 0, 4000);
   TH1F *adc3 = new TH1F("adc3", "adc3", 2000, 0, 4000);
   TH1F *adc4 = new TH1F("adc4", "adc4", 2000, 0, 4000);
   TH1F *adc5 = new TH1F("adc5", "adc5", 2000, 0, 4000);
   TH1F *adc6 = new TH1F("adc6", "adc6", 2000, 0, 4000);
   TH1F *adc7 = new TH1F("adc7", "adc7", 2000, 0, 4000);
   TH1F *adc8 = new TH1F("adc8", "adc8", 2000, 0, 4000);
   TH1F *tts  = new TH1F("tts", "pos", 1000000, 10, 1000000000);

   //Bool_t DEBUG = kTRUE;
   Bool_t DEBUG = kFALSE;
   Int_t wrong = 0;

   printf("%s\n", filename);
   FILE *f = fopen(Form("%s", filename), "r");
   if (f == NULL) {
      printf("Cannot find file \'%s\'\n", filename);
      return;
      }

   UInt_t i = 0;
   while( fread(&m_Header, sizeof(m_Header), 1, f) > 0){
     if(DEBUG) printf("read data 0x%X in the data stream, wordnr: %d, header_sig: %d\n", m_Header, i, m_EventHeader.header_sig); 
     if(m_EventHeader.header_sig == 0x1){
        if(DEBUG){
           printf(" + Found data 0x%X in the data stream, nr %d\n", m_Header, i); 
           printf("	header_sig: %d\n", m_EventHeader.header_sig ); 
           printf("	sub_header: %d\n", m_EventHeader.sub_header ); 
           printf("	module_id : %d\n", m_EventHeader.module_id  ); 
           printf("	out_format: %d\n", m_EventHeader.out_format ); 
           printf("	adc_res	  : %d\n", m_EventHeader.adc_res    ); 
           printf("	n_words	  : %d\n", m_EventHeader.n_words    ); 
          }
        UInt_t m_value_adc[33];;
        Int_t pos1;
        for(Int_t n = 0; n < m_EventHeader.n_words-1; n++){
            fread(&m_Word, sizeof(m_Word), 1, f);

            m_value_adc[ m_DataWord.channel ] = m_DataWord.adc_data;

            if(DEBUG) printf("	dsig: %d, chn: %d, over: %d, adc: %d\n",m_DataWord.data_sig, m_DataWord.channel, 
                                                                        m_DataWord.overflow, m_DataWord.adc_data); 
           }
        fread(&m_Footer, sizeof(m_Footer), 1, f);
        m_value_adc[32 ] = m_EndOfEvent.counter_tts; 
        
        adc1->Fill(m_value_adc[0]);  
        adc2->Fill(m_value_adc[1]);  
        adc3->Fill(m_value_adc[2]);  
        adc4->Fill(m_value_adc[3]);  
        adc5->Fill(m_value_adc[4]);  
        adc6->Fill(m_value_adc[5]);  
        adc7->Fill(m_value_adc[6]);  
        adc8->Fill(m_value_adc[7]);  

        tts->Fill(m_value_adc[32]);  
        cout << m_value_adc[32] << endl;
        }
       
       
       else{
         cout << "something is wrong there is no header signal "<< wrong++ << endl;
       }
       //break;
  
     i++;  
    //if( !(i % 1000)) printf("read %d words\n", i);
    }

  printf("read %d words, wrong = %d\n", i, wrong);
  fclose(f); 

  TCanvas *c = new TCanvas("c","c",800, 600);
  TCanvas *c1 = new TCanvas("c1","c1",800, 600);
  c->Divide(4,2);
  //...............
  c->cd(1); adc1->Draw(); gPad->SetLogy();
  c->cd(2); adc2->Draw(); gPad->SetLogy();
  c->cd(3); adc3->Draw(); gPad->SetLogy();
  c->cd(4); adc4->Draw(); gPad->SetLogy();
  c->cd(5); adc5->Draw(); gPad->SetLogy();
  c->cd(6); adc6->Draw(); gPad->SetLogy();
  c->cd(7); adc7->Draw(); gPad->SetLogy();
  c->cd(8); adc8->Draw(); gPad->SetLogy();
  //...............
  c->cd();
  c1->cd();
  tts->Draw();
  
//////////////////////////////////////////////////////////////////
// pos = 22mV/bin + 460(fixed_offset_adc) + 355*address;
//////////////////////////////////////////////////////////////////
}
