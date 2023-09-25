#ifndef MACRO_INPUTMANAGERTEST_C
#define MACRO_INPUTMANAGERTEST_C

#include <GlobalVariables.C>

#include "FROG.h"
#include "TSystem.h"
#include "TString.h"
#include "TROOT.h"

#include <DisplayOn.C>
#include <G4Setup_sPHENIX.C>
#include <G4_Bbc.C>
#include <G4_CaloTrigger.C>
#include <G4_DSTReader.C>
#include <G4_Global.C>
#include <G4_HIJetReco.C>
#include <G4_Input.C>
#include <G4_Jets.C>
#include <G4_KFParticle.C>
#include <G4_ParticleFlow.C>
#include <G4_Production.C>
#include <G4_TopoClusterReco.C>
#include <G4_Tracking.C>
#include <G4_User.C>
#include <QA.C>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)

// For HepMC Hijing
// try inputFile = /sphenix/sim/sim01/sphnxpro/sHijing_HepMC/sHijing_0-12fm.dat

int Fun4All_InputManagerTest(const int nEvents = 1, const int skip = 0)
{
    bool runDisplay = false;

    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(INT_MAX);

    Input::READHITS = true;

    // Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
    PHRandomSeed::Verbosity(1);

    const vector<string> &filelist = {
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/clustertest/dst_calo_cluster.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/clustertest/dst_trkr_hit.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/clustertest/dst_truth.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/clustertest/g4hits.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/clustertest/dst_tracks.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/clustertest/dst_trkr_cluster.list"
    };

    for (unsigned int i = 0; i < filelist.size(); ++i)
    {
        INPUTREADHITS::listfile[i] = filelist[i];
    }

    TrackingInit();

    se->skip(skip);
    se->run(nEvents);

    //-----
    // Exit
    //-----

    (Fun4AllServer::instance())->PrintTimer();

    se->End();
    std::cout << "All done" << std::endl;
    delete se;

    gSystem->Exit(0);
    return 0;
}
#endif
