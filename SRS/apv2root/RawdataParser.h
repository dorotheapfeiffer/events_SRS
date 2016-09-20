
#include <vector>
#include <list>
#include <map>

#include "RootFile.h"



class RawdataParser
{
public:
	RawdataParser(std::string fileName, std::string pedestalName, bool isRawPedestal, bool isPedestal, bool isZS, float ZSCut);
	~RawdataParser();
	void SetRunFlags(bool isRawPedestalRun, bool isPedestalRun);
	unsigned int AnalyzeWord(unsigned int eventID, unsigned int rawdata,unsigned int rawdata_before,unsigned int rawdata_before_two);
	void AnalyzeEventZS();
	void AnalyzeEvent();
	void ComputeRawPedestalData(int theApvID);
	void ComputePedestalData(int theApvID);
	void ComputeCorrectedData(int theApvID);
	void CreateHistograms();
private:
	RootFile *fRoot=0;
	signed int unixtimestamp = 0;
	unsigned int timestamp_us = 0;
	unsigned int eventNr = 0;
	unsigned int headerLDC = 0;
	unsigned int headerEquipment = 0;
	unsigned int header = 0;
	bool inEquipmentHeader = 0;

	unsigned int fecID = 0;
	unsigned int apvID = 0;
	unsigned int minFECID = 9999;
	unsigned int maxFECID = 0;
	unsigned int minAPVID = 9999;
	unsigned int maxAPVID = 0;

	unsigned int idata = 0;
	bool inEvent = false;

	bool isRawPedestalRun = false;
	bool isPedestalRun = false;
	bool isZSRun = false;
	bool startDataFlag = false;
	float fZsCut = 0;
	unsigned int wordCountEquipmentHeader = 0;
	unsigned int wordCountEvent = 0;
	unsigned int packetSize = 0;

	unsigned int numTimeBins = 0;
	unsigned int chNo = 0;
	float maxADC = 0;
	unsigned int timeBinMaxADC = 0;
	unsigned int apvheaderlevel = 1300;
	int theTimeBin=0;
	unsigned int fRawData16bits[2];
	std::vector<float> timeBinADCs;


	std::vector<float> commonModeOffset;
	std::vector<float> timeBinOffset;




};
