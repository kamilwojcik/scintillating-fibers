#include "tracks.h"

tracks::tracks(load* input)
{
	event=input;
	name_part1="Arr[";
	name_part2="]-Rib[0][0][";
	string name;
	
	for (int mod1=0; mod1<16; mod1++) for (int mod3=0; mod3<16; mod3++)
	{
		name=name_part1+to_string(mod1)+name_part2+to_string(mod3)+"]";
		h_tracks[mod1][mod3]=new TH1I(name.c_str(), name.c_str(), 16,0,16);
		h_tracks[mod1][mod3]->SetStats(kFALSE);
		h_tracks[mod1][mod3]->GetXaxis()->SetTitle("Kanal Arr1");
		h_tracks[mod1][mod3]->GetYaxis()->SetTitle("Ilosc maksimow w kanale");
		h_tracks[mod1][mod3]->GetYaxis()->SetTitleOffset(1.5);
	}
	
	map_how_many=new TH2I("Ilosc sladow", "Ilosc sladow", 16,0,16, 16,0,16);
	map_where_max=new TH2I("Najczestsze maksimum w Arr1 dla sladu","Najczestsze maksimum w Arr1 dla sladu", 16,0,16, 16,0,16);
	
	map_how_many->SetStats(kFALSE);
	map_where_max->SetStats(kFALSE);
	
	map_how_many->GetXaxis()->SetTitle("Kanal Arr0");
	map_how_many->GetYaxis()->SetTitle("Kanal Rib0");
	
	map_where_max->GetXaxis()->SetTitle("Kanal Arr0");
	map_where_max->GetYaxis()->SetTitle("Kanal Rib0");
}

tracks::~tracks()
{
	for (int mod1=0; mod1<16; mod1++) for (int mod3=0; mod3<16; mod3++)
	{
		delete h_tracks[mod1][mod3];
	}
	delete map_how_many;
	delete map_where_max;
}

void tracks::FillTracks()
{
	if (event->reject.Array[0]==0 && event->reject.Ribbon[0][0]==0 && event->reject.Array[1]==0)
	{
		Arr0=event->WhereMaxArray[0];
		Arr1=event->WhereMaxArray[1];
		Rib0=event->WhereMaxRibbon[0][0];
		if (Arr0!=0 && Arr1!=0 && Rib0!=0)
		{
			h_tracks[Arr0][Rib0]->Fill(Arr1);
			map_how_many->Fill(Arr0, Rib0);
		}
	}
}

void tracks::MakeMapWhereMax()
{
	for (int mod1=0; mod1<16; mod1++) for (int mod3=0; mod3<16; mod3++)
	{
		map_where_max->Fill(mod1,mod3,h_tracks[mod1][mod3]->GetMaximumBin());
	}
	for (int mod1=0; mod1<=16; mod1++) for (int mod3=0; mod3<=16; mod3++)
	{
		//here we must cut off bins correspondint to rare tracks, because readability counts.
		if (map_how_many->GetBinContent(mod1,mod3)<64) //64, because it's a cool number ;)
		{
			map_where_max->SetBinContent(mod1,mod3,0);
		}
	}
}


void tracks::Save()
{
	cout<<"Saving tracks and maps into ./tracks/png_files directory"<<endl;
	
	string path_png, path_tracks;
	path_tracks="./tracks";
	mkdir(path_tracks.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png=path_tracks+"/png_files";
	mkdir(path_png.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png+="/";
	string filename;
	
	TCanvas *cnv=new TCanvas();
	cnv->cd();
	
	for (int mod1=0; mod1<16; mod1++) for (int mod3=0; mod3<16; mod3++)
	{
		filename=path_png+name_part1+to_string(mod1)+name_part2+to_string(mod3)+".png";
		h_tracks[mod1][mod3]->Draw();
		cnv->SaveAs(filename.c_str());
	}

	map_where_max->Draw("colz");
	filename=path_png+"map_where_max.png";
	cnv->SaveAs(filename.c_str());

	map_how_many->Draw("colz");
	filename=path_png+"map_how_many.png";
	cnv->SaveAs(filename.c_str());
	
	cnv->Close();
}
