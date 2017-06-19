/*****************************************************************************
*
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <exception>
#include <ctime>

#include <TThread.h>
#include <TMutex.h>
#include <TGFileDialog.h>

#include "ADataSave.h"
#include "ARingBuffer.h"
#include "AManager.h"

using namespace std;

extern int gDEBUG_SAVE;

ClassImp(ADataSave)

ADataSave::ADataSave(){} 

			     
ADataSave::ADataSave(bool a) : aRun(a), aRingBuffer(1000), aThread("aThread", ThreadFunc, this ) 
{
   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::ADataSave]" );
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

  aManager->SetDataFileName("test");

  aMaxFileSize 	= aManager->GetFileSize() * 1024 * 1024; // to have it in MB
  aMaxFiles 	= aManager->GetMaxFiles(); 
  aEventCounter	= 0;
  aFileNr	= 0;
  aEmptyBuffer  = 0;
  aThread.Run(this);
}

//===============================================================================

ADataSave::~ADataSave(){
   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::~ADataSave]" );
   #endif
  aRun = kFALSE;   
  aThread.Join();
  
  aManager = 0;
}


//===============================================================================
void ADataSave::OpenNewFile(){

   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::OpenNewFile] " );
   #endif

  char charNr[10];

  if(fout.is_open()) { 
      cout << "The file: " << aManager->GetDataFileName() << " was opened, close it..." << endl;
      fout.close();
      }

 cout << "aFileNr   = " << aFileNr << "\t aMaxFiles = " << aMaxFiles << endl;	   
  if(aFileNr < aMaxFiles || aMaxFiles == 0){

     sprintf(charNr, "_%03d.dat", aFileNr++);
     TString aFileName = aManager->GetDataFileName() + charNr;

     cout << "Try to open a new file: " << aFileName;
     fout.open(aFileName, ios::out);
     if(!fout.is_open()) { 
       cout << "could not open the file... "<< endl; 
       }
     else
       cout << " is open and ready to write data"<< endl; 
      
    }
  else{
    aRun = kFALSE;
    }

}

//===============================================================================

void ADataSave::InitDataSave(){

   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::InitDataSave]");
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

void ADataSave::DeleteDataSave(){

   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::DeleteDataSave] " );
   #endif

  if(fout.is_open()) { 
      fout.close();
      }



}

//===============================================================================
void ADataSave::SaveInBuffer(AEvent *aEvent){

  aRingBuffer.push_front(*aEvent);

}
//===============================================================================

void* ADataSave::ThreadFunc(void* aPtr){
   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::ThreadFunction] !!" );
   #endif

   ADataSave* p = (ADataSave*)aPtr;

   p->InitDataSave();

   UInt_t aFileSize = 0;

   AEvent aEvent;

   while( (p->aRun) && (&p->aThread) && p->aThread.GetState() == TThread::kRunningState ){

	   
     if(!p->aRingBuffer.pop_back_time(&aEvent)){
        //TThread::Printf(" ------------- thread2 ------- [ADataSave::ThreadFunction] read event from buffer and write it to file");  
        aEvent.Write2F(p->fout);
        aFileSize += 2*aEvent.GetEventSize();
           //cout << "aFileSize = " << aFileSize << "\t aMaxFileSize = " << p->aMaxFileSize << endl;	   
           //cout << "aFileNr   = " << p->aFileNr << "\t aMaxFiles = " << p->aMaxFiles << endl;	   
        if( (aFileSize > p->aMaxFileSize) ){
           p->OpenNewFile(); 
	   aFileSize = 0;
           }
        }

      TThread::Sleep(0, 10*1000*1000); // sleep 10ms   10 * 1000 * 1000 ns
     }      
     
 //  p->EmptyBuffer();
   p->DeleteDataSave(); 

return aPtr;      
}

//==========================================================================
Int_t ADataSave::Stop(Int_t timeout_ms){
   #ifdef DEBUG
     if(gDEBUG_SAVE > 0) TThread::Printf(" [ADataSave::StopDataSave] ");
   #endif

   if(aThread.GetState() == TThread::kRunningState)
     aRun = kFALSE;

   Int_t retval = 0;
   Int_t granularity = 25;
   Int_t i = 0;
   timeout_ms /= granularity;
                                
   while(aThread.GetState() == TThread::kRunningState && (i < granularity)){
     TThread::Sleep(0,granularity * 1000); 
     i++;
     }


    if ( i > granularity) { 
       TThread::Printf("Failed to stop the thread"); 
       aThread.Kill();
       retval = 1;
       }
    

return retval;
}
//==========================================================================
void ADataSave::EmptyBuffer(){

         TThread::Printf(" Empty buffer is not implemented yet, some data could not be stored....");	
}

//==========================================================================
/*
void ADataSave::SetHeader(const char* header){
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



