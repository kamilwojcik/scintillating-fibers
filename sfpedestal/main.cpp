#include <iostream>
#include <string>
#include <stdexcept>

#include "args.h"
#include "interfaceStructs.h"
#include "pedestal.h"
#include "channelspectrum.h"

using namespace std;

int main(int argc, char **argv)
{
	//processing arguments
	args arg(argc, argv);
	if (arg.args_correct())
	{
		cout<<"Processing pedestal..."<<endl;
		pedestal p1(arg.setup);
		p1.save();
	}
	return 0;
}
 