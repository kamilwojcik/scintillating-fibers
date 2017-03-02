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


bool load::loadTreeFile(string Input)
{
	inFile=new TFile(Input.data());
	inFile->GetObject<TTree>("tree",tree);
	currentEvent=new Event();
	branch=tree->GetBranch("Event");
	branch->SetAddress(&currentEvent);
	nevent=branch->GetEntries();
}

bool load::setup(string Input)
{
	floatEvent= new FEvent;

	loadTreeFile(Input);
	ped_correction=false;
	return true;
}

void load::setPedestal(pedestal* pptr)
{
	if (pptr->getStatus())
	{
		ped_values=&pptr->values;
		ped_correction=true;
	}
}

bool load::applyPedestalCorrection()
{
	for (int j=0; j<2; j++) for (int channel=0; channel<16; channel++)
	{
		currentEvent->qdcArray[j][channel]=(currentEvent->qdcArray[j][channel]-ped_values->Array[j][channel]);
		for (int layer=0; layer<3; layer++)
		{
			currentEvent->qdcRibbon[j][layer][channel]=(currentEvent->qdcRibbon[j][layer][channel]-ped_values->Ribbon[j][layer][channel]);
		}
	}
}

void load::setGain(gain* gain_in)
{
	gain_data=gain_in;
	apply_gain=true;
}


bool load::applyGain()
{
	for (int j=0; j<2; j++) for (int channel=0; channel<16; channel++)
	{
		{
			floatEvent->qdcArray[j][channel]=(gain_data->Array[j][channel]* floatEvent->qdcArray[j][channel]);
		}
		for (int layer=0; layer<4; layer++)
		{
			floatEvent->qdcRibbon[j][layer][channel]=(gain_data->Ribbon[j][layer][channel]*floatEvent->qdcRibbon[j][layer][channel]);
		}
	}
	return true;
}


void load::eventCpy()
{
	for (int j=0; j<2; j++) for (int channel=0; channel<16; channel++)
	{
		floatEvent->qdcArray[j][channel]=(double)currentEvent->qdcArray[j][channel];
		for (int layer=0; layer<3; layer++)
		{
			floatEvent->qdcRibbon[j][layer][channel]=(double)currentEvent->qdcRibbon[j][layer][channel];
		}
	}
}


bool load::eventByIndex(Long64_t index)
{
	if (index>=0 && index<nevent)
	{
		tree->GetEvent(index);

		if (ped_correction) applyPedestalCorrection();
		eventCpy();
		if (apply_gain) applyGain();
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


bool load::GetPedStatus()
{
	return ped_correction;
}
