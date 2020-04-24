package ICLRoutines;


$ICLOVERHEADTIME = 30; # microseconds

# ICL tracing format specific

#########################################################################
#
# Routines used to control tracing 
#
#########################################################################

sub DoTrace {
    local($cmd) = @_;
    system("dfstrace setset -set trace -inactive");
    printf("Tracing $cmd\n");
    system("dfstrace setset -set trace -active");
    system("$cmd");
    system("dfstrace setset -set trace -inactive");
}

#
# Routines used for to extract/generate the raw trace record file
# for input to trace postprocessor
#
#########################################################################

# Extract and massage the raw trace records into a format understood
# by the trace postprocessor.

sub ExtractRawTraceData {
    local($dataFile) = @_;

    printf("Extracting trace logs into $dataFile \n");
    system("dfstrace dump -log trace -file $dataFile -raw");
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
    while ($line !~ /^Contents of log .*:$/) {
	if ($line !~ /^\s*$/) {
	    $sumheader .= $line;
	}
	$line = <STDIN>;
    }
    $initheader = "";
    $realTime = 0;
    $firstEventSeen = 0;
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
# Variable $currentTime is a global simulating a static variable needed 
# by this routine.
#
sub ProcessEvent {
    local($line, *pname, *pid, *int, *realTime, *elapsedTime, *dfsEvent, *eventType, *funcID, *data) = @_;
    local($eventTime, $rem);
    local($eType);

    $pid = $int = $realTime = $elapsedTime = $funcID = 0;
    $dfsEvent = 0;
    $pname = $eventType = $data = "";

    if ($line =~
	/raw op (.*), time (\d*)\.(\d*), pid (\d*)/) {
	$event = hex($1);
#	$eventTime = $2 * 1000000 + $3 + $currentTime;
	$realTime = $2 * 1000000 + $3;
	$pname = $pid = $4;

	if (!$firstEventSeen) {
	    $firstEventSeen = 1;
	    $prevRealTime = $realTime;
	    $prevEventTime = 0;
	}
	$deltaTime = ($realTime - $prevRealTime - $ICLOVERHEADTIME);
	if ($deltaTime < 0) {
	    $deltaTime = 0;
	}
	$eventTime = $prevEventTime + $deltaTime;
	$prevEventTime = $eventTime;
	$prevRealTime = $realTime;

	$pname =~ s/\s//g;
	$pid =~ s/\s//g;
	$elapsedTime = int($eventTime);
	$dfsEvent = 1;

	$eType = ($event & 0xf0000000);
	$eType >>= 28;
	$funcID = ($event & 0x0fffffff);

	if ($eType == 4) {
	    $eventType = ENTRY	} elsif ($eType == 6) {
	    $eventType = EXIT;
	}
    } elsif ($line =~ /^current time event,.*p0:(\d*)/) {
	$currentTime = hex($1);
	if (!$firstEventSeen) {
	    $firstEventSeen = 1;
	    $prevRealTime = $currentTime;
	    $prevEventTime = 0;
	} else {
	    $delta = $currentTime - $prevRealTime - $ICLOVERHEADTIME;
	    $prevRealTime += $delta
	}
    } else {
	$data = $line;
    }
    return 0;
}

1;

