#ifndef Files_h
#define Files_h

#include <TObject.h>
#include <TString.h>

class TObject;

class Files : public TObject {

public:
	static Files& getInstance(){
		static Files aFiles;
		return aFiles;
		}
	void AddFile(TString s){
		files.push_back(s);
		}
	TString GetFile(Int_t a){
		return files.at(a);
		}
	void FileListClear(){
		files.clear();
		}
	void SetDirectory( TString &d){
		filesDir = d;
		}
	Int_t GetVersion(){
		return version;
		}
	Int_t GetSize(){
		return files.size();
		}

	TString GetDirectory(){
		return filesDir;
		}

private:
	Files() : filesDir(0), version(1) {}
	Files(Files const&);
	void operator=(Files const &);

	virtual ~Files(){ files.clear(); }
 
        TString		filesDir;               
        Int_t		version;                
        std::vector<TString> files; 


ClassDef(Files, 0)
};

#endif
