# include in package main itself	
package main;

#
# Load specifications
# 
%cthonSpec = (MIN_DIRS,		    1,
			  MIN_DIR_QUOTA,    "1.1M",
			  MIN_DURATION,		1,
			  MIN_INTENSITY,	1,
			  DEF_INTENSITY,	1
			  );

# Canonalize cthon load specs
$cthonSpec{MIN_DIR_QUOTA}=&SizeInKB($cthonSpec{MIN_DIR_QUOTA});

# Define required subroutines - cthonParse, cthonValidate, cthonRun

sub cthonParse {
    local($loadName, $loadNumber, $start, $end) =@_;

	if ( $loadName ne "cthon" ) {
		&Exit("Load name specified ($loadName) should be cthon",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DIRS, MULTIPLE, REQUIRED);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DURATION, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_CONCURRENCY, SINGLE, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_INTENSITY, SINGLE, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_ENVVARS, MULTIPLE, OPTIONAL);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_CMDARGS, MULTIPLE, OPTIONAL);

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_CTHON_SRCDIR, SINGLE, REQUIRED );
	0;
}

# Also sets defaults 
# return zero if all ok
sub cthonValidate {
    local($loadName, $loadNumber)=@_;
    local($dir);
    local($errorCount)=0;
    local($index);
    local($quotaSpec);
    local($nthreads) = 0;

	if ( $loadName ne "cthon" ) {
		&Exit("Load name specified ($loadName) should be cthon",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}
	

    # Check if required specifications have been specified.
	if ( !defined($cthonSpec{DEF_INTENSITY}) ) {
		&Log("Required default value for DEF_INTENSITY not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}
    if ( !defined($cthonSpec{MIN_DIR_QUOTA}) ) {
		&Log("Required specification - MIN_DIR_QUOTA not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

    # Handle required attributes
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DIRS}) ) {
		&Log("Dirs for load $loadName instance $loadNumber not specified");
		$errorCount++; 
	}

	# Set intensity to default value always for now. At some point
	# when multiple intensity levels are used, uncomment out the #
	# in the following lines  for INTENSITY handling.
#	if ( !defined($loadAttr{$loadName, $loadNumber, LS_INTENSITY}) ) {
		$loadAttr{$loadName, $loadNumber, LS_INTENSITY} =
			$cthonSpec{DEF_INTENSITY};
#    } 
	
    # DURATION defaults to forever.
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DURATION}) ) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

    # Find out if specified attributes are valid.

    # Check min. number of dirs has been specified
    local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
    if (@dirs < $cthonSpec{MIN_DIRS}) {
		&Log("Load $loadName number $loadNumber - fails MIN_DIR ($cthonSpec{MIN_DIR}) spec");
		$errorCount++;
    }
	
    # Check quota in each dir and determine number of threads that can be run
	# in the dirs specified
    $quotaSpec=$cthonSpec{MIN_DIR_QUOTA};
    foreach (@dirs) {
		$dirQuota=&DirToQuota($_);
		if ( $dirQuota >= $quotaSpec ) {
			$nthreads += int($dirQuota/$quotaSpec);
		}
    }
	
    #Check sufficient space allocated to tests
   	# If CONCURRENCY not specified, run as many threads as possible (at
    # least 1) else as many as specified. 
	if ( !$nthreads ) {
		&Log("Load $loadName number $loadNumber - insufficient dir space for even 1 thread");
		$errorCount++;
	} elsif ( $loadAttr{$loadName, $loadNumber, LS_CONCURRENCY} ) {
		local($desiredNThreads) =
			$loadAttr{$loadName, $loadNumber, LS_CONCURRENCY};
		if ( $nthreads < $desiredNThreads ) {
			&Log("Load $loadName number $loadNumber - insufficient dir space desired $desiredNThreads threads, (max. $nthreads threads)");
			$errorCount++;
		} 
	} 
	$errorCount;
}

sub cthonRun {
	local($loadName, $loadNumber)=@_;
	local($index);
	local($nThreads, $threadRepCnt, $threadDir,$threadLogFile, $pwd);
	local($threadIndex, $threadPid);
	local($errors)=0;
	local($quotaSpec, $dirQuota);
	local($cthonBinSrc, $cthonExecFile);
	local($dir, $nDirThreads);
	local($dead);
	local(%cthonThread);

	if ( $loadName ne "cthon" ) {
		&Exit("Load name specified ($loadName) should be cthon",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

	local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
	# Determine number of threads that can be run in the dirs specified XXX
    $quotaSpec=$cthonSpec{MIN_DIR_QUOTA};
    foreach (@dirs) {
		$dirQuota=&DirToQuota($_);
		if ( $dirQuota >= $quotaSpec ) {
			$dirThreadCount{$_} = int($dirQuota/$quotaSpec);
		}
    }

	# if CONCURRENCY specified, run only that many
	$nThreads=$loadAttr{$loadName, $loadNumber, LS_CONCURRENCY};
	$threadRepCnt=$loadAttr{$loadName, $loadNumber, LS_DURATION};
	if ( $nThreads ) {
		if ( $threadRepCnt ) {
			$threadRepCnt = &RoundUp($threadRepCnt / $nThreads);
            # num threads = $nthread, num iters per thread = $threadRepCnt 
			&Log("RL: Load cthon, instance $loadNumber: num threads = $nThreads, num iterations per thread = $threadRepCnt");
		} else {
			$threadRepCnt=0;
			# num threads = $nthreads, num iters per thread = forever
			&Log("RL: Load cthon, instance $loadNumber: num threads = $nThreads, num iterations per thread = forever");
		}
	} else {
		# num threads = as many as allowed by dir space
		$nThreads=0;
		local($sum)=0;
		grep($sum+=$_,values(%dirThreadCount));
		if ( $threadRepCnt ) {
			$threadRepCnt = &RoundUp($threadRepCnt / $sum );
            # num threads = $sum, num iters per thread = $threadRepCnt 
			&Log("RL: Load cthon, instance $loadNumber: num threads = $sum (dir space dependent), num iterations per thread = $threadRepCnt");
		} else {
			$threadRepCnt=0;
			# num threads = $sum, num iters per thread = forever
			&Log("RL: Load cthon, instance $loadNumber: num threads = $sum (dir space dependent), num iterations per thread = forever");
		}
	}

	$cthonBinSrc=$loadAttr{$loadName, $loadNumber, LS_CTHON_SRCDIR};
	$ENV{CDIR}=$cthonBinSrc;
	$cthonExecFile="${cthonBinSrc}/run";

	$threadIndex=0;
	while ( ($dir,$nDirThreads) = each(%dirThreadCount) ) {
		if ($nThreads) {
			# Could not find a way of initialising "each" to 
			# start fresh next time, so the following kludge.
			next if ($threadIndex >= $nThreads);

			$nDirThreads = &Min($nThreads - $threadIndex,
								$nDirThreads);
		}
		foreach (0..$nDirThreads-1) {
			$threadDir="$dir/cthon.i${loadNumber}.t${threadIndex}";
			&MakeDir($threadDir,0777);
			$threadLogFile="${LOGDIR}/cthon.i${loadNumber}.t${threadIndex}";

            #Run cthon number $threadIndex+$_
			if ( $threadPid = fork() ) {
               &Log("RL: Forked load cthon, instance $loadNumber thread $threadIndex (pid = $threadPid), log in $threadLogFile");
				$cthonThread{$threadPid}=join(' ',$threadIndex,$threadDir);
			} else {
	            &SetLogFile($threadLogFile,0);
				&Sleep(10);
				&Chdir($threadDir);
				chop($pwd=`pwd`);
	            &Log("THR: Load cthon, instance $loadNumber thread $threadIndex starting in $pwd");
				if ($threadRepCnt) {
					foreach (0..$threadRepCnt-1) {
						&Log("THR: Starting iteration $_ of cthon instance $loadNumber, thread $threadIndex");
						&Sys("$cthonExecFile") &&
							&Exit("Failure of iteration $_ of cthon instance $loadNumber, thread $threadIndex ", __FILE__, __LINE__ );
						&Log("THR: Success of iteration $_ cthon instance $loadNumber, thread $threadIndex");
					}
				} else {
					local($iter)=0;
					while ( 1 ) {
						&Log("THR: Starting cthon instance $loadNumber, thread $threadIndex iteration $iter");
						&Sys("$cthonExecFile") &&
							&Exit("Failure of iteration $iter of Cthon instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );
						&Log("THR: Success of iteration $iter of cthon instance $loadNumber, thread $threadIndex");
						$iter++;
					}
				}
				&Log("THR: Load cthon, instance $loadNumber thread $threadIndex completed");		
				exit($EXIT_SUCCESS);				
			}
			$threadIndex++;
		}
	}

	# Wait for all threads.
	&Log("RL: Waiting for all cthon threads to complete");
	while ( ($dead = wait) != -1 ) { 
		if ( !defined($cthonThread{$dead}) ) {
			&Log("RL: Internal error - completed cthon thread (pid $dead) not in cthon thread table, in file __FILE__ at  __LINE__");
		} else {
			($threadIndex, $threadDir)=split(' ',$cthonThread{$dead});
			if ( $? ) {
				$errors++;
				&Log("RL: Load $load, instance $loadNumber, thread $threadIndex in dir $threadDir (pid $dead) failed, exit status $?");
				# remove $dead from %cthonThread

			} else {
				&Log("RL: Load $load, instance $loadNumber, thread $threadIndex in dir $threadDir (pid $dead) passed");
			}
			undef($cthonThread{$dead});
		}
	}
	$errors;
}

1;

__END__
# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:


