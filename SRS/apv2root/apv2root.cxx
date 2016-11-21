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

bool isRawPedestal = false;
bool isPedestal = false;
bool isZS = false;
unsigned int numEvents = 0;
std::string fileName = "";
std::string pedestalName = "";
unsigned long counter = 0;
int eventNr = 0;
int uTPCThreshold = 0;
int limit = 0;
float zsCut = 0;

bool pFound = false;
bool rdFound = false;
bool rpFound = false;
bool nFound = false;
bool cFound = false;
bool uFound = false;

int printUsage(std::string errorMessage);

int main(int argc, char**argv)
{

	for (int i = 1; i < argc; i+=2)
	{
		if (strncmp(argv[i], "-rd", 3) == 0)
		{
			rdFound = true;
			fileName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-rp", 3) == 0)
		{
			rpFound = true;
			isRawPedestal = true;
			fileName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-p", 2) == 0)
		{
			pFound = true;
			pedestalName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-n", 2) == 0)
		{
			nFound = true;
			numEvents = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-c", 2) == 0)
		{
			cFound = true;
			zsCut = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-utpc", 5) == 0)
		{
			uFound = true;
			uTPCThreshold = atof(argv[i + 1]);

		}
		else
		{
			return printUsage("Wrong type of argument!");
		}
	}
	if (argc == 1 || argc % 2 == 0)
	{
		return printUsage("Wrong number of arguments!");
	}
	if (rpFound && pFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if (rdFound && rpFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if (rpFound && (cFound || uFound))
	{
		return printUsage("Wrong combination of arguments!");

	}
	if ((!rdFound || !pFound) && cFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if ((rdFound && pFound) && !cFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if ((cFound && zsCut == 0) || (nFound && numEvents == 0))
	{
		return printUsage("Wrong combination of arguments!");
	}

	if (fileName.find(".raw") == std::string::npos)
	{
		return printUsage("Wrong extension: .raw file required!");
	}
	if (pFound && pedestalName.find(".root") == std::string::npos)
	{
		return printUsage("Wrong extension: .root file required!");
	}
	if (rdFound && !pFound)
	{
		isZS = true;
	}

	time_t start = time(0);

	if (isRawPedestal)
	{
		limit = 2;
	}
	else
	{
		limit = 1;
	}
	RawdataParser *parser = 0;
	for (int z = 0; z < limit; z++)
	{
		eventNr = 0;
		counter = 0;
		in = fopen(fileName.c_str(), "rb");
		if (!in)
		{
			std::stringstream message;
			message << "File " << fileName << " does not exist!" << std::endl;
			return printUsage(message.str());
		}
		if (z == 0)
		{
			parser = new RawdataParser(fileName, pedestalName, isRawPedestal,
					isPedestal, isZS, zsCut, uFound, uTPCThreshold);

		}
		else
		{
			isPedestal = true;
			isRawPedestal = false;
			parser->SetRunFlags(isRawPedestal, isPedestal);
		}

		if (in)
		{
			while (!feof(in) && (eventNr < numEvents || numEvents == 0))
			{

				rawdata_before_two = rawdata_before;
				rawdata_before = rawdata;
				n = fread(&rawdata, sizeof(unsigned int), BUFFER_SIZE_INT, in);
				counter += n;
				eventNr = parser->AnalyzeWord(eventNr, rawdata, rawdata_before,
						rawdata_before_two);
				if (eventNr < 0)
				{
					fclose(in);
					delete parser;
					return -1;
				}

			}

			time_t end = time(0);
			unsigned int duration = difftime(end, start);

			printf("\n%ld kB, %ld words read from library in %d s.\n",
					(long int) (counter * 4 / 1024), counter, duration);
			if (isRawPedestal)
			{
				printf("RawPedestal Events %d\n\n", eventNr);
			}
			else if (isPedestal)
			{
				printf("Pedestal Events %d\n\n", eventNr);
			}
			else
			{
				limit = 0;
				printf("Events %d\n", eventNr);
			}
			fclose(in);

		}
		else
		{
			printf("fail\n");
		}
		parser->CreateHistograms();

	}

	delete parser;

	return 0;

}

int printUsage(std::string errorMessage)
{
	std::cout << "\nERROR: " << errorMessage << std::endl;

	printf("\nUsages:\n");
	printf(
			"1a) analyse non zero-suppressed pedestal data:\n\traw2root -rp pedestal.raw [-n events]\n");
	printf(
			"1b) analyse non zero-suppressed data and subtract pedestal:\n\traw2root -rd data.raw -r pedestal.root -c ZSCut [-n events] [-utpc]\n");
	printf(
			"2) analyse zero-suppressed data:\n\traw2root -rd data.raw [-n events]  [-utpc]\n");

	printf("\nFlags:\n");
	printf(
			"-rd: raw data file with the extension .raw\n\tUsed for non-zero suppressed and zero suppressed data\n\tThe data file was created by DATE.\n");
	printf(
			"-rp: raw pedestal file with the extension .raw\n\tUsed only for non-zero suppressed data.\n\tThe pedestal file was created by DATE.\n");
	printf(
			"-p: pedestal root file with the extension .root\n\tUsed only for non-zero suppressed data.\n\tThis file has to be created with raw2root in step 1a)\n");
	printf(
			"-c: zero-supression cut for non-zero supressed data.\n\tA threshold is calculated by multiplying the pedestal noise with the cut value for each strip.\n\t"
					"Per event it is checked whether the mean strip ADC is over the threshold, in which case the strip is included in the data.\n");
	printf(
			"-n: number of events to analyze. Optional argument.\n\tIf not used, all events in the file will be analyzed.\n");
	printf("-utpc: Add uTPC analysis. Optional argument.\n\n");

	return -1;
}

