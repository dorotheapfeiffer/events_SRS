#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <TROOT.h>
#include <TApplication.h>
#include "MainWindow.h"
#include "Files.h"


//--------------------------------------------------------------------------

static const char *optString = "d:f:vh?";

//--------------------------------------------------------------------------

static const struct option longOpts[] = {
	{ "dir",	required_argument,	NULL,	'd' },
	{ "files",	required_argument,	NULL,	'f' },
	{ "version",	no_argument,		NULL,	'v' },
	{ "help",	no_argument,		NULL,	'h' },
	{ "randomize",	no_argument,		NULL,  	 0  },
	{  NULL,	no_argument,		NULL,	 0  }
};

//--------------------------------------------------------------------------

using namespace std;

string trim(string const& source, char const* delims  = " \t\r\n");
void display_usage( void ); 
void execute( void );  // this function is only temporary....


int main( int argc, char *argv[] )
{
	int opt = 0;
	int longIndex = 0;

	/* Process the arguments with getopt_long(), then 
	 * populate aFiles. 
	 */
	opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	while( opt != -1 ) {
         switch( opt ) {
	    case 'd':
            //Files::getInstance().SetDirectory(optarg);
            cerr << "directory " << optarg  << endl;
	    break;
	
            case 'f':
              {
              ifstream fin( optarg , ios::in);
              if(fin.is_open()){
                 string line;
                 while(getline(fin,line)){
                    if(! line.length()) continue;
                    if(line[0]=='\r')   continue;
                    Files::getInstance().AddFile(trim(line));
                    }
                 fin.close();
                 }
              else{
                 cerr << "the file " << optarg << " doesn't exist!" << endl;
               }
              }
	    break;
				
	    case 'v':
	       cout << "exdna version = " << Files::getInstance().GetVersion() << endl;
	    break;
				
	    case 'h':	
	    case '?':
	    display_usage();
	    break;

	    case 0:
                cout << "long option without a short arg if necessary..." << endl;
		if( strcmp( "randomize", longOpts[longIndex].name ) == 0 ) {
	    	  //aFiles.randomized = 1;
		  }
	    break;
				
	    default:
		/* You won't actually get here. */
	    break;
	    }
		
          opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	}

        for(int i = 0; i < argc - optind; i++){
           Files::getInstance().AddFile(argv[optind + i]);
           } 	
	
 
	execute();
	TApplication theApp("App",&argc,argv);
	MainWindow mMainWindow(gClient->GetRoot(),600,400);

	if( Files::getInstance().GetSize() ){
		mMainWindow.SetCurrentFile(Files::getInstance().GetFile(0).Data()); 
		mMainWindow.ReadData();
		}
	//else if () // for net, not implemented yet
	//else       // just in case

	theApp.Run();
	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------

string trim(string const& source, char const* delims) {
 string result(source);
 string::size_type index = result.find_last_not_of(delims);

 if(index != string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}

//-----------------------------------------------------------------------

void display_usage( void ) {
    cout << "usage: display [OPTION]... FILES..." << endl;
    cout << "  Start data analysis application" << endl; 
    cout << endl;
    cout << "Mandatory arguments to long options are mandatory for short options too" << endl;
    cout << "  -d[dir],	--dir[dir]	\tset directory where display will start looking for data files" << endl;
    cout << "  -f[name],	--files[name]	\tload file with files to analyse" << endl;
    cout << "  		--help		\tdisplay this help message and exit" <<endl;
    cout << "  		--version	\toutput version information and exit" << endl;
    cout << "  example1:"<<endl;
    cout << "     ./display -d/np/vdg/data \n" <<endl;
    cout << "  example2:"<<endl;
    cout << "     ./display data1.dat data2.dat data3.dat" <<endl;
    cout << "  example3:"<<endl;
    cout << "     ./display -f list_data_file " <<endl;

    exit( EXIT_FAILURE );
}

//-----------------------------------------------------------------------

void execute( void ) {
	printf( "Files to analise: %d\n", Files::getInstance().GetSize());
        for(Int_t i = 0; i < Files::getInstance().GetSize(); i++){
	    cout << "\t "<< i+1 << "/" << Files::getInstance().GetSize() << " " << Files::getInstance().GetFile(i) << endl;
            }
  
	//printf( "Options:\n");
	//printf( "dir:\t%s\n",        Files::getInstance().GetDirectory().Data() );
	//printf( "version:\t%d\n",    Files::getInstance().GetVersion()  );
}

//-----------------------------------------------------------------------
