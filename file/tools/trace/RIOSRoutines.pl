package RIOSRoutines;

# RIOS native tracing package specific

#########################################################################
#
# Routines used to control tracing 
#
#########################################################################

sub DoTrace {
    local($cmd) = @_;
    system("trcstop");
    printf("Tracing $cmd\n");
    system("trace -f -d -a");
    system("trcon");
    system("$cmd");
    system("trcstop");
}

#########################################################################
#
# Routines used for to extract/generate the raw trace record file
# for input to trace postprocessor
#
#########################################################################

# Extract and massage the raw trace records into a format understood
# by the trace postprocessor.

sub ExtractRawTraceData {
    local($dataFile) = @_;

    local($formatFile) = "/tmp/trcfmt.dfs.$$";

    # Generate DFS specialized version of template format file used by AIX
    # trace package
    system("cp /etc/trcfmt $formatFile");
    open(FMTFILE, ">>$formatFile");
    printf(FMTFILE "0FE 1.0 \"DFS_ENTRY\" D2\n");
    printf(FMTFILE "0FF 1.0 \"DFS_EXIT\" D2\n");    
    close(FMTFILE);
    printf("Extracting trace logs into $dataFile\n");
    system("trcrpt -o $dataFile -t $formatFile -O exec=on,svc=on,timestamp=1,pid=on");
    unlink($formatFile);
}

#########################################################################
#
# Routines used by trace postprocessor for generating final report
#
#########################################################################

# Process the header lines in the raw trace records file (STDIN)
sub ProcessHeader {
    local(*sumheader, *initheader) = @_;
    local($line);
    $line = <STDIN>;
    while ($. <= 8) {
	if ($line !~ /^\s*$/) {
	    $sumheader .= $line;
	}
	$line = <STDIN>;
    }

    while ($line !~ /^ID  PROCESS NAME   PID/) {
	$initheader .= $line;
	$line = <STDIN>;
    }
}

# Process a raw trace event.
#
# NOTES:
# If $dfsEvent is set to 1
#    $eventType should have one of ENTER or EXIT and
#    $funcID should have the numeric identifier of the function
#    $data should have a NULL value
# else 
#    $data should have the remainder of the line
#    $eventType = ""
#    $funcID = 0
#   
sub ProcessEvent {
    local($line, *pname, *pid, *int, *realTime, *elapsedTime, *dfsEvent, *eventType, *funcID, *data) = @_;
    local($eventTime, $rem);

    $pid = $int = $realTime = $elapsedTime = $funcID = 0;
    $dfsEvent = 0;
    $pname = $eventType = $data = "";

    if ($line =~
	#  4d        14d            9d                   14d           10d                     
	/^....(..............).(.........)([0-9 ]) .............. (..........)(.*)$/) {

	($pname, $pid, $int, $eventTime, $rem) = ($1, $2, $3, $4, $5);
	$pname =~ s/\s//g;
	$pname =~ s/^-(\d*)-$/\1/;
	$pid =~ s/\s//g;
	$eventTime =~s/\s//g;
	$eventTime *= 1000000;
	$realTime = $elapsedTime = int($eventTime);

	# Process a DFS trace event
	if ($rem =~ /DFS_(ENTRY|EXIT)\s*(\d+)$/) {
	    $dfsEvent = 1;
	    $eventType = $1;
	    $funcID = $2;
	} else {
	    $data = $rem;
	}
    } else {
	$data = $line;
    }
    return 0;
}

1;
