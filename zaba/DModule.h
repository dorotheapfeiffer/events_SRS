#ifndef DMODULE_H 
#define DMODULE_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <stdlib.h>
#include <string.h>

#include <TROOT.h>
#include <TObject.h>
#include <TSocket.h>
#include <TMessage.h>
#include <TFormula.h>
#include "CAENVMElib.h"


//extern "C" void swab(const void *, void *, size_t);
extern "C" void setbit(UShort_t *, Int_t, Int_t);

//*****************************************************************************
class DGDisplay;
class DDataSave;
class DMultiGrid;

class DModule : public TObject {
  
private:
  Char_t 	fGenTypeDescription[80];            // generic type description
  Char_t 	fActTypeDescription[80];            // actual type description
  Char_t 	fModuleDescription[80];             // label
  UInt_t 	fAdBase;                            // hardware base address
  
protected:
  UShort_t 	fDummy;                           // location for dummy operations
  CVBoardTypes  m_VMEBridge;
  Int_t         m_Handle;

public:
  DModule()                       		{std::cout << "DModule ctor called" << std::endl;}
  DModule(Char_t *gtdesc, Char_t *atdesc, Char_t *mdesc, UInt_t addr);
  virtual ~DModule();
  void    PrintAddress(); 
  UInt_t  GetAdBase()             		{return fAdBase;}
  Char_t* GetGenTypeDescription() 		{return fGenTypeDescription;}
  Char_t* GetActTypeDescription() 		{return fActTypeDescription;}
  Char_t* GetModuleDescription()  		{return fModuleDescription;}
  Int_t   Access(Char_t *flag, Int_t adoff, Int_t len, void *buf);
  Int_t   BitAccess(Int_t adoff, Int_t bioff);
  void    BitAccess(Int_t adoff, Int_t bioff, Int_t val);
  std::string  Trim(std::string const& source, char const* delims = " \t\r\n");
  virtual void ReadVME()          		{ std::cout << this << " DModule ReadVME called\n";         }
  virtual void ResetModule()      		{ std::cout << this << " DModule ResetModule called\n";     }
  virtual void InitModule()       		{ std::cout << this << " DModule InitModule called\n";      }
  virtual void ShowData(DGDisplay *, DMultiGrid *){ std::cout << this << " DModule ShowData called\n";        }
  virtual void DataSave(DMultiGrid *)		{ std::cout << this << " DModule DataSave called\n";        }
  virtual void ConfigureModule()  		{ std::cout << this << " DModule ConfigureModule called\n"; }
  virtual void StartAcq()	  		{ std::cout << this << " DModule StartAcq called\n"; }
  virtual void StopAcq()	  		{ std::cout << this << " DModule StopAcq called\n"; }
  virtual void Log(std::ofstream &)  		{ std::cout << this << " DModule Log called\n"; }
  virtual void SaveConfig(std::ofstream &) 	{ std::cout << this << " DModule SaveConfig called\n"; }
  virtual void LoadConfig(std::ifstream &) 	{ std::cout << this << " DModule LoadConfig called\n"; }

  ClassDef(DModule,1)               // Parent to all VME module classes

};
//*****************************************************************************
#endif
