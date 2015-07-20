/*****************************************************************************
*
*****************************************************************************/

#include <iostream>
#include <string>
#include <exception>
#include "TSocket.h"
#include "TServerSocket.h"
#include "TMonitor.h"
#include "TMessage.h"
#include "TMutex.h"


#include "OnlineServer.h"
#include "MainFrame.h"
#include "AManager.h"
#include "ARingBuffer.h"

using namespace std;


ClassImp(OnlineServer)
extern Int_t gDEBUG_ONLINE;

//===============================================================================
OnlineServer::OnlineServer() {

   #ifdef DEBUG
      if(gDEBUG_ONLINE  > 0)   TThread::Printf( "\t\t\tDEBUG [OnlineServer::OnlineServer] Create DEFAULT OnlineServer");
   #endif
}

//===============================================================================
OnlineServer::OnlineServer(Int_t socket) : 
	aRun(kFALSE), 
	aRingBuffer(100), 
	aThread("aThread", 
	ThreadFunc, this ), 
	aManager(&AManager::GetInstance()), 
	aEmptyBuffer(kFALSE),
	aSocket(socket) 
{
   #ifdef DEBUG
      if(gDEBUG_ONLINE  > 0)   TThread::Printf( "\t\t\tDEBUG [OnlineServer::OnlineServer] Create OnlineServer");
   #endif

}

//===============================================================================

OnlineServer::~OnlineServer(){

   #ifdef DEBUG
      if(gDEBUG_ONLINE > 0)   TThread::Printf( "\t\t\tDEBUG [OnlineServer::~OnlineServer]" );
   #endif

}

   
//===============================================================================

void OnlineServer::InitOnlineServer(){

   #ifdef DEBUG
     if(gDEBUG_ONLINE > 0) TThread::Printf( "\t\t\tDEBUG [OnlineServer::InitOnlineServer]" );
   #endif

   fServ = new TServerSocket(9090, kTRUE);
   if (!fServ->IsValid())
      cout << "ERROR No socket connection" << endl;

   // Add server socket to monitor so we are notified when a client needs to be accepted
   fMon  = new TMonitor;
   fMon->Add(fServ);

   // Create a list to contain all client connections
   fSockets = new TList;

}
   
//===============================================================================

void OnlineServer::DeleteOnlineServer(){

   #ifdef DEBUG
     if(gDEBUG_ONLINE > 0) TThread::Printf( "\t\t\tDEBUG [OnlineServer::DeleteOnlineServer]");
   #endif
   fSockets->Delete();
   delete fMon;
   delete fSockets;
   delete fServ;

}

//===============================================================================
/*
void OnlineServer::Run(){
   #ifdef DEBUG
     if(gDEBUG_ONLINE > 0) TThread::Printf( "\t\t\tDEBUG [OnlineServer::Run]" );
   #endif

  aRun = kFALSE;   
  
  aThread = new TThread("OnlineThread", (void(*) (void *))ThreadFunc, (void*) this);
  aThread->Run();
}
*/


//===============================================================================

void OnlineServer::Send(AEvent *){

}

//===============================================================================

void* OnlineServer::ThreadFunc(void* aPtr){
   #ifdef DEBUG
     if(gDEBUG_ONLINE > 0) TThread::Printf( "\t\t\tDEBUG [OnlineServer::ThreadFunction] ");
   #endif

     
   OnlineServer* p = (OnlineServer*)aPtr;
  
   p->InitOnlineServer();

   #ifdef DEBUG
     if(gDEBUG_ONLINE > 2) TThread::Printf( "\t\t\tDEBUG [OnlineServer::ThreadFunction] before the while loop");
   #endif
   TSocket *s;


   while( (p->aRun) && (&p->aThread) && (p->aThread.GetState() == TThread::kRunningState) ){

       if((s = p->fMon->Select(20)) != (TSocket*)-1){
            p->HandleSocket(s);
       }
       else{
         gSystem->Sleep(100);
       } 
        #ifdef DEBUG
          if(gDEBUG_ONLINE > 4) TThread::Printf( "\t\t\tDEBUG [OnlineServer::ThreadFunction] inside the while loop");
        #endif
   
   }




   p->DeleteOnlineServer();

return aPtr;
}

//===============================================================================

void OnlineServer::HandleSocket(TSocket *s) {
   
   #ifdef DEBUG
     if(gDEBUG_ONLINE > 2) TThread::Printf( "\t\t\tDEBUG [OnlineServer::HandleSocket]");
   #endif
   
     
   if (s->IsA() == TServerSocket::Class()) {
      // accept new connection from spy
      TSocket *sock = ((TServerSocket*)s)->Accept();
      fMon->Add(sock);
      fSockets->Add(sock);
      printf("accepted connection from %s\n", sock->GetInetAddress().GetHostName());
   } else {
      // we only get string based requests from the spy
      cout << "HandleSocket jest else "<< endl;
      char request[64];
      if (s->Recv(request, sizeof(request)) <= 0) {
         fMon->Remove(s);
         fSockets->Remove(s);
         printf("closed connection from %s\n", s->GetInetAddress().GetHostName());
         delete s;
         return;
      }

      AEvent aEvent;
      AEvent *p = &aEvent;
      TString s1("set");
      // send requested object back
      TMessage answer(kMESS_OBJECT);
      if (!strcmp(request, "get")){
	 cout << "dostalem zadanie eventu "<< endl;     
	 aRingBuffer.pop_back(&aEvent);    
	 aEvent.EventShow(); 
	 cout << "wysylam obiekt "<< endl;     
         answer.WriteObject(p);
         }
      else
         Error("SpyServ::HandleSocket", "unexpected message");

      s->Send(answer);
   }
}
//====================================================================================

Int_t OnlineServer::Stop(Int_t timeout_ms){
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
void OnlineServer::EmptyBuffer(){

         TThread::Printf(" Empty buffer is not implemented yet, some data could not be stored....");
}

