#ifndef DMADC32_H 
#define DMADC32_H

#include <ostream>
#include <string>

#include <TObject.h>
#include <TObjArray.h>

#include "DV1718.h"
#include "DMadc32.h"
#include "DCAEN1740.h"
#include "DCAEN1740D.h"
#include "gnuplot-iostream.h"



//*****************************************************************************
class DGDisplay;

class DAcquisition : public TObject {

private:
  TObjArray   		*fModuleList;           // list of pointers to VME modules


public:
  Int_t			m_Timerout;		// sleep in the main loop
  Int_t			m_Status;		// reprezent status of the acquisition: 0 - stop, 1 - run (without saving), 2 - acq
  Int_t			m_Display;		// variable to swich on/off online Display
  Int_t			m_HistogramSave;	// variable to either save data or not
  Int_t			m_DataSave;		// variable to either save data or not
  Int_t			m_AcqStatusEntry1;	// keep total acq time
  Int_t			m_AcqStatusEntry2;	// keep nr of events
  Int_t			m_AcqStatusEntry3;	// keep nr of events / sec
  Int_t			m_AcqStatusEntry4;	// keep data transfer in MB
  Int_t			m_StopAfterSecEntry;    // keep nr of seconds when acq must stop
  Int_t			m_StopAfterFileEntry;   // keep nr of files when acq must stop
  Int_t			m_StopAfterEventsEntry; // keep nr of events when acq must stop
  Int_t			m_SaveFileSizeCB;
  Int_t			m_SaveFileSizeEntry;	// keep size of the stored file
  Int_t			m_SaveFileTimeCB;	
  Int_t			m_SaveFileTimeEntry;	// keep gui value of how many second the file should be stored
  Int_t			m_nrFile;		// keep how many files are stored in acq mode
  Int_t		 	m_PrevEvent;
  Int_t		 	m_PrevSize;
  Int_t		 	m_EmptyBuffer;
  Int_t		 	m_StopCondition;
 

  time_t		m_StartAcqTime; 
  time_t		m_TimeNow;

  ULong_t		m_ElapsedAcqTime;
  ULong_t		m_ElapsedTimeMS;
  ULong_t		m_PrevTimeMS;
  ULong_t		m_CurrentTimeMS;
  Int_t			m_NrOfSavedFiles;


  DV1718		*fDV1718;		// Bridge VME_USB
  DMadc32		*fDMadc32;		// Mesytec pulse sensitive ADC
  DCAEN1740		*fDCAEN1740;
  DCAEN1740D		*fDCAEN1740D;

  Gnuplot		m_Gnuplot;

  std::string		m_AcqName;
  std::string		m_FileName;
  std::string		m_FileTime;
  std::string		m_Path;
  std::string		m_ConfigPath; 
  std::string		m_DataPath; 
  std::ofstream         *fLog;                  // log-file stream pointer

public:
  DAcquisition(){} //default constructor
  DAcquisition(std::string s);
  virtual 	~DAcquisition();

  void 		Exit(){std::cout << "exit does not work properlly....\n"; this->~DAcquisition();}
  void 		ResetModules();                   // Call ResetModule() for each VME module
  void 		InitModules();                    // Load the fixed part of the settings
  void 		LoadConfig(char *filename);       // Load the variable part of settings from file
  void 		SaveConfig(char *filename);       // Save the variable part of settings to file
  void 		ResetHistograms(){}

  void 		StartAcq();
  void 		StopAcq();
  void 		EmptyBuffer();

  void 		BuildEvent();

  void 		ReadVME();                           // read VME data from module to the class objects
  void 		ShowData(DGDisplay *display = NULL); // do online analysis and fill histograms 
  void 		DataSave();                          // save data to file
  void 		SetFileName(std::string filename) { m_FileName = filename; }
  void 		SetFileTime(std::string filename) { m_FileTime = filename; }
  void		SetDataPath(std::string datapath) { m_DataPath = datapath; }
  bool 		CheckCondition();
  void 		GnuplotOnline();

  ULong_t	GetTimeMS();
  std::string 	GetFileName(){ return m_FileName; }
  std::string 	GetFileTime(){ return m_FileTime; }
  std::string 	Trim(std::string const& source, char const* delims = " \t\r\n" ); 

  ClassDef(DAcquisition,1)  		// MultiGrid readout 
};
//*****************************************************************************

#endif

