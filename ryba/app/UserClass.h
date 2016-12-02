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
	TObjArray       m_SignalList;
	TString		m_Name;			// string to keep the output result file name (only core) will be added _CD_SUM, _CD_MAX or _IND 
	TH1F		*m_ADC[32];		// histograms for keeping grid amlitude
	TH1F		*m_tts;		// histograms for keeping grid amlitude

	Int_t		m_Mode;
	Int_t		m_NrADC;

public:
	UserClass(){}
	UserClass(TString, int );
	virtual 	~UserClass();

	virtual void 	DoAnalysis(AEvent &, int);
	virtual void 	SignalProcessing(AEvent &);

		void 	DrawCanvas(Int_t);
		void 	UpdateCanvas();
		Int_t	adc2pos(Int_t);

	TCanvas		c1;
	TCanvas		c2;

ClassDef(UserClass, 0)
};
#endif
