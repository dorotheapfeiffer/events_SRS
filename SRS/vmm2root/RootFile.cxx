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

	if (m_clusterSizeX)
		delete[] m_clusterSizeX;
	if (m_clusterSizeY)
		delete[] m_clusterSizeY;
	if (m_clusterX)
		delete[] m_clusterX;
	if (m_clusterXUTPC)
		delete[] m_clusterXUTPC;
	if (m_clusterY)
		delete[] m_clusterY;
	if (m_clusterYUTPC)
		delete[] m_clusterYUTPC;
	if (m_clusterADCX)
		delete[] m_clusterADCX;
	if (m_clusterTimeX)
		delete[] m_clusterTimeX;
	if (m_clusterADCY)
		delete[] m_clusterADCY;
	if (m_clusterTimeY)
		delete[] m_clusterTimeY;

}

//====================================================================================================================
void RootFile::AddHits(signed int timestamp, unsigned int us,
		unsigned int eventNr, unsigned short fecID, unsigned short eventSize,
		unsigned int frameCounter, unsigned short vmmID,
		double triggerTimestamp, UChar_t overThresholdFlag, unsigned short chNo,
		short x, short y, short adc, short tdc, short bcid, double chipTime)
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
		if (x > -1)
		{
			m_x[m_nchX] = x;
			m_nchX++;
		}
		if (y > -1)
		{
			m_y[m_nchY] = y;
			m_nchY++;
		}

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

//====================================================================================================================
void RootFile::AddClusters(float clusterPosition, float clusterPositionUTPC,
		short clusterSize, unsigned int clusterADC, float clusterTime,
		string coordinate)
{

	if (m_nclX < max_hit || m_nclY < max_hit)
	{
		if (coordinate == "x" && clusterPosition > -1.0)
		{
			m_clusterX[m_nclX] = clusterPosition;
			m_clusterXUTPC[m_nclX] = clusterPositionUTPC;
			m_clusterSizeX[m_nclX] = clusterSize;
			m_clusterADCX[m_nclX] = clusterADC;
			m_clusterTimeX[m_nclX] = clusterTime;
			clustersX.insert(std::make_pair(clusterTime, clusterPosition));
			m_nclX++;
		}
		if (coordinate == "y" && clusterPosition > -1.0)
		{
			m_clusterY[m_nclY] = clusterPosition;
			m_clusterYUTPC[m_nclY] = clusterPositionUTPC;
			m_clusterSizeY[m_nclY] = clusterSize;
			m_clusterADCY[m_nclY] = clusterADC;
			m_clusterTimeY[m_nclY] = clusterTime;
			clustersY.insert(std::make_pair(clusterTime, clusterPosition));
			m_nclY++;
		}

	}
	else
	{
		std::cout << "ERROR! More than " << max_hit << " clusters produced!"
				<< std::endl;
	}
}

void RootFile::FillHits()
{
	std::multimap<float, float>::iterator itClusterX = clustersX.begin();

	for (; itClusterX != clustersX.end(); itClusterX++)
	{
		float tx = itClusterX->first;
		float px = itClusterX->second;
		int nx = 0;

		std::multimap<float, float>::iterator itClusterY = clustersY.begin();
		m_clusterNumberY[nx] = 0;
		for (; itClusterY != clustersY.end(); itClusterY++)
		{
			float ty = itClusterY->first;
			float py = itClusterY->second;

			if (abs(ty - tx) <= minDeltaT)
			{
				m_clusterNumberY[nx]++;

				//std::cout << "Cluster time x " << tx << ", " << px << ", cluster time y "
				//	<< ty << ", " << py << " - " << m_clusterNumberY[nx] << std::endl;
			}
			if (ty - tx > minDeltaT)
			{
				break;
			}
		}
		nx++;
	}

	std::multimap<float, float>::iterator itClusterY = clustersY.begin();

	for (; itClusterY != clustersY.end(); itClusterY++)
	{
		float ty = itClusterY->first;
		float py = itClusterY->second;
		float tx = 0;
		float px = 0;
		int ny = 0;

		std::multimap<float, float>::iterator itClusterX = clustersX.begin();
		m_clusterNumberX[ny] = 0;
		for (; itClusterX != clustersX.end(); itClusterX++)
		{
			tx = itClusterX->first;

			if (abs(tx - ty) <= minDeltaT)
			{
				px = itClusterX->second;
				m_clusterNumberX[ny]++;
			}
			if (tx - ty > minDeltaT)
			{
				break;
			}
		}

		ny++;
	}
	if (m_nch > 0)
		fHitTree->Fill();
	//fHitTree->Write(0, TObject::kWriteDelete);
	m_nch = 0;
	m_nchX = 0;
	m_nchY = 0;
	m_nclX = 0;
	m_nclY = 0;
	clustersX.clear();
	clustersY.clear();
}

//====================================================================================================================
void RootFile::InitRootFile()
{
	m_eventNr = 0;
	m_nch = 0;
	m_nchX = 0;
	m_nchY = 0;

	m_nclX = 0;
	m_nclY = 0;

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
	m_x = new unsigned short[max_hit];
	m_y = new unsigned short[max_hit];
	m_adc = new unsigned short[max_hit];
	m_tdc = new unsigned short[max_hit];
	m_bcid = new unsigned short[max_hit];
	//m_gray_bcid = new unsigned short[max_hit];
	m_chipTime = new double[max_hit];
	m_clusterSizeX = new unsigned short[max_hit];
	m_clusterSizeY = new unsigned short[max_hit];
	m_clusterNumberX = new unsigned short[max_hit];
	m_clusterNumberY = new unsigned short[max_hit];
	m_clusterX = new float[max_hit];
	m_clusterY = new float[max_hit];
	m_clusterXUTPC = new float[max_hit];
	m_clusterYUTPC = new float[max_hit];
	m_clusterADCX = new unsigned int[max_hit];
	m_clusterTimeX = new float[max_hit];
	m_clusterADCY = new unsigned int[max_hit];
	m_clusterTimeY = new float[max_hit];

	fHitTree->Branch("timestamp", &m_timestamp, "timestamp/I");
	fHitTree->Branch("us", &m_us, "us/i");
	fHitTree->Branch("eventNr", &m_eventNr, "eventNr/i");
	fHitTree->Branch("nch", &m_nch, "nch/i");
	fHitTree->Branch("nchX", &m_nchX, "nchX/i");
	fHitTree->Branch("nchY", &m_nchY, "nchY/i");
	fHitTree->Branch("fecID", &m_fecID, "fecID/s");
	fHitTree->Branch("eventSize", &m_eventSize, "eventSize/s");
	fHitTree->Branch("frameCounter", m_frameCounter, "frameCounter[nch]/i");
	fHitTree->Branch("vmmID", m_vmmID, "vmmID[nch]/s");
	fHitTree->Branch("triggerTimestamp", m_triggerTimestamp,
			"triggerTimestamp[nch]/D");
	fHitTree->Branch("overThresholdFlag", m_overThresholdFlag,
			"overThresholdFlag[nch]/b");
	fHitTree->Branch("chNo", m_chNo, "chNo[nch]/s");
	fHitTree->Branch("x", m_x, "x[nchX]/s");
	fHitTree->Branch("y", m_y, "y[nchY]/s");
	fHitTree->Branch("adc", m_adc, "adc[nch]/s");
	fHitTree->Branch("tdc", m_tdc, "tdc[nch]/s");
	fHitTree->Branch("bcid", m_bcid, "bcid[nch]/s");
	fHitTree->Branch("chipTime", m_chipTime, "chipTime[nch]/D");

	fHitTree->Branch("nclX", &m_nclX, "nclX/i");
	fHitTree->Branch("nclY", &m_nclY, "nclY/i");

	fHitTree->Branch("clusterX", m_clusterX, "clusterX[nclX]/F");
	fHitTree->Branch("clusterXUTPC", m_clusterXUTPC, "clusterXUTPC[nclX]/F");
	fHitTree->Branch("clusterSizeX", m_clusterSizeX, "clusterSizeX[nclX]/s");
	fHitTree->Branch("clusterADCX", m_clusterADCX, "clusterADC[nclX]/s");
	fHitTree->Branch("clusterTimeX", m_clusterTimeX, "clusterTime[nclX]/F");
	fHitTree->Branch("clusterNumberY", m_clusterNumberY,
			"m_clusterNumberY[nclX]/s");
	fHitTree->Branch("clusterY", m_clusterY, "clusterY[nclY]/F");
	fHitTree->Branch("clusterYUTPC", m_clusterYUTPC, "clusterYUTPC[nclY]/F");
	fHitTree->Branch("clusterSizeY", m_clusterSizeY, "clusterSizeY[nclY]/s");
	fHitTree->Branch("clusterADCY", m_clusterADCY, "clusterADC[nclY]/s");
	fHitTree->Branch("clusterTimeY", m_clusterTimeY, "clusterTime[nclY]/F");
	fHitTree->Branch("clusterNumberX", m_clusterNumberX,
			"m_clusterNumberX[nclY]/s");

	std::cout << "Root file " << fFileName << " created!" << std::endl;

}

//====================================================================================================================
void RootFile::WriteRootFile()
{

	fFile->Write();
}

