#ifndef CORRELATIONS_H
#define CORRELATIONS_H

#include "load.h"


class correlations
{
public:
	TH2F *corr_histos[3][10][10];
	load *event;
	
	correlations(load* input, int maxArr0=0, int maxArr1=0, int maxRib0=0, bool max_condition=false);
	~correlations();
	
	void Fill();
	void Save();
	
protected:
	bool max_conditions;
	int max_Arr0, max_Arr1, max_Rib0;
};

#endif // CORRELATIONS_H
