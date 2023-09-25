#!/bin/sh
# CreateFileList.pl -n 4000 -run 40 -type 4 DST_TRKR_CLUSTER DST_TRACKS DST_TRKR_HIT DST_TRKR_G4HIT DST_TRUTH_G4HIT DST_TRUTH DST_VERTEX

# No-pileup samples
CreateFileList.pl -type 4 -nopileup -n 500000 DST_CALO_CLUSTER DST_TRKR_HIT DST_TRUTH G4Hits

# Private samples from Cameron
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/Disable_FERMIMOTION_FLOW/pass1/*.root | awk '{print $9}' > ./test/g4hits_sHijing_0_20fm.list
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/Disable_FERMIMOTION_FLOW/pass2_nopileup/*.root | awk '{print $9}' > ./test/dst_calo_cluster_sHijing_0_20fm.list
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/Disable_FERMIMOTION_FLOW/pass3_trkr/DST_TRKR_HIT*.root | awk '{print $9}' > ./test/dst_trkr_hit_sHijing_0_20fm.list
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/Disable_FERMIMOTION_FLOW/pass3_trkr/DST_TRUTH*.root | awk '{print $9}' > ./test/dst_truth_sHijing_0_20fm.list

# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass1/*.root | awk '{print $9}' > ./test/g4hits_sHijing_0_20fm.list
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass2_nopileup/*.root | awk '{print $9}' > ./test/dst_calo_cluster_sHijing_0_20fm.list
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass3_trkr/DST_TRKR_HIT*.root | awk '{print $9}' > ./test/dst_trkr_hit_sHijing_0_20fm.list
# ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass3_trkr/DST_TRUTH*.root | awk '{print $9}' > ./test/dst_truth_sHijing_0_20fm.list

# for idx in {15000..16999}
# do
#     num=$(($idx-15000))
#     # echo $num
#     ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass1/G4Hits_sHijing_0_20fm-0000000050-$idx.root | awk '{print $9}' > ./test/g4hits_sHijing_0_20fm_$num.list
#     ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass2_nopileup/DST_CALO_CLUSTER_sHijing_0_20fm-0000000050-$idx.root | awk '{print $9}' > ./test/dst_calo_cluster_sHijing_0_20fm_$num.list
#     ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass3_trkr/DST_TRKR_HIT_sHijing_0_20fm-0000000050-$idx.root | awk '{print $9}' > ./test/dst_trkr_hit_sHijing_0_20fm_$num.list
#     ls -l /sphenix/user/cdean/DSTs/MDC2_Run50_Hijing_noPileup/singleEvent/pass3_trkr/DST_TRUTH_sHijing_0_20fm-0000000050-$idx.root | awk '{print $9}' > ./test/dst_truth_sHijing_0_20fm_$num.list
# done
