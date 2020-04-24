#!/afs/tr/proj/tools/bin/perl
# include in package main itself	

require "error.pl";
require "parse.pl";
require "fsutil.pl";
require "util.pl";
require "pctsBasic.pl";

$errors = 0 ;

$specFile=$ARGV[0];
&Log("Spec file is $specFile");
&ReadConfigFile($specFile);;

&GetAttr(LS_FSTYPE, SINGLE, REQUIRED, *fstype);
&GetAttr(LS_NAME, SINGLE, REQUIRED, *loadName);
&GetAttr(LS_INSTANCE, SINGLE, REQUIRED, *loadNumber);
&GetAttr(LS_LOGDIR, SINGLE, REQUIRED, *logDir);
&GetAttr(LS_DIRS, MULTIPLE, REQUIRED, *dirsWithQuota);
&GetAttr(LS_PCTS_DEVICES, MULTIPLE, REQUIRED, *pctsDevices);
&GetAttr(LS_PCTS_TESTLIST, SINGLE, REQUIRED, *testListFile);

&GetAttr(LS_DURATION, SINGLE, OPTIONAL, *duration);
&GetAttr(LS_CONCURRENCY, SINGLE, OPTIONAL, *concurrency);
&GetAttr(LS_PCTS_LIBROOT, SINGLE, OPTIONAL, *pctsLib);
&GetAttr(LS_PCTS_CTSROOT, SINGLE, OPTIONAL, *pctsCtsroot);


&Log("Starting pcts, instance $loadNumber on $fstype dirs");

# All default values are expected to be valid here. If they are not
# the validate function called earlier in the process should have generated 
# errors;
$quotaSpec = $pctsSpec{MIN_DIR_QUOTA};
foreach ( @dirsWithQuota ) {
    ($dir,$dirQuota) = split(' ');
    if ( $dirQuota >= $quotaSpec ) {
		$dirThreadCount{$dir} = int($dirQuota/$quotaSpec);
    }
}
$nThreads = $concurrency;
$threadRepCnt = $duration;
($pctsLib = $pctsSpec{DEF_PCTS_LIBROOT}) if !defined($pctsLib);
# Now $pctsLib points to the directory containing the LIB directory
# Change $pctsLib to point to LIB directory directly.
$pctsLib .= "/LIB";

($pctsCtsroot = $pctsSpec{DEF_PCTS_CTSROOT}) if !defined($pctsCtsroot);
@pctsTests = ("pcts/1003.1");
@scratchDevs = @pctsDevices;

# if CONCURRENCY specified, run only that many
if ( $nThreads ) {
    if ( $threadRepCnt ) {
		$threadRepCnt = &RoundUp($threadRepCnt / $nThreads);
# num threads = $nthread, num iters per thread = $threadRepCnt 
		&Log("RL: Load pcts, instance $loadNumber, $fstype dirs: num threads = $nThreads, num iterations per thread = $threadRepCnt");
    } else {
		$threadRepCnt=0;
# num threads = $nthreads, num iters per thread = forever
		&Log("RL: Load pcts, instance $loadNumber, $fstype dirs: num threads = $nThreads, num iterations per thread = forever");
    }
} else {
# num threads = as many as allowed by dir space
    $nThreads = 0;
    local( $sum ) = 0;
    grep($sum += $_, values(%dirThreadCount));
    if ( $threadRepCnt ) {
		$threadRepCnt = &RoundUp( $threadRepCnt / $sum );
# num threads = $sum, num iters per thread = $threadRepCnt 
		&Log("RL: Load pcts, instance $loadNumber, $fstype dirs: num threads = $sum (dir space dependent), num iterations per thread = $threadRepCnt");
    } else {
		$threadRepCnt=0;
# num threads = $sum, num iters per thread = forever
		&Log("RL: Load pcts, instance $loadNumber, $fstype dirs: num threads = $sum (dir space dependent), num iterations per thread = forever");
    }
}

$ENV{LIB} = "${pctsLib}";
$ENV{PATH} .= ":$ENV{LIB}/bin";
$ENV{CTSROOT} = $pctsCtsroot;
$ENV{CRASHSKIP} = "${pctsLib}/RScrashskip";
$ENV{EXEC_RESTRICT} = $testListFile;
# always set this 
$ENV{INTERACTIVE} = "FALSE";
$ENV{SILENT} = "TRUE";

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
		$threadDir="$dir/pcts.i${loadNumber}.${fstype}.t${threadIndex}";	
		&MakeDir($threadDir,0777);
		$threadLogDir = "${logDir}/pcts.i${loadNumber}.${fstype}.t${threadIndex}.posixlogs";
		&MakeDir($threadLogDir,0777);
		$threadLogFile = "${threadLogDir}/stdlog";
		$ENV{CTSLOGDIR} = $threadLogDir;
		$ENV{CTSTESTDIR} = $threadDir;
		$ENV{DEVICE1} = shift(@scratchDevs);
		$ENV{DEVICE2} = shift(@scratchDevs);
		
#Run pcts number $threadIndex+$_
		if ( $threadPid = fork() ) {
			&Log("RL: Forked load pcts, instance $loadNumber, $fstype dirs, thread $threadIndex (pid = $threadPid), log in $threadLogFile");
			$pctsThread{$threadPid}=join(' ',$threadIndex,$threadDir);
		} else {
			&SetLogFile($threadLogFile,0);
			&Sleep(10);
			&Chdir($threadDir);
			chop($pwd=`pwd`);
			&Log("THR: Load pcts, instance $loadNumber, $fstype dirs, thread $threadIndex starting in $pwd");
			&Log("THR: Main PCTS logs are in subdirs of $threadLogDir");
			if ($threadRepCnt) {
				&LogEnv;
				foreach $iter (0..$threadRepCnt-1) {
					&Log("THR: Starting pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter");
					foreach ( @pctsTests ) {
						&Log("THR: Starting pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter, test $_");
						&Sys("GO $_");
# pcts does not return non-zero values
#&& &Exit("Failure of pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter, test $_", __FILE__, __LINE__);
						&Log("THR: Completion of pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter, test $_");
					}
					&Log("THR: Completion of pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter");
				}
			} else {
				local($iter)=0;
				&LogEnv;
				while ( 1 ) {
					&Log("THR: Starting pcts instance $loadNumber, $fstype dirs, thread $threadIndex iteration $iter");
					foreach ( @pctsTests ) {
						&Log("THR: Starting pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter, test $_");
						&Sys("GO $_");
						&Log("THR: Completion of pcts instance $loadNumber, $fstype dirs, thread $threadIndex, iteration $iter, test $_");
					}
					&Log("THR: Completion of pcts instance $loadNumber, $fstype dirs, thread $threadIndex iteration $iter");
					$iter++;
				}
			}
			&Log("THR: Load pcts, instance $loadNumber, $fstype dirs, thread $threadIndex completed");		
			exit($EXIT_SUCCESS);				
		}
		$threadIndex++;
    }
}

# Wait for all threads.
&Log("Waiting for all pcts threads to complete");
while ( ($dead = wait) != -1 ) { 
    if ( !defined($pctsThread{$dead}) ) {
		&Log("RL: Internal error - completed pcts thread (pid $dead) not in pcts thread table, in file __FILE__ at  __LINE__");
    } else {
		($threadIndex, $threadDir)=split(' ',$pctsThread{$dead});
		if ( $? ) {
			$errors++;
			&Log("RL: Load pcts, instance $loadNumber, $fstype dirs, thread $threadIndex in dir $threadDir (pid $dead) failed, exit status $?");
# remove $dead from %pctsThread
			undef($pctsThread{$dead});
		} else {
			&Log("RL: Load pcts, instance $loadNumber, $fstype dirs, thread $threadIndex in dir $threadDir (pid $dead) passed");
		}
    }
}

$errors;

__END__
	
	
# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:
	
	
