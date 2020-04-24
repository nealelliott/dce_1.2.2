#!/afs/tr/proj/tools/bin/perl

#############################################################################
#
# Program to execute commands with tracing activated and to pull
# out the raw trace logs and massage them if necessary for
# input to the trace post processor.
#
#############################################################################

# Get program name and path
if (($o = rindex($0, "/")) != -1) {
    $progPath = substr($0, 0, $o);
    $0 = substr($0, $o+1);
} else {
    $progPath = ".";
}

require "$progPath/common.pl";

#Parse command line
if (!@ARGV) {
    &Usage;
}
undef($type);
undef($prefix);
undef($cmd);
undef($xtractonly);
while (@ARGV) {
    if ($ARGV[0] !~ /^-/) {
	&Usage("multple commands specified") if ($cmd);
	&Usage("<command> and -extractonly are mutually exclusive") 
	    if ($xtractonly);
	$cmd = shift;
	while (@ARGV && ($ARGV[0] !~ /^-/)) {
	    $cmd .= ' '.shift;
	}
    } else {
	if ($ARGV[0] =~ /^-type/) {
	    &Usage("-type specified multiple times")
		if ($type); #Does not return;
	    shift;
	    $type = shift;
	} elsif ($ARGV[0] =~ /^-prefix/) {
	    &Usage("-prefix specified multiple times") if ($prefix); 
	    shift;
	    $prefix = shift;	
	} elsif ($ARGV[0] =~ /^-extractonly/) {
	    &Usage("-extractonly specified multiple times") if ($xtractonly);
	    &Usage("<command> and -extractonly are mutually exclusive") 
		if ($cmd);
	    shift;
	    $xtractonly = 1;
	} else {
	    &Usage("Unknown switch \"$ARGV[0]\" specified");
	}
    }
}
if (!defined($cmd) && !defined($xtractonly)) {
    &Usage("One of <command> or -extractonly must be specified");
}

# Find out $targetMachine
$code = &common'DetermineSystemType(*targetMachine);
if ($code) {
    die("$0: System Type \"$targetMachine\" not supported\n");
}

#Handle defaults
if (!defined($type)) {
    $type = "icl";
}
if (!defined($prefix)) {
    $hostName = `hostname`;
    chop $hostName;
    
    @date = localtime(time);
    ($year, $month, $date, $hour, $min) = @date[5, 4, 3, 2, 1];
    $prefix = sprintf("/tmp/trout.%s.%02d_%d_%d.%d_%d", $hostName,
                      $month + 1, $date, $year, $hour, $min);
}

#Determine file name to store trace logs
$file = "${prefix}.${type}.rpt";

# Pull in tracing package specific module that implements the worker routines
if ($type eq "icl") {
    $includePackage = "ICLRoutines";
} else {
    #$traceDataStyle eq native    
    if ($targetMachine eq RIOS) {
	$includePackage = "RIOSRoutines";
    } else {
	die("$0: Native tracing package on system type \"$targetMachine\" not supported\n");
    }
}
require "$progPath/$includePackage.pl";

# Execute command with tracing activated
if (!defined($xtractonly)) {
    eval "\&${includePackage}'DoTrace(\$cmd)";
}

# Extract trace logs
eval "\&${includePackage}'ExtractRawTraceData(\$file)";


sub Usage {
    local($msg) = @_;
    local($usage) = "$0: { <command> | -extractonly } [-prefix <report-name-prefix>]  <[-type {native | icl}] ";
    local($msg2);
    if ($msg) {
	$msg2 = "$0: $msg \n";
    }
    $msg2 .= "$0: usage: $usage\n";
    die($msg2);
}


