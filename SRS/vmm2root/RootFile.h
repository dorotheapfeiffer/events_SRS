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
const int minDeltaStrip = 2;
const int minDeltaT = 600;

using namespace std;

class RootFile: public TObject
{

public:

	RootFile(TString fileName);
	~RootFile();

	void InitRootFile();
	void WriteRootFile();
	void AddHits(signed int timestamp, unsigned int us, unsigned int eventNr,
			unsigned short fecID, unsigned short eventSize,
			unsigned int frameCounter, unsigned short vmmID,
			double triggerTimestamp, UChar_t overThresholdFlag,
			unsigned short chNo, short x, short y, short adc, short tdc,
			short bcid, double chipTime);

	void AddClusters(float clusterPosition, float clusterPositionUTPC,
			short clusterSize, unsigned int clusterADC, float clusterTime,
			string coordinate);
	void FillHits();

	void DeleteHitsTree();

private:

	TFile * fFile;
	TTree * fHitTree;

	TString fFileName;

	unsigned long m_nch;
	unsigned long m_nchX;
	unsigned long m_nchY;

	signed int m_timestamp; 		//Unix time stamp
	unsigned int m_us;
	unsigned int m_eventNr;

	unsigned short m_fecID;        // fecID
	unsigned short m_eventSize;

	UChar_t * m_overThresholdFlag;
	double *m_triggerTimestamp;
	unsigned int *m_frameCounter;
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
	float * m_clusterXUTPC;
	float * m_clusterYUTPC;
	unsigned int * m_clusterADCX;
	unsigned int * m_clusterADCY;
	float * m_clusterTimeX;
	float * m_clusterTimeY;

	std::multimap<float, float> clustersX;
	std::multimap<float, float> clustersY;

	ClassDef(RootFile,1)
};

