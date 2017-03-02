#ifndef TRACKS_H
#define TRACKS_H

#include "load.h"

class tracks
{
public:
	TH1I * h_tracks[16][16];
	TH2I *map_how_many;
	TH2I *map_where_max;
	load * event;
	
	string name_part1, name_part2;
	
	tracks(load * input);
	~tracks();
	
	void FillTracks();
	void MakeMapWhereMax();
	void Save();
	
protected:
	int Arr0, Arr1, Rib0; //to allocate it once instead of many many times in loop;
};

#endif // TRACKS_H
