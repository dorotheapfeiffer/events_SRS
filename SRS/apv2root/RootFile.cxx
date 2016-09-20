#include "RootFile.h"
#include <TROOT.h>
#include <TStyle.h>
#include "TMath.h"

RootFile::RootFile(TString fileName, TString pedestalName,
		bool isRawPedestalRun, bool isPedestalRun, bool isZSRun) :
		isRawPedestalRun(isRawPedestalRun), isPedestalRun(isPedestalRun), isZSRun(
				isZSRun), fFileName(fileName),fPedestalName(pedestalName), rawPedestalNoise(
		NFEC * NAPV,  0), rawPedestalOffset(NFEC * NAPV,  0), pedestalNoise(
		NFEC * NAPV,  0), pedestalOffset(NFEC * NAPV, 0), chipData(
		NFEC * NAPV, std::vector<TH1F*>(NCH,  0))
{

	if (!isZSRun && !isRawPedestalRun)
	{
		fFilePedestal = new TFile(fPedestalName);
		std::cout << "Pedestal root file " << fPedestalName << " opened!" << std::endl;
	}

	InitRootFile();

}

//====================================================================================================================
RootFile::~RootFile()
{

	fFile->Close();

	if (!isRawPedestalRun && !isPedestalRun)
	{
		DeleteHitsTree();
	}
}

void RootFile::SetRunFlags(bool isRawPedestal, bool isPedestal)
{

	isRawPedestalRun = isRawPedestal;
	isPedestalRun = isPedestal;
}

void RootFile::InitPedestalData(int fecID, int apvID)
{

	if (isRawPedestalRun || (!isRawPedestalRun && !isPedestalRun && !isZSRun))
	{
		float min = -2048;
		float max = 2048;
		float nbin = 4097;

		for (int stripNo = 0; stripNo < NCH; stripNo++)
		{
			int theID = (fecID - 1) * NAPV + apvID;
			std::stringstream out;
			out << theID << " " << stripNo;
			TString idStr = out.str();
			chipData[theID][stripNo] = new TH1F(idStr, idStr, nbin, min, max);
			if (stripData.size() < NCH)
			{
				stripData.push_back(new std::vector<float>());
			}
		}
	}
	else if (isPedestalRun)
	{
		for (int stripNo = 0; stripNo < NCH; stripNo++)
		{
			int theID = (fecID - 1) * NAPV + apvID;
			chipData[theID][stripNo]->Reset();
		}
	}
}

void RootFile::FillStripData(int stripNo, float rawdata)
{

	//if(isPedestalRun)
	//std::cout << stripNo << " " << rawdata << std::endl;
	stripData[stripNo]->push_back(rawdata);

}

bool RootFile::StripDataExists(int stripNo)
{
	if (stripData[stripNo]->empty())
	{
		return false;
	}
	else
	{
		return true;
	}

}

float RootFile::GetStripData(int stripNo, int timeBin)
{
	return stripData[stripNo]->at(timeBin);
}

float RootFile::GetMeanStripData(int stripNo)
{
	float mean = TMath::Mean(stripData[stripNo]->begin(),
			stripData[stripNo]->end());
	stripData[stripNo]->clear();
	return mean;
}

void RootFile::ClearStripData(int stripNo)
{
	stripData[stripNo]->clear();
}

void RootFile::InitPedestalHistograms(int fecID, int apvID)
{
	int theID = (fecID - 1) * NAPV + apvID;

	std::stringstream outFecID;
	outFecID << fecID;
	TString fecIDStr = outFecID.str();

	std::stringstream outApvID;
	outApvID << apvID;
	TString apvIDStr = outApvID.str();

	if (isRawPedestalRun)
	{

		float min = -0.5;
		float max = 127.5;
		float nbin = 128;

		TString pedName = "fecID_" + fecIDStr + "_apvID_" + apvIDStr
				+ "_rawNoise";
		rawPedestalNoise[theID] = new TH1F(pedName, pedName, nbin, min, max);
		pedName = "fecID_" + fecIDStr + "_apvID_" + apvIDStr + "_rawOffset";
		rawPedestalOffset[theID] = new TH1F(pedName, pedName, nbin, min, max);

	}
	else if (isPedestalRun)
	{
		float min = -0.5;
		float max = 127.5;
		float nbin = 128;

		TString pedName = "fecID_" + fecIDStr + "_apvID_" + apvIDStr + "_noise";
		pedestalNoise[theID] = new TH1F(pedName, pedName, nbin, min, max);
		pedName = "fecID_" + fecIDStr + "_apvID_" + apvIDStr + "_offset";
		pedestalOffset[theID] = new TH1F(pedName, pedName, nbin, min, max);
	}
	else if (!isZSRun)
	{
		float min = -0.5;
		float max = 127.5;
		float nbin = 128;

		TString pedName = "fecID_" + fecIDStr + "_apvID_" + apvIDStr + "_noise";
		pedestalNoise[theID] = new TH1F(pedName, pedName, nbin, min, max);
		pedestalNoise[theID] = (TH1F*) fFilePedestal->Get(pedName);
		pedName = "fecID_" + fecIDStr + "_apvID_" + apvIDStr + "_offset";
		pedestalOffset[theID] = new TH1F(pedName, pedName, nbin, min, max);
		pedestalOffset[theID] = (TH1F*) fFilePedestal->Get(pedName);


	}

}

void RootFile::FillPedestalData(int fecID, int apvID, int stripNo, float mean)
{
	int theID = (fecID - 1) * NAPV + apvID;
	chipData[theID][stripNo]->Fill(mean);

}

void RootFile::CreateHistograms(int minFecID, int maxFecID,
		int minAPVID, int maxAPVID)
{
	if (isRawPedestalRun)
	{
		for (int fecID = minFecID; fecID <= maxFecID; fecID++)
		{
			for (int apvID = minAPVID; apvID <= maxAPVID; apvID++)
			{
				int theID = (fecID - 1) * NAPV + apvID;
				for (int stripNo = 0; stripNo < NCH; stripNo++)
				{
					//std::cout << theID << std::endl;
					float offset = chipData[theID][stripNo]->GetMean();
					float noise = chipData[theID][stripNo]->GetRMS();
					rawPedestalNoise[theID]->Fill(stripNo, noise);
					rawPedestalOffset[theID]->Fill(stripNo, offset);


				}

			}
		}
	}
	else if (isPedestalRun)
	{

		for (int fecID = minFecID; fecID <= maxFecID; fecID++)
		{
			for (int apvID = minAPVID; apvID <= maxAPVID; apvID++)
			{
				int theID = (fecID - 1) * NAPV + apvID;

				for (int stripNo = 0; stripNo < NCH; stripNo++)
				{
					float offset = chipData[theID][stripNo]->GetMean();

					float noise = chipData[theID][stripNo]->GetRMS();
					pedestalNoise[theID]->Fill(stripNo, noise);
					pedestalOffset[theID]->Fill(stripNo, offset);


				}

			}
		}
	}
}

float RootFile::GetStripPedestalOffset(int fecID, int apvID, int stripNr)
{
	int theID = (fecID - 1) * NAPV + apvID;
	return pedestalOffset[theID]->GetBinContent(stripNr + 1);
}

float RootFile::GetStripPedestalNoise(int fecID, int apvID, int stripNr)
{
	int theID = (fecID - 1) * NAPV + apvID;
	return pedestalNoise[theID]->GetBinContent(stripNr + 1);
}

float RootFile::GetStripRawPedestalOffset(int fecID, int apvID, int stripNr)
{
	int theID = (fecID - 1) * NAPV + apvID;
	return rawPedestalOffset[theID]->GetBinContent(stripNr + 1);
}

float RootFile::GetStripRawPedestalNoise(int fecID, int apvID, int stripNr)
{
	int theID = (fecID - 1) * NAPV + apvID;
	return rawPedestalNoise[theID]->GetBinContent(stripNr + 1);
}

float RootFile::GetMeanPedestalOffset(int fecID, int apvID)
{
	int theID = (fecID - 1) * NAPV + apvID;
	return pedestalOffset[theID]->GetMean();
}
//====================================================================================================================
void RootFile::DeleteHitsTree()
{
	if (m_adc0)
		delete[] m_adc0;

	if (m_adc1)
		delete[] m_adc1;
	if (m_adc2)
		delete[] m_adc2;
	if (m_adc3)
		delete[] m_adc3;
	if (m_adc4)
		delete[] m_adc4;
	if (m_adc5)
		delete[] m_adc5;
	if (m_adc6)
		delete[] m_adc6;
	if (m_adc7)
		delete[] m_adc7;
	if (m_adc8)
		delete[] m_adc8;
	if (m_adc9)
		delete[] m_adc9;
	if (m_adc10)
		delete[] m_adc10;
	if (m_adc11)
		delete[] m_adc11;
	if (m_adc12)
		delete[] m_adc12;
	if (m_adc13)
		delete[] m_adc13;
	if (m_adc14)
		delete[] m_adc14;
	if (m_adc15)
		delete[] m_adc15;
	if (m_adc16)
		delete[] m_adc16;
	if (m_adc17)
		delete[] m_adc17;
	if (m_adc18)
		delete[] m_adc18;
	if (m_adc19)
		delete[] m_adc19;
	if (m_adc20)
		delete[] m_adc20;
	if (m_adc21)
		delete[] m_adc21;
	if (m_adc22)
		delete[] m_adc22;
	if (m_adc23)
		delete[] m_adc23;
	if (m_adc24)
		delete[] m_adc24;
	if (m_adc25)
		delete[] m_adc25;
	if (m_adc26)
		delete[] m_adc26;
	if (m_adc27)
		delete[] m_adc27;
	if (m_adc28)
		delete[] m_adc28;
	if (m_adc29)
		delete[] m_adc29;

	if (m_hitTimeBin)
		delete[] m_hitTimeBin;
	if (m_hitMaxADC)
		delete[] m_hitMaxADC;
	if (m_strip)
		delete[] m_strip;
	if (m_strip_chip)
		delete[] m_strip_chip;

	if (m_planeID)
		delete[] m_planeID;
	if (m_apvID)
		delete[] m_apvID;
	if (m_fecID)
		delete[] m_fecID;
}

//====================================================================================================================
void RootFile::AddHits(signed int timestamp, unsigned int us, unsigned int eventId,  unsigned int fecID,
		unsigned int apvID, unsigned int chNo, float maxADC,
		unsigned int timeBinMaxADC, std::vector<float> &timeBinADCs)
{
	m_timestamp = timestamp;
	m_us = us;
	m_evtID = eventId;
	m_fecID[m_chID] = fecID;
	m_apvID[m_chID] = apvID;

	unsigned int stripNo = chNo;
	//std::cout << stripNo << std::endl;
	m_strip_chip[m_chID] = stripNo;
	stripNo = 127 - stripNo;
	if (apvID == 0)
	{
		m_strip[m_chID] = stripNo;
		m_planeID[m_chID] = 0;
	}
	else if (apvID == 1)
	{
		m_strip[m_chID] = 128 + stripNo;
		m_planeID[m_chID] = 0;
	}
	else if (apvID == 2)
	{
		m_strip[m_chID] = stripNo;
		m_planeID[m_chID] = 1;
	}
	else if (apvID == 3)
	{
		m_strip[m_chID] = 128 + stripNo;
		m_planeID[m_chID] = 1;
	}

	m_hitTimeBin[m_chID] = timeBinMaxADC;
	m_hitMaxADC[m_chID] = maxADC;
	m_adc0[m_chID] = timeBinADCs[0];
	m_adc1[m_chID] = timeBinADCs[1];
	m_adc2[m_chID] = timeBinADCs[2];
	m_adc3[m_chID] = timeBinADCs[3];
	m_adc4[m_chID] = timeBinADCs[4];
	m_adc5[m_chID] = timeBinADCs[5];
	m_adc6[m_chID] = timeBinADCs[6];
	m_adc7[m_chID] = timeBinADCs[7];
	m_adc8[m_chID] = timeBinADCs[8];
	m_adc9[m_chID] = timeBinADCs[9];
	m_adc10[m_chID] = timeBinADCs[10];
	m_adc11[m_chID] = timeBinADCs[11];
	m_adc12[m_chID] = timeBinADCs[12];
	m_adc13[m_chID] = timeBinADCs[13];
	m_adc14[m_chID] = timeBinADCs[14];
	m_adc15[m_chID] = timeBinADCs[15];
	m_adc16[m_chID] = timeBinADCs[16];
	m_adc17[m_chID] = timeBinADCs[17];
	m_adc18[m_chID] = timeBinADCs[18];
	m_adc19[m_chID] = timeBinADCs[19];
	m_adc20[m_chID] = timeBinADCs[20];
	m_adc21[m_chID] = timeBinADCs[21];
	m_adc22[m_chID] = timeBinADCs[22];
	m_adc23[m_chID] = timeBinADCs[23];
	m_adc24[m_chID] = timeBinADCs[24];
	m_adc25[m_chID] = timeBinADCs[25];
	m_adc26[m_chID] = timeBinADCs[26];
	m_adc27[m_chID] = timeBinADCs[27];
	m_adc28[m_chID] = timeBinADCs[28];
	m_adc29[m_chID] = timeBinADCs[29];
	m_chID++;

}

void RootFile::FillHits()
{
	if (!isPedestalRun && !isRawPedestalRun)
	{
		fHitTree->Fill();
		m_chID = 0;
	}
}

//====================================================================================================================
void RootFile::InitRootFile()
{
	m_evtID = 0;
	m_chID = 0;

	fFile = TFile::Open(fFileName, "RECREATE");
	std::cout << "Root file " << fFileName << " created!" << std::endl;

	if (!isPedestalRun && !isRawPedestalRun)
	{
		fHitTree = new TTree("THit", "GEM Hit Data Rootfile");
		fHitTree->SetDirectory(fFile);
		m_fecID = new unsigned int[10000];
		m_apvID = new unsigned int[10000];
		m_strip = new unsigned int[10000];
		m_strip_chip = new unsigned int[10000];
		m_planeID = new unsigned int[10000];
		m_hitTimeBin = new unsigned int[10000];
		m_hitMaxADC = new float[10000];
		m_adc0 = new float[10000];
		m_adc1 = new float[10000];
		m_adc2 = new float[10000];
		m_adc3 = new float[10000];
		m_adc4 = new float[10000];
		m_adc5 = new float[10000];
		m_adc6 = new float[10000];
		m_adc7 = new float[10000];
		m_adc8 = new float[10000];
		m_adc9 = new float[10000];
		m_adc10 = new float[10000];
		m_adc11 = new float[10000];
		m_adc12 = new float[10000];
		m_adc13 = new float[10000];
		m_adc14 = new float[10000];
		m_adc15 = new float[10000];
		m_adc16 = new float[10000];
		m_adc17 = new float[10000];
		m_adc18 = new float[10000];
		m_adc19 = new float[10000];
		m_adc20 = new float[10000];
		m_adc21 = new float[10000];
		m_adc22 = new float[10000];
		m_adc23 = new float[10000];
		m_adc24 = new float[10000];
		m_adc25 = new float[10000];
		m_adc26 = new float[10000];
		m_adc27 = new float[10000];
		m_adc28 = new float[10000];
		m_adc29 = new float[10000];

		fHitTree->Branch("timestamp", &m_timestamp, "timestamp/I");
		fHitTree->Branch("us", &m_us, "us/i");
		fHitTree->Branch("evtID", &m_evtID, "evtID/i");
		fHitTree->Branch("nch", &m_chID, "nch/i");
		fHitTree->Branch("fecID", m_fecID, "fecID[nch]/i");
		fHitTree->Branch("apvID", m_apvID, "apvID[nch]/i");
		fHitTree->Branch("planeID", m_planeID, "planeID[nch]/i");
		fHitTree->Branch("strip_chip", m_strip_chip, "strip_chip[nch]/i");
		fHitTree->Branch("strip", m_strip, "strip[nch]/i");

		fHitTree->Branch("hitTimebin", m_hitTimeBin, "hitTimeBin[nch]/i");
		fHitTree->Branch("hitMaxADC", m_hitMaxADC, "hitMaxADC[nch]/F");
		fHitTree->Branch("adc0", m_adc0, "adc0[nch]/F");
		fHitTree->Branch("adc1", m_adc1, "adc1[nch]/F");
		fHitTree->Branch("adc2", m_adc2, "adc2[nch]/F");
		fHitTree->Branch("adc3", m_adc3, "adc3[nch]/F");
		fHitTree->Branch("adc4", m_adc4, "adc4[nch]/F");
		fHitTree->Branch("adc5", m_adc5, "adc5[nch]/F");
		fHitTree->Branch("adc6", m_adc6, "adc6[nch]/F");
		fHitTree->Branch("adc7", m_adc7, "adc7[nch]/F");
		fHitTree->Branch("adc8", m_adc8, "adc8[nch]/F");
		fHitTree->Branch("adc9", m_adc9, "adc9[nch]/F");
		fHitTree->Branch("adc10", m_adc10, "adc10[nch]/F");
		fHitTree->Branch("adc11", m_adc11, "adc11[nch]/F");
		fHitTree->Branch("adc12", m_adc12, "adc12[nch]/F");
		fHitTree->Branch("adc13", m_adc13, "adc13[nch]/F");
		fHitTree->Branch("adc14", m_adc14, "adc14[nch]/F");
		fHitTree->Branch("adc15", m_adc15, "adc15[nch]/F");
		fHitTree->Branch("adc16", m_adc16, "adc16[nch]/F");
		fHitTree->Branch("adc17", m_adc17, "adc17[nch]/F");
		fHitTree->Branch("adc18", m_adc18, "adc18[nch]/F");
		fHitTree->Branch("adc19", m_adc19, "adc19[nch]/F");
		fHitTree->Branch("adc20", m_adc20, "adc20[nch]/F");
		fHitTree->Branch("adc21", m_adc21, "adc21[nch]/F");
		fHitTree->Branch("adc22", m_adc22, "adc22[nch]/F");
		fHitTree->Branch("adc23", m_adc23, "adc23[nch]/F");
		fHitTree->Branch("adc24", m_adc24, "adc24[nch]/F");
		fHitTree->Branch("adc25", m_adc25, "adc25[nch]/F");
		fHitTree->Branch("adc26", m_adc26, "adc26[nch]/F");
		fHitTree->Branch("adc27", m_adc27, "adc27[nch]/F");
		fHitTree->Branch("adc28", m_adc28, "adc28[nch]/F");
		fHitTree->Branch("adc29", m_adc29, "adc29[nch]/F");
		//std::cout << "Hit tree created!" << std::endl;
	}

}

//====================================================================================================================
void RootFile::WriteRootFile()
{

	if (isRawPedestalRun)
	{

		int i = 0;
		std::vector<TH1F*>::const_iterator itr;
		for (itr = rawPedestalNoise.begin(); itr != rawPedestalNoise.end();
				++itr)
		{
			if (rawPedestalNoise[i])
			{
				rawPedestalNoise[i]->SetOption("HIST");
				rawPedestalOffset[i]->SetOption("HIST");
				rawPedestalNoise[i]->Write(0, TObject::kWriteDelete);
				rawPedestalOffset[i]->Write(0, TObject::kWriteDelete);
			}
			i++;
		}
	}
	else if (isPedestalRun)
	{

		int i = 0;
		std::vector<TH1F*>::const_iterator itr;

		for (itr = rawPedestalNoise.begin(); itr != rawPedestalNoise.end();
				++itr)
		{
			if (rawPedestalNoise[i])
			{
				rawPedestalNoise[i]->SetOption("HIST");
				rawPedestalOffset[i]->SetOption("HIST");
				rawPedestalNoise[i]->Write(0, TObject::kWriteDelete);
				rawPedestalOffset[i]->Write(0, TObject::kWriteDelete);
			}

			i++;
		}
		i = 0;

		for (itr = pedestalNoise.begin(); itr != pedestalNoise.end(); ++itr)
		{

			if (pedestalNoise[i])
			{
				pedestalNoise[i]->SetOption("HIST");
				pedestalOffset[i]->SetOption("HIST");
				pedestalNoise[i]->Write(0, TObject::kWriteDelete);
				pedestalOffset[i]->Write(0, TObject::kWriteDelete);
			}
			i++;
		}
	}
	else
	{
		fHitTree->Write(0, TObject::kWriteDelete);
	}
	//fFile->Write(0, TObject::kWriteDelete);
//fFile->Write();
}

int RootFile::GetStripNumber(int chNo)
{
	chNo = (32 * (chNo % 4)) + (8 * (int) (chNo / 4))
			- (31 * (int) (chNo / 16));

	return chNo;
}
