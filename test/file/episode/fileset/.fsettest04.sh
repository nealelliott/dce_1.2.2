#!/bin/sh

FSETTESTDIR=${FSETTESTDIR:-/afs/tr/usr/rajesh/epi/test/recovery/fset}
export FSETTESTDIR

# Load base routines and variables
. ${FSETTESTDIR}/fsettestbasic

NUMITER=${NUMITER:-200}
NUMFSET=${NUMFSET:-5}
NUMCTHON=${NUMCTHON:-5}
LOGCTHON=${LOGCTHON:-0}

export NUMITER NUMFSET NUMCTHON LOGCTHON

# 
# Main program
#

logstarttest

# Create mount points parent dir
if [ ! -d "$MNTDIR" ] 
then
	run "mkdir $MNTDIR" 0  "mkdir MNTDIR ($MNTDIR) failed" "$FATAL"
else
	run "rm -rf $MNTDIR/*" 0 "clear MNTDIR ($MNTDIR) failed" "$FATAL"
fi

# Initialize aggregates
run "$NEWAGGR $AGGR1 $BLKSIZE $FRAGSIZE -overwrite" 0 "newaggr AGGR1 ($AGGR1) failed" "$FATAL"

run "$EFTS attach $AGGR1" 0 "efts attach AGGR1 ($AGGR1) failed" "$FATAL"

# Initialize filesets
index=1
while [ "$index" -le "$NUMFSET" ]
do
	fsetId=`calcFsetID $BASE_FSETID_AGGR1 $index`
	fsetName=`detFsetName $BASE_FSETNAME_AGGR1 $index`
	fsetMntDir="${MNTDIR}/${fsetName}"

	# Create fileset	
	run "$EFTS create $AGGR1 -id $fsetId -name $fsetName" 0 "efts create fileset number $index" "$FATAL"

	# Create fileset mount point
	if [ ! -d "$fsetMntDir" ]
	then
		run "mkdir $fsetMntDir" 0  "mkdir mount dir for fileset number $index" "$FATAL"
	fi	

	#Mount fileset
	run "$EPIMOUNT $fsetName $fsetMntDir" 0 "epimount fileset number $index failed" "$FATAL"	

	# Create cthon test dirs
	index2=1
	while [ "$index2" -le "$NUMCTHON" ]
	do
		# Create cthon test dirs
		run "mkdir ${fsetMntDir}/dir${index2}" 0
		index2=`expr $index2 + 1`
	done
	index=`expr $index + 1`
done

# Start cthons
if [ "$LOGCTHON" -ne 0 ] 
then
	cthonLogDir="${FSETLOGDIR}/fsettest04a.$$"
	cthonLogMsg="Starting Cthon, log in $cthonLogDir"
else 
	cthonLogDir="/dev/null"
	cthonLogMsg="Starting Cthon, not logged"
fi

log "${cthonLogMsg}"
( 
	sh ${FSETTESTDIR}/fsettest04a "$$" > $cthonLogDir
) &  
cthonPid=$!
log "Sleep a while"
sleep 20;

# Start fileset ops script
filesetLog="${FSETLOGDIR}/fsettest04b.$$"
log "Starting fileset operations, log in $filesetLog"

log "Starting fileset operations"
sh ${FSETTESTDIR}/fsettest04b 2>&1 | tee -a $filesetLog

# time up
log "Killing cthon tests"
kill -30 $cthonPid
wait $cthonPid

log "Sleep 20 secs"
sleep 20

finishup "$0"

# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
