#!/bin/sh

. "${FSETTESTDIR}/fsettestfunc"

# Create clones for each fileset
log "Create clones for each fileset"
index=1
while [ "$index" -le "$NUMFSET" ]
do
	fsetId=`calcFsetID $BASE_FSETID_AGGR1 $index`		
	targetFsetID=`expr $fsetId + $OFFSET_FACTOR_CLONE_FSET_ID`
	run "$EFTS clone $AGGR1 $fsetId -destid $targetFsetID" 0 \
	  "Initial clone of fileset $fsetId in aggregate $AGGR failed" "$FATAL"
	index=`expr $index + 1`		
done

#Perform Clone and reclone alternately
log "Perform Clone and reclone alternately"

clone=0
iter=1

niter=`expr $NUMITER \* 2 - 1 `
while [ "$iter" -le "$niter" ]
do
	log "Starting iteration $iter"
	index=1

	log "Sleep for 1 sec"
	sleep 1

	while [ "$index" -le "$NUMFSET" ]
	do
		srcFsetId=`calcFsetID $BASE_FSETID_AGGR1 $index`
		targetFsetID=`expr $srcFsetId + \( $iter / 2 + 1 \) \* $OFFSET_FACTOR_CLONE_FSET_ID`
		if [ "$clone" -eq 1 ]
		then
			run "$EFTS clone $AGGR1 $srcFsetId -destid $targetFsetID" 0 \
			 "Clone of fileset $srcFsetId in aggregate $AGGR1 failed" "$FATAL" 
		else
			run  "$EFTS reclone $AGGR1 $srcFsetId" 0\
			 "Reclone of fileset $srcFsetId in aggregate $AGGR1 failed" "$FATAL"
		fi
		index=`expr $index + 1`		
	done
	clone=`expr 1 - $clone`
	iter=`expr $iter + 1`
done


# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
