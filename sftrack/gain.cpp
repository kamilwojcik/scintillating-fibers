#include "gain.h"
#include <iostream>
#include <algorithm>

gain::gain()
{
}

void gain::setOnes()
{
	for (int d=0; d<2; d++) for (int channel=0; channel<16; channel++)
	{
		Array[d][channel]=1;
		for (int layer=0; layer<4; layer++) Ribbon[d][layer][channel]=1;
	}
}

void gain::invert()
{
	for (int d=0; d<2; d++) for (int channel=0; channel<16; channel++)
	{
		Array[d][channel]=1./Array[d][channel];
		for (int layer=0; layer<4; layer++) Ribbon[d][layer][channel]=1./Ribbon[d][layer][channel];
	}
}


void gain::multiply(double coef)
{
	for (int d=0; d<2; d++) for (int channel=0; channel<16; channel++)
	{
		Array[d][channel]*=coef;
		for (int layer=0; layer<4; layer++) Ribbon[d][layer][channel]*=coef;
	}
}

void gain::print()
{
	cout<<"Gain values:"<<endl;
	//Array
	for (int d=0; d<2; d++)
	{
		for (int channel=0; channel<16; channel++)
		{
			cout<<"Array["<<d<<"]["<<channel<<"]= "<<Array[d][channel]<<endl;
		}
	}
	
	//Ribbon
	for (int d=0; d<2; d++)
	{
		for (int layer=0; layer<4; layer++)
		{
			for (int channel=0; channel<16; channel++)
			{
				cout<<"Ribbon["<<d<<"]["<<layer<<"]["<<channel<<"]= "<<Ribbon[d][layer][channel]<<endl;
			}
		}
	}
}


bool gain::load(string filename)
{
	ifstream file;
	bool status=false;
	
	stringstream conv;
	string buf, trash;
	
	cout<<"Loading gain values"<<endl;
	file.open(filename.c_str(),ios::in);
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
				conv>>trash>>Array[j][channel];
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
					conv>>trash>>Ribbon[j][layer][channel];
				}
			}
		}
		status=true;
		cout<<"Gain values loaded."<<endl;
	}
	else
	{
		cout<<"Cannot load gain values."<<endl;
	}
	return status;
}

bool gain::saveAs(string filename)
{
	cout<<"Saving gain values into a file"<<endl;
	bool status=false;
	string temp;
	ofstream file(filename.c_str(), ofstream::trunc);
	if (file.good())
	{
		//Aarray
		for (int j=0; j<2; j++)
		{
			for (int channel=0; channel<16; channel++)
			{
				temp="Array[";
				temp+=to_string(j)+"][";
				temp+=to_string(channel)+"]: "+to_string(Array[j][channel])+"\n";
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
					temp+=to_string(channel)+"]: "+to_string(Ribbon[j][layer][channel])+"\n";
					file<<temp;
				}
			}
		}
		status=true;
		file.close();
	}
	else cout<<"Unable to save gain values into a file."<<endl;
	return status;
}