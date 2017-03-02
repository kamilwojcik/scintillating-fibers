#include "args.h"

using namespace std;

args::args(int argc, char** argv)
{
	args_good=true;
	setup.rangeBegin=setup.rangeEnd=0;
	setup.rangeMax=true;
	setup.ped_in_file="";
	setup.gain_in_file="";
	setup.beam_in_file="";
	
	string tempArg, input="input=", pedestal="pedestal=", gain="gain=", begin="from=", end="to=", help="help";
	size_t found;

	//setup: input, pedestal, *gain, *begin, *end, *help;  * - optional
	
	for (int i=1; i<argc; i++)
    {
		tempArg=argv[i];
		
		
		found=tempArg.find(input);
		if (found!=string::npos)
		{
			tempArg.replace(0,6,"");
			setup.beam_in_file=tempArg;
			continue;
		}

		found=tempArg.find(gain);
		if (found!=string::npos)
		{
			tempArg.replace(0,5,"");
			setup.gain_in_file=tempArg;
			continue;
		}
		
		found=tempArg.find(pedestal);
		if (found!=string::npos)
		{
			tempArg.replace(0,9,"");
			setup.ped_in_file=tempArg;
			continue;
		}
		
		found=tempArg.find(begin);
		if (found!=string::npos)
		{
			tempArg.replace(0,5,"");
			if (tempArg=="min")
			{
				setup.rangeBegin=0;
			}
			else
			{
				try
				{
					setup.rangeBegin=stoul(tempArg);
					//cout<<i<<" \"begin\" option: "<<setup.rangeBegin<<endl;
				}
				catch(const invalid_argument& err)
				{
					cout<<"Invalid range argument(s). Try again."<<endl;
					args_good=false;
					break;
				}
			}
			continue;
		}
		
		
		found=tempArg.find(end);
		if (found!=string::npos)
		{
			tempArg.replace(0,3,"");
			if (tempArg=="max") cout<<i<<" \"end\" option: "<<"max"<<endl;
			else
			{
				try
				{
					setup.rangeMax=false;
					setup.rangeEnd=stoul(tempArg);
				}
				catch(const invalid_argument& err)
				{
					cout<<"Invalid range argument(s). Try again."<<endl;
					args_good=false;
					break;
				}
			}
			continue;
		}
		
		else if (tempArg=="help")
		{
			cout<<"\n    ~~~// Sci-Fi track tool //~~~\n\n"<<endl;
			cout<<"  Arguments:\n"<<endl;
			cout<<"input=path           - path to beam input file"<<endl;
			cout<<"pedestal=path        - path to pedestal *.info file"<<endl;
			cout<<"gain=path            - path to gain *.info file"<<endl;
			cout<<"from=number          - number of first event to analise  OR  min"<<endl;
			cout<<"to=number            - number of last event to analise   OR  max"<<endl;
			cout<<"help                 - prints this help\n"<<endl;
			cout<<"  Default arguments\n"<<endl;
			cout<<"Input argument is obligatory. The rest is optional, by default set as follows:"<<endl;
			cout<<"pedestal=./pedestal.info"<<endl;
			cout<<"from=0"<<endl;
			cout<<"to=max\n"<<endl;
			cout<<"Gain - not set\n"<<endl;
			cout<<"At the beginning you can set a beam input file with pedestal (then pedestal correction is enable) or not."<<endl;
			cout<<"Then beam data starts to loading, to provide the preview of spectra of each channel."<<endl;
			cout<<"It generates 1D spectra (*.png and *.root) for each channel and default gain.info file with gain values set to 1."<<endl;
			cout<<"Then you can view it and MANUALLY change gains for each channel in gain.info to the value you think it's ok."<<endl;
			cout<<"Then you can run program again giving not only beam input file, but also gain.info file."<<endl;
			cout<<"If you do so, program generates channel spectra with max condition for each channel. Then it fits spectra"<<endl;
			cout<<"to find position of argon peak (or something else) more accurate than you, and overwrites gain values in file."<<endl;
			cout<<"Live long and prosper!\n"<<endl;
		}
		else
		{
			args_good=false;
			cout<<"Bad argument(s). Try again."<<endl;
			break;
		}
    }
    if (setup.rangeBegin > setup.rangeEnd && setup.rangeMax==false)
	{
		args_good=false;
		cout<<"Bad range arguments: begin is bigger than end. Try again."<<endl;
	}
	
	if (setup.beam_in_file=="" )
	{
		args_good=false;
		cout<<"You need to give a path to an input file. Try again."<<endl;
	}
}

bool args::args_correct(void)
{
	return args_good;
}


