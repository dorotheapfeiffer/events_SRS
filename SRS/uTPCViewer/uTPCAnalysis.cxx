#include "uTPCAnalysis.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include "uTPCViewer.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TMath.h"
#include "TF1.h"

uTPCPlotData::uTPCPlotData()
{
	fThresholdAmplitude = 0;
	fThresholdDiscard = 0;
}
;

uTPCPlotData::~uTPCPlotData()
{
	clearDataStructures();
	fThresholdAmplitude = 0;
	fThresholdDiscard = 0;
}
;

void uTPCPlotData::clearDataStructures()
{
	this->fPointClustersX.clear();
	this->fPointClustersY.clear();
	this->fEnergyX.clear();
	this->fEnergyY.clear();
	this->fEnergyMaximaX.clear();
	this->fEnergyMaximaY.clear();
	this->fStripMaximaX.clear();
	this->fStripMaximaY.clear();
	this->fTimeMaximaX.clear();
	this->fTimeMaximaY.clear();
	this->fXEigenvectorX.clear();
	this->fXEigenvectorY.clear();
	this->fXNormalX.clear();
	this->fXNormalY.clear();
	this->fYEigenvectorX.clear();
	this->fYEigenvectorY.clear();
	this->fYNormalX.clear();
	this->fYNormalY.clear();
}

uTPCRawData::uTPCRawData()
{
	fileIn = 0;
	treeIn = 0;
	event = 0;

}

uTPCRawData::~uTPCRawData()
{

	if (event != 0)
	{
		delete event;
		event = 0;
	}

	treeIn = 0;

	if (fileIn != 0)
	{
		fileIn->Close();
		fileIn = 0;
	}

}

int uTPCRawData::getNumberOfStripsHit()
{
	return pNCH;
}

int uTPCRawData::getWaveformSize(int strip)
{
	//return pWaveform[strip].size();
	return 27;
}

double uTPCRawData::getWaveformAtTimeBin(int strip, int timeBin)
{

	switch (timeBin)
	{
	case 0:
		return pADC0[strip];
		break;
	case 1:
		return pADC1[strip];
		break;
	case 2:
		return pADC2[strip];
		break;
	case 3:
		return pADC3[strip];
		break;
	case 4:
		return pADC4[strip];
		break;
	case 5:
		return pADC5[strip];
		break;
	case 6:
		return pADC6[strip];
		break;
	case 7:
		return pADC7[strip];
		break;
	case 8:
		return pADC8[strip];
		break;
	case 9:
		return pADC9[strip];
		break;
	case 10:
		return pADC10[strip];
		break;
	case 11:
		return pADC11[strip];
		break;
	case 12:
		return pADC12[strip];
		break;
	case 13:
		return pADC13[strip];
		break;
	case 14:
		return pADC14[strip];
		break;
	case 15:
		return pADC15[strip];
		break;
	case 16:
		return pADC16[strip];
		break;
	case 17:
		return pADC17[strip];
		break;
	case 18:
		return pADC18[strip];
		break;
	case 19:
		return pADC19[strip];
		break;
	case 20:
		return pADC20[strip];
		break;
	case 21:
		return pADC21[strip];
		break;
	case 22:
		return pADC22[strip];
		break;
	case 23:
		return pADC23[strip];
		break;
	case 24:
		return pADC24[strip];
		break;
	case 25:
		return pADC25[strip];
		break;
	case 26:
		return pADC26[strip];
		break;
	case 27:
		return pADC27[strip];
		break;
	case 28:
		return pADC28[strip];
		break;
	case 29:
		return pADC29[strip];
		break;
	default:
		return 0;
	}

	//return pWaveform[strip].at(timeBin);
}

int uTPCRawData::getStripNumber(int strip)
{
	return pStripNumber[strip];
}

int uTPCRawData::getStripId(int planeId, int stripNumber)
{
	for (int i = 0; i < SIZE; i++)
	{
		if (stripNumber == pStripNumber[i] && getPlaneId(i) == planeId)
		{
			return i;
		}
	}
	return -1;
}

int uTPCRawData::getNumber()
{
	return pNumber;
}

int uTPCRawData::getPlaneId(int strip)
{
	return pPlaneId[strip];
}

int uTPCRawData::importData(TString dir, TString fileName, Bool_t analyze,
		Bool_t fit, uTPCPlotData* plotData, uTPCViewer* mainFrame, int num)
{
	TString filePath = dir + "/" + fileName;

	fileIn = TFile::Open(filePath);

	treeIn = (TTree*) fileIn->Get("THit");
	if (!treeIn)
	{
		treeIn = (TTree*) fileIn->Get("events");
	}
	if (!treeIn)
	{
		std::cout << "File does not contain a tree called THit or events!"
				<< std::endl;
		return -1;
	}

	event = new uTPCEvent();

	int numEvents = treeIn->GetEntries();

	treeIn->SetBranchAddress("nch", &pNCH);
	treeIn->SetBranchAddress("planeID", &pPlaneId);
	treeIn->SetBranchAddress("strip", &pStripNumber);
	treeIn->SetBranchAddress("adc0", &pADC0);
	treeIn->SetBranchAddress("adc1", &pADC1);
	treeIn->SetBranchAddress("adc2", &pADC2);
	treeIn->SetBranchAddress("adc3", &pADC3);
	treeIn->SetBranchAddress("adc4", &pADC4);
	treeIn->SetBranchAddress("adc5", &pADC5);
	treeIn->SetBranchAddress("adc6", &pADC6);
	treeIn->SetBranchAddress("adc7", &pADC7);
	treeIn->SetBranchAddress("adc8", &pADC8);
	treeIn->SetBranchAddress("adc9", &pADC9);
	treeIn->SetBranchAddress("adc10", &pADC10);
	treeIn->SetBranchAddress("adc11", &pADC11);
	treeIn->SetBranchAddress("adc12", &pADC12);
	treeIn->SetBranchAddress("adc13", &pADC13);
	treeIn->SetBranchAddress("adc14", &pADC14);
	treeIn->SetBranchAddress("adc15", &pADC15);
	treeIn->SetBranchAddress("adc16", &pADC16);
	treeIn->SetBranchAddress("adc17", &pADC17);
	treeIn->SetBranchAddress("adc18", &pADC18);
	treeIn->SetBranchAddress("adc19", &pADC19);
	treeIn->SetBranchAddress("adc20", &pADC20);
	treeIn->SetBranchAddress("adc21", &pADC21);
	treeIn->SetBranchAddress("adc22", &pADC22);
	treeIn->SetBranchAddress("adc23", &pADC23);
	treeIn->SetBranchAddress("adc24", &pADC24);
	treeIn->SetBranchAddress("adc25", &pADC25);
	treeIn->SetBranchAddress("adc26", &pADC26);
	treeIn->SetBranchAddress("adc27", &pADC27);
	treeIn->SetBranchAddress("adc28", &pADC28);
	treeIn->SetBranchAddress("adc29", &pADC29);

	return numEvents;
}

void uTPCRawData::selectEvent(unsigned int eventNr, int thresholdAmplitude,
		int thresholdDistance, int thresholdPCA, int thresholdGamma,
		int thresholdDiscard)
{
	fThresholdAmplitude = thresholdAmplitude;
	fThresholdDiscard = thresholdDiscard;

	//std::cout << "rawData->getThresholdDiscard() " <<  this->getThresholdDiscard() << std::endl;
	//std::cout << "thresholdDiscard " <<  thresholdDiscard << std::endl;
	if (!fClusterX.empty())
	{
		fClusterX.clear();
	}
	if (!fClusterY.empty())
	{
		fClusterY.clear();
	}
	treeIn->GetEntry(eventNr);

}

void uTPCRawData::analyzeEvent(uTPCPlotData* plotData)
{
	event->analyseEvent(this, plotData);
}

uTPCEvent::uTPCEvent()
{
	//pHits = 0;
	pStripNumber = 0;
	pPlaneNumber = 0;
	initializeDataStructures();
}

uTPCEvent::~uTPCEvent()
{
	freeDataStructures();
}

void uTPCEvent::freeDataStructures()
{

	if (pStripNumber)
	{
		pStripNumber->clear();
		delete pStripNumber;
		pStripNumber = 0;
	}

	if (pPlaneNumber)
	{
		pPlaneNumber->clear();
		delete pPlaneNumber;
		pPlaneNumber = 0;
	}
}

void uTPCEvent::initializeDataStructures()
{
	freeDataStructures();
	//pHits = new std::vector<uTPCHit>();
	//pHits->clear();
	pStripNumber = new std::vector<unsigned int>();
	pStripNumber->clear();
	pPlaneNumber = new std::vector<unsigned int>();
	pPlaneNumber->clear();
	x0 = y0 = xf = yf = xm = ym = xi = yi = xs = ys = xd = yd = xmax = ymax = 0;

	xAmplitude = xIntegral = yAmplitude = yIntegral = 0.;
	nx = ny = 0;
	tx0 = ty0 = txf = tyf = txs = tys = txd = tyd = txmax = tymax = -1.;
	ex0 = ey0 = exf = eyf = exs = eys = exd = eyd = exmax = eymax = 9999999;
	nx = ny = 0;
	ntx = nty = 0;

	maxAmplitudeX = maxAmplitudeY = tMaxAmplitudeX = tMaxAmplitudeY = 0;

}

//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//START:    void uTPCEvent::analyseEvent(uTPCRawData* rawData, uTPCPlotData *plotData)
//**************************************************************************************************
//**************************************************************************************************
void uTPCEvent::analyseEvent(uTPCRawData* rawData, uTPCPlotData *plotData)
{

	initializeDataStructures();
	plotData->clearDataStructures();

	int discardFlag = 0;
	int numMaximaStripX = 0;
	int numMaximaStripY = 0;
	int timeBin1;

	float maxX = 0;
	float minX = 10000000;
	float maxY = 0;
	float minY = 10000000;

	float amp0 = 0;
	float amp1 = 0;
	float amp2 = 0;

	std::map<unsigned int, unsigned int> clusterX = rawData->getClusterX();
	std::map<unsigned int, unsigned int> clusterY = rawData->getClusterY();

	for (int strip = 0; strip < rawData->getNumberOfStripsHit(); strip++)
	{
		unsigned int stripNumber = rawData->getStripNumber(strip);
		//uTPCHit hit;
		float pAmplitude = 0;
		float pHalf = 0;
		float pPeak = 0;
		float pIntegral = 0;
		float pStrip = 0;
		float pPlane = 0;
		if (rawData->getPlaneId(strip) == 0)
		{
			pPlane = 0;
			pStrip = stripNumber;
			amp0 = 0;
			amp1 = 0;
			amp2 = 0;
			timeBin1 = 100;
			numMaximaStripX = 0;
			for (int timeBin = rawData->getWaveformSize(strip) - 1;
					timeBin >= 0; timeBin--)
			{
				if (rawData->getWaveformAtTimeBin(strip, timeBin)
						> rawData->getThresholdAmplitude())
				{
					if (txd < timeBin
							|| (txd == timeBin
									&& rawData->getWaveformAtTimeBin(strip,
											timeBin) < exd))
					{

						xd = stripNumber;
						txd = timeBin;
						exd = rawData->getWaveformAtTimeBin(strip, timeBin);

					}
					break;
				}
			}
			for (int timeBin = 0; timeBin < rawData->getWaveformSize(strip);
					timeBin++)
			{
				clusterX[stripNumber] = strip;
				if (maxX < rawData->getWaveformAtTimeBin(strip, timeBin))
				{
					maxX = rawData->getWaveformAtTimeBin(strip, timeBin);
				}
				if (minX > rawData->getWaveformAtTimeBin(strip, timeBin))
				{
					minX = rawData->getWaveformAtTimeBin(strip, timeBin);
				}

				amp2 = amp1;
				amp1 = amp0;
				amp0 = rawData->getWaveformAtTimeBin(strip, timeBin);
				if (amp2 <= amp1 && amp0 < amp1
						&& amp1 > rawData->getThresholdAmplitude()
						&& abs(timeBin1 - timeBin) > 2)
				{
					plotData->fStripMaximaX.push_back(stripNumber);
					plotData->fTimeMaximaX.push_back(timeBin - 1);
					plotData->fEnergyMaximaX.push_back(amp1);
					timeBin1 = timeBin - 1;
					if (amp0 > 1400)
					{
						numMaximaStripX++;
					}
				}
				else if (timeBin == rawData->getWaveformSize(strip) - 1
						&& amp0 > amp1 && amp1 > amp2
						&& amp0 > rawData->getThresholdAmplitude()
						&& abs(timeBin1 - timeBin) > 2)
				{
					plotData->fStripMaximaX.push_back(stripNumber);
					plotData->fTimeMaximaX.push_back(timeBin);
					plotData->fEnergyMaximaX.push_back(amp0);
					timeBin1 = timeBin;
					if (amp0 > 1400)
					{
						numMaximaStripX++;
					}
				}

				if (pAmplitude < rawData->getWaveformAtTimeBin(strip, timeBin)
						&& rawData->getThresholdAmplitude()
								< rawData->getWaveformAtTimeBin(strip, timeBin))
				{
					pAmplitude = rawData->getWaveformAtTimeBin(strip, timeBin);
					pPeak = timeBin;
					if (pAmplitude > maxAmplitudeX)
					{
						maxAmplitudeX = pAmplitude;
						tMaxAmplitudeX = timeBin;
					}

				}
				pIntegral += rawData->getWaveformAtTimeBin(strip, timeBin);
			}
			if (numMaximaStripX >= 4)
			{
				discardFlag = 1;
			}
			int timeBin;
			for (timeBin = 0; timeBin < rawData->getWaveformSize(strip);
					timeBin++)
			{
				if (rawData->getWaveformAtTimeBin(strip, timeBin)
						> pAmplitude / 2.)
					break;
			}
			if (timeBin > 0 && timeBin < rawData->getWaveformSize(strip) - 1)
			{
				pHalf = (timeBin - 1)
						+ (pAmplitude / 2.
								- rawData->getWaveformAtTimeBin(strip,
										timeBin - 1))
								/ (rawData->getWaveformAtTimeBin(strip, timeBin)
										- rawData->getWaveformAtTimeBin(strip,
												timeBin - 1));
			}
			else
			{
				pHalf = -.5;
			}

			if ((pHalf > tx0 || tx0 < 0.)
					|| (pHalf == tx0
							&& ex0
									> rawData->getWaveformAtTimeBin(strip,
											timeBin)))
			{
				tx0 = pHalf;
				x0 = pStrip;
				ex0 = rawData->getWaveformAtTimeBin(strip, timeBin);
			}

			if ((pHalf < txf || txf < 0.)
					|| (pHalf == txf
							&& exf
									> rawData->getWaveformAtTimeBin(strip,
											timeBin)))
			{
				txf = pHalf;
				xf = pStrip;
				exf = rawData->getWaveformAtTimeBin(strip, timeBin);
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

			//pHits->push_back(hit);
			pStripNumber->push_back(stripNumber);
			pPlaneNumber->push_back(pPlane);
		}

		else if (rawData->getPlaneId(strip) == 1)
		{
			pPlane = 1;
			pStrip = stripNumber;
			amp0 = 0;
			amp1 = 0;
			amp2 = 0;
			numMaximaStripY = 0;
			for (int timeBin = rawData->getWaveformSize(strip) - 1;
					timeBin >= 0; timeBin--)
			{
				if (rawData->getWaveformAtTimeBin(strip, timeBin)
						> rawData->getThresholdAmplitude())
				{
					if (tyd < timeBin
							|| (tyd == timeBin
									&& rawData->getWaveformAtTimeBin(strip,
											timeBin) < eyd))
					{

						yd = stripNumber;
						tyd = timeBin;
						eyd = rawData->getWaveformAtTimeBin(strip, timeBin);

					}

					break;
				}
			}
			for (int timeBin = 0; timeBin < rawData->getWaveformSize(strip);
					timeBin++)
			{
				clusterY[stripNumber] = strip;
				if (maxY < rawData->getWaveformAtTimeBin(strip, timeBin))
				{
					maxY = rawData->getWaveformAtTimeBin(strip, timeBin);
				}
				if (minY > rawData->getWaveformAtTimeBin(strip, timeBin))
				{
					minY = rawData->getWaveformAtTimeBin(strip, timeBin);
				}

				amp2 = amp1;
				amp1 = amp0;
				amp0 = rawData->getWaveformAtTimeBin(strip, timeBin);
				if (amp2 <= amp1 && amp0 < amp1
						&& amp1 > rawData->getThresholdAmplitude())
				{
					plotData->fStripMaximaY.push_back(pStrip);
					plotData->fTimeMaximaY.push_back(timeBin - 1);
					plotData->fEnergyMaximaY.push_back(amp1);
					if (amp0 > 1400)
					{
						numMaximaStripY++;
					}
				}
				else if (timeBin == rawData->getWaveformSize(strip) - 1
						&& amp0 > amp1 && amp1 > amp2
						&& amp0 > rawData->getThresholdAmplitude())
				{
					plotData->fStripMaximaY.push_back(pStrip);
					plotData->fTimeMaximaY.push_back(timeBin);
					plotData->fEnergyMaximaY.push_back(amp0);
					if (amp0 > 1400)
					{
						numMaximaStripY++;
					}
				}
				if (pAmplitude < rawData->getWaveformAtTimeBin(strip, timeBin)
						&& rawData->getThresholdAmplitude()
								< rawData->getWaveformAtTimeBin(strip, timeBin))
				{
					pAmplitude = rawData->getWaveformAtTimeBin(strip, timeBin);
					pPeak = timeBin;
					if (pAmplitude > maxAmplitudeY)
					{
						maxAmplitudeY = pAmplitude;
						tMaxAmplitudeY = timeBin;
					}
				}
				pIntegral += rawData->getWaveformAtTimeBin(strip, timeBin);
			}
			if (numMaximaStripY >= 4)
			{
				discardFlag = 1;
			}
			int timeBin;
			for (timeBin = 0; timeBin < rawData->getWaveformSize(strip);
					timeBin++)
			{
				if (rawData->getWaveformAtTimeBin(strip, timeBin)
						> pAmplitude / 2.)
					break;
			}
			if (timeBin > 0 && timeBin < rawData->getWaveformSize(strip) - 1)
			{
				pHalf = (timeBin - 1)
						+ (pAmplitude / 2.
								- rawData->getWaveformAtTimeBin(strip,
										timeBin - 1))
								/ (rawData->getWaveformAtTimeBin(strip, timeBin)
										- rawData->getWaveformAtTimeBin(strip,
												timeBin - 1));
			}
			else
			{
				pHalf = -.5;
			}

			if ((pHalf > ty0 || ty0 < 0.)
					|| (pHalf == ty0
							&& ey0
									> rawData->getWaveformAtTimeBin(strip,
											timeBin)))
			{
				ty0 = pHalf;
				y0 = pStrip;
				ey0 = rawData->getWaveformAtTimeBin(strip, timeBin);
			}

			if ((pHalf < tyf || tyf < 0.)
					|| (pHalf == tyf
							&& eyf
									> rawData->getWaveformAtTimeBin(strip,
											timeBin)))
			{
				tyf = pHalf;
				yf = pStrip;
				eyf = rawData->getWaveformAtTimeBin(strip, timeBin);
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
			//pHits->push_back(hit);
			pStripNumber->push_back(stripNumber);
			pPlaneNumber->push_back(pPlane);
		}
	}
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
	ntx = TMath::Abs(tx0 - txf) + 1;
	nty = TMath::Abs(ty0 - tyf) + 1;

	rawData->setMaxX(maxX);
	rawData->setMaxY(maxY);
	rawData->setMinX(minX);
	rawData->setMinY(minY);

	rawData->setClusterX(clusterX);
	rawData->setClusterY(clusterY);

	if (plotData->fStripMaximaX.size() > 0 && plotData->fStripMaximaY.size() > 0
			&& plotData->fStripMaximaX.size() < rawData->getThresholdDiscard()
			&& plotData->fStripMaximaY.size() < rawData->getThresholdDiscard() && discardFlag == 0)
	{

		for (int row = 0; row < plotData->fStripMaximaX.size(); row++)
		{
			plotData->fPointClustersX.push_back(row);
			if ((txs < plotData->fTimeMaximaX[row])
					|| (txs == plotData->fTimeMaximaX[row]
							&& exs > plotData->fEnergyMaximaX[row]))
			{
				txs = plotData->fTimeMaximaX[row];

				xs = plotData->fStripMaximaX[row];
				exs = plotData->fEnergyMaximaX[row];
			}
		}

		for (int row = 0; row < plotData->fStripMaximaY.size(); row++)
		{
			plotData->fPointClustersY.push_back(row);
			if ((tys < plotData->fTimeMaximaY[row])
					|| (tys == plotData->fTimeMaximaY[row]
							&& eys > plotData->fEnergyMaximaY[row]))
			{
				tys = plotData->fTimeMaximaY[row];

				ys = plotData->fStripMaximaY[row];
				eys = plotData->fEnergyMaximaY[row];
			}
		}
	}
}
