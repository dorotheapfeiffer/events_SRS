#include <vector>
#include <list>
#include <map>

#include "RootFile.h"

class RawdataParser
{
public:
	RawdataParser(std::string fileName, double bc, double tac,
			std::vector<int> xChipIDs, std::vector<int> yChipIDs,
			std::string readout, bool viewEvent, int viewStart, int viewEnd);
	~RawdataParser();

	unsigned int AnalyzeWord(unsigned int rawdata, unsigned int rawdata_before,
			unsigned int rawdata_before_two);
	unsigned int GrayToBinary32(unsigned int num);
	unsigned int ReverseBits(unsigned int n);
	unsigned int GetPlaneID(unsigned int chipID);
	unsigned int GetChannelX(unsigned int chipID, unsigned int channelID,
			std::string readout);
	unsigned int GetChannelY(unsigned int chipID, unsigned int channelID,
			std::string readout);

	int MapChipChannelToReadout(unsigned int chNo, std::string readout);
	int MMStripMappingHybrid1(unsigned int chNo);
	int MMStripMappingHybrid2(unsigned int chNo);
	int MMStripMappingHybrid3(unsigned int chNo);

	int clusterStrips(std::multimap<int, std::pair<double, unsigned int>> & cluster,
			int minDeltaStrip);
	int createClusters(std::multimap<double, std::pair<int, unsigned int>>& hits,
			double minDeltaT,int minDeltaStrip);
private:
	RootFile *fRoot = 0;
	double bcClock = 0;
	double tacSlope = 0;
	std::vector<int> xChipIDs;
	std::vector<int> yChipIDs;
	std::string readoutType;
	bool fViewEvent = false;
	int fViewStart = 0;
	int fViewEnd = 0;
	long discarded = 0;

	std::multimap<double, std::pair<int, unsigned int> > hitsX;
	std::multimap<double, std::pair<int, unsigned int> > hitsY;

	signed int unixtimestamp = 0;
	unsigned int timestamp_us = 0;
	signed int oldUnixtimestamp = 0;
	unsigned int oldTimestamp_us = 0;
	double deltaUnixTimestamp = 0;
	unsigned int frameCounter = 0;
	unsigned int triggerTimestamp = 0;
	double theTriggerTimestamp = 0;
	double oldTriggerTimestamp = 0;
	double deltaTriggerTimestamp = 0;
	unsigned int flag = 0;
	unsigned int overThresholdFlag = 0;
	unsigned int adc = 0;
	unsigned int tdc = 0;
	unsigned int bcid = 0;
	unsigned int oldVmmID = -1;
	unsigned int oldAdc = -1;
	unsigned int oldTdc = -1;
	unsigned int oldBcid = -1;
	unsigned int oldChNo = -1;
	double chipTime = 0;
	double oldChipTime = 0;
	double bcTime = 0;
	double tdcTime = 0;

	unsigned int runNr = 0;
	unsigned int eventNr = 0;
	unsigned int eventSize = 0;
	unsigned int headerEquipment = 0;
	unsigned int header = 0;
	bool inEquipmentHeader = 0;
	unsigned int fecID = 0;
	unsigned int vmmID = 0;
	unsigned int planeID = -1;
	unsigned int chNo = 0;

	int x = -1;
	int oldX = -1;
	int y = -1;
	bool inEvent = false;
	bool validEvent = false;
	unsigned int wordCountEquipmentHeader = 0;
	unsigned int wordCountEvent = 0;

};
