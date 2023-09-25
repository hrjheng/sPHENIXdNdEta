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
os.system('rm condor_singleEvt_set*.job')
os.system('rm ./condor_out/out/condor_Nevt*.out')
os.system('rm ./condor_out/err/condor_Nevt*.err')
os.system('rm ./condor_out/log/condor_Nevt*.log')

EventsPerSet = 1
Sets = 2000
# TotalEvt = EventsPerSet * Sets
for i in range(Sets):
    newfile = 'condor_singleEvt_set{}.job'.format(i)
    os.system('cp condor.job {}'.format(newfile))
    replacetext(newfile, '48GB', '10GB')
    replacetext(newfile, 'NEVENTS', '{}'.format(EventsPerSet))
    replacetext(newfile, 'INPUTFILELISTIDX', '{}'.format(i))
    replacetext(newfile, 'FILENAME', 'NoPileup_Nevt500_ana325private_singleEvtDst/MVTXRecoClusters_evt{}'.format(i))
    cmd = 'condor_submit ' + newfile
    os.system(cmd)