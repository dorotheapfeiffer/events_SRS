#ifndef USERCLASS_h
#define USERCLASS_h

#include <string>
#include <TCanvas.h>

class AEvent;
class ATrack;
class TObjArray;

using namespace std;

class UserClass : public TObject {

private:
	TObjArray       *mSignalList;
public:
	UserClass();
	virtual 	~UserClass();
	virtual void 	DoAnalysis(AEvent &);
	virtual void 	SignalProcessing(AEvent &);
	virtual Int_t	CasetteNr(AEvent &);

	TCanvas		*c;
	TH1F*		MakeTH1F(ATrack &);
	//Double_t 	BaseLineCorrection(TH1F*, int n);
ClassDef(UserClass, 0)
};
#endif
