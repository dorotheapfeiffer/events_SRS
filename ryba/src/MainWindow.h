/*******************************************************************************************
*
* Declaration of the class MainWindow, it contains the main window and all widgets
* 
*
*
********************************************************************************************/

#ifndef MAINWINDOW_h
#define MAINWINDOW_h

#include <TGFrame.h>
#include <TString.h>
#include <vector>
#include <TObjArray.h>
#include <TGStatusBar.h>
#include <TGButtonGroup.h>

class TRootEmbeddedCanvas;
class TGButton;
class TGPopupMenu;
class TGMenuBar;
class TGComboBox;
class TGGroupFrame;
class TH1F;
class UserClass;
class TGStatusBar;
class TMultiGraph;
class TGHProgressBar;
class TGNumberEntry;
class TGTextEntry;
class TGListBox;
class TGButtonGroup;
class TGRadioButton;
class TGTransientFrame;
class TString;
class Files;
class ReadData; 



class MainWindow : public TGMainFrame {

private:
	enum CommandId{
		M_FILE_OPEN, 
		M_FILE_CONNECT, 
		M_FILE_DISCONNECT, 
		M_FILE_EXIT, 
		M_VIEW_OPTION,
		B_START,
		B_STOP,
		B_RESET,
		eDISPLAYMODE,
		eEVENTNUMBER,
		eRBCASETTE,
		eRBGROUP,
		eRBTOGETHER,
		eSEPARATE,
		eCASSETENR
		};
	
	TGMenuBar		*fMenuBar;
	TGPopupMenu		*fMenuFile;
	TGPopupMenu		*fMenuView;
	TGPopupMenu		*fMenuHelp;

	TGStatusBar		*fStatusBar;
	TGHorizontalFrame	*fMainF;
	TGCompositeFrame	*fCommonFrame;
	TGCompositeFrame	*fOptionMenu;
	TRootEmbeddedCanvas	*fEcan;
	TGPopupMenu		*fMenuEdit;
	TGHorizontalFrame	*fButtonsFrame;

        TGTextButton		*fButtonStop;
        TGTextButton		*fButtonStart;
        TGTextButton		*fButtonReset;
        TGTextButton		*fButtonAlgorythm;

	TGHProgressBar		*progressBar;
	TGNumberEntry		*fEventNumber;
	TGTransientFrame	*fCustWindow;
	TGButtonGroup**         gRBG1;//button groups holding check button in the custom window manager 
	TGButtonGroup*          fBGDisplay;
	TGGroupFrame		*fDisplayGroup;
	TGGroupFrame		*fHistogramGroup;
	TGComboBox		*fCBDisplayMode;
	TGComboBox		*fCBCasetteNr;

	TGVButtonGroup      	*fButtonGroup;
	TGVButtonGroup      	*fAlgorythmGroup;
	TGRadioButton 		*fRadiob[4];


	UserClass		*mUserClass;
	TMultiGraph             *mMultiGraph = 0; // new standard c++11
	TObjArray 		*mGraphList;
	Int_t			mAlgorythm;	
	Int_t			mMode;
	ReadData 		*aReadData = NULL;

public:
	MainWindow() {};
	MainWindow(const TGWindow *p, UInt_t w = 800, UInt_t h = 600);
	virtual ~MainWindow();
//	void CloseWindow();
	void HandleMenu(Int_t id);

	void DoStop(); 
	void DoStart();
	void DoReset();
	void DoAlgorythm(); 
        void DoRadioButton();
	void NumberEntrySet(Long_t);

	void ReadDataFrom();
	void ReadConfigFile();
	void WriteConfigFile();

	
	TRootEmbeddedCanvas* GetEmbCanvas(){return fEcan;} 

	void AddStatusBar();
	void AddCommonFrame();
	void AddMenuBar();
	void AddButtonsPanel();

	void DisplayEvent(Int_t);
	void Reading(int status); //update status bar during reading
	void MousePosition(Int_t, Int_t, Int_t, TObject *);
	void DrawEventStatus(Int_t , Int_t , Int_t , TObject * );
	void DivideCanvas();
	void DivideCanvasCustom();
	void SetFirstEvent(Long_t);
	void SetLastEvent(Long_t);
	void SetTimeRefresh(Long_t);
	void SetEventRefresh(Long_t);
	void HandelBGDisplay(Int_t);
	void CloseCustom();
	void SetOutText(char*);

	UserClass* GetUserClass()		{ return mUserClass; }

	TString GetCurrentFile() 		{ return iCurrentFile; }
	void	SetCurrentFile( TString s )	{ iCurrentFile = s; }
	Bool_t	GetLoopCondition()		{ return mLoopCondition;}
	void 	SetLoopCondition(Bool_t a)	{ mLoopCondition = a;}	
	void	ResetDisplay();

	void UpdateProgressBar(char* operation);
	void UpdateEventNumber(Int_t a);
	void AnalyseData();
	
//	void UpdateStatusBar(char* , Int_t);

	
	Int_t			iCurrentEvent;
	Bool_t			mLoopCondition;
	Bool_t			mResetDisplay;
	TString			iCurrentFile;
	Int_t			iNrEvents;
	Int_t			iNrTracks;
	Int_t			iNrOfFiles;
	Int_t			iNrFile;
	Bool_t			bLoopCon;
	Bool_t			bToBeDisplayed[16];                      //is the track n going to be drawn (kTRUE) or not (kFALSE)
	Int_t			iNbToDisplay;
	Int_t			iFirstEvent;                              //first event to be process by UserClass
	Int_t			iLastEvent;                               //last event to be process by UserClass
	Bool_t			bCustomDisplay;                          //determine if the custom display window is open
	Int_t			iNbTrackByPad[16];
	Int_t			iTracktoBeShown[16][16];
	Int_t			iBGDisplay;
	Int_t			iEventRefresh;
	Int_t			iTimeRefresh;
ClassDef(MainWindow, 0)
};
#endif
