#ifndef CHANNELSPECTRUM_H
#define CHANNELSPECTRUM_H

#include "interfaceStructs.h"
#include "load.h"


using namespace std;

class channelSpectrum
{
protected:
	const static int nChannels=16;
	int nbins, rangeFromZeroTo;
	string prefix, suffix;
	load * event;
	bool onlyMax;
	
public:
	TH1D *Array[2][16];
	TH1D *Ribbon[2][4][16];
	
	channelSpectrum(load *evnt, int bins=1024, int range=2, bool max_condition=false);
	channelSpectrum(load *evnt, const char* pref, const char* suff, int bins=1024, int range=2, bool max_condition=false);
	bool setup(load *evnt, int bins, int range, bool max_condition);
	
	~channelSpectrum();
	
	bool Fill();
	bool Save(const char* path=".");
	bool SetNamePrefix(const char*);
	bool SetNameSuffix(const char*);
};

#endif // CHANNELSPECTRUM_H
