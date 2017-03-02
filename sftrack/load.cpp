#include "load.h"
#include <algorithm>
#include <math.h>

using namespace std;

load::load()
{

}

load::load(string Input, bool filtration)
{
	setup(Input, filtration);
}

load::~load()
{
	inFile->Close();
}


bool load::setCutLevels()
{
	cut_levels.Array[0]=0.8;
	cut_levels.Array[1]=0.88;
	for (int j=0; j<2; j++)
	{
		for (int layer=0; layer<4; layer++)
		{
			cut_levels.Ribbon[j][layer]=0.8;
		}
	}
	return true;
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

bool load::setup(string Input, bool filtration)
{
	floatEvent= new FEvent;
	setCutLevels();
	apply_filtration=filtration;
	resetRejection();
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
		if (currentEvent->qdcArray[j][channel]<0) currentEvent->qdcArray[j][channel]=0;
		for (int layer=0; layer<3; layer++)
		{
			currentEvent->qdcRibbon[j][layer][channel]=(currentEvent->qdcRibbon[j][layer][channel]-ped_values->Ribbon[j][layer][channel]);
			if (currentEvent->qdcRibbon[j][layer][channel]<0) currentEvent->qdcRibbon[j][layer][channel]=0;
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
		findMax();
		check_event(); //event selection (to reject or not?)
		cout<<"Event: "<<index<<'\r'<<flush;
		
		return true;
	}
	else return false;
}

bool load::resetRejection()
{
	//reset rejection info
	for (int i=0; i<2; i++)
	{
		reject.Array[i]=0;
		for (int layer=0; layer<4; layer++) reject.Ribbon[i][layer]=0;
	}
	return true;
}

bool load::check_event()
{
	resetRejection();
	return (filtration1() && filtration2());
}

int load::count_if_over(double* array, int size, Double_t level)
{
	nbins_over_cut_level=0;
	for (int i=0; i<size; i++)
	{
		if (array[i]>level) nbins_over_cut_level++;
	}
	return nbins_over_cut_level;
}

bool load::filtration1() //reject if no bin is over cut level (or if not exactly 1 for ribbon)
{
	for (int i=0; i<2; i++)
	{
		count_if_over(floatEvent->qdcArray[i],16, cut_levels.Array[i]);
		if (nbins_over_cut_level<1) reject.Array[i]+=1;
		
		for (int layer=0; layer<3; layer++)
			{
				count_if_over(floatEvent->qdcRibbon[i][layer],16, cut_levels.Ribbon[i][layer]);
				if (nbins_over_cut_level<1) reject.Ribbon[i][layer]+=1;
			}
		}
	return true;
}


bool load::filtration2() //reject if too much bins (more than 3) is over the level of 80% of maximum
{
	
	for (int i=0; i<2; i++)
	{
		temp=max_element<double*>(floatEvent->qdcArray[i],floatEvent->qdcArray[i]+16);
		temp_secondary_cut_level=0.75*(*temp);
		count_if_over(floatEvent->qdcArray[i],16, temp_secondary_cut_level);
		if (nbins_over_cut_level>3) reject.Array[i]+=2;
		
		for (int layer=0; layer<3; layer++)
		{
			temp=max_element<double*>(floatEvent->qdcRibbon[i][layer],floatEvent->qdcRibbon[i][layer]+16);
			temp_secondary_cut_level=0.75*(*temp);
			count_if_over(floatEvent->qdcRibbon[i][layer],16, temp_secondary_cut_level);
			if (nbins_over_cut_level>3) reject.Ribbon[i][layer]+=2;
		}
		
	}
	return true;
}



Long64_t load::GetNumberOfEvents()
{
	return nevent;
}

bool load::close()
{
	inFile->Close();
}

bool load::findMax()
{
	for (int i=0; i<2; i++)
	{
		temp = max_element(floatEvent->qdcArray[i],floatEvent->qdcArray[i] + 16);
		WhereMaxArray[i]= temp - (floatEvent->qdcArray[i]);
		for (int layer=0; layer<3; layer++)
		{
			temp = max_element(floatEvent->qdcRibbon[i][layer],floatEvent->qdcRibbon[i][layer] + 16);
			WhereMaxRibbon[i][layer]= temp - (floatEvent->qdcRibbon[i][layer]);
		}
	}
	return true;
}

bool load::GetPedStatus()
{
	return ped_correction;
}

void load::print()
{	
	cout<<"reject:"<<endl;
	cout<<"Arr[0]:"<<reject.Array[0]<<endl;
	cout<<"Arr[1]:"<<reject.Array[1]<<endl;
	cout<<"Rib[0][0]:"<<reject.Ribbon[0][0]<<endl;
	cout<<"Rib[0][1]:"<<reject.Ribbon[0][1]<<endl;
	cout<<"Rib[0][2]:"<<reject.Ribbon[0][2]<<endl;
	cout<<"Rib[0][3]:"<<reject.Ribbon[0][3]<<endl;
	cout<<"MaxArr[0]:"<<WhereMaxArray[0]<<endl;
	cout<<"MaxArr[1]:"<<WhereMaxArray[1]<<endl;
	cout<<"MaxRib[0][0]:"<<WhereMaxRibbon[0][0]<<endl;
}
