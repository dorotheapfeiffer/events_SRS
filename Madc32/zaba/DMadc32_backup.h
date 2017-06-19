#ifndef MADC32_H
#define MADC32_H

#include "DModule.h"
#include "CAENVMElib.h"

//*****************************************************************************
class DGDisplay;

class DMadc32 : public DModule {

private:

 CVBoardTypes		m_VMEBridge;
 Short_t         	m_Link;   
 Short_t         	m_Device;
 Int_t			m_Handle;

 Int_t			m_BaseAddress;
 Int_t			m_ModuleID;

 UInt_t			m_Buffer[1024*1024*810]; //810MB max buffer
 UInt_t			m_BufferPos;
 Int_t			m_BufferSize;

 UInt_t			m_dataSize;
 UInt_t			m_Events;


public:
  DMadc32()     {printf("Default Madc32 constructor called\n");}
  DMadc32(Char_t *mdesc, UInt_t addr);
  virtual 	~DMadc32();

  void		InitModule();
  void		ConfigureModule();
  void		ReadVME();
  void		ShowData(DGDisplay* );
  void		DataSave(DMultiGrid *);
  void		ResetModule();
  UInt_t	GetNrEvents() { return m_Events;}
  UInt_t	GetDataSize() { return m_Events;}

  ClassDef(DMadc32,1)       // Mesytec Pulse Sensitive Digitizer MADC-32
};
//*****************************************************************************

#endif
