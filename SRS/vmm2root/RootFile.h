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
const long max_hit = 10000000000;

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
			unsigned short chNo, unsigned short x, unsigned short y, short adc,
			short tdc, short bcid, double chipTime);
	void FillHits();

	void DeleteHitsTree();

private:

	TFile * fFile;
	TTree * fHitTree;
	TString fFileName;

	unsigned long m_nch;

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
	short * m_x;
	short * m_y;

	unsigned short * m_adc;     //ADC value
	unsigned short * m_tdc;     //TDC value
	unsigned short * m_bcid;    //BCID value
	double * m_chipTime;    //Composed time of BCID and TDC (1 ns resolution)

	ClassDef(RootFile,1)
};

