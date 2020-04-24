#!/bin/sh

FSETTESTDIR=${FSETTESTDIR:-/afs/tr/usr/rajesh/epi/test/recovery/fset}

# Load base routines and variables
. ${FSETTESTDIR}/fsettestbasic

NUMITER=${NUMITER:-10}
FSETID_1_1=`calcFsetID $BASE_FSETID_AGGR1 1`
FSETNAME_1_1=`detFsetName $BASE_FSETNAME_AGGR1 1`
MNTDIR_FSET_1_1="${MNTDIR}/${FSETNAME_1_1}"

# 
# Main program
#

logstarttest

if [ ! -d "$MNTDIR" ] 
then
	run "mkdir $MNTDIR" 0  "mkdir MNTDIR ($MNTDIR) failed" "$FATAL"
else
	run "rm -rf $MNTDIR/*" 0 "clear MNTDIR ($MNTDIR) failed" "$FATAL"
fi

if [ ! -d "$MNTDIR_FSET_1_1" ] 
then
	run "mkdir $MNTDIR_FSET_1_1" 0  "mkdir MNTDIR_FSET_1_1 ($MNTDIR_FSET_1_1) failed" "$FATAL"
fi

# Initialize aggregates
run "$NEWAGGR $AGGR1 $BLKSIZE $FRAGSIZE -overwrite" 0 "newaggr AGGR1 ($AGGR1) failed" "$FATAL"

run "$EFTS attach $AGGR1" 0 "efts attach AGGR1 ($AGGR1) failed" "$FATAL"

run "$EFTS create $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "efts create FSET_1_1 failed" "$FATAL"

run "$EPIMOUNT $FSETID_1_1 $FSETNAME_1_1 $MNTDIR_FSET_1_1" 0 "epimount FSET_1_1 failed" "$FATAL"

echo; log "C L O N E / R E C L O N E   P H A S E"; echo
clone=1
index=1
while [ $index -le "$NUMITER" ]
do
	echo; log "CLONE/RECLONE iteration $index; `date`"; echo;
	for test in 1 2 3 4 4a 5 5a 5b 5c 5d 5e 6 7 7a 7b 8 9 
	do
		runcthon $MNTDIR_FSET_1_1 $test	
		if [ "$clone" -ne 0 ] 
		then
			run "$EFTS clone $AGGR1 $FSETID_1_1" 0\
				"Clone iter $index failed" "$FATAL"
			clone=0
		else
			run "$EFTS reclone $AGGR1 $FSETID_1_1" 0\
				"Reclone iter $index failed" "$FATAL"
			clone=1
		fi	
		run "ls $MNTDIR_FSET_1_1" 0 "Robustness check in iter $index failed" "$FATAL"		
	done 
	index=`expr $index + 1`
done

run "$EPIUNMOUNT $MNTDIR_FSET_1_1" 0 "epiunmount *$MNTDIR_FSET_1_1 (FSET_1_1) failed" "$FATAL"

run "$EFTS detach $AGGR1" 0 "efts detach AGGR1 ($AGGR1) failed" "$FATAL"

finishup "$0"
