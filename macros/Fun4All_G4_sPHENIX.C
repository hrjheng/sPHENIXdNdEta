#ifndef MACRO_FUN4ALLG4SPHENIX_C
#define MACRO_FUN4ALLG4SPHENIX_C

#include <GlobalVariables.C>

#include <G4_Centrality.C>
#include <G4_Input.C>
#include <Trkr_Clustering.C>
#include <Trkr_RecoInit.C>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <g4centrality/PHG4CentralityReco.h>
#include <trackreco/PHTruthVertexing.h>

#include <dndetaanalyzer/dNdEtaAnalyzer.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libdNdEtaAnalyzer.so)
R__LOAD_LIBRARY(libcentrality_io.so)
R__LOAD_LIBRARY(libg4centrality.so)

// For HepMC Hijing
// try inputFile = /sphenix/sim/sim01/sphnxpro/sHijing_HepMC/sHijing_0-12fm.dat

int Fun4All_G4_sPHENIX(const int nEvents = 1, const int inputfilelistidx = 0, const string &outputFile = "MVTXRecoClusters_test", const int skip = 0)
{
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(0);

    recoConsts *rc = recoConsts::instance();

    Input::VERBOSITY = 0;
    Input::READHITS = true;
    // const vector<string> &filelist = {Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/dst_calo_cluster.list"), Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/dst_trkr_hit.list"),
                                    //   Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/dst_truth.list"), Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/g4hits.list")};
    const vector<string> &filelist = {"/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/dNdEta_INTT/dst_INTTdNdEta.list"};

    for (unsigned int i = 0; i < filelist.size(); ++i)
    {
        INPUTREADHITS::listfile[i] = filelist[i];
    }

    // register all input generators with Fun4All
    InputRegister();

    Enable::MVTX = true;
    Enable::INTT = true;

    //===============
    // conditions DB flags
    //===============
    Enable::CDB = true;
    // global tag
    rc->set_StringFlag("CDB_GLOBALTAG", CDB::global_tag);
    // 64 bit timestamp
    rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);

    // TrkrHit reconstructions
    // Mvtx_Cells();
    // Load ActsGeometry object
    TrackingInit();
    // Reco clustering
    // Mvtx_Clustering();

    //-----------------
    // Centrality Determination
    //-----------------
    // Centrality();
    PHG4CentralityReco *cent = new PHG4CentralityReco();
    cent->Verbosity(0);
    cent->GetCalibrationParameters().ReadFromFile("centrality", "xml", 0, 0, string(getenv("CALIBRATIONROOT")) + string("/Centrality/"));
    se->registerSubsystem(cent);

    auto vtxing = new PHTruthVertexing;
    vtxing->associate_tracks(false); // This is set to false because we do not run tracking
    se->registerSubsystem(vtxing);

    bool isData = false;
    // dNdEtaAnalyzer *myAnalyzer = new dNdEtaAnalyzer(outputFile, isData, 0);
    dNdEtaAnalyzer *myAnalyzer = new dNdEtaAnalyzer(outputFile, isData, inputfilelistidx);
    myAnalyzer->GetTruthPV(true);
    myAnalyzer->GetG4Particle(false);
    myAnalyzer->GetRecoCluster(true);
    myAnalyzer->GetMVTXhits(true);
    myAnalyzer->GetCentrality(true);
    se->registerSubsystem(myAnalyzer);

    //--------------
    // Set up Input Managers
    //--------------
    InputManagers();

    //-----------------
    // Event processing
    //-----------------
    se->skip(skip);
    se->run(nEvents);

    //-----
    // Exit
    //-----
    se->End();
    std::cout << "All done" << std::endl;
    delete se;

    gSystem->Exit(0);
    return 0;
}
#endif
