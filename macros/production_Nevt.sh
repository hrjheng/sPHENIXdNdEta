#! /bin/bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}
# source /opt/sphenix/core/bin/sphenix_setup.sh -n new
# source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.325
source /cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh -n ana.329

export MYINSTALL=/sphenix/u/hjheng/install
export LD_LIBRARY_PATH=$MYINSTALL/lib:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=$MYINSTALL/include:$ROOT_INCLUDE_PATH

source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

# print the environment - needed for debugging
# printenv
# this is how you run your Fun4All_G4_sPHENIX.C macro in batch:
root.exe -q -b Fun4All_G4_sPHENIX.C\($1,$2,\"$3\"\,$4\)

echo all done
