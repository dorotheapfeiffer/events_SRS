#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "RawdataParser.h"

const int BUFFER_SIZE_INT = 1;

FILE *in;
FILE *out;
int n;

unsigned int rawdata_before_two = 0;
unsigned int rawdata_before = 0;
unsigned int rawdata = 0;


unsigned int validEvents = 0;
std::string fileName = "";
unsigned long counter = 0;
int discarded = 0;
int vStart = 0;
int vEnd = 0;
int limit = 0;
double tac = 125;
double bc = 40;
std::vector<int> xChips;
std::vector<int> yChips;

bool rdFound = false;

bool vFound = false;
bool tacFound = false;
bool bcFound = false;
bool xFound = false;
bool yFound = false;

int printUsage(std::string errorMessage);

int main(int argc, char**argv)
{

	if (argc == 1 || argc % 2 == 0)
	{
		return printUsage("Wrong number of arguments!");
	}
	for (int i = 1; i < argc; i += 2)
	{
		if (strncmp(argv[i], "-rd", 3) == 0)
		{
			rdFound = true;
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

		else
		{
			return printUsage("Wrong type of argument!");
		}
	}

	if (!rdFound)
	{
		return printUsage("Data file has to be loaded with -rd data.raw!");

	}

	if (fileName.find(".raw") == std::string::npos)
	{
		return printUsage("Wrong extension: .raw file required!");
	}

	time_t start = time(0);

	RawdataParser *parser = 0;

	counter = 0;
	in = fopen(fileName.c_str(), "rb");
	if (!in)
	{
		std::stringstream message;
		message << "File " << fileName << " does not exist!" << std::endl;
		return printUsage(message.str());
	}
	parser = new RawdataParser(fileName, bc, tac, xChips, yChips, vFound,
			vStart, vEnd);

	if (in)
	{
		while (!feof(in))
		{

			rawdata_before_two = rawdata_before;
			rawdata_before = rawdata;
			n = fread(&rawdata, sizeof(unsigned int), BUFFER_SIZE_INT, in);
			counter += n;
			discarded = parser->AnalyzeWord(rawdata, rawdata_before,
					rawdata_before_two);

			if (discarded < 0)
			{
				fclose(in);
				delete parser;
				return -1;
			}

		}

		time_t end = time(0);
		unsigned int duration = difftime(end, start);

		printf("%ld kB, %ld words read from library in %d s.\n",
				(long int) (counter * 4 / 1024), counter, duration);
		printf("Discarded events %d\n\n", discarded);

		fclose(in);

	}
	else
	{
		printf("fail\n");
	}

	delete parser;

	return 0;

}

int printUsage(std::string errorMessage)
{
	std::cout << "\nERROR: " << errorMessage << std::endl;

	printf("\nUsages:\n");
	printf(
			"analyse raw data:\n\traw2root -rd data.raw -x 0,1,2,3 -y 4,5,6,7 [-n events -v start-end -bc MHz - tac ns] \n");

	printf("\nFlags:\n");
	printf(
			"-rd: raw data file with the extension .raw\n\tThe data file was created by DATE.\n");
	printf("-x: mapping of chips, list of chips in x direction separated by comma\n\n");
	printf("-y: mapping of chips, list of chips in y direction separated by comma\n\n");
	printf(
			"-n: number of events to analyze. Optional argument.\n\tIf not used, all events in the file will be analyzed.\n\n");
	printf(
			"-v: events to display. Optional argument.\n\tIf not used, a root tree will be created.\n\n");
	printf("-bc: bunch crossing clock. Optional argument (default 40 MHz)\n\n");
	printf("-tac: tac slope. Optional argument (default 125 ns)\n\n");

	return -1;
}

