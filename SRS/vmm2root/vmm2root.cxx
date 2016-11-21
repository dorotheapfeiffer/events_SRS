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

unsigned int numEvents = 0;
unsigned int validEvents = 0;
std::string fileName = "";
unsigned long counter = 0;
int eventNr = 0;
int vStart = 0;
int vEnd = 0;
int limit = 0;

bool rdFound = false;
bool nFound = false;
bool vFound = false;

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
		else if (strncmp(argv[i], "-n", 2) == 0)
		{
			nFound = true;
			numEvents = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-v", 2) == 0)
		{
			vFound = true;
			std::string viewEvents = argv[i + 1];
			int pos = viewEvents.find("-");
			if(pos !=std::string::npos)
			{
				std::string sStart = viewEvents.substr(0,pos);
				std::string sEnd = viewEvents.substr(pos+1);
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

	eventNr = 0;
	counter = 0;
	in = fopen(fileName.c_str(), "rb");
	if (!in)
	{
		std::stringstream message;
		message << "File " << fileName << " does not exist!" << std::endl;
		return printUsage(message.str());
	}
	parser = new RawdataParser(fileName, vFound, vStart, vEnd);

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

		printf("%ld kB, %ld words read from library in %d s.\n",
				(long int) (counter * 4 / 1024), counter, duration);
		printf("Events %d\n\n", eventNr);

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
	printf("analyse raw data:\n\traw2root -rd data.raw [-n events -v start-end]\n");

	printf("\nFlags:\n");
	printf(
			"-rd: raw data file with the extension .raw\n\tThe data file was created by DATE.\n");
	printf(
			"-n: number of events to analyze. Optional argument.\n\tIf not used, all events in the file will be analyzed.\n\n");
	printf(
				"-v: events to display. Optional argument.\n\tIf not used, a root tree will be created.\n\n");
	return -1;
}

