#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <TObject.h>
#include "TString.h"

#define NFEC 4
const long max_hit = 1000000000;
const int minDeltaStrip = 3;
const int minDeltaT = 100;

using namespace std;

class RootFile: public TObject
{

public:

	RootFile(std::string fileName, double bc, double tac,
			std::vector<int> xChips, std::vector<int> yChips, std::string readout,
			bool viewEvent, int viewStart, int viewEnd, int threshold,
			int clusterSize) ;
	~RootFile();

	void InitRootFile();
	void WriteRootFile();

	int AnalyzeData(long srs_timestamp, int fec, int chip_id,int channel, int bcid, int tdc, int adc, int overthreshold);

	void AddHits(unsigned int eventNr,
			unsigned short fecID, unsigned short vmmID,
			double triggerTimestamp, UChar_t overThresholdFlag,
			unsigned short chNo, short x, short y, short adc, short tdc,
			short bcid, double chipTime);

	void AddClusters(float clusterPosition, float clusterPositionUTPC,
			short clusterSize, unsigned int clusterADC, float clusterTime,
			string coordinate);
	void FillHits();
	unsigned int GetPlaneID(unsigned int chipID);
	unsigned int GetChannelX(unsigned int chipID, unsigned int channelID,
			std::string readout);
	unsigned int GetChannelY(unsigned int chipID, unsigned int channelID,
			std::string readout);

	int MapChipChannelToReadout(unsigned int chNo, std::string readout);
	int MMStripMappingHybrid1(unsigned int chNo);
	int MMStripMappingHybrid2(unsigned int chNo);
	int MMStripMappingHybrid3(unsigned int chNo);

	int clusterStrips(
			std::multimap<int, std::pair<double, unsigned int>> & cluster,
			int minDeltaStrip, string coordinate);
	int createClusters(
			std::multimap<double, std::pair<int, unsigned int>>& hits,
			double minDeltaT, int minDeltaStrip, string coordinate);
	void DeleteHitsTree();
	void SaveClusters();
private:

	TFile * fFile;
	TTree * fHitTree;

	TString fFileName;

	unsigned long m_nch;
	unsigned long m_nchX;
	unsigned long m_nchY;

	unsigned int m_eventNr;
	unsigned short m_fecID;        // fecID

	UChar_t * m_overThresholdFlag;
	double *m_triggerTimestamp;
	unsigned short * m_vmmID;        // vmmID
	unsigned short * m_chNo;        // Strip Number
	unsigned short * m_x;
	unsigned short * m_y;

	unsigned short * m_adc;     //ADC value
	unsigned short * m_tdc;     //TDC value
	unsigned short * m_bcid;    //BCID value
	double * m_chipTime;    //Composed time of BCID and TDC (1 ns resolution)

	unsigned long m_nclX;
	unsigned long m_nclY;
	unsigned short * m_clusterNumberX;
	unsigned short * m_clusterNumberY;
	unsigned short* m_clusterSizeX;
	unsigned short* m_clusterSizeY;
	float * m_clusterX;
	float * m_clusterY;
	float * m_clusterUTPCX;
	float * m_clusterUTPCY;
	unsigned int * m_clusterADCX;
	unsigned int * m_clusterADCY;
	double * m_clusterTimeX;
	double * m_clusterTimeY;

	std::multimap<double, float> clustersX;
	std::multimap<double, float> clustersY;

	std::multimap<double, std::pair<int, unsigned int> > hitsX;
	std::multimap<double, std::pair<int, unsigned int> > hitsY;

	double bcClock = 0;
	double tacSlope = 0;
	std::vector<int> xChipIDs;
	std::vector<int> yChipIDs;
	std::string readoutType;

	bool fViewEvent = false;
	int fViewStart = 0;
	int fViewEnd = 0;
	int fThreshold = 0;
	int fMinClusterSize = 0;

	unsigned int triggerTimestamp = 0;
	double theTriggerTimestamp = 0;
	double oldTriggerTimestamp = 0;
	double deltaTriggerTimestamp = 0;
	unsigned int flag = 0;
	unsigned int overThresholdFlag = 0;
	unsigned int adc = 0;
	unsigned int tdc = 0;
	unsigned int oldTdc = 0;
	unsigned int bcid = 0;
	unsigned int oldBcid = 0;
	unsigned int oldVmmID = -1;
	double chipTime = 0;
	double oldChipTime = 0;
	double bcTime = 0;
	double tdcTime = 0;

	unsigned int eventNr = 0;
	unsigned int fecID = 0;
	unsigned int vmmID = 0;
	unsigned int planeID = -1;
	unsigned int chNo = 0;

	int x = -1;
	int y = -1;


	ClassDef(RootFile,1)
};

