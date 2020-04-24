package main;

# ib = 8192 - 32 = 8160 / 4 = 2040 
$IB_NSLOTS = 2040;

# can't go beyond 2 ^ 31 - 1 
@offsets = 
    (0,
     1,
     2,
     95,
     96, 			# inline
     97,
     1 * 1024 - 1,		# frag 
     1 * 1024,			
     1 * 1024 + 1,		
     2 * 1024 - 1,
     2 * 1024,
     2 * 1024 + 1,
     4 * 1024 - 1,			
     4 * 1024,			# page 
     4 * 1024 + 1,			
     7 * 1024 - 1,		
     7 * 1024,			# block 
     7 * 1024 + 1,		
     8 * 1024 - 1,			
     8 * 1024,			
     8 * 1024 + 1,			
     8 * 8192 - 1,			
     8 * 8192,			# block - 1st indirect 
     8 * 8192 + 1,			
     (8+1) * 8192 - 1,		
     (8+1) * 8192,		
     (8+1) * 8192 + 1,		
     (8+1020) * 8192 - 1,
     (8+1020) * 8192,
     (8+1020) * 8192 + 1,
     (8+2039) * 8192 - 1,
     (8+2039) * 8192,
     (8+2039) * 8192 + 1,
     (8+2040) * 8192 - 1,		
     (8+2040) * 8192,		# block - 2nd indirect
     (8+2040) * 8192 + 1,		
     (8+2040+1) * 8192 - 1,
     (8+2040+1) * 8192,     
     (8+2040+1) * 8192 + 1,
     (8+2040+1020) * 8192 - 1,
     (8+2040+1020) * 8192,
     (8+2040+1020) * 8192 + 1,
     (8+2040+2039) * 8192 - 1,
     (8+2040+2039) * 8192,     
     (8+2040+2039) * 8192 + 1,
     (8+2040+2040) * 8192 - 1,
     (8+2040+2040) * 8192,
     (8+2040+2040) * 8192 + 1,
     (8+2040+64*2040) * 8192 - 1,
     (8+2040+64*2040) * 8192,
     (8+2040+64*2040) * 8192 + 1,
     (8+2040+127*2040) * 8192 - 1, 
     (8+2040+127*2040) * 8192, 
     (8+2040+127*2040) * 8192 + 1);

sub WriteOrTrun {
    local($func, $file, $fileh, $seqlen) = @_;
    local($off);

    foreach (1 .. $seqlen) {
	$off = $offsets[int(rand(@offsets))];
	if ($func eq "write") {
	    &Write($file, $fileh, $off);
	} else {
	    &Trun($file, $fileh, $off);
	}
    }
}	

%promoteSpec = (MIN_DIRS, 1,
		MIN_DIR_QUOTA, &SizeInKB("1M"),
		MIN_DURATION, 1);

sub promoteParse {
    local($loadName, $loadNumber, $start, $end) =@_; 
    
    if ( $loadName ne "promote" ) {
	&Exit("Load name specified ($loadName) should be promote",
	      __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
    
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DIRS, MULTIPLE, REQUIRED);
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DURATION, SINGLE, OPTIONAL );
    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DETERMINISTIC, FLAG, OPTIONAL );
    0;
}

sub promoteValidate {
    local($loadName, $loadNumber)=@_;
    local($dir);
    local($errorCount)=0;
    local($index);
    local($quotaSpec);
    local($nthreads) = 0;

    if ($loadName ne "promote") {
	&Exit("Load name specified ($loadName) should be promote",
	      __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }

    if (!defined($promoteSpec{MIN_DIR_QUOTA})) {
	&Log("Required specification - MIN_DIR_QUOTA not specified for load $loadName, instance $loadNumber");
	$errorCount++;
    }

    # Handle required attributes	
    if (!defined($loadAttr{$loadName, $loadNumber, LS_DIRS})) {
	&Log("Dirs for load $loadName instance $loadNumber not specified");
	$errorCount++; 
    }

    # DURATION defaults to forever.
    if (!defined($loadAttr{$loadName, $loadNumber, LS_DURATION})) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

    # Find out if specified attributes are valid.
    # Check min. number of dirs has been specified
    local(*dirs)=$loadAttr{$loadName, $loadNumber, LS_DIRS};
    if (@dirs < $prmoteSpec{MIN_DIRS}) {
	&Log("Load $loadName number $loadNumber - fails MIN_DIR ($promoteSpec{MIN_DIR}) spec");
	$errorCount++;
    }				# 
    $errorCount++;	
}

sub promoteRun {
    local($loadName, $loadNumber) = @_;
    local($threadLogFilePrefix);
    local($errors) = 0;
    local($dir);
    local($dead);
    
    if ($loadName ne "promote" ) {
	&Exit("Load name specified ($loadName) should be promote",
	      __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
    
    local(*dirs) = $loadAttr{$loadName, $loadNumber, LS_DIRS};

    $dirIndex = -1;
    foreach $dir (@dirs) {
	$dirIndex++;
	$threadFilePrefix = "${dir}/promote";
	$threadLogFilePrefix = "${LOGDIR}/promote.l${loadNumber}.d${dirIndex}";
	$fh = 'FH0';
	$file = "${threadFilePrefix}.c";
	&CreateFile($file, $fh);
	&Log("Starting promote competiting threads for instance $loadNumber");
	&StartThreads($file, $fh, 5, "${threadLogFilePrefix}.c");
	
	
	&Log("Starting promote individual threads for instance $loadNumber");
	foreach $i (0 .. 4) {
	    $fh = 'FH'.$i;
	    $file = "${threadFilePrefix}.i${i}";
	    &CreateFile($file, $fh);
	    &StartThreads($file, $fh, 1, "${threadLogFilePrefix}.i${i}");
	}
    }

    # Wait for all threads.
    &Log("RL: Waiting for all promote threads to complete");
    while (($dead = wait) != -1) { 
	&Log("Promote thread pid $dead completed");
    }				
    $errors;
}

sub StartThreads {
    local($file, $fileh, $count, $logPrefix) = @_;
    local($i, $threadPid, $threadLogFile);

    foreach $i (0 .. $count - 1) {
	$threadLogFile = "${logPrefix}.t${i}";
	if ($threadPid = fork()) {
	    &Log("RL: Forked load promote, thread $i (pid = $threadPid), log in $threadLogFile");
	} else {
	    &SetLogFile($threadLogFile, 0);
	    &Sleep(10);
	    &Log("THR: Load promote, pid $threadPid starting");
	    srand(time());
	    while (1) {
		$seqlen = int(rand(@offsets));
		if (rand(1) < 0.5) {
		    &WriteOrTrun("write", $file, $fileh, $seqlen);
		} else {
		    &WriteOrTrun("trun", $file, $fileh, $seqlen);
		}
	    }
	    &Log("THR: Load promote, thread $i completed");
	    exit($EXIT_SUCCESS);				
	}
    }
}

1;

