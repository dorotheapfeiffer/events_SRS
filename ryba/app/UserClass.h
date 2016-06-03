#ifndef USERCLASS_h
#define USERCLASS_h

#include <string>
#include <TCanvas.h>

class AEvent;
class TGraph;
class ATrack;
class ASignal;
class TObjArray;
class TH2F;


using namespace std;

class UserClass : public TObject {

private:
	TObjArray       *mSignalList;
	TString		mName;			// string to keep the output result file name (only core) will be added _CD_SUM, _CD_MAX or _IND 
	TH1F		*m_GridAmplitude;	// histograms for keeping grid amlitude
	TH1F		*m_GridPosition;	// histograms for wires position
	TH1F		*m_WireAmplitude;	// histograms for keeing position amplitude
	TH1F		*m_WirePosition;	// histograms for strips position
	TH2F		*m_2DView;		// histograms 2D for imagine
	TGraph		*m_TTS;			// graph for trigger time stamp

	Int_t		mMode;

public:
	UserClass(){}
	UserClass(TString, int );
	virtual 	~UserClass();

	virtual void 	DoAnalysis(AEvent &, int);
	virtual void 	SignalProcessing(AEvent &);

		void 	DrawCanvas(Int_t);
		Int_t	adc2pos(Int_t);

	TCanvas		*c;
	TCanvas		*c_2DV;

ClassDef(UserClass, 0)
};
#endif
