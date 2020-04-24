# include in package main itself	
package main;

require "pctsBasic.pl";

# Define required subroutines - pctsParse, pctsValidate, pctsRun

sub pctsParse {
    local($loadName, $loadNumber, $start, $end) =@_;

	if ( $loadName ne "pcts" ) {
		&Exit("Load name specified ($loadName) should be pcts",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DIRS, MULTIPLE, REQUIRED);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DURATION, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_CONCURRENCY, SINGLE, OPTIONAL);

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_PCTS_TESTER, SINGLE, REQUIRED );

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_PCTS_TESTER_DCE_PASSWD, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_PCTS_LIBROOT, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_PCTS_CTSROOT, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_PCTS_TESTS, MULTIPLE, OPTIONAL );

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_PCTS_DEVICES, MULTIPLE, OPTIONAL );

	0;
}

# Also sets defaults 
# return zero if all ok
sub pctsValidate {
    local($loadName, $loadNumber)=@_;
    local($errorCount)=0;
    local($quotaSpec);
    local($nthreads) = 0;

	if ( $loadName ne "pcts" ) {
		&Exit("Load name specified ($loadName) should be pcts",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}
	
    # Check if required specifications have been specified.
    if ( !defined($pctsSpec{MIN_DIR_QUOTA}) ) {
		&Log("Required specification - MIN_DIR_QUOTA not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

    # Handle required attributes
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DIRS}) ) {
		&Log("Dirs for load $loadName instance $loadNumber not specified");
		$errorCount++ 
	}

    if ( !defined($loadAttr{$loadName, $loadNumber, LS_PCTS_TESTER}) ) {
		&Log("Tester identity for load $loadName instance $loadNumber not specified");
		$errorCount++ 
    }

	# Handle optional specs and defaults and determine validity of given attributes
    # DURATION defaults to forever.
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DURATION}) ) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

#    if ( !defined($loadAttr{$loadName, $loadNumber, LS_PCTS_TESTER_DCEPASSWD}) ) {
#
#    }

	$pctsLib=$loadAttr{$loadName, $loadNumber, LS_PCTS_LIBROOT};
    if ( !defined($pctsLib) ) {
		if ( ! -d $pctsSpec{DEF_PCTS_LIBROOT} ) {
			&Log("PCTS lib root dir not specified, default ( $pctsSpec{DEF_PCTS_LIBROOT} ) non-existent");
			$errorCount++;
		}
		$pctsLib=$pctsSpec{DEF_PCTS_LIBROOT};
	} elsif ( ! -d $pctsLib ) {
		# The lib dir may have been specified as shared:/cts. Try to mount this 
		# at /cts.
		if ( $pctsLib =~ /([^:]*):(.*)/ ) {
			$nfsServer=$1;
			$dir=$2;
			if ( $dir !~ m#^/# ) {
				$dir = "/" . $dir;
			}
			&Sys("mount ${nfsServer}:${dir} $dir") &&
				&Exit("Unable to NFS mount pcts LIB dir ${nfsServer}:${dir} on ${dir}");
		}
		if ( ! -d $pctsLib ) {
			&Log("Specified PCTS lib root dir $pctsLib non-existent");
			$errorCount++;
		}
	}
	if ( ! -d "${pctsLib}/LIB" ) {
		&Log("Specified/Default PCTS lib root dir $pctsLib does not contain needed LIB dir");
	}

	$pctsCtsroot=$loadAttr{$loadName, $loadNumber, LS_PCTS_CTSROOT};
    if ( !defined($pctsCtsroot) ) {
		if ( ! -d $pctsSpec{DEF_PCTS_CTSROOT} ) {
			&Log("PCTS root source  dir not specified, default ( $pctsSpec{DEF_PCTS_CTSROOT} ) non-existent");
			$errorCount++;
		}
	} elsif ( ! -d $pctsCtsroot ) {
		&Log("Specified PCTS root source dir $pctsCtsroot non-existent");
		$errorCount++;
	}

	local(*pctsDevices)=$loadAttr{$loadName, $loadNumber, LS_PCTS_DEVICES};
    if ( !defined(@pctsDevices) ) {
		$dev1=$pctsSpec{DEF_PCTS_DEVICE1};
		$dev2=$pctsSpec{DEF_PCTS_DEVICE2};
		if ( ! -b $dev1 || ! -b $dev2) {
			&Log("PCTS scratch devices not specified, default $dev1 and/or $dev2 non-existent");
			$errorCount++;
		}
		push(@pctsDevices,$dev1,$dev2);
	} else {
		foreach (@pctsDevices) {
			$dev=$_;
			$_ = "/dev/" . $_  if ( $_ !~ m#^/# );
			if ($_ !~ m#^/dev#) {
				&Log("PCTS scratch device incorrectly specified ( $dev )");
				$errorCount++;
			} else {
				if ( ! -b $_ ) {
					&Log("Specified PCTS scratch device ($dev) non-existent");
					$errorCount++;
				}
			}
		}
	}

    # Check min. number of dirs has been specified
    local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
    if (@dirs < $pctsSpec{MIN_DIRS}) {
		&Log("Load $loadName number $loadNumber - fails MIN_DIR ($pctsSpec{MIN_DIR}) spec");
		$errorCount++;
    }
	
    # Check quota in each dir and determine number of threads that can be run
	# in the dirs specified
    $quotaSpec=$pctsSpec{MIN_DIR_QUOTA};
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
		$desiredNumDevices= 2 * $desiredNThreads;
	} else {
		$desiredNumDevices= 2 * $nthreads;
	} 
	$numDevices=@pctsDevices;
	if ( $numDevices < $desiredNumDevices ) {
		&Log("Load $loadName instance $loadNumber - insufficient number, $numDevices  scratch devices avaliable, need $desiredNumDevices");
		$errorCount++;
	}

	# Also chmod all test dirs to 777;
	&Log("Load $loadName instance $loadNumber - chmod all test dirs to 777");
	foreach ( @dirs ) {
		&Chmod($_, 0777);
	}

	# Handle LS_PCTS_TESTS
	local(@pctsTests,*pctsTests);
	if (!defined($loadAttr{$loadName, $loadNumber, LS_PCTS_TESTS})) {
        # PCTS chapters to be run are
		#  pcts/1003.1/05/
		#  pcts/1003.1/06/
		#  pcts/1003.1/08/01/09/
		#  pcts/1003.1/08/02/
		@pctsTests=("pcts/1003.1/05/",
					"pcts/1003.1/06/",
					"pcts/1003.1/08/01/09/",
					"pcts/1003.1/08/02/");
	} else {
		*pctsTests=$loadAttr{$loadName, $loadNumber, LS_PCTS_TESTS};
	}
	foreach (@pctsTests) {
		if ( ! m#^pcts/1003.1# ) {
			&Log("Specified PCTS test $_ is invalid");
			$errorCount++;
		}
	}

	if ( !$errorCount ) {
		$restrictFile="${LOGDIR}/pcts.i${loadNumber}.testList";		
		&WriteArrayToFile($restrictFile,"\n",@pctsTests);
	}

	$errorCount;
}

sub pctsRun {
	local($loadName, $loadNumber)=@_;
	local($tester, $passwd);
	local(@localDirs, @dfsDirs);
	local($dirQuota);

	if ( $loadName ne "pcts" ) {
		&Exit("Load name specified ($loadName) should be pcts",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

	# Seperate Local Dirs and DFS dirs
	local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
    foreach (@dirs) {
		if ( &IsDfsDir($_) ) {
			push(@dfsDirs, $_);
		} else {
			push(@localDirs, $_);
		}
    }

    # Create 2 config files - 1 for local dirs and 1 for DFS files.
	if (@localDirs) {
		$localFile="${LOGDIR}/pcts.i${loadNumber}.localspec";
		open(LOCAL, ">$localFile") || &Exit("Unable to create $localFile ($!)");
		select( (select(LOCAL), $|=1 )[0]);		
		print(LOCAL "LS_FSTYPE local\n");
		&WriteLoadSpec(LOCAL);
		print(LOCAL "LS_DIRS");
		foreach (@localDirs) {
			$dirQuota=&DirToQuota($_);
			print(LOCAL "\t $_ $dirQuota\n");
		}
		close($LOCAL);
		&Chmod($localFile,0444);
	} elsif (@dfsDirs) {
		$dfsFile="${LOGDIR}/pcts.i${loadNumber}.dfsspec";
		open(DFS, ">$dfsFile") || &Exit("Unable to create $dfsFile ($!)");
		print(DFS "LS_FSTYPE dfs\n");
		&WriteLoadSpec(DFS);	
		foreach (@dfsDirs) {
			$dirQuota=&DirToQuota($_);
			print(DFS "\t $_ $dirQuota\n");
		}
		close(DFS);
		&Chmod($dfsFile,0444);
	}

    $tester=$loadAttr{$loadName, $loadNumber, LS_PCTS_TESTER};
	$passwd=$loadAttr{$loadName, $loadNumber, LS_PCTS_TESTER_DCEPASSWD};
	
	chop($pwd = `pwd`);
	if ( @localDirs || @dfsDirs ) {
		if (@localDirs) {
			$logFile="${LOGDIR}/pcts.i${loadNumber}.local";
			if ( ($localPid = fork()) ) {
				&Log("Started pcts, instance $loadNumber on all local dirs, (pid = $localPid), log in $logFile");
			} else {
				&SetLogFile($logFile,0);
				&Log("Started pcts, instance $loadNumber on all local dirs: ");
				&Log(sprintf("%s",&AryToStr(@localDirs)));
				&Sys("su $tester -f -c \"${pwd}/pctsRun.pl $localFile\"") &&
					&Exit("Failed pcts, instance $loadNumber on local dirs ($?)",
						  __FILE__, __LINE__, $EXIT_LOAD_FAIL);
				exit($EXIT_SUCCESS);
			}
		}
		if (@dfsDirs) {
			$logFile="${LOGDIR}/pcts.i${loadNumber}.local";
			if ( ($dfsPid = fork()) ) {
				&Log("Started pcts, instance $loadNumber on all dfs dirs, (pid = $dfsPid), log in $logFile");
			} else {
				&SetLogFile($logFile,0);
				&Log("Started pcts, instance $loadNumber on all dfs dirs");
				&Log(sprintf("%s",&AryToStr(@dfsDirs)));
				&Sys("dce_login $tester $passwd -exec \"${pwd}/pctsRun $dfsFile\"")
					&& &Exit("Failed pcts, instance $loadNumber on dfs dirs",
							 __FILE__, __LINE__, $EXIT_LOAD_FAIL);
				exit($EXIT_SUCCESS);
			}
		}
		&Log("RL: Waiting for pcts to complete");
		while ( ($dead = wait) != -1 ) {
			if ($dead == $localPid) {
				&Log("RL: pcts, instance $loadNumber on all local dirs completed");
			} else {
				&Log("RL: pcts, instance $loadNumber on all dfs dirs completed");
			}
		}
	}
	0;
}

sub WriteLoadSpec {
	local($fh)=@_;
	
	print($fh "LS_NAME $loadName\n");
	print($fh "LS_INSTANCE $loadNumber\n");

	print($fh "LS_LOGDIR $LOGDIR\n");
	if ( defined($loadAttr{$loadName, $loadNumber, LS_DURATION})) {
		print($fh
			  "LS_DURATION $loadAttr{$loadName, $loadNumber, LS_DURATION}\n");
	}
	if (defined($loadAttr{$loadName, $loadNumber, LS_CONCURRENCY})) {
		print($fh 
			  "LS_CONCURRENCY $loadAttr{$loadName, $loadNumber, LS_CONCURRENCY}\n");
	}
	if (defined($loadAttr{$loadName, $loadNumber, LS_PCTS_LIBROOT})) {
		print($fh 
			  "LS_PCTS_LIBROOT $loadAttr{$loadName, $loadNumber, LS_PCTS_LIBROOT}\n");
	}
	if (defined($loadAttr{$loadName, $loadNumber, LS_PCTS_CTSROOT})) {
		print($fh 
			  "LS_PCTS_CTSROOT $loadAttr{$loadName, $loadNumber, LS_PCTS_CTSROOT}\n");
	}

	print($fh "LS_PCTS_TESTLIST $restrictFile \n");

	# The following attr should always be defined  at this stage.
	local(*devices)=$loadAttr{$loadName, $loadNumber, LS_PCTS_DEVICES};
	print($fh "LS_PCTS_DEVICES");
	foreach (@devices) {
		print($fh "\t $_\n");
	}
}

1;

__END__
# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:


