#!/bin/sh
# CreateFileList.pl -n 4000 -run 40 -type 4 DST_TRKR_CLUSTER DST_TRACKS DST_TRKR_HIT DST_TRKR_G4HIT DST_TRUTH_G4HIT DST_TRUTH DST_VERTEX

# No-pileup samples
CreateFileList.pl -type 4 -nopileup DST_CALO_CLUSTER DST_TRKR_HIT DST_TRUTH G4Hits