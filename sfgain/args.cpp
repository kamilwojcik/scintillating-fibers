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
			//cout<<i<<" \"input\" option: "<<setup.in_file<<endl;
			continue;
		}

		found=tempArg.find(gain);
		if (found!=string::npos)
		{
			tempArg.replace(0,5,"");
			setup.gain_in_file=tempArg;
			//cout<<i<<" \"input\" option: "<<setup.in_file<<endl;
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
				//cout<<i<<" \"begin\" option: "<<"min"<<endl;
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
					//cout<<i<<" \"begin\" option: "<<setup.rangeEnd<<endl;
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
			cout<<"\n    ~~~// Sci-Fi gain tool //~~~\n\n"<<endl;
			cout<<"  Arguments:\n"<<endl;
			cout<<"input=path           - path to beam input file"<<endl;
			cout<<"pedestal=path        - path to pedestal *.info file"<<endl;
			cout<<"gain=path            - path to gain *.info file"<<endl;
			cout<<"from=number          - number of first event to analise  OR  min"<<endl;
			cout<<"to=number            - number of last event to analise   OR  max"<<endl;
			cout<<"help                 - prints this help\n"<<endl;
			cout<<"  Default arguments\n"<<endl;
			cout<<"from=0"<<endl;
			cout<<"to=max\n"<<endl;
			cout<<"If you give just input file, you get raw spectra."<<endl;
			cout<<"If you give input file and pedestal.info file, you get corrected spectra (pedestal correction)."<<endl;
			cout<<"If you give input file, pedestal.info file and gain.info file, you get calibrated & corrected spectra."<<endl;
			cout<<"If you give only *.info file with inverting gains, program generates gain.info file with gains.\n"<<endl;
			
			cout<<"Normal use order:"<<endl;
			cout<<"1. One gives input and pedestal files and gets corrected spectra and gain_inv.info file with ones."<<endl;
			cout<<"2. One looks on spectra to figure out where argon peak is and manually writes it into gain_inv.info."<<endl;
			cout<<"3. One passes only gain_inv.info file to the program and gets gain.info file with inversions."<<endl;
			cout<<"4. One runs program again with input, pedestal and gain and gets calibrated and correctrd spectra."<<endl;
			cout<<"\nNow one is happy. One can use pedestal and gain files with sftrack program."<<endl;
			cout<<"One also can figure out what cut levels should be set in sftrack."<<endl;
			cout<<"\nLive long and prosper!\n"<<endl;
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
	
	if (setup.beam_in_file=="")
	{
		args_good=false;
	}
}

bool args::args_correct(void)
{
	return args_good;
}

