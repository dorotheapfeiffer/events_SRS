/*****************************************************************************
*
*****************************************************************************/

#ifndef OnlineServer_h
#define OnlineServer_h


#include <iostream>
#include <string>
#include <exception>
#include "ARingBuffer.h"

class AManager;
class TMonitor;
class TServerSocket;
class TThread;
class TList;
class TSocket;

using namespace std;

class OnlineServer: public TObject{


	Bool_t          aRun;                   // variable saing if the thread is running or not
        ARingBuffer     aRingBuffer;            // the buffer to store the data
        TThread         aThread;                // separate thread

        static void*    ThreadFunc(void*);      // fuction running in separate thread doing all the job

        AManager*       aManager;

        bool		aEmptyBuffer;
        long            aEventCounter;
	Int_t		aSocket;

  	TServerSocket 	*fServ;			// server socket
  	TMonitor      	*fMon;			// socket monitor
  	TList         	*fSockets;		// list of open spy sockets

	OnlineServer( const OnlineServer& );	// disable copy contructor
	OnlineServer operator=( OnlineServer );	// disaple = operator

public:

	OnlineServer();				// default contructor needed by ROOT
	OnlineServer(Int_t);			// constructor
	virtual ~OnlineServer();		// destructor

        void             Send(AEvent *);
        void             EmptyBuffer();
        ARingBuffer     *GetBuffer()            { return &aRingBuffer; }

	void		InitOnlineServer();
	void		DeleteOnlineServer();
	void		HandleSocket(TSocket *s);
	//void		Run();
	Int_t		Stop(Int_t);

ClassDef(OnlineServer, 0)
};

#endif

