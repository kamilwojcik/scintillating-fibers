#include "load.h"
#include <algorithm>
#include <math.h>

using namespace std;

load::load()
{

}

load::load(string Input)
{
	setup(Input);
}


load::~load()
{
}


bool load::loadTreeFile(string pedestalInput)
{
	inFile=new TFile(pedestalInput.data());
	inFile->GetObject<TTree>("tree",tree);
	currentEvent=new Event();
	branch=tree->GetBranch("Event");
	branch->SetAddress(&currentEvent);
	nevent=branch->GetEntries();
}

bool load::setup(string Input)
{
	loadTreeFile(Input);
	return true;
}

bool load::eventByIndex(Long64_t index)
{
	if (index>=0 && index<nevent)
	{
		tree->GetEvent(index);
		cout<<"Event: "<<index<<'\r'<<flush;
		return true;
	}
	else return false;
}

Long64_t load::GetNumberOfEvents()
{
	return nevent;
}

bool load::close()
{
	inFile->Close();
}


