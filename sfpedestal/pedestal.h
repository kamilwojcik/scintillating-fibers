#ifndef PEDESTAL_H
#define PEDESTAL_H


//sfbeam includes
#include "load.h"
#include "interfaceStructs.h"
#include "channelspectrum.h"


using namespace std;

class pedestal : public TObject
{
public:
	load event;
	channelSpectrum spectrum;
	pedestVals values;
	argSet arguments;
	
	pedestal();
	pedestal(argSet&);
	
	bool setup(argSet&);
	bool save();
protected:
	Long64_t range;
	double ifzero;
	double forall;
	
	bool loading();
};

#endif // PEDESTAL_H
