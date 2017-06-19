///////////////////////////////////////////////////////////////////////////////
// DGScalerViewer is a CERES scaler history viewer. 
///////////////////////////////////////////////////////////////////////////////
//
//

//

#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#include "DGScalerViewer.h"

  ClassImp(DGScalerViewer)

char *chna[32] = {(char*)"BC1",(char*)"BC2",(char*)"BC3",(char*)"MD",(char*)"MDlow",(char*)"VC",(char*)"VW",(char*)"res7",
		  (char*)"beam_bp",(char*)"minb_bp",(char*)"cent_bp",(char*)"beam",(char*)"minb",(char*)"cent",(char*)"ab",(char*)"ai",
		  (char*)"FLT",(char*)"cFLT",(char*)"SDDa",(char*)"bapa",(char*)"iapa",(char*)"abort",(char*)"SLT",(char*)"reset",
		  (char*)"res24",(char*)"10MHz",(char*)"50Hz",(char*)"res27",(char*)"res28",(char*)"res29",(char*)"res30",(char*)"res31"};

/*****************************************************************************/
 DGScalerViewer::DGScalerViewer(Char_t* filename) : 
  TGMainFrame(gClient->GetRoot(),0,0) {
  Int_t i, j;
  TGLayoutHints *fL0, *fL1, *fL10;

  fMemList = new TObjArray();

  fL0 = new TGLayoutHints(kLHintsLeft|kLHintsCenterY|kLHintsExpandY,5,5,5,5);
  fL1 = new TGLayoutHints(kLHintsRight|kLHintsCenterY|kLHintsExpandY,5,5,5,5);
  fL10 = new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandX,2,2,2,2);

  // main frame

  fLabelFrame   = new TGHorizontalFrame(this, 0, 0);
  fCanvasFrame  = new TRootEmbeddedCanvas("c", this, 800, 300);
  fControlFrame = new TGHorizontalFrame(this, 0, 0);
  AddFrame(fLabelFrame, fL10);
  AddFrame(fCanvasFrame, fL10);
  AddFrame(fControlFrame, fL10);
  fMemList->Add(fLabelFrame);
  fMemList->Add(fCanvasFrame);
  fMemList->Add(fControlFrame);

  // label frame

  char tip[1000];
  char *atip = tip;
  for (i=0; i<32; i++) {
    if (strcmp(chna[i],"res") < 0) {
      j = sprintf(atip, "%s ", chna[i]);
      atip += j;
    }
  }
  *(atip-1) = 0;
  fLabel = new TGLabel(fLabelFrame, new TGString(tip));
  fLabelFrame->AddFrame(fLabel,fL0);
  fMemList->Add(fLabel);

  // canvas frame

  fCanvasFrame->GetContainer()->ChangeOptions(kFixedWidth);
  fCanvasFrame->GetCanvas()->SetEditable(kFALSE);
  fCanvasFrame->GetCanvas()->SetFillColor(18);
  fCanvasFrame->GetCanvas()->SetGrid();
  
  // control frame

  fSelectionFrame = new TGGroupFrame(fControlFrame, "selection");
  fRangeFrame     = new TGGroupFrame(fControlFrame, "range");
  //  fZoomFrame      = new TGGroupFrame(fControlFrame, "zoom");
  fWindowFrame     = new TGGroupFrame(fControlFrame, "window");
  fControlFrame->AddFrame(fSelectionFrame, fL0);
  fControlFrame->AddFrame(fRangeFrame, fL0);
  //  fControlFrame->AddFrame(fZoomFrame, fL0);
  fControlFrame->AddFrame(fWindowFrame, fL1);
  fMemList->Add(fSelectionFrame);
  fMemList->Add(fRangeFrame);
  fMemList->Add(fZoomFrame);
  fMemList->Add(fWindowFrame);

  // control-selection

  for (i=0; i<4; i++) {
    fSelectionB[i] = new TGTextBuffer(80);
    fSelectionE[i] = new TGTextEntry(fSelectionFrame, fSelectionB[i], 13000+i);
    fSelectionE[i]->Resize(120,fSelectionE[i]->GetDefaultHeight());
    fSelectionFrame->AddFrame(fSelectionE[i], fL0);
    fSelectionE[i]->Associate(this);
    fMemList->Add(fSelectionE[i]);
  }

  // control-range

  fRangeFrame->SetLayoutManager(new TGMatrixLayout(fRangeFrame,4,0,10));
  fRangeExpandButton = new TGTextButton(fRangeFrame," 2 x",13011);
  fRangeShrinkButton = new TGTextButton(fRangeFrame," 0.5 x",13012);
  fRangeLeftButton   = new TGTextButton(fRangeFrame,"<-----",13013);
  fRangeRightButton  = new TGTextButton(fRangeFrame,"----->",13014);
  fLastHourButton  = new TGTextButton(fRangeFrame, "last hour",  13015);
  fLastDayButton   = new TGTextButton(fRangeFrame, "last day ",  13016);
  fLastWeekButton  = new TGTextButton(fRangeFrame, "last week",  13017);
  fLastMonthButton = new TGTextButton(fRangeFrame, "last month", 13018);

  fRangeExpandButton->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fRangeShrinkButton->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fRangeLeftButton  ->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fRangeRightButton ->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fLastHourButton   ->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fLastDayButton    ->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fLastWeekButton   ->Resize(65, fRangeExpandButton->GetDefaultHeight());
  fLastMonthButton  ->Resize(65, fRangeExpandButton->GetDefaultHeight());
  
  fRangeFrame->AddFrame(fRangeExpandButton);
  fRangeFrame->AddFrame(fRangeShrinkButton);
  fRangeFrame->AddFrame(fRangeLeftButton);
  fRangeFrame->AddFrame(fRangeRightButton);
  fRangeFrame->AddFrame(fLastHourButton);
  fRangeFrame->AddFrame(fLastDayButton);
  fRangeFrame->AddFrame(fLastWeekButton);
  fRangeFrame->AddFrame(fLastMonthButton);

  fRangeLeftButton->Associate(this);
  fRangeRightButton->Associate(this);
  fRangeExpandButton->Associate(this);
  fRangeShrinkButton->Associate(this);
  fLastHourButton->Associate(this);
  fLastDayButton->Associate(this);
  fLastWeekButton->Associate(this);
  fLastMonthButton->Associate(this);

  fMemList->Add(fRangeLeftButton);
  fMemList->Add(fRangeRightButton);
  fMemList->Add(fRangeExpandButton);
  fMemList->Add(fRangeShrinkButton);
  fMemList->Add(fLastHourButton);
  fMemList->Add(fLastDayButton);
  fMemList->Add(fLastWeekButton);
  fMemList->Add(fLastMonthButton);

  // control-zoom

  /*
  //  fZoomVexpandButton = new TGTextButton(fZoomFrame,"ver +",13021);
  //  fZoomVshrinkButton = new TGTextButton(fZoomFrame,"ver -",13022);
  fZoomHexpandButton = new TGTextButton(fZoomFrame,"<----->",13023);
  fZoomHshrinkButton = new TGTextButton(fZoomFrame,"--> <--",13024);
  //  fZoomFrame->AddFrame(fZoomVexpandButton, fL10);
  //  fZoomFrame->AddFrame(fZoomVshrinkButton, fL10);
  fZoomFrame->AddFrame(fZoomHexpandButton, fL10);
  fZoomFrame->AddFrame(fZoomHshrinkButton, fL10);
  //  fZoomVexpandButton->Associate(this);
  //  fZoomVshrinkButton->Associate(this);
  fZoomHexpandButton->Associate(this);
  fZoomHshrinkButton->Associate(this);
  fMemList->Add(fZoomVExpandButton);
  fMemList->Add(fZoomVShrinkButton);
  */

  // control-window

  fWindowRatesButton = new TGTextButton(fWindowFrame,"  rates  ",13031);
  fWindowExitButton = new TGTextButton(fWindowFrame,"exit",13032);
  fWindowFrame->AddFrame(fWindowRatesButton, fL10);
  fWindowFrame->AddFrame(fWindowExitButton, fL10);
  fWindowRatesButton->Associate(this);
  fWindowExitButton->Associate(this);
  fMemList->Add(fWindowRatesButton);
  fMemList->Add(fWindowExitButton);

  // options

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetLabelFont(1,"X");
  gStyle->SetLabelFont(1,"Y");
  gStyle->SetLabelSize(0.045,"X");
  gStyle->SetLabelSize(0.045,"Y");

  // initialization of parameters

  fSelectionB[0]->AddText(0,"BC1");
  fSelectionB[1]->AddText(0,"BC2");
  fFilename = filename;
  fMin = 0;
  fMax = 0;
  fZoom = 1.0;
  fCanvasFrame->GetCanvas()->SetLeftMargin(0.06/fZoom);
  fCanvasFrame->GetCanvas()->SetRightMargin(0.04/fZoom);

  MapSubwindows();
  Resize(GetDefaultSize());
  SetWindowName("CERES scaler viewer");
  MapWindow();

  ReadAllEvents();
  fMin = SecondsAgo(7200);
  fMax = fEvents-1;
  MakeVectors();
  DrawGraphs();

  fTimer = new TTimer(this,500);
  fMemList->Add(fTimer);
  fTimer->TurnOn();
}
/*****************************************************************************/
 DGScalerViewer::~DGScalerViewer() {
  std::cout << "ScalerViewer destructedn";
  fTimer->TurnOff();
  fMemList->Delete();
}
/*****************************************************************************/
 void DGScalerViewer::ReadLastEvent() {
  // Copy the last line of the scaler database file fFilename into
  // a temporary file temp_file; then read this line and append to
  // the actual data in memory, i.e. to fTime and fScaler

  char command_str[500];
  char *temp_file = (char*)"/tmp/scavi.tmp";
  
  snprintf(command_str, 500, "tail -n 1 %s > %s", fFilename, temp_file);
  gSystem->Exec(command_str);
  ReadData(temp_file, fEvents);
}
/****************************************************************************/
 void DGScalerViewer::ReadData(Char_t *filename, Int_t start) {
  // Read data from filename and fill fTime andfScaler vectors, 
  // starting from index start.

  TDatime *time;
  UInt_t treference;
  Int_t i;
  
  // open file

  std::fstream f(filename, std::ios::in);
  if (!f.good()) {
    std::cout << "cannot open " << filename << std::endl;
    return;
  }
  f.flags(std::ios::skipws | std::ios::hex);

  // read scaler data

  fEvents = start;
  while (f >> fTime[fEvents]) {
    for (i=0; i<32; f >> fScaler[i++][fEvents]);
    fEvents++;
  }

  // I should have stored time in the standard time_t format
  // Now I have to translate.

  for (i=start; i<fEvents; i++) {
    time = new TDatime(fTime[i]);
    fTime[i] = time->Convert();
    delete time;
  }

  // Subtract January 1, 1999 to make the numbers smaller

  time = new TDatime();
  time->Set(1999,1,1,0,0,0);
  treference = time->Convert();
  delete time;
  for (i=start; i<fEvents; i++) fTime[i] -= treference;
}
/****************************************************************************/
 void DGScalerViewer::MakeVectors() {
  // Pick the (fMin,fMax) subrange of all (0,fEvents-1) events, pack them
  // into new fX and fY vectors. fY definition is given by user via
  // fSelection text. 

  Int_t    len;
  Int_t    i, j, n;
  Int_t    chan[4];
  
  // initial checks

  if (fMin < 0) fMin = 0;
  if (fMax > fEvents-1) fMax = fEvents-1;
  if (fMax < fMin) {std::cout << "fMax < fMin, bombingn"; return;}

  // pack the data into float arrays

  len = fMax-fMin+1;

  if (fX) delete fX;
  if (fY[0]) delete fY[0];
  if (fY[1]) delete fY[1];
  if (fY[2]) delete fY[2];
  if (fY[3]) delete fY[3];
  
  fX   = new Float_t[len];
  fY[0]  = new Float_t[len];
  fY[1]  = new Float_t[len];
  fY[2]  = new Float_t[len];
  fY[3]  = new Float_t[len];
  
  for (j=0; j<4; j++) {
    fGraphOn[j] = (strlen(fSelectionB[j]->GetString()) > 0);
    chan[j] = Chan(fSelectionB[j]->GetString());
  }

  n = 0;
  for (i=fMin; i<=fMax; i++) {
    fX[n] = (Float_t) fTime[i];
    for (j=0; j<4; j++) {
      if (fGraphOn[j]) {
	if (chan[j] > -1) {
	  fY[j][n] = (Float_t) fScaler[chan[j]][i];           // simple chnam
	} else {
	  fY[j][n] = EvalFormula(fSelectionB[j]->GetString(), i);  // formula
	}
	} else fY[j][n] = 0.0;
    }
    n++;
  }
  fGraphEvents = n;
}
/****************************************************************************/
 void DGScalerViewer::DrawGraphs() {
  // Draw the active (fGraphOn[i] == 1) graphs. 

  Int_t   i,j;
  Float_t xmin; 
  Float_t xmax; 
  Float_t ymin; 
  Float_t ymax; 
  
  fCanvasFrame->GetCanvas()->Clear();
  for (i=0; i<4; i++) if (fGraph[i]) {
    delete fGraph[i];
    fGraph[i] = 0;     // don't understand why I have to do this
  }

  // make graphs; find vertical range

  ymin = 0.0;
  ymax = 0.0;
  for (i=0; i<4; i++) if (fGraphOn[i]) {
    fGraph[i] = new TGraph(fGraphEvents, fX, fY[i]);
    fGraph[i]->SetLineColor(i+1);
  }

  // cosmetics

  xmin = fX[0];
  xmax = fX[fGraphEvents-1];
  if (fMax == fEvents-1) xmax = xmax + (xmax-xmin)/10.0;

  for (i=0; i<4; i++) {
    if (fGraphOn[i]) {
      for (j=0; j<fGraphEvents; j++) {
	if (fY[i][j] > ymax) ymax = fY[i][j];
	if (fY[i][j] < ymin) ymin = fY[i][j];
      }
    }
    break;
  }
  if (ymax > 0.0)    ymax = (ymax + (ymax-ymin)/10.0);
  if (ymin < 0.0)    ymin = (ymin + (ymin-ymax)/10.0);
  if (ymax <= ymin)  ymax = ymin + 1;
  if (ymin > 0.0)   ymin = 0.0;

  // make histogram for range and axes

  if (fHist) delete fHist;
  fHist = new TH2F("hr", "", 2, xmin, xmax, 2, ymin, ymax);
  fHist->Draw();
  fHist->GetXaxis()->SetTimeDisplay(1);

  // draw the stuff

  for (i=0; i<4; i++) if (fGraphOn[i]) fGraph[i]->Draw("L");  
  fCanvasFrame->GetCanvas()->Modified();
  fCanvasFrame->GetCanvas()->Update();
}
/*****************************************************************************/
 Int_t DGScalerViewer::SecondsAgo(UInt_t seconds) {

  // Return index of the event which was taken approx. seconds ago.

  Int_t i;
  for (i = fEvents-1; i>=0; i--) {
    if (fTime[fEvents-1] - fTime[i] > seconds) break;
  }
  return(i);
}
/*****************************************************************************/
 Int_t DGScalerViewer::Chan(const char* name) {
  // Decode string name into scaler channel number (between 0 and 31).

  TString buf;
  Int_t i, ret;

  buf.Append(name);
  ret = -1;
  for (i=0; i<32; i++) {
    if (buf.CompareTo(chna[i]) == 0) {
      ret = i;
      break;
    }
  }
  return(ret);
}
/*****************************************************************************/
 Float_t DGScalerViewer::EvalFormula(const char *form,  int event) {
  // Interpret form as a formula with channel names, and return 
  // the corresponding numerical value.

  char *psource;
  char *ptarget;
  char *poper;
  char buf[100];
  char nbuf[100];
  char target[200];
  int len;
  Float_t result;
  

  // Copy form into target, substituting channel names with fScaler
  // numerical values. For example, "BC1/BC2 - 1" will be converted to
  // "137002/121034 - 1".

  
  //  printf("form = %sn", form);
  psource = (char *) form;
  ptarget = target;
  while (1) {
    poper = strpbrk(psource, "+-/*");
    if (poper) len = poper-psource;
    else       len = strlen(psource);
    memcpy(buf, psource, len);
    buf[len] = 0;

    if (Chan(buf) > -1) {                      // substitute nr
      sprintf(nbuf,"%d",fScaler[Chan(buf)][event]); 
      len = strlen(nbuf);
    } else {
      memcpy(nbuf, buf, len);                  // simply copy channel name
    }      

    memcpy(ptarget, nbuf, len);
    ptarget += len;
    if (poper) *ptarget = *poper;
    else {
      *ptarget = 0;
      break;
    }    
    ptarget++;
    psource = poper+1;    
  } 
	 
  // Evaluate the formula and return the number
  TFormula *tf = new TFormula("form", target);
  result = tf->Eval(0.0);
  delete tf;
  return result;
}
/*****************************************************************************/
 Int_t DGScalerViewer::FileChanged() {
  // Return 1 if the scaler database file fFilename has been changed
  // since the last visit.

  struct stat buf;
  static time_t old;
  Int_t file_changed;
  
  stat (fFilename, &buf);
  file_changed = (buf.st_mtime != old);
  old = buf.st_mtime;
  return file_changed;
}
/****************************************************************************/
 void DGScalerViewer::Rates(Int_t n, UInt_t thre) {    
  // Print some rates in the traditional format, including average over 
  // last n bursts with BC1 above thr.

  Int_t    i, j, ngood;
  Float_t mean[32];
  Float_t BC1;
  Float_t BC2;
  Float_t BC3;
  Float_t BC1_mean;
  Float_t BC2_mean;
  Float_t BC3_mean;

  if (n<1) return;
  fTimer->TurnOff();

  i = fEvents-1;                 // event index
  ngood = 0;                     // good event number
  for (j=0; j<32; j++) mean[j] = 0.0;
  while ((i>=0) && (ngood<n)) {
    //    printf("%d: testing %d vs %dn", i, fScaler[Chan("BC1")][i], thre);
    if (fScaler[Chan("BC1")][i] > thre) {
      for (j=0; j<32; j++) mean[j] += (Float_t) fScaler[j][i];
      ngood++;
    }
    i--;
  }
  for (j=0; j<32; j++) mean[j] /= (Float_t) n;


  BC1 = (Float_t) fScaler[Chan("BC1")][fEvents-1];
  BC2 = (Float_t) fScaler[Chan("BC2")][fEvents-1];
  BC3 = (Float_t) fScaler[Chan("BC3")][fEvents-1];
  BC1_mean = (Float_t) mean[Chan("BC1")];
  BC2_mean = (Float_t) mean[Chan("BC2")];
  BC3_mean = (Float_t) mean[Chan("BC3")];

  printf("newrate etcn");
  printf("average: over %d bursts with BC1 above %dn", n, thre);
  printf("       last burst      average   last burst/BC1  average/BC1n");
  printf("BC1 %13.0f%13.1f%13.1f%13.1fn", BC1, BC1_mean, 100 * BC1/BC1, 100 * BC1_mean/BC1_mean);
  printf("BC2 %13.0f%13.1f%13.1f%13.1fn", BC2, BC2_mean, 100 * BC2/BC1, 100 * BC2_mean/BC1_mean);

 printf("BC3 %13.0f%13.1f%13.1f%13.1fn", BC3, BC3_mean, 100 * BC3/BC1, 100 * BC3_mean/BC1_mean);
  fTimer->TurnOn();
}
/*****************************************************************************/
 Bool_t DGScalerViewer::HandleTimer(TTimer *tim) {
  Int_t i;
  
  if (FileChanged()) {

    // update graphs

    ReadLastEvent();
    fMin++;
    fMax++;
    MakeVectors();
    DrawGraphs();

    // printout

    for (i=0; i<4; i++) {
      if (fSelectionB[i]->GetTextLength() > 0) {
	printf("%15.3f", EvalFormula(fSelectionB[i]->GetString(), fEvents-1));
      }
    }
    printf("n");
  }
    
  fTimer->Reset();
  return kTRUE;
}
/****************************************************************************/
 Bool_t DGScalerViewer::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {
  Int_t len;
  Int_t mid;
  Int_t sft;

  switch(GET_MSG(msg)) {
  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_TEXTCHANGED:
      fTimer->Reset();
      break;
    case kTE_ENTER:
      MakeVectors();
      DrawGraphs();
      break;
    default:
      break;
    }
    break;
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_MENU:
      break;
    case kCM_BUTTON:
      switch (parm1) {
      case 13011:     // double range, try to keep center
	len = fMax - fMin;
	mid = (fMin + fMax)/2;
	fMin = mid - len;
	fMax = mid + len;
	if (fMax > fEvents - 1) {
	  sft = fMax - fEvents + 1; 
	  fMin -= sft;
	  fMax -= sft;
	  if (fMin < 0) fMin = 0;
	}
	if (fMin < 0) {
	  sft = -fMin; 
	  fMin += sft;
	  fMax += sft;
	  if (fMax > (fEvents-1)) fMax = fEvents - 1;
	}
	MakeVectors();
	DrawGraphs();
	break;
      case 13012:               // decrease range; if at the edge, keep edge

	if (fMin == 0) {
	  fMax = fMax/2;
	} else if (fMax == fEvents-1) {
	  fMin = (fMin + fMax)/2;
	} else {
	  len = fMax - fMin;
	  if (len < 16) break;
	  mid = (fMin + fMax)/2;
	  fMax = mid + len/4;
	  fMin = mid - len/4;
	}
	MakeVectors();
	DrawGraphs();
	break;
      case 13013:               // shift left
	len = fMax - fMin;
	fMin -= len * 1/10;
	fMax -= len * 1/10;
	if (fMin < 0) {
	  sft = -fMin; 
	  fMin += sft;
	  fMax += sft;
	  if (fMax > (fEvents-1)) fMax = fEvents - 1;
	}
	MakeVectors();
	DrawGraphs();
	break;
      case 13014:               // shift right
	len = fMax - fMin;
	fMin += len * 1/10;
	fMax += len * 1/10;
	if (fMax > fEvents - 1) {
	  sft = fMax - fEvents + 1; 
	  fMin -= sft;
	  fMax -= sft;
	  if (fMin < 0) fMin = 0;
	}
	MakeVectors();
	DrawGraphs();
	break;
      case 13015:
	fMin = SecondsAgo(3600);
	fMax = fEvents - 1;
	MakeVectors();
	DrawGraphs();
	break;
      case 13016:
	fMin = SecondsAgo(3600*24);
	fMax = fEvents - 1;
	MakeVectors();
	DrawGraphs();
	break;
      case 13017:
	fMin = SecondsAgo(3600*24*7);
	fMax = fEvents - 1;
	MakeVectors();
	DrawGraphs();
	break;
      case 13018:
	fMin = SecondsAgo(3600*24*7*4);
	fMax = fEvents - 1;
	MakeVectors();
	DrawGraphs();
	break;
      case 13021:
	fGraph[0]->SetMaximum(fGraph[0]->GetHistogram()->GetMaximum()/2.0);
	break;
      case 13022:
	fGraph[0]->SetMaximum(fGraph[0]->GetHistogram()->GetMaximum()*2.0);
	break;
      case 13023:
	//	Hzoom(2.0);
	break;
      case 13024:
	//	Hzoom(0.5);
	break;
      case 13031:
	Rates(10, 1000);
	break;
      case 13032:
	//	delete this;
	fTimer->TurnOff();
	CloseWindow();
	gApplication->Terminate(0);
	break;
      default:
	break;
      }
      break;
    case kCM_CHECKBUTTON:
      break;
    case kCM_RADIOBUTTON:
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return kTRUE;
}
/*****************************************************************************/

