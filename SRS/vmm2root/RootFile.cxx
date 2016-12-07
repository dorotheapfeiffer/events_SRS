#include "RootFile.h"
#include <TROOT.h>
#include <TStyle.h>
#include "TMath.h"

RootFile::RootFile(TString fileName) :
		fFileName(fileName)
{
	InitRootFile();
}

//====================================================================================================================
RootFile::~RootFile()
{

	fFile->Close();
	DeleteHitsTree();
}

//====================================================================================================================
void RootFile::DeleteHitsTree()
{

	if (m_adc)
		delete[] m_adc;

	if (m_tdc)
		delete[] m_tdc;

	if (m_bcid)
		delete[] m_bcid;
	if (m_chipTime)
		delete m_chipTime;
	//if (m_gray_bcid)
	//	delete[] m_gray_bcid;

	if (m_chNo)
		delete[] m_chNo;
	if (m_x)
		delete[] m_x;
	if (m_y)
		delete[] m_y;

	//if (m_triggerCount)
	//	delete[] m_triggerCount;
	if (m_frameCounter)
		delete[] m_frameCounter;
	if (m_triggerTimestamp)
		delete[] m_triggerTimestamp;
	//if (m_clockCycles)
	//	delete[] m_clockCycles;
	if (m_overThresholdFlag)
		delete[] m_overThresholdFlag;
	if (m_vmmID)
		delete[] m_vmmID;

}

//====================================================================================================================
void RootFile::AddHits(signed int timestamp, unsigned int us,
		unsigned int eventNr, unsigned short fecID, unsigned short eventSize,
		unsigned int frameCounter, unsigned short vmmID,
		double triggerTimestamp, UChar_t overThresholdFlag, unsigned short chNo,
		unsigned short x, unsigned short y, short adc, short tdc, short bcid,
		double chipTime)
{
	if (m_nch < max_hit)
	{

		m_eventNr = eventNr;

		m_timestamp = timestamp;
		m_us = us;
		m_fecID = fecID;
		m_eventSize = eventSize;

		m_frameCounter[m_nch] = frameCounter;

		m_triggerTimestamp[m_nch] = triggerTimestamp;
		m_overThresholdFlag[m_nch] = overThresholdFlag;
		m_vmmID[m_nch] = vmmID;
		m_chNo[m_nch] = chNo;
		m_x[m_nch] = x;
		m_y[m_nch] = y;

		m_adc[m_nch] = adc;
		m_tdc[m_nch] = tdc;
		m_bcid[m_nch] = bcid;
		m_chipTime[m_nch] = chipTime;
		m_nch++;

	}
	else
	{
		std::cout << "ERROR! More than " << max_hit << " channels hit!"
				<< std::endl;
	}
}

void RootFile::FillHits()
{
	fHitTree->Fill();
	//fHitTree->Write(0, TObject::kWriteDelete);
	m_nch = 0;
}

//====================================================================================================================
void RootFile::InitRootFile()
{
	m_eventNr = 0;
	m_nch = 0;

	fFile = TFile::Open(fFileName, "RECREATE");

	fHitTree = new TTree("events", "vmm2 events");
	fHitTree->SetDirectory(fFile);
	m_frameCounter = new unsigned int[max_hit];
	m_vmmID = new unsigned short[max_hit];
	//m_triggerCount = new unsigned int[max_hit];
	m_triggerTimestamp = new double[max_hit];
	//m_clockCycles = new unsigned int[max_hit];
	m_overThresholdFlag = new UChar_t[max_hit];
	m_chNo = new unsigned short[max_hit];
	m_x = new short[max_hit];
	m_y = new short[max_hit];
	m_adc = new unsigned short[max_hit];
	m_tdc = new unsigned short[max_hit];
	m_bcid = new unsigned short[max_hit];
	//m_gray_bcid = new unsigned short[max_hit];
	m_chipTime = new double[max_hit];

	fHitTree->Branch("timestamp", &m_timestamp, "timestamp/I");
	fHitTree->Branch("us", &m_us, "us/i");
	fHitTree->Branch("eventNr", &m_eventNr, "eventNr/i");
	fHitTree->Branch("nch", &m_nch, "nch/i");
	fHitTree->Branch("fecID", &m_fecID, "fecID/s");
	fHitTree->Branch("eventSize", &m_eventSize, "eventSize/s");
	fHitTree->Branch("frameCounter", m_frameCounter, "frameCounter[nch]/i");
	fHitTree->Branch("vmmID", m_vmmID, "vmmID[nch]/s");
	//fHitTree->Branch("triggerCount", m_triggerCount, "triggerCount[nch]/i");
	fHitTree->Branch("triggerTimestamp", m_triggerTimestamp,
			"triggerTimestamp[nch]/D");
	//fHitTree->Branch("clockCycles", m_clockCycles,
	//			"clockCycles[nch]/i");
	fHitTree->Branch("overThresholdFlag", m_overThresholdFlag,
			"overThresholdFlag[nch]/b");
	fHitTree->Branch("chNo", m_chNo, "chNo[nch]/s");
	fHitTree->Branch("x", m_x, "x[nch]/S");
	fHitTree->Branch("y", m_y, "y[nch]/S");
	fHitTree->Branch("adc", m_adc, "adc[nch]/s");
	fHitTree->Branch("tdc", m_tdc, "tdc[nch]/s");
	fHitTree->Branch("bcid", m_bcid, "bcid[nch]/s");
	//fHitTree->Branch("gray_bcid", m_gray_bcid, "gray_bcid[nch]/s");
	fHitTree->Branch("chipTime", m_chipTime, "chipTime[nch]/D");

	std::cout << "Root file " << fFileName << " created!" << std::endl;

}

//====================================================================================================================
void RootFile::WriteRootFile()
{

	fFile->Write();
}

