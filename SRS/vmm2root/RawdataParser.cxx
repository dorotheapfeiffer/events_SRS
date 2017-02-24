#include "RawdataParser.h"
#include "TMath.h"
#include <time.h>

RawdataParser::RawdataParser(std::string fileName, double bc, double tac,
		std::vector<int> xChips, std::vector<int> yChips, std::string readout,
		bool viewEvent, int viewStart, int viewEnd) :
		bcClock(bc), tacSlope(tac), xChipIDs(xChips), yChipIDs(yChips), readoutType(
				readout), fViewEvent(viewEvent), fViewStart(viewStart), fViewEnd(
				viewEnd)
{
	if (!fViewEvent)
	{
		std::stringstream ending;
		ending << ".root";
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

unsigned int RawdataParser::AnalyzeWord(unsigned int rawdata,
		unsigned int rawdata_before, unsigned int rawdata_before_two)
{

	//0x564d32 = VM2
	if ((rawdata_before >> 8) == 0x564d32)
	{
		inEvent = true;
		inEquipmentHeader = false;
		wordCountEquipmentHeader = 0;
		wordCountEvent = 0;
		validEvent = false;
	}
	//empty event
	else if ((rawdata_before >> 8) == 0x564132)
	{
		inEvent = false;
		wordCountEvent = 0;
	}
	else if (rawdata_before == 0xfafafafa)
	{
		if (validEvent && !fViewEvent)
		{
			const int minSize=1;
			const int minDeltaT=250;
			double time1 = -1, time2 = -1;

			//std::multimap<double, std::pair<int, unsigned int> > buffer;

			std::multimap<int, std::pair<double, unsigned int> > cluster;

			std::multimap<double, std::pair<int, unsigned int>>::iterator itHits =
					hitsX.begin();

			int count = 0;
			for (; itHits != hitsX.end(); itHits++)
			{
				time2 = time1;
				time1 = itHits->first;

				if ((time1 - time2 <= minDeltaT) || count == 0)
				{
					cluster.insert(
							std::make_pair(itHits->second.first,
									std::make_pair(time1,
											itHits->second.second)));
				}
				else
				{

					if (cluster.size() >= minSize)
					{
						std::multimap<int, std::pair<double, unsigned int> >::iterator itCluster =
								cluster.begin();
						float centerOfGravity = 0;
						float centerOfTime = 0;
						unsigned int totalADC = 0;
						unsigned int adc1 = 9999, adc2 = 9999;
						int strip1 = -1, strip2 = -1;
						int stripCount = 0;
						//std::cout << "**************************" << std::endl;
						for (; itCluster != cluster.end(); itCluster++)
						{
							adc2 = adc1;
							adc1 = itCluster->second.second;
							strip2 = strip1;
							strip1 = itCluster->first;
							if(stripCount != 0  && abs(strip1 - strip2))
							{

								//std::cout << strip1 << " " << itCluster->second.first << " " << adc1 << " " << adc1%16 << " " << itCluster->second.first << " " <<adc2 << " " << adc2%16 << " " << abs((int)adc1-(int)adc2) << std::endl;


							}
							if (stripCount == 0 || (abs(strip1 - strip2) != 0 && abs(strip1 - strip2) <=2) )
							{
								stripCount++;
								centerOfGravity += itCluster->first
										* itCluster->second.second;
								centerOfTime += itCluster->second.first
										* itCluster->second.second;
								totalADC += itCluster->second.second;
								/*
								 std::cout << itBuf->first << " "
								 << itBuf->second.first << " "
								 << itBuf->second.second << std::endl;
								 */
							}

						}
						if (stripCount >= minSize)
						{
							centerOfGravity = (centerOfGravity / (double)totalADC);
							centerOfTime = (centerOfTime / totalADC);
							if(centerOfGravity > 127)
								std::cout <<  "\n" << centerOfTime << " " << centerOfGravity	<< " " << totalADC << std::endl;
							fRoot->AddClusters(centerOfGravity, -1.0, stripCount, totalADC,
									centerOfTime);
						}

					}
					cluster.clear();
					cluster.insert(
							std::make_pair(itHits->second.first,
									std::make_pair(time1,
											itHits->second.second)));

				}
				count++;
			}

			if (cluster.size() >= minSize)
			{
				std::multimap<int, std::pair<double, unsigned int> >::iterator itCluster =
						cluster.begin();
				float centerOfGravity = 0;
				float centerOfTime = 0;
				unsigned int totalADC = 0;
				unsigned int adc1 = 9999, adc2 = 9999;
				int strip1 = -1, strip2 = -1;
				int stripCount = 0;
				//std::cout << "**************************" << std::endl;
				for (; itCluster != cluster.end(); itCluster++)
				{
					adc2 = adc1;
					adc1 = itCluster->second.second;
					strip2 = strip1;
					strip1 = itCluster->first;
					if (stripCount == 0 || (abs(strip1 - strip2) != 0 && abs(strip1 - strip2) <=2) )
					{
						stripCount++;
						centerOfGravity += itCluster->first
								* itCluster->second.second;
						centerOfTime += itCluster->second.first
								* itCluster->second.second;
						totalADC += itCluster->second.second;
						/*
						 std::cout << itBuf->first << " "
						 << itBuf->second.first << " "
						 << itBuf->second.second << std::endl;
						 */
					}

				}
				if (stripCount >= minSize)
				{
					centerOfGravity = (centerOfGravity / (double)totalADC);
					centerOfTime = (centerOfTime / totalADC);
					if(centerOfGravity > 127)
					std::cout <<  "\n" << centerOfTime << " " << centerOfGravity	<< " " << totalADC << std::endl;
					fRoot->AddClusters(centerOfGravity, -1.0, stripCount,totalADC,
							centerOfTime);
				}

			}
			cluster.clear();

			//std::cout << "*****************************    Fill Hits!" << std::endl;
			fRoot->FillHits();
			hitsX.clear();
			hitsY.clear();

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
				wordCountEquipmentHeader = 0;
			}
		}
		else if (wordCountEquipmentHeader == 5)
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
		else if (wordCountEquipmentHeader == 19)
		{
			oldUnixtimestamp = unixtimestamp;
			oldTimestamp_us = timestamp_us;
			unixtimestamp = rawdata_before_two;
			timestamp_us = rawdata_before;
			deltaUnixTimestamp = 1000 * (unixtimestamp - oldUnixtimestamp)
					+ 0.001 * (timestamp_us - oldTimestamp_us);
			//deltaUnixTimestamp =  0.001*(timestamp_us-oldTimestamp_us);
			eventSize = rawdata;

			if (fViewStart <= eventNr && fViewEnd >= eventNr)
			{
				printf("Time since last unixtime  %.4f ms\n",
						deltaUnixTimestamp);
				time_t unixtime = (time_t) unixtimestamp;
				printf("unixtimestamp %d -> %s", unixtimestamp,
						asctime(gmtime(&unixtime)));
				printf("timestamp_us  %d\n", timestamp_us);
				printf("eventSize  %d\n", eventSize);

			}

		}
		else if (wordCountEquipmentHeader == 22)
		{

			fecID = rawdata_before & 0xff;
			oldVmmID = -1;
			oldAdc = -1;
			oldTdc = -1;
			oldBcid = -1;
			oldChNo = -1;
			if (fViewStart <= eventNr && fViewEnd >= eventNr)
			{
				printf("fecID  %d\n", fecID);

			}

		}

	}
// **********************************************************************************
// Readout frequency
// **********************************************************************************
// Trigger preset: Hex number that gives time period, to be multiplied by 25 ns
// Example:
// 	- Trigger preset 0xFAO = 4000, 4000 * 25 ns = 100000 ns = 100 us,
//	=> frequency = 10kHz
// **********************************************************************************
// Bunch crossing clock: 2.5 - 160 MHz (400 ns - 6.25 ns)
// 12-bit bcid, overflows after 4096 clock cycles
// To get useful bcid, trigger preset has to be chosen so that bcid does not overflow
// **********************************************************************************

// **********************************************************************************
// VMM2 register
// **********************************************************************************
	/*
	 (*RegGlobNames)[0] ="glob_polarity";           (*RegGlob)[0] = 0;   (*RegGlobVals)[0]={"wires", "strips"};
	 (*RegGlobNames)[1] ="glob_leakage_current";    (*RegGlob)[1] = 1;   (*RegGlobVals)[1]={"0", "1", "true", "false"};
	 (*RegGlobNames)[2] ="analog_tristates";        (*RegGlob)[2] = 0;   (*RegGlobVals)[2]={"0", "1", "true", "false"};
	 (*RegGlobNames)[3] ="double_leakage";          (*RegGlob)[3] = 0;   (*RegGlobVals)[3]={"0", "1", "true", "false"};
	 (*RegGlobNames)[4] ="gain";                    (*RegGlob)[4] = 2;   (*RegGlobVals)[4]={"0.5", "1.0", "3.0", "4.5", "6.0", "9.0", "12.0", "16.0"};//mV/fC
	 (*RegGlobNames)[5] ="peak_time";               (*RegGlob)[5] = 0;   (*RegGlobVals)[5]={"200", "100", "50", "25"}; //ns
	 (*RegGlobNames)[6] ="neighbor_trigger";        (*RegGlob)[6] = 0;   (*RegGlobVals)[6]={"0", "1", "true", "false"};
	 (*RegGlobNames)[7] ="tac_slope";               (*RegGlob)[7] = 0;   (*RegGlobVals)[7]={"125", "250", "500", "1000"}; //ns
	 (*RegGlobNames)[8] ="disable_at_peak";         (*RegGlob)[8] = 0;   (*RegGlobVals)[8]={"0", "1", "true", "false"};
	 (*RegGlobNames)[9] ="art";                     (*RegGlob)[9] = 1;   (*RegGlobVals)[9]={"threshold", "peak"};
	 (*RegGlobNames)[10]="art_mode";                (*RegGlob)[10] = 0;  (*RegGlobVals)[10]={"0", "1", "true", "false"};
	 (*RegGlobNames)[11]="dual_clock_art";          (*RegGlob)[11] = 0;  (*RegGlobVals)[11]={"0", "1", "true", "false"};
	 (*RegGlobNames)[12]="out_buffer_mo";           (*RegGlob)[12] = 0;  (*RegGlobVals)[12]={"0", "1", "true", "false"};
	 (*RegGlobNames)[13]="out_buffer_pdo";          (*RegGlob)[13] = 0;  (*RegGlobVals)[13]={"0", "1", "true", "false"};
	 (*RegGlobNames)[14]="out_buffer_tdo";          (*RegGlob)[14] = 0;  (*RegGlobVals)[14]={"0", "1", "true", "false"};
	 (*RegGlobNames)[15]="channel_monitor";         (*RegGlob)[15] = 0;  (*RegGlobVals)[15]={"0", "1", "true", "false"};
	 (*RegGlobNames)[16]="monitoring_control";      (*RegGlob)[16] = 1;  (*RegGlobVals)[16]={"0", "1", "true", "false"};
	 (*RegGlobNames)[17]="monitor_pdo_out";         (*RegGlob)[17] = 0;  (*RegGlobVals)[17]={"0", "1", "true", "false"};
	 (*RegGlobNames)[18]="adcs";                    (*RegGlob)[18] = 1;  (*RegGlobVals)[18]={"0", "1", "true", "false"};
	 (*RegGlobNames)[19]="sub_hysteresis";          (*RegGlob)[19] = 0;  (*RegGlobVals)[19]={"0", "1", "true", "false"};
	 (*RegGlobNames)[20]="direct_time";             (*RegGlob)[20] = 0;  (*RegGlobVals)[20]={"0", "1", "true", "false"};
	 (*RegGlobNames)[21]="direct_time_mode";        (*RegGlob)[21] = 1;  (*RegGlobVals)[21]={"TtP", "ToT", "PtP", "PtT"};
	 (*RegGlobNames)[22]="direct_time_mode0";       (*RegGlob)[22] = 0;  (*RegGlobVals)[22]={"0", "1", "true", "false"};
	 (*RegGlobNames)[23]="direct_time_mode1";       (*RegGlob)[23] = 1;  (*RegGlobVals)[23]={"0", "1", "true", "false"};
	 (*RegGlobNames)[24]="conv_mode_8bit";          (*RegGlob)[24] = 1;  (*RegGlobVals)[24]={"0", "1", "true", "false"};
	 (*RegGlobNames)[25]="enable_6bit";             (*RegGlob)[25] = 0;  (*RegGlobVals)[25]={"0", "1", "true", "false"};
	 (*RegGlobNames)[26]="adc_10bit";               (*RegGlob)[26] = 0;  (*RegGlobVals)[26]={"200ns", "+60ns"};
	 (*RegGlobNames)[27]="adc_8bit";                (*RegGlob)[27] = 0;  (*RegGlobVals)[27]={"100ns", "+60ns"};
	 (*RegGlobNames)[28]="adc_6bit";                (*RegGlob)[28] = 0;  (*RegGlobVals)[28]={"low", "middle", "up"};
	 (*RegGlobNames)[29]="dual_clock_data";         (*RegGlob)[29] = 0;  (*RegGlobVals)[29]={"0", "1", "true", "false"};
	 (*RegGlobNames)[30]="dual_clock_6bit";         (*RegGlob)[30] = 0;  (*RegGlobVals)[30]={"0", "1", "true", "false"};
	 (*RegGlobNames)[31]="threshold_dac";           (*RegGlob)[31] = 200;    //(*RegGlobVals)[31] filled before
	 (*RegGlobNames)[32]="test_pulse_dac";          (*RegGlob)[32] = 300;    //(*RegGlobVals)[32] filled before
	 (*RegGlobNames)[33]="UseMapping";              (*RegGlob)[33] = 0;  (*RegGlobVals)[33]={"0", "1", "true", "false"};
	 */

	if (inEvent)
	{
		wordCountEvent++;
		if (wordCountEvent == 1)
		{
			//Register 0x0A: evbld_mode
			// 0x0: (default) use frame-of-event counter (8-bit)
			// 0x01: use frame-of-run counter (32-bit)
			// 0x02: use timestamp(24-bit) and frame-of-event (8-bit) ctr
			frameCounter = rawdata_before_two;
			oldVmmID = vmmID;
			vmmID = rawdata_before & 0xff;

			if (fViewStart <= eventNr && fViewEnd >= eventNr)
			{

				printf("\n\tframeCounter %d\n", frameCounter);
				printf("\tvmmID  %d\n", vmmID);

			}

		}

		if (wordCountEvent > 1 && (rawdata >> 8) != 0x564d32
				&& rawdata != 0xFAFAFAFA)
		{
			validEvent = true;
			//printf("\n wordcount %d - rawdata_before_two %x - rawdata_before %x - rawdata %x\n", wordCountEvent,
			//	rawdata_before_two, rawdata_before, rawdata);
			if (wordCountEvent == 2)
			{

				//Register 0x0C: evbld_eventInfoData
				//          31-16          15-0
				// 0x00: HINFO_LABEL EVBLD_DATALENGTH
				// 0x01: TRIGGERCOUNTER EVBLD_DATALENGTH
				// 0x02: TRIGGERCOUNTER (31-0)
				// 0x03: TRIGGERTIMESTAMP EVBLD_DATALENGTH
				// 0x04: TRIGGERTIMESTAMP (31-0)
				// 0x05: TRIGGERCOUNTER TRIGGERTIMESTAMP

				// High resolution checkbox (Atlas tool)
				// modifies register 0x0C: evbld_eventInfoData
				// enable: 0x80
				// disable: 0x00 (default)
				// enable: removes top 3 bits of 32 bit timestamp, adds 3 bit for high res
				// 3 bit high res are 320 MHz = 3.125 ns
				// high res disabled: 25 ns resolution
				// high res enabled: 3.125 ns resolution
				oldTriggerTimestamp = theTriggerTimestamp;

				triggerTimestamp = rawdata_before;
				theTriggerTimestamp = triggerTimestamp * 3.125;

				if (oldTriggerTimestamp > theTriggerTimestamp)
				{
					deltaTriggerTimestamp = (13421772800 + theTriggerTimestamp
							- oldTriggerTimestamp) * 0.001;

				}
				else
				{
					deltaTriggerTimestamp = (theTriggerTimestamp
							- oldTriggerTimestamp) * 0.001;

				}
				if (fViewStart <= eventNr && fViewEnd >= eventNr)
				{
					if (deltaTriggerTimestamp > 0)
					{
						printf("\tTime since last trigger %.4f us (%.4f kHz)\n",
								deltaTriggerTimestamp,
								1000 / deltaTriggerTimestamp);
						printf("\tTriggerTimestamp %.2f\n",
								theTriggerTimestamp);
					}

				}

			}
			if (wordCountEvent > 2)
			{
				if (wordCountEvent % 2 == 0)
				{
					oldChNo = chNo;
					unsigned int data = ReverseBits(rawdata_before);

					flag = (data & 0x1);
					overThresholdFlag = (data & 0x2) >> 1;
					chNo = (data & 0xfc) >> 2;
					//printf("%d: %x %x %x %x\n", wordCountEvent, rawdata_before, data, data & 0xfc, chNo);
					planeID = GetPlaneID(vmmID);
					// Plane 0: x
					// plane 1: y
					oldX = x;
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

					/*
					 if ((adc % 16 != 0 && overThresholdFlag == 1)
					 && (oldVmmID != vmmID || oldChNo != chNo
					 || oldBcid != bcid))
					 */
					//if ((adc % 16 != 0))
					if (true)
					{
						if (oldChipTime > (chipTime + 1000))
						{
							bcTime = (bcid + 4096) * (1 / (double) bcClock);
							chipTime = bcTime * 1000 + tdcTime;
						}
						if (!fViewEvent)
						{
							fRoot->AddHits(unixtimestamp, timestamp_us, eventNr,
									fecID, eventSize, frameCounter, vmmID,
									triggerTimestamp, overThresholdFlag, chNo,
									x, y, adc, tdc, bcid, chipTime);
						}
						if (x > -1)
						{
							hitsX.insert(
									std::make_pair(chipTime,
											std::make_pair(x, adc)));
						}
						if (y > -1)
						{
							hitsY.insert(
									std::make_pair(chipTime,
											std::make_pair(y, adc)));
						}

						if (fViewStart <= eventNr && fViewEnd >= eventNr)
						{

							if (planeID == 0)
							{
								printf(
										"\t\tx-channel %d (chNo  %d) - overThresholdFlag %d\n",
										x, chNo, overThresholdFlag);
							}
							else if (planeID == 1)
							{
								printf(
										"\t\ty-channel %d (chNo  %d) - overThresholdFlag %d\n",
										y, chNo, overThresholdFlag);
							}
							else
							{
								printf("\t\tPlane for vmmID %d not defined!\n",
										vmmID);
							}
							printf("\t\t\tbcid %d, tdc %d, adc %d\n", bcid, tdc,
									adc);
							printf(
									"\t\t\tbcTime %.2f us, tdcTime %.2f ns, time %.2f ns\n",
									bcTime, tdcTime, chipTime);
						}
					}
					else
					{
						discarded++;
					}
				}
				else
				{

					unsigned int data = ReverseBits(rawdata_before);

					//adc: 0-7 14-15
					//tdc: 8-13 22-23
					//bcid: 16-21 26-31
					unsigned int data1 = (data >> 24) & 0xFF;
					unsigned int data2 = (data >> 16) & 0x3;
					unsigned int data3 = (data >> 18) & 0x3F;
					unsigned int data4 = (data >> 8) & 0x3;
					unsigned int data5 = (data >> 10) & 0x3F;
					unsigned int data6 = data & 0x3F;
					oldAdc = adc;
					oldTdc = tdc;
					oldBcid = bcid;
					//10 bits (8+2)
					adc = (data2 << 8) + data1;
					//8 bits (6+2)
					tdc = (data4 << 6) + data3;
					//***********************************************************
					//Bunch crossing clock: 2.5 - 160 MHz (400 ns - 6.25 ns)
					//***********************************************************
					//12 bits (6+6)
					unsigned int gray_bcid = (data6 << 6) + data5;
					bcid = GrayToBinary32(gray_bcid);
					//BC time: bcid value * 1/(clock frequency)
					bcTime = bcid * (1 / (double) bcClock);
					//TDC time: tacSlope * tdc value (8 bit) * ramp length
					tdcTime = tacSlope * (double) tdc / 255;
					//Chip time: bcid plus tdc value
					//Talk Vinnie: HIT time  = BCIDx25 + ADC*125/255 [ns]

					oldChipTime = chipTime;
					chipTime = bcTime * 1000 + tdcTime;

					//if(adc % 16 != 0)
					//if (oldVmmID != vmmID || oldChNo != chNo || oldBcid != bcid)
					//if(adc % 16 != 0)

				}

			}

		}

	}

	return discarded;

}

unsigned int RawdataParser::GetPlaneID(unsigned int chipID)
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

unsigned int RawdataParser::GetChannelX(unsigned int chipID,
		unsigned int channelID, std::string readout)
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

unsigned int RawdataParser::GetChannelY(unsigned int chipID,
		unsigned int channelID, std::string readout)
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

unsigned int RawdataParser::ReverseBits(unsigned int n)
{
	n = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
	n = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
	n = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
	n = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
	n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
	return n;
}

unsigned int RawdataParser::GrayToBinary32(unsigned int num)
{
	num = num ^ (num >> 16);
	num = num ^ (num >> 8);
	num = num ^ (num >> 4);
	num = num ^ (num >> 2);
	num = num ^ (num >> 1);
	return num;
}

int RawdataParser::MapChipChannelToReadout(unsigned int chNo,
		std::string readout)
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

int RawdataParser::MMStripMappingHybrid1(unsigned int chNo)
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
int RawdataParser::MMStripMappingHybrid2(unsigned int chNo)
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

int RawdataParser::MMStripMappingHybrid3(unsigned int chNo)
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

