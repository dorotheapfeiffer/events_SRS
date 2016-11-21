
#include <vector>
#include <list>
#include <map>

#include "RootFile.h"



class RawdataParser
{
public:
	RawdataParser(std::string fileName, std::string pedestalName, bool isRawPedestal, bool isPedestal, bool isZS, float ZSCut, bool isUPTC, int uTPCThreshold);
	~RawdataParser();
	void SetRunFlags(bool isRawPedestal, bool isPedestal);
	int AnalyzeWord(int eventID, int rawdata,int rawdata_before,int rawdata_before_two);
	void AnalyzeEventZS();
	void AnalyzeEvent();
	void ComputeRawPedestalData(int theApvID);
	void ComputePedestalData(int theApvID);
	void ComputeCorrectedData(int theApvID);
	void CreateHistograms();
private:
	RootFile *fRoot=0;
	signed int unixtimestamp = 0;
	int timestamp_us = 0;
	int eventNr = 0;
	int headerLDC = 0;
	int headerEquipment = 0;
	int header = 0;
	bool inEquipmentHeader = 0;

	int fecID = 0;
	int apvID = 0;
	int minFECID = 9999;
	int maxFECID = 0;
	int minAPVID = 9999;
	int maxAPVID = 0;

	int idata = 0;
	bool inEvent = false;

	bool isRawPedestalRun = false;
	bool isPedestalRun = false;
	bool isZSRun = false;
	bool startDataFlag = false;
	float fZsCut = 0;
	int wordCountEquipmentHeader = 0;
	int wordCountEvent = 0;
	int packetSize = 0;

	int numTimeBins = 0;
	int chNo = 0;
	float maxADC = 0;
	int timeBinMaxADC = 0;
	int apvheaderlevel = 1300;
	int theTimeBin=0;
	unsigned int fRawData16bits[2];
	std::vector<float> timeBinADCs;


	std::vector<float> commonModeOffset;
	std::vector<float> timeBinOffset;




};