
TCanvas *c = new TCanvas("c","c",1000, 400);

void readTxt(){

   gROOT->Reset();

   unsigned int m_amp = 0;
   unsigned int m_pos = 0;
   unsigned int m_tts = 0;

   c->Divide(2,1);
   TH1F *amplitude = new TH1F("amplitude","amplitude",50,10,35);
   TH1F *position = new TH1F("position","position",4000,0,4000);

   char filename[] = "saat";

   std::ifstream inpfile(filename, std::ios::in);
     if (!inpfile) {
        std::cout << "cannot open file " << filename << std::endl;
        return;
     }

   ULong_t i = 0;
   while( !inpfile.eof() ){
        i++;  
        inpfile >> m_amp >> m_pos >> m_tts;
        //m_amp &= 0x3FF;
        //m_pos &= 0x3FF;
        
        //if(m_amp > 210) 

        amplitude->Fill( m_amp );
        //position->Fill(  m_pos );
        //if( !(i%1000000)) std::cout << i << " " << m_amp << " " << m_pos << " " << m_tts << endl;
      }

   inpfile.close();

   c->cd(1);
   gPad->SetLogy();
   amplitude->Draw();
   c->cd(2);
   gPad->SetLogy();
   position->Draw();
   c->Modified();
   c->Update();
   c->cd();
}
