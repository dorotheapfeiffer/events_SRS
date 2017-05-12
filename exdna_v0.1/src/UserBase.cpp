#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <TString.h>

/// Root Includes
#include <TObject.h>
#include <TTimer.h>

#include "UserBase.h"

using namespace std;

ClassImp(UserBase)

//============================================================

UserBase::UserBase() {
cout << "Constructor UserBase "<< endl; 
retval = kTRUE;
 time = new TTimer();
 time->Connect("Timeout()","UserBase",this,"Refresh()");
 iNbEvent=0;
 OutFileName = new TString("./out.root");

}

//============================================================

Bool_t UserBase::ReturnStatus(){
  Bool_t temp = retval;
  retval = kTRUE;
return temp;
}

//============================================================
     
void UserBase::TimeRefresh(Int_t ms){
  //cout << "DEBUG [UserBase::TimeRefresh] " << ms << "[ms]" << endl; 
// time->Start(ms, kFALSE);
 if(ms>0){
   iTimeRefresh=ms;
   time->Start(iTimeRefresh, kFALSE);
 }
 else{ 
   iTimeRefresh=0;
   time->Stop();
 }
 // }
}

//============================================================

void UserBase::EventRefresh(Int_t event){
if(event <= 0) return;
 iEventRefresh=event;
 //cout << "DEBUG [UserBase::EventRefresh]\t "<<iEventRefresh<< endl; 
}

//============================================================

void UserBase::Pause(){
retval = kFALSE;
cout << "DEBUG [UserBase::EventRefresh] "<< endl; 
}

//============================================================

UserBase::~UserBase() {
cout << "DEBUG [Destructor UserBase ]"<< endl; 
 time->Stop();
 cout<<"time stop"<<endl;
 delete time;
}

//============================================================

Bool_t UserBase::DoBaseAnalysis(CTrack** cCTracks, Int_t aNr){
  DoAnalysis(cCTracks, aNr);
  iNbEvent++;
  if((iNbEvent%iEventRefresh)==0)Refresh();
  return ReturnStatus();
}

//============================================================

void UserBase::Refresh(){
  cout<<"No UserClass Refresh function defined"<<endl;
}

//============================================================
/*void UserBase::SetCurrentFile(TString s){
  cout<<"a"<<endl;
  iUserCurrentFile = "stu";
  cout<<"b"<<endl;
  }*/
/*void UserBase::SetCurrentFile(){
  cout<<"a"<<endl;
  iUserCurrentFile = "stu";
  cout<<"b"<<endl;
  }*/
 
