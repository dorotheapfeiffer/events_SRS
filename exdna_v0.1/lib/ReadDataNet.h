#ifndef ReadDataNet_h
#define ReadDataNet_h

#include <TString.h>
#include "ReadData.h"

class ReadDataNet : public ReadData {

	ReadDataNet();
	ReadDataNet(const std::string& name);
	virtual ~ReadDataNet();
	friend class ReadData;
private:

ClassDef(ReadDataNet, 0)
};
#endif
