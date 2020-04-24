#!/afs/tr/proj/tools/bin/perl

%types =
    ('epi_MutexLock: sleeping on', 'block',
     'epi_MutexLock: obtained', 'hold',
     'epi_MutexLock: grabbed', 'grab',
     'epi_MutexLock: released', 'rele',
     'epi_CondWait: sleeping', 'sleep',
     'epi_CondWait: waking up', 'awake',
     'epi_CondSignal:', 'signal',
     'epi_CondBroadcast:', 'broad');

%first = ();				# 1st index in trace arrays
%last = ();				# last 

%index = ();				# index of each timestamp
@time = (0);				# timestamp
@pid = (0);				# thread id
@object = (0);				# address of mutex or cv
@op = (0);				# operation happening to object
@nextPid = (0);				# next timestamp for this pid
@nextObj = (0);				# next timestamp affecting this object
@prevPid = (0);				# previous timestamp for this pid
@prevObj = (0);				# previous one for this object

%name = ();				# name of each object/thread
%type = ();				# type of each object/thread

$inFile = $ARGV[0];
open (IN, $inFile) || die ("Can't open $inFile: $!");

($inBase) = ($inFile =~ m=(.*)\.[a-z0-9]=i);
$inBase || ($inBase = $inFile);
$outThr = "$inBase.thr";
$outLock = "$inBase.lock";
$outCond = "$inBase.cond";
$outInt = "$inBase.int";
$outTime = "$inBase.time";

$maxTraces = 10;			# plot load on 1st ten sync objects

open (OUTTHR, ">$outThr") || die ("Can't write to $outThr: $!");
open (OUTLOCK, ">$outLock") || die ("Can't write to $outLock: $!");
open (OUTCOND, ">$outCond") || die ("Can't write to $outCond: $!");
open (OUTINT, ">$outInt") || die ("Can't write to $outInt: $!");
open (OUTTIME, ">$outTime") || die ("Can't write to $outTime: $!");

while (<IN>) {
    ($sec, $usec, $pid, $text, $addr) =
	(/^time (\d+)\.(\d+), pid (\d+): (.*) 0x([0-9a-z]+)/);
    if (!$sec) {
	# if is wasn't a synchronization trace, then maybe its a regular trace
	($sec, $usec, $pid, $rest) = (/^time (\d+)\.(\d+), pid (\d+): (.*)/);
	next if (!$sec);
	$name{$rest} = $rest;
    } else {
	undef $rest;
    }
    $time = $sec*1000000 + $usec;
    $op = $text && $types{$text};
    if (!$op && $addr) {
	if (($initializer, $name) =
	    (/(epi_Mutex|epi_Cond|lock_)Init: initializing (.*) at/)) {
	    $objType = ($initializer eq 'epi_Mutex' ? 'Mutex' :
			($initializer eq 'epi_Cond' ? 'Cond' :
			 ($initializer eq 'lock_' ? 'DLock' :
			  '???')));
	    $name{$addr} = "$name@$addr";
	    $type{$addr} = $objType;
	} else {
	    warn("Unknown trace type: $text line is $_");
	}
	next;
    }

    !@time || ($time-$time[$#time] >= 0) || die ("out of order timestamp");

    push (@time, $time);

    push (@pid, $pid);
    push (@prevPid, ($lp = $last{$pid}));
    $nextPid[$lp] = $#time;
    $first{$pid} = $#time if (!$first{$pid});
    $last{$pid} = $#time;
    $count{$pid}++;

    push (@op, $op);
    push (@object, $addr || $rest);
    push (@prevObj, $addr && ($lo = $last{$addr}));
    if ($addr) {
	$nextObj[$lo] = $#time;
	$last{$addr} = $#time;
	$first{$addr} = $#time if (!$first{$addr});
	$count{$addr}++;
    }

    # track gaps in synchronization timeline for computing active interval
    if ($addr) {
	$int = ($lastTime ? $time-$lastTime : 0);
	if ($int > $gap) {
	    $firstTime = $time;
	    $firstI = $#time;
	    $gap = $int;
	}
	$lastTime = $time;
	$lastI = $#time;
    }

    # only keep 100K entries to avoid huge paging problems
    last if (@time >= 100000);
}
close IN;
printf STDERR "Finished parsing icl log by %g\n", &cpu();

%blocked = ();				# time blocked for each thread
%intervals = ();			# time spent doing anything
@intervals = ();			# indexes of >1msec intervals

printf STDERR ("  skipping intervals that cross %7.3f.\n",
	       int($time[$firstI]/1000)/1000.0);
foreach $p (keys(%first)) {
    $first = $first{$p};
    $isPid = ($p eq $pid[$first]);
    if ($isPid) {
	$name{$p} = "t@$p";
	$type{$p} = 'Thread';
    } elsif (!$name{$p}) {
	$name{$p} = "???@$p";
	$type{$p} = 
	    ((($op[$first] eq 'block') || ($op[$first] eq 'hold') ||
	      ($op[$first] eq 'grab') || ($op[$first] eq 'rele'))
	     ? 'Mutex' : 'Cond');
    }
    $prev = 0;
    $blocked = 0;
    $active = 0;
    for ($i = $first; $i; ($prev = $i,
			   $i=($isPid ? $nextPid[$i] : $nextObj[$i]))) {
	# skip bogus intervals
	next unless ($prev && (($prev < $firstI) == ($i < $firstI)));

	$block = 0;
	$op = $op[$i];
	if (($op eq 'hold') || ($op eq 'awake')) {

	    # Count blocked time for objects.  For threads only count blocks
	    # due to CVs and Mutexes, DLocks are implemented in terms of them.

	    $obj = $object[$i];
	    if (!$isPid || ($type{$obj} ne 'DLock')) {
		$block = &Held();
		$blocked += $block;
	    }
	}
	if ($isPid) {
	    $interval = $time[$i] - $time[$prev];
	    $active += $interval if (!$block);

	    if ($interval > 10000) {
		push (@intervals, $i);
		$intervals{$i} = $interval;
	    }
	} elsif ($op eq 'rele') {
	    # for mutexes and locks compute holding time
	    $active += &Held();
	}
    }
    $blocked{$p} = $blocked;
    $active{$p} = $active;
}
printf STDERR "Computed blocking times by %g\n", &cpu();

#list of objects for sorting
@threads = ();
@mutexs = ();
@condvars = ();

foreach $p (keys(%first)) {
    if (($type{$p} eq 'Mutex') || ($type{$p} eq 'DLock')) {
	push(@mutexes, $p);
    } elsif ($type{$p} eq 'Cond') {
	push(@condvars, $p);
    } else {
	push(@threads, $p);
    }
}

sub ByBlocked { $blocked{$b} <=> $blocked{$a}; }

@threads = sort ByBlocked @threads;
@mutexes = sort ByBlocked @mutexes;
@condvars = sort ByBlocked @condvars;

sub ByInterval { $intervals{$b} <=> $intervals{$a}; }

@intervals = sort ByInterval @intervals;

printf STDERR "Finished sorting by %g\n", &cpu();

&PlotLoads();
printf STDERR "Finished plotting loads by %g\n", &cpu();

select OUTINT;
print "Intervals over 10 millisecond:\n";
foreach $i (@intervals) {
    printf(" +%6d = %10.6f  $name{$pid[$i]} $op[$i] $name{$object[$i]}\n",
	   $intervals{$i}, $time[$i]/1000000.0);
}
close OUTINT;
printf STDERR "Finished printing intervals by %g\n", &cpu();

select OUTTHR;
foreach $p (@threads) {
    print "$type{$p} $name{$p} has $count{$p} events.\n";
    @held = ();
    $prev = 0;
    $oldStamp = 0;
    %longest = ();			# updated by &Held()
    for ($i = $first{$p};
	 $i && ($i <= $lastI);
	 ($prev = $i, $i = $nextPid[$i])) {
	next if ($i < $firstI);
	$op = $op[$i];
	$obj = $object[$i];

	$holding = "";
	if (($op eq 'hold') || ($op eq 'grab')) {
	    push (@held, $obj);
	} elsif ($op eq 'rele') {
	    @held = grep ($_ ne $obj, @held); # filter out lock being released
	} elsif ((@held > 0) && (($op eq 'block') || ($op eq 'sleep'))) {
	    $holding = " HOLDING:$name{$held[0]}";
	    foreach $h (@held[1..$#held]) {
		$holding .= ",$name{$h}";
	    }
	}

	printf(" +%s = %s  $name{$obj}%s\n",
	       &PrintedInterval($prev), &Stamp(),
	       $op && (": $op" . &PrintedHeld() . $holding));
    }
    print " Longest wait $longest{'hold'}; held $longest{'rele'}; sleep $longest{'awake'}.\n";
    print " Blocked $blocked{$p} and running $active{$p} in $type{$p} $name{$p}\n\n";
}
close OUTTHR;
printf STDERR "Finished printing threads by %g\n", &cpu();

select OUTTIME;
$oldStamp = 0;
for ($i = $firstI; $i<=$lastI; $i++) {
    $op = $op[$i];
    printf(" +%s = %s  $name{$pid[$i]} $name{$object[$i]}%s\n",
	   &PrintedInterval($i-1), &Stamp(), 
	   $op && (": $op" . &PrintedHeld()));
}
close OUTTIME;
printf STDERR "Finished printing time line by %g\n", &cpu();

foreach $p (@mutexes, @condvars) {
    select (($type{$p} eq 'Cond') ? OUTCOND : OUTLOCK);
    print "$type{$p} $name{$p} has $count{$p} events.\n";
    $prev = 0;
    $oldStamp = 0;
    %longest = ();			# updated by &Held()
    for ($i = $first{$p}; $i; ($prev = $i, $i= $nextObj[$i])) {
	printf(" +%s = %s  $name{$pid[$i]}: $op[$i]%s\n",
	       &PrintedInterval($prev), &Stamp(), &PrintedHeld());
    }
    print (" Longest " .
	   ($longest{'awake'}
	    ? "sleep $longest{'awake'}"
	    : "wait $longest{'hold'}; held $longest{'rele'}") .
	   ".\n");
    print " Blocked $blocked{$p}";
    print " and held $active{$p}" if ($active{$p});
    print " on $type{$p} $name{$p}\n\n";
}
close OUTLOCK;
close OUTCOND;
select STDOUT;
printf STDERR "Done writing output by %g\n", &cpu();

# CAUTIONS -- free variables: @blkdIndex, @objs, $j

sub SelectObj
{
    local ($expr) = @_;
    local (@matches) = grep (eval($expr), @objs);

    if (!@matches) {
	print "No matches for '$expr'.\n";
    } elsif (@matches > 1) {
	print "Ambiguous match for '$expr':\n";
	foreach $o (@matches) {
	    print "  $type{$o} $name{$o}\n";
	}
    } else {
	$blkdIndex{$matches[0]} = $j;
	$blkdObj[$j] = $matches[0];
	$j++;
    }
}

sub PlotLoads
{
    local (@blkd);			# counts of threads blocked objs
    local (@blkdObj);			# object of each blkd index
    local (%thr);			# whether thread is running (or not)
    local (@initblkd);			# initial counts
    local (@blkt);			# time last change in blkd
    local (@desc);			# file descriptors for each curve
    local (%blkdIndex);			# which blkd cntr to use for each obj
    local (@objs) = sort ByBlocked (@mutexes,@condvars);
    local ($running);
    local ($h,$i,$j,$k,$f,$o,$p,$t);
    local (@is, @js);			# all the deltas
    local ($baseSec,$baseTime);			# x-origin for plotting

    $j = 1;

    &SelectObj('$name{$_} =~ /^buffer@/ && $type{$_} eq "Mutex"');
    &SelectObj('$name{$_} =~ m=^I/O@=');
    &SelectObj('$name{$_} =~ /^tran sleepers@/');
    &SelectObj('$name{$_} =~ /^quota@/');

    foreach $o (@objs) {
	next if ($blkdIndex{$o});	# already assigned
	if ($name{$o} =~ /^incomplete work@/) {
	    # put I/O thread waiting in "other" category
	    $blkdIndex{$o} = $maxTraces;
	} else {
	    $blkdIndex{$o} = $j;
	    $blkdObj[$j] = $o;
	    $j++ if ($j < $maxTraces);
	}
    }

    $baseSec = int($firstTime/1e6);
    $baseTime = 1000000*$baseSec;

    foreach $i (0..$maxTraces) {
	$f = "$inBase.plot.$i";
	$file = $desc[$i] = "plot.$i";
	open ($file, ">$f") || die ("Can't write to $f: $!");
	if ($i == 0) {
	    print $file "\"running\"\n";
	    print $file "YLowLimit: 0\nYHighLimit: " . (0+@threads) . "\n";
	    print $file "XUnitText: $baseSec + usec\n";
	    print $file "YUnitText: threads\n";
	    print $file "TitleText: $inBase\n";
	} elsif ($i == $maxTraces) {
	    print $file "\"other\"\n";
	} else {
	    print $file "\"$name{$blkdObj[$i]}\"\n";
	}
    }

    %ignore = ();
    foreach $o (@condvars) {
	$ignore{$o} = 1
	    if ($name{$o} =~ /^d(lock|cond) cond \#\d+@/);
    }

    %thr = ();
    @blkd = @initblkd = ();
    @is = @js = ();
    for ($i=$firstI; $i<@time; $i++) {
	next if ($ignore{$object[$i]});
	$o = $op[$i];
	$p = $pid[$i];
	$j = $blkdIndex{$object[$i]};
	if ((($o eq 'hold') || ($o eq 'awake')) && !$thr{$p}) {
	    if (!defined($thr{$p})) {
		$initblkd[$j]++;
		$blkd[$j]++;
	    }
	    $thr{$p} = 1;
	    $running = 1;
	} elsif (($o eq 'block') || ($o eq 'sleep')) {
	    if (!defined($thr{$p})) {
		$thr{$p} = 1;
		$initblkd[0]++;
		$blkd[0]++;
	    }
	    if ($thr{$p}) {
		$running = -1;
		$thr{$p} = 0;
	    }
	} else {
	    next;
	}
	$blkd[0] += $running;
	$blkd[$j] -= $running;
	print ("no running threads @ $time[$i]\n")
	    if ($blkd[0] < 0);
	$initblkd[$j] = -$blkd[$j] if (-$blkd[$j] > $initblkd[$j]);
	push (@is, $i);
	push (@js, $running*($j+1));
    }

    @blkd = @initblkd;
    @blkt = ();
    for ($h=0; $h<@is; $h++) {
	$i = $is[$h];
	$j = $js[$h];
	if ($j < 0) {
	    $j = -$j-1;
	    $running = -1;
	} else {
	    $j = $j-1;
	    $running = 1;
	}
	$blkd[0] += $running;
	$blkd[$j] -= $running;
	$t = $time[$i]-$baseTime;
	$l = 0;
	foreach $k (0..$j) {
	    $l += $blkd[$k];
	    if (defined($blkt[$k])) {
		$file = $desc[$k];
		printf $file ("$t %g\n", $l + (($k == $j) ? 0 : -$running));
		printf $file ("$t %g\n", $l);
	    }
	    $l += 0.05;			# offset traces a bit
	    $blkt[$k] = $t;
	    print "Too many threads @ $t\n"
		if ($l > 1+@threads);
	}
	$ot = $t;
    }
    foreach (@desc) {
	close $_;
    }
}

# Interval -- compute interval between $time[$i] and $time[<arg>].

sub Interval
{
    local ($prev) = @_;

    if (!$prev || (($i < $firstI) != ($prev < $firstI))) {
	$oldStamp = 0;
	0
    } else {
	$time[$i] - $time[$prev];
    }
}

sub PrintedInterval
{
    local ($i) = &Interval(@_);
    ($i ? sprintf("%6d", $i) : " -----");
}

# Stamp -- returns time stamp of $time[$i] and updating $oldStamp.  It takes no
#     explicit arguments.

sub Stamp
{
    local ($stamp);

    $stamp = sprintf("%7.3f", int($time[$i]/1000)/1000.0);
    if ($stamp == $oldStamp) {
	$stamp = (' 'x7);
    } else {
	$oldStamp = $stamp;
    }
}

# Held -- determines interesting intervals.
#
# CAUTIONS -- $i and %longest are free variables.

sub Held
{
    local ($op) = $op[$i];
    local ($obj) = $object[$i];
    local ($prev, $int);

    if ($op eq 'rele') {
	for ($prev = $prevPid[$i]; $prev; $prev = $prevPid[$prev]) {
	    next if ($object[$prev] ne $obj);
	    last if (($op[$prev] eq 'grab') || ($op[$prev] eq 'hold'));
	}
    } elsif ($op eq 'hold') {
	for ($prev = $prevPid[$i]; $prev; $prev = $prevPid[$prev]) {
	    next if ($object[$prev] ne $obj);
	    last if ($op[$prev] eq 'block');
	}
    } elsif ($op eq 'awake') {
	for ($prev = $prevPid[$i]; $prev; $prev = $prevPid[$prev]) {
	    next if ($object[$prev] ne $obj);
	    last if ($op[$prev] eq 'sleep');
	} 
    }
    if ($prev) {
	$int = &Interval($prev);
	$longest{$op} = $int if ($int > $longest{$op});
	$int;
    } else {
	0;
    }
}

sub PrintedHeld
{
    $int = &Held();
    ($int ? sprintf(" (%d)", $int) : "");
}

sub cpu
{
    local (@t) = times;
    $t[0]+$t[1];
}
