#ifndef LOAD_H
#define LOAD_H


//sfbeam includes
#include "interfaceStructs.h"

using namespace std;


class load : public TObject
{
public:
	Event * currentEvent;
	
	load();
	load(string);
	~load();
	Long64_t GetNumberOfEvents();
	bool eventByIndex(Long64_t);
	bool setup(string);
	bool close();

protected:
	TFile * inFile;
	TTree * tree;
	TBranch * branch;
	Long64_t nevent;
	
	
	bool loadTreeFile(string);
};

#endif // LOAD_H
