#! /usr/bin/env python
from optparse import OptionParser
import sys
import os
import datetime
from array import *
from ROOT import *
import numpy as np

gROOT.LoadMacro('./sPHENIXStyle/sPhenixStyle.C')
gROOT.ProcessLine('SetsPhenixStyle()')
gROOT.SetBatch(True)
gStyle.SetPalette(kLightTemperature)
# TColor.InvertPalette()

def SetPalette():
    TColor.CreateGradientColorTable(3,array ("d", [0.00, 0.50, 1.00]),
                                      array ("d", [1.00, 1.00, 0.00]),
                                      array ("d", [0.70, 1.00, 0.34]),
                                      array ("d", [0.00, 1.00, 0.82]),
                                      255,  1.0)

def pypowarray(numpyvec, pow):
    return numpyvec**pow

if __name__ == '__main__':

    # plotpath = './plot_MVTXhits/'
    # plotpath = './plot_CheckClusSimplePion/'
    # plotpath = './plot_CheckClus_listtype3/'
    plotpath = './plot_MVTXRecoClusters_v2/'
    # plotpath = './plot_MVTXRecoClusters_SimplePions/'
    os.makedirs(plotpath, exist_ok=True)

    # SetPalette()

    # df = ROOT.RDataFrame('EventTree', '/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/MVTXRecoClusters_SIMPLE5000Pions_VtxZmean0cm_VtxZwidth0cm.root')
    df = ROOT.RDataFrame('EventTree', '/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/MVTXRecoClusters_NoPileup_Nevt2000_v2.root')
    # df = df.Filter("TruthPV_trig_z < 2.5 && TruthPV_trig_z > -2.5")
    hM_hitx = df.Histo1D(("hM_hitx", " ", 40, -10, 10), "ClusX")
    # hM_hitz = df.Define("Layer1ClusZ", "ClusZ[ClusLayer==0]").Histo1D(("hM_hitz", " ", 60, -15, 15), "Layer1ClusZ")
    hM_hitz = df.Histo1D(("hM_hitz", " ", 60, -15, 15), "ClusZ")
    hM_G4Hitpos = df.Histo2D(("hM_G4Hitpos", " ", 10000, -100, 100, 10000, -100, 100), "HitX", "HitY")
    hM_G4HitR_G4HitZ = df.Define("G4HitR", "sqrt(HitX*HitX+HitY*HitY)").Define("MVTX_G4HitR","G4HitR[G4HitR<5]").Define("MVTX_G4HitZ","HitZ[G4HitR<5]").Histo2D(("hM_G4HitR_G4HitZ", " ", 120, 2, 5, 600, -15, 15), "MVTX_G4HitR", "MVTX_G4HitZ")
    hM_TruthPV_trig_z = df.Histo1D(("hM_TruthPV_trig_z", " ", 50, -25, 25), "TruthPV_trig_z")
    hM_TruthPV_trig_XvsY = df.Histo2D(("hM_TruthPV_trig_XvsY", " ", 100, -0.1, 0.1, 100, -0.1, 0.1), "TruthPV_trig_x", "TruthPV_trig_y")
    hM_TruthPV_trig_XYZpos = df.Histo3D(("hM_TruthPV_trig_XYZpos", " ", 100, -1, 1, 100, -1, 1, 1000, -25, 25), "TruthPV_trig_x", "TruthPV_trig_y", "TruthPV_trig_z")
    hM_hitpos3d = df.Histo3D(("hM_hitpos3d", " ", 240, -6, 6, 240, -6, 6, 40, -10, 10), "ClusX", "ClusY", "ClusZ")
    hM_hitpos = df.Define("Layer1ClusX", "ClusX[ClusLayer==0]").Define("Layer1ClusY", "ClusY[ClusLayer==0]").Histo2D(("hM_hitpos", " ", 1200, -6, 6, 1200, -6, 6), "Layer1ClusX", "Layer1ClusY")
    hM_ClusEta_ClusZSize = df.Define("Layer1ClusEta", "ClusEta[ClusLayer==0]").Define("Layer1ClusZSize", "ClusZSize[ClusLayer==0]").Histo2D(("hM_ClusEta_ClusZSize", " ", 120, -3, 3, 20, 0, 20), "Layer1ClusEta", "Layer1ClusZSize")
    hM_ClusEta_ClusAdc = df.Define("Layer1ClusEta", "ClusEta[ClusLayer==0]").Define("Layer1ClusAdc", "ClusAdc[ClusLayer==0]").Histo2D(("hM_ClusEta_ClusAdc", " ", 120, -3, 3, 20, 0, 20), "Layer1ClusEta", "Layer1ClusAdc")
    hM_ClusPhi_ClusEta = df.Define("Layer1ClusPhi", "ClusPhi[ClusLayer==0]").Define("Layer1ClusEta", "ClusEta[ClusLayer==0]").Histo2D(("hM_ClusPhi_ClusEta", " ", 140, -3.5, 3.5, 120, -3, 3), "Layer1ClusPhi", "Layer1ClusEta")
    hM_ClusPhi_ClusEta_largebin = df.Define("Layer1ClusPhi", "ClusPhi[ClusLayer==0]").Define("Layer1ClusEta", "ClusEta[ClusLayer==0]").Histo2D(("hM_ClusPhi_ClusEta_largebin", " ", 14, -3.5, 3.5, 12, -3, 3), "Layer1ClusPhi", "Layer1ClusEta")
    hM_GenParPhi_GenParEta = df.Histo2D(("hM_GenParPhi_GenParEta", " ", 140, -3.5, 3.5, 120, -3, 3), "G4PartPhi", "G4PartEta")
    hM_GenParPhi_GenParEta_wide = df.Histo2D(("hM_GenParPhi_GenParEta_wide", " ", 140, -3.5, 3.5, 500, -10, 10), "G4PartPhi", "G4PartEta")
    hM_GenParPhi_GenParEta_largebin = df.Histo2D(("hM_GenParPhi_GenParEta_largebin", " ", 14, -3.5, 3.5, 12, -3, 3), "G4PartPhi", "G4PartEta")
    hM_ClusEff_Phi_Eta = TH2F('hM_ClusEff_Phi_Eta', 'hM_ClusEff_Phi_Eta', 140, -3.5, 3.5, 120, -3, 3)
    hM_ClusEff_Phi_Eta.Divide(hM_ClusPhi_ClusEta.GetPtr(), hM_GenParPhi_GenParEta.GetPtr())
    hM_ClusEff_Phi_Eta_largebin = TH2F('hM_ClusEff_Phi_Eta_largebin', 'hM_ClusEff_Phi_Eta_largebin', 14, -3.5, 3.5, 12, -3, 3)
    hM_ClusEff_Phi_Eta_largebin.Divide(hM_ClusPhi_ClusEta_largebin.GetPtr(), hM_GenParPhi_GenParEta_largebin.GetPtr())

    c = TCanvas('c', '', 1500, 1300)
    c.cd()
    gPad.SetRightMargin(0.05)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.18)
    gPad.SetBottomMargin(0.15)
    hM_hitz.GetXaxis().SetTitle('Cluster Z position (cm)')
    hM_hitz.GetYaxis().SetTitle('Entries')
    hM_hitz.GetYaxis().SetTitleOffset(1.6)
    hM_hitz.Draw('hist')
    c.RedrawAxis()
    c.SaveAs(plotpath+"ClusterPosZ.pdf")
    c.SaveAs(plotpath+"ClusterPosZ.png")

    c.cd()
    gPad.SetRightMargin(0.05)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.18)
    gPad.SetBottomMargin(0.15)
    hM_TruthPV_trig_z.GetXaxis().SetTitle('Truth PV (trig) Z position (cm)')
    hM_TruthPV_trig_z.GetYaxis().SetTitle('Entries')
    hM_TruthPV_trig_z.GetYaxis().SetTitleOffset(1.6)
    hM_TruthPV_trig_z.Draw('hist')
    c.RedrawAxis()
    c.SaveAs(plotpath+"TruthPV_trig_z.pdf")
    c.SaveAs(plotpath+"TruthPV_trig_z.png")

    c.cd()
    gPad.SetRightMargin(0.17)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_TruthPV_trig_XvsY.GetXaxis().SetTitle('Truth PV (trig) X position (cm)')
    hM_TruthPV_trig_XvsY.GetYaxis().SetTitle('Truth PV (trig) Y position (cm)')
    hM_TruthPV_trig_XvsY.GetXaxis().SetLabelSize(0.035)
    hM_TruthPV_trig_XvsY.GetYaxis().SetLabelSize(0.035)
    hM_TruthPV_trig_XvsY.SetContour(10000)
    hM_TruthPV_trig_XvsY.Draw('colz')
    c.SaveAs(plotpath+"TruthPV_trig_XvsY.pdf")
    c.SaveAs(plotpath+"TruthPV_trig_XvsY.png")

    c.cd()
    # gPad.SetTheta(90)
    # gPad.SetPhi(0)
    gPad.SetRightMargin(0.15)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.12)
    gPad.SetBottomMargin(0.15)
    hM_hitpos3d.GetXaxis().SetTitle('Cluster x position (cm)')
    hM_hitpos3d.GetYaxis().SetTitle('Cluster y position (cm)')
    hM_hitpos3d.GetZaxis().SetTitle('Cluster z position (cm)')
    hM_hitpos3d.SetMarkerSize(0.1)
    hM_hitpos3d.SetMarkerColorAlpha(1, 0.5)
    hM_hitpos3d.Draw()
    c.RedrawAxis()
    c.SaveAs(plotpath+"hitpos.pdf")
    c.SaveAs(plotpath+"hitpos.png")

    
    c.cd()
    # gPad.SetTheta(90)
    # gPad.SetPhi(0)
    gPad.SetRightMargin(0.15)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.12)
    gPad.SetBottomMargin(0.15)
    hM_TruthPV_trig_XYZpos.GetXaxis().SetTitle('Truth PV (trig) X position (cm)')
    hM_TruthPV_trig_XYZpos.GetYaxis().SetTitle('Truth PV (trig) Y position (cm)')
    hM_TruthPV_trig_XYZpos.GetZaxis().SetTitle('Truth PV (trig) Z position (cm)')
    hM_TruthPV_trig_XYZpos.SetMarkerSize(0.2)
    hM_TruthPV_trig_XYZpos.SetMarkerColorAlpha(1, 0.5)
    hM_TruthPV_trig_XYZpos.Draw()
    c.RedrawAxis()
    c.SaveAs(plotpath+"TruthPV_trig_XYZpos.pdf")
    c.SaveAs(plotpath+"TruthPV_trig_XYZpos.png")

    c.cd()
    gPad.SetRightMargin(0.17)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_hitpos.GetXaxis().SetTitle('Cluster x pos (cm)')
    hM_hitpos.GetYaxis().SetTitle('Cluster y pos (cm)')
    hM_hitpos.SetContour(10000)
    hM_hitpos.Draw('colz')
    c.SaveAs(plotpath+"clusterpos.pdf")
    c.SaveAs(plotpath+"clusterpos.png")

    c.cd()
    gPad.SetRightMargin(0.17)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_G4Hitpos.GetXaxis().SetTitle('G4Hit x pos (cm)')
    hM_G4Hitpos.GetYaxis().SetTitle('G4Hit y pos (cm)')
    # hM_G4Hitpos.SetMarkerSize(0.2)
    hM_G4Hitpos.SetContour(10000)
    hM_G4Hitpos.Draw('colz')
    c.SaveAs(plotpath+"g4Hitpos.pdf")
    c.SaveAs(plotpath+"g4Hitpos.png")

    c.cd()
    gPad.SetRightMargin(0.17)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_G4HitR_G4HitZ.GetXaxis().SetTitle('G4Hit Radius (cm)')
    hM_G4HitR_G4HitZ.GetYaxis().SetTitle('G4Hit z pos (cm)')
    # hM_G4HitR_G4HitZ.SetMarkerSize(0.2)
    hM_G4HitR_G4HitZ.SetContour(10000)
    hM_G4HitR_G4HitZ.Draw('colz')
    c.SaveAs(plotpath+"g4HitR_g4HitZ.pdf")
    c.SaveAs(plotpath+"g4HitR_g4HitZ.png")

    # c.cd()
    # gPad.SetRightMargin(0.1)
    # gPad.SetTopMargin(0.08)
    # gPad.SetLeftMargin(0.15)
    # gPad.SetBottomMargin(0.15)
    # hM_hitpos.GetXaxis().SetTitle('Cluster x pos (cm)')
    # hM_hitpos.GetYaxis().SetTitle('Cluster y pos (cm)')
    # hM_hitpos.SetMarkerSize(0.2)
    # hM_hitpos.Draw()
    # c.SaveAs(plotpath+"clusterpos_scat.pdf")
    # c.SaveAs(plotpath+"clusterpos_scat.png")

    c.cd()
    gPad.SetRightMargin(0.14)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_ClusEta_ClusZSize.GetXaxis().SetTitle('Cluster #eta')
    hM_ClusEta_ClusZSize.GetYaxis().SetTitle('Pixel cluster length along z')
    hM_ClusEta_ClusZSize.Draw('box')
    c.SaveAs(plotpath+"ClusEta_ClusZSize.pdf")
    c.SaveAs(plotpath+"ClusEta_ClusZSize.png")

    c.cd()
    gPad.SetRightMargin(0.14)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_ClusEta_ClusAdc.GetXaxis().SetTitle('Cluster #eta')
    hM_ClusEta_ClusAdc.GetYaxis().SetTitle('Pixel cluster Adc')
    hM_ClusEta_ClusAdc.Draw('box')
    c.SaveAs(plotpath+"ClusEta_ClusAdc.pdf")
    c.SaveAs(plotpath+"ClusEta_ClusAdc.png")

    c.cd()
    gPad.SetRightMargin(0.15)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_ClusPhi_ClusEta.GetXaxis().SetTitle('Cluster #phi')
    hM_ClusPhi_ClusEta.GetYaxis().SetTitle('Cluster #eta')
    hM_ClusPhi_ClusEta.Draw('colz')
    c.SaveAs(plotpath+"ClusPhi_ClusEta.pdf")
    c.SaveAs(plotpath+"ClusPhi_ClusEta.png")

    c.cd()
    gPad.SetRightMargin(0.13)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_GenParPhi_GenParEta.GetXaxis().SetTitle('Cluster #phi')
    hM_GenParPhi_GenParEta.GetYaxis().SetTitle('Cluster #eta')
    hM_GenParPhi_GenParEta.Draw('colz')
    c.SaveAs(plotpath+"GenParPhi_GenParEta.pdf")
    c.SaveAs(plotpath+"GenParPhi_GenParEta.png")

    c.cd()
    gPad.SetRightMargin(0.15)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_GenParPhi_GenParEta_wide.GetXaxis().SetTitle('Gen-particle #phi')
    hM_GenParPhi_GenParEta_wide.GetYaxis().SetTitle('Gen-particle #eta')
    hM_GenParPhi_GenParEta_wide.Draw('colz')
    c.SaveAs(plotpath+"GenParPhi_GenParEta_wide.pdf")
    c.SaveAs(plotpath+"GenParPhi_GenParEta_wide.png")

    c.cd()
    gPad.SetRightMargin(0.13)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_ClusEff_Phi_Eta.GetXaxis().SetTitle('#phi')
    hM_ClusEff_Phi_Eta.GetYaxis().SetTitle('#eta')
    hM_ClusEff_Phi_Eta.SetContour(10000)
    hM_ClusEff_Phi_Eta.Draw('colz')
    c.SaveAs(plotpath+"ClusEff_Phi_Eta.pdf")
    c.SaveAs(plotpath+"ClusEff_Phi_Eta.png")

    c.cd()
    gStyle.SetPaintTextFormat('1.2f')
    gPad.SetRightMargin(0.13)
    gPad.SetTopMargin(0.08)
    gPad.SetLeftMargin(0.15)
    gPad.SetBottomMargin(0.15)
    hM_ClusEff_Phi_Eta_largebin.GetXaxis().SetTitle('#phi')
    hM_ClusEff_Phi_Eta_largebin.GetYaxis().SetTitle('#eta')
    hM_ClusEff_Phi_Eta_largebin.SetContour(10000)
    hM_ClusEff_Phi_Eta_largebin.Draw('colztext')
    c.SaveAs(plotpath+"ClusEff_Phi_Eta_largebin.pdf")
    c.SaveAs(plotpath+"ClusEff_Phi_Eta_largebin.png")

    fout = TFile.Open(plotpath+'hists.root',"RECREATE")
    fout.cd()
    hM_hitpos3d.Write()
    fout.Close()