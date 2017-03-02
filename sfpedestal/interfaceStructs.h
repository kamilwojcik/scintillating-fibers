//////////////////////////////////////////////////////////
// it's convenient to create some structures or classes
// that may be uset in various classes as a specific type
// i.e. as a set of options shared between objects
// 

#ifndef INTERFACESTRUCTS_H
#define INTERFACESTRUCTS_H

//root cern
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TChain.h"
#include "Ttypes.h"
#include "TDirectory.h"
#include "TStyle.h"
#include "TText.h"
#include "TLatex.h"
#include "TLine.h"
#include "TPad.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TStyle.h"
#include "TDataType.h"
#include "TBranch.h"
#include "TStopwatch.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TObject.h"
#include "TF1.h"
#include "TRint.h"
#include "TOrdCollection.h"
#include "cintdictversion.h"
#include "RVersion.h"
#include "TClass.h"

//std
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h> //mkdir

//sfbeam includes
#include "event.h"

//argRerutns - set of parameters determining analysis, returned by "args" class.
//it's more convenient to put it in one structure than to manage so many variables separately
struct argSet
{
	std::string ped_in_file, ifzero, forall;
	bool just_help, if_forall;
};

//carries pedestal values for each channel
struct pedestVals
{
	Double_t Array[2][16];
	Double_t Ribbon[2][4][16];
};


//this one carries calculated peaks values
struct peaks
{
	Double_t Array[2];
	Double_t Ribbon[2][4];
};

typedef peaks sigmas;

#endif // INTERFACESTRUCTS_H

typedef double Double_t;
typedef int Int_t;
//typedef long Long64_t;