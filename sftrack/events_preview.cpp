#include "events_preview.h"

events_preview::events_preview(load *input, TCanvas *canvas)
{
	event=input;
	cnv=canvas;
	
	name_list[0]=(char *)"Array[0]";
	name_list[1]=(char *)"Array[1]";
	name_list[2]=(char *)"Ribbon[0]";
	file_name_list[0]=(char *)"Array0_event_";
	file_name_list[1]=(char *)"Array1_event_";
	file_name_list[2]=(char *)"Ribbon0_event_";
	
	for (int module=0; module<3; module++)
	{
		event_preview[module]=new TH1D(name_list[module],name_list[module],16,0,16);
		event_preview[module]->SetStats(kFALSE);
	}
}

events_preview::~events_preview()
{
	for (int module=0; module<3; module++) delete event_preview[module];
}



void events_preview::Reload(Long64_t event_index)
{
	index=event_index;
	string title="Event: "+to_string(index)+" "+(string)name_list[0]+" cuts: "+to_string(event->reject.Array[0]);
	event_preview[0]->SetTitle(title.c_str());
	
	title="Event: "+to_string(index)+" "+(string)name_list[1]+" cuts: "+to_string(event->reject.Array[1]);
	event_preview[1]->SetTitle(title.c_str());
	
	title="Event: "+to_string(index)+" "+(string)name_list[2]+" cuts: "+to_string(event->reject.Ribbon[0][0]);
	event_preview[2]->SetTitle(title.c_str());
	
	
	for (int channel=0; channel<16; channel++)
	{
		event_preview[0]->SetBinContent(channel,event->floatEvent->qdcArray[0][channel]);
		event_preview[1]->SetBinContent(channel,event->floatEvent->qdcArray[1][channel]);
		event_preview[2]->SetBinContent(channel,event->floatEvent->qdcRibbon[0][0][channel]);
	}
}

void events_preview::Save()
{
	cout<<"Saving previews of events into ./events/png_files directory"<<endl;
	
	string path_png, path_events;
	path_events="./events";
	mkdir(path_events.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png=path_events+"/png_files";
	mkdir(path_png.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png+="/";
	string filename;

	for (int module=0; module<3; module++)
	{
		filename=path_png+file_name_list[module]+to_string(index)+".png";
		event_preview[module]->Draw();
		cnv->SaveAs(filename.c_str());
	}
}
