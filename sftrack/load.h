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
	load(string, bool);
	~load();
	Long64_t GetNumberOfEvents();
	bool eventByIndex(Long64_t);
	bool setup(string, bool);
	void setPedestal(pedestal*);
	bool close();
	bool check_event();
	bool findMax();
	void setGain(gain *);
	bool GetPedStatus();
	void print();
	rejection reject;
	gain *gain_data;

	int WhereMaxArray[2];
	int WhereMaxRibbon[2][3];


protected:
	TFile * inFile;
	TTree * tree, map;
	TBranch * branch;
	Long64_t nevent;
	pedestVals *ped_values;
	bool ped_correction;
	
	void eventCpy();
	
	///////////////////
	// dealing with events
	cutoff_levels cut_levels;
	Double_t temp_secondary_cut_level; //to avoid memory allocation many times
	double *temp;
	bool apply_filtration, apply_gain;
	int position, nbins_over_cut_level;
	
	bool filtration1();
	bool filtration2();
	bool resetRejection();
	bool setCutLevels();
	bool loadTreeFile(string);
	bool applyGain();
	bool applyPedestalCorrection();
	
	///////////////////////////////////////
	//ACHTUNG! To use count_if I need this:
	int count_if_over(double *, int, Double_t);

};

#endif // LOAD_H
