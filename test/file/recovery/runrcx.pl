#!/afs/tr/proj/tools/bin/perl
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: runrcx.pl,v $
# Revision 1.1.8.1  1996/10/17  18:34:13  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:59  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:58  devsrc
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:28  delgado]
# 
# 	delta bob-add-rcx 1.1
# 	[1992/05/04  18:47:14  jdp]
# 
# Revision 1.1.17.1  1994/02/04  20:14:41  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:04  devsrc]
# 
# Revision 1.1.15.4  1994/01/11  18:35:58  berman
# 	Transarc Delta: rajesh-db4799-rcx-copyaggr-optional-checksum 1.5
# 	    During compare aggr phase compare only as many
# 	    blocks as necessary.
# 	[1994/01/11  18:32:23  berman]
# 
# Revision 1.1.15.3  1994/01/10  16:08:56  berman
# 	Transarc Delta: bwl-o-ot9284-hp-port-rcx-tests 1.7
# 	    Fix some typos.
# 	    A "line" of output may be split between two lines.  Also,
# 	    must not confuse agfs mounts with Episode local mounts.
# 	    Sleep 30 seconds before invoking bdf, so that the syncer can
# 	    update /etc/mnttab, since we aren't updating it ourselves in
# 	    epimount/epiunmount.
# 	    Invoke sync (command) after invoking chftids.
# 	    close/open the block devices around the call to chftids, so
# 	    that it gets fresh buffers for all its block device I/O and
# 	    invalidates them all when it's done.
# 	[1994/01/10  16:00:23  berman]
# 
# Revision 1.1.15.2  1993/12/07  20:32:18  jaffe
# 	Fixed Bad Comment leaders
# 	[1993/12/07  20:31:14  jaffe]
# 
# Revision 1.1.15.1  1993/12/07  17:20:22  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.1.2  1993/12/02  20:34:59  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.4.6  1993/01/19  15:31:47  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:53:20  cjd]
# 
# Revision 1.1.4.5  1992/12/09  20:04:49  jaffe
# 	Transarc delta: rajesh-ot6144-make-rcx-work-with-AGFS 1.1
# 	  Selected comments:
# 
# 	    AGFS code causes efts attach to create an additional mount point for
# 	    the aggregate. RCX looks to epiunmount all filesets that may be mounted
# 	    from relevant aggregates by parsing df output using the device name as
# 	    the key.  This causes an epiunmount of the aggregate mount point,
# 	    which fails.  Solution is to detach all aggregates before trying to
# 	    epiunmount filesets so that the aggregate mount points do not show up
# 	    in df output.
# 	    See above.
# 	[1992/12/03  22:15:39  jaffe]
# 
# Revision 1.1.4.4  1992/11/18  19:19:45  jaffe
# 	Transarc delta: rajesh-ot5542-port-rcx-to-osf1 1.2
# 	Transarc delta: rajesh-ot5591-rcx-tests-vary-transaction-concurrency 1.2
# 	Transarc delta: rajesh-ot5841-clean-up-rcx 1.11
# 	[1992/11/17  20:34:01  jaffe]
# 
# Revision 1.1.4.3  1992/10/28  21:43:16  jaffe
# 	Fixed RCS id
# 	[1992/10/28  21:13:59  jaffe]
# 
# Revision 1.1.4.2  1992/10/27  20:52:51  jaffe
# 	Transarc delta: rajesh-ot5391-errors-in-runrcx 1.6
# 	  Selected comments:
# 	    Fix bugs in runrcx script.
# 	    The bugs are
# 	    1. Wrt packaging the runrcx script access perl from Bob's home dir.
# 	    2. It does not create the log directory as expected but assumes it to
# 	    be present even though it allows the user to set the name of the log
# 	    dir to create.
# 	    3. Syntax error in a print statement
# 	    Further updates
# 	    A checkpoint.
# 	    Running version
# 	    Make a portion of code more readable.
# 	    The reason behind the if statement, changed by this delta was not
# 	    immediately obvious. Change the if statement to be more readable by
# 	    using the variable $norecover instead of number of elements in the
# 	    pids array. This is a checkpoint and needs testing.
# 	    Minor cleanup to all_tests and stress_tests
# 	Transarc delta: rajesh-ot5517-modify-rcx-to-use-efts 1.3
# 	  Selected comments:
# 	    Modify rcx test to not use the obsolete efts but to use efts.
# 	    The script now used efts attach, detach and createft.
# 	    An aggregate cannot be attached if its not already an Episode aggregate.
# 	    The script uses 3 aggregates specified by PAR1, PAR2 and PAR3.
# 	    However, only PAR1 was being initialised as episode. [ PAR2 and PAR3
# 	    are used as copy aggregates that are "dd"ed into and hence strictly
# 	    speaking do not need to be initialised.] Since PAR2 and PAR3 were not
# 	    being initialised as episode, efts attach could fail if PAR2 and/or
# 	    PAR3 was a non-Episode filesystem e.g. UFS.
# 	    Tested version
# 	    Detach the aggregates before exiting.
# 	Transarc delta: rajesh-ot5591-rcx-tests-vary-transaction-concurrency 1.1
# 	  Selected comments:
# 	    Currently, rcx tests create the aggregate with default logsize, [
# 	    which usually restricts transaction concurrency to a v. small value
# 	    e.g 1 or 2 ]. So modify rcx to use the -translimit option to newaggr
# 	    for max. transaction concurrency as default behaviour. Also, allow
# 	    setting of non-default concurrency by usage of a env variable
# 	    RCX_CONCURRENT_TRANS_NUM.
# 	    See above. Changes tested.
# 	[1992/10/27  13:01:33  jaffe]
# 
# $EndLog$
#
# runrcx -- Wrapper for rcx
#
# $Header: /u0/rcs_trees/dce/rcs/test/file/recovery/runrcx.pl,v 1.1.8.1 1996/10/17 18:34:13 damon Exp $
# Copyright (C) 1993, 1992 Transarc Corporation - All rights reserved
#
$USAGE = "Usage: runrcx rcx_script";

#
# Assumptions: 
#   RIOS:  kernel extensions dfscore.ext and dcelfs.ext are loaded 
#          and epidaemon running  
#   PMAX:  kernel used is efsvmunix and epiinit has been run.  
# 
#   SUNOS5: None as relevant kernel module are automatically loaded on first
#           access.
#

# Determine system type
$SYSTYPE=`uname`;
chop $SYSTYPE;
$OSREL=`uname -r`;
chop $OSREL;

# Turn off when not needed.
#$debug = 1;

# Special flag for debugging on OSF/1

if ( ! $ENV{'OSFDEBUG'}  ) {
  $osfdebug=0;
} else {
  $osfdebug=1;
}

# Arguments
$RCX_SCRIPT = shift;
&x("Usage: runrcx script_name") if (!$RCX_SCRIPT || shift);

&console("***runrcx $RCX_SCRIPT***");

# Options

# Turn off recovery testing.
# $norecover = 1;

# Log steps on console, as well as on file.
$log_on_console = 1;

# Don't create any log files (for debugging subsidiary programs).
#$log_only_on_console = 1;

# Run salvage if recovery fails. Off by default.
# $runsalvage = 1


# Run verification tests after recovery.
# Level = 1: basic tests. Level = 2: run read/write test for a while
# Also set $rcx_verify_kernel to force verification of kernel recovery.
$rcx_verify = 2;
#$rcx_verify_kernel = 1;

# Run tests in verbose mode (probably should set noswitchlogs to one
# for this to be useful, so that all test output is interleaved).
# $rcxecho = 1;	

# Switch logs on each cycle, if this isn't set.
# $noswitchlogs = 1;

# Save the episode log for each aggregate whenever a log block changes.
# $savelogs = 1;

# Whenever the number of good logs acquired reaches this number, then delete
# a log.
# $deletelogs = 25;

# Also save the aggregate when a log block changes.
# $savelogandaggr=1; 

# Set maximum write step: if this is non-zero, then output will be frozen
# after a randomly determined number of output steps up to this maximum
# number. Otherwise, output will continue for an undetermined amount of time.
$minstep = 1;
$maxstep = 100;

# This option can cause a deadlock. - When PAR1 is frozen, it is
# holding the global buffer lock. An epimount of a fileset from PAR2
# calls elbb_ReadGeneral which tries to obtain the buffer lock and
# goes to sleep on it --> deadlock. Hence filesets should not be mounted 
# from PAR2 while PAR1 is frozen. 
# Set this if aggregate should remain frozen between cycles.
# This is necessary if you are single stepping, but impossible if you
# are mounting other aggregates within the cycle.
# $freeze_each_cycle = 1;

# Time all commands executed via system() i.e. sub sys().
$timeCommands = 0;
if ($ENV{RCX_TIMECMDS}) {
   $timeCommands = 1;
}

# When an error occurs, single step the aggregate, rather than using
# the original value of $maxstep.
# $singlestep_on_error = 1;

# Save aggregates with errors.
$saveerrors = 1;

# Stop on first error
$stopOnStatError = 0;
if ($ENV{RCX_STOP_ON_STAT_ERROR}) {
   $stopOnStatError = 1;
} 

# Stop processes between tests, if this is set.
if ($ENV{RCX_NOSTOP}) {
   $stop = 0;
} else {
   $stop = 1;
}

$saveRecoveredAggrOnError = 0;
if ($ENV{RCX_SAVE_RECOVERED_AGGR_ON_ERROR}) {
    $saveRecoveredAggrOnError = 1;
}

$checkPhaseReadonlyFsetMount = 0;
if ($ENV{RCX_CHECK_PHASE_READONLY_FSET_MOUNT}) {
   $checkPhaseReadonlyFsetMount = 1;
}

$verifyRecoveredAggrAgain = 0;
if ($ENV{RCX_VERIFY_RECOVERED_AGGR_AGAIN}) {
   $verifyRecoveredAggrAgain = 1;
}

# Stagger processes (each cycle, allow one process to continue for a random
# interval).
$stagger = 1;

# Sleep time between each cycle, to allow rcx processes to run.
$sleepeachcycle = 30;

$VERSION = "1.25";

# Episode sets that max transaction concurrency size to 
# 10 (FB_MAX_STARTED_TRANS) in anode/block_alloc.c. Hence set the same limit
# for RCX tests.
$MAX_CONCURRENT_TRANS_NUM=10; 

$EPIBIN = $ENV{'RCX_EPIBIN'}; # if this is null, will just use user's path.

# Programs & default parameters.
$CHECKSUM = "sum";

$NEWAGGR = "${EPIBIN}newaggr";
$EFTS = "${EPIBIN}efts";
$MOUNT = "${EPIBIN}epimount";
$UMOUNT = "${EPIBIN}epiunmount";
$SALVAGE = "${EPIBIN}salvage";
$COPYAGGR = "${EPIBIN}copyaggr";
$ASDB = "${EPIBIN}asdb";	# Freeze/unfreeze aggregate through async layer.
$FINDLOG = "${EPIBIN}findlog";	# Find episode log for an aggregate
$CALCLOGSIZE = "${EPIBIN}calcLogSize";

$DFSTRACE = "dfstrace";

$CMPSIZE = 64*1024;		# **cmpaggr assumes that aggr is multiple of this size.

$BLOCKSIZE = $ENV{'RCX_BLOCKSIZE'} || 8192;
$FRAGSIZE = $ENV{'RCX_FRAGSIZE'} || 1024;

$doubleLogSpace = 0;
if ($ENV{'RCX_DOUBLE_LOGSPACE'}) {
	$doubleLogSpace = 1;
}

if (  $ENV{'RCX_LOGBLOCKS'} && $ENV{'RCX_CONCURRENT_TRANS_NUM'} ) {
	&x("runrcx: cannot set both RCX_LOGBLOCKS and RCX_CONCURRENT_TRANS_NUM");
}

$LOGBLOCKS = $ENV{'RCX_LOGBLOCKS'};
$NUM_CONCURRENT_TRANS=$ENV{'RCX_CONCURRENT_TRANS_NUM'};

if ($LOGBLOCKS ) {
	$logsize=$LOGBLOCKS;
   	$LOGOPTION = "-logsize $logsize";
} elsif  ( $NUM_CONCURRENT_TRANS ) {
	#
	# calcLogSize is a new utility in episode/anode that calculates
	# the log size necessary for a certain transaction concurrency
	# level and a block size
	#
	&step("Calculate logsize to allow $NUM_CONCURRENT_TRANS concurrent transactions");
	$logsize=`$CALCLOGSIZE $BLOCKSIZE $NUM_CONCURRENT_TRANS`;
	chop($logsize);
	if ($doubleLogSpace) {
		$logsize *= 2;
	}
	$LOGOPTION= "-logsize $logsize";
} else {
	&step("Calculate logsize to allow $MAX_CONCURRENT_TRANS_NUM concurrent transactions");
	$logsize=`$CALCLOGSIZE $BLOCKSIZE  $MAX_CONCURRENT_TRANS_NUM`;
	chop($logsize);	
	if ($doubleLogSpace) {
		$logsize *= 2;
	}
	$LOGOPTION= "-logsize $logsize";
}
	&step("Aggregate logsize will be $logsize blocks");

$PARBASE = 'epircx';
if ( $SYSTYPE eq "SunOS" && $OSREL >= 5.0 ) {
# V. quick hack to take care of different device naming style in SunOS 5.2
	$par = $ENV{'RCX_DEV1'} || "${PARBASE}a";
	$PAR1 =	"dsk/${par}";

	$par = $ENV{'RCX_DEV2'} || "${PARBASE}b";
	$PAR2 =	"dsk/${par}";

	$par = $ENV{'RCX_DEV3'} || "${PARBASE}c";
	$PAR3 =	"dsk/${par}";
} else {
	$PAR1 = $ENV{'RCX_DEV1'} || "${PARBASE}a";
	$PAR2 = $ENV{'RCX_DEV2'} || "${PARBASE}b";
	$PAR3 = $ENV{'RCX_DEV3'} || "${PARBASE}c";
}
# These are used by mkdir to set up mount points for the aggregates.
$MNT1 = $ENV{'RCX_MNT1'} || "${PAR1}";
$MNT2 = $ENV{'RCX_MNT2'} || "${PAR2}";
$MNT3 = $ENV{'RCX_MNT3'} || "${PAR3}";

# Decide whether or not we need to prepend "/dev/" to the device names.
if ( $PAR1 =~ m#/dev/# ) {
    $PREPENDDEV = "";
} else {
    $PREPENDDEV = "/dev/";
}

# Base name of mount points. Mount point is $MOUNTBASE/dev/vol
# Does this need to be created?
$MOUNTBASE = $ENV{'RCX_DIR'} || "/tmp/rcx";

# Magic file name - also defined in rcx.
$MAGIC_FILE = ".recovery_test_dir";

$RCX = "rcx";			   	# Recovery excerciser program

$NTHREADS = $ENV{'RCX_NTHREADS'} || 10; 	# Number of copies of rcx to run.
$NCYCLES = $ENV{'RCX_NCYCLES'} || 20;       # Number of cycles.

# run salvage verification before aggregate  has been recovered. Should not 
# be normally done
$VERIFYBEFORERECOVERY = $ENV{'RCX_VERIFYBEFORERECOVERY'} || 0;

# Where to find perl scripts to be included in chftid
$PERLINC=$ENV{'RCX_PERLINC'};
# The number of bits that fileset ids from $PAR3 will be shifted.
# The scavenge/chftid script assumes the shift factor to be 30.
$newvolidshift = 30 ;

# number to add to NTHREAD index to obtain volids.  To avoid DFS clash.
$volidOffset = 100;

# Save stderr, since logging takes it over.
open(CONSOLE, ">&STDERR");
$RUNLOGDIR = $ENV{'RCX_LOGDIR'} || 'runlog';

# Initialize for logging.
@ltime = localtime(time);
$date = sprintf("%02d%02d", $ltime[4] + 1, $ltime[3]);
$id = "aa";
# Create log dir if it does not exist
&makedir("$RUNLOGDIR");
if (open(LOGID,"$RUNLOGDIR/.logid")) {
	chop($logid = <LOGID>);
	($olddate, $oldid) = split(/\./, $logid);
	$id = ++$oldid if $date eq $olddate;
}		
$logid = "$date.$id";
open(LOGID, ">$RUNLOGDIR/.logid")
	|| &x("Unable to create $RUNLOGDIR/.logid ($!)");
print LOGID "$logid\n";
close(LOGID);

&log("$RUNLOGDIR/runlog.$logid");
chop($date = `date`);
&step("Runrcx, version $VERSION, Started $date

Parameters:

    rcx driver      $RCX
    rcx script      $RCX_SCRIPT
    blocksize       $BLOCKSIZE
    fragsize        $FRAGSIZE
    logsize         $logsize blocks
    nprocs          $NTHREADS
    ncycles         $NCYCLES
");

&step("Environment variables affecting this run are");
while (($envVar, $val) = each(%ENV)) {
	if ($envVar =~ /RCX_/i) {
		&step("Var $envVar     $val");
	}
}

# Dump contents of rcx script, runrcx script and the .rs script used for 
# the current run.

&dump($0, "$RUNLOGDIR/runrcx.$logid");
&dump($RCX, "$RUNLOGDIR/rcx.$logid");
&dump("$RCX_SCRIPT.rs", "$RUNLOGDIR/script.rs.$logid");

# Unfreeze any aggregates that may inadvertently be frozen.
&epi_unfreeze("silent", $PAR1, $PAR2, $PAR3); 

# Detach aggregates if they are attached.
&step("Detaching aggregates");
&efts_detach("silent",$PAR1,$PAR2,$PAR3);

# Unmount any volumes that we may care about.
# ***This is platform dependent***
&step("Unmounting file systems");

$DF = "df";
if ($SYSTYPE =~ /HP-UX/i) {
   $DF = "bdf";
   # sleep a while to let syncer update /etc/mnttab, since
   # we aren't updating /etc/mnttab ourselves in epimount/epiunmount
   &sleep(30);
}
if (($SYSTYPE =~ /SunOS/i) && ($OSREL >= 5.0)) {
   $DF = "df -k";
}

for (`$DF`) {
	if ( $SYSTYPE =~ /AIX/i ) {
		($dev,$d1,$d2,$d3,$d4,$d5,$on) = split;
		if ($dev =~ m#/dev/($PAR1|$PAR2|$PAR3)#o) {
			&epi_umount($on) && &x("couldn't unmount $on ($sysexit)");
		}
	} elsif ( $SYSTYPE =~ /HP-UX/i ) {
		($dev,$d1,$d2,$d3,$d4,$on) = split(' ');
		# output might be on two lines
		if ($dev =~ m#($PAR1|$PAR2|$PAR3)#o
		    && $dev !~ m#DFS#o) {
			if ($on) {
				&epi_umount($on) && &x("couldn't unmount $on ($sysexit)");
			} else {
				$olddev = $dev;
			}
		} elsif ( $olddev ) {
			if ($d4) {
				&epi_umount($d4) && &x("couldn't unmount $d4 ($sysexit)");
			}
		}
	} elsif ( $SYSTYPE =~ /OSF1/i ) {
		($dev,$d1,$d2,$d3,$d4,$on,$d5) = split;

		# hack to get around non standard white space usage in df
		# output for mounted episode filesets. Yuck!
		if ( $on =~ /%/ ) {
			$on=$d5;
		}

		if ( ($dev =~ m#/dev/($PAR1|$PAR2|$PAR3)#o) || ($on =~ m#/($PAR1|$PAR2|$PAR3)#o) ) {
			&epi_umount($on) && &x("couldn't unmount $on ($sysexit)");
		}
	} elsif (($SYSTYPE =~ /SunOS/i)) {
		if ($OSREL >= 5.0) { 
			($dev,$d1,$d2,$d3,$d4,$on) = split(' ');
			if ( $dev =~ /\((.*)\):/ ) { 
				$dev=$1;
			}
			if ($dev =~ m#/dev/($PAR1|$PAR2|$PAR3)#o) {
				&epi_umount($on) && &x("couldn't unmount $on ($sysexit)");
			}
		}
	} else {
		&step("Unable to epiunmount any relevant filesets as non-AIX and non_OSF1 system");
		last;
	}
}

&step("Making mount points");
&makedir("$MOUNTBASE");
foreach $aggr ($MNT1, $MNT2, $MNT3) {
	&makedir("$MOUNTBASE/$aggr");
	for $volid (1..$NTHREADS) {
		$volid += $volidOffset;
		&makedir("$MOUNTBASE/$aggr/rcx.$volid");
	}
}

# Make aggregate and volumes.

&step("Creating new aggregates.");
if ($ENV{RCX_SRCAGGR_NEWAGGR_SPEC}) {
	&newaggr_spec($PAR1, $BLOCKSIZE, $FRAGSIZE, $LOGOPTION);
	$SRC_PAR_NBLKS = $AGGR_NBLOCKS;
} else {
	&x ("Could not create aggregate $PAR1 ($sysexit)")
		if &sys("$NEWAGGR $PAR1 $BLOCKSIZE $FRAGSIZE -overwrite $LOGOPTION");
}

&x ("Could not create aggregate $PAR2 ($sysexit)")
	if &sys("$NEWAGGR $PAR2 $BLOCKSIZE $FRAGSIZE -overwrite $LOGOPTION");
&x ("Could not create aggregate $PAR3 ($sysexit)")
	if &sys("$NEWAGGR $PAR3 $BLOCKSIZE $FRAGSIZE -overwrite $LOGOPTION");

&step("Attaching source aggregate");
&efts_attach($PAR1);

&step("Creating new filesets.");
foreach $volid (1..$NTHREADS) {
	$volid += $volidOffset;
	if (&efts_createvol("$volid","rcx.$volid",32768,"$PAR1"))  {
		&x ("Couldn't create fileset rcx.$volid ($sysexit)");
	}
}


# Avoid vnode reclaimation deadlock on OSF1 systems
#if ( $SYSTYPE =~ /OSF1/i ) {
#	&openexecfiles;
#	&openaggrs($PAR1,$PAR2,$PAR3);
#	&findpathdirs;
#	&openalldirs;
#}

# We need not do all of the above, now that we keep the aggregate frozen 
# for as short a time as possible by using "copyaggr" instead of "asdb"
# and "dd(1)". But we still need to some of the above i.e. keep copyaggr
# open and the devices also open
&openaggrs($PAR1,$PAR2,$PAR3);
$COPYAGGRFILE = &which($COPYAGGR);
&step("open executable $COPYAGGR"); 	
&openfile(COPYAGGR,$COPYAGGRFILE);
$CHECKSUMFILE = &which($SUM);
&step("open executable $SUM"); 	
&openfile(CHECKSUM,$CHECKSUMFILE);

# Find the log blocks, for later use by savelog code.
&findlog($PAR1);

# Create the test dirs with magic files for all filesets
&createTestdirs($PAR1, $MNT1);

# Compile rcx test script, using target file system.
&step("Compiling test script, $RCX_SCRIPT");
$volid = 1+$volidOffset;
if (&epi_mount($PAR1, $MNT1, $volid)) {
	&x ("Couldn't mount fileset rcx.$volid on $PAR1 ($MOUNTBASE/$MNT1/rcx.1) ($sysexit)");
}

if (&sys("$RCX -c $RCX_SCRIPT $MOUNTBASE/$MNT1/rcx.$volid/rcx.test $BLOCKSIZE $FRAGSIZE")) {
	&x("compilation of test script $RCX_SCRIPT failed");
}

# Dump the compiled (.rc, .rh) versions of the .rs script 
&dump("$RCX_SCRIPT.rc", "$RUNLOGDIR/script.rc.$logid");
&dump("$RCX_SCRIPT.rh", "$RUNLOGDIR/script.rh.$logid");

# Unmount fileset with id rcx.$volid and 
if (&epi_umount("$MOUNTBASE/$MNT1/rcx.$volid")) {
	&x ("Couldn't unmount fileset rcx.$volid from $PAR1 ($MOUNTBASE/$MNT1/rcx.1) ($sysexit)");
}

# Mount partition 1 volumes.
&mountvols($PAR1, $MNT1);

# Start some processes.
&runprocs($MNT1,0);
&step("Running rcx tests (this will take 30 seconds)");
&sleep(30);

$DEVICE_CHECKSUM_FILE = "/tmp/checksum.$$";

# differ seed for random # generator
srand(time());
$LSTATERROR = 2;
$exitNow = 0;

for ($cycle = 1; $cycle <= $NCYCLES; $cycle++) {
	local($error) = 0;
	local($kernelLogFile) = sprintf("$RUNLOGDIR/klog.$logid.%03d", $cycle);

	&stagger if $stagger;
	if ($sleepeachcycle) {
		&step("Letting test(s) run for $sleepeachcycle seconds");
		&sleep($sleepeachcycle);
	}
	&log(sprintf("$RUNLOGDIR/runlog.$logid.%03d", $cycle)) unless $noswitchlogs;
	$dumpname = sprintf("$RUNLOGDIR/aggr.$logid.%03d", $cycle);
	$dumpRecoveredAggrName = "$dumpname.rcvrd";
	chop($date = `date`);
	&console("
RCX Test cycle #$cycle starting at $date ...
");

# Freeze the aggregate, stop processes, copy it, unfreeze it.

	if ($norecover) {
		# Hack to test not using recovery...
		# This kills the processes, unmounts the partition, and then re-runs
		# the processes afterwards. Also test freeze/unfreeze by doing it a 
		# few times.
		&epi_stepio($PAR1, $minstep, $maxstep); # freeze or step aggr.
		&stop if $stop;
		&sleep(5);	# Sleep to allow last I/O to finish.
		&step("No-recovery test: freeze/unfreeze. This will take 30 seconds");
		&cont;
		for $tries (1..10) {
			&epi_unfreeze($PAR1);
			&sleep(3);	
			&epi_freeze($PAR1);
		}
		&epi_unfreeze($PAR1);
		&step("No-recovery test: killing processes");
		&kill;
		&sleep(5);
		&step("No-recovery test: unmounting partition $PAR1");
		&unmountvols($PAR1);
	}
	# Reverse temporary partitions, for any tests that may need old partition.
	($PAR2, $PAR3) = ($PAR3, $PAR2);
	&logDate;
	&step("Freeze aggregate, make copies, unfreeze it");
	open(DEVICE_CHECKSUM_FH, ">$DEVICE_CHECKSUM_FILE") ||
		&x("Unable to create $DEVICE_CHECKSUM_FILE ($!)");
	if ($stop) {
		local($count) = (rand($maxstep - $minstep + 1) | 0) + $minstep;
		&copyaggr($PAR1, $PAR2, $PAR3, $DEVICE_CHECKSUM_FILE, $count);
	} else {
		&copyaggr($PAR1, $PAR2, $PAR3, $DEVICE_CHECKSUM_FILE);
	}
	close(DEVICE_CHECKSUM_FH);
	open(DEVICE_CHECKSUM_FH, "$DEVICE_CHECKSUM_FILE") ||
		&x("Unable to open $DEVICE_CHECKSUM_FILE ($!)");
	chop($deviceChecksum = <DEVICE_CHECKSUM_FH>);
	close(DEVICE_CHECKSUM_FH);	
	&step("Aggregate checksum is $deviceChecksum");

	&logDate;
	&stop if $stop;
	@changedblocks = &cmpaggr($PAR2, $PAR3) if $cycle > 1;
	&logDate;
	if ($savelogs && ($cycle==1 || &logchanged(@changedblocks))) {
		&savelog($PAR2);
		&compress($PAR2, $dumpname) if $savelogandaggr;
		&logDate;
	}
	
	if ( $VERIFYBEFORERECOVERY != 0 ) {
# This step is misleading and should not be done by default
		&step("Verify with salvager first, to see if recovery makes a difference");
		&epi_verify($PAR3);
	}
	&step("Testing recovery");
	undef @faults;
	undef @mp;

#
# Call rcx_verify only if PAR1 has been unfrozen -> if neither 
# $singlestep_on_error nor $freeze_each_cycle is set.
#
	
	if (&epi_recover($PAR3)) {
		push(@faults, "recovery failed ($sysexit)");
		&step("Try salvaging un-recoverable aggregate");
		if ($runsalvage) {
			&logDate;
			if (&epi_salvage($PAR3)) {
				push(@faults, "Un-recoverable partition does not salvage");
			} else {
				&logDate;
				&step("Verify the salvaged aggregate");
				if ($code = &epi_verify($PAR3)) {
					push(@faults, sprintf("verification of salvaged aggregate failed (exit code 0x%x)", $code)); 
				} else {
					&logDate;
					&step("Change fileset ids in aggregate $PAR3 to avoid clash with mounted filesets from $PAR1");
					&changeFilesetIDs($PAR3);
	
					if ( (! $singlestep_on_error) && (!$freeze_each_cycle)) {
						&logDate;
						&step("Try mounting and checking the salvaged aggregate");
						$error = &rcx_verify($PAR3, $MNT3, "after salvage",
											 $kernelLogFile);
					} else {
						&step("Cannot checkout recovered filesets on $PAR2 as $PAR1 is frozen ");
					}
				}
			}
		}
	} else {
		if ($saveRecoveredAggrOnError) {
			&compress($PAR3, $dumpRecoveredAggrName);
		}
		if ($rcx_verify) {
			&step("User space recovery seems to have succeeded");
			&logDate;
			&step("Change fileset ids in aggregate $PAR3 to avoid clash with mounted filesets from $PAR1");
			&changeFilesetIDs($PAR3);
			&logDate;
			if ( (! $singlestep_on_error) && (!$freeze_each_cycle)) {
				&step("Check out each recovered fileset");
				$error = &rcx_verify($PAR3, $MNT3, "after user space recovery",
									 $kernelLogFile);
			} else {
				&step("Cannot checkout recovered filesets on $PAR2 as $PAR1 is frozen ");
			}
			if ($rcx_verify_kernel) {
				&step("Make another copy to test kernel recovery");
				&logDate;
				&dd($PAR2, $PAR3);
				&logDate;
				&step("Change fileset ids in aggregate $PAR3 to avoid clash with mounted filesets from $PAR1");
				&changeFilesetIDs($PAR3);			
				if ( (! $singlestep_on_error) && (!$freeze_each_cycle)) {
					$error = &rcx_verify($PAR3, $MNT3, "after kernel recovery",
										 $kernelLogFile);
				}
			} else {
				&step("Testing of kernel recovery not enabled");
			}
		}
	}
	if (@faults) {
		&step("Summary of faults for this test:\n");
		for $fault (@faults) {
			&step("    $fault\n");
		}
		if ($saveerrors) {
			&logDate;
			&step("Compressing a copy of the aggregate to $dumpname.Z");
			&compress($PAR2, $dumpname);
		}
		&logDate;
		&console("Test cycle $cycle completed, with errors");
		if ($maxstep > 1 && $singlestep_on_error) {
			&step("Switching to step size of one I/O in response to error");
			$maxstep = $minstep = 1;
		}
	} else {
		&logDate;
		# Delete saved recovered aggregate as validation successful.
		if ($saveRecoveredAggrOnError) {
			&step("Unlinking saved copy of recovered aggregate on success");
			unlink("$dumpRecoveredAggrName.Z");
		}
		&console("Test cycle $cycle completed successfully");
		push(@logs, $cycle) if $deletelogs;
	}

	#
	#	The following is v. confusing, so am commenting it out and
	#	replacing it more logical code.
	#
	#
	#	if (@pids) {
	#		&cont if $stop;
	#	} else {
	#		&mountvols($PAR1, $MNT1);
	#		&runprocs($MNT1);
	#	}
	#

	if ($norecover) {
		#
		# The processes would have been killed and @pid would be empty, and 
		# the volumes unmounted. Hence mount the volumes again and start the
		# the processes again. Also since the processes have been killed	 
		# the $stop variable is of no use here.
		#
		&mountvols($PAR1, $MNT1);
		&runprocs($MNT1,0);
	} else {
		#
		# If $stop is set, the processes were put to sleep immediately after 
		# the aggregate $PAR1 was frozen. Hence awaken them.
		#		
		&cont if $stop;
	}

	# delete excess logs if necessary.
	if (@logs > $deletelogs) {
		$oldlog = sprintf("$RUNLOGDIR/runlog.$logid.%03d", shift(@logs));
		&step("Unlinking old log $oldlog");
		unlink($oldlog);
	}
	
	if (($error == $LSTATERROR) && ($stopOnStatError)) {
		$exitNow = 1;
		&step("**********");
		&step(sprintf("**********  STOPPING TESTS AFTER CYCLE $logid.%03d",
					  $cycle));
		&step("**********");
		last;
	}
}

if ( $exitNow ) {
	# Exit without cleaning up
	chop($date = `date`);
    # Die immediately
    &x("Runrcx FAILED: CRASH EXIT at $date");
}

# This section cleans up and then exits.
&kill;

#
# Unmount filesets from $PAR1
#
&step("Unmounting partition $PAR1 filesets");
&unmountvols($PAR1, $MNT1);

# 
# Detach attached aggregates 
#
&step("Detaching aggregates");
&efts_detach($PAR1);

#if ( $SYSTYPE =~ /OSF1/i ) {
#	&closeaggrs($PAR1,$PAR2,$PAR3);
#	&closeexecfiles;
#	&closealldirs;
#}

# Close the handles on all the aggregates and copyaggr obtained in the 
# beginning.
&closeaggrs($PAR1,$PAR2,$PAR3);
&step("Close executable $COPYAGGR");
close(COPYAGGR);
&step("Close executable $CHECKSUM");
close(CHECKSUM);
	
chop($date = `date`);
&step("Runrcx, completed at $date");
exit 0;

#############################END OF MAIN######################################
##############################################################################

#
# Subroutines
#
sub changeFilesetIDs {
	local($aggr)=@_;
	&closeaggrs($PAR1,$PAR2,$PAR3);
	&sys("perl -I $PERLINC $PERLINC/chftids $PREPENDDEV$aggr; sync ");
	&openaggrs($PAR1,$PAR2,$PAR3);
	&sleep(30);
}

# Mount all volumes on specified partition, and check them out.
sub rcx_verify {
	return if !$rcx_verify;
	local($partition, $mntpoint, $message, $klogFilePrefix) = @_;
	local($code, $mp, @mp, $newvolid, $error, $errorFlag);
	local($iter);
	local($klogFile);

	$errorFlag = 0;
	for $iter (1..$NTHREADS) {
		$error = 0;
		# The fileset ids on the partition has been changed to avoid
		# clash with filesets, with same id, on other aggregates that 
		# are already mounted.
		$volid = $iter + $volidOffset;
		$newvolid = $volid | ( 1 << $newvolidshift);
		if ($code =
			   &epi_mount_for_changed_volid($partition,
							$mntpoint,
							$newvolid,
							$volid,
							$checkPhaseReadonlyFsetMount)) {
			push(@faults,
				 "mount failed, fileset# $iter ($volid) $newvolid ($sysexit)");
	        $error = 1;
		} else {
			$mp = "$MOUNTBASE/$mntpoint/rcx.$volid";
			push(@mp, $mp);
			$code =
				&sys("$RCX -r $RCX_SCRIPT $mp/rcx.test $BLOCKSIZE $FRAGSIZE");
			if ($code) {
				push(@faults,
					 "rcx -r failed $message, fileset ($volid) $newvolid ($sysexit)");
				if ($code == $LSTATERROR) {
					$error = $LSTATERROR;
				} else {
					$error = 1;
				}
			}
			&logDate;
		}
		if ($error) {
			$errorFlag++;
			$klogFile = sprintf("$klogFilePrefix.%03d", $iter);
			&dumpKernelLogs($klogFile);
		}
		if ($error == $LSTATERROR) {
			last;
		}
	}			

	# Only run the foll. robustness tests if the aggregate is in a known
    # consistent RCX state, if the filesets have not been mounted readonly.
	if ( ! $checkPhaseReadonlyFsetMount && !$errorFlag ) {	
		# Now try running the rc test for a while, to make sure that no errors
		# have been introduced (by, e.g. the salvager) which might cause
		# problems (e.g. a panic)
		if ($rcx_verify > 1) {
			&step("Try running rcx to test robustness of partition");
			local(@pids);
			# Check robustness
			&runprocs($mntpoint,1);
			&sleep(30);
			&kill;
		}
	}
	if ($error != $LSTATERROR) {
		if (@mp) {
			&epi_umount(@mp) && &x("Unmount failed for @mp");
		}
	}
	return $error;
}

sub check_robustness {
	local($par)=@_;
	local($mntpoint) = @_;
	local($pid,$echo);
	$echo = '-x' if $rcxecho;
	&step("Forking rcx processes");
	foreach $volid (1..$NTHREADS) {
		$volid += $volidOffset;
		if ($pid = fork) {
			push(@pids, $pid);
		} else {
			&exec("$RCX $echo $RCX_SCRIPT $MOUNTBASE/$mntpoint/rcx.$volid/rcx.test $BLOCKSIZE $FRAGSIZE");
		}
	}
}


# 'dd' one partition to another.
# Uses dd, simply because then we don't have to allocate humongous buffers.
sub dd {
	local($p1, $p2) = @_;
	local($i,$f1,$f2,$len,$writelen,$buf,$readoffset,$writeoffset,$ioblksize);

	if ( $SYSTYPE =~ /OSF1/i ) {
		&step1("[ dd partition $p1 to $p2 \n");
		$ioblksize=65536;
		
		$f1="/dev/r$p1";
		$f2="/dev/r$p2";
		open(P1,"$f1");
		open(P2,">$f2");
		
		$i=0;				
		$readoffset=0;		
		while($len = sysread(P1,$buf,$ioblksize)) {
			if ( !defined($len)) {
				&x("dd failed -  System read error: $!");
			}
			&step(sprintf(" Iteration %d:  read %d bytes at offset %d from %s",$i,$len,$readoffset,$f1)) if $debug; 
			
			$writeoffset=0;
			while ($len) {
				$writelen=syswrite(P2,$buf,$len,$writeoffset);
				if ( !defined($writelen)) {
					&x("dd failed - System write error: $!");
				}
				&step(sprintf(" 	wrote %d bytes (of %d bytes) at offset %d to %s",$writelen,$len,$readoffset+$writeoffset,$f2)) if $debug; 
				$len -=$writelen;
				$writeoffset += $writelen;
			}
			$i++;
			&step1("... \n");
		}
		close(P1);
		close(P2);
		&step1("... complete ] \n");
	} else {
		# Use raw devices on HPUX for speed since we have to use
		# whole disks, not logical volumes.
		$p1 =~ s#/dev/dsk/#/dev/rdsk/#;
		$p2 =~ s#/dev/dsk/#/dev/rdsk/#;
		&step("dd partition $p1 to $p2 ");
		&sys("dd if=$PREPENDDEV$p1 of=$PREPENDDEV$p2 bs=1024k") && &x("dd command failed");
	}
	&step(" dd of partition $p1 to $p2 complete");
}

# compress a partition
sub compress {
	local($part, $file) = @_;
	# Use raw device on HPUX for speed since we have to use whole disks,
	# not logical volumes
	$part =~ s#/dev/dsk/#/dev/rdsk/#;
	if (&sys("dd if=$PREPENDDEV$part | compress > $file.Z")) {
		&step("unable to compress $PREPENDDEV$part to $file.Z");
	    &step("will retry after 60 seconds");
		&sleep(60);
	    &sys("dd if=$PREPENDDEV$part | compress > $file.Z") && &x("unable to compress $PREPENDDEV$part to $file.Z. Giving up");
	}
}

# Find log blocks.
sub findlog {
	local($aggr) = @_;
	local($findlog) = "$RUNLOGDIR/findlog.$logid";
	local($block);
	&step("Saving log meta information in $findlog");
	&sys("$FINDLOG $PREPENDDEV$aggr $findlog");
	open(FINDLOG, $findlog) || &x("Can't read $findlog");
	chop(@logblocks = <FINDLOG>);
	close(FINDLOG);
	&x("Bad output from $FINDLOG") if ($logblocks[6] + 6 != $#logblocks);
	$maxblocknumber = $logblocks[5];
	splice(@logblocks, 0, 7);
	print("Logblocks are: @logblocks\n");
	# Note $logblocks{} is a different array from $logblocks[];
	for $block (@logblocks) {$logblocks{$block} = 1;}
}

# Decide whether the log has changed, based upon the list of
# changed blocks provided.
sub logchanged {
	local(@blocks) = @_;
	for $block(@blocks) {
		return 1 if ($logblocks{$block});
	}
	return 0;
}

# Save the episode log of an aggregate.
sub savelog {
	local($aggr) = @_;
	local($buf);
	$logdumpname = sprintf("$RUNLOGDIR/savelog.$logid.%03d", $cycle);
	$logdescname = sprintf("$RUNLOGDIR/findlog.$logid.%03d", $cycle);
	&step("Saving episode log from $aggr in $logdumpname");
	open(LOGAGGR, "$PREPENDDEV$aggr") || &x("Unable to read $PREPENDDEV$aggr");
	open(DUMPLOG, ">$logdumpname") || &x("Unable to create $logdumpname");
	open(LOGDESC, ">$logdescname") || &x("Unable to create $logdescname");
	local($nblocks) = 0 + @logblocks;
	print LOGDESC ("Herewith, three lines of comments:
		A bug a day...
				keeps the hackers well paid.
$logdumpname
$BLOCKSIZE
$maxblocknumber
$nblocks\n");
	for $block(0 .. $#logblocks) {
		print LOGDESC ("$block\n");
	}
	for $block(@logblocks) {
		seek(LOGAGGR, $block * $BLOCKSIZE, 0) || &x("seek failed on $aggr");
		sysread(LOGAGGR, $buf, $BLOCKSIZE) == $BLOCKSIZE
			|| &x("read failed on $aggr");
		syswrite(DUMPLOG, $buf, $BLOCKSIZE) == $BLOCKSIZE
			|| &x("write failed on $log");
	}
	close(LOGAGGR);
	close(DUMPLOG);
	close(LOGDESC);
}

# Compare two partitions; generate list of block numbers that are different.
# Assumes that the partition is a power of 2 in size.
# Returns the changed block numbers.
sub cmpaggr {
	local($par1, $par2) = @_;
	local($buf1, $buf2, $base, @blocks);
	&step("Look for block changes between $par1 and $par2");
	local(@offsets) = (0 .. $CMPSIZE/$BLOCKSIZE - 1);
	open(CMPAR1, "$PREPENDDEV$par1") || &x("Couldn't open $par1");
	open(CMPAR2, "$PREPENDDEV$par2") || &x("Couldn't open $par2");
	if ($SRC_PAR_NBLKS) {
		local($rem) = $SRC_PAR_BLKS * $BLOCKSIZE;
		while ($rem) {
			if ($CMPSIZE > $rem) {
				$CMPSIZE = $rem;
				@offsets = (0 .. $CMPSIZE/$BLOCKSIZE - 1);
			}
			&x("error reading $par1")
				if (!sysread(CMPAR1, $buf1, $CMPSIZE));
			&x("error reading $par2")
				if (!sysread(CMPAR2, $buf2, $CMPSIZE));
			for $offset (@offsets) {
				if (substr($buf1, $offset * $BLOCKSIZE, $BLOCKSIZE)
					ne substr($buf2, $offset * $BLOCKSIZE, $BLOCKSIZE)) {
					push(@blocks, $base + $offset);
				}
			}
			$rem -= $CMPSIZE;
			$base += $CMPSIZE/$BLOCKSIZE;
		}
	} else {
		while (sysread(CMPAR1, $buf1, $CMPSIZE)) {
			&x("error reading $par2") if !sysread(CMPAR2, $buf2, $CMPSIZE);
			if ($buf1 ne $buf2) {
				for $offset (@offsets) {
					if (substr($buf1, $offset * $BLOCKSIZE, $BLOCKSIZE)
						ne substr($buf2, $offset * $BLOCKSIZE, $BLOCKSIZE)) {
						push(@blocks, $base + $offset);
					}
				}
			}
			$base += $CMPSIZE/$BLOCKSIZE;
		}
	}
	print("Compare aggregate: blocks changed: @blocks\n") if @blocks;
	print("Compare aggregate: no blocks changed\n") if !@blocks;
	close(CMPAR1);
	close(CMPAR2);
	return (@blocks);
}

# Fork $NTHREAD's rcx processes on specified partition.
sub runprocs {
# If $robust is set, then create a new dir on that fileset and run the rcx ops
    local($mntpoint,$robust) = @_;
	local($pid,$echo,$dir);
	$echo = '-x' if $rcxecho;
	&step("Forking rcx processes");
	foreach $volid (1..$NTHREADS) {
		$volid += $volidOffset;
		if ($robust != 0 ) {
			$dir="$MOUNTBASE/$mntpoint/rcx.$volid/rcx.robusttest";
			mkdir($dir,0777) || 
				&x("unable to create dir $dir for robustness check for fileset $volid ($!)");
			# Create magic files in the robust dirs to prevent rcx from
			# complaining
			&create_magic_file("$dir")
		} else {
			$dir="$MOUNTBASE/$mntpoint/rcx.$volid/rcx.test";
		}
		if ($pid = fork) {
			push(@pids, $pid);
		} else {
			&exec("$RCX $echo $RCX_SCRIPT $dir $BLOCKSIZE $FRAGSIZE");
		}
	}
}

# Mount all filesets on specified partition and create test dirs and
# magic files in them and then unmount the filesets to ensure that the magic
# file actually is flushed to the disk always.  And the rcx should always find
# the magic file in both compilation and run modes and flag error otherwise.

sub createTestdirs {
	local($partition, $mntpoint) = @_;

	&step("Mount all filesets");
	&mountvols($partition, $mntpoint);

	&step("Creating test directories with magic files on each fileset");
	foreach $volid (1..$NTHREADS) {
		$volid += $volidOffset;
		&step("  for fileset rcx.$volid "); 
		$testdir = "$MOUNTBASE/$mntpoint/rcx.$volid/rcx.test";	
	    if (-d $testdir) {
    	    &checkdir($testdir);
	        &scandir($testdir, *remove);
    	}
	    mkdir($testdir, 0777) || &x("Unable to create test directory, $testdir ($!)");
		&create_magic_file("$testdir")
	}

	&step("Unmount all filesets");
	&unmountvols($partition, $mntpoint);

	&step("Sleep for 10 seconds");		
	sleep(10);
}

sub create_magic_file {
	local($dir)=@_;
	open(TEST, ">$dir/$MAGIC_FILE")
		|| &x("Unable to create magic file, $testdir/$MAGIC_FILE ($!)");
    close TEST;
}

# Mount NTHREADS volumes on the specified partition.
sub mountvols {
	local($partition, $mntpoint) = @_;
	&step("Mounting filesets on $partition");
	foreach $volid (1..$NTHREADS) {
		$volid += $volidOffset;
		if (&epi_mount($partition, $mntpoint, $volid)) {
			&x ("Couldn't mount fileset rcx.$volid on $partition ($sysexit)");
		}
	}
}

# Unmount NTHREADS volumes from the specified partition.
sub unmountvols {
	local($partition, $mntpoint) = @_;
	&step("Un-mounting filesets from $partition");
	foreach $volid (1..$NTHREADS) {
		$volid += $volidOffset;
		if (&epi_umount("$MOUNTBASE/$mntpoint/rcx.$volid")) {
			&x ("Couldn't unmount fileset rcx.$volid from $partition ($sysexit)");
		}
	}
}

# Stagger running processes in time (so they're not all doing the same thing at once)
sub stagger {
	return if @pids == 1;
	local(@savePids) = @pids;
	local(@p) = @pids;
	shift(@p);
	&step("Staggering process $pid");
	foreach $p (@p) {
		@pids = ($p);
		&stop;
		&sleep(1);
	}
	@pids = @savePids;
	&cont;
}

# Stop sub-processes
sub stop {
	local($str);	
	$str=join(" ",@pids);
	&step("Suspending (SIGSTOP) processes $str");
	kill ("STOP", @pids);
}	

# Continue sub-processes
sub cont {
	local($str);	
	$str=join(" ",@pids);
	&step("Continuing (SIGCONT) processes $str");
	kill ("CONT", @pids);
}

# Kill sub-processes
sub kill {
	local($pid,$str);
	$str=join(" ",@pids);
	&step("Kill (SIGKILL) processes $str");
	kill(9, @pids);
	while ($pid = shift(@pids)) {
		&step("Wait for process $pid");
		waitpid($pid, 0);		
	}
}

sub efts_detach {
	local(@aggr) = @_;
	local($silent);
	for  $aggr (@aggr) {
		$silent = 1, next if $aggr eq "silent";
		if (&sys("$EFTS detach -aggregate $aggr")) {  
			if ($silent) {
				&step("Ignoring aggregate detach error ($aggr).");
			} else {
				&x("couldn't detach aggregate $aggr");
			}
		}
	}
}

sub efts_attach {
	local(@aggr) = @_;
	local($silent);
	for $aggr (@aggr) {
		$silent = 1, next if $aggr eq "silent";
		if (&sys("$EFTS attach -device $aggr")) {  
			if ($silent) {
				&step("Ignoring aggregate attach error.");
			} else {
				&x("couldn't attach aggregate $aggr");
			}
		}
	}
}
	
sub efts_createvol {
	local($volid,$volname,$quota,$aggr)=@_;
	return &sys("$EFTS createft -aggregate $aggr -id $volid -name $volname -quota $quota");
}

sub epi_mount_for_changed_volid {
	local($partition, $mntpoint, $newvolid, $originalvolid, $readOnly) = @_;
    local($mntCmd) = "$MOUNT ";
    if ($readOnly) {
       $mntCmd .= "-readonly";
    }
	return &sys(
		"$mntCmd $PREPENDDEV$partition rcx.$originalvolid $MOUNTBASE/$mntpoint/rcx.$originalvolid");
}
 
sub epi_mount {
	local($partition, $mntpoint, $volid, $readOnly) = @_;
    local($mntCmd) = "$MOUNT ";
    if ($readOnly) {
       $mntCmd .= "-readonly";
    }
	return &sys(
		"$mntCmd $PREPENDDEV$partition rcx.$volid $MOUNTBASE/$mntpoint/rcx.$volid");
}

sub epi_umount {
	local(@mounts) = @_;
	local(@codes, $code);
	foreach $mount(@_) {
		if ($code = &sys($UMOUNT, $mount)) {
			&step("Try unmount again in 5 seconds, in case the error was EBUSY");
			&sleep(5);
			$code = &sys($UMOUNT, $mount);
		}
		push(@codes, $code) if ($code);
	}
	@codes;
}

#Freeze aggregate, copy it and unfreeze it all within one binary. 
#This routine has been created to get around the problem # of kernel
#deadlock due to various activity, on episode files on the # frozen
#aggregate, generated by the kernel outside the perview of DFS/Episode.
# Aim to keep the aggregate frozen for as short a time as possible.
sub copyaggr {
	local($source, $target, $checksumBinaryAbsPath, @optArgs) = @_;
	&sys("$COPYAGGR $source $target $checksumBinaryAbsPath @optArgs") &&
		&x("couldn't copy aggregate $source to $target");
}

# Freeze the specified aggregate (calls async debug utility, asdb)
sub epi_freeze {
	local($aggr) = @_;
	&sys("$ASDB -f $aggr") && &x("couldn't freeze aggregate $aggr");
}

# Unfreeze the specified aggregate.
# If first parameter is "silent", ignores errors.
sub epi_unfreeze {
	local(@aggr) = @_;
	local($silent);
	for $aggr (@aggr) {
		$silent = 1, next if $aggr eq "silent";
		if (&sys("$ASDB -u $aggr")) {
			if ($silent) {
				&step("Ignoring unfreeze error.");
			} else {
				&x("couldn't unfreeze aggregate $aggr");
			}
 		}
	}
}

# Step I/O by a particular amount on the specified aggregate.
sub epi_stepio {
	local($aggr, $minstep, $maxstep) = @_;
	local($count) = (rand($maxstep - $minstep + 1) | 0) + $minstep;
	if ($count) {
		&step("Freezing $aggr for write count of $count");
		&sys("$ASDB -f -c $count -w $aggr") && &x("couldn't freeze aggregate $aggr");
	} else {
		&epi_freeze($aggr);
	}
}

# Recover and verify.
sub epi_recover {
    local($aggr) = @_;
	local($code1) = &sys("$SALVAGE -recover -verify $aggr");
    local($code2);
    if ($verifyRecoveredAggrAgain) {
       &step("Re-verifying recovered aggregate $aggr");
       $code2 = &sys("$SALVAGE -verify $aggr");
       if ($code2) {
          &step("Re-verify of recovered aggregate $aggr failed with code $code2");
       }
    }
	return $code1 if $code1 == 0;
	&x(sprintf("Salvager returned unexpected error code 0x%x", $code1));
}

# Salvage.
sub epi_salvage {
	local($code) = &sys("$SALVAGE @_");
	return 0 if $code == 0 || $code == 3;
	if ($code & 0x4) {
		&step("**** RE-SALVAGING, since 0x4 bit returned by salvager ****");
		$code = &sys("$SALVAGE @_");
		return 0 if $code == 0 || $code == 3;
		&x("re-salvage loop") if $code == 0x4;
	}
	# Assume any other errors are salvager failures.
	return $code;
}

# Verify only.
sub epi_verify {
	return &sys("$SALVAGE -verify @_");	
}

# Same as system, except that it dies on error.
sub sys {
	local(@args) = @_;
    local($cmd, $code);
    
    if ($timeCommands) {
       $cmd = "time @args";
    } else {
       $cmd = "@args";
    }
	&step("execute $cmd");
    $code = system("$cmd");
	if ($code) {
		&x("Exec of \"$cmd\" failed ($code)") if $code & 0xFF;
		$code >>= 8;
		&step("Nonzero exit status ($code) for command \"@args\"\n") if $code;
		$sysexit = sprintf("exit status $code");
	}
	return $code;
}

# Same as exec, except it logs the exec.
sub exec {
	local(@args) = @_;
	&step("fork @args");
	exec(@args);
	&x("Exec of @args failed");
}

# Make a directory, if necessary. Aborts if it can't be made.
sub makedir {
	local($dir) = @_;
	if (! -d $dir) {
		mkdir($dir, 0777) || &x("couldn't make directory $dir ($!)");
	}
}

# Die
sub x {
	&kill;
	&sleep(10);
	&step("***Script died: @_***");
	open(STDERR, ">&CONSOLE");
	die("runrcx: @_\n");
}

# Sleep
sub sleep {
	local($sec) = @_;
	&step("Sleeping $sec seconds");
	&step(sprintf("Actually slept %d seconds", sleep($sec)));
}

# Print the supplied argument in an eye-catching format.
# Would this be this easier using formats?
sub stars {
	local($arg) = "  @_  ";
	local($len) = length($arg);
	local($prefix);
	$stars = "\n" . ("*" x 79) . "\n";
	$prefix = ((79 - $len)/2) | 0;
	$suffix = 79 - $len - $prefix;
	print($stars, '*' x $prefix, $arg, '*' x $suffix, $stars);
}

# Print script step
sub step {
	print("[@_]\n");
	print CONSOLE ("[@_]\n") if $log_on_console;
}

sub step1 {
	print("@_");
	print CONSOLE ("@_") if $log_on_console;
}


# Print message on user's console and in the log.
sub console {
	&step(@_);
	print CONSOLE "@_\n" if !$log_on_console;
}

sub logDate {
	local($date);
	chop($date=`date`);
	&step($date);
}

# Start logging on the specified file
# If the file is the same name, fsync it (on AFS this means that we'll be
# able to see it...)
sub log {
	return if $log_only_on_console;
    local($logname) = @_;
	&x("Log not initialized") if !$log_currentlogname && !$logname;
	$logname = $log_currentlogname if (!$logname);
	if ($logname ne $log_currentlogname) {
		&console("Now logging to $logname");
	}
	local($out) = ">$logname";
	if ($log_currentlogname) {
		close STDERR;
		close STDOUT;
		$out = ">>$logname"	if $log_currentlogname eq $logname;
	}
	open(STDOUT, $out) || &x("Couldn't open log file, $logname ($!)");
	open(STDERR, ">&STDOUT");
	select(STDERR); $| = 1; # unbuffered output.
	select(STDOUT); $| = 1;
	$log_currentlogname = $logname;
}

sub dump {
	local($source, $dump) = @_;
	open(DUMPSOURCE, $source) || &x("Can't open $source");
	open(DUMPFILE, ">$dump") || &x("Can't write $dump");
	while (<DUMPSOURCE>) {
		print (DUMPFILE $_);
	}
	close(DUMPSOURCE);
	close(DUMPFILE);
}

sub openaggrs {
	&step("Open all aggregate devices");	
	local($aggr1,$aggr2,$aggr3)=@_;
	&step("open block device for aggregate $aggr1");
	&openfile(AGGR1,"$PREPENDDEV$aggr1");
	&step("open block device for aggregate $aggr2");
	&openfile(AGGR2,"$PREPENDDEV$aggr2");
	&step("open block device for aggregate $aggr3");
	&openfile(AGGR3,"$PREPENDDEV$aggr3");
	&step("open raw device for aggregate $aggr1");
	if ( $aggr1 =~ s#/dev/dsk/#/dev/rdsk/# ) {
		&openfile(RAGGR1,"$aggr1");
	} elsif ( $aggr1 =~ m#/dev/r# ) {
		&openfile(RAGGR1,"$aggr1");
	} else {
		&openfile(RAGGR1,"${PREPENDDEV}r$aggr1");
	}
	&step("open raw device for aggregate $aggr2");
	if ( $aggr2 =~ s#/dev/dsk/#/dev/rdsk/# ) {
		&openfile(RAGGR2,"$aggr2");
	} elsif ( $aggr2 =~ m#/dev/r# ) {
		&openfile(RAGGR2,"$aggr2");
	} else {
		&openfile(RAGGR2,"${PREPENDDEV}r$aggr2");
	}
	&step("open raw device for aggregate $aggr3");
	if ( $aggr3 =~ s#/dev/dsk/#/dev/rdsk/# ) {
		&openfile(RAGGR3,"$aggr3");
	} elsif ( $aggr3 =~ m#/dev/r# ) {
		&openfile(RAGGR3,"$aggr3");
	} else {
		&openfile(RAGGR3,"${PREPENDDEV}r$aggr3");
	}
}

sub closeaggrs {
	local($aggr1,$aggr2,$aggr3)=@_;
    &step("close block device for aggregate $aggr1");
	close(AGGR1);
	&step("close block device for aggreagate $aggr2");
	close(AGGR2);
	&step("close block device for aggreagate $aggr3");
	close(AGGR3);
	&step("close raw device for aggreagate $aggr1");
	close(RAGGR1);
	&step("close raw device for aggreagate $aggr2");
	close(RAGGR2);
	&step("close raw device for aggreagate $aggr3");
	close(RAGGR3);
}


sub openexecfiles {
	&step("Open all executable files");
    $ASDBFILE=&which($ASDB);
    $DDFILE=&which(dd);

	$COMPRESS=&which(compress);
	$EFTSFILE=&which($EFTS);
	$NEWAGGRFILE=&which($NEWAGGR);
	$UMOUNTFILE=&which($UMOUNT);
	$MOUNTFILE=&which($MOUNT);
	$SALVAGEFILE=&which($SALVAGE);
	$FINDLOGFILE=&which($FINDLOG);
	$CALCLOGSIZEFILE=&which($CALCLOGSIZE);
	$RUNRCXFILE=&which(runrcx);
	$RCXFILE=&which(rcx);
    $PERLFILE=&which(perl);

	&step("open executable $COMPRESS"); 	
	&openfile(COMPRESS,$COMPRESS);
	&step("open executable $EFTSFILE"); 	
	&openfile(EFTSFILE,$EFTSFILE);
	&step("open executable $NEWAGGRFILE"); 	
	&openfile(NEWAGGRFILE,$NEWAGGRFILE);
	&step("open executable $UMOUNTFILE"); 	
	&openfile(UMOUNTFILE,$UMOUNTFILE);
	&step("open executable $MOUNTFILE"); 	
	&openfile(MOUNTFILE,$MOUNTFILE);

	&step("open executable $SALVAGEFILE"); 	
	&openfile(SALVAGEFILE,$SALVAGEFILE);
	&step("open executable $FINDLOGFILE"); 	
	&openfile(FINDLOGFILE,$FINDLOGFILE);

	&step("open executable $CALCLOGSIZEFILE"); 	
	&openfile(CALCLOGSIZEFILE,$CALCLOGSIZEFILE);
	&step("open executable $RUNRCXFILE"); 	
	&openfile(RUNRCXFILE,$RUNRCXFILE);

	&step("open executable $RCXFILE"); 	
	&openfile(RCXFILE,$RCXFILE);
	&step("open executable $PERLFILE"); 	
	&openfile(PERLFILE,$PERLFILE);

# The following definitely need to be opened
	&step("open executable $ASDBFILE"); 	
	&openfile(ASDBFILE,$ASDBFILE);
	&step("open executable $DDFILE"); 	
	&openfile(DDFILE,$DDFILE);

# Open for good measure to allow some basic debugging
	$WHICHFILE=&which(which);
	$PSFILE=&which(ps);
	$WHOFILE=&which(who);
	$LSFILE=&which(ls);	
	$KILLFILE=&which(kill);	

	&step("open executable $WHICHFILE"); 	
	&openfile(WHICHFILE,$WHICHFILE);
	&step("open executable $PSFILE"); 	
	&openfile(PSFILE,$PSFILE);
	&step("open executable $WHOFILE"); 	
	&openfile(WHOFILE,$WHOFILE);
	&step("open executable $LSFILE"); 	
	&openfile(LSFILE,$LSFILE);
	&step("open executable $KILLFILE"); 	
	&openfile(KILLFILE,$KILLFILE);

# Also open some shells
	$SHELLFILE=$ENV{'SHELL'};
	if ( $SHELLFILE ) {
		&step("open SHELL ($SHELLFILE) specified in the environment");
		&openfile(SHELLFILE,$SHELLFILE);
	}
	$SHFILE=&which(sh);
	&step("open bourne shell $SHFILE");
	&openfile(SHFILE,$SHFILE);
	$CSHFILE=&which(csh);
	&step("open cshell $CSHFILE");
    &openfile(CSHFILE,$CSHFILE);
}

sub closeexecfiles {
	&step("Close all executables");
	close(ASDBFILE);
	close(DDFILE);
# The following do not need to be opened , but to be over careful. 
	close(COMPRESS);
	close(EFTSFILE);
	close(NEWAGGRFILE);
	close(UMOUNTFILE);
	close(MOUNTFILE);
	close(SALVAGEFILE);
	close(FINDLOGFILE);
	close(CALCLOGSIZEFILE);
	close(RUNRCXFILE);
	close(RCXFILE);
    close(PERLFILE);

    close(WHICHFILE);
    close(PSFILE);
    close(WHOFILE);
    close(LSFILE);
    close(KILLFILE);
    close(SHELLFILE);
    close(SHFILE);
    close(CSHFILE);

}

sub openfile {
	local($filehandle,$file)=@_;
    local($result);

	# do neccessary setup to open vnodes for all ancestor dirs too
	&findDirs($file);
	#open file
	$result=open($filehandle,$file);
	if (!defined($result)) {
		&x("couldn't open file $file ($!)");
	}
}

# finds all dirs leading to the pathname and stores in an 
# associative array dirAssocList

sub findDirs {
    local($abspath)=@_;
    local($pos,$dir,$dotpos,$curdir,$tmppath,$origpath);

	# exit if null pathname
    if ( ! $abspath ) {
		&x("Null pathname given to subroutine findDirs");
    }
	
	# handle relative pathnames
    if ( index($abspath,"/") != 0 ) {
		$origpath=$abspath;
	    $curdir=`pwd`;
	    chop $curdir;
		if ( ( $dotpos = index($abspath,"./")) == 0 ) {
		    $tmppath=substr($abspath,$dotpos+2);
		} else {
			$tmppath=$abspath;
		}
	    $abspath="$curdir/$tmppath";
    }

    $pos=-1;
    while ( ( $pos = index("$abspath","/",$pos+1))  != -1 ) {
		$dir=substr($abspath,0,$pos+1);
	    if ($osfdebug) {
			&step("Path $abspath, ancestor $dir \n");
		}
		if ( !$dirAssocList{$dir} ) {
		    $dirAssocList{$dir}=$dir;
		}
    }
} 

sub findpathdirs {
	local($path,@path,$dir);
     &step("Determine all dirs in PATH environment variable");	
	$path=$ENV{'PATH'};
	@path=split(':',$path);

	foreach $dir (@path) {
	    &step("Analysing $dir");
	    &findDirs($dir);
	}
}

sub openalldirs {
#	local(@dirList,@dirSortList,$i,$dir,$dirfd);

    &step("Open all ancestor dirs of executables and devices");
	@dirList=values(%dirAssocList);
	@dirSortList=sort(@dirList);
	for ($i=0; $i <= $#dirSortList; $i++) {
    	$dirhandle[$i]="dh"."$i";
	    $dir=$dirSortList[$i];
	    &step("opening dir $dir");
# A bit of kludge to help debugging kernel deadlock on OSF/1 if
# debug flag is set. Vnodes can be tracked thru file descriptors.
		if ( ! $osfdebug ) {
		    if ( !opendir($dirhandle[$i],$dir) ) {
		 		&step("WARNING: Failed to opendir $dir ($!) \n");
			}	
		} else {
	 	    if ( !open($dirhandle[$i],$dir) ) {
		 		&step("WARNING: Failed to open $dir ($!) \n");
	    	} else {
				$dirfd=fileno($dirhandle[$i]);
				&step("File Desc $dir = $dirfd");
			}
		}
	}
}

sub closealldirs {
	local($dir);
	&step("Close all ancestor dirs");
	for ($i=0; $i <= $#dirHandle; $i++) {
		$dir=$dirSortList[$i];
	    &step("closing dir $dir");
		if ( ! $osfdebug ) {
		    if ( !closedir($dirhandle[$i])) {
		 		&step("WARNING: Failed to closedir $dir ($!) \n");
			}	
		} else {
			if ( !close($dirhandle[$i]) ) {
		 		&step("WARNING: Failed to close $dir ($!) \n");
			}
		}
	}
}

# Simulates the "which" csh builtin
sub which {
    local($file) = @_;
    local($abs);
    @PATH=split(":",$ENV{PATH});
    foreach $path (@PATH) {
	if ( -x "$path/$file" ) {
	    $abs = "$path/$file";
	    last;
	}
    }
    return $abs;
}

sub dumpKernelLogs {
    local($logfile) = @_;
    &sys("$DFSTRACE dump -file $logfile");
}

#
# The following routines are present in both rcx and runrx
#

# Routine that checks existence of magic file in the directory
sub checkdir {
    local($dir) = @_;
    &x("File $MAGIC_FILE is missing from directory $dir")
        if ! -f "$dir/$MAGIC_FILE";
}       

# Scan directory and do the supplied operation on each element,
# passing the relative pathname of the element to the function.
sub scandir {
    local($dir, *func) = @_;
    local(@names, $path, $entry, @paths, @entries, @dirs);
    $path = $dir;
    do {
        if (! -l $path && -d _) {
            push(@dirs, $path);
            opendir(DIR, $path) || &x("can't read directory $path ($!)");
            while (defined($entry = readdir(DIR))) {
                next if $entry eq '.' || $entry eq '..';
                push(@entries, $entry);
                push(@paths, $path? "$path/$entry" : $entry);
            }
        } else {
            &func($path, $entry);
        }
        $path = pop(@paths);
        $entry = pop(@entries);
    } while $path;
    for $dir (reverse @dirs) {
        &func($dir);
    }
}

# Remove file or directory. This routine is called to cleanup a 
# target tree. Does not remove the root of the tree itself.
sub remove {
    local($path, $entry) = @_;
    return if unlink($path);
    return if $entry eq '.';
    return if $entry eq '..';
    return if ! $path;
    &x("unable to remove $path") if -e $path && ! -d _;
    rmdir($path) || &x("unable to remove $path ($!)");
}

sub newaggr_spec {
    local($par, $blksize, $fragsize, $logoption) = @_;
    local($tmpfile) = "/tmp/runrcx_$$";
	local ($cmd) = "$NEWAGGR $par $blksize $fragsize $logoption -overwrite";
	local($code);

    open(SAVEERR, ">&STDERR");
    open(SAVEOUT, ">&STDOUT");	
    
    open(STDOUT, ">${tmpfile}");
    open(STDERR, ">&STDOUT");
    
    select(STDERR); $| = 1;
    select(STDOUT); $| = 1;


	$code = &sys("$cmd");
    
    close(STDOUT);
    close(STDERR);
    
    open(STDOUT, ">&SAVEOUT");
    open(STDERR, ">&SAVEERR");

    open(TMPFILE, "$tmpfile");
	if ($code) {
		while (<TMPFILE>) {
			print $_;
		}
		&x ("Could not create aggregate $par ($sysexit)");
	} else {
		while (<TMPFILE>) {
			if (/blocks: (\d*)$/) {
				$AGGR_NBLOCKS = $1;
			}
			# for logging
			print $_;
		}
	}
	
    unlink($tmpfile);
}


__END__

# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
