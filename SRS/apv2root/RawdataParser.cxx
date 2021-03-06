#include <sstream>
#include "RawdataParser.h"
#include "TMath.h"

RawdataParser::RawdataParser(std::string fileName, std::string pedestalName,
		bool isRawPedestal, bool isPedestal, bool isZS, float zsCut,
		bool isUTPC, int uTPCThreshold, std::vector<int> xChips,
		std::vector<int> yChips, bool viewEvent, int viewStart, int viewEnd) :
		isRawPedestalRun(isRawPedestal), isPedestalRun(isPedestal), isZSRun(
				isZS), fZsCut(zsCut), fViewEvent(viewEvent), fViewStart(
				viewStart), fViewEnd(viewEnd)
{
	fRawData16bits[0] = 0;
	fRawData16bits[1] = 0;
	if (!fViewEvent)
	{
		if (!isRawPedestalRun && !isZS)
		{
			std::stringstream ending;
			ending << "_ZsCut_" << fZsCut << ".root";
			fileName.replace(fileName.size() - 4, fileName.size(),
					ending.str());
		}
		else
		{
			std::stringstream ending;
			if (isUTPC)
			{
				ending << "_uTPC_" << uTPCThreshold << ".root";
			}
			else
			{
				ending << ".root";
			}
			fileName.replace(fileName.size() - 4, fileName.size(),
					ending.str());
		}
		fRoot = new RootFile(fileName, pedestalName, isRawPedestal, isPedestal,
				isZSRun, isUTPC, uTPCThreshold, xChips, yChips);

	}
}

RawdataParser::~RawdataParser()
{
	if (!fViewEvent)
	{
		delete fRoot;
	}
}

void RawdataParser::SetRunFlags(bool isRawPedestal, bool isPedestal)
{
	isRawPedestalRun = isRawPedestal;
	isPedestalRun = isPedestal;

	numTimeBins = 0;
	eventNr = 1;
	if (!fViewEvent)
	{
		fRoot->SetRunFlags(isRawPedestalRun, isPedestalRun);

	}
}

int RawdataParser::AnalyzeWord(int rawdata, int rawdata_before,
		int rawdata_before_two)
{
//std::cout <<eventNr<< " " << rawdata_before_two << " " << rawdata_before << " " << rawdata << std::endl;
	if ((rawdata_before >> 8) == 0x414443)
	{

		if (isZSRun == true)
		{
			std::cout
					<< "Error: The raw data file is not zero suppressed, but the analysis for zero-suppressed data has been chosen!"
					<< std::endl;
			return -1;
		}
		inEvent = true;
		inEquipmentHeader = false;
		wordCountEquipmentHeader = 0;
		apvID = rawdata_before & 0xff;
		wordCountEvent = 0;
		chNo = 0;
		maxADC = 0;
		startDataFlag = false;

		if (eventNr == 1)
		{

			if (maxAPVID < apvID)
			{
				maxAPVID = apvID;
			}
			if (minAPVID > apvID)
			{
				minAPVID = apvID;
			}
			if (!fViewEvent)
			{
				fRoot->InitPedestalData(fecID, apvID);
				fRoot->InitPedestalHistograms(fecID, apvID);
			}
		}
	}
	else if ((rawdata_before >> 8) == 0x41505a)
	{
		if (isZSRun == false)
		{
			std::cout
					<< "Error: The raw data file is zero suppressed, but the analysis for non zero-suppressed data has been chosen!"
					<< std::endl;
			return -1;
		}
		isRawPedestalRun = false;
		isPedestalRun = false;
		apvID = rawdata_before & 0xff;
		numTimeBins = 0;
		inEvent = true;
		inEquipmentHeader = false;
		wordCountEquipmentHeader = 0;
		wordCountEvent = 0;
		chNo = 0;
		maxADC = 0;
		startDataFlag = false;

	}

	else if (rawdata_before == 0xfafafafa)
	{

		startDataFlag = false;
		inEvent = false;

		if (!fViewEvent)
		{
			fRoot->FillHits();
			timeBinADCs.clear();
			if (isPedestalRun)
			{
				ComputePedestalData(apvID);
			}
			else if (isRawPedestalRun)
			{
				ComputeRawPedestalData(apvID);
			}
			else if (!isRawPedestalRun && !isPedestalRun && !isZSRun)
			{

				ComputeCorrectedData(apvID);
			}
		}

	}
	else if (rawdata_before == 0xda1e5afe && rawdata == 0x50
			&& rawdata_before_two == 0x50)
	{
		format = 0;
		header++;
		inEvent = false;
	}
	else if (rawdata_before == 0xda1e5afe && rawdata == 0x48
			&& rawdata_before_two == 0x48)
	{
		format = 1;
		header++;
		inEvent = false;
	}
	else if (rawdata_before == 0xda1e5afe && rawdata == 0x50
			&& rawdata_before_two != 0x50)
	{
		headerLDC++;
		headerEquipment++;

		inEquipmentHeader = true;
		inEvent = false;
	}
	else if (rawdata_before == 0xda1e5afe && rawdata == 0x48
			&& rawdata_before_two != 0x48)
	{
		headerLDC++;
		headerEquipment++;

		inEquipmentHeader = true;
		inEvent = false;
	}

	if (inEquipmentHeader)
	{
		wordCountEquipmentHeader++;

		if (format == 0)
		{
			if (wordCountEquipmentHeader == 5)
			{
				runNr = rawdata_before;
				eventNr = rawdata;

				if (fViewEnd < eventNr && fViewEnd != 0)
				{
					return -1;
				}

				if (fViewStart <= eventNr && fViewEnd >= eventNr)
				{
					printf("\neventNr  %d - run Nr %d\n", eventNr, runNr);

				}
			}
			else if (wordCountEquipmentHeader == 18)
			{
				unixtimestamp = rawdata_before;
				timestamp_us = rawdata;

			}
			if (wordCountEquipmentHeader == 22)
			{
				fecID = rawdata_before & 0xff;
				if (maxFECID < fecID)
				{
					maxFECID = fecID;
				}
				if (minFECID > fecID)
				{
					minFECID = fecID;
				}
			}
		}
		else
		{
			if (wordCountEquipmentHeader == 5)
			{
				runNr = rawdata_before;
				eventNr = rawdata;

				if (fViewEnd < eventNr && fViewEnd != 0)
				{
					return -1;
				}
				if (fViewStart <= eventNr && fViewEnd >= eventNr)
				{
					printf("\neventNr  %d - run Nr %d\n", eventNr, runNr);

				}
			}
			else if (wordCountEquipmentHeader == 16)
			{
				unixtimestamp = rawdata_before;
				timestamp_us = rawdata;

			}
			if (wordCountEquipmentHeader == 20)
			{
				fecID = rawdata_before & 0xff;
				if (maxFECID < fecID)
				{
					maxFECID = fecID;
				}
				if (minFECID > fecID)
				{
					minFECID = fecID;
				}
			}
		}
	}
	if ((rawdata >> 8) == 0x41505a || (rawdata >> 8) == 0x414443)
	{
		inEvent = false;
	}
	if (inEvent)
	{
		wordCountEvent++;

		if (wordCountEvent == 2)
		{
			packetSize = (rawdata_before & 0xffff);
			idata = 0;
			theTimeBin = 0;
		}
		else if (wordCountEvent > 2)
		{

			int data1 = (rawdata_before >> 24) & 0xff;
			int data2 = (rawdata_before >> 16) & 0xff;
			int data3 = (rawdata_before >> 8) & 0xff;
			int data4 = rawdata_before & 0xff;
			fRawData16bits[0] = ((data2 << 8) | data1);
			fRawData16bits[1] = ((data4 << 8) | data3);

			if (isZSRun)
			{
				AnalyzeEventZS();
			}
			else
			{
				if (!fViewEvent)
				{
					AnalyzeEvent();
				}
			}

		}

	}
	return eventNr;

}

void RawdataParser::AnalyzeEventZS()
{

	if (idata == 0)
	{
		std::stringstream ss;
		ss << std::hex << (fRawData16bits[1] >> 8);
		ss >> numTimeBins;
		//std::cout << "time bins " << numTimeBins << std::endl;
	}

	for (int i = 0; i <= 1; i++)
	{
		if (idata >= 4)
		{

			if (((idata - 4) % (numTimeBins + 1)) == 0)
			{
				chNo = fRawData16bits[idata % 2];
			}
			else
			{
				float data = ((float) fRawData16bits[i]);

				//Originally the if condition was like this, which was wrong
				//For a 12 bit ADC, the sign bit is bit 12
				//if ((fRawData16bits[i] >> 8) != 0)

				if ((fRawData16bits[i] >> 11) != 0)
				{
					data = data - 65536;
				}
				data = -data;
				if (data > maxADC)
				{
					maxADC = data;
					timeBinMaxADC = (idata - 4) % (numTimeBins + 1);

				}

				timeBinADCs.push_back(data);
				//printf("%f\n", data);

			}
			if (((idata - 4) % (numTimeBins + 1)) == numTimeBins)
			{
				if (!fViewEvent)
				{
					if (timeBinADCs.size() > 0
							&& fRoot->GetStripNumber(chNo) <= 127)
					{
						fRoot->AddHits(unixtimestamp, timestamp_us, eventNr,
								fecID, apvID, fRoot->GetStripNumber(chNo),
								maxADC, timeBinMaxADC, timeBinADCs);
					}
				}
				timeBinADCs.clear();
				maxADC = 0;
				timeBinMaxADC = 0;

			}
		}
		idata++;
	}

}

//****************************************************************************
// Analysis of data that is not zero suppressed (pedestal or physics data)
//****************************************************************************
// After 3 consecutive words (fRawData16bits) below the APV header level,
// a block with meaning full data starts 9 words later
// Data format:
// time bin 0: strip 0, strip 1, strip 2, .., strip 127
// time bin 1: strip 0, strip 1, strip 2, .., strip 127
// time bin 2: strip 0, strip 1, strip 2, .., strip 127
// time bin n-1 (n<=30): strip 0, strip 1, strip 2, .., strip 127

//****************************************************************************
void RawdataParser::AnalyzeEvent()
{

	for (int i = 0; i <= 1; i++)
	{
		if (startDataFlag)
		{
			if (idata >= 9)
			{
				int chNo = (idata - 9) % NCH;
				int stripNo = fRoot->GetStripNumber((idata - 9) % NCH);

				//12 bit ADC value comes as 16 bit number
				float rawdata = ((float) fRawData16bits[i]);
				rawdata = 4096 - rawdata;

				//Raw pedestal run: raw pedestal has to be calculated first
				//Nothing to subtract
				if (isRawPedestalRun == false)
				{
					float theOffset = 0;
					//Pedestal run, subtract raw pedestal offset for present strip
					if (isPedestalRun)
					{
						theOffset = fRoot->GetStripRawPedestalOffset(fecID,
								apvID, stripNo);
					}
					//Physics run, subtract pedestal offset for present strip
					else
					{
						theOffset = fRoot->GetStripPedestalOffset(fecID, apvID,
								stripNo);
					}
					//Offset for time bin t: store values of 128 strips
					commonModeOffset.push_back(rawdata - theOffset);
				}

				//Last strip (strip 127) in time bin
				if ((idata - 9) % NCH == (NCH - 1))
				{
					//Calculate the mean of the common mode offset vector filled above
					if (!isRawPedestalRun)
					{
						float weightedMean = TMath::Mean(
								commonModeOffset.begin(),
								commonModeOffset.end());
						//Store the weighted mean in a vector containing a value for each time bin
						timeBinOffset.push_back(weightedMean);
						commonModeOffset.clear();
					}

					startDataFlag = false;
					idata = -1;
					theTimeBin++;
					//Determine the number of time bins in the data block
					if (theTimeBin >= numTimeBins)
					{
						numTimeBins = theTimeBin;
					}

				}
				//std::cout << chNo << " " << stripNo << " " <<  theTimeBin << std::endl;
				//For each of the 128 strips, a vector is filled with the ADC values
				//of the n time bins
				//Data structure contains the full event (one APV)
				fRoot->FillStripData(stripNo, rawdata);

			}
			idata++;
		}
		else
		{
			if (idata == 2)
			{
				if (fRawData16bits[i] < apvheaderlevel)
				{
					startDataFlag = true;
					if (!isRawPedestalRun && !isPedestalRun)
					{
						if (theTimeBin == 0 && fRoot->StripDataExists(0))
						{
							ComputeCorrectedData(apvID - 1);
							timeBinOffset.clear();
						}
					}
					else if (isRawPedestalRun)
					{
						if (theTimeBin == 0 && fRoot->StripDataExists(0))
						{
							ComputeRawPedestalData(apvID - 1);
							timeBinOffset.clear();
						}
					}
					else if (isPedestalRun)
					{
						if (theTimeBin == 0 && fRoot->StripDataExists(0))
						{
							ComputePedestalData(apvID - 1);
							timeBinOffset.clear();
						}
					}

				}
				idata = 0;

			}
			else if (idata == 1)
			{
				if (fRawData16bits[i] < apvheaderlevel)
				{
					idata = 2;
				}
				else
				{
					idata = 0;
				}

			}
			else if (idata == 0)
			{
				if (fRawData16bits[i] < apvheaderlevel)
				{
					idata = 1;
				}
				else
				{
					idata = 0;
				}
			}

		}

	}

}

//****************************************************************************
// Raw pedestal run:  (pedestal data)
// For each strip in each event, the mean of the ADC values of the n time bins
// is calculated, and subtracted from the raw data
//****************************************************************************
void RawdataParser::ComputeRawPedestalData(int theApvID)
{
	for (int stripNo = 0; stripNo < NCH; stripNo++)
	{
		float mean = fRoot->GetMeanStripData(stripNo);
		fRoot->FillPedestalData(fecID, theApvID, stripNo, mean);

	}

}

//****************************************************************************
// Pedestal run:  (pedestal data)
// For each time bin of each strip in each event, the time bin offset
// that was calculated in AnalyzeEvent() is subtracted from the raw data
//****************************************************************************
void RawdataParser::ComputePedestalData(int theApvID)
{
	for (int stripNo = 0; stripNo < NCH; stripNo++)
	{
		std::vector<float> meanPedestal;

		for (int timeBin = 0; timeBin < numTimeBins; timeBin++)
		{
			float rawdata = fRoot->GetStripData(stripNo, timeBin);
			if (isPedestalRun)
			{
				//std::cout << timeBin << " " << rawdata << " " << timeBinOffset[timeBin] << std::endl;
			}
			meanPedestal.push_back(rawdata - timeBinOffset[timeBin]);

		}

		float mean = TMath::Mean(meanPedestal.begin(), meanPedestal.end());

		fRoot->FillPedestalData(fecID, theApvID, stripNo, mean);
		fRoot->ClearStripData(stripNo);
		meanPedestal.clear();
	}
}

void RawdataParser::ComputeCorrectedData(int theApvID)
{

	for (int stripNo = 0; stripNo < NCH; stripNo++)
	{
		maxADC = 0;
		timeBinMaxADC = 0;
		timeBinADCs.clear();

		for (int timeBin = 0; timeBin < numTimeBins; timeBin++)
		{
			float rawdata = fRoot->GetStripData(stripNo, timeBin);
			rawdata -= timeBinOffset[timeBin];
			rawdata -= fRoot->GetStripPedestalOffset(fecID, theApvID, stripNo);
			timeBinADCs.push_back(rawdata);
			if (rawdata > maxADC)
			{
				maxADC = rawdata;
				timeBinMaxADC = timeBin;
			}
		}

		if (fZsCut > 0)
		{
			if (TMath::Mean(timeBinADCs.begin(), timeBinADCs.end())
					> fZsCut
							* fRoot->GetStripPedestalNoise(fecID, theApvID,
									stripNo))
			{
				fRoot->AddHits(unixtimestamp, timestamp_us, eventNr, fecID,
						theApvID, stripNo, maxADC, timeBinMaxADC, timeBinADCs);
			}
		}
		else
		{
			fRoot->AddHits(unixtimestamp, timestamp_us, eventNr, fecID,
					theApvID, stripNo, maxADC, timeBinMaxADC, timeBinADCs);
		}
		fRoot->ClearStripData(stripNo);
	}
}

void RawdataParser::CreateHistograms()
{

	fRoot->CreateHistograms(minFECID, maxFECID, minAPVID, maxAPVID);

}

