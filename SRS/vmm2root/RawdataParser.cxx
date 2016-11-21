#include "RawdataParser.h"
#include "TMath.h"

RawdataParser::RawdataParser(std::string fileName, bool viewEvent,
		int viewStart, int viewEnd) :
		fViewEvent(viewEvent), fViewStart(viewStart), fViewEnd(viewEnd)
{
	if (!fViewEvent)
	{
		std::stringstream ending;
		ending << "_TREE.root";
		fileName.replace(fileName.size() - 4, fileName.size(), ending.str());

		fRoot = new RootFile(fileName);
	}
}

RawdataParser::~RawdataParser()
{
	if (!fViewEvent)
	{
		fRoot->WriteRootFile();
		if (fRoot)
		{
			delete fRoot;
		}
	}
}

unsigned int RawdataParser::AnalyzeWord(unsigned int eventID,
		unsigned int rawdata, unsigned int rawdata_before,
		unsigned int rawdata_before_two)
{
	//eventNr = eventID;

	if ((rawdata_before >> 8) == 0x564d32)
	{
		inEvent = true;
		inEquipmentHeader = false;
		wordCountEquipmentHeader = 0;
		wordCountEvent = 0;
		validEvent = false;
	}
	else if (rawdata_before == 0xfafafafa)
	{
		if (validEvent && !fViewEvent)
		{
			//std::cout << "*****************************    Fill Hits!" << std::endl;
			fRoot->FillHits();
		}
		inEvent = false;
		wordCountEvent = 0;
	}
	else if (rawdata_before == 0xda1e5afe && rawdata == 0x50
			&& rawdata_before_two == 0x50)
	{
		header++;
		inEvent = false;
	}
	else if (rawdata_before == 0xda1e5afe && rawdata == 0x50
			&& rawdata_before_two != 0x50)
	{
		headerEquipment++;
		inEvent = false;
		inEquipmentHeader = true;
		validEvent = false;
	}

	if (inEquipmentHeader)
	{
		wordCountEquipmentHeader++;
		if (wordCountEquipmentHeader == 3)
		{
			//Physics event
			if (rawdata != 7)
			{
				inEquipmentHeader = false;
			}
		}
		else if (wordCountEquipmentHeader == 5)
		{
			runNr = rawdata_before;
			eventNr = rawdata;
			if (fViewStart <= eventNr && fViewEnd >= eventNr)
			{
				printf("\neventNr  %d\n", eventNr);

			}

		}
		else if (wordCountEquipmentHeader == 19)
		{
			unixtimestamp = rawdata_before_two;
			timestamp_us = rawdata_before;
			eventSize = rawdata;
		}
		else if (wordCountEquipmentHeader == 22)
		{

			fecID = rawdata_before & 0xff;

		}
		//printf("%d   %x \n", wordCountEquipmentHeader, rawdata_before);

	}

	if (inEvent)
	{
		wordCountEvent++;
		if (wordCountEvent == 1)
		{
			frameCounter = rawdata_before_two;
			vmmID = rawdata_before & 0xff;
			if (fViewStart <= eventNr && fViewEnd >= eventNr)
			{

				printf("frameCounter %d - vmmID  %d\n", frameCounter, vmmID);

			}

		}

		//printf("wordCountEvent %d rawdata_before_two  %x\n", wordCountEvent, rawdata_before_two);
		if (wordCountEvent > 2 && (rawdata >> 8) != 0x564d32)
		{
			validEvent = true;
			if (wordCountEvent == 3)
			{

				triggerCount = (rawdata_before_two >> 16);
				triggerTimestamp = rawdata_before_two & 0xFFFF;
				if (fViewStart <= eventNr && fViewEnd >= eventNr)
				{
					printf("TriggerCount  %d - TriggerTimestamp %d\n",
							triggerCount, triggerTimestamp);
				}

			}
			if (wordCountEvent % 2 == 0)
			{
				unsigned int data = ReverseBits(rawdata_before);
				flag = (data & 0x1);
				overThresholdFlag = (data & 0x2) >> 1;
				chNo = (data & 0xfc) >> 2;

			}
			else
			{
				unsigned int data = ReverseBits(rawdata_before);
				unsigned int data1 = (data >> 24) & 0xFF;
				unsigned int data2 = (data >> 16) & 0x3;
				unsigned int data3 = (data >> 18) & 0x3F;
				unsigned int data4 = (data >> 8) & 0x3;
				unsigned int data5 = (data >> 10) & 0x3F;
				unsigned int data6 = data & 0x3F;

				//10 bits (8+2)
				adc = (data2 << 8) + data1;
				//8 bits (6+2)
				tdc = (data4 << 6) + data3;
				//12 bits (6+6)
				bcid = (data6 << 6) + data5;

				if (!fViewEvent)
				{
					fRoot->AddHits(unixtimestamp, timestamp_us, eventNr, fecID,
							eventSize, vmmID, triggerCount, triggerTimestamp,
							overThresholdFlag, chNo, adc, tdc, bcid);
				}
				else
				{
					if (fViewStart <= eventNr && fViewEnd >= eventNr)
					{
						printf(
								"chNo  %d - overThresholdFlag %d - adc %d - tdc %d - bcid %d\n",
								chNo, overThresholdFlag, adc, tdc, bcid);

					}
				}

			}

		}

	}

	return eventNr;

}

unsigned int RawdataParser::ReverseBits(unsigned int n)
{
	n = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
	n = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
	n = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
	n = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
	n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
	return n;
}
