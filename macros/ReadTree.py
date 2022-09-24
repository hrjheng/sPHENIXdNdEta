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
    f = TFile('/sphenix/user/hjheng/TrackletAna/data/MVTXRecoClusters/MVTXRecoClusters_NoPileup_Nevt2000_v2.root', 'r')
    tree = f.Get("EventTree")
    for idx in range(tree.GetEntries()):
        tree.GetEntry(idx)

        Nhits_MVTX = 0
        for i in range(len(tree.HitX)):
            rho = sqrt(tree.HitX[i]*tree.HitX[i] + tree.HitY[i]*tree.HitY[i])
            if rho < 5.:
                Nhits_MVTX += 1
        
        print ('event={}, Nhits_MVTX={}, NCLuster_MVTX={}, NG4Particle={}'.format(tree.event, Nhits_MVTX, len(tree.ClusLayer),len(tree.G4PartPID)))
