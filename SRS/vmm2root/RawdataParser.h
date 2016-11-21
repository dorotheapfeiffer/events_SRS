
#include <vector>
#include <list>
#include <map>

#include "RootFile.h"



class RawdataParser
{
public:
	RawdataParser(std::string fileName, bool viewEvent, int viewStart, int viewEnd);
	~RawdataParser();
	
	unsigned int AnalyzeWord(unsigned int eventID,  unsigned int rawdata,unsigned int rawdata_before,unsigned int rawdata_before_two);
	
	unsigned int ReverseBits(unsigned int n);
private:
	RootFile *fRoot=0;
	bool fViewEvent=false;
	int fViewStart=0;
	int fViewEnd=0;
	signed int unixtimestamp = 0;
	unsigned int timestamp_us = 0;
	unsigned int frameCounter = 0;
	unsigned int triggerCount = 0;
	unsigned int triggerTimestamp = 0;
	unsigned int flag = 0;
	unsigned int overThresholdFlag = 0;
	unsigned int adc = 0;
	unsigned int tdc = 0;
	unsigned int bcid = 0;
	
	unsigned int runNr = 0;
	unsigned int eventNr = 0;
	unsigned int eventSize = 0;
	unsigned int headerEquipment = 0;
	unsigned int header = 0;
	bool inEquipmentHeader = 0;
	unsigned int fecID = 0;
	unsigned int vmmID = 0;
	unsigned int chNo = 0;


	bool inEvent = false;
	bool validEvent = false;
	unsigned int wordCountEquipmentHeader = 0;
	unsigned int wordCountEvent = 0;

};
