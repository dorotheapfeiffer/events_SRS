#ifndef MADC32_H
#define MADC32_H
#include <ostream>

#include "DModule.h"
#include "CAENVMElib.h"

#define MAX_BLT_SIZE (256*1024)

//*****************************************************************************
class DGDisplay;

class DMadc32 : public DModule {

private:

 Int_t			m_BaseAddress;
 Int_t			m_ModuleID;

 UInt_t			m_Buffer[MAX_BLT_SIZE]; 
 UInt_t			m_localBuffer[MAX_BLT_SIZE]; 
 UInt_t			m_BufferPos;
 UInt_t			m_BufferSize;

 Int_t			m_dataSizeByte;
 UInt_t			m_Events;
 Int_t			m_IRQ;

public:
 Int_t			m_IgnoreThreshold;    // keep check button state of ignore threshold
 Int_t			m_ThresholdOn[32];    // keep check button state
 Int_t			m_ThresholdValue[32]; // keep value of threshold
 Int_t			m_ThresholdCache[32]; // needed to keep threshold value when m_IgnoreThreshold is set
 Int_t			m_GateGenerator;
 Int_t			m_GateDelay;
 Int_t			m_GateWidth;
 Int_t			m_GateOutput;
 Int_t			m_ADCResolution;

public:
  DMadc32()     {printf("Default Madc32 constructor called\n");}
  DMadc32(Char_t *mdesc, UInt_t addr);
  virtual 	~DMadc32();

  void		InitModule();
  void		SaveConfig(std::ofstream &);
  void		LoadConfig(std::ifstream &);
  void		StartAcq();
  void		StopAcq();
  void          Log(std::ofstream &);
  void		ConfigureModule();
  void		ReadVME();
  void		ShowData(DGDisplay*, DMultiGrid *);
  void		DataSave(DMultiGrid *);
  void		ResetModule();
  UInt_t	GetNrEvents() { return m_Events;}
  UInt_t	GetDataSize() { return m_dataSizeByte;}
  void		ShowSettings();

  ClassDef(DMadc32,1)       // Mesytec Pulse Sensitive Digitizer MADC-32
};
//*****************************************************************************

#endif
