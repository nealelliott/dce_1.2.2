#!/afs/tr/proj/tools/bin/perl

# Globals
# %symbols      key    function id 
#               value  function name
# %files        key    function id 
#               value  file name
# %indents      key    process id
#               value  current function level
#                      - initial value 0, 
#                         on first traced DFS function, value is 1.
# %etime        keys   (process id, function level) 
#               value  time at which the process entered the specified level
# %lfunc        keys   (process id, function level)
#               value  function corresponding to the specified level for the
#                      specified process
# %ncalls       key    function name
#               value  function invocation count
# %pnfuncs      key    function name 
#               value  number of functions for which the specified function
#                      is the parent
# %pncalls      key    (parent function, child function)
#               value  cumulative number of time the child function has been 
#                      invoked from the specified parent
# %seq
# %sequence
# %seqix
#
# XXX
# %abstime      key    function name
#               value  absolute time spent in this function only. Does not 
#                      include time spent in called functions.
#
# %cumtime      key    function name
#               value  cumulative time spent in function including any
#                      called functions.
# %pcumtime     key    (parent function, child function)
#               value  cumulative time spent for in the child for a particular
#                      parent child combination. Includes time spent in
#                      called functions from the child.
#
# $prevtime     time of immediately preceeding event
# $time         time of current event

if (($o = rindex($0, "/")) != -1) {
    $progPath = substr($0, 0, $o);
    $0 = substr($0, $o+1);
} else {
    $progPath = ".";
}

require "$progPath/common.pl";

# Find out $targetMachine
$code = &DetermineSystemType(*targetMachine);
if ($code) {
    die("$0: System Type \"$targetMachine\" not supported\n");
}

$trp = 1;       # Default is to print extended rpt listing.
if ($#ARGV >= 0 && $ARGV[0] eq "-s") {$trp = 0;shift;}

$eventFile = $ARGV[0]; shift;
if ($eventFile !~ /^(.*).(native|icl).rpt$/) {
    &Usage;
# Never returns here
}

$basename = $1;

$traceDataStyle = $2;

if ($traceDataStyle eq icl) {
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

open(STDIN, "<$eventFile") || die("$0: can't read \"$eventFile\" ($!)\n");

if ($ARGV[0] eq "-sb") {
    $sb = $ARGV[1]; shift; shift;
    if (! $sb) {
	&Usage;
	# Never returns
    }
    if (! -r $sb) {
	die("$0: can't read \"$sb\", specified value for option -sb ($!)\n");
    }
    $symbolsDir = "${sb}/src/TRACE/${targetMachine}/SYMBOLS";
} else {
    $symbolsDir = "./SYMBOLS";
}

if (! -d $symbolsDir) {
    die("$0: can't read \"$symbolsDir\", function id to name mapping database dir ($!)\n");
}

# Build function id to function name and containing file mappings, 
# by reading symbol files. Assumes that a SYMBOLS is a link to the symbol file
# directory (which is normally in
# ${sandbox}/src/TRACE/${TARGET_MACHINE}/SYMBOLS in your project dir).
while ($file = <$symbolsDir/*.i>) {
    open (SYMFILE, $file);
    $file =~ s/${symbolsDir}\/(.*).icl.i/\1/o;
    while (<SYMFILE>) {
	chop; split;
	$symbols{$_[0]} = $_[1];
	$files{$_[0]} = "$file.c";
    }
}

# Support for varying the width of the time fields in the report.
$tw = 5;                # basic width
$tb = ' ' x $tw;        # blanks
$td = '-' x $tw;        # dashes
$tf = "%${tw}s";        # Format
sub tn { # Format number
    local($fn) = pop(@_);
    local($s, $ms);
    if (length($fn) > $tw) {
	$s = $fn / 1000000;
	$ms = $fn % 1000000;
	$fn = sprintf("%6.2f", $fn/1000000);
    }
    $fn;
}

open(CONSOLE, ">&STDOUT");

# Write extended trace report to file.rpx.
open(STDOUT, ">$basename.rpx") || die "rpx: can't write \"$basename.rpx\"\n";
chmod(0444, "$basename.rpx") || die "rpx: can't chmod 0444 \"$basename.rpx\"\n";

if ($trp) {
    print("
*************************************************************************************************************
*************************************************************************************************************
**                                                                                                         **
**                                                                                                         **
**                                  FUNCTION EXECUTION TIME REPORT                                         **
**                                                                                                         **
** For times in format xxx.yy, xxx are in seconds and yy are in microseconds.                              **
** All other times are in microseconds.                                                                    **
*************************************************************************************************************
*************************************************************************************************************
\n");}

# Scan report file.
$prevtime = $time = 0;

eval "\&${includePackage}'ProcessHeader(\*sumhead, \*inithead)";

print $sumhead;
print $inithead;
printf("REAL TIME represents actual time\n");
printf("TIME represents real time adjusted by any tracing package overhead\n");
printf("\n%1s %-10s %10s %10s %10s\n\n", "I", "PROCESS", "REAL TIME", "TIME", "EVENT");

$firstDFSEvent = 1;
scan: while (<STDIN>) {
    $event = $_;
    if ($event =~ /^\s*$/) {
	next scan;
    }
    # time in microseconds

    $code = eval "\&${includePackage}'ProcessEvent(\$_, \*pname, \*pid, \*int, \*rtime, \*time, \*dfsEvent, \*type, \*procid, \*data)";

    if ($code) {
	printf("Malformed event record at line $., \" $event \"\n");
	next scan;
    }

#       # Maintain a per-process clock (removing time for interrupts, 
#       # context switches, etc.)
#       # This code can be deleted to deliver real-time instead (just set 
#       # $clock to $time).
#       if ($time) {
#           $clock = ($clock{$pid,$int} += ($time - $lasttime));
#           $lasttime = $time;
#       }
#       $time = $clock;
    
    # Breakout DFS tracepoint information.
    undef($file);
    if ($dfsEvent) {
	$file = "$files{$procid}";
    }
    
    # Format common part of line (common to AIX and AFS tracing).
#    if ($trp && ($pid || $pname || $sys)) {
#	$idstring = sprintf("         CONTEXT: process id: $pid, process name: \"$pname\", syscall:  \"$sys\"\n");
#	print ($lastidstring = $idstring) if ($lastidstring ne $idstring);
#    }
    printf("%1s %10s %10s %10s ", $int, $pname, $rtime, $time) if $trp;
    
    # Format DFS tracepoints.
    if ($dfsEvent) {
	if ($firstDFSEvent) {
	    $delta = 0;
	    $firstDFSEvent = 0;
	} else {
	    $delta = $time - $prevtime;
	}
	if ($type eq ENTRY) {
	    $level = 0 + $indents{$pid}++;
	    # called function
	    $func = $lfunc{$pid, $level} = $symbols{$procid};
	    if (length($func) == 0) {
		printf(CONSOLE "func id %d has no mapping at line %d\n",
		       $procid, $.);
		exit(1);
	    }
            # calling function
	    $parent = $lfunc{$pid, $level - 1};
	    $etime{$pid, $level} = $time;
	    $ncalls{$func}++; 
	    $abstime{$parent} += $delta; 
	    $pnfuncs{$parent}++ unless $pncalls{"$parent.$func"}++;
	    $seqix{$pid, $level} = length($seq{$pid});
	    $seq{$pid} .= "($func";
	    $trp && print ("${tb} |" x $level);
	    $trp && printf("${tf} |${td}-", &tn($delta));
	} else {
	    # $type eq EXIT
	    $level = --$indents{$pid};
	    if ($level < 0) {
		$level = $indents{$pid} = 0;
		$etime{$pid,$level} = 0;
		$func = $symbols{$procid};
		$functime = 0;
		$delta = 0;
	    } else {
		$func = $lfunc{$pid,$level};
		if ($func ne $symbols{$procid}) {
		    die;
		}
                # XXX Ignore everthing if level is < 0 ie. do not count function 
		# times etc.
		$seq{$pid} .= ")";
		$sequence{substr($seq{$pid}, $seqix{$pid,$level})}++;
		$parent = $lfunc{$pid,$level - 1};
		$functime = $time - $etime{$pid,$level};
		$abstime{$func} += $delta;
		$cumtime{$func} += $functime;
		$pcumtime{"$parent.$func"} += $functime;	    
	    }
	    $trp && print ("${tb} |" x $level);
	    $trp && printf("${tf}+|${tf} ", &tn($functime), &tn($delta));
	}
	$trp && print $func;
	$prevtime = $time;
    } else {
	$data =~ s/\s+/ /g;
	$data =~ s/^\s//;
	$trp && print $data;
    }
    $trp && print "\n";
}

# List common subsequences of code, in file.rpc.
open(STDOUT, ">$basename.rpc") || die "rpc: can't write \"$basename.rpc\"\n";
chmod(0444, "$basename.rpc") || die "rpx: can't chmod 0444 \"$basename.rpc\"\n";
#open(STDOUT, STDERR);
$level = $nextline = 0;
$indents[$level] = 0;
foreach $seq (sort keys(%sequence)) {
    @tokens = split(/(\(|\))/, $seq);
    next if ($#tokens <= 3);
    next if ($count = $sequence{$seq}) < 2;
    print("Sequence repeated $count times:\n    ");
    $nextline = 0;
    while ($#tokens >= 0) {
	next unless $token = shift(@tokens);
	if ($token eq "(") {
	    if ($nextline) {
		print("\n    ", ' ' x $indents[$level]);
		$nextline = 0;
	    }
	} elsif ($token eq ")") {
	    printf ")";
	    $nextline = 1;
	    $level--;
	} else {
	    print "(", $token;
	    $level++;
	    $indents[$level] = $indents[$level - 1] + length($token) + 1;
	}
    }
    printf("\n\n");
}               

# Write summary data to file.rps.
open(STDOUT, ">$basename.rps") || die "rpx: can't write \"$basename.rps\"\n";
chmod(0444, "$basename.rps") || die "rpx: can't chmod 0444 \"$basename.rps\"\n";

# Summarize report file.
# For each function: cumulative and absolute times.

print(
"               SUMMARY OF TRACE REPORT, FOR FILE $basename.rpx

$sumhead

This report contains six sections:

* Function execution times sorted by total cumulative time used.
* Function execution times sorted by average cumulative time used, per call.
* Function execution times, sorted by total internal time used.
* Function execution times, sorted by average internal time used, per call.
* Function execution times sorted by function name.
* Distribution of execution times over function's immediate descendants.");
printf('

                SUMMARY OF EXECUTION TIMES FOR EACH TRACED FUNCTION

In the following five reports, the number of calls gives the total calls for
the function for the duration of this trace. "cumtime" and "cumavg" give the
cumulative time for each function and the average cumulative time, per call.
"time" and "avg" give the time recorded within the function, excluding the
times of monitored calls from the function. All times are reported in
microseconds.

The report is reproduced five times, each of the first four times is sorted on 
one of the four time fields and the fifth is sorted by function name. In 
each copy of the report, the major sort key is identified by an asterisk.

');
$header = "\f
(microsecond units)   Function  #calls   cumtime    cumavg      time       avg
_______________________________________________________________________________
";

sub cumtime {$cumtime{$b} <=> $cumtime{$a};}
sub cumavg {$cumtime{$b}/$ncalls{$b} <=> $cumtime{$a}/$ncalls{$a};}
sub time {$abstime{$b} <=> $abstime{$a};}
sub avg {$abstime{$b}/$ncalls{$b} <=> $abstime{$a}/$ncalls{$a};}

foreach $type ("cumtime", "cumavg", "time", "avg") {
    $head = $header;
    $head =~ s/ $type/*$type/;
    print $head;
    foreach $func (sort $type keys(%ncalls)) {
	$ncalls = $ncalls{$func};
	printf("%30s%8d%10d%10d%10d%10d\n", $func, $ncalls,
	       $cumtime{$func}, $cumtime{$func}/$ncalls,
	       $abstime{$func}, $abstime{$func}/$ncalls);
    }
}

$head = $header;
$head =~ s/ Function/*Function/;
print $head;
foreach $func (sort keys(%ncalls)) {
    $ncalls = $ncalls{$func};
    printf("%30s%8d%10d%10d%10d%10d\n", $func, $ncalls,
	   $cumtime{$func}, $cumtime{$func}/$ncalls,
	   $abstime{$func}, $abstime{$func}/$ncalls);

}
print "\n";

print("\f",
'       DISTRIBUTION OF EXECUTION TIME OVER IMMEDIATE DESCENDENTS

In this report, the distribution of execution time for each function is shown
as a percentage of the time spent in the function and in each of its immediate
descendants. Only times actually accumulated while executing on behalf of the
parent function are considered. Time which is accumulated by the parent itself
is credited to "SELF" (except in the case of recursive calls to the parent,
which are credited in some non-obvious way to the name of the recursively
called routine).

As well as the percentages specified above, the average execution time of each
parent function is included for reference, and for each subsidiary function,
the average time spent in that function for one invocation of this particular
parent (which may include more than one invocation of the subsidiary
function), the average time for each of the calls to the function from this
particular parent, the average number of calls of the function for any given
call of the parent, and, finally, the total number of calls of the subsidiary
function in all invocations of the parent, are provided. The total number of
calls shown for "SELF" is the total number of times the parent function was
called. The total of the "avg time" column should equal the average time for
the parent.

');

sub callsort {
    @a = split(/\./, $a);
    @b = split(/\./, $b);
    (($a[0] cmp $b[0]) ||
     ($pcumtime{$b} <=> $pcumtime{$a}));
}

sub prtimes {
    local($name,$percent,$ncalls, $time) = @_;
    printf("        %5.1f%% %-20s%10.0f  %10.1f%10.1f%8d\n",
	   $percent, $name, $time/$parentcalls, $time/$ncalls,
	   $ncalls/$parentcalls, $ncalls);
}

loop: foreach $combo (sort callsort keys(%pcumtime)) {
    @combo = split(/\./,$combo);
    $parent = @combo[0];
    $func = @combo[1];
    next loop unless $parent;
    if ($parent ne $lastparent) {
	$ignoreParent = 0;
	$lastparent = $parent;
	$parentcalls = $ncalls{$parent};
	$parenttime = $cumtime{$parent};
	$absparenttime = $abstime{$parent};
	if (!$parenttime || !$absparenttime) {
	    $ignoreParent = 1;
	    print("INSUFFICIENT TRACE DATA IN LOGS FOR ANALYSIS OF FUNCTION $parent\n");
	    next loop;
	}
	$parentpercent = $absparenttime/$parenttime * 100;
	$pnfuncs = $pnfuncs{$parent};
	printf("\n%-22s(%7d usec) avg usec  avg usec/call avg calls ncalls\n",
	       "$parent:", $parenttime/$parentcalls);
    } else {
	next loop if ($ignoreParent);
    }
    
    $ncalls = ($pncalls{$combo} || 1);
    $time = $pcumtime{$combo};
    $percent = $time/$parenttime * 100;
    if ($parentpercent > $percent || $pnfuncs-- == 0) {
	&prtimes("SELF", $parentpercent, $parentcalls, $absparenttime);
	$parentpercent = -1;
    }
    &prtimes($func, $percent, $ncalls, $time);
    if (! $pnfuncs && $parentpercent != -1) {
	&prtimes("SELF", $parentpercent, $parentcalls, $absparenttime);
    }
}

sub Usage {
    local($msg) = @_;
    local($usage) = "$0 <path>.{native|icl}.rpt [ -sb <sandbox-path> ]";
    local($msg2);
    if ($msg) {
	$msg2 = "$0: $msg \n";
    }
    $msg2 .= "$0: usage: $usage\n";
    die($msg2);
}

sub DetermineSystemType {
    local(*sysType) = @_;
    local($retval) = 0;

    local($os) = `uname -s`;
    chop($os);
    if ($os =~ /^SunOS$/) {
	local($ver);
	$ver = `uname -r`;
	chop($ver);
	if ($ver >= 5.0) {
	    $sysType = SPARC_SOLARIS2;
	} else {
	    $sysType = "$os $ver";
	    $retval = 1;
	}
    } elsif ($os =~ /^AIX$/) {
	$sysType = RIOS;
    } else {
	$sysType = $os;
	$retval = 1;
    }
    $retval;
}
