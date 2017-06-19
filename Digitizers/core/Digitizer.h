
/*****************************************************************************
*
* Abstract class for digitizer --+
*
*****************************************************************************/

#ifndef Digitizer_h
#define Digitizer_h


#include <iostream>
#include <string>
#include <exception>
#include <TObject.h>
#include <TGFrame.h>
#include "AEvent.h"

using namespace std;

class ATrack;

class Digitizer: public TObject{

protected: 
  static int mDigitizers;

public:

  virtual void		Refresh(); 
  virtual void		DoEnable(bool )	; 
  virtual Bool_t	IsEnabled() const	;
  
  virtual void		BuildGui(TGCompositeFrame *){;}
  virtual void		PrintInfo();
  virtual void		Reset();
  virtual Int_t		Initialize();
  virtual Int_t		Configure();
  virtual void		StartAcq();
  virtual void		StopAcq();
  virtual void		Close();
  virtual const char*	GetName()     const 	{return "Digitizer";}
 
  virtual void          PopEvent()              {;}
  virtual AEvent*	GetEvent()		{return NULL;} 


  virtual      ~Digitizer()      {}  
//----------------------------------------------------------------------
  class BadDigitizerCreation : public exception {
        string reason;
  public:
        BadDigitizerCreation(string type){
        reason = "Cannot create type " + type;
        }
        ~BadDigitizerCreation() throw() {}
        const char *what() const throw() {
        return reason.c_str();
        }
  };

//----------------------------------------------------------------------
  static Digitizer* Factory(const string&) throw(BadDigitizerCreation);

//----------------------------------------------------------------------

  virtual Int_t         GetTrackLength()                        const   { return 0; }
  virtual Int_t         GetRange(Int_t)                	        const   { return 0; }

  virtual Int_t         GetOffset(Int_t )			const   { return 0;} 
  virtual Int_t         GetThresholdMode(Int_t )		const   { return 0;} 
  virtual Int_t         GetThreshold(Int_t, Bool_t b=kTRUE )    const   { return b;} 
  virtual Int_t		GetActiveChannel(Int_t )		const   { return 0;}
  virtual Int_t         GetActiveGroup(Int_t  )                 const   { return 0;}
  virtual Int_t		GetNrOfChannels()			const	{ return 0;}
  virtual Int_t		GetNrOfGroups()				const	{ return 0;}
  virtual Int_t		ReadConfigFile(ifstream &)			{return 0;}
  virtual Int_t		WriteConfigFile(ofstream &)			{return 0;}

  virtual Int_t		AcqReady() 					{return -1;}
  virtual UInt_t	GetData()					{return 0;}

  virtual Int_t		GetSegmentSize() 			const {return 0;}
  virtual void		SendSWTrigger(){}
  virtual UInt_t        GetDataSize()   {return 0;}
 
  string 		Trim(string const &, char const* delims = " \t\r\n");

ClassDef(Digitizer, 0)
};


#endif

