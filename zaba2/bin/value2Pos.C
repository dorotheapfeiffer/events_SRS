

Int_t value2Pos(Int_t value){
  
   Int_t pos = 0;
   const Int_t size = 21;
   Int_t tab[size] = {100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300};
   
    if(value < tab[0]) return pos;
    else if(value > tab[size-1]) return size;
    else return pos = size * (int)(float(value) - (float)tab[0]) / ((float)tab[size-1] - (float)tab[0]);
}

