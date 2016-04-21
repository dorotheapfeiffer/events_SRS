#ifndef UserBase_h
#define UserBase_h

#include <TString.h>

class TObject;
class TTimer;
class CTrack;


class UserBase : public TObject {

	TString iCurrentFile;

public:
	UserBase();
	Bool_t retval;
	Int_t iTimeRefresh;
	Int_t iEventRefresh;
        Bool_t ReturnStatus();
	void TimeRefresh(Int_t ms);
	void EventRefresh(Int_t event);
        void Pause();
	Bool_t DoBaseAnalysis(CTrack**, Int_t);
	Int_t GetEventNb(){return iNbEvent;}
	void SetOutFileName(TString* out){*OutFileName=out->Data();}
	const char* GetOutFileName(){return OutFileName->Data();}
	void SetCurrentFile(TString s){iCurrentFile = s;}
	//	void SetCurrentFile();//{iCurrentFile = s.Copy();}
        TString GetCurrentFile() const { return iCurrentFile; }

	virtual void DoAnalysis(CTrack**, Int_t)=0;

	virtual void Refresh();
	virtual ~UserBase();
 private:
	TString* OutFileName;
	Int_t iNbEvent;
	TTimer* time;

ClassDef(UserBase, 0)
};
#endif
