import time
import os
import sys
import re
from argparse import ArgumentParser
from datetime import datetime

# Ref: https://www.geeksforgeeks.org/how-to-search-and-replace-text-in-a-file-in-python/
def replacetext(filename, search_text, replace_text):
    with open(filename, 'r+') as f:
        file = f.read()
        file = re.sub(search_text, replace_text, file)
        f.seek(0)
        f.write(file)
        f.truncate()
    # return 'Text {} replaced by {}'.format(search_text, replace_text)


os.makedirs('./condor_out/', exist_ok=True)
os.makedirs('./condor_out/out/', exist_ok=True)
os.makedirs('./condor_out/err/', exist_ok=True)
os.makedirs('./condor_out/log/', exist_ok=True)
os.system('rm condor_SimplePions_vtxZmean*_vtxZwidth*.job')
os.system('rm ./condor_out/out/*')
os.system('rm ./condor_out/err/*')
os.system('rm ./condor_out/log/*')

# vtx_meanwidth = [[0, 0], [0, 3], [0, 5], [0, 8], [1, 0], [1, 3], [1, 5], [1, 8], [3, 0], [3, 3], [3, 5], [3, 8]]
vtx_meanwidth = [[-1, 0], [-1, 3], [-1, 5], [-1, 8], [-3, 0], [-3, 3], [-3, 5], [-3, 8]]
nEvents = 500
nParticles = 1000
for v in vtx_meanwidth:
    print('Run Fun4All simple (pion) mode, nEvents = {}, nParticles(pions) = {}, vtxZ (mean, width) = ({}, {}) cm'.format(nEvents, nParticles, v[0], v[1]))
    newfile = 'condor_SimplePions_vtxZmean{:d}_vtxZwidth{:d}.job'.format(v[0], v[1])
    os.system('cp condor_SimplePions.job {}'.format(newfile))
    replacetext(newfile, 'NEVENTS', '{}'.format(nEvents))
    replacetext(newfile, 'NPARTICLES', '{}'.format(nParticles))
    replacetext(newfile, 'VTXZMEAN', '{}'.format(v[0]))
    replacetext(newfile, 'VTXZWIDTH', '{}'.format(v[1]))
    cmd = 'condor_submit ' + newfile
    os.system(cmd)