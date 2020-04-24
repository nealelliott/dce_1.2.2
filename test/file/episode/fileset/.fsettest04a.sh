#!/bin/sh
#set -x

. "${FSETTESTDIR}/fsettestfunc"
# set variable expected by the cthon scripts				
CDIR="$CTHONDIR"; export CDIR

trap '
	log "$0 received SIGUSR1"
	log "$0 kill cthon threads"
	for pid in $cthonPids
	do	
		kill -KILL $pid
	done
	log "$0 Waiting for all cthon threads to complete ($cthonPids)"
	wait
	log "$0 Exiting"
	exit
   ' 30

cthonPids=""
pid=$$

log "Number of filesets = $NUMFSET, Number of cthon threads = $NUMCTHON"
index1=1;	
while [ "$index1" -le "$NUMFSET" ]
do
	# Start cthons in fileset number "index1"
	fsetId=`calcFsetID $BASE_FSETID_AGGR1 $index1`
	fsetName=`detFsetName $BASE_FSETNAME_AGGR1 $index1`
	fsetMntDir="${MNTDIR}/${fsetName}"

	log "Starting cthon on fileset # $index1, id $fsetId, mounted at $fsetMntDir"
	index2=1
	while [ "$index2" -le "$NUMCTHON" ]
	do
		# Create cthon test dirs
		cthonDir="${fsetMntDir}/dir${index2}"

		log "Start cthon thread # $index2 for fileset $index1"
		( 
			cycle=1
			while [ 1 ] 
			do
				if [ "$LOGCTHON" -ne 0 ]
				then
					cthonThreadLogDir="${FSETLOGDIR}/cthon.${pid}.f${index1}.d${index2}.c${cycle}"
					cthonThreadLogMsg="cthon thread # $index2, for fileset #$index1, cycle # $cycle in `pwd`, log in $cthonThreadLogDir" 
				else
					cthonThreadLogDir="/dev/null"
					cthonThreadLogMsg="cthon thread # $index2, for fileset #$index1, cycle # $cycle in `pwd`, not logged" 
				fi

				cd $cthonDir

				log "${cthonThreadLogMsg}"

				$CTHONDIR/runsmall > $cthonThreadLogDir
				if [ $? -ne 0 ] 
				then
					log "Failure in cthon tests fileset $index1, cthon $index2 cycle $cycle; exiting"					
					exit 1
				fi
				cycle=`expr $cycle + 1`
			done				
		) &
		childpid=$!
		log "Cthon pid $childpid is thread # $index2 for fileset # $index1"
		cthonPids="$cthonPids $childpid"	
			
		index2=`expr $index2 + 1`
	done
	index1=`expr $index1 + 1`
done

# wait for all children or SIGUSR1 from parent
wait

# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
