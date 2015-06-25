 /*\brief
 *********************************************************************************************
 * This is the most important class, contains all necessary information and variable used    *
 * in the program flow.                                                                      *
 *                                                                                           *
 * This class is a signleton design patern,                                                  *
 * This class manage the in/out file                                                         *
 *                                                                                           *
 * All other classes in this application have a pointer to this class. Because it is         * 
 * a singleton its guarante there is only one instance of this class                         *
 ********************************************************************************************/


#ifndef AManager_h
#define AManager_h

#include <vector>
#include <time.h>

#include <TQObject.h>
#include <TCanvas.h>

#include "Digitizer.h"
#include "MainFrame.h"
#include "ControlAcq.h"
#include "ControlFrame.h"
#include "ADataSave.h"
#include "OnlineServer.h"

using namespace std;

class MainFrame;
class ARingBuffer;

class AManager : public TQObject{

	AManager();
	~AManager(){}
	AManager( const AManager& );	//!< disable copy contructor
	AManager operator=( AManager );	//!< disable =operator

	vector<string>      	mFindDev;//!< help vector used for finding devices, then the pointer to all devices will be kept in the mAcqDev vector	

	TString mDataFileName;     	//!< Name of the current data file (only corename without numbering, without path)
	TString mInfoFileName;     	//!< Name of the current info file
	TString mConfigureFileName;     //!< Name of the current configuration file
	TString mLogFileName;     	//!< Name of the current log file
	TString mWorkingDir;		//!< Working directory with configuration data (.acqrc)

	MainFrame	*mWindow;	//!< a pointer to the main window 
        ControlAcq	*mAcq;		//!< a pointer to the opject which control all the acquisition
	ControlFrame	*mControlFrame;//!< a pointer to the control window
	ADataSave	*mDataSave;	//!< a pointer to the storage object 
	OnlineServer    *mOnlineServer;	//!< a poitner to the online server, is not impemented yet

	/* \brief the buffers are arganised in such that acquisition store data in separate buffers, one for online display, another for data save
	 * 	and another for online server
	 */


	Int_t mSegments;
	Int_t mAcqLoops;
	Int_t mFileSize;
	Int_t mFileTime;
	Int_t mFileNr;
	Int_t mMaxFiles;
	Int_t mTimeout;
	Int_t mMaxRange;
	Int_t mMaxNrSamples;
	Int_t mEventNr;
	Int_t mMaxEvents;
	Int_t mDataSize;

	Bool_t mDisplayEvents;
	time_t aAcqTime0;

	void FindDevices();


public:
	vector<Digitizer*>	mAcqDev; //!< those devices will be used in acquisition
	static AManager& GetInstance(){
                static AManager mManager; 
                return mManager;
                }
	void		ClearAll();

	void		CreateGui();

        void		ConfigureFrame(Bool_t create = kTRUE);
	void		LoadConfiguration();
	void		SaveConfiguration();
	void		StartAcquisition(MainFrame::mSTATE);
	void		StopAcquisition(MainFrame::mSTATE);
	void		SetDisplay(Bool_t aDisplayEvents)  { mDisplayEvents = aDisplayEvents;};
	Bool_t		GetDisplay()			{return mDisplayEvents;}
        MainFrame	*GetFrame()	{return mWindow;}	

	void		SetLoops(int aAcqLoops)		{ mAcqLoops  = aAcqLoops; }
	void		SetFileSize(int afilesize)	{ mFileSize  = afilesize; }
	void		SetFileTime(int afiletime)	{ mFileTime  = afiletime; }
        void		SetFileNr(int afileNr)		{ mFileNr    = afileNr;   }
        void		SetMaxFiles(int afileNr)	{ mMaxFiles  = afileNr;   }
	void		SetTimeout(int atimeout) 	{ mTimeout   = atimeout;  }        
	void		SetSegments(int aseg)		{ mSegments  = aseg;      }
	void		SetMaxRange(int a)		{ mMaxRange  = a;         }
	void		SetMaxSamples(int a)		{ mMaxNrSamples  = a;	  }
	void		SetEventNr(int a)		{ mEventNr   = a;	  }
	void		SetMaxEvents(int a)		{ mMaxEvents   = a;	  }
	void		SetDataSize(int a)		{ mDataSize   = a;	  }

	void		SetLabelEnable(bool a)		{ mWindow->SetLabelAcq(a); }
	void		SetStartAcqTime()		{ time(&aAcqTime0); 	}
	time_t		GetStartAcqTime()		{ return aAcqTime0;	}

	//const Digitizer& operator[] (size_t i) const {return *mAcqDev[i];}	
	const vector<Digitizer*> GetDigitizers() const {return mAcqDev;}


        void 		SetMode(MainFrame::mSTATE aState) 	{ mWindow->SetAcquisitionState(aState);}
	MainFrame::mSTATE GetMode()	                { return mWindow->GetAcquisitionState()      ;}

	Int_t		GetLoops()			{ return mAcqLoops;	}
	Int_t		GetFileSize()			{ return mFileSize;	}
	Int_t		GetFileTime()			{ return mFileTime;	}
	Int_t		GetFileNr()			{ return mFileNr;	}
	Int_t		GetMaxFiles()			{ return mMaxFiles;	}
        Int_t		GetTimeout()			{ return mTimeout;	}
	Int_t		GetSegments()			{ return mSegments;	}
	Int_t		GetMaxRange()			{ return mMaxRange;	}
	Int_t		GetMaxSamples()			{ return mMaxNrSamples;	}
	Int_t		GetEventNr()			{ return mEventNr;	}
	Int_t		GetMaxEvents()			{ return mMaxEvents;	}
	Int_t		GetDataSize()			{ return mDataSize;	}

	void		SetDataFileName( TString a )	{ mDataFileName = a; }

	TString 	GetWorkingDir()			{ return mWorkingDir;	}
	TString 	GetInfoFileName()		{ return mInfoFileName;	} 
	TString 	GetDataFileName()		{ return mDataFileName;	} 
	TString 	GetLogFileName()		{ return mLogFileName;	} 

	TString		GetConfigureFileName() 		{ return mConfigureFileName; } 
	void		SetConfigureFileName(TString aFile) { mConfigureFileName = aFile; } 

	Digitizer* GetDigitizer(Int_t a = 0){ 
		if(!mAcqDev.size()) { return NULL;}
		else { return mAcqDev.at(a);}
                }
        
        Int_t 		GetNrDigitizers(){ return mAcqDev.size(); }
	

	Int_t 		ReadConfigFile(const char *);
	Int_t 		WriteConfigFile(const char *file = NULL);
	string 		Trim(string const&, char const* delims  = " \t\r\n");


ClassDef(AManager, 0)

};

#endif

