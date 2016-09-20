#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <TObject.h>
#include "TString.h"

#define NFEC 4
#define NAPV 16
#define MAXCHANNELHIT 1000

using namespace std;

class RootFile: public TObject {
    
public:
    
	RootFile(TString fileName);
    ~RootFile();

    void InitRootFile() ;
    void WriteRootFile() ;
    void AddHits(signed int timestamp, unsigned int us,
    		unsigned int eventNr, unsigned short fecID, unsigned short eventSize, unsigned short vmmID,
    		unsigned int triggerCount, unsigned int triggerTimestamp,UChar_t overThresholdFlag,
    		unsigned short chNo, short adc, short tdc, short bcid);
	void FillHits() ;

    void DeleteHitsTree() ;


private:
   
    TFile * fFile;
    TTree * fHitTree;
    TString fFileName;

    unsigned short m_nch;

    signed int m_timestamp; 		//Unix time stamp
    unsigned int m_us;
    unsigned int m_eventNr;
    unsigned short m_fecID;        // fecID
    unsigned short m_eventSize;


    UChar_t * m_overThresholdFlag;
    unsigned int *m_triggerCount;
    unsigned int *m_triggerTimestamp;
    
    unsigned short * m_vmmID;        // vmmID
    unsigned short * m_chNo;        // Strip Number

    short * m_adc;     //ADC value
    short * m_tdc;     //TDC value
    short * m_bcid;    //BCID value



    ClassDef(RootFile,1)
};



