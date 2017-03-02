Scintillating Sibers (SciFi) Beam Position Detector  -- resolution analysis tools

To remember:

0. compilation: root-config, --std=c++11

1. pedestal.root -> sfpedestal -> pedestal.info
2. pedestal.info + beam.root -> sfgain -> corrected spectra + gain_inv.info
    corrected spectra --[positions of argon peak]--> gain_inv.info
    gain_inv.info -> sfgain -> gain.info
    gain.info +pedestal.info + beam.root -> sfpedestal -> calibrated spectra

3.  calibrated spectra --[argon cut-off levels]--> sftrack
    gain.info + pedestal.info + beam.root -> sftrack -> final: spectra + cut stats + track stats + preview of events + correlations

output contains of many things to analyze ;)

