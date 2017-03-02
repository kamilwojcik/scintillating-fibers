#include "gain.h"
#include "interfaceStructs.h"
#include "args.h"
#include "load.h"
#include "pedestal.h"
#include <iostream>
#include "channelspectrum.h"

using namespace std;

int main(int argc, char **argv) {
    
	args input_args(argc, argv);
	
	gain g1;
	g1.setOnes();
	
	channelSpectrum spectra;
	pedestal ped(input_args.setup);
	load event;
	
	event.setGain(&g1);
	
	if (input_args.setup.beam_in_file!="" && input_args.args_correct())
	{
		event.setup(input_args.setup.beam_in_file);
		event.setPedestal(&ped);
		cout<<"Pedestal correction?: "<<event.GetPedStatus()<<endl;
		if (input_args.setup.gain_in_file=="")
		{
			//creating spectra, map.info and gain.info
			Long64_t rangeEnd;
			(input_args.setup.rangeMax) ? rangeEnd=event.GetNumberOfEvents() : rangeEnd=input_args.setup.rangeEnd; 
			
			spectra.loopLoad(&event, input_args.setup.rangeBegin, rangeEnd);
			spectra.Save(".");
			g1.saveAs((string)"gain_inv.info");
		}
		else
		{
			g1.load(input_args.setup.gain_in_file);
			g1.print();
			event.setGain(&g1);
			
			Long64_t rangeEnd;
			(input_args.setup.rangeMax) ? rangeEnd=event.GetNumberOfEvents() : rangeEnd=input_args.setup.rangeEnd;
			
			spectra.setup(1024,2);
			spectra.loopLoad(&event, input_args.setup.rangeBegin, rangeEnd);
			spectra.Save(".");
		}
	}
	else
	{
		if (input_args.setup.gain_in_file!="")
		{
			g1.load(input_args.setup.gain_in_file);
			cout<<"Inverting gains"<<endl;
			g1.invert();
			g1.saveAs("gain.info");
			cout<<"Saved."<<endl;
		}
		else cout<<"Nothing more to do."<<endl;
	}
    return 0;
}
