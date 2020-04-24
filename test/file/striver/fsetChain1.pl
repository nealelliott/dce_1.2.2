# include in package main itself    
package main;

require 'modesum.pl';

#
# Load specifications
%fsetChain1Spec = ( MIN_FSETS,            1,
              );

# Define required subroutines - fsetChain1Parse, fsetChain1Validate, fsetChain1Run

sub fsetChain1Parse {
    local($loadName, $loadNumber, $start, $end) =@_;

    if ($loadName ne "fsetChain1") {
        &Exit("Load name specified ($loadName) should be fsetChain1",
              __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
         LS_FSETNAMES, MULTIPLE, REQUIRED);

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
         LS_DURATION, SINGLE, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_MODESUM_FSET_THREAD, FLAG, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_MODESUM_FSET_THREAD_NOLOG, FLAG, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_MODESUM_BACKING_FSET_THREAD, FLAG, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_MODESUM_BACKING_FSET_THREAD_NOLOG, FLAG, OPTIONAL);

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_MODESUM_SRC, SINGLE, OPTIONAL);

    0;
}

# Also sets defaults 
# return zero if all ok
sub fsetChain1Validate {
    local($loadName, $loadNumber)=@_;
    local($errorCount)=0;
    local($index);
    local($modesumThreadSrc);

    if ( $loadName ne "fsetChain1" ) {
        &Exit("Load name specified ($loadName) should be fsetChain1",
              __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
    
    # Check if required specifications have been specified.
    if (!defined($fsetChain1Spec{MIN_FSETS})) {
        &Log("Required minimum value for MIN_FSETS not specified for load $loadName, instance $loadNumber");
        $errorCount++;
    }

    # Handle required attributes
    if (!defined($loadAttr{$loadName, $loadNumber, LS_FSETNAMES})) {
        &Log("Filesets for load $loadName instance $loadNumber not specified");
        $errorCount++;
    }

	# Handle optional attributes
    if ((defined($loadAttr{$loadName, $loadNumber,
							   LS_MODESUM_FSET_THREAD}) ||
		 defined($loadAttr{$loadName, $loadNumber,
							   LS_MODESUM_BACKING_FSET_THREAD})) && 
		!defined($loadAttr{$loadName, $loadNumber,
							   LS_MODESUM_SRC})) {
		if ($ENV{LS_MODESUM_SRC}) {
			$loadAttr{$loadName, $loadNumber,LS_MODESUM_SRC} =
				$ENV{LS_MODESUM_SRC};
		} else {
			# Assume its on the path
			$loadAttr{$loadName, $loadNumber, LS_MODESUM_SRC} =	"modesum";
		}
	}

    # DURATION defaults to forever.
    if (!defined($loadAttr{$loadName, $loadNumber, LS_DURATION})) {
        $loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

    # Find out if specified attributes are valid.

    # Check min. number of dirs has been specified
    local(*fsets) = $loadAttr{$loadName, $loadNumber, LS_FSETNAMES};

    if (@fsets < $fsetChain1Spec{MIN_FSETS}) {
        &Log("Load $loadName number $loadNumber - fails MIN_FSETS ($fsetChain1Spec{MIN_FSETS}) spec");
        $errorCount++;
    }
    if (defined($loadAttr{$loadName, $loadNumber,
							  LS_MODESUM_FSET_THREAD}) || 
		defined($loadAttr{$loadName, $loadNumber,
							  LS_MODESUM_BACKING_FSET_THREAD})) {
		$modesumThreadSrc =
			$loadAttr{$loadName, $loadNumber, LS_MODESUM_SRC};
		if (!-x $modesumThreadSrc) {
			&Log("Load $loadName number $loadNumber - specified MODESUM_SRC $modesumThreadSrc not executable");
			$errorCount++;
		}
	}

    # XXX Check if filesets exist 

    $errorCount;
}

sub fsetChain1Run {
    local($loadName, $loadNumber) = @_;
    local($index);
    local($threadRepCnt, $threadLogFile);
    local($threadPid, %fsetThread);
    local($fsetThreadCnt) = 0;
    local($errors) = 0;
    local($dead);
    local($whichFset);
    local($numReclones1, $numReclones2);
    local($deletedCloneFset, $savedCloneFset);
	local($modesumThread, $modesumBackingThread, $modesumSrc);
	local($modesumThreadNolog, $modesumBackingThreadNolog);
	local($maxSleep) = 120;

    if ($loadName ne "fsetChain1") {
        &Exit("Load name specified ($loadName) should be fsetChain1",
              __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
    local(*fsets) = $loadAttr{$loadName, $loadNumber, LS_FSETNAMES};

    $threadRepCnt = $loadAttr{$loadName, $loadNumber, LS_DURATION};

    if ($threadRepcnt) {
        &Log("RL: Load fsetChain1, instance $loadNumber, num iterations per thread = $threadRepcnt");
    } else {
        $threadRepCnt = 0 ;
        &Log("RL: Load fsetChain1, instance $loadNumber, num iterations per thread = forever");
    }

	if (defined($loadAttr{$loadName, $loadNumber,
							  LS_MODESUM_FSET_THREAD})) {
		$modesumThread = 1;
	} 
	if (defined($loadAttr{$loadName, $loadNumber,
							  LS_MODESUM_BACKING_FSET_THREAD})) {
		$modesumBackingThread = 1;
	} 
	if ($modesumThread || $modesumBackingThread) {
		$modesumSrc = 
			$loadAttr{$loadName, $loadNumber, LS_MODESUM_SRC};
	}
	if (defined($loadAttr{$loadName, $loadNumber,
							  LS_MODESUM_FSET_THREAD_NOLOG})) {
		$modesumThreadNolog = 1;
	} else {
		$modesumThreadNolog = 0;
	}
	if (defined($loadAttr{$loadName, $loadNumber,
							  LS_MODESUM_BACKING_FSET_THREAD_NOLOG})) {
		$modesumBackingThreadNolog = 1;
	} else {
		$modesumBackingThreadNolog = 0;
	}

    &Log(sprintf("RL: Load fsetChain1, instance $loadNumber, filesets are %s",
                 &AryToStr(@fsets)));

    foreach $fset (@fsets) {
        local($fsetMntDir) = &FindFsetMntPt($fset);
		if ($modesumThread) {
			$threadLogFile = "${LOGDIR}/fsetChain1.modesum.i${loadNumber}.fset-${fset}";
			if ($threadPid = fork()) {
				&Log("RL: Forked load fsetChain1ModesumThr, instance $loadNumber on fileset $fset at $fsetMntDir, log in $threadLogFile");		
				$modesumThread{$threadPid} = $fset;
			} else {
				&SetLogFile($threadLogFile, 0);
				&Log("Load fsetChain1ModesumThread, instance $loadNumber starting in fileset $fset at dir $fsetMntDir");
				while (1) {
					&Sleep(rand($maxSleep));
					&ModesumRecursive($modesumSrc, $fsetMntDir,
									  $modesumThreadNolog);
				}
				# Should never reach here.
				exit($EXIT_SUCCESS);
			}
		}

        $threadLogFile="${LOGDIR}/fsetChain1.i${loadNumber}.fset-${fset}";
        if ($threadPid = fork()) {
            &Log("RL: Forked load fsetChain1, instance $loadNumber on fileset $fset, log in $threadLogFile");
            $fsetThread{$threadPid} = $fset;
            $fsetThreadCnt++;
        } else {
            &SetLogFile($threadLogFile, 0);
             &Log("Load fsetChain1, instance $loadNumber starting in fileset $fset");
			srand(time());


            local($continue) = 1;
            local($maxReclones) = 11;

            # $destFset1 is the middle fset in the chain.
            local($destFset1) = "${fset}.back1";
            local($destFset1MntPt) = $fsetMntDir . "back1";

            # $destFset2 is the last fset in the chain.
            local($destFset2) = "${fset}.back2";
            local($destFset2MntPt) = $fsetMntDir . "back2";

            local($aggrName) = &FsetToAggrName($fset);
            
            &MakeDir($destFset1MntPt);
            &MakeDir($destFset2MntPt);

            while ($continue) {
                &Sleep(rand($maxSleep));
                &FsetClone($fset, $destFset2);

                # Insert $destFset1 between $fset and $destFset2.
                &Sleep(rand($maxSleep));
                &FsetClone($fset, $destFset1);

                # Fileset chain is $fset ---> $destFset1 --> $destFset2

                # Mount backing filesets
                &LocalMountFset2($destFset1, $destFset1MntPt, $EXIT_LOAD_FAIL);
                &LocalMountFset2($destFset2, $destFset2MntPt, $EXIT_LOAD_FAIL);

				# Start read threads on backing fset if specified.
				if ($modesumBackingThread) {
					$backingFset1ThreadLogFile = "${LOGDIR}/fsetChain1.modesum.i${loadNumber}.backingFset1-${destFset1}";
					$backingFset2ThreadLogFile = "${LOGDIR}/fsetChain1.modesum.i${loadNumber}.backingFset2-${destFset2}";

					if ($modesumBackingFset1ThreadPid = fork()) {
						&Log("RL: Forked load fsetChain1ModesumBackingFset1Thread, instance $loadNumber on backing fileset1 $destFset1 at $destFset1MntPt, log in $backingFset1ThreadLogFile");		
					} else {
						&SetLogFile($backingFset1ThreadLogFile, 0);
						&Log("Load fsetChain1ModesumBackingFset1Thread, instance $loadNumber starting in backing fileset1 $destFset1 at dir $destFset1MntPt");
						while (1) {
							&Sleep(rand($maxSleep));
							&ModesumRecursive($modesumSrc, $destFset1MntPt,
											  $modesumBackingThreadNolog);
						}
                        # Should never reach here
						exit($EXIT_SUCCESS);
					}

					if ($modesumBackingFset2ThreadPid = fork()) {
						&Log("RL: Forked load fsetChain1ModesumBackingFset2Thread, instance $loadNumber on backing fileset2 $destFset2 at $destFset2MntPt, log in $backingFset2ThreadLogFile");		
					} else {
						&SetLogFile($backingFset2ThreadLogFile, 0);
						&Log("Load fsetChain1ModesumBackingFset2Thread, instance $loadNumber starting in backing fileset2 $destFset2 at dir $destFset2MntPt");
						while (1) {
							&Sleep(rand($maxSleep));
							&ModesumRecursive($modesumSrc, $destFset2MntPt,
											  $modesumBackingThreadNolog);
						}
                        # Should never reach here
						exit($EXIT_SUCCESS);
					}
				}

                # For a random number of times, reclone either $fset into 
                # $destFset1 or $destFset1 into $destFset2 at a random 
                # instant
                $numReclones1 = int(rand($maxReclones));
                $numReclones2 = int(rand($maxReclones));
                foreach (0 .. $numReclones1 - 1) {
                    &Sleep(rand($maxSleep));
                    # Choose which fileset to reclone from.
                    $whichFset = int(rand(2));
                    if ($whichFset == 0) {
                        # Reclone R/W i.e. $fset into $destFset1
                        &Log("Recloning (iter ${_}) $fset into $destFset1");
                        &FsetReclone2($aggrName, $fset);
                    } else {
                        # Reclone 1st clone $destFset1 into 2nd $destFset2
                        &Log("Recloning (iter ${_}) $destFset1 into $destFset2");
                        &FsetReclone2($aggrName, $destFset1);
                    }
                }
                &Sleep(rand($maxSleep));                
                
                # Destroy one of the backing filesets ($destFset1 or 
                # $destFset2)
                $whichFset = int(rand(2));
                if ($whichFset == 0) {
					# Stop any modesum threads
					if ($modesumBackingThread) {
						&DestroyProcesses($modesumBackingFset1ThreadPid);
					}					

                    # Destroy $destFset1
                    &LocalUnmountFset($destFset1MntPt, $EXIT_LOAD_FAIL);
                    $deletedCloneFset = $destFset1;
                    $savedCloneFset = $destFset2;
                    $savedCloneFsetMntPt = $destFset2MntPt;
					$savedModesumBackingFsetThreadPid =
						$modesumBackingFset2ThreadPid;
                } else {
					# Stop any modesum threads
					if ($modesumBackingThread) {
						&DestroyProcesses($modesumBackingFset2ThreadPid);
					}					

                    # Destroy $destFset2
                    &LocalUnmountFset($destFset2MntPt, $EXIT_LOAD_FAIL);
                    $deletedCloneFset = $destFset2;
                    $savedCloneFset = $destFset1;
                    $savedCloneFsetMntPt = $destFset1MntPt;
					$savedModesumBackingFsetThreadPid =
						$modesumBackingFset1ThreadPid;
                }
                &DeleteLFSFset($aggrName, $deletedCloneFset);
                
                # For a random number of times, reclone the R/W fset $fset
                # into the remaining backing fileset.
                foreach (0 .. $numReclones2 - 1) {
                    &Sleep(rand($maxSleep));
                    # Reclone R/W i.e. $fset into $destFset1
                    &Log("Recloning (iter ${_}) $fset into saved clone $savedCloneFset");
                    &FsetReclone2($aggrName, $fset);
                }
                &Sleep(rand($maxSleep));

                # Stop any modesum threads
				if ($modesumBackingThread) {
					&DestroyProcesses($modesumBackingFset2ThreadPid);
				}					
				# Destroy remaining clone
                &LocalUnmountFset($savedCloneFsetMntPt);
				&DeleteLFSFset($aggrName, $savedCloneFset);
                if ($threadRepCnt) {
                    if (--$threadRepCnt == 0) {
                        $continue = 0;
                    }
                }
            }
            &Log("Load fsetChain1, instance $loadNumber, fileset $fset completed");
            exit($EXIT_SUCCESS);
        }
    }

    # Wait for all threads
    &Log("RL: Waiting for all fsetChain1 threads to complete");
    # Move on if all fset threads have completed.
    while ($fsetThreadCnt != 0) {
        $dead = wait;
        if (defined($fsetThread{$dead})) {
            $fset = $fsetThread{$dead};
            if ($?) {
                $errors++;
                &Log("RL: Load $load, instance $loadNumber, fileset $fset (pid $dead) failed, exit status $?");
                # remove $dead from %fsetThread
            } else {
                &Log("RL: Load $load, instance $loadNumber, fileset $fset (pid $dead) passed");
            }
            undef($fsetThread{$dead});
            $fsetThreadCnt--;
        } elsif (defined($modesumThread{$dead})) {
            &Log("RL: Internal error - modesum thread pid $dead for fset $modesumThread{$dead} exited prematurely");
        } else {
            &Log("RL: Internal error - completed fsetChain1 thread (pid $dead) not in fsetChain1 thread table, in file __FILE__ at  __LINE__");
        }
    }

    # Kill all modesum threads
    &DestroyProcesses(keys(%modesumThread));

    $errors;
}

1;

__END__

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:

