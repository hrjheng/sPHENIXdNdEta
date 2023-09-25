#ifndef MACRO_READCLUSTERSRUNTRACKING_C
#define MACRO_READCLUSTERSRUNTRACKING_C

#include <GlobalVariables.C>

#include <G4_Tracking.C>

#include <g4eval/SvtxEvaluator.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include "FROG.h"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4eval.so)

void ReadHitsRunClusteringOnly(const int nEvents = 1)
{
    const int nJob = 1;
    // const int nEvents = 1000;
    // const float pTval = 1.0;
    // const int nOff = 1000 + nJob;

    const vector<string> &embed_infile = {
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/dst_calo_cluster_sHijing_0_20fm_45.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/dst_trkr_hit_sHijing_0_20fm_45.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/dst_truth_sHijing_0_20fm_45.list",
        "/sphenix/user/hjheng/sPHENIXdNdEta/macros/list/test/g4hits_sHijing_0_20fm_45.list"};

    char outputFile[256];
    // sprintf(outputFile, "/sphenix/user/hjheng/TrackletAna/data/ClusterInfo_SvtxEvaluator_Nevt%d.root", nEvents);
    sprintf(outputFile, "/sphenix/user/hjheng/TrackletAna/data/NoPileup_ana325private_singleEvtDst_evt45_SvtxEvaluator.root");

    recoConsts *rc = recoConsts::instance();
    rc->set_IntFlag("RANDOMSEED", nJob);
    rc->set_IntFlag("RUNNUMER", nJob);

    // gSystem->Load("libFROG.so");
    // FROG *fr = new FROG();

    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(INT_MAX);

    Enable::MICROMEGAS = true;
    
    // Mvtx_Cells();
    // Intt_Cells();
    // TPC_Cells();
    // Micromegas_Cells();
    TrackingInit(); // initialize tracking
    // TPC_Clustering();
    // Mvtx_Clustering();
    // Intt_Clustering();
    // Micromegas_Clustering();
    // Tracking_Reco();

    // explicitely configure the evaluator
    SvtxEvaluator *eval;
    const int n_maps_layer = 3;
    const int n_intt_layer = 4;
    int n_gas_layer = 48;

    eval = new SvtxEvaluator("SVTXEVALUATOR", outputFile, "SvtxTrackMap", n_maps_layer, n_intt_layer, n_gas_layer);
    int do_default = 0;

    eval->do_cluster_eval(true);
    eval->do_g4cluster_eval(false);
    eval->do_hit_eval(true); // enable to see the hits that includes the chamber physics...

    eval->do_vertex_eval(false);
    eval->do_g4hit_eval(false);
    eval->do_gtrack_eval(false);
    eval->do_track_eval(false);
    eval->do_track_match(false);

    eval->do_gpoint_eval(false);
    eval->do_eval_light(true);
    eval->set_use_initial_vertex(true);
    eval->set_use_genfit_vertex(false);
    eval->scan_for_embedded(false); // take all tracks if false - take only embedded tracks if true
    eval->scan_for_primaries(true);
    eval->set_cluster_version(4);
    eval->Verbosity(0);
    se->registerSubsystem(eval);

    for (unsigned int i = 0; i < embed_infile.size(); ++i)
    {
        string mgrname = "DSTin" + to_string(i);
        Fun4AllInputManager *in = new Fun4AllDstInputManager(mgrname);
        in->AddListFile(embed_infile[i]);
        in->Verbosity(INT_MAX);
        se->registerInputManager(in);
    }

    // Enable::DSTOUT_COMPRESS = false;
    // DstOut::OutputFile = "./ReadHitsOnly.root";
    // string FullOutFile = DstOut::OutputFile;
    // Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", FullOutFile);
    // se->registerOutputManager(out);

    se->run(nEvents);
    std::cout << " Done Run, ending... " << std::endl;
    se->End();

    se->PrintTimer();

    std::cout << " Success!! " << std::endl;
    // deleting the server shows if the memory is corrupted, if the job dies here - it is
    delete se;
    gSystem->Exit(0);
}

#endif
