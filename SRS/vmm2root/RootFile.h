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
const long max_hit = 100000;

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

	void AddClusters(float clusterX, float clusterY,short clusterSize,
			unsigned int clusterADC, float clusterTime);
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

	unsigned long m_ncl;
	unsigned long m_nclX;
	unsigned long m_nclY;
	unsigned int m_clusterNr;
	short* m_clusterSize;
	float * m_clusterX;
	float * m_clusterY;
	unsigned int * m_clusterADC;
	float * m_clusterTime;

	ClassDef(RootFile,1)
};

