#include "channelspectrum.h"
#include "interfaceStructs.h"



channelSpectrum::channelSpectrum(int bins, int range)
{
	setup(bins, range);
}


channelSpectrum::channelSpectrum(const char* name_prefix, const char* name_suffix, int bins, int range)
{
	setup(bins, range);
	SetNamePrefix(name_prefix);
	SetNameSuffix(name_suffix);
}

channelSpectrum::~channelSpectrum()
{
}

bool channelSpectrum::setup(int bins, int range)
{
	nbins=bins;
	rangeFromZeroTo=range;
	
	string hname;
	
	//generating histograms with proper names and titles
	for (int j=0;j<2;j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			hname="Spectrum_QDC_Array_"+to_string(j)+"_channel_"+to_string(channel);
			Array[j][channel] = new TH1D(hname.data(),hname.data(),nbins,0,rangeFromZeroTo);
			for (int layer=0; layer<4; layer++)
			{
				hname="Spectrum_QDC_Ribbon_"+to_string(j)+"_channel_"+to_string(channel)+"_layer_"+to_string(layer);
				Ribbon[j][layer][channel] = new TH1D(hname.data(),hname.data(),nbins,0,rangeFromZeroTo);
			}
		}
	}
	return true;
}

bool channelSpectrum::Fill()
{
	for (int j=0;j<2;j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			Array[j][channel]->Fill(event->floatEvent->qdcArray[j][channel]);
		}
		
		for (int layer=0; layer<3; layer++)
		{
			for (int channel=0; channel<16; channel++)
			{
				Ribbon[j][layer][channel]->Fill(event->floatEvent->qdcRibbon[j][layer][channel]);
			}
		}
	}
	return true;
}

bool channelSpectrum::loopLoad(load * input, Long64_t rangeBegin, Long64_t rangeEnd)
{
	event = input;
	if (rangeEnd>event->GetNumberOfEvents()) rangeEnd=event->GetNumberOfEvents();
	for (Long64_t i=rangeBegin; i<rangeEnd; i++)
	{
		event->eventByIndex(i);
		Fill();
	}
	return true;
}


bool channelSpectrum::Fit()
{
	double *params;

	for (int j=0;j<2;j++)
	{
		
		for (int channel=0; channel<16; channel++)
		{
			if (Array[j][channel]->GetMean()!=0)
			{
				Array[j][channel]->Fit("gaus","Q",0,rangeFromZeroTo);
				params=Array[j][channel]->GetFunction("gaus")->GetParameters();
			}
			else
			{
				cout<<"    Spectrum Fit warning: Array["<<j<<"]["<<channel<<"] is empty. Skipping..."<<endl;
			}
			for (int layer=0; layer<3; layer++)
			{
				if (Ribbon[j][layer][channel]->GetMean()!=0)
				{
					Ribbon[j][layer][channel]->Fit("gaus","Q",0,rangeFromZeroTo);
					params=Ribbon[j][layer][channel]->GetFunction("gaus")->GetParameters();
				}
				else
				{
					cout<<"    Spectrum Fit warning: Ribbon["<<j<<"]["<<layer<<"]["<<channel<<"] is empty. Skipping..."<<endl;
				}
			}
		}
	}
	return true;
}

bool channelSpectrum::Save(const char* path)
{
	cout<<"Saving spectra into "<<path<<"/spectra/root_files and "<<path<<"/spectra/png_files directories"<<endl;
	
	TCanvas * cnv= new TCanvas;
	cnv->cd();
	cnv->SetLogy();
	string path_root, path_png, path_spectra;
	path_spectra=path;
	path_spectra+="/spectra";
	mkdir(path_spectra.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_root=path_spectra+"/root_files";
	path_png=path_spectra+"/png_files";
	mkdir(path_root.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	mkdir(path_png.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	string filename;
	
	for (int j=0;j<2;j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			Array[j][channel]->Draw();
			filename=path_png+"/"+prefix+"Spectrum_Array_"+to_string(j)+"_channel_"+to_string(channel)+".png";
			cnv->SaveAs(filename.data());
			
			filename=path_root+"/"+prefix+"Spectrum_Array_"+to_string(j)+"_channel_"+to_string(channel)+".root";
			cnv->SaveAs(filename.data());
			
			for (int layer=0; layer<4; layer++)
			{
				Ribbon[j][layer][channel]->Draw();
				filename=path_png+"/"+prefix+"Spectrum_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel)+".png";
				cnv->SaveAs(filename.data());
				
				filename=path_root+"/"+prefix+"Spectrum_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel)+".root";
				cnv->SaveAs(filename.data());
			}
		}
	}
	delete cnv;
	cout<<"...ok."<<endl;
	return true;
}

bool channelSpectrum::SetNamePrefix(const char* name_prefix)
{
	string hname;
	prefix=name_prefix;
	
	for (int j=0;j<2;j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			hname=prefix+"Spectrum_QDC_Array_"+to_string(j)+"_channel_"+to_string(channel)+suffix;
			Array[j][channel]->SetName(hname.data());
			Array[j][channel]->SetTitle(hname.data());
			for (int layer=0; layer<4; layer++)
			{
				hname=prefix+"Spectrum_QDC_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel)+suffix;
				Ribbon[j][layer][channel]->SetName(hname.data());
				Ribbon[j][layer][channel]->SetTitle(hname.data());
			}
		}
	}
	return true;
}

bool channelSpectrum::SetNameSuffix(const char* name_suffix)
{
	string hname;
	suffix=name_suffix;
	
	for (int j=0;j<2;j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			hname=prefix+"Spectrum_QDC_Array_"+to_string(j)+"_channel_"+to_string(channel)+suffix;
			Array[j][channel]->SetName(hname.data());
			Array[j][channel]->SetTitle(hname.data());
			for (int layer=0; layer<4; layer++)
			{
				hname=prefix+"Spectrum_QDC_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel)+suffix;
				Ribbon[j][layer][channel]->SetName(hname.data());
				Ribbon[j][layer][channel]->SetTitle(hname.data());
			}
		}
	}
	return true;
}