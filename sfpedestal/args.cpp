#include "args.h"

using namespace std;

args::args(int argc, char** argv)
{
	args_good=true;
	setup.just_help=false;
	setup.if_forall=false;
	setup.ped_in_file="";
	
	string tempArg, input="input=", ifzero="ifzero=", forall="forall=", help="help";
	size_t found;

	//setup: input, *help, *ifzero, *forall;  * - optional
	
	for (int i=1; i<argc; i++)
    {
		tempArg=argv[i];
		
		
		found=tempArg.find(input);
		if (found!=string::npos)
		{
			tempArg.replace(0,6,"");
			setup.ped_in_file=tempArg;
			continue;
		}
		
		found=tempArg.find(ifzero);
		if (found!=string::npos)
		{
			tempArg.replace(0,7,"");
			setup.ifzero=tempArg;
			continue;
		}
		
		found=tempArg.find(forall);
		if (found!=string::npos)
		{
			tempArg.replace(0,7,"");
			setup.forall=tempArg;
			setup.if_forall=true;
			continue;
		}
		
		if (tempArg=="help")
		{
			setup.just_help=true;
			cout<<"\n    ~~~// Sci-Fi pedestal analysis //~~~\n"<<endl;
			cout<<"  Arguments:\n"<<endl;
			cout<<"input=path           - path to the pedestal input file"<<endl;
			cout<<"ifzero=value         - use this if you want to set pedestal value for empty channels"<<endl;
			cout<<"forall=value         - use this is you just want to set the same value for every channel"<<endl;
			cout<<"help                 - prints this help\n"<<endl;
			
			cout<<"You need to set input argument or forall value."<<endl;
			cout<<"If input file is given and forall is set (what would be illogical), input file is ignored."<<endl;
			cout<<"Output file is named pedestal.info.\n"<<endl;
			
			
			cout<<"  Default arguments\n"<<endl;
			cout<<"If input pedestal file is given then ifzero is set to zero by default."<<endl;
			cout<<"If no arguments given then forall=0 is assumed."<<endl;
			cout<<"Live long and prosper!\n"<<endl;
			break;
		}
		else
		{
			args_good=false;
			cout<<"Bad argument(s). Try again."<<endl;
			break;
		}
    }
    if (argc<2)
	{
		forall="0";
		setup.if_forall=true;
	}
	
	if (setup.just_help) args_good=false;
}

bool args::args_correct(void)
{
	return args_good;
}

