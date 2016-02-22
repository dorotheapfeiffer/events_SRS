/*****************************************************************************
*
* This is anoter immplementation of DataSave WITHOUT multithread buffer
* This immplementation is a simply one buffer, to check whether is faster than multithread or not.
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <exception>
#include <ctime>
#include <TDatime.h>

#include <TGFileDialog.h>

#include "ADataSave2.h"
#include "AManager.h"

using namespace std;

extern int gDEBUG_SAVE;

ClassImp(ADataSave2)

ADataSave2::ADataSave2(){} 

			     
ADataSave2::ADataSave2(bool a) 
{
   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave2::ADataSave2]" );
   #endif

  aManager	= &AManager::GetInstance();

  // Flie dialog to get the file name
  static TString dir(".");
  TGFileInfo fi;
  fi.fIniDir    = StrDup(dir);
  new TGFileDialog(gClient->GetRoot(), NULL, kFDSave, &fi);
  dir = fi.fIniDir;
  if(fi.fFilename){
    //int l = (fi.fFilename).Last('_');
    //   if(l > 0) (fi.fFilename) = (fi.fFilename).Remove((fi.fFilename).Last('_'));
    aManager->SetDataFileName(fi.fFilename);
    }

//  aManager->SetDataFileName("test");

  aMaxFileSize 	= aManager->GetFileSize() * 1024 * 1024; // to have it in MB
  aBuffer	= new Char_t [ aMaxFileSize + (40 * 1024*1024) ]; 	 // *1.2 to have some extra space...
  aBufferPos	= 0;
  aBufferCounter= 0;
  aMaxFiles 	= aManager->GetMaxFiles(); 
  aEventCounter	= 0;
  aFileNr	= 0;
  aEmptyBuffer  = 0;


  // a log file for the aquisition

  TString aFileLog = aManager->GetDataFileName();
  ofstream Logout(aFileLog, ios::out);
  if(!Logout.is_open()) { 
     cout << "ERROR could not open the file... " << aFileLog << endl; 
    }
  else{
    TDatime t;
    Logout << "Acquisition starts: " << t.GetYear() << "." << t.GetMonth() << "." << t.GetDay() << " at " << t.GetHour() << ":" << t.GetSecond() << endl;
    Logout.close();
    }
   

  
}

//===============================================================================

ADataSave2::~ADataSave2(){
   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave2::~ADataSave2]" );
   #endif
  delete [] aBuffer; 
  aManager = 0;
}


//===============================================================================
void ADataSave2::OpenNewFile(){

   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave2::OpenNewFile] " );
   #endif

  char charNr[10];
  TString aFileName;
  TDatime t;

  if(fout.is_open()) { 
      cout << "The file: " << aManager->GetDataFileName() << " was opened, close it..." << endl;
      fout.close();
      }

  if(aFileNr < aMaxFiles || aMaxFiles == 0){

     sprintf(charNr, "_%03d.dat", aFileNr++);
     aFileName = aManager->GetDataFileName() + charNr;

     fout.open(aFileName, ios::out);
     if(!fout.is_open()) { 
       cout << "could not open file... " << aFileName << endl; 
       }
     else{
       fout.write((char *) aBuffer, aBufferPos);
       fout.close();
       cout << "New file (" << aFileNr << "/" << aMaxFiles << ")   [" << aFileName << "] was created at " << t.GetHour() << ":" << t.GetSecond() << endl;
       }
    }


  // writing information to acquisition log file
  TString aFileLog = aManager->GetDataFileName();
  ofstream Logout(aFileLog, std::ofstream::out | std::ofstream::app);
  if(!Logout.is_open()) { 
     cout << "ERROR could not open the file... " << aFileName << endl; 
    }
  else{
    Logout << "New file [" << aFileName << "] was created at" << t.GetHour() << ":" << t.GetSecond() << endl;
    Logout.close();
    }
  

}

//===============================================================================

void ADataSave2::InitDataSave(){

   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave2::InitDataSave]");
   #endif
 
  char charNr[10];   
  sprintf(charNr, "_%03d.dat", aFileNr++);
  TString aFileName = aManager->GetDataFileName() + charNr;
  fout.open(aFileName, ios::out);
  if(!fout.is_open()) { 
     cout << "ERROR could not open the file... " << aFileName << endl; 
    }

}

//===============================================================================

void ADataSave2::DeleteDataSave(){

   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave2::DeleteDataSave] " );
   #endif

  if(fout.is_open()) { 
      fout.close();
      }



}

//===============================================================================
UInt_t ADataSave2::SaveInBuffer(AEvent &aEvent){

   //cout << "DEBUG [ADataSave2::SaveInBuffer] aBufferPos = " << aBufferPos << " aMaxFileSize = " << aMaxFileSize << endl;
   aBufferPos += aEvent.SaveInBuffer(aBuffer + aBufferPos);
   if( (aBufferPos > aMaxFileSize) ){
      OpenNewFile(); 
      aBufferCounter = 0;
      aBufferPos = 0;
      cout << "DEBUG [ADataSave2::SaveInBuffer] Buffer save to file, aBufferPos = " << aBufferPos << endl;
      }
 
   aManager->SetFileNr(aFileNr);  

return 0;

  
}
//===============================================================================
/*
        aEvent.Write2F(p->fout);
        aFileSize += 2*aEvent.GetEventSize();
        if( (aFileSize > p->aMaxFileSize) ){
           p->OpenNewFile(); 
	   aFileSize = 0;
           }
        }
*/

//==========================================================================
void ADataSave2::EmptyBuffer(){

         TThread::Printf(" Empty buffer is not implemented yet, some data could not be stored....");	
}

//==========================================================================
/*
void ADataSave2::SetHeader(const char* header){
for(int i = 0; i < mCards; i++)
   {
   cout << mAcqDev[i]->GetName()        << endl;
   cout << mAcqDev[i]->GetMemSize()     << endl;
   //cout << mAcqDev[i]->GetPostTrigger() << endl;
   //cout << mAcqDev[i]->GetEnableCh(0)   << endl;
   //cout << mAcqDev[i]->GetEnableCh(1)   << endl;
   //cout << mAcqDev[i]->GetClockValue()  << endl;
   }
}

*/



