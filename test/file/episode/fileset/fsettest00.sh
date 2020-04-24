#!/bin/sh

FSETTESTDIR=${FSETTESTDIR:-/afs/tr/usr/rajesh/epi/test/recovery/fset}
# Load base routines and variables
. ${FSETTESTDIR}/fsettestbasic

FSETID_1_1=`calcFsetID $BASE_FSETID_AGGR1 1`
FSETNAME_1_1=`detFsetName $BASE_FSETNAME_AGGR1 1`
MNTDIR_FSET_1_1="${MNTDIR}/${FSETNAME_1_1}"

FSETID_1_2=`calcFsetID $BASE_FSETID_AGGR1 2`
FSETNAME_1_2=`detFsetName $BASE_FSETNAME_AGGR1 2`
MNTDIR_FSET_1_2="${MNTDIR}/${FSETNAME_1_2}"

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

#1.
#attach
#create
#mount
#unmount
#detach
case=1
logcasestart $case
run "$NEWAGGR $AGGR1 $BLKSIZE $FRAGSIZE -overwrite" 0 "newaggr AGGR1 ($AGGR1) failed" "$FATAL"
run "$EFTS attach $AGGR1" 0 "efts attach AGGR1 ($AGGR1) failed" "$FATAL"
run "$EFTS create $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "efts create FSET_1_1 failed" "$FATAL"
run "$EPIMOUNT $FSETID_1_1 $MNTDIR_FSET_1_1" 0 "epimount FSET_1_1 failed" "$FATAL"

curDir=`pwd`
run "cd $MNTDIR_FSET_1_1" 0 "cd $MNTDIR_FSET_1_1 failed" "$FATAL"
run "ls > /dev/null" 0 "ls of $MNTDIR_FSET_1_1 failed" "$FATAL"
run "cd $curDir" 0 "cd out of $MNTDIR_FSET_1_1 failed" "$FATAL"
run "touch $MNTDIR_FSET_1_1/file1" 0 "touch $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "mkdir $MNTDIR_FSET_1_1/dir1" 0 "mkdir $MNTDIR_FSET_1_1/dir1 failed" "$FATAL"
run "touch $MNTDIR_FSET_1_1/dir1/file1" 0 "touch $MNTDIR_FSET_1_1/dir1/file1 failed" "$FATAL"
run "cat /etc/passwd >> $MNTDIR_FSET_1_1/file1" 0 "cat /etc/passwd >> $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "ls -l $MNTDIR_FSET_1_1/file1" 0 "ls -l of $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "diff /etc/passwd $MNTDIR_FSET_1_1/file1" 0 "diff /etc/passwd $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "cat $MNTDIR_FSET_1_1/file1 > /dev/null" 0 "cat $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "rm -f $MNTDIR_FSET_1_1/dir1/file1" 0 "rm -f $MNTDIR_FSET_1_1/dir1/file1 failed" "$FATAL"
run "rmdir $MNTDIR_FSET_1_1/dir1" 0 "rm -f $MNTDIR_FSET_1_1/dir1 failed" "$FATAL"

run "$EPIUNMOUNT $MNTDIR_FSET_1_1" 0 "epiunmount FSET_1_1 failed" "$NONFATAL"
run "$EFTS detach $AGGR1" 0 "efts detach AGGR1 ($AGGR1) failed" "$FATAL"



#2.
#attach
#create
#delete
#detach
case=2
logcasestart $case
run "$NEWAGGR $AGGR1 $BLKSIZE $FRAGSIZE -overwrite" 0 "newaggr AGGR1 ($AGGR1) failed" "$FATAL"
run "$EFTS attach $AGGR1" 0 "efts attach AGGR1 ($AGGR1) failed" "$FATAL"
run "$EFTS create $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "efts create FSET_1_1 failed" "$FATAL"
run "$EFTS delete $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "efts delete FSET_1_1 failed" "$FATAL"
run "$EFTS detach $AGGR1" 0 "efts detach AGGR1 ($AGGR1) failed" "$FATAL"

#3.
#attach 
#create
#mount 
#unmount
#delete
#detach
#
case=3
logcasestart $case
run "$NEWAGGR $AGGR1 $BLKSIZE $FRAGSIZE -overwrite" 0 "newaggr AGGR1 ($AGGR1) failed" "$FATAL"
run "$EFTS attach $AGGR1" 0 "efts attach AGGR1 ($AGGR1) failed" "$FATAL"
run "$EFTS create $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "efts create FSET_1_1 failed" "$FATAL"
run "$EPIMOUNT $FSETID_1_1 $MNTDIR_FSET_1_1" 0 "epimount FSET_1_1 failed" "$FATAL"
curDir=`pwd`
run "cd $MNTDIR_FSET_1_1" 0 "cd $MNTDIR_FSET_1_1 failed" "$FATAL"
run "ls > /dev/null" 0 "ls of $MNTDIR_FSET_1_1 failed" "$FATAL"
run "cd $curDir" 0 "cd out of $MNTDIR_FSET_1_1 failed" "$FATAL"
run "touch $MNTDIR_FSET_1_1/file1" 0 "touch $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "mkdir $MNTDIR_FSET_1_1/dir1" 0 "mkdir $MNTDIR_FSET_1_1/dir1 failed" "$FATAL"
run "touch $MNTDIR_FSET_1_1/dir1/file1" 0 "touch $MNTDIR_FSET_1_1/dir1/file1 failed" "$FATAL"
run "cat /etc/passwd >> $MNTDIR_FSET_1_1/file1" 0 "cat /etc/passwd >> $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "ls -l $MNTDIR_FSET_1_1/file1" 0 "ls -l of $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "diff /etc/passwd $MNTDIR_FSET_1_1/file1" 0 "diff /etc/passwd $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "cat $MNTDIR_FSET_1_1/file1 > /dev/null" 0 "cat $MNTDIR_FSET_1_1/file1 failed" "$FATAL"
run "rm -f $MNTDIR_FSET_1_1/dir1/file1" 0 "rm -f $MNTDIR_FSET_1_1/dir1/file1 failed" "$FATAL"
run "rmdir $MNTDIR_FSET_1_1/dir1" 0 "rm -f $MNTDIR_FSET_1_1/dir1 failed" "$FATAL"
run "$EPIUNMOUNT $MNTDIR_FSET_1_1" 0 "epiunmount FSET_1_1 failed" "$NONFATAL"
run "$EFTS delete $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "efts delete FSET_1_1 failed" "$FATAL"
run "$EFTS create $AGGR1 -id $FSETID_1_1 -name $FSETNAME_1_1" 0 "2nd efts create FSET_1_1 failed" "$FATAL"
run "$EPIMOUNT $FSETID_1_1 $MNTDIR_FSET_1_1" 0 "2nd epimount FSET_1_1 failed" "$FATAL"
run "$EPIUNMOUNT $MNTDIR_FSET_1_1" 0 "2nd epiunmount FSET_1_1 failed" "$NONFATAL"
run "$EFTS detach $AGGR1" 0 "efts detach AGGR1 ($AGGR1) failed" "$FATAL"

finishup "$0"


