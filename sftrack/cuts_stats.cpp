#include "cuts_stats.h"
#include <iostream>


cuts_stats::cuts_stats(load* event_input)
{
	event=event_input;
	string modules_Xaxis[4]={"Wszystkie zdarzenia", "Array[0]", "Array[1]", "Ribbon[0]"};
	string ribbon_stats_Xaxis[4]={"Wszystkie zdarzenia", "cut 1", "cut 2", "cut 1 i 2"};
	
	modules=new TH1I("Selekcja na modulach","Selekcja na modulach",4,0,4);
	for (int label=0; label<4; label++) modules->GetXaxis()->SetBinLabel(label+1, modules_Xaxis[label].c_str());
	modules->SetBarWidth(1);
	modules->SetBarOffset(0);
	modules->SetFillColor(12);
	modules->GetXaxis()->SetLabelSize(0.05);
	modules->GetYaxis()->SetTitle("Ilosc zdarzen");
	modules->GetYaxis()->SetTitleOffset(1.5);
	modules->SetMinimum(0);
	
	ribbon_stats[0]=new TH1I("Array[0]","Array[0]",4,0,4);
	ribbon_stats[1]=new TH1I("Array[1]","Array[1]",4,0,4);
	ribbon_stats[2]=new TH1I("Ribbon[0]","Ribbon[0]",4,0,4);
	
	modules->SetStats(kFALSE);
	for (int module=0; module<3; module++)
	{
		ribbon_stats[module]->SetStats(kFALSE);
		for (int label=0; label<4; label++) ribbon_stats[module]->GetXaxis()->SetBinLabel(label+1, ribbon_stats_Xaxis[label].c_str());
		ribbon_stats[module]->SetBarWidth(1);
		ribbon_stats[module]->SetBarOffset(0);
 		ribbon_stats[module]->SetFillColor(12);
		ribbon_stats[module]->GetXaxis()->SetLabelSize(0.05);
		ribbon_stats[module]->GetYaxis()->SetTitle("Ilosc zdarzen");
		ribbon_stats[module]->GetYaxis()->SetTitleOffset(1.5);
	}
}


cuts_stats::~cuts_stats()
{
	delete modules;
	for (int i=0; i<3; i++) delete ribbon_stats[i];
}


void cuts_stats::Fill()
{
	modules->Fill(0);
	for (int module=0; module<3; module++) ribbon_stats[module]->Fill(0);
		
	if (event->reject.Array[0]==0)
	{
		modules->Fill(1);
		if (event->reject.Array[1]==0)
		{
			modules->Fill(2);
			if (event->reject.Ribbon[0][0]==0)
			{
				modules->Fill(3);
			}
		}
	}
	
	int reject_status=0;
	
	reject_status=event->reject.Array[0];
	if (reject_status!=0) ribbon_stats[0]->Fill(reject_status);
	
	reject_status=event->reject.Array[1];
	if (reject_status!=0) ribbon_stats[1]->Fill(reject_status);
	
	reject_status=event->reject.Ribbon[0][0];
	if (reject_status!=0) ribbon_stats[2]->Fill(reject_status);
}

void cuts_stats::Save()
{
	cout<<"Saving cuts statistics into ./cuts_stats/png_files directory"<<endl;
	
	TCanvas * cnv= new TCanvas;
	cnv->cd();
	string path_png, path_cuts;
	path_cuts="./cuts_stats";
	mkdir(path_cuts.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png=path_cuts+"/png_files";
	mkdir(path_png.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	string filename;
	
	filename=path_png+"/modules.png";
	modules->Draw("bar2");
	cnv->SaveAs(filename.c_str());
	
	filename=path_png+"/Array0.png";
	ribbon_stats[0]->Draw("bar2");
	cnv->SaveAs(filename.c_str());
	
	filename=path_png+"/Array1.png";
	ribbon_stats[1]->Draw("bar2");
	cnv->SaveAs(filename.c_str());
	
	filename=path_png+"/Ribbon0.png";
	ribbon_stats[2]->Draw("bar2");
	cnv->SaveAs(filename.c_str());
	
	cnv->Close();
}
