#ifndef CHANNELSPECTRUM_H
#define CHANNELSPECTRUM_H

#include "interfaceStructs.h"
#include "load.h"


using namespace std;

class channelSpectrum : public TObject
{
protected:
	const static int nChannels=16;
	bool correction;
	int nbins, rangeFromZeroTo;
	string prefix, suffix;
	
public:
	TH1D *Array[2][16];
	TH1D *Ribbon[2][4][16];
	
	pedestVals pValues;
	
	channelSpectrum();
	channelSpectrum(const char*, const char*, int, int);
	bool setup(int, int);
	
	~channelSpectrum();
	
	bool Fill(load *);
	bool Fit();
	bool Save(const char*);
	bool SetNamePrefix(const char*);
	bool SetNameSuffix(const char*);
};

#endif // CHANNELSPECTRUM_H
