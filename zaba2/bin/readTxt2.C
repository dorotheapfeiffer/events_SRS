
TCanvas *c = new TCanvas("c","c",1000, 400);

void readTxt(){

   gROOT->Reset();

   unsigned int m_ch = 0;
   unsigned int m_charge = 0;
   unsigned int m_tts = 0;

   c->Divide(3,1);
   TH1F *channel = new TH1F("channel","channel",64,0,64);
   TH1F *charge = new TH1F("charge","charge",4000,0,4000);
   TH1F *timestamp = new TH1F("timestamp","timestamp",10000,0,100000);

   char filename[] = "saat";

   std::ifstream inpfile("2016_11_25_1303_data_000.lst1", std::ios::in);
     if (!inpfile) {
        std::cout << "cannot open file " << filename << std::endl;
        return;
     }

   ULong_t i = 0;
   while( !inpfile.eof() ){
        i++;  
        inpfile >> m_ch >> m_charge >> m_tts;
        //m_ch &= 0x3FF;
        //m_charge &= 0x3FF;
        
        //if(m_ch > 210) 

        channel->Fill( m_ch );
        charge->Fill(  m_charge );
        timestamp->Fill(m_tts);
        //if( !(i%1000000)) std::cout << i << " " << m_ch << " " << m_charge << " " << m_tts << endl;
      }

   inpfile.close();

   c->cd(1);
   gPad->SetLogy();
   channel->Draw();
   c->cd(2);
   gPad->SetLogy();
   charge->Draw();
   c->cd(3);
   timestamp->Draw();
 
   c->Modified();
   c->Update();
   c->cd();
}
