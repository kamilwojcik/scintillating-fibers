#include "channelspectrum.h"
#include "interfaceStructs.h"



channelSpectrum::channelSpectrum(load *evnt, int bins, int range, bool max_condition)
{
	setup(evnt, bins, range, max_condition);
}


channelSpectrum::channelSpectrum(load *evnt, const char* pref, const char* suff, int bins, int range, bool max_condition)
{
	setup(evnt, bins, range, max_condition);
	SetNamePrefix(pref);
	SetNameSuffix(suff);
}

channelSpectrum::~channelSpectrum()
{
}

bool channelSpectrum::setup(load *evnt, int bins, int range, bool max_condition)
{
	onlyMax=max_condition;
	event=evnt;
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
		}
	
		for (int layer=0; layer<3; layer++)
		{
			for (int channel=0; channel<16; channel++)
			{
				hname="Spectrum_QDC_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel);
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
		if (event->reject.Array[j]==0)
		{
			for (int channel=0; channel<16; channel++)
			{
				if (event->WhereMaxArray[j]==channel || onlyMax==false)
				{
					Array[j][channel]->Fill(event->floatEvent->qdcArray[j][channel]);
				}
			}
		}
		
		for (int layer=0; layer<3; layer++)
		{
			if (event->reject.Ribbon[j][layer]==0)
			{
				for (int channel=0; channel<16; channel++)
				{
					if (event->WhereMaxRibbon[j][layer]==channel || onlyMax==false)
					{
						Ribbon[j][layer][channel]->Fill(event->floatEvent->qdcRibbon[j][layer][channel]);
					}
				}
			}
		}
	}
	return true;
}


bool channelSpectrum::Save(const char* path)
{
	cout<<"Saving spectra into "<<path<<"/spectra/png_files directories"<<endl;
	
	TCanvas * cnv= new TCanvas;
	cnv->cd();
	cnv->SetLogy();
	string path_png, path_spectra;
	path_spectra=path;
	path_spectra+="/spectra";
	mkdir(path_spectra.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	path_png=path_spectra+"/png_files";
	mkdir(path_png.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	string filename;
	
	for (int j=0;j<2;j++)
	{
		for (int channel=0; channel<16; channel++)
		{
			Array[j][channel]->Draw();
			filename=path_png+"/"+prefix+"Spectrum_Array_"+to_string(j)+"_channel_"+to_string(channel)+"].png";
			cnv->SaveAs(filename.data());
			
			for (int layer=0; layer<3; layer++)
			{
				Ribbon[j][layer][channel]->Draw();
				filename=path_png+"/"+prefix+"Spectrum_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel)+"].png";
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
			
			for (int layer=0; layer<3; layer++)
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
			
			for (int layer=0; layer<3; layer++)
			{
				hname=prefix+"Spectrum_QDC_Ribbon_"+to_string(j)+"_layer_"+to_string(layer)+"_channel_"+to_string(channel)+suffix;
				Ribbon[j][layer][channel]->SetName(hname.data());
				Ribbon[j][layer][channel]->SetTitle(hname.data());
			}
		}
	}
	
	return true;
}
