# include in package main itself	
package main;

#
# Load specifications
# 
%gccSpec = (  MIN_DIRS,		               1,
			  MIN_DIR_QUOTA,               "62M",
			  MIN_DIR_NO_BOOTSTRAP_QUOTA,  "34M",
			  MIN_DURATION,		           1,
			  DEF_CONCURRENCY,	           1
			);

# Canonalize gcc load specs
$gccSpec{MIN_DIR_QUOTA}=&SizeInKB($gccSpec{MIN_DIR_QUOTA});
$gccSpec{MIN_DIR_NO_BOOTSTRAP_QUOTA} =
	&SizeInKB($gccSpec{MIN_DIR_NO_BOOTSTRAP_QUOTA});

%gccTarget = ( "AIX", "rs6000-ibm-aix" );

# Define required subroutines - gccParse, gccValidate, gccRun

sub gccParse {
    local($loadName, $loadNumber, $start, $end) =@_;

	if ( $loadName ne "gcc" ) {
		&Exit("Load name specified ($loadName) should be gcc",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
				 LS_DIRS, MULTIPLE, REQUIRED);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
				 LS_DURATION, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
				 LS_CONCURRENCY, SINGLE, OPTIONAL);

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
				 LS_GCC_SRCDIR, SINGLE, REQUIRED );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
				 LS_GCC_NO_BOOTSTRAP, FLAG, OPTIONAL );
	0;
}

# Also sets defaults 
# return zero if all ok
sub gccValidate {
    local($loadName, $loadNumber)=@_;
    local($dir);
    local($errorCount)=0;
    local($index);
    local($quotaSpec);
    local($nthreads) = 0;

	if ( $loadName ne "gcc" ) {
		&Exit("Load name specified ($loadName) should be gcc",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}
	

    # Check if required specifications have been specified.
    if ( !defined($gccSpec{DEF_CONCURRENCY}) ) {
		&Log("Required default value for DEF_CONCURRENCY not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}
    if ( !defined($gccSpec{MIN_DIR_QUOTA}) ) {
		&Log("Required specification - MIN_DIR_QUOTA not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

    if ( !defined($gccSpec{MIN_DIR_NO_BOOTSTRAP_QUOTA}) ) {
		&Log("Required specification - MIN_DIR_NO_BOOTSTRAP_QUOTA not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

    # Handle required attributes
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DIRS}) ) {
		&Log("Dirs for load $loadName instance $loadNumber not specified");
		$errorCount++ 
	}

	$gccSrcDir=$loadAttr{$loadName, $loadNumber, LS_GCC_SRCDIR};
	if ( ! -d $gccSrcDir ) {
		&Log("Specified gcc source dir $gccSrcDir for load $loadName instance $loadNumber does not exist");
		$errorCount++;
	}
		
    # DURATION defaults to forever.
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DURATION}) ) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

    # Find out if specified attributes are valid.


    # Check min. number of dirs has been specified
    local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
    if (@dirs < $gccSpec{MIN_DIRS}) {
		&Log("Load $loadName number $loadNumber - fails MIN_DIR ($gccSpec{MIN_DIR}) spec");
		$errorCount++;
    }
	
    # Check quota in each dir and determine number of threads that can be run
	# in the dirs specified
	if ( defined($loadAttr{$loadName, $loadNumber, LS_GCC_NO_BOOTSTRAP}) ) {
		$quotaSpec=$gccSpec{MIN_DIR_NO_BOOTSTRAP_QUOTA};
	} else {
		$quotaSpec=$gccSpec{MIN_DIR_QUOTA};
	}
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

	# Binary checking
	chop($gnumake=`which gnumake`);
	if ( $gnumake=~ m/^no/ ) {
		&Log("Load $loadName number $loadNumber - needed binary gnunake not accessible");
		$errorCount++;
	}

	chop($osType=`uname`);
	if (! grep($osType eq $_, %gccTarget)) {
		&Log("Load $loadName number $loadNumber - not implemented on $osType yet");
		$errorCount++;
	}
	$errorCount;
}

sub gccRun {
	local($loadName, $loadNumber)=@_;
	local($index);
	local($nThreads, $threadRepCnt, $threadDir,$threadLogFile, $pwd);
	local($threadIndex, $threadPid);
	local($errors)=0;
	local($quotaSpec, $dirQuota);
	local($dir, $nDirThreads);
	local($dead);
	local(%gccThread);

	if ( $loadName ne "gcc" ) {
		&Exit("Load name specified ($loadName) should be gcc",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

	local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
	# Determine number of threads that can be run in the dirs specified XXX
    $quotaSpec=$gccSpec{MIN_DIR_QUOTA};
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
			&Log("RL: Load gcc, instance $loadNumber: num threads = $nThreads, num iterations per thread = $threadRepCnt");
		} else {
			$threadRepCnt=0;
			# num threads = $nthreads, num iters per thread = forever
			&Log("RL: Load gcc, instance $loadNumber: num threads = $nThreads, num iterations per thread = forever");
		}
	} else {
		# num threads = as many as allowed by dir space
		$nThreads=0;
		local($sum)=0;
		grep($sum+=$_,values(%dirThreadCount));
		if ( $threadRepCnt ) {
			$threadRepCnt = &RoundUp($threadRepCnt / $sum );
            # num threads = $sum, num iters per thread = $threadRepCnt 
			&Log("RL: Load gcc, instance $loadNumber: num threads = $sum (dir space dependent), num iterations per thread = $threadRepCnt");
		} else {
			$threadRepCnt=0;
			# num threads = $sum, num iters per thread = forever
			&Log("RL: Load gcc, instance $loadNumber: num threads = $sum (dir space dependent), num iterations per thread = forever");
		}
	}

	$gccSrcDir=$loadAttr{$loadName, $loadNumber, LS_GCC_SRCDIR};
	$gccNoBootstrap=$loadAttr{$loadName, $loadNumber, LS_GCC_NO_BOOTSTRAP};
	&Log("Load gcc instance $loadNumber no bootstrap checking")
		if defined($gccNoBootstrap);

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
			$threadDir="$dir/gcc.i${loadNumber}.t${threadIndex}";
			&MakeDir($threadDir,0777);
			$threadLogFile="${LOGDIR}/gcc.i${loadNumber}.t${threadIndex}";

            #Run gcc number $threadIndex+$_
			if ( $threadPid = fork() ) {
               &Log("RL: Forked load gcc, instance $loadNumber thread $threadIndex (pid = $threadPid), log in $threadLogFile");
				$gccThread{$threadPid}=join(' ',$threadIndex,$threadDir);
			} else {
	            &SetLogFile($threadLogFile,0);
				&Sleep(10);
				&Chdir($threadDir);
				chop($pwd=`pwd`);
	            &Log("THR: Load gcc, instance $loadNumber thread $threadIndex starting in $pwd");
				chop($osType=`uname`);
				$target=$gccTarget{$osType};
				&Sys("${gccSrcDir}/configure --srcdir=${gccSrcDir} --target=$target") && 
					&Exit("Load $load instance $loadNumber - configuration failed",
						  __FILE__, __LINE__ );
				if ($threadRepCnt) {
					foreach (0..$threadRepCnt-1) {
						&Log("THR: Starting iteration $_ of gcc instance $loadNumber, thread $threadIndex");
						&gccExec($_, $gccNoBootstrap);
						&Log("THR: Success of iteration $_ gcc instance $loadNumber, thread $threadIndex");
					}
				} else {
					local($iter)=0;
					while ( 1 ) {
						&Log("THR: Starting gcc instance $loadNumber, thread $threadIndex iteration $iter");
						&gccExec($iter, $gccNoBootstrap);
						&Log("THR: Success of iteration $iter of gcc instance $loadNumber, thread $threadIndex");
						$iter++;
					}
				}
				&Log("THR: Load gcc, instance $loadNumber thread $threadIndex completed");		
				exit($EXIT_SUCCESS);
			}
			$threadIndex++;
		}
	}

	# Wait for all threads.
	&Log("RL: Waiting for all gcc threads to complete");
	while ( ($dead = wait) != -1 ) { 
		if ( !defined($gccThread{$dead}) ) {
			&Log("RL: Internal error - completed gcc thread (pid $dead) not in gcc thread table, in file __FILE__ at  __LINE__");
		} else {
			($threadIndex, $threadDir)=split(' ',$gccThread{$dead});
			if ( $? ) {
				$errors++;
				&Log("RL: Load $load, instance $loadNumber, thread $threadIndex in dir $threadDir (pid $dead) failed, exit status $?");
				# remove $dead from %gccThread
				undef($gccThread{$dead});
			} else {
				&Log("RL: Load $load, instance $loadNumber, thread $threadIndex in dir $threadDir (pid $dead) passed");
			}
		}
	}
	$errors;
}

sub gccExec {
	local($iter,$noBootstrap)=@_;

	&Sys("gnumake LANGUAGES=c") && 
		&Exit("Failure of \"gnumake LANGUAGES=c\" in iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	

	if ( ! $noBootstrap ) {
		&Sys("gnumake stage1") &&
			&Exit("Failure of \"gnumake stage1\" in iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	
		
		&Sys("gnumake CC=\"stage1/xgcc -Bstage1/\" CLAGS=\"-g -O\" LANGUAGES=\"c\"")
			&& &Exit("Failure of \"gnumake CC=stage1/xgcc -Bstage1/ CLAGS=-g -O LANGUAGES=c\" in iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	
		
		&Sys("gnumake stage2") && 
			&Exit("Failure of \"gnumake stage2\" in iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	
		
		&Sys("gnumake CC=\"stage2/xgcc -Bstage2/\" CLAGS=\"-g -O\" LANGUAGES=\"c\"")
			&& &Exit("Failure of \"gnumake CC=stage2/xgcc -Bstage2/ CLAGS=-g -O LANGUAGES=c\" in iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	
		
		&Sys("gnumake compare") && 
			&Exit("Failure of \"gnumake compare\" iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	
	}

	&Sys("gnumake clean") && &Exit("Failure of \"gnumake clean\" iteration $iter of Gcc instance $loadNumber, thread $threadIndex", __FILE__, __LINE__, $EXIT_LOAD_FAIL );	

}

1;

__END__
# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:


