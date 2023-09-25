#! /usr/bin/env python
from optparse import OptionParser
import sys
import os
import datetime
from array import *
from ROOT import *
import numpy
import math
import glob

if __name__ == '__main__':
    # f = TFile('/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/MVTXRecoClusters_NoPileup_Nevt2000_v3.root', 'r')
    f = TFile('/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/HijingMBwoPU_private/MVTXRecoClusters_HijingMBwoPU_private.root','r')
    tree = f.Get("EventTree")

    for idx in range(tree.GetEntries()):
    # for idx in range(20):
        tree.GetEntry(idx)

        Nhits_MVTX = 0
        weirdevt = False
        for i in range(len(tree.HitX)):
            rho = sqrt(tree.HitX[i]*tree.HitX[i] + tree.HitY[i]*tree.HitY[i])
            if rho < 5.:
                Nhits_MVTX += 1

        for i in range(len(tree.TruthPV_t)):
            # condition_to_print = tree.TruthPV_Nhits[i] == 0 or (tree.TruthPV_Nhits[i] > 0 and tree.TruthPV_NClus[i]/tree.TruthPV_Nhits[i] < 0.9)
            condition_to_print = True
            if condition_to_print:
                weirdevt = True
                print('event={}, TruthPV_Npart={}, TruthPV_NClus={}, TruthPV_Nhits={}, TruthPV_z={}'.format(tree.event, tree.TruthPV_Npart[i], tree.TruthPV_NClus[i], tree.TruthPV_Nhits[i], tree.TruthPV_z[i]))

        if weirdevt:
            print ('event={}, NG4Particle={}, NCLuster_MVTX={}, NhitsFromG4P={}, Nhits_MVTX={}\n--------------'.format(tree.event, len(tree.G4PartPID), len(tree.ClusLayer), len(tree.HitfromG4P_X), Nhits_MVTX))
