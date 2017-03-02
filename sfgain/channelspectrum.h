#ifndef CHANNELSPECTRUM_H
#define CHANNELSPECTRUM_H

#include "interfaceStructs.h"
#include "load.h"


using namespace std;

class channelSpectrum : public TObject
{
protected:
	const static int nChannels=16;
	int nbins, rangeFromZeroTo;
	string prefix, suffix;
	load * event;
	
public:
	TH1D *Array[2][16];
	TH1D *Ribbon[2][4][16];
	
	channelSpectrum(int bins=4096, int range=4096);
	channelSpectrum(const char*, const char*, int bins=4096, int range=4096);
	bool setup(int, int);
	
	~channelSpectrum();
	
	bool loopLoad(load *, Long64_t, Long64_t);
	bool Fill();
	bool Fit();
	bool Save(const char* path=".");
	bool SetNamePrefix(const char*);
	bool SetNameSuffix(const char*);
};

#endif // CHANNELSPECTRUM_H
