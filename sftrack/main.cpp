#include <iostream>
#include "gain.h"
#include "interfaceStructs.h"
#include "args.h"
#include "load.h"
#include "pedestal.h"
#include "channelspectrum.h"
#include "cuts_stats.h"
#include "events_preview.h"
#include "tracks.h"
#include "correlations.h"
#include <algorithm>

using namespace std;

int main(int argc, char **argv)
{
	args input_args(argc, argv);
	
	if (input_args.setup.beam_in_file!="" && input_args.args_correct() && input_args.setup.gain_in_file!="" && input_args.setup.ped_in_file!="")
	{
		gain g1;
		pedestal ped(input_args.setup);
		load event(input_args.setup.beam_in_file, true);
		channelSpectrum spectra(&event, 1024, 2);
		channelSpectrum spectraMax(&event, "Max_", "", 1024, 2, true); //filled only with max values. Name prefix="Max_"
		cuts_stats cuts(&event);
		tracks tracks_analiser(&event);
		correlations corrs(&event);
		correlations corrsMax(&event,6,4,3,true);
		
		TCanvas * cnv=new TCanvas;
		cnv->cd();
		events_preview event_preview(&event, cnv);
		
		g1.setOnes();
		
		event.setup(input_args.setup.beam_in_file, true);
		event.setPedestal(&ped);
		
		g1.load(input_args.setup.gain_in_file);
		g1.print();
		event.setGain(&g1);
		
			
		Long64_t rangeEnd;
		(input_args.setup.rangeMax) ? rangeEnd=event.GetNumberOfEvents() : rangeEnd=input_args.setup.rangeEnd;
		
		for (Long64_t i=0; i<rangeEnd; i++)
		{
			event.eventByIndex(i);
			cout<<"Event: "<<i<<"\r"<<flush;
			
			cuts.Fill();
			tracks_analiser.FillTracks();
			spectra.Fill();
			spectraMax.Fill();
			corrs.Fill();
			corrsMax.Fill();
			
			if (i<40)
			{
				event_preview.Reload(i);
				event_preview.Save();
			}
		}
		cout<<endl;
		
		cuts.Save();
		
		tracks_analiser.MakeMapWhereMax();
		tracks_analiser.Save();

		spectra.Save();
		spectraMax.Save();
		
		corrs.Save();
		corrsMax.Save();
		
		cout<<"\n    ~~  Live long and prosper  ~~\n"<<endl;
	}
	else cout<<"Something wrong with the arguments"<<endl;
	return 0;
}
