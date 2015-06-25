/*****************************************************************************
*
*****************************************************************************/

#ifndef OnlineServer_h
#define OnlineServer_h


#include <iostream>
#include <string>
#include <exception>


class AManager;
class TMonitor;
class TServerSocket;
class TThread;
class ARingBuffer;
class TList;
class TSocket;

using namespace std;

class OnlineServer: public TObject{

private:
  AManager*		aManager;
  
  ARingBuffer*		aRingBuffer;		// buffer with data
  TServerSocket 	*fServ;			// server socket
  TMonitor      	*fMon;			// socket monitor
  TList         	*fSockets;		// list of open spy sockets

  TThread*		aThread;		// separate thread
  static void*		ThreadFunc(void*);	// fuction running in separate thread doing the all the job
  Bool_t		aRun;			// variable saing if the thread is running or not

  OnlineServer( const OnlineServer& );		// disable copy contructor
  OnlineServer operator=( OnlineServer );	// disaple = operator

public:
   OnlineServer();
   virtual ~OnlineServer();
  
   void			InitOnlineServer();
   void			DeleteOnlineServer();
   void			HandleSocket(TSocket *s);


   void			Run();
   Int_t		Stop(Int_t);

ClassDef(OnlineServer, 0)
};

#endif

