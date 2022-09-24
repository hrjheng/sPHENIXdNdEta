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
os.system('rm condor_set*.job')
os.system('rm ./condor_out/out/*')
os.system('rm ./condor_out/err/*')
os.system('rm ./condor_out/log/*')

EventsPerSet = 1000
Sets = 1
TotalEvt = EventsPerSet * Sets
for i in range(Sets):
    print('Run Fun4All (Fun4All_G4_sPHENIX_v2.C with [test - Reco clusters] module), file list set {}'.format(i))
    newfile = 'condor_set{}.job'.format(i)
    os.system('cp condor.job {}'.format(newfile))
    replacetext(newfile, 'NEVENTS', '{}'.format(EventsPerSet))
    replacetext(newfile, 'INPUTFILELISTIDX', '{}'.format(i))
    replacetext(newfile, 'FILENAME', 'MVTXRecoClusters_set{}'.format(i))
    # cmd = 'condor_submit ' + newfile
    # os.system(cmd)