#ifndef DV1718_H
#define DV1718_H

#include "DModule.h"
#include "CAENVMElib.h"

//*****************************************************************************
class DGDisplay;
class DAcquisition;

class DV1718 : public DModule {

//  CVBoardTypes  m_VMEBridge;
//  Int_t		m_Handle;

public:
  UInt_t	m_Input;
  UInt_t	m_EndCounts;
  UInt_t	m_CountOutput;
  UInt_t	m_Value;



public:
  DV1718()     { printf("Default V1718 constructor called\n"); }
  DV1718(Char_t *mdesc, UInt_t addr);
  virtual 	~DV1718();

  void		InitModule();
  void		ConfigureModule();
  void		ReadVME();
  void		GetEvent();
  void		StartAcq();
  void		StopAcq();
  void		Log(std::ofstream &);
  void		SaveConfig(std::ofstream &){}
  void		LoadConfig(std::ifstream &){}
  void		ShowData(DGDisplay*, DAcquisition* );
  void		DataSave(DAcquisition *);
  void		ResetModule();
  UInt_t	GetNrCounts() { return m_Value;}
  void		ShowSettings();

  ClassDef(DV1718,1)       // CANE Bridge VME-USB
};
//*****************************************************************************

#endif
