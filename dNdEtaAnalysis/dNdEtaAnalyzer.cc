//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in dNdEtaAnalyzer.h.
//
// dNdEtaAnalyzer(const std::string &name = "dNdEtaAnalyzer")
// everything is keyed to dNdEtaAnalyzer, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// dNdEtaAnalyzer::~dNdEtaAnalyzer()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int dNdEtaAnalyzer::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int dNdEtaAnalyzer::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int dNdEtaAnalyzer::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT;
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT;
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int dNdEtaAnalyzer::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int dNdEtaAnalyzer::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int dNdEtaAnalyzer::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int dNdEtaAnalyzer::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void dNdEtaAnalyzer::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "dNdEtaAnalyzer.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>

#include <phool/PHCompositeNode.h>

namespace
{
    template <class T>
    void CleanVec(std::vector<T> &v)
    {
        std::vector<T>().swap(v);
        v.shrink_to_fit();
    }
}

//____________________________________________________________________________..
dNdEtaAnalyzer::dNdEtaAnalyzer(const std::string &name, const bool &isData, const int &iniEvt) : SubsysReco(name), _get_truth_pv(true), _get_g4particle(true), _get_reco_cluster(true), _get_mvtx_hit(true), IsData(isData), IniEvt(iniEvt), svtx_evalstack(nullptr), truth_eval(nullptr), clustereval(nullptr), hiteval(nullptr), m_truth_info(nullptr)
{
    std::cout << "dNdEtaAnalyzer::dNdEtaAnalyzer(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
dNdEtaAnalyzer::~dNdEtaAnalyzer()
{
    std::cout << "dNdEtaAnalyzer::~dNdEtaAnalyzer() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::Init(PHCompositeNode *topNode)
{
    std::cout << "dNdEtaAnalyzer::Init(PHCompositeNode *topNode) Initializing" << std::endl
              << "Running on Data or simulation? -> IsData = " << IsData << std::endl
              << "Initial eventnum = " << IniEvt << std::endl;

    PHTFileServer::get().open(Form("/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/%s.root", Name().c_str()), "RECREATE");

    outtree = new TTree("EventTree", "EventTree");
    outtree->Branch("event", &event_);
    outtree->Branch("NTruthVtx", &NTruthVtx_);
    outtree->Branch("TruthPV_x", &TruthPV_x_);
    outtree->Branch("TruthPV_y", &TruthPV_y_);
    outtree->Branch("TruthPV_z", &TruthPV_z_);
    outtree->Branch("TruthPV_Npart", &TruthPV_Npart_);
    outtree->Branch("TruthPV_Nhits", &TruthPV_Nhits_);
    outtree->Branch("TruthPV_NClus", &TruthPV_NClus_);
    outtree->Branch("TruthPV_t", &TruthPV_t_);
    outtree->Branch("TruthPV_embed", &TruthPV_embed_);
    outtree->Branch("TruthPV_trig_x", &TruthPV_trig_x_);
    outtree->Branch("TruthPV_trig_y", &TruthPV_trig_y_);
    outtree->Branch("TruthPV_trig_z", &TruthPV_trig_z_);
    outtree->Branch("TruthPV_trig_Npart", &TruthPV_trig_Npart_);
    outtree->Branch("TruthPV_mostNpart_x", &TruthPV_mostNpart_x_);
    outtree->Branch("TruthPV_mostNpart_y", &TruthPV_mostNpart_y_);
    outtree->Branch("TruthPV_mostNpart_z", &TruthPV_mostNpart_z_);
    outtree->Branch("TruthPV_mostNpart_Npart", &TruthPV_mostNpart_Npart_);
    outtree->Branch("ClusLayer", &ClusLayer_);
    outtree->Branch("ClusX", &ClusX_);
    outtree->Branch("ClusY", &ClusY_);
    outtree->Branch("ClusZ", &ClusZ_);
    outtree->Branch("ClusR", &ClusR_);
    outtree->Branch("ClusPhi", &ClusPhi_);
    outtree->Branch("ClusEta", &ClusEta_);
    outtree->Branch("ClusAdc", &ClusAdc_);
    outtree->Branch("ClusPhiSize", &ClusPhiSize_);
    outtree->Branch("ClusZSize", &ClusZSize_);
    outtree->Branch("G4PartPID", &G4PartPID_);
    outtree->Branch("G4PartTrackID", &G4PartTrackID_);
    outtree->Branch("G4PartVtxID", &G4PartVtxID_);
    outtree->Branch("G4PartParentID", &G4PartParentID_);
    outtree->Branch("G4PartPrimaryID", &G4PartPrimaryID_);
    outtree->Branch("G4PartPt", &G4PartPt_);
    outtree->Branch("G4PartEta", &G4PartEta_);
    outtree->Branch("G4PartPhi", &G4PartPhi_);
    outtree->Branch("G4PartE", &G4PartE_);
    outtree->Branch("HitLayerID", &HitLayerID_);
    outtree->Branch("HitStaveID", &HitStaveID_);
    outtree->Branch("HitChipID", &HitChipID_);
    outtree->Branch("HitRow", &HitRow_);
    outtree->Branch("HitColumn", &HitColumn_);
    outtree->Branch("HitX", &HitX_);
    outtree->Branch("HitY", &HitY_);
    outtree->Branch("HitZ", &HitZ_);
    outtree->Branch("HitfromG4P_X", &HitfromG4P_X_);
    outtree->Branch("HitfromG4P_Y", &HitfromG4P_Y_);
    outtree->Branch("HitfromG4P_Z", &HitfromG4P_Z_);

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::InitRun(PHCompositeNode *topNode)
{
    std::cout << "dNdEtaAnalyzer::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::process_event(PHCompositeNode *topNode)
{
    std::cout << "dNdEtaAnalyzer::process_event(PHCompositeNode *topNode) Processing Event" << IniEvt + eventNum << std::endl;

    PHNodeIterator nodeIter(topNode);

    if (!svtx_evalstack)
    {
        svtx_evalstack = new SvtxEvalStack(topNode);
        clustereval = svtx_evalstack->get_cluster_eval();
        hiteval = svtx_evalstack->get_hit_eval();
        truth_eval = svtx_evalstack->get_truth_eval();
    }
    else
    {
        svtx_evalstack->next_event(topNode);
    }

    GetMVTXhitsInfo(topNode);

    if (!IsData)
    {
        std::cout << "Running on simulation" << std::endl;
        GetTruthPVInfo(topNode);
        GetG4ParticleInfo(topNode);
    }

    GetRecoClusterInfo(topNode);

    event_ = IniEvt + eventNum;
    outtree->Fill();
    eventNum++;

    ResetVectors();

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::ResetEvent(PHCompositeNode *topNode)
{
    std::cout << "dNdEtaAnalyzer::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::EndRun(const int runnumber)
{
    std::cout << "dNdEtaAnalyzer::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::End(PHCompositeNode *topNode)
{
    std::cout << "dNdEtaAnalyzer::End(PHCompositeNode *topNode) This is the End - Output to " << Form("/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/%s.root", Name().c_str()) << std::endl;

    PHTFileServer::get().cd(Form("/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/%s.root", Name().c_str()));
    outtree->Write("", TObject::kOverwrite);

    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int dNdEtaAnalyzer::Reset(PHCompositeNode *topNode)
{
    std::cout << "dNdEtaAnalyzer::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void dNdEtaAnalyzer::Print(const std::string &what) const
{
    std::cout << "dNdEtaAnalyzer::Print(const std::string &what) const Printing info for " << what << std::endl;
}

//____________________________________________________________________________..
void dNdEtaAnalyzer::GetMVTXhitsInfo(PHCompositeNode *topNode)
{
    std::cout << "Get MVTX hits info." << std::endl;

    TrkrHitSetContainer *hitsetcontainer = findNode::getClass<TrkrHitSetContainer>(topNode, "TRKR_HITSET");

    // std::cout << __LINE__ << std::endl;
    TrkrHitSetContainer::ConstRange hitset_range = hitsetcontainer->getHitSets(TrkrDefs::TrkrId::mvtxId);
    for (TrkrHitSetContainer::ConstIterator hitset_iter = hitset_range.first; hitset_iter != hitset_range.second; ++hitset_iter)
    {
        TrkrHitSet::ConstRange hit_range = hitset_iter->second->getHits();
        for (TrkrHitSet::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; ++hit_iter)
        {
            hitKey = hit_iter->first;
            PHG4Hit *hit = hiteval->max_truth_hit_by_energy(hitKey); // PHG4Hit

            if (hit == nullptr)
                continue;

            HitLayerID_.push_back(TrkrDefs::getLayer(hitKey));
            HitStaveID_.push_back(MvtxDefs::getStaveId(hitKey));
            HitChipID_.push_back(MvtxDefs::getChipId(hitKey));
            HitRow_.push_back(MvtxDefs::getRow(hitKey));
            HitColumn_.push_back(MvtxDefs::getCol(hitKey));
            HitX_.push_back(hit->get_x(0));
            HitY_.push_back(hit->get_y(0));
            HitZ_.push_back(hit->get_z(0));
        }
    }
}

//____________________________________________________________________________..
void dNdEtaAnalyzer::GetRecoClusterInfo(PHCompositeNode *topNode)
{
    std::cout << "Get reconstructed cluster info." << std::endl;

    dst_clustermap = findNode::getClass<TrkrClusterContainer>(topNode, "TRKR_CLUSTER");
    if (!dst_clustermap)
    {
        std::cout << PHWHERE << "Error, can't find trkr clusters" << std::endl;
        // return Fun4AllReturnCodes::ABORTEVENT;
        return;
    }

    clusterhitmap = findNode::getClass<TrkrClusterHitAssoc>(topNode, "TRKR_CLUSTERHITASSOC");
    if (!clusterhitmap)
    {
        std::cout << PHWHERE << "Error, can't find cluster-hit map" << std::endl;
        return;
    }

    hitsets = findNode::getClass<TrkrHitSetContainer>(topNode, "TRKR_HITSET");
    if (!hitsets)
    {
        std::cout << PHWHERE << "Error, can't find cluster-hit map" << std::endl;
        return;
    }

    _tgeometry = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");
    if (!_tgeometry)
    {
        std::cout << PHWHERE << "Error, can't find Acts geometry" << std::endl;
        return;
        // return Fun4AllReturnCodes::ABORTEVENT;
    }

    // Reference: https://github.com/sPHENIX-Collaboration/coresoftware/blob/master/simulation/g4simulation/g4eval/SvtxEvaluator.cc#L1731-L1984
    for (const auto &hitsetkey : dst_clustermap->getHitSetKeys())
    {
        auto range = dst_clustermap->getClusters(hitsetkey);
        for (auto iter = range.first; iter != range.second; ++iter)
        {
            TrkrDefs::cluskey cluster_key = iter->first;
            TrkrCluster *cluster = dst_clustermap->findCluster(cluster_key);

            unsigned int trkrId = TrkrDefs::getTrkrId(cluster_key);
            if (trkrId != TrkrDefs::mvtxId)
                continue; // we want only MVTX clusters

            auto globalpos = _tgeometry->getGlobalPosition(cluster_key, cluster);
            ClusLayer_.push_back(TrkrDefs::getLayer(cluster_key));
            ClusX_.push_back(globalpos(0));
            ClusY_.push_back(globalpos(1));
            ClusZ_.push_back(globalpos(2));
            ClusAdc_.push_back(cluster->getAdc());
            TVector3 pos(globalpos(0), globalpos(1), globalpos(2));
            ClusR_.push_back(pos.Perp());
            ClusPhi_.push_back(pos.Phi());
            ClusEta_.push_back(pos.Eta());

            std::set<int> phibins;
            std::set<int> zbins;
            // TrkrDefs::hitsetkey hitsetkey =  TrkrDefs::getHitSetKeyFromClusKey(cluster_key);
            // TrkrHitSetContainer::Iterator hitset = hitsets->findOrAddHitSet(hitsetkey);
            std::pair<std::multimap<TrkrDefs::cluskey, TrkrDefs::hitkey>::const_iterator, std::multimap<TrkrDefs::cluskey, TrkrDefs::hitkey>::const_iterator> hitrange = clusterhitmap->getHits(cluster_key);
            for (std::multimap<TrkrDefs::cluskey, TrkrDefs::hitkey>::const_iterator clushititer = hitrange.first; clushititer != hitrange.second; ++clushititer)
            {
                int col = MvtxDefs::getCol(clushititer->second);
                int row = MvtxDefs::getRow(clushititer->second);
                zbins.insert(col);
                phibins.insert(row);
            }
            // ClusPhiSize_.push_back(phibins.size());
            // ClusZSize_.push_back(zbins.size());
            ClusPhiSize_.push_back(cluster->getPhiSize());
            ClusZSize_.push_back(cluster->getZSize());
            phibins.clear();
            zbins.clear();
        }
    }
}

//____________________________________________________________________________..
void dNdEtaAnalyzer::GetTruthPVInfo(PHCompositeNode *topNode)
{
    std::cout << "Get truth primary vertex info." << std::endl;

    // Truth information for truth vertex
    m_truth_info = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
    if (!m_truth_info)
    {
        std::cout << PHWHERE << "Error, can't find G4TruthInfo" << std::endl;
        // return Fun4AllReturnCodes::ABORTEVENT;
        return;
    }

    dst_clustermap = findNode::getClass<TrkrClusterContainer>(topNode, "TRKR_CLUSTER");
    if (!dst_clustermap)
    {
        std::cout << PHWHERE << "Error, can't find trkr clusters" << std::endl;
        // return Fun4AllReturnCodes::ABORTEVENT;
        return;
    }

    auto vrange = m_truth_info->GetPrimaryVtxRange();
    const auto prange = m_truth_info->GetPrimaryParticleRange();

    std::map<int, unsigned int> vertex_particle_count;
    std::map<int, unsigned int> vertex_hit_count;
    std::vector<int> layerID_mvtx{0, 1, 2};
    for (auto iter = prange.first; iter != prange.second; ++iter) // process all primary paricle
    {
        ++vertex_particle_count[iter->second->get_vtx_id()];

        // PHG4Particle *particle = iter->second;
        std::set<PHG4Hit *> g4hits = truth_eval->all_truth_hits(iter->second);
        for (auto hit : g4hits)
        {
            std::vector<int>::iterator it = std::find(layerID_mvtx.begin(), layerID_mvtx.end(), hit->get_layer());
            if (it != layerID_mvtx.end())
            {
                ++vertex_hit_count[iter->second->get_vtx_id()];
                HitfromG4P_X_.push_back(hit->get_x(0));
                HitfromG4P_Y_.push_back(hit->get_y(0));
                HitfromG4P_Z_.push_back(hit->get_z(0));
            }
        }
    }
    int total = 0, falsecount = 0, truecount = 0;
    std::map<int, unsigned int> vertex_cluster_count;
    for (const auto &hitsetkey : dst_clustermap->getHitSetKeys())
    {
        auto range = dst_clustermap->getClusters(hitsetkey);
        for (auto iter = range.first; iter != range.second; ++iter)
        {
            total++;
            TrkrDefs::cluskey cluster_key = iter->first;
            PHG4Particle *g4particle = clustereval->max_truth_particle_by_cluster_energy(cluster_key);

            unsigned int trkrId = TrkrDefs::getTrkrId(cluster_key);
            if (trkrId != TrkrDefs::mvtxId)
                continue; // we want only MVTX clusters
            else
            {
                if (g4particle)
                {
                    truecount++;
                    ++vertex_cluster_count[g4particle->get_vtx_id()];
                }
                else
                {
                    falsecount++;
                }
            }
        }
    }

    // std::cout << total << " " << falsecount << " " << truecount << std::endl;

    int TruthPV_idx = 0, tmp_Npart = -999;
    for (auto iter = vrange.first; iter != vrange.second; ++iter) // process all primary vertices
    {
        const int point_id = iter->first;
        PHG4VtxPoint *point = iter->second;

        if (point)
        {
            TruthPV_x_.push_back(point->get_x());
            TruthPV_y_.push_back(point->get_y());
            TruthPV_z_.push_back(point->get_z());
            TruthPV_Npart_.push_back(vertex_particle_count[point_id]);
            TruthPV_Nhits_.push_back(vertex_hit_count[point_id]);
            TruthPV_NClus_.push_back(vertex_cluster_count[point_id]);
            TruthPV_t_.push_back(point->get_t());
            TruthPV_embed_.push_back(m_truth_info->isEmbededVtx(point_id));

            if (m_truth_info->isEmbededVtx(point_id) == 0)
            {
                TruthPV_trig_x_ = point->get_x();
                TruthPV_trig_y_ = point->get_y();
                TruthPV_trig_z_ = point->get_z();
                TruthPV_trig_Npart_ = vertex_particle_count[point_id];
            }

            if ((int)vertex_particle_count[point_id] > tmp_Npart)
            {
                tmp_Npart = vertex_particle_count[point_id];
                TruthPV_mostNpart_x_ = point->get_x();
                TruthPV_mostNpart_y_ = point->get_y();
                TruthPV_mostNpart_z_ = point->get_z();
                TruthPV_mostNpart_Npart_ = vertex_particle_count[point_id];
            }

            std::cout << "TruthPV_idx = " << TruthPV_idx << "(x,y,z,t,Npart,NMvtxhits,NMvtxClus,embed) = (" << point->get_x() << "," << point->get_y() << "," << point->get_z() << "," << point->get_t() << "," << vertex_particle_count[point_id] << "," << vertex_hit_count[point_id] << "," << vertex_cluster_count[point_id] << "," << m_truth_info->isEmbededVtx(point_id) << ")" << std::endl;
            TruthPV_idx++;
        }
    }

    NTruthVtx_ = (int)TruthPV_x_.size();
}

//____________________________________________________________________________..
void dNdEtaAnalyzer::GetG4ParticleInfo(PHCompositeNode *topNode)
{
    std::cout << "Get truth G4Particle info." << std::endl;

    m_truth_info = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
    if (!m_truth_info)
    {
        std::cout << PHWHERE << "Error, can't find G4TruthInfo" << std::endl;
        // return Fun4AllReturnCodes::ABORTEVENT;
        return;
    }

    const auto prange = m_truth_info->GetPrimaryParticleRange();
    for (auto iter = prange.first; iter != prange.second; ++iter) // process all primary paricles
    {
        PHG4Particle *particle = iter->second;

        G4PartPID_.push_back(particle->get_pid());
        G4PartTrackID_.push_back(particle->get_track_id());
        G4PartVtxID_.push_back(particle->get_vtx_id());
        G4PartParentID_.push_back(particle->get_parent_id());
        G4PartPrimaryID_.push_back(particle->get_primary_id());
        TLorentzVector g4part;
        g4part.SetPxPyPzE(particle->get_px(), particle->get_py(), particle->get_pz(), particle->get_e());
        G4PartE_.push_back(particle->get_e());
        G4PartPt_.push_back(g4part.Pt());
        G4PartEta_.push_back(g4part.Eta());
        G4PartPhi_.push_back(g4part.Phi());

        // printf("(PID,TrackID,ParentID,PrimaryID)=(%d,%d,%d,%d), (pt,eta,phi,E)=(%f,%f,%f,%f)\n", particle->get_pid(), particle->get_track_id(), particle->get_parent_id(), particle->get_primary_id(), g4part.Pt(), g4part.Eta(), g4part.Phi(), particle->get_e());
    }

    // To-Do: add decayed particle (simulated particles)
}

//____________________________________________________________________________..
void dNdEtaAnalyzer::ResetVectors()
{
    CleanVec(ClusLayer_);
    CleanVec(ClusX_);
    CleanVec(ClusY_);
    CleanVec(ClusZ_);
    CleanVec(ClusR_);
    CleanVec(ClusPhi_);
    CleanVec(ClusEta_);
    CleanVec(ClusAdc_);
    CleanVec(ClusPhiSize_);
    CleanVec(ClusZSize_);
    CleanVec(TruthPV_x_);
    CleanVec(TruthPV_y_);
    CleanVec(TruthPV_z_);
    CleanVec(TruthPV_Npart_);
    CleanVec(TruthPV_Nhits_);
    CleanVec(TruthPV_NClus_);
    CleanVec(TruthPV_t_);
    CleanVec(TruthPV_embed_);
    CleanVec(G4PartPID_);
    CleanVec(G4PartTrackID_);
    CleanVec(G4PartVtxID_);
    CleanVec(G4PartParentID_);
    CleanVec(G4PartPrimaryID_);
    CleanVec(G4PartPt_);
    CleanVec(G4PartEta_);
    CleanVec(G4PartPhi_);
    CleanVec(G4PartE_);
    CleanVec(HitLayerID_);
    CleanVec(HitStaveID_);
    CleanVec(HitChipID_);
    CleanVec(HitRow_);
    CleanVec(HitColumn_);
    CleanVec(HitX_);
    CleanVec(HitY_);
    CleanVec(HitZ_);
    CleanVec(HitfromG4P_X_);
    CleanVec(HitfromG4P_Y_);
    CleanVec(HitfromG4P_Z_);
}
