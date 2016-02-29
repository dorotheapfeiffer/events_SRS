#ifndef USERCLASS_h
#define USERCLASS_h

#include <string>
#include <TCanvas.h>

class AEvent;
class ATrack;
class ASignal;
class TObjArray;

using namespace std;

class UserClass : public TObject {

private:
	TObjArray       *mSignalList;
	TString		mName;		// string to keep the output result file name (only core) will be added _CD_SUM, _CD_MAX or _IND 
	TH1F		*pHW[8];	// histograms for wires pulse high
	TH1F		*pHS[8];	// histograms for strips pulse high
	TH1F		*posW[8];	// histograms for wires position
	TH1F		*posS[8];	// histograms for strips position
	TH1F		*posAll;	// histogram for wires position all together
	TH1F		*posIND;	// histogram for wires position in indyvidual readout
	TH1F		*phIND[32];

	ASignal		*aSignal[32];	// convert AEvent to TH1F in order to aplly different algorythms
	Int_t		mChannel[32];	// table to keep channel number of each signal

	Double_t	mMaximumSUM[33];// table to keep maximum of chdiv readout and SUM algorythm
	Double_t	mMaximumMAX[33];// table to keep maximum of chdiv readout and MAX algorythm
	Double_t	mMaximumIND[33];// table to keep maximum of indyvidual readout
					// these tables have 33 elements, the last element is for timestamp        
					

public:
	UserClass(TString );
	virtual 	~UserClass();
	virtual void 	DoAnalysis(AEvent &);
	virtual void 	SignalProcessing(AEvent &);

	void IndyvidualReadout(AEvent &aEvent);
	void ChargeDiviReadout(AEvent &aEvent);

	TCanvas		*c;

ClassDef(UserClass, 0)
};
#endif
