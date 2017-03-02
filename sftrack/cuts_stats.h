/////////////////////////////////////////////////////////
// This class prepares histogram with statistics of cuts
//

#ifndef CUTS_STATS_H
#define CUTS_STATS_H

#include "interfaceStructs.h"
#include "load.h"
#include <string>

class cuts_stats
{
public:
	TH1I *modules;
	TH1I *ribbon_stats[3];
	load *event;
	
	cuts_stats(load *event_input);
	~cuts_stats();
	
	void Fill();
	void Save();
};

#endif // CUTS_STATS_H
