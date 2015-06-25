/*****************************************************************************
*
*****************************************************************************/

#ifndef ADataSave_h
#define ADataSave_h


#include <iostream>
#include <fstream>
#include "ARingBuffer.h"


class TThread;
class AManager;
class AEvent;

using namespace std;

class ADataSave : public TObject{
private:

	Bool_t		aRun;			// variable saing if the thread is running or not
	ARingBuffer	aRingBuffer;		// the buffer to store the data
	TThread		aThread;		// separate thread

	static void*	ThreadFunc(void*);	// fuction running in separate thread doing the all the job

	AManager*	aManager;

	Int_t		aEmptyBuffer;
	char		aHeader[4096]; 
	long		aEventCounter;
	Int_t		aFileNr;
	UInt_t		aMaxFiles;
	UInt_t		aMaxFileSize;
	ofstream	fout;	

        ADataSave( const ADataSave& );		// Disable copy constructor
        ADataSave operator=( ADataSave );	// Disable =Operator
public:
        ADataSave();				// default contructor
        ADataSave(bool);
	virtual	~ADataSave();

	void 		 SaveInBuffer(AEvent *);
	void		 OpenNewFile();
	void		 InitDataSave();
	void		 DeleteDataSave();
	void		 EmptyBuffer();
	ARingBuffer	*GetBuffer()		{ return &aRingBuffer; }	
	//void		 Run();
	Int_t		 Stop(Int_t);


ClassDef(ADataSave, 0)				// ADataSave class
};




#endif

