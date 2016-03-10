/*****************************************************************************
*
*****************************************************************************/

#ifndef ADataSave2_h
#define ADataSave2_h


#include <iostream>
#include <fstream>
#include <bitset>
#include "ARingBuffer.h"


class AManager;
class AEvent;

using namespace std;

class ADataSave2 : public TObject{
private:

	AManager*	aManager;
	Char_t		*aBuffer;
	UInt_t		aBufferPos;
	UInt_t		aBufferCounter;
	Int_t		aBufferSize;
	Int_t		aEmptyBuffer;
	char		aHeader[4096]; 
	long		aEventCounter;
	UInt_t		aFileNr;
	UInt_t		aMaxFiles;
	UInt_t		aMaxFileSize;
	ofstream	fout;	

        ADataSave2( const ADataSave2& );	// Disable copy constructor
        ADataSave2 operator=( ADataSave2 );	// Disable =Operator
public:
        ADataSave2();				// default contructor needed by ROOT
        ADataSave2(bool);			// constructor
	virtual	~ADataSave2();			// destructor

	UInt_t 		 SaveInBuffer(AEvent &);
	void		 OpenNewFile();
	void		 InitDataSave();
	void		 DeleteDataSave();
	void		 EmptyBuffer();
	char*		 GetBuffer()		{ return aBuffer; }	


ClassDef(ADataSave2, 0)				// ADataSave2 class
};




#endif

