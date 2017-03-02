#include "pedestal.h"

using namespace std;

pedestal::pedestal()
{
}

pedestal::pedestal(argSet& args)
{
	setup(args);
}

pedestal::~pedestal()
{
}

bool pedestal::setup(argSet& args)
{
	arguments=args;
	if (arguments.ped_in_file!="")
	{
		status=load_file();
	}
	else
	{
		status=false;
	}
}

bool pedestal::getStatus()
{
	return status;
}

bool pedestal::load_file()
{
	ifstream file;
	status=false;
	
	stringstream conv;
	string buf, trash;
	
	cout<<"Loading pedestal values from file"<<endl;
	if (arguments.ped_in_file!="")
	{
		file.open(arguments.ped_in_file.c_str(),ios::in);
		if (file.good())
		{
			//Array
			for (int j=0; j<2; j++)
			{
				for (int channel=0; channel<16; channel++)
				{
					buf.clear();
					conv.clear();
					getline(file,buf);
					conv<<buf;
					conv>>trash>>values.Array[j][channel];
				}
			}
			//Ribbon
			for (int j=0; j<2; j++)
			{
				for (int layer=0; layer<4; layer++)
				{
					for (int channel=0; channel<16; channel++)
					{
						buf.clear();
						conv.clear();
						getline(file,buf);
						conv<<buf;
						conv>>trash>>values.Ribbon[j][layer][channel];
					}
				}
			}
		}
		status=true;
		cout<<"Pedestal loaded."<<endl;
	}
	else
	{
		cout<<"Cannot load pedestal values."<<endl;
	}
	return true;
}

void pedestal::printValues()
{
	//Array
	cout<<"Pedestal values: "<<endl;
	for (int j=0; j<2; j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			cout<<"Array["<<j<<"]["<<channel<<"]="<<values.Array[j][channel]<<endl;
		}
	}
	//Ribbon
	for (int j=0; j<2; j++)
	{
		for (int layer=0; layer<4; layer++)
		{
			for (int channel=0; channel<16; channel++)
			{
				cout<<"Ribbon["<<j<<"]["<<layer<<"]["<<channel<<"]="<<values.Ribbon[j][layer][channel]<<endl;
			}
		}
	}

}
#include "pedestal.h"
