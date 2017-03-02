#include "pedestal.h"

using namespace std;

pedestal::pedestal()
{
}

pedestal::pedestal(argSet& args)
{
	setup(args);
}


bool pedestal::setup(argSet& args)
{
	arguments=args;
	if (arguments.ped_in_file!="" && arguments.if_forall==false)
	{
		event.setup(arguments.ped_in_file);
		ifzero=0;
		if (arguments.ifzero!="") ifzero=stod(arguments.ifzero);
		range=event.GetNumberOfEvents()-1;
		loading();
		//event.close();
	}
	else
	{
		if (arguments.forall!=0 && arguments.if_forall==true) forall=stod(arguments.forall);
		else forall=0;
	}
}

bool pedestal::loading()
{
	if (!arguments.if_forall)
	{
		spectrum.setup(700,700);
		spectrum.SetNamePrefix("Pedestal_");
		
		cout<<"Loading events"<<endl;
		for (int i=0; i<=range; ++i)
		{
			event.eventByIndex(i);
			spectrum.Fill(&event);
			cout<<"Event: "<<i<<'\r'<<flush;
			//if (arguments.printAll==true) temp1D.Fill(event.currentEvent);
			
		}
		cout<<"\nFitting channels"<<endl;
		if (spectrum.Fit()) //TO DO: make fit() returning false in exception occurs
		{
			values=spectrum.pValues;
			cout<<"Pedestal values calculated.";
		}
		else
		{
			cout<<"Cannot calculate pedestal values."<<endl;
		}
	}
	else cout<<"Creating optput file with the same value for all channels"<<endl;
	
	return true;
}


bool pedestal::save()
{
	cout<<"Saving pedestal.info into ./pedestal directory..."<<endl;
	mkdir("pedestal",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	string temp;
	ofstream file("./pedestal/pedestal.info", ofstream::trunc);
	
	if (arguments.if_forall)
	{
		//Aarray
		for (int j=0; j<2; j++)
		{
			for (int channel=0; channel<16; channel++)
			{
				temp="Array[";
				temp+=to_string(j)+"][";
				temp+=to_string(channel)+"]: "+to_string(forall)+"\n";
				file<<temp;
			}
		}
		//Ribbon
		for (int j=0; j<2; j++)
		{
			for (int layer=0; layer<4; layer++)
			{
				for (int channel=0; channel<16; channel++)
				{
					temp="Ribbon[";
					temp+=to_string(j)+"][";
					temp+=to_string(layer)+"][";
					temp+=to_string(channel)+"]: "+to_string(forall)+"\n";
					file<<temp;

				}
			}
		}
		file.close();
		cout<<"ok."<<endl;
	}
	else if (file.is_open())
	{
		//Aarray
		for (int j=0; j<2; j++)
		{
			for (int channel=0; channel<16; channel++)
			{
				temp="Array[";
				temp+=to_string(j)+"][";
				temp+=to_string(channel)+"]: "+to_string((values.Array[j][channel]==0) ? ifzero : values.Array[j][channel])+"\n";
				file<<temp;
			}
		}
		//Ribbon
		for (int j=0; j<2; j++)
		{
			for (int layer=0; layer<4; layer++)
			{
				for (int channel=0; channel<16; channel++)
				{
					temp="Ribbon[";
					temp+=to_string(j)+"][";
					temp+=to_string(layer)+"][";
					temp+=to_string(channel)+"]: "+to_string((values.Ribbon[j][layer][channel]==0) ? ifzero : values.Ribbon[j][layer][channel] )+"\n";
					file<<temp;
				}
			}
		}
		file.close();
		cout<<"ok."<<endl;
	}
	else 
	{
		cout<<"Unable to save pedestal values."<<endl;
	}
	if (arguments.if_forall==false) spectrum.Save("./pedestal");
	return true;
}
