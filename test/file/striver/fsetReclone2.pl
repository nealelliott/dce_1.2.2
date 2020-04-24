# include in package main itself	
package main;

require 'modesum.pl';

#
# Load specifications
%fsetReclone2Spec = ( MIN_FSETS,		    1,
        	  );

# Define required subroutines - fsetReclone2Parse, fsetReclone2Validate, fsetReclone2Run

sub fsetReclone2Parse {
    local($loadName, $loadNumber, $start, $end) =@_;

    if ($loadName ne "fsetReclone2") {
		&Exit("Load name specified ($loadName) should be fsetReclone2",
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
sub fsetReclone2Validate {
    local($loadName, $loadNumber)=@_;
    local($errorCount)=0;
    local($index);
	local($modesumThreadSrc);

    if ( $loadName ne "fsetReclone2" ) {
		&Exit("Load name specified ($loadName) should be fsetReclone2",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
	
    # Check if required specifications have been specified.
    if (!defined($fsetReclone2Spec{MIN_FSETS})) {
		&Log("Required minimum value for MIN_FSETS not specified for load $loadName, instance $loadNumber");
		$errorCount++;
    }

    # Handle required attributes
    if (!defined($loadAttr{$loadName, $loadNumber, LS_FSETNAMES})) {
		&Log("Filesets for load $loadName instance $loadNumber not specified");
		$errorCount++;
    }

    # DURATION defaults to forever.
    if (!defined($loadAttr{$loadName, $loadNumber, LS_DURATION})) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
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

    # Find out if specified attributes are valid.

    # Check min. number of dirs has been specified
    local(*fsets) = $loadAttr{$loadName, $loadNumber, LS_FSETNAMES};

    if (@fsets < $fsetReclone2Spec{MIN_FSETS}) {
        &Log("Load $loadName number $loadNumber - fails MIN_FSETS ($fsetReclone2Spec{MIN_FSETS}) spec");
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

sub fsetReclone2Run {
	local($loadName, $loadNumber) = @_;
	local($index);
	local($threadRepCnt, $threadLogFile);
	local($threadPid, %fsetThread);
	local($fsetThreadCnt) = 0;
	local($errors) = 0;
	local($dead);
	local($modesumThread, $modesumBackingThread, $modesumSrc);
	local($modesumThreadNolog, $modesumBackingThreadNolog);
	local($maxSleep) = 120;

	if ($loadName ne "fsetReclone2") {
		&Exit("Load name specified ($loadName) should be fsetReclone2",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}
	local(*fsets) = $loadAttr{$loadName, $loadNumber, LS_FSETNAMES};

	$threadRepCnt = $loadAttr{$loadName, $loadNumber, LS_DURATION};
	if ($threadRepcnt) {
		&Log("RL: Load fsetReclone2, instance $loadNumber, num iterations per thread = $threadRepcnt");
	} else {
		$threadRepCnt = 0 ;
		&Log("RL: Load fsetReclone2, instance $loadNumber, num iterations per thread = forever");
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

	&Log(sprintf("RL: Load fsetReclone2, instance $loadNumber, filesets are %s",
				 &AryToStr(@fsets)));

	foreach $fset (@fsets) {
		local($fsetMntDir) = &FindFsetMntPt($fset);
		if ($modesumThread) {
			$threadLogFile = "${LOGDIR}/fsetReclone2.modesum.i${loadNumber}.fset-${fset}";
			if ($threadPid = fork()) {
				&Log("RL: Forked load fsetReclone2ModesumThr, instance $loadNumber on fileset $fset at $fsetMntDir, log in $threadLogFile");		
				$modesumThread{$threadPid} = $fset;
			} else {
				&SetLogFile($threadLogFile, 0);
				&Log("Load fsetReclone2ModesumThread, instance $loadNumber starting in fileset $fset at dir $fsetMntDir");
				while (1) {
					&Sleep(rand($maxSleep));
					&ModesumRecursive($modesumSrc, $fsetMntDir,
sl									  $modesumThreadNolog);
				}
				# Should never reach here.
				exit($EXIT_SUCCESS);
			}
		}

		$threadLogFile="${LOGDIR}/fsetReclone2.i${loadNumber}.fset-${fset}";
		if ($threadPid = fork()) {
			&Log("RL: Forked load fsetReclone2, instance $loadNumber on fileset $fset, log in $threadLogFile");
			$fsetThread{$threadPid} = $fset;
			$fsetThreadCnt++;
		} else {
			&SetLogFile($threadLogFile, 0);
			&Log("Load fsetReclone2, instance $loadNumber starting in fileset $fset");
			srand(time());

			local($continue) = 1;
			local($maxReclones) = 11;
			local($destFset) = "${fset}.back1";
			local($destFsetMntPt) = $fsetMntDir . "back1";
			local($aggrName) = &FsetToAggrName($fset);
			
			&MakeDir($destFsetMntPt);

			while ($continue) {
				&Sleep(rand($maxSleep));
				&FsetClone($fset, $destFset);
				# Mount Backing Fileset
				&LocalMountFset2($destFset, $destFsetMntPt, $EXIT_LOAD_FAIL);

				# Start read threads on backing fset if specified.
				if ($modesumBackingThread) {
					$backingThreadLogFile = "${LOGDIR}/fsetReclone2.modesum.i${loadNumber}.backingFset-${destFset}";
					if ($modesumBackingThreadPid = fork()) {
						&Log("RL: Forked load fsetReclone2ModesumBackingThread, instance $loadNumber on backing fileset $destTset at $destFsetMntDir, log in $backingThreadLogFile");		
					} else {
						&SetLogFile($backingThreadLogFile, 0);
						&Log("Load fsetReclone2ModesumBackingThread, instance $loadNumber starting in backing fileset $destFset at dir $destFsetMntDir");
						while (1) {
							&Sleep(rand($maxSleep));
							&ModesumRecursive($modesumSrc, $destFsetMntPt,
											  $modesumBackingThreadNolog);
						}
                        # Should never reach here
						exit($EXIT_SUCCESS);
					}
				}

				local($numReclones) = int(rand($maxReclones));
				foreach (0 .. $numReclones - 1) {
					&Sleep(rand($maxSleep));
					&Log("Recloning $fset ${_}th time");
					&FsetReclone($fset);
				}
				if ($modesumBackingThread) {
					&DestroyProcesses($modesumBackingThreadPid);
				}
				&Sleep(rand($maxSleep));				
				&LocalUnmountFset($destFsetMntPt, $EXIT_LOAD_FAIL);
				&DeleteLFSFset($aggrName,$destFset);
				if ($threadRepCnt) {
					if (--$threadRepCnt == 0) {
						$continue = 0;
					}
				}
			}
			&Log("Load fsetReclone2, instance $loadNumber, fileset $fset completed");
			exit($EXIT_SUCCESS);
		}
	}

	# Wait for all threads
	&Log("RL: Waiting for all fsetReclone2 threads to complete");
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
			&Log("RL: Internal error - completed fsetReclone2 thread (pid $dead) not in fsetReclone2 thread table, in file __FILE__ at  __LINE__");
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

