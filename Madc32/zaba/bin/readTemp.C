
static std::string Dec2BinStr(Char_t dec){
    std::string strBin;

    for(UInt_t i = 0x80; i > 0; i >>= 1){
       if(i & dec) strBin += '1';
       else        strBin += '0';
       }

return strBin;
}



void readTemp(){

gROOT->Reset();

std::string readStr;

TH1F *h = new TH1F("h","h",4000,0x40000,0x50000);

ifstream inp;
inp.open("temp.txt", ios::in | ios::binary);
int begin = inp.tellg();
inp.seekg(9, ios::end);
int end = inp.tellg();
inp.seekg(9, ios::beg);

int size = end - begin;

Int_t buf[size];
inp.read((char*)buf, size);


for(Int_t i = 0; i < size/4; i++) {

   readStr = Dec2BinStr((buf[i] & 0xFF000000) >> 24) + std::string("'") + Dec2BinStr((buf[i] & 0xFF0000) >> 16) + std::string("'") 
             + Dec2BinStr((buf[i] & 0x0000FF00) >> 8 ) + std::string("'") + Dec2BinStr((buf[i] & 0x0000FF));
   cout << i << " " << readStr << endl;
   if((buf[i] & 0xC0000000 >> 30) == 3){
      h->Fill((buf[i] & 0xCFFFFFFF));
      //printf("%d, 0x%X\n", i % 34, (buf[i] & 0xCFFFFFFF));
      }
   }

h->Draw();
}
