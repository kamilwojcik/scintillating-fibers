#include "correlations.h"

correlations::correlations(load* input, int maxArr0, int maxArr1, int maxRib0, bool max_condition)
{
	event=input;
	max_conditions=max_condition;
	max_Arr0=maxArr0;
	max_Arr1=maxArr1;
	max_Rib0=maxRib0;
	
	string name;
	
	for (int x=0; x<10; x++) for (int y=0; y<10; y++)
	{
		name="Correlation_Array0_"+to_string(x)+"-"+to_string(y);
		if (max_conditions) name+="_max_on_"+to_string(max_Arr0);
		corr_histos[0][x][y]=new TH2F(name.c_str(),name.c_str(), 128,0,1.8, 128,0,1.8);
		corr_histos[0][x][y]->SetStats(kFALSE);
		corr_histos[0][x][y]->SetMinimum(0);
		corr_histos[0][x][y]->SetMaximum(1000);
		
		name="Correlation_Array1_"+to_string(x)+"-"+to_string(y);
		if (max_conditions) name+="_max_on_"+to_string(max_Arr1);
		corr_histos[1][x][y]=new TH2F(name.c_str(),name.c_str(), 128,0,1.8, 128,0,1.8);
		corr_histos[1][x][y]->SetStats(kFALSE);
		corr_histos[1][x][y]->SetMinimum(0);
		corr_histos[1][x][y]->SetMaximum(1000);
		
		name="Correlation_Ribbon0_"+to_string(x)+"-"+to_string(y);
		if (max_conditions) name+="_max_on_"+to_string(max_Rib0);
		corr_histos[2][x][y]=new TH2F(name.c_str(),name.c_str(), 128,0,1.8, 128,0,1.8);
		corr_histos[2][x][y]->SetStats(kFALSE);
		corr_histos[2][x][y]->SetMinimum(0);
		corr_histos[2][x][y]->SetMaximum(1000);
	}
}

correlations::~correlations()
{
	for (int module=0; module<3; module++) for (int x=0; x<10; x++) for (int y=0; y<10; y++)
	{
		delete corr_histos[module][x][y];
	}
}


void correlations::Fill()
{
	for (int x=0; x<10; x++) for (int y=0; y<10; y++)
	{
		if (max_conditions)
		{
			if (event->WhereMaxArray[0]==max_Arr0)
			{
				corr_histos[0][x][y]->Fill(event->floatEvent->qdcArray[0][x],event->floatEvent->qdcArray[0][y]);
			}
			
			if (event->WhereMaxArray[1]==max_Arr1)
			{
				corr_histos[1][x][y]->Fill(event->floatEvent->qdcArray[1][x],event->floatEvent->qdcArray[1][y]);
			}
			
			if (event->WhereMaxRibbon[0][0]==max_Rib0)
			{
				corr_histos[2][x][y]->Fill(event->floatEvent->qdcRibbon[0][0][x],event->floatEvent->qdcRibbon[0][0][y]);
			}
		}
		else
		{
			corr_histos[0][x][y]->Fill(event->floatEvent->qdcArray[0][x],event->floatEvent->qdcArray[0][y]);
			corr_histos[1][x][y]->Fill(event->floatEvent->qdcArray[1][x],event->floatEvent->qdcArray[1][y]);
			corr_histos[2][x][y]->Fill(event->floatEvent->qdcRibbon[0][0][x],event->floatEvent->qdcRibbon[0][0][y]);
		}
		
	}
}

void correlations::Save()
{
	cout<<"Saving previews of events into ./events/png_files directory"<<endl;
	
	string path_png, path_corr;
	path_corr="./correlations";
	mkdir(path_corr.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png=path_corr+"/png_files";
	mkdir(path_png.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png+="/";
	string filename;
	
	TCanvas *cnv=new TCanvas("cnv","cnv",890,800);
	cnv->cd();
	cnv->SetRightMargin(0.12);
	for (int x=0; x<10; x++) for (int y=0; y<10; y++)
	{
		filename=path_png+"Correlation_Array0_"+to_string(x)+"-"+to_string(y);
		if (max_conditions) filename+="_max_on_"+to_string(max_Arr0);
		filename+=".png";
		corr_histos[0][x][y]->Draw("colz");
		cnv->SaveAs(filename.c_str());
		
		filename=path_png+"Correlation_Array1_"+to_string(x)+"-"+to_string(y);
		if (max_conditions) filename+="_max_on_"+to_string(max_Arr1);
		filename+=".png";
		corr_histos[1][x][y]->Draw("colz");
		cnv->SaveAs(filename.c_str());
		
		filename=path_png+"Correlation_Ribbon0_"+to_string(x)+"-"+to_string(y);
		if (max_conditions) filename+="_max_on_"+to_string(max_Rib0);
		filename+=".png";
		corr_histos[2][x][y]->Draw("colz");
		cnv->SaveAs(filename.c_str());
	}
	cnv->Close();
}

