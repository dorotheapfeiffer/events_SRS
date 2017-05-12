#include <iostream>
#include <fstream>
#include <vector>
#include <map>
//#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <TObject.h>
#include "TString.h"
#include "TCanvas.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"

void plot()
{

	TString pathData = "./reset";
	TSystemDirectory dir(pathData, pathData);
	TList *files = dir.GetListOfFiles();

	if (files)
	{
		TSystemFile *file;
		TString fname;
		TString plotName;
		TIter next(files);

		while ((file = (TSystemFile*) next()))
		{
			fname = file->GetName();
			//&& fname.BeginsWith("run")
			if (!file->IsDirectory() && fname.BeginsWith("run")
					&& fname.EndsWith(".root"))
			{
				double periodNs = 0;
				double TACSlopeNs = 0;
				double chipTime = 0;

				//bcTime = bcid * (1 / (double) bcClock);
				//TDC time: tacSlope * tdc value (8 bit) * ramp length
				//tdcTime = tacSlope * (double) tdc / 255;

				if (fname.Contains("5MHz"))
				{
					periodNs = 200;

				}
				else if (fname.Contains("10MHz"))
				{
					periodNs = 100;
				}
				else if (fname.Contains("20MHz"))
				{
					periodNs = 50;
				}
				else if (fname.Contains("40MHz"))
				{
					periodNs = 25;
				}
				else if (fname.Contains("80MHz"))
				{
					periodNs = 12.5;
				}

				if (fname.Contains("TAC_125ns"))
				{
					TACSlopeNs = 125;
				}
				else if (fname.Contains("TAC_250ns"))
				{
					TACSlopeNs = 250;
				}
				else if (fname.Contains("TAC_500ns"))
				{
					TACSlopeNs = 500;
				}
				else if (fname.Contains("TAC_1000ns"))
				{
					TACSlopeNs = 1000;
				}

				std::cout << "period " << periodNs << std::endl;

				std::cout << fname << std::endl;
				TFile* file = new TFile(pathData + "/" + fname);

				TTree * fHitTree = (TTree*) file->Get("events");

				TH1F *h1 = new TH1F("Time first pulse [us]",
						"Time first pulse [us]", 2000, 0, 200);
				TH1F *h2 = new TH1F("Time second pulse [us]",
						"Time second pulse [us]", 2000, 0, 200);
				TH1F *h3 = new TH1F("Time third pulse [us]",
						"Time third pulse [us]", 2000, 0, 200);

				TH1F *h4 = new TH1F("ADC first pulse", "ADC first pulse", 1024,
						0, 1024);
				TH1F *h5 = new TH1F("ADC second pulse", "ADC second pulse",
						1024, 0, 1024);
				TH1F *h6 = new TH1F("ADC third pulse", "ADC third pulse", 1024,
						0, 1024);
				/*
				 TH1F *h7 = new TH1F(
				 "Time difference between first and second pulse [ns]",
				 "Time difference between first and second pulse [ns]",
				 3000, -150000, 150000);
				 TH1F *h8 =
				 new TH1F(
				 "Time difference between before last and last pulse [ns]",
				 "Time difference between before last and last pulse [ns]",
				 3000, -150000, 150000);
				 TH1F *h9 = new TH1F("Time difference first pulses [ns]",
				 "Time difference first pulses [ns]", 2000, -1000, 1000);
				 TH1F *h1 = new TH1F(
				 "Time difference between first and last pulse [ns]",
				 "Time difference between first and last pulse [ns]",
				 3000, -150000, 150000);
				 */
				TH1F *h7 = new TH1F("BCID first pulse", "BCID first pulse",
						4096, 0, 4095);
				TH1F *h8 = new TH1F("BCID second pulse", "BCID second pulse",
						4096, 0, 4095);
				TH1F *h9 = new TH1F("BCID third pulse", "BCID third pulse",
						4096, 0, 4095);
				unsigned int m_nch;
				unsigned int m_nchX;
				unsigned int m_nchY;

				signed int m_timestamp; 		//Unix time stamp
				unsigned int m_us;
				unsigned int m_eventNr;
				unsigned int m_clusterNr;
				unsigned short m_fecID;        // fecID
				unsigned short m_eventSize;

				UChar_t * m_overThresholdFlag;
				double *m_triggerTimestamp;
				unsigned int *m_frameCounter;
				unsigned short * m_vmmID;        // vmmID
				unsigned short * m_chNo;        // Strip Number
				unsigned short * m_x;
				unsigned short * m_y;

				unsigned short * m_adc;     //ADC value
				unsigned short * m_tdc;     //TDC value
				unsigned short * m_bcid;    //BCID value
				double * m_chipTime;

				const long max_hit = 10000000000;

				//Composed time of BCID and TDC (1 ns resolution)
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

				fHitTree->SetBranchAddress("timestamp", &m_timestamp);
				fHitTree->SetBranchAddress("us", &m_us);
				fHitTree->SetBranchAddress("eventNr", &m_eventNr);
				fHitTree->SetBranchAddress("nch", &m_nch);
				fHitTree->SetBranchAddress("nchX", &m_nchX);
				fHitTree->SetBranchAddress("nchY", &m_nchY);
				fHitTree->SetBranchAddress("fecID", &m_fecID);
				fHitTree->SetBranchAddress("eventSize", &m_eventSize);
				fHitTree->SetBranchAddress("frameCounter", m_frameCounter);
				fHitTree->SetBranchAddress("vmmID", m_vmmID);
				//fHitTree->Branch("triggerCount", m_triggerCount, "triggerCount[nch]/i");
				fHitTree->SetBranchAddress("triggerTimestamp",
						m_triggerTimestamp);
				//fHitTree->Branch("clockCycles", m_clockCycles,
				//			"clockCycles[nch]/i");
				fHitTree->SetBranchAddress("overThresholdFlag",
						m_overThresholdFlag);

				fHitTree->SetBranchAddress("chNo", m_chNo);
				fHitTree->SetBranchAddress("x", m_x);
				fHitTree->SetBranchAddress("y", m_y);
				fHitTree->SetBranchAddress("adc", m_adc);
				fHitTree->SetBranchAddress("tdc", m_tdc);

				fHitTree->SetBranchAddress("bcid", m_bcid);
				//fHitTree->Branch("gray_bcid", m_gray_bcid, "gray_bcid[nch]/s");
				fHitTree->SetBranchAddress("chipTime", m_chipTime);

				fHitTree->SetBranchAddress("clusterNr", &m_clusterNr);

				int eNumEvents = fHitTree->GetEntries();

				int n = 0;
				unsigned int oldEventId = 0;
				unsigned int eventId = 0;
				double time1 = 0;
				double time2 = 0;
				double time3 = 0;
				double timeLast = 0;
				double timeBeforeLast = 0;
				double oldTime1 = 0;
				double oldTime2 = 0;
				double oldBCID1 = 0;
				double oldBCID2 = 0;

				int bcid1 = 0;
				int bcid2 = 0;
				int bcid3 = 0;
				int adc1 = 0;
				int adc2 = 0;
				int adc3 = 0;
				for (int i = 0; i < eNumEvents; i++)
				{
					fHitTree->GetEntry(i);

					oldEventId = eventId;
					eventId = m_eventNr;
					if (m_nch >= 1)
					{

						time1 = m_chipTime[0];
						//time2 = m_chipTime[1];
						//time3 = m_chipTime[2];

						adc1 = m_adc[0];
						//adc2 = m_adc[1];
						//adc3 = m_adc[2];

						bcid1 = m_bcid[0];
						//bcid2 = m_bcid[1];
						//bcid3 = m_bcid[2];

						h1->Fill(time1);
						//h2->Fill(time2);
						//h3->Fill(time3);

						h4->Fill(adc1);
						//h5->Fill(adc2);
						//h6->Fill(adc3);
						h7->Fill(bcid1);
						//h8->Fill(bcid2);
						//h9->Fill(bcid3);

					}
					else if (m_nch >= 3)
					{

						time1 = m_chipTime[0];
						time2 = m_chipTime[1];
						time3 = m_chipTime[2];

						adc1 = m_adc[0];
						adc2 = m_adc[1];
						adc3 = m_adc[2];

						bcid1 = m_bcid[0];
						bcid2 = m_bcid[1];
						bcid3 = m_bcid[2];

						h1->Fill(time1);
						h2->Fill(time2);
						h3->Fill(time3);

						h4->Fill(adc1);
						h5->Fill(adc2);
						h6->Fill(adc3);
						h7->Fill(bcid1);
						h8->Fill(bcid2);
						h9->Fill(bcid3);
						/*
						 //std::cout <<"time1 " <<  time1 << " time2 " << time2 << std::endl;
						 h1->Fill((bcid1 - bcid2) * 25);
						 h4->Fill(m_adc[0]);
						 h5->Fill(m_adc[m_nch - 1]);
						 h2->Fill(0.001 * time1);
						 h3->Fill(0.001 * timeLast);
						 h6->Fill(0.001 * time2);
						 h7->Fill((time1 - time2));
						 h8->Fill((timeBeforeLast - timeLast));
						 h9->Fill((oldBCID1 - bcid1) * 25);
						 h10->Fill(bcid1);
						 h11->Fill(bcid2);
						 */
					}

				}
				TString plotName = "";
				TString theName = fname.ReplaceAll(".root", "_");

				TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
				h1->Draw();
				plotName = pathData + "/" + theName + "Time1.png";
				c1->SaveAs(plotName);

				TCanvas *c2 = new TCanvas("c2", "c2", 600, 400);
				h2->Draw();
				plotName = pathData + "/" + theName + "Time2.png";
				c2->SaveAs(plotName);

				TCanvas *c3 = new TCanvas("c3", "c3", 600, 400);
				h3->Draw();
				plotName = pathData + "/" + theName + "Time3.png";
				c3->SaveAs(plotName);

				TCanvas *c4 = new TCanvas("c4", "c4", 600, 400);
				h4->Draw();
				plotName = pathData + "/" + theName + "ADC1.png";
				c4->SaveAs(plotName);

				TCanvas *c5 = new TCanvas("c5", "c5", 600, 400);
				h5->Draw();
				plotName = pathData + "/" + theName + "ADC2.png";
				c5->SaveAs(plotName);

				TCanvas *c6 = new TCanvas("c6", "c6", 600, 400);
				h6->Draw();
				plotName = pathData + "/" + theName + "ADC3.png";
				c6->SaveAs(plotName);

				TCanvas *c7 = new TCanvas("c7", "c7", 600, 400);
				h7->Draw();
				plotName = pathData + "/" + theName + "BCID1.png";
				c7->SaveAs(plotName);

				TCanvas *c8 = new TCanvas("c8", "c8", 600, 400);
				h8->Draw();
				plotName = pathData + "/" + theName + "BCID2.png";
				c8->SaveAs(plotName);

				TCanvas *c9 = new TCanvas("c9", "c9", 600, 400);
				h9->Draw();
				plotName = pathData + "/" + theName + "BCID3.png";
				c9->SaveAs(plotName);

				/*
				 TCanvas *c10 = new TCanvas("c10", "c10", 600, 400);
				 h10->Draw();
				 plotName = theName + "BCID_1_.png";
				 c10->SaveAs(plotName);

				 TCanvas *c11 = new TCanvas("c11", "c11", 600, 400);
				 h11->Draw();
				 plotName = theName + "BCID_2_.png";
				 c11->SaveAs(plotName);

				 t->Draw("adc", "");
				 TString theName = fname.ReplaceAll(".root","_");
				 plotName = theName + "adc.png";
				 c->SaveAs(plotName);
				 t->Draw("x", "");
				 plotName= theName + "x.png";
				 c->SaveAs(plotName);
				 t->Draw("tdc", "");
				 plotName = theName + "tdc.png";
				 c->SaveAs(plotName);
				 t->Draw("bcid", "");
				 plotName = theName + "bcid.png";
				 c->SaveAs(plotName);
				 t->Draw("chipTime", "");
				 plotName = theName + "chipTime.png";
				 c->SaveAs(plotName);
				 t->Draw("overThresholdFlag", "");
				 plotName = theName + "overThresholdFlag.png";
				 c->SaveAs(plotName);
				 */
			}
		}
	}
}
