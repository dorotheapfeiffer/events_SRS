#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "RootFile.h"

int n;
RootFile *fRoot = 0;

unsigned int validEvents = 0;
std::string fileName = "";
unsigned long counter = 0;
int discarded = 0;
int vStart = 0;
int vEnd = 0;
int limit = 0;
int threshold = 0;
int clusterSize = 3;
double tac = 125;
double bc = 40;
std::vector<int> xChips;
std::vector<int> yChips;
std::string readout = "GEM";

bool dFound = false;
bool vFound = false;
bool tacFound = false;
bool bcFound = false;
bool xFound = false;
bool yFound = false;
bool readoutFound = false;
bool thresholdFound = false;
bool clusterSizeFound = false;

int printUsage(std::string errorMessage);

int main(int argc, char**argv)
{

	if (argc == 1 || argc % 2 == 0)
	{
		return printUsage("Wrong number of arguments!");
	}
	for (int i = 1; i < argc; i += 2)
	{
		if (strncmp(argv[i], "-d", 2) == 0)
		{
			dFound = true;
			fileName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-tac", 4) == 0)
		{
			tacFound = true;
			tac = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-bc", 3) == 0)
		{
			bcFound = true;
			bc = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-x", 2) == 0)
		{
			xFound = true;
			std::string xString = argv[i + 1];
			std::string delims = ",";
			size_t lastOffset = 0;

			while (true)
			{
				size_t offset = xString.find_first_of(delims, lastOffset);
				xChips.push_back(
						atoi(
								xString.substr(lastOffset, offset - lastOffset).c_str()));
				if (offset == std::string::npos)
				{
					break;
				}
				else
				{
					lastOffset = offset + 1; // add one to skip the delimiter
				}
			}
			//std::cout << "x1 " << xChips[0] << std::endl;
			//	std::cout << "x2 " << xChips[1] << std::endl;

		}
		else if (strncmp(argv[i], "-y", 2) == 0)
		{
			yFound = true;
			std::string yString = argv[i + 1];
			std::string delims = ",";
			size_t lastOffset = 0;

			while (true)
			{
				size_t offset = yString.find_first_of(delims, lastOffset);
				yChips.push_back(
						atoi(
								yString.substr(lastOffset, offset - lastOffset).c_str()));
				if (offset == std::string::npos)
				{
					break;
				}
				else
				{
					lastOffset = offset + 1; // add one to skip the delimiter
				}
			}

		}
		else if (strncmp(argv[i], "-v", 2) == 0)
		{
			vFound = true;
			std::string viewEvents = argv[i + 1];
			int pos = viewEvents.find("-");
			if (pos != std::string::npos)
			{
				std::string sStart = viewEvents.substr(0, pos);
				std::string sEnd = viewEvents.substr(pos + 1);
				//std::cout << sStart << "-" << sEnd << std::endl;
				vStart = atof(sStart.c_str());
				vEnd = atof(sEnd.c_str());

			}
			else
			{
				return printUsage("Wrong usage of -v start-end!");
			}
		}
		else if (strncmp(argv[i], "-readout", 8) == 0)
		{
			readoutFound = true;
			readout = argv[i + 1];
			if (readout != "MM1" && readout != "MM2" && readout != "MM3"
					&& readout != "mm1" && readout != "mm2" && readout != "mm3"
					&& readout != "GEM" && readout != "gem")
			{
				return printUsage(
						"Wrong type of readout: MM1, MM2, MM3 or GEM are valid!");
			}
		}
		else if (strncmp(argv[i], "-th", 2) == 0)
		{
			thresholdFound = true;
			threshold = atoi(argv[i + 1]);

		}
		else if (strncmp(argv[i], "-cs", 2) == 0)
		{
			clusterSizeFound = true;
			clusterSize = atoi(argv[i + 1]);

		}
		else
		{
			return printUsage("Wrong type of argument!");
		}
	}

	if (!dFound)
	{
		return printUsage("Data file has to be loaded with -d data.csv!");

	}

	if (dFound && fileName.find(".csv") == std::string::npos)
	{
		return printUsage("Wrong extension: .csv file required!");
	}

	counter = 0;

	if (dFound)
	{
		ifstream inFile(fileName.c_str());
		if (!inFile)
		{
			std::stringstream message;
			message << "File " << fileName << " does not exist!" << std::endl;
			return printUsage(message.str());
		}


		std::stringstream ending;
		ending << ".root";
		fileName.replace(fileName.size() - 4, fileName.size(), ending.str());

		fRoot = new RootFile(fileName, bc, tac, xChips, yChips, readout,
				vFound, vStart, vEnd, threshold, clusterSize);

		int fec, chip_id, channel, bcid, tdc, adc, overthreshold;
		long srs_timestamp;
		int i = 0;

		if (inFile.is_open())
		{

			std::string line;
			while (inFile.good() && std::getline(inFile, line))
			{
				i++;
				if (i > 2)
				{
					std::stringstream iss(line);
					char ch;
					iss >> fec;
					iss >> ch;
					iss >> chip_id;
					iss >> ch;
					iss >> srs_timestamp;
					iss >> ch;
					iss >> channel;
					iss >> ch;
					iss >> bcid;
					iss >> ch;
					iss >> tdc;
					iss >> ch;
					iss >> adc;
					iss >> ch;
					iss >> overthreshold;

					int result = fRoot->AnalyzeData(srs_timestamp, fec, chip_id, channel, bcid, tdc, adc, overthreshold);
					if(result == -1)
					{
						break;
					}
				}
			}
			inFile.close();
			fRoot->SaveClusters();
		}

	}

	delete fRoot;

	return 0;

}

int printUsage(std::string errorMessage)
{
	std::cout << "\nERROR: " << errorMessage << std::endl;

	printf("\nUsages:\n");
	printf(
			"analyse raw data:\n\traw2root -rd data.raw -x 0,1,2,3 -y 4,5,6,7 [-n events -v start-end -bc MHz - tac ns -readout MM1] \n");

	printf("\nFlags:\n");
	printf(
			"-d: csv data file with the extension .csv\n\tThe data file was created by ESS tool.\n");
	printf(
			"-rd: raw data file with the extension .raw\n\tThe data file was created by DATE.\n");
	printf(
			"-x: mapping of chips, list of chips in x direction separated by comma\n\n");
	printf(
			"-y: mapping of chips, list of chips in y direction separated by comma\n\n");
	printf(
			"-n: number of events to analyze. Optional argument.\n\tIf not used, all events in the file will be analyzed.\n\n");
	printf(
			"-v: events to display. Optional argument.\n\tIf not used, a root tree will be created.\n\n");
	printf("-bc: bunch crossing clock. Optional argument (default 40 MHz)\n\n");
	printf("-tac: tac slope. Optional argument (default 125 ns)\n\n");
	printf(
			"-readout: type of readout board. Optional argument (default GEM)\n\n");
	printf(
			"-th: threshold value in ADC counts. Optional argument (default 0)\n\n");
	printf("-cs: minimum cluster size. Optional argument (default 3)\n\n");

	return -1;
}

