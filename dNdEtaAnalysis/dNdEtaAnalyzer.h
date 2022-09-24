// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef DNDETAANALYZER_H
#define DNDETAANALYZER_H

#include <fun4all/SubsysReco.h>

#include <g4eval/SvtxEvalStack.h>
#include <g4eval/SvtxClusterEval.h>
#include <g4eval/SvtxHitEval.h>
#include <g4eval/SvtxTruthEval.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4VtxPoint.h>
#include <phool/getClass.h>

#include <trackbase/MvtxDefs.h>
#include <phool/getClass.h>
#include <trackbase/TrkrCluster.h>
#include <trackbase/TrkrClusterv4.h>
#include <trackbase/TrkrClusterContainerv4.h>
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHitSetContainer.h>
#include <trackbase/TrkrDefs.h>
#include <trackbase/TrkrClusterHitAssoc.h>
#include <trackbase/ActsGeometry.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxVertexMap.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/ActsTransformations.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>

class PHCompositeNode;
class SvtxTrack;
class SvtxTrackMap;
class TrkrHitSetContainer;
class TrkrClusterContainer;

class dNdEtaAnalyzer : public SubsysReco
{
public:
    dNdEtaAnalyzer(const std::string &name = "dNdEtaAnalyzer", const bool &isData = false, const int &iniEvt = 0);

    ~dNdEtaAnalyzer() override;

    /** Called during initialization.
        Typically this is where you can book histograms, and e.g.
        register them to Fun4AllServer (so they can be output to file
        using Fun4AllServer::dumpHistos() method).
     */
    int Init(PHCompositeNode *topNode) override;

    /** Called for first event when run number is known.
        Typically this is where you may want to fetch data from
        database, because you know the run number. A place
        to book histograms which have to know the run number.
     */
    int InitRun(PHCompositeNode *topNode) override;

    /** Called for each event.
        This is where you do the real work.
     */
    int process_event(PHCompositeNode *topNode) override;

    /// Clean up internals after each event.
    int ResetEvent(PHCompositeNode *topNode) override;

    /// Called at the end of each run.
    int EndRun(const int runnumber) override;

    /// Called at the end of all processing.
    int End(PHCompositeNode *topNode) override;

    /// Reset
    int Reset(PHCompositeNode * /*topNode*/) override;

    void Print(const std::string &what = "ALL") const override;
    void GetTruthPV(bool b) { _get_truth_pv = b; }
    void GetG4Particle(bool b) { _get_g4particle = b; }
    void GetRecoCluster(bool b) { _get_reco_cluster = b; }
    void GetMVTXhits(bool b) { _get_mvtx_hit = b; }

private:
    void ResetVectors();
    void GetTruthPVInfo(PHCompositeNode *topNode);
    void GetG4ParticleInfo(PHCompositeNode *topNode);
    void GetRecoClusterInfo(PHCompositeNode *topNode);
    void GetMVTXhitsInfo(PHCompositeNode *topNode);

    bool _get_truth_pv;
    bool _get_g4particle;
    bool _get_reco_cluster;
    bool _get_mvtx_hit;

    unsigned int eventNum = 0;
    bool IsData;
    int IniEvt;

    TTree *outtree;
    int event_;
    // Truth primary vertex information
    float TruthPV_trig_x_, TruthPV_trig_y_, TruthPV_trig_z_, TruthPV_mostNpart_x_, TruthPV_mostNpart_y_, TruthPV_mostNpart_z_;
    int NTruthVtx_, TruthPV_trig_Npart_, TruthPV_mostNpart_Npart_;
    std::vector<float> TruthPV_x_, TruthPV_y_, TruthPV_z_, TruthPV_t_, TruthPV_embed_;
    std::vector<int> TruthPV_Npart_, TruthPV_Nhits_, TruthPV_NClus_;
    // Truth G4Particle information
    std::vector<int> G4PartPID_, G4PartTrackID_, G4PartVtxID_, G4PartParentID_, G4PartPrimaryID_;
    std::vector<float> G4PartPx_, G4PartPy_, G4PartPz_, G4PartPt_, G4PartEta_, G4PartPhi_, G4PartE_;
    // Reconstructed cluster information
    std::vector<int> ClusLayer_;
    std::vector<float> ClusX_, ClusY_, ClusZ_, ClusR_, ClusPhi_, ClusEta_;
    std::vector<unsigned int> ClusAdc_;
    std::vector<float> ClusPhiSize_, ClusZSize_;
    // MVTX hit information
    std::vector<unsigned int> HitLayerID_, HitStaveID_, HitChipID_;
    std::vector<uint16_t> HitRow_, HitColumn_;
    std::vector<float> HitX_, HitY_, HitZ_;
    std::vector<float> HitfromG4P_X_, HitfromG4P_Y_, HitfromG4P_Z_;

    // SvtxTrackMap *dst_trackmap = nullptr;
    // SvtxVertexMap *dst_vertexmap = nullptr;

    // TrkrClusterContainerv4 *dst_clustermap = nullptr;
    TrkrClusterContainer *dst_clustermap = nullptr;
    TrkrClusterHitAssoc *clusterhitmap = nullptr;
    TrkrHitSetContainer *hitsets = nullptr;

    TrkrDefs::hitkey hitKey;
    TrkrDefs::cluskey clusKey;

    ActsGeometry *_tgeometry = nullptr;

    SvtxEvalStack *svtx_evalstack;
    SvtxTruthEval *truth_eval;
    SvtxClusterEval *clustereval;
    SvtxHitEval *hiteval;
    PHG4TruthInfoContainer *m_truth_info;
};

#endif // DNDETAANALYZER_H
