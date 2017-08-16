#include "RootFile.h"
#include <TROOT.h>
#include <TStyle.h>
#include "TMath.h"
#include <time.h>

RootFile::RootFile(std::string fileName, double bc, double tac,
		std::vector<int> xChips, std::vector<int> yChips, std::string readout,
		bool viewEvent, int viewStart, int viewEnd, int threshold,
		int clusterSize) :
		bcClock(bc), tacSlope(tac), xChipIDs(xChips), yChipIDs(yChips), readoutType(
				readout), fViewEvent(viewEvent), fViewStart(viewStart), fViewEnd(
				viewEnd), fThreshold(threshold), fMinClusterSize(clusterSize)
{
	if (!fViewEvent)
	{
		std::stringstream ending;
		ending << "root";
		fileName.replace(fileName.size() - 4, fileName.size(), ending.str());
		fFileName = fileName;
		InitRootFile();
	}
}

RootFile::~RootFile()
{
	if (!fViewEvent)
	{
		WriteRootFile();
		fFile->Close();
		DeleteHitsTree();
	}
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

	if (m_chNo)
		delete[] m_chNo;
	if (m_x)
		delete[] m_x;
	if (m_y)
		delete[] m_y;

	if (m_triggerTimestamp)
		delete[] m_triggerTimestamp;
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
	if (m_clusterUTPCX)
		delete[] m_clusterUTPCX;
	if (m_clusterY)
		delete[] m_clusterY;
	if (m_clusterUTPCY)
		delete[] m_clusterUTPCY;
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
void RootFile::AddHits(unsigned int eventNr, unsigned short fecID,
		unsigned short vmmID, double triggerTimestamp,
		UChar_t overThresholdFlag, unsigned short chNo, short x, short y,
		short adc, short tdc, short bcid, double chipTime)
{

	if (m_nch < max_hit)
	{

		m_eventNr = eventNr;
		m_fecID = fecID;

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
			m_clusterUTPCX[m_nclX] = clusterPositionUTPC;
			m_clusterSizeX[m_nclX] = clusterSize;
			m_clusterADCX[m_nclX] = clusterADC;
			m_clusterTimeX[m_nclX] = clusterTime;
			m_clusterNumberX[m_nclX] = 0;
			clustersX.insert(std::make_pair(clusterTime, clusterPosition));
			m_nclX++;
		}
		if (coordinate == "y" && clusterPosition > -1.0)
		{
			m_clusterY[m_nclY] = clusterPosition;
			m_clusterUTPCY[m_nclY] = clusterPositionUTPC;
			m_clusterSizeY[m_nclY] = clusterSize;
			m_clusterADCY[m_nclY] = clusterADC;
			m_clusterTimeY[m_nclY] = clusterTime;
			m_clusterNumberY[m_nclY] = 0;
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


	std::multimap<double, float>::iterator itClusterX = clustersX.begin();


	int nx = 0;

	for (; itClusterX != clustersX.end(); itClusterX++)
	{
		double tx = itClusterX->first;

		int ny = 0;
		double oldDeltaT = 999999999999;
		double deltaT = 0;
		std::multimap<double, float>::iterator itClusterY = clustersY.begin();
		for (; itClusterY != clustersY.end(); itClusterY++)
		{

			double ty  = itClusterY->first;
			deltaT = abs(ty - tx) ;
			if (deltaT <= minDeltaT && m_clusterNumberY[ny]==0 && m_clusterNumberX[nx]==0)
			{

				m_clusterNumberY[ny]=1;
				m_clusterNumberX[nx]=1;
				break;
			}
			oldDeltaT = deltaT ;
			ny++;

		}
		nx++;
	}
/*
	std::multimap<double, float>::iterator itClusterY = clustersY.begin();


	int ny = 0;
	for (; itClusterY != clustersY.end(); itClusterY++)
	{
		double ty = itClusterY->first;
		if (m_eventNr >= 564326 && m_eventNr <= 564326)
		std::cout << m_eventNr << " y cluster " << " " << ty << std::endl;
		double tx = 0;
		m_clusterNumberX[ny] = 0;
		std::multimap<double, float>::iterator itClusterX = tmpClustersX.begin();
		for (; itClusterX != tmpClustersX.end(); itClusterX++)
		{
			tx = itClusterX->first;
			if (m_eventNr >= 564326 && m_eventNr <= 564326)
			std::cout << tx << std::endl;
			if (abs(tx - ty) <= minDeltaT && m_clusterNumberX[ny] == 0)
			{
				m_clusterNumberX[ny]++;
				if (m_eventNr >= 564326 && m_eventNr <= 564326)
					std::cout << m_eventNr << " y cluster " << ty << " " << ny
							<< " " << m_clusterNumberX[ny] << " x cluster "
							<< tx << std::endl;
				tmpClustersX.erase(itClusterX);
				break;
			}

		}

		ny++;
	}
*/
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

	fHitTree = new TTree("events", "vmm3 events");
	fHitTree->SetDirectory(fFile);
	m_vmmID = new unsigned short[max_hit];
	m_triggerTimestamp = new double[max_hit];
	m_overThresholdFlag = new UChar_t[max_hit];
	m_chNo = new unsigned short[max_hit];
	m_x = new unsigned short[max_hit];
	m_y = new unsigned short[max_hit];
	m_adc = new unsigned short[max_hit];
	m_tdc = new unsigned short[max_hit];
	m_bcid = new unsigned short[max_hit];
	m_chipTime = new double[max_hit];
	m_clusterSizeX = new unsigned short[max_hit];
	m_clusterSizeY = new unsigned short[max_hit];
	m_clusterNumberX = new unsigned short[max_hit];
	m_clusterNumberY = new unsigned short[max_hit];
	m_clusterX = new float[max_hit];
	m_clusterY = new float[max_hit];
	m_clusterUTPCX = new float[max_hit];
	m_clusterUTPCY = new float[max_hit];
	m_clusterADCX = new unsigned int[max_hit];
	m_clusterTimeX = new double[max_hit];
	m_clusterADCY = new unsigned int[max_hit];
	m_clusterTimeY = new double[max_hit];

	fHitTree->Branch("eventNr", &m_eventNr, "eventNr/i");
	fHitTree->Branch("nch", &m_nch, "nch/i");
	fHitTree->Branch("nchX", &m_nchX, "nchX/i");
	fHitTree->Branch("nchY", &m_nchY, "nchY/i");
	fHitTree->Branch("fecID", &m_fecID, "fecID/s");
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
	fHitTree->Branch("clusterUTPCX", m_clusterUTPCX, "clusterUTPCX[nclX]/F");
	fHitTree->Branch("clusterSizeX", m_clusterSizeX, "clusterSizeX[nclX]/s");
	fHitTree->Branch("clusterADCX", m_clusterADCX, "clusterADC[nclX]/s");
	fHitTree->Branch("clusterTimeX", m_clusterTimeX, "clusterTime[nclX]/D");
	fHitTree->Branch("clusterNumberX", m_clusterNumberX,
			"m_clusterNumberX[nclX]/s");
	fHitTree->Branch("clusterY", m_clusterY, "clusterY[nclY]/F");
	fHitTree->Branch("clusterUTPCY", m_clusterUTPCY, "clusterUTPCY[nclY]/F");
	fHitTree->Branch("clusterSizeY", m_clusterSizeY, "clusterSizeY[nclY]/s");
	fHitTree->Branch("clusterADCY", m_clusterADCY, "clusterADC[nclY]/s");
	fHitTree->Branch("clusterTimeY", m_clusterTimeY, "clusterTime[nclY]/D");
	fHitTree->Branch("clusterNumberY", m_clusterNumberY,
			"m_clusterNumberY[nclY]/s");

	std::cout << "Root file " << fFileName << " created!" << std::endl;

}

//====================================================================================================================
void RootFile::WriteRootFile()
{

	fFile->Write();
}

void RootFile::SaveClusters()
{
	if (!fViewEvent)
	{
		int clusterCount = createClusters(hitsX, minDeltaT, minDeltaStrip, "x");
		clusterCount = createClusters(hitsY, minDeltaT, minDeltaStrip, "y");

		FillHits();
		hitsX.clear();
		hitsY.clear();
	}
}

int RootFile::AnalyzeData(long triggerTimestamp, int fecID, int vmmID, int chNo,
		int bcid, int tdc, int adc, int overThresholdFlag)
{
	int newEvent = 0;
	theTriggerTimestamp = triggerTimestamp * 3.125;
	if (oldTriggerTimestamp != theTriggerTimestamp)
	{
		eventNr++;
		newEvent = 1;

		if (!fViewEvent)
		{
			SaveClusters();
		}

	}
	if (oldVmmID != vmmID || newEvent)
	{
		oldChipTime = 0;
	}

	if (oldTriggerTimestamp > theTriggerTimestamp)
	{
		deltaTriggerTimestamp = (13421772800 + theTriggerTimestamp
				- oldTriggerTimestamp) * 0.001;

	}
	else
	{
		deltaTriggerTimestamp = (theTriggerTimestamp - oldTriggerTimestamp)
				* 0.001;

	}
	if (bcid == 0 && tdc == 0 && overThresholdFlag)
	{
		bcid = oldBcid;
		tdc = oldTdc;
	}
	//bcTime [us]
	bcTime = bcid * (1 / (double) bcClock);

	//TDC time: tacSlope * tdc value (8 bit) * ramp length
	// [ns]
	tdcTime = tacSlope * (double) tdc / 255;
	//Chip time: bcid plus tdc value
	//Talk Vinnie: HIT time  = BCIDx25 + ADC*125/255 [ns]

	chipTime = bcTime * 1000 + tdcTime;

	if (oldChipTime > (chipTime + 1000))
	{
//		bcTime = (bcid + 4096) * (1 / (double) bcClock);
//		chipTime = bcTime * 1000 + tdcTime;
	}

	planeID = GetPlaneID(vmmID);
	// Plane 0: x
	// plane 1: y
	if (planeID == 0)
	{
		x = GetChannelX(vmmID, chNo, readoutType);
		y = -1;
	}
	else if (planeID == 1)
	{
		y = GetChannelY(vmmID, chNo, readoutType);
		x = -1;
	}
	else
	{
		x = -1;
		y = -1;
	}
	if (!fViewEvent)
	{
		AddHits(eventNr, fecID, vmmID, triggerTimestamp, overThresholdFlag,
				chNo, x, y, adc, tdc, bcid, chipTime);

		if (x > -1 && (adc >= fThreshold || overThresholdFlag))
		{
			hitsX.insert(std::make_pair(chipTime, std::make_pair(x, adc)));
		}
		if (y > -1 && (adc >= fThreshold || overThresholdFlag))
		{
			hitsY.insert(std::make_pair(chipTime, std::make_pair(y, adc)));
		}
	}
	else
	{
		if (fViewEnd < eventNr && fViewEnd != 0)
		{
			return -1;
		}
		if (fViewStart <= eventNr && fViewEnd >= eventNr)
		{
			if (newEvent)
			{
				printf("\neventNr  %d\n", eventNr);
				printf("fecID  %d\n", fecID);
			}

			if (deltaTriggerTimestamp > 0)
			{
				printf("\tTime since last trigger %.4f us (%.4f kHz)\n",
						deltaTriggerTimestamp, 1000 / deltaTriggerTimestamp);
				printf("\tTriggerTimestamp %.2f\n", theTriggerTimestamp);
			}
			if (oldVmmID != vmmID || newEvent)
			{
				printf("\tvmmID  %d\n", vmmID);
			}
			if (planeID == 0)
			{
				printf("\t\tx-channel %d (chNo  %d) - overThresholdFlag %d\n",
						x, chNo, overThresholdFlag);
			}
			else if (planeID == 1)
			{
				printf("\t\ty-channel %d (chNo  %d) - overThresholdFlag %d\n",
						y, chNo, overThresholdFlag);
			}
			else
			{
				printf("\t\tPlane for vmmID %d not defined!\n", vmmID);
			}
			printf("\t\t\tbcid %d, tdc %d, adc %d\n", bcid, tdc, adc);
			printf("\t\t\tbcTime %.2f us, tdcTime %.2f ns, time %.2f us\n",
					bcTime, tdcTime, chipTime * 0.001);

		}
	}
	oldTriggerTimestamp = theTriggerTimestamp;
	oldChipTime = chipTime;
	oldTdc = tdc;
	oldBcid = bcid;
	oldVmmID = vmmID;
	return 0;

}

unsigned int RootFile::GetPlaneID(unsigned int chipID)
{
	std::vector<int>::iterator it;

	it = find(xChipIDs.begin(), xChipIDs.end(), chipID);
	if (it != xChipIDs.end())
	{
		return 0;
	}
	else
	{
		it = find(yChipIDs.begin(), yChipIDs.end(), chipID);
		if (it != yChipIDs.end())
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

unsigned int RootFile::GetChannelX(unsigned int chipID, unsigned int channelID,
		std::string readout)
{
	std::vector<int>::iterator it;

	it = find(xChipIDs.begin(), xChipIDs.end(), chipID);
	if (it != xChipIDs.end())
	{
		int pos = it - xChipIDs.begin();
		return MapChipChannelToReadout(channelID + pos * 64, readout);
	}
	else
	{
		return -1;
	}
}

unsigned int RootFile::GetChannelY(unsigned int chipID, unsigned int channelID,
		std::string readout)
{
	std::vector<int>::iterator it;

	it = find(yChipIDs.begin(), yChipIDs.end(), chipID);
	if (it != yChipIDs.end())
	{
		int pos = it - yChipIDs.begin();
		return MapChipChannelToReadout(channelID + pos * 64, readout);;
	}
	else
	{
		return -1;
	}
}

int RootFile::MapChipChannelToReadout(unsigned int chNo, std::string readout)
{
	if (readout == "MM1" || readout == "mm1")
	{
		return MMStripMappingHybrid1(chNo);
	}
	if (readout == "MM2" || readout == "mm2")
	{
		return MMStripMappingHybrid2(chNo);
	}
	if (readout == "MM3" || readout == "mm3")
	{
		return MMStripMappingHybrid3(chNo);
	}
	if (readout == "GEM" || readout == "gem")
	{
		return (chNo);
	}
	return (chNo);
}

int RootFile::MMStripMappingHybrid1(unsigned int chNo)
{
	if ((chNo % 2) == 1)
	{
		chNo = ((chNo - 1) / 2) + 32;
	}
	else
	{
		chNo = (chNo / 2);
		if (chNo < 32)
			chNo = 31 - chNo;
		else if (chNo > 37)
			chNo = 159 - chNo;
		else
			chNo += 90;
	}
	return chNo;
}

//=====================================================
int RootFile::MMStripMappingHybrid2(unsigned int chNo)
{
	if ((chNo % 2) == 1)
	{
		chNo = ((chNo - 1) / 2) + 27;
	}
	else
	{
		chNo = (chNo / 2);
		if (chNo < 27)
			chNo = 26 - chNo;
		else if (chNo > 38)
			chNo = 154 - chNo;
		else
			chNo += 89;
	}
	return chNo;
}

int RootFile::MMStripMappingHybrid3(unsigned int chNo)
{
	if ((chNo % 2) == 1)
	{
		chNo = ((chNo - 1) / 2) + 26;
	}
	else
	{
		chNo = (chNo / 2);
		if (chNo < 26)
			chNo = 25 - chNo;
		else if (chNo > 31)
			chNo = 153 - chNo;
		else
			chNo += 96;
	}
	return chNo;
}

int RootFile::createClusters(
		std::multimap<double, std::pair<int, unsigned int>>& hits,
		double minDeltaT, int minDeltaStrip, string coordinate)
{
	std::multimap<int, std::pair<double, unsigned int> > cluster;

	std::multimap<double, std::pair<int, unsigned int>>::iterator itHits =
			hits.begin();

	int stripCount = 0;
	int clusterCount = 0;
	double time1 = 0, time2 = 0;
	unsigned int adc1 = 0;
	int strip1 = 0;
	for (; itHits != hits.end(); itHits++)
	{
		time2 = time1;
		time1 = itHits->first;
		strip1 = itHits->second.first;
		adc1 = itHits->second.second;

		if ((time1 - time2 > minDeltaT) && stripCount > 0)
		{
			clusterCount += clusterStrips(cluster, minDeltaStrip, coordinate);
			cluster.clear();
		}
		cluster.insert(std::make_pair(strip1, std::make_pair(time1, adc1)));
		stripCount++;
	}
	clusterCount += clusterStrips(cluster, minDeltaStrip, coordinate);
	return clusterCount;
}

int RootFile::clusterStrips(
		std::multimap<int, std::pair<double, unsigned int>> & cluster,
		int minDeltaStrip, string coordinate)
{

	std::multimap<int, std::pair<double, unsigned int> >::iterator itCluster =
			cluster.begin();
	double lastTime = 0;
	double lastTimeStrip = 0;
	double centerOfGravity = 0;
	double centerOfTime = 0;
	unsigned int totalADC = 0;
	double time1 = 0;
	unsigned int adc1 = 0;
	int strip1 = 0, strip2 = 0;
	int stripCount = 0;
	int clusterCount = 0;

	for (; itCluster != cluster.end(); itCluster++)
	{
		adc1 = itCluster->second.second;
		strip2 = strip1;
		strip1 = itCluster->first;
		time1 = itCluster->second.first;

		if (stripCount == 0
				|| (abs(strip1 - strip2) > 0
						&& abs(strip1 - strip2) <= minDeltaStrip))
		{
			if (time1 > lastTime)
			{
				lastTime = time1;
				lastTimeStrip = strip1;
			}
			centerOfGravity += strip1 * adc1;
			centerOfTime += time1 * adc1;
			totalADC += adc1;
			stripCount++;

		}
		else if (abs(strip1 - strip2) > minDeltaStrip)
		{
			centerOfGravity = (centerOfGravity / (double) totalADC);
			centerOfTime = (centerOfTime / totalADC);
			if (stripCount >= fMinClusterSize)
			{
				AddClusters(centerOfGravity, lastTimeStrip, stripCount,
						totalADC, centerOfTime, coordinate);
			}

			clusterCount++;
			lastTime = 0;
			lastTimeStrip = 0;
			stripCount = 0;
			centerOfGravity = 0;
			centerOfTime = 0;
			totalADC = 0;
			strip1 = 0;
		}
	}
	if (stripCount > 0)
	{
		centerOfGravity = (centerOfGravity / (double) totalADC);
		centerOfTime = (centerOfTime / totalADC);
		if (stripCount >= fMinClusterSize)
		{
			AddClusters(centerOfGravity, lastTimeStrip, stripCount, totalADC,
					centerOfTime, coordinate);
		}
		clusterCount++;
	}
	return clusterCount;
}

