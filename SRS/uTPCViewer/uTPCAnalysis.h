#ifndef ANALYSIS_CLASS
#define ANALYSIS_CLASS 1

#define SIZE 2000

#include <vector>
#include <algorithm>
#include <TString.h>
#include <TTree.h>
#include <TFile.h>

class uTPCViewer;
//class uTPCHit;
class uTPCEvent;

class uTPCPlotData: public TObject
{
public:
	uTPCPlotData();
	~uTPCPlotData();
	void clearDataStructures();
	std::vector<unsigned int> fPointClustersX;
	std::vector<unsigned int> fPointClustersY;

	std::vector<double> fEnergyX;
	std::vector<double> fEnergyY;
	std::vector<unsigned int> fEnergyMaximaX;
	std::vector<unsigned int> fEnergyMaximaY;
	std::vector<unsigned int> fStripMaximaX;
	std::vector<unsigned int> fStripMaximaY;
	std::vector<unsigned int> fTimeMaximaX;
	std::vector<unsigned int> fTimeMaximaY;

	std::vector<double> fXEigenvectorX;
	std::vector<double> fXEigenvectorY;
	std::vector<double> fXNormalX;
	std::vector<double> fXNormalY;

	std::vector<double> fYEigenvectorX;
	std::vector<double> fYEigenvectorY;
	std::vector<double> fYNormalX;
	std::vector<double> fYNormalY;

	double fThresholdAmplitude;
	double fThresholdDiscard;
};

class uTPCRawData: public TObject
{
public:
	uTPCRawData();
	virtual ~uTPCRawData();
	//int getNumberOfWaveforms();
	int getNumberOfStripsHit();
	//std::vector<double> getWaveform(unsigned int);
	int getStripNumber(int);
	int getWaveformSize( int);
	double getWaveformAtTimeBin( int strip,  int timeBin);
	int getStripId( int plane,  int stripNumber);
	 int getNumber();
	int getThresholdAmplitude()
	{
		return fThresholdAmplitude;
	}

	int getThresholdDiscard()
	{
		return fThresholdDiscard;
	}


	int getPlaneId(int);
	int importData(TString dir, TString fileName, Bool_t analyze, Bool_t fit,
			uTPCPlotData* plotData, uTPCViewer* mainFrame = 0, int num = 0);
	void selectEvent(unsigned int eventNr, int thresholdAmplitude,
			int thresholdDistance, int thresholdPCA, int thresholdGamma,
			int thresholdDiscard);

	TTree* getInputTree()
		{
			return treeIn;
		}
		;
	void analyzeEvent(uTPCPlotData *plotData);
	uTPCEvent* getEvent()
	{
		return event;
	}
	//void smooth(int, int);
	//void dump();
	//void calculateScalingFactor();
	std::map<unsigned int, unsigned int>& getClusterX()
	{
		return fClusterX;
	}
	std::map<unsigned int, unsigned int>& getClusterY()
	{
		return fClusterY;
	}
	void setClusterX(std::map<unsigned int, unsigned int>& map)
	{
		fClusterX = map;
	}
	void setClusterY(std::map<unsigned int, unsigned int>& map)
	{
		fClusterY = map;
	}
	double getScaleX()
	{
		return fScaleX;
	}
	double getScaleY()
	{
		return fScaleY;
	}

	void setMaxX(float maxX)
	{
		fMaxX = maxX;
	}
	void setMaxY(float maxY)
	{
		fMaxY = maxY;
	}
	void setMinX(float minX)
	{
		fMinX = minX;
	}
	void setMinY(float minY)
	{
		fMinY = minY;
	}

	double getMaxX()
	{
		return fMaxX;
	}
	double getMaxY()
	{
		return fMaxY;
	}
	double getMinX()
	{
		return fMinX;
	}
	double getMinY()
	{
		return fMinY;
	}

	ClassDef(uTPCRawData,0)
private:
	TFile * fileIn;
	TTree * treeIn;
	uTPCEvent* event;
	int pNCH;
	//std::vector<std::vector<double> >* pWaveform;
	short pADC0[SIZE];
	short pADC1[SIZE];
	short pADC2[SIZE];
	short pADC3[SIZE];
	short pADC4[SIZE];
	short pADC5[SIZE];
	short pADC6[SIZE];
	short pADC7[SIZE];
	short pADC8[SIZE];
	short pADC9[SIZE];
	short pADC10[SIZE];
	short pADC11[SIZE];
	short pADC12[SIZE];
	short pADC13[SIZE];
	short pADC14[SIZE];
	short pADC15[SIZE];
	short pADC16[SIZE];
	short pADC17[SIZE];
	short pADC18[SIZE];
	short pADC19[SIZE];
	short pADC20[SIZE];
	short pADC21[SIZE];
	short pADC22[SIZE];
	short pADC23[SIZE];
	short pADC24[SIZE];
	short pADC25[SIZE];
	short pADC26[SIZE];
	short pADC27[SIZE];
	short pADC28[SIZE];
	short pADC29[SIZE];
	int pStripNumber[SIZE];
	//std::vector<TString>* pPlaneName;
	int pPlaneId[SIZE];
	std::map<unsigned int, unsigned int> fClusterX;
	std::map<unsigned int, unsigned int> fClusterY;
	double fScaleX;
	double fScaleY;
	double fMaxX;
	double fMaxY;
	double fMinX;
	double fMinY;
	unsigned int fNcx;
	unsigned int fNcy;
	unsigned int pNumber;
	int fThresholdAmplitude;
	int fThresholdDiscard;
};

class uTPCEvent
{
public:
	uTPCEvent();
	virtual ~uTPCEvent();
	void initializeDataStructures();
	void freeDataStructures();
	//uTPCHit getHit(int = 0);
	void analyseEvent(uTPCRawData* rawData, uTPCPlotData *plotData);
	//void analyseEventPCA(uTPCRawData* rawData, uTPCPlotData *plotData);
	//void dump();
	void setEventId(int id)
	{
		eventId = id;
	}





	int getClusterStripStartX()
	{
		return xs;
	}

	int getClusterStripStartX2()
	{
		return xmax;
	}


	int getClusterTimeStartX()
	{
		return txs;
	}

	int getClusterTimeStartX2()
	{
		return txmax;
	}

	int getClusterStripStartY()
	{
		return ys;
	}

	int getClusterTimeStartY()
	{
		return tys;
	}

	int getClusterStripStartY2()
	{
		return ymax;
	}

	int getClusterTimeStartY2()
	{
		return tymax;
	}

private:
	std::vector<unsigned int>* pStripNumber;
	std::vector<unsigned int>* pPlaneNumber;

public:


	double x0, y0, xf, yf, xm, ym, xi, yi, xs, ys, xd, yd, xmax, ymax;
	double xAmplitude, xIntegral, yAmplitude, yIntegral;
	double maxAmplitudeX, maxAmplitudeY;
	double tMaxAmplitudeX, tMaxAmplitudeY;
	double tx0, ty0, txf, tyf, txs, tys, txd, tyd, txmax, tymax;
	double ex0, ey0, exf, eyf, exs, eys, exd, eyd, exmax, eymax;

	int nx, ny, ntx, nty;

	int eventId;

};

#endif

