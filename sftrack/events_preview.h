#ifndef EVENTS_PREVIEW_H
#define EVENTS_PREVIEW_H

#include "load.h"
#include "interfaceStructs.h"

class events_preview
{
public:
	TH1D *event_preview[3];
	TCanvas * cnv;
	load *event;
	char *name_list[3];
	char *file_name_list[3];
	Long64_t index;
	
	events_preview(load * input, TCanvas * canvas);
	~events_preview();
	
	void Reload(Long64_t event_index);
	void Save();
};

#endif // EVENTS_PREVIEW_H
