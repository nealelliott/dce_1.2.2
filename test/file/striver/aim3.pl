# include in package main itself	
package main;

#
# Load specifications
# For aim3, CONCURRENCY is not useful
# DURATION becomes number of iterations
%aim3Spec = ( MIN_DIRS,		    1,
			  MIN_DURATION,		1,
			  MIN_INTENSITY,	1,
			  DEF_INTENSITY,	1,
			  MIN_DIR_QUOTA_PER_USER, "3M",
			  MIN_INITIAL_NUSERS, 1,
			  DEF_INITIAL_NUSERS, 1,
              DEF_FINAL_NUSERS,   100,
			  DEF_INCR_NUSERS,    10
			  );

# Canonalize aim3 load specs
$aim3Spec{MIN_DIR_QUOTA_PER_USER}=&SizeInKB($aim3Spec{MIN_DIR_QUOTA_PER_USER});

# Define required subroutines - aim3Parse, aim3Validate, aim3Run

sub aim3Parse {
    local($loadName, $loadNumber, $start, $end) =@_;

	if ( $loadName ne "aim3" ) {
		&Exit("Load name specified ($loadName) should be aim3",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DIRS, MULTIPLE, REQUIRED);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DURATION, SINGLE, OPTIONAL );

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_AIM3_SRCDIR, SINGLE, REQUIRED );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_AIM3_INITIAL_NUSERS, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_AIM3_FINAL_NUSERS, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_AIM3_INCR_NUSERS, SINGLE, OPTIONAL );

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_AIM3_NO_VM_TEST, FLAG, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_AIM3_NO_ADAPTIVE_TIMER, FLAG, OPTIONAL );
	
	0;
}

# Also sets defaults 
# return zero if all ok
sub aim3Validate {
    local($loadName, $loadNumber)=@_;
    local($dir);
    local($errorCount)=0;
    local($index);
    local($quotaSpec);
    local($nUsers) = 0;

	if ( $loadName ne "aim3" ) {
		&Exit("Load name specified ($loadName) should be aim3",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}
	
    # Check if required specifications have been specified.
	if ( !defined($aim3Spec{MIN_DIRS}) ) {
		&Log("Required minimum value for MIN_DIRSnot specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

	if ( !defined($aim3Spec{DEF_INTENSITY}) ) {
		&Log("Required default value for DEF_INTENSITY not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

	if ( !defined($aim3Spec{DEF_INITIAL_NUSERS}) ) {
		&Log("Required default value for DEF_INITIAL_NUSERS not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}
	if ( !defined($aim3Spec{DEF_INCR_NUSERS}) ) {
		&Log("Required default value for DEF_INCR_NUSERS not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

    if ( !defined($aim3Spec{MIN_DIR_QUOTA_PER_USER}) ) {
		&Log("Required specification - MIN_DIR_QUOTA_PER_USER not specified for load $loadName, instance $loadNumber");
		$errorCount++ 
	}

    # Handle required attributes
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DIRS}) ) {
		&Log("Dirs for load $loadName instance $loadNumber not specified");
		$errorCount++ 
	}

	# Set intensity to default value always for now. At some point
	# when multiple intensity levels are used, uncomment out the #
	# in the following lines  for INTENSITY handling.
#	if ( !defined($loadAttr{$loadName, $loadNumber, LS_INTENSITY}) ) {
		$loadAttr{$loadName, $loadNumber, LS_INTENSITY} =
			$aim3Spec{DEF_INTENSITY};
#    } 
	
    # DURATION defaults to forever.
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DURATION}) ) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

    # Find out if specified attributes are valid.

    # Check min. number of dirs has been specified
    local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};

	if ( @dirs <  $aim3Spec{MIN_DIRS} ) {
        &Log("Load $loadName number $loadNumber - fails MIN_DIR ($aim3Spec{MIN_DIR}) spec");
		$errorCount++;
    }
	
    # Check quota in each dir and determine number of threads that can be run
	# in the dirs specified
    $quotaSpec=$aim3Spec{MIN_DIR_QUOTA_PER_USER};
    foreach (@dirs) {
		$dirQuota=&DirToQuota($_);
		if ( $dirQuota >= $quotaSpec ) {
			$nUsers += int($dirQuota/$quotaSpec);
		}
    }
	
    #Check sufficient space allocated to tests
   	# If LS_AIM3_FINAL_NUSERS not specified, use as many users as possible (at
    # least 1) else as many as specified. 
	if ( !$nUsers ) {
		&Log("Load $loadName number $loadNumber - insufficient dir space for even 1 user");
		$errorCount++;
	} elsif ( $loadAttr{$loadName, $loadNumber, LS_AIM3_FINAL_NUSERS} ) {
		if (defined($loadAttr{$loadName, $loadNumber,LS_AIM3_INITIAL_NUSERS})){
			local($initialNusers) =
				$loadAttr{$loadName, $loadNumber, LS_AIM3_INITIAL_NUSERS};
			local($desiredNUsers) =
				$loadAttr{$loadName, $loadNumber, LS_AIM3_FINAL_NUSERS};
			if ($desiredNUsers < $initialNusers) {
				&Log("Config file error -  final no. of users ($desiredNUsers) less that initial number of users ($initialNUsers)");
				$errorCount++;
			} 
		} else {
			if ( $nUsers < $desiredNUsers ) {
				&Log("Load $loadName number $loadNumber - insufficient dir space,  desired $desiredNUsers users, (space for max. $nUsers users)");
				$errorCount++;
			} 
		}
	} 
    $errorCount;
}

sub aim3Run {
	local($loadName, $loadNumber)=@_;
	local($index);
	local($threadRepCnt, $threadDir, $pwd);
	local($quotaSpec, $dirQuota);
	local($aim3BinSrc, $aim3ExecCmd);
	local($dir, $dirsDone);
	local($nUsers,$initialNUsers,$incrNUsers,$finalNUsers);

	if ( $loadName ne "aim3" ) {
		&Exit("Load name specified ($loadName) should be aim3",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

	local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
	# Determine number of users that can be run in the dirs specified XXX
    $quotaSpec=$aim3Spec{MIN_DIR_QUOTA_PER_USER};
    foreach (@dirs) {
		$dirQuota=&DirToQuota($_);
		if ( $dirQuota >= $quotaSpec ) {
			$nUsers += int($dirQuota/$quotaSpec);
		}
    }

	# if CONCURRENCY specified, run only that many
	$initialNUsers=$loadAttr{$loadName, $loadNumber, LS_AIM3_INITIAL_NUSERS};
	$initialNUsers || ($initialNUsers = $aim3Spec{DEF_INITIAL_NUSERS});
	$incrNUsers=$loadAttr{$loadName, $loadNumber, LS_AIM3_INCR_NUSERS};
	$incrNUsers || ($incrNUsers = $aim3Spec{DEF_INCR_NUSERS});
	$finalNUsers=$loadAttr{$loadName, $loadNumber, LS_AIM3_FINAL_NUSERS};
	$finalNUsers || ( $finalNUsers = $nUsers );

	$threadRepCnt=$loadAttr{$loadName, $loadNumber, LS_DURATION};

	&Log("RL: Load aim3, instance $loadNumber, no. of users: initial $initialNUsers, final $finalNUsers, increment $incrNUsers");
	if ( $threadRepCnt ) {
		&Log("RL: Load aim3, instance $loadNumber, no. of iterations: $threadRepCnt");
	} else {
		&Log("RL: Load aim3, instance $loadNumber, no. of iterations: forever");
	}

	$aim3BinSrc=$loadAttr{$loadName, $loadNumber, LS_AIM3_SRCDIR};
	$aim3ExecCmd="./multiuser";
	if ( defined($loadAttr{$loadName, $loadNumber, LS_AIM3_NO_ADAPTIVE_TIMER})) {
		$aim3ExecCmd .= " -t";
		&Log("RL: Load aim3, instance $loadNumber, non adaptive timer mode ");
	}

	# Update 
	$dir=$dirs[0];
	$threadDir="$dir/aim3.i${loadNumber}";
	&MakeDir($threadDir,0777);
	&Sleep(10);
	&Chdir($threadDir);
	chop($pwd=`pwd`);
	&Log("THR: Load aim3, instance $loadNumber starting in $pwd");
    if ( &Sys("${aim3BinSrc}/build") ) {
		&Exit("Failed to install AIM III");
	}
	
	# Edit mixb to have 100 stress.
	if ( -e "mixb" ) {
		&Rename("mixb", "mixb.orig");
	}
	open(MFH, ">mixb") ||
		&Exit("Load aim3, instance $loadNumber, could not create ${threadDir}/mixb file");
	print MFH "100 disk\n";
	close (MFH);
	&Sys("./blend") && 
		&Exit("THR: Load aim3, instance $loadNumber, ${threadDir}/blend failed");

	
	# Handle LS_AIM3_NO_VM_TEST flag and multiple directories
	if ( ! -e "config" ) {
		&Exit("Load aim3, instance $loadNumber, could not find ${threadDir}/config file");
	}
	&Rename("config", "config.orig");
	open(OLDCONFIG,"config.orig") ||
		&Exit("Load aim3, instance $loadNumber, could not read ${threadDir}/config.orig file ($!)");
	open(CONFIG,">config") ||
		&Exit("Load aim3, instance $loadNumber, could not create ${threadDir}/config file ($!)");		
	
	while ( <OLDCONFIG> ) {
		if (/^6/) {
            # Handle virtual memory
			if ( $loadAttr{$loadName,$loadNumber,LS_AIM3_NO_VM_TEST} ) {
 				print(CONFIG "6 \"OFF\" \n");
            } else { 
				print(CONFIG "6 \"ON\" \n");
			}
		} elsif (/^3/) {
			if ( !$dirsDone ) {
                # Handle multiple test dirs
				for ($index = $[; $index <= $#dirs; $index++ ) {
					printf(CONFIG "3 \"%s\" \n", $dirs[$index]);
				}
			    $dirsDone = 1;
			}
		} else {
			print CONFIG "$_";
		}
	}
	close(OLDCONFIG);
	close(CONFIG);

    # AIM3 multiuser program relies on sending SIGINT to all the members of 
    # its process group for synchronization. But is does not create 
    # its own process group, if SIGINT is not already being ignored
    # when its invoked. This seems to be bogus, but as per discussion with
    # Ted, modify this driver to work around the problem instead
    # of modifying the multiuser program.
    # Hence explicitly ignore SIGINT.;

	$SIG{'INT'}='IGNORE';
	
	if ($threadRepCnt) {
		$backupPathPrefix="${LOGDIR}/aim3.i${loadNumber}";
		&Log("THR: Starting $threadRepCnt iterations of aim3 instance $loadNumber");
		&Log("Copies of AIM III spec/result files will be stored with path prefix - $backupPathPrefix");
		$result = &aim3RunIters($aim3ExecCmd, $threadRepCnt,
								$initialNUsers, $incrNUsers, $finalNUsers);
		&backupAim3Files($backupPathPrefix);
		if ( $result ) {
			&Exit("THR: Failure of aim3 instance $loadNumber");
		} else {
			&Log("THR: Success of aim3 instance $loadNumber");
		}
	} else {
		local($round);
		local($roundNIter)=100;
		for ($round=0; ; $round++) {
			$backupPathPrefix="${LOGDIR}/aim3.i${loadNumber}.r${round}";
			&Log("THR: Starting round $round of $roundNIter iterations of aim3 instance $loadNumber");
			&Log("Copies of AIM III spec/result files will be stored with path prefix - $backupPathPrefix");
			$result = &aim3RunIters($aim3ExecCmd, $roundNIter,
									$initialNUsers, $incrNUsers, $finalNUsers);

			&backupAim3Files($backupPathPrefix);
			if ( $result ) {
				&Exit("THR: Failure of round $round of $roundNIter iterations of aim3 instance $loadNumber");
			} else {
				&Log("THR: Success of round $round $roundNIter iterations of aim3 instance $loadNumber");
			}
		}
	}
	&Log("THR: Load aim3, instance $loadNumber completed");		
	
	0;
}

sub backupAim3Files {
	local($pathPrefix,$srcDir)=@_;
	if ( defined($srcDir) ) {
		if ( rindex($srcDir,"/") != (length($srcDir) - 1) ) {
			$srcDir .= "/";
		}
	} else { 
		$srcDir= "./";
	}
	&CopyFile("${srcDir}/config", "${pathPrefix}.config");
	&CopyFile("${srcDir}/mixb", "${pathPrefix}.mixb");
	&CopyFile("${srcDir}/toutput", "${pathPrefix}.toutput");
	&CopyFile("${srcDir}/suite3.ss", "${pathPrefix}.suite3.ss");
	&CopyFile("${srcDir}/results", "${pathPrefix}.results");
}

sub aim3RunIters {
	local($execFile, $nIters,$iNUsers, $incrNUsers, $fNUsers) = @_;
	local($hostName);

	chop($hostName=`hostname`);
	open(MULTIUSER, "| $execFile");
	print(MULTIUSER "$hostName\n");
	print(MULTIUSER "$nIters\n");	
	foreach (0..$nIters-1) {
		print(MULTIUSER "$iNUsers\n");
		print(MULTIUSER "$fNUsers\n");
		print(MULTIUSER "$incrNUsers\n");
	}
	close(MULTIUSER);
	$?;
}

1;

__END__
# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:

