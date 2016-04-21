#ifndef ReadDataFile_h
#define ReadDataFile_h

#include "ReadData.h"

class ReadDataFile : public ReadData {

        ReadDataFile();
        ReadDataFile(const std::string& name);
        virtual ~ReadDataFile();
	friend class ReadData;
private:

ClassDef(ReadDataFile, 0)
};

#endif

