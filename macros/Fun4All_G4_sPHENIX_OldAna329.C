// #ifndef MACRO_FUN4ALLG4SPHENIX_C
// #define MACRO_FUN4ALLG4SPHENIX_C

#include <GlobalVariables.C>

#include <G4Setup_sPHENIX.C>
#include <G4_DSTReader.C>
#include <G4_Input.C>
#include <G4_Tracking.C>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <dndetaanalyzer/dNdEtaAnalyzer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libdNdEtaAnalyzer.so)

int Fun4All_G4_sPHENIX_OldAna329(const int nEvents = 1, const int inputfilelistidx = 1879, const string &outputFile = "MVTXRecoClusters_test", const int skip = 0)
{
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(0);

    Input::VERBOSITY = INT_MAX;
    Input::READHITS = true;
    const vector<string> &filelist = {Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/dst_calo_cluster_sHijing_0_20fm_%d.list", inputfilelistidx),
                                      Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/dst_trkr_hit_sHijing_0_20fm_%d.list", inputfilelistidx),
                                      Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/dst_truth_sHijing_0_20fm_%d.list", inputfilelistidx),
                                      Form("/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/g4hits_sHijing_0_20fm_%d.list", inputfilelistidx)};

    for (unsigned int i = 0; i < filelist.size(); ++i)
    {
        INPUTREADHITS::listfile[i] = filelist[i];
    }

    Enable::MICROMEGAS = true;
    // TrkrHit reconstructions
    // Mvtx_Cells();
    // Intt_Cells();
    // TPC_Cells();
    // Micromegas_Cells();
    // Load ActsGeometry object
    TrackingInit();
    // Reco clustering
    // Mvtx_Clustering();
    // Intt_Clustering();
    // TPC_Clustering();
    // Micromegas_Clustering();

    auto vtxing = new PHTruthVertexing;
    vtxing->associate_tracks(false); // This is set to false because we do not run tracking
    se->registerSubsystem(vtxing);

    bool isData = false;
    // dNdEtaAnalyzer *myAnalyzer = new dNdEtaAnalyzer(outputFile, isData, 0);
    dNdEtaAnalyzer *myAnalyzer = new dNdEtaAnalyzer(outputFile, isData, inputfilelistidx);
    myAnalyzer->GetTruthPV(true);
    myAnalyzer->GetG4Particle(true);
    myAnalyzer->GetRecoCluster(true);
    myAnalyzer->GetMVTXhits(true);
    se->registerSubsystem(myAnalyzer);

    InputManagers();

    // se->skip(skip);
    se->run(nEvents);

    se->End();
    std::cout << "All done" << std::endl;
    delete se;

    gSystem->Exit(0);
    return 0;
}