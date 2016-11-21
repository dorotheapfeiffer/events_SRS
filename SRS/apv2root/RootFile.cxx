#include "RootFile.h"
#include <TROOT.h>
#include <TStyle.h>
#include "TMath.h"

RootFile::RootFile(TString fileName, TString pedestalName,
		bool isRawPedestalRun, bool isPedestalRun, bool isZSRun, bool isUTPC,int uTPCThreshold) :
		isRawPedestalRun(isRawPedestalRun), isPedestalRun(isPedestalRun), isZSRun(
				isZSRun), isUTPCRun(isUTPC), amplitudeThreshold(uTPCThreshold), fFileName(fileName), fPedestalName(
				pedestalName), rawPedestalNoise(
		NFEC * NAPV, 0), rawPedestalOffset(NFEC * NAPV, 0), pedestalNoise(
		NFEC * NAPV, 0), pedestalOffset(NFEC * NAPV, 0), chipData(
		NFEC * NAPV, std::vector<TH1F*>(NCH, 0))
{

	if (!isZSRun && !isRawPedestalRun)
	{
		fFilePedestal = new TFile(fPedestalName);
		std::cout << "Pedestal root file " << fPedestalName << " opened!"
				<< std::endl;
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

void RootFile::CreateHistograms(int minFecID, int maxFecID, int minAPVID,
		int maxAPVID)
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
void RootFile::AddHits(signed int timestamp, int us, int eventId, int fecID,
		int apvID, int chNo, float maxADC, int timeBinMaxADC,
		std::vector<float> &timeBinADCs)
{
	m_timestamp = timestamp;
	m_us = us;
	m_evtID = eventId;
	m_fecID[m_chID] = fecID;
	m_apvID[m_chID] = apvID;

	int stripNo = chNo;
	//std::cout << stripNo << std::endl;
	m_strip_chip[m_chID] = stripNo;
	//stripNo = 127 - stripNo;
	if (apvID == 0)
	{
		m_strip[m_chID] = stripNo;
		m_planeID[m_chID] = 0;
	}
	else if (apvID == 1)
	{
		m_strip[m_chID] = stripNo + 128;
		m_planeID[m_chID] = 0;
	}
	else if (apvID == 2)
	{
		m_strip[m_chID] = stripNo;
		m_planeID[m_chID] = 1;
	}
	else if (apvID == 3)
	{
		m_strip[m_chID] = stripNo + 128;
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
	if (isUTPCRun)
	{
		int pStrip = m_strip[m_chID];
		int numMaximaStripX = 0;
		int numMaximaStripY = 0;

		if (m_planeID[m_chID] == 0)
		{
			float amp0 = 0;
			float amp1 = 0;
			float amp2 = 0;
			int timeBin1 = 0;

			float pAmplitude = 0;
			float pHalf = 0;
			float pPeak = 0;
			float pIntegral = 0;
			for (int timeBin = 29; timeBin >= 0; timeBin--)
			{
				if (timeBinADCs[timeBin] > amplitudeThreshold)
				{
					if (txd < timeBin
							|| (txd == timeBin && timeBinADCs[timeBin] < exd))
					{

						xd = pStrip;
						txd = timeBin;
						exd = timeBinADCs[timeBin];

					}
					break;
				}
			}
			for (int timeBin = 0; timeBin < 30; timeBin++)
			{
				amp2 = amp1;
				amp1 = amp0;
				amp0 = timeBinADCs[timeBin];
				if (amp2 <= amp1 && amp0 < amp1 && amp1 > amplitudeThreshold
						&& abs(timeBin1 - timeBin) > 2)
				{
					stripMaximaX.push_back(pStrip);
					timeMaximaX.push_back(timeBin - 1);
					energyMaximaX.push_back(amp1);
					timeBin1 = timeBin - 1;
					if (amp0 > 1400)
					{
						numMaximaStripX++;
					}
				}
				else if (timeBin == 29 && amp0 > amp1 && amp1 > amp2
						&& amp0 > amplitudeThreshold && abs(timeBin1 - timeBin) > 2)
				{
					stripMaximaX.push_back(pStrip);
					timeMaximaX.push_back(timeBin);
					energyMaximaX.push_back(amp0);
					timeBin1 = timeBin;
					if (amp0 > 1400)
					{
						numMaximaStripX++;
					}
				}

				if (pAmplitude < timeBinADCs[timeBin]
						&& amplitudeThreshold < timeBinADCs[timeBin])
				{
					pAmplitude = timeBinADCs[timeBin];
					pPeak = timeBin;
					if (pAmplitude > maxAmplitudeX)
					{
						maxAmplitudeX = pAmplitude;
						tMaxAmplitudeX = timeBin;
					}

				}
				pIntegral += timeBinADCs[timeBin];
			}
			if (numMaximaStripX >= 4)
			{
				discardFlag = 1;
			}
			int timeBin;
			for (timeBin = 0; timeBin < 30; timeBin++)
			{
				if (timeBinADCs[timeBin] > pAmplitude / 2.)
					break;
			}
			if (timeBin > 0 && timeBin < 29)
			{
				pHalf = (timeBin - 1)
						+ (pAmplitude / 2. - timeBinADCs[timeBin - 1])
								/ (timeBinADCs[timeBin]
										- timeBinADCs[timeBin - 1]);
			}
			else
			{
				pHalf = -.5;
			}

			if ((pHalf > tx0 || tx0 < 0.)
					|| (pHalf == tx0 && ex0 > timeBinADCs[timeBin]))
			{
				tx0 = pHalf;
				x0 = pStrip;
				ex0 = timeBinADCs[timeBin];
			}

			if ((pHalf < txf || txf < 0.)
					|| (pHalf == txf && exf > timeBinADCs[timeBin]))
			{
				txf = pHalf;
				xf = pStrip;
				exf = timeBinADCs[timeBin];
			}

			if ((txmax < pPeak || txmax < 0.)
					|| (pPeak == txmax && exmax > pAmplitude))
			{
				txmax = pPeak;
				xmax = pStrip;
				exmax = pAmplitude;
			}

			xm += pAmplitude * (double) pStrip;
			xi += pIntegral * (double) pStrip;
			xAmplitude += pAmplitude;
			xIntegral += pIntegral;
			nx++;

		}
		else
		{
			float amp0 = 0;
			float amp1 = 0;
			float amp2 = 0;
			int timeBin1 = 0;

			float pAmplitude = 0;
			float pHalf = 0;
			float pPeak = 0;
			float pIntegral = 0;

			for (int timeBin = 29; timeBin >= 0; timeBin--)
			{
				if (timeBinADCs[timeBin] > amplitudeThreshold)
				{
					if (tyd < timeBin
							|| (tyd == timeBin && timeBinADCs[timeBin] < eyd))
					{

						yd = pStrip;
						tyd = timeBin;
						eyd = timeBinADCs[timeBin];

					}
					break;
				}
			}
			for (int timeBin = 0; timeBin < 30; timeBin++)
			{
				amp2 = amp1;
				amp1 = amp0;
				amp0 = timeBinADCs[timeBin];
				if (amp2 <= amp1 && amp0 < amp1 && amp1 > amplitudeThreshold
						&& abs(timeBin1 - timeBin) > 2)
				{
					stripMaximaY.push_back(pStrip);
					timeMaximaY.push_back(timeBin - 1);
					energyMaximaY.push_back(amp1);
					timeBin1 = timeBin - 1;
					if (amp0 > 1400)
					{
						numMaximaStripY++;
					}
				}
				else if (timeBin == 29 && amp0 > amp1 && amp1 > amp2
						&& amp0 > amplitudeThreshold && abs(timeBin1 - timeBin) > 2)
				{
					stripMaximaY.push_back(stripNo);
					timeMaximaY.push_back(pStrip);
					energyMaximaY.push_back(amp0);
					timeBin1 = timeBin;
					if (amp0 > 1400)
					{
						numMaximaStripY++;
					}
				}

				if (pAmplitude < timeBinADCs[timeBin]
						&& amplitudeThreshold < timeBinADCs[timeBin])
				{
					pAmplitude = timeBinADCs[timeBin];
					pPeak = timeBin;
					if (pAmplitude > maxAmplitudeY)
					{
						maxAmplitudeY = pAmplitude;
						tMaxAmplitudeY = timeBin;
					}

				}
				pIntegral += timeBinADCs[timeBin];
			}
			if (numMaximaStripX >= 4)
			{
				discardFlag = true;
			}
			int timeBin;
			for (timeBin = 0; timeBin < 30; timeBin++)
			{
				if (timeBinADCs[timeBin] > pAmplitude / 2.)
					break;
			}
			if (timeBin > 0 && timeBin < 29)
			{
				pHalf = (timeBin - 1)
						+ (pAmplitude / 2. - timeBinADCs[timeBin - 1])
								/ (timeBinADCs[timeBin]
										- timeBinADCs[timeBin - 1]);
			}
			else
			{
				pHalf = -.5;
			}

			if ((pHalf > ty0 || ty0 < 0.)
					|| (pHalf == ty0 && ey0 > timeBinADCs[timeBin]))
			{
				ty0 = pHalf;
				y0 = pStrip;
				ey0 = timeBinADCs[timeBin];
			}

			if ((pHalf < tyf || tyf < 0.)
					|| (pHalf == tyf && eyf > timeBinADCs[timeBin]))
			{
				tyf = pHalf;
				yf = pStrip;
				eyf = timeBinADCs[timeBin];
			}

			if ((tymax < pPeak || tymax < 0.)
					|| (pPeak == tymax && eymax > pAmplitude))
			{
				tymax = pPeak;
				ymax = pStrip;
				eymax = pAmplitude;
			}

			ym += pAmplitude * (double) pStrip;
			yi += pIntegral * (double) pStrip;
			yAmplitude += pAmplitude;
			yIntegral += pIntegral;
			ny++;

		}

	}

	m_chID++;

}

void RootFile::FillHits()
{
	if (!isPedestalRun && !isRawPedestalRun)
	{
		ntx = TMath::Abs(tx0 - txf) + 1;
		nty = TMath::Abs(ty0 - tyf) + 1;
		if (xAmplitude > 0)
		{
			xm /= xAmplitude;
		}
		if (xIntegral > 0)
		{
			xi /= xIntegral;
		}
		if (yAmplitude > 0)
		{
			ym /= yAmplitude;
		}
		if (yIntegral > 0)
		{
			yi /= yIntegral;
		}
		numMaximaX = timeMaximaX.size();
		numMaximaY = timeMaximaY.size();
		if (numMaximaX > 0 && numMaximaY > 0 && numMaximaX < DISCARD_THRESHOLD
				&& numMaximaY < DISCARD_THRESHOLD && discardFlag == false)
		{
			for (int row = 0; row < numMaximaX; row++)
			{
				if ((txs < timeMaximaX[row])
						|| (txs == timeMaximaX[row] && exs > energyMaximaX[row]))
				{
					txs = timeMaximaX[row];

					xs = stripMaximaX[row];
					exs = energyMaximaX[row];
				}
			}

			for (int row = 0; row < timeMaximaY.size(); row++)
			{

				if ((tys < timeMaximaY[row])
						|| (tys == timeMaximaY[row] && eys > energyMaximaY[row]))
				{
					tys = timeMaximaY[row];

					ys = stripMaximaY[row];
					eys = energyMaximaY[row];
				}

			}
		}
		fHitTree->Fill();
		resetUTPCData();
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
		fHitTree = new TTree("events", "GEM events ROOT file");
		fHitTree->SetDirectory(fFile);
		m_fecID = new int[10000];
		m_apvID = new int[10000];
		m_strip = new int[10000];
		m_strip_chip = new int[10000];
		m_planeID = new int[10000];
		m_hitTimeBin = new int[10000];
		m_hitMaxADC = new short[10000];
		m_adc0 = new short[10000];
		m_adc1 = new short[10000];
		m_adc2 = new short[10000];
		m_adc3 = new short[10000];
		m_adc4 = new short[10000];
		m_adc5 = new short[10000];
		m_adc6 = new short[10000];
		m_adc7 = new short[10000];
		m_adc8 = new short[10000];
		m_adc9 = new short[10000];
		m_adc10 = new short[10000];
		m_adc11 = new short[10000];
		m_adc12 = new short[10000];
		m_adc13 = new short[10000];
		m_adc14 = new short[10000];
		m_adc15 = new short[10000];
		m_adc16 = new short[10000];
		m_adc17 = new short[10000];
		m_adc18 = new short[10000];
		m_adc19 = new short[10000];
		m_adc20 = new short[10000];
		m_adc21 = new short[10000];
		m_adc22 = new short[10000];
		m_adc23 = new short[10000];
		m_adc24 = new short[10000];
		m_adc25 = new short[10000];
		m_adc26 = new short[10000];
		m_adc27 = new short[10000];
		m_adc28 = new short[10000];
		m_adc29 = new short[10000];

		fHitTree->Branch("timestamp", &m_timestamp, "timestamp/I");
		fHitTree->Branch("us", &m_us, "us/I");
		fHitTree->Branch("evtID", &m_evtID, "evtID/I");
		fHitTree->Branch("nch", &m_chID, "nch/I");
		fHitTree->Branch("fecID", m_fecID, "fecID[nch]/I");
		fHitTree->Branch("apvID", m_apvID, "apvID[nch]/I");
		fHitTree->Branch("planeID", m_planeID, "planeID[nch]/I");
		fHitTree->Branch("strip_chip", m_strip_chip, "strip_chip[nch]/I");
		fHitTree->Branch("strip", m_strip, "strip[nch]/I");

		fHitTree->Branch("hitTimebin", m_hitTimeBin, "hitTimeBin[nch]/I");
		fHitTree->Branch("hitMaxADC", m_hitMaxADC, "hitMaxADC[nch]/S");
		fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
		fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
		fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
		fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
		fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
		fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
		fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
		fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
		fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
		fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
		fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
		fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
		fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
		fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
		fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
		fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
		fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
		fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
		fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
		fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
		fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
		fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
		fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
		fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
		fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
		fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
		fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
		fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
		fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
		fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");
		//std::cout << "Hit tree created!" << std::endl;
		if (isUTPCRun)
		{
			resetUTPCData();

			fHitTree->Branch("nx", &nx, "nx/I");
			fHitTree->Branch("ny", &ny, "ny/I");
			fHitTree->Branch("ntx", &ntx, "ntx/I");
			fHitTree->Branch("nty", &nty, "nty/I");
			fHitTree->Branch("numMaximaX", &numMaximaX, "numMaximaX/I");
			fHitTree->Branch("numMaximaY", &numMaximaY, "numMaximaY/I");
			fHitTree->Branch("maxAmplitudeX", &maxAmplitudeX,
					"maxAmplitudeX/D");
			fHitTree->Branch("maxAmplitudeY", &maxAmplitudeY,
					"maxAmplitudeY/D");
			fHitTree->Branch("tMaxAmplitudeX", &tMaxAmplitudeX,
					"tMaxAmplitudeX/D");
			fHitTree->Branch("tMaxAmplitudeY", &tMaxAmplitudeY,
					"tMaxAmplitudeY/D");
			fHitTree->Branch("xAmplitude", &xAmplitude, "xAmplitude/D");
			fHitTree->Branch("yAmplitude", &yAmplitude, "yAmplitude/D");
			fHitTree->Branch("xIntegral", &xIntegral, "xIntegral/D");
			fHitTree->Branch("yIntegral", &yIntegral, "yIntegral/D");

			fHitTree->Branch("xm", &xm, "xm/D");
			fHitTree->Branch("ym", &ym, "ym/D");
			fHitTree->Branch("xi", &xi, "xi/D");
			fHitTree->Branch("yi", &yi, "yi/D");

			fHitTree->Branch("x0", &x0, "x0/D");
			fHitTree->Branch("tx0", &tx0, "tx0/D");
			fHitTree->Branch("y0", &y0, "y0/D");
			fHitTree->Branch("ty0", &ty0, "ty0/D");
			fHitTree->Branch("xf", &xf, "xf/D");
			fHitTree->Branch("txf", &txf, "txf/D");
			fHitTree->Branch("yf", &yf, "yf/D");
			fHitTree->Branch("tyf", &tyf, "tyf/D");

			fHitTree->Branch("xs", &xs, "xs/D");
			fHitTree->Branch("txs", &txs, "txs/D");
			fHitTree->Branch("ys", &ys, "ys/D");
			fHitTree->Branch("tys", &tys, "tys/D");

			fHitTree->Branch("xd", &xd, "xd/D");
			fHitTree->Branch("txd", &txd, "txd/D");
			fHitTree->Branch("yd", &yd, "yd/D");
			fHitTree->Branch("tyd", &tyd, "tyd/D");

			fHitTree->Branch("xmax", &xmax, "xmax/D");
			fHitTree->Branch("txmax", &txmax, "txmax/D");
			fHitTree->Branch("ymax", &ymax, "ymax/D");
			fHitTree->Branch("tymax", &tymax, "tymax/D");

		}
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

void RootFile::resetUTPCData()
{
	discardFlag = false;
	x0 = y0 = xf = yf = xm = ym = xi = yi = xs = ys = xd = yd = xmax = ymax = 0;

	xAmplitude = xIntegral = yAmplitude = yIntegral = 0.;

	nx = ny = 0;
	tx0 = ty0 = txf = tyf = txs = tys = txd = tyd = txmax = tymax = -1.;
	ex0 = ey0 = exf = eyf = exs = eys = exd = eyd = exmax = eymax = 9999999;
	nx = ny = 0;
	ntx = nty = 0;

	numMaximaX = 0;
	numMaximaY = 0;

	maxAmplitudeX = maxAmplitudeY = tMaxAmplitudeX = tMaxAmplitudeY = 0;
	stripMaximaX.clear();
	stripMaximaY.clear();
	timeMaximaX.clear();
	timeMaximaY.clear();
	energyMaximaX.clear();
	energyMaximaY.clear();
}
