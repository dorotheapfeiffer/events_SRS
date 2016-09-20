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
#define NAPV 16
#define NCH 128

using namespace std;

class RootFile: public TObject
{

public:

	RootFile(TString fileName, TString pedestalName, bool isRawPedestalRun,
			bool isPedestalRun, bool isZSRun);
	~RootFile();

	void InitRootFile();
	void InitPedestalHistograms(int fecID, int apvID);
	void InitPedestalData(int fecID, int apvID);
	void SetRunFlags(bool isRawPedestalRun, bool isPedestalRun);
	float GetStripRawPedestalNoise(int fecID, int apvID, int stripNr);
	float GetStripRawPedestalOffset(int fecID, int apvID, int stripNr);
	float GetStripPedestalNoise(int fecID, int apvID, int stripNr);
	float GetStripPedestalOffset(int fecID, int apvID, int stripNr);
	float GetMeanPedestalOffset(int fecID, int apvID);

	void FillPedestalData(int fecID, int apvID, int stripNo, float mean);
	void CreateHistograms(int minFECID, int maxFECID, int minAPVID,
			int maxAPVID);
	void FillStripData(int stripNo, float rawdata);
	bool StripDataExists(int stripNo);
	float GetMeanStripData(int stripNo);
	float GetStripData(int stripNo, int timeBin);
	void ClearStripData(int stripNo);
	void WriteRootFile();
	void AddHits(signed int timestamp, unsigned int us, unsigned int eventId, unsigned int fecID, unsigned int apvID,
			unsigned int chNo, float maxADC, unsigned int timeBinMaxADC,
			std::vector<float> &timeBinADCs);
	void FillHits();

	void DeleteHitsTree();

	int GetStripNumber(int chNo);

private:
	bool isRawPedestalRun;
	bool isPedestalRun;
	bool isZSRun;

	TFile * fFile;
	TFile * fFilePedestal;
	TTree * fHitTree;
	TString fFileName;
	TString fPedestalName;

	std::vector<TH1F*> rawPedestalNoise;
	std::vector<TH1F*> rawPedestalOffset;
	std::vector<TH1F*> pedestalNoise;
	std::vector<TH1F*> pedestalOffset;
	std::vector<std::vector<TH1F*> > chipData;
	std::vector<std::vector<float>*> stripData;

	signed int m_timestamp; 		//Unix time stamp
	unsigned int m_us;
	unsigned int m_evtID;
	unsigned int m_chID;

	unsigned int * m_planeID;      // Plane Number
	unsigned int * m_fecID;        // APVId
	unsigned int * m_apvID;        // APVId
	unsigned int * m_strip_chip;   // Strip Number chip
	unsigned int * m_strip;        // Strip Number


	unsigned int * m_hitTimeBin;     //time bin with maximum ADC
	float* m_hitMaxADC; //Maximum ADC value of hit

	float * m_adc0;     //ADC value for 1st time sample
	float * m_adc1;     //ADC value for 2nd time sample
	float * m_adc2;     //ADC value for 3rd time sample
	float * m_adc3;     //ADC value for 4th time sample
	float * m_adc4;     //ADC value for 5th time sample
	float * m_adc5;     //ADC value for 6th time sample
	float * m_adc6;     //ADC value for 7th time sample
	float * m_adc7;     //ADC value for 8th time sample
	float * m_adc8;     //ADC value for 9th time sample
	float * m_adc9;     //ADC value for 10th time sample
	float * m_adc10;     //ADC value for 11th time sample
	float * m_adc11;     //ADC value for 12th time sample
	float * m_adc12;     //ADC value for 13th time sample
	float * m_adc13;     //ADC value for 14th time sample
	float * m_adc14;     //ADC value for 15th time sample
	float * m_adc15;     //ADC value for 16th time sample
	float * m_adc16;     //ADC value for 17th time sample
	float * m_adc17;     //ADC value for 18th time sample
	float * m_adc18;     //ADC value for 19th time sample
	float * m_adc19;     //ADC value for 20th time sample
	float * m_adc20;     //ADC value for 21th time sample
	float * m_adc21;     //ADC value for 22th time sample
	float * m_adc22;     //ADC value for 23th time sample
	float * m_adc23;     //ADC value for 24th time sample
	float * m_adc24;     //ADC value for 25th time sample
	float * m_adc25;     //ADC value for 26th time sample
	float * m_adc26;     //ADC value for 27th time sample
	float * m_adc27;     //ADC value for 28th time sample
	float * m_adc28;     //ADC value for 29th time sample
	float * m_adc29;     //ADC value for 30th time sample

	ClassDef(RootFile,1)
};

