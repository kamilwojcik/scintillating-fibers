#ifndef LOAD_H
#define LOAD_H


//sfbeam includes
#include "interfaceStructs.h"
#include "gain.h"
#include <string>
#include "pedestal.h"

using namespace std;


class load : public TObject
{
public:
	Event * currentEvent;
	FEvent *floatEvent;
	
	load();
	load(string);
	~load();
	Long64_t GetNumberOfEvents();
	bool eventByIndex(Long64_t);
	bool setup(string);
	void setPedestal(pedestal*);
	bool close();
	void setGain(gain *);
	bool GetPedStatus();
	gain *gain_data;


protected:
	bool ped_correction;
	bool apply_gain;
	TFile * inFile;
	TTree * tree, map;
	TBranch * branch;
	Long64_t nevent;
	pedestVals *ped_values;
	
	void eventCpy();
	
	///////////////////
	//for event assessment
	//
	double *temp;
	cutoff_levels cut_levels;
	Double_t temp_secondary_cut_level; //to avoid memory allocation many times

	bool loadTreeFile(string);
	bool applyGain();
	bool applyPedestalCorrection();
};

#endif // LOAD_H
