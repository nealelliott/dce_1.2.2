#!/afs/tr/proj/tools/bin/perl

# Check authorization mechanisms
$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};

require "$TESTACL_SRCDIR/scripts/common/testInit";

$code = &commonSetup("accheck");
&ErrorExit("ERROR: Unable to do common setup for test $testName") if ($code);

# Source in comm.pl and typeLen.pl modules only after subroutine 
# commonSetup has been called.
require "$TESTACL_SRCDIR/scripts/common/typeLen";
require "$TESTACL_SRCDIR/scripts/common/comm";

# Globals specific to this test

# the different scripts that will masquerade as shells
$ACCHECK = "$TESTACL_SRCDIR/scripts/accheck/accheckChild";
$MAKETESTFILES = "$TESTACL_SRCDIR/scripts/accheck/makeTestFiles";

@files = (file1);
$ENV{TESTACL_TEST_FILES} = join(' ', @files);

# XXX Check this out. The original sh tests disabled directory based ACL access
# tests. Its been kept that way for now. When we enhance coverage of these 
# tests to directories, we need to disable this.
# add testdir1 later
# @dirs = (testdir1)
@dirs = ();
$ENV{TESTACL_TEST_DIRS} = join(' ', @dirs);

$TESTACL_ALLPERMS = $ALLPERMS;

# different combinations of permission bits
@ENTRY_PERMS = ("rc", "wc", "xc", "ci", "cd", "c", "wci", "wcd");
@MASK_PERMS = ("rc", "wc", "xc", "ci", "cd", "wci", "wcd");

@testsToRun = ("user_obj", 
	       "group_obj", 
	       "other_obj",
	       "user",
	       "group");

&setup;

@entriesToModify = ("user_obj",
		    "group_obj",
		    "other_obj",
		    "user:$USER_USER",
		    "group:$NOT_GROUP_OBJ_GROUP",
		    "group:$GROUP_OBJ_GROUP"
		    );

# the different types of tests we can run
%testIds = ("user_obj",  "$USER_OBJ_USER:$GROUP_OBJ_GROUP",
	    "group_obj", "$GROUP_OBJ_USER:$GROUP_OBJ_GROUP",
	    "other_obj", "$OTHER_OBJ_USER:$NO_GROUP_GROUP",
	    "user",      "$USER_USER:$NO_GROUP_GROUP",
	    "group",     "$GROUP_USER:$NOT_GROUP_OBJ_GROUP");

@pipeFHs = (C1P1R, C1P1W, C1P2R, C1P2W, 
	    C2P1R, C2P1W, C2P2R, C2P2W,
	    C3P1R, C3P1W, C3P2R, C3P2W,
	    C4P1R, C4P1W, C4P2R, C4P2W,
	    C5P1R, C5P1W, C5P2R, C5P2W);
#
#  This is the "main" section
#

&Log("CHECKPOINT: Starting to create different identities threads");

if (!$ENV{TESTACL_DEBUG}) {
    $childCnt = @testsToRun;
} else {
    $childCnt = 1; 
    $COMMON_USER_PASSWD = "-dce-";
}

for ($i = 0; $i < $childCnt; $i++) {
    foreach $j (0, 1) {
	($rd, $wr) = ($i * 4 + $j * 2, $i * 4 + $j * 2 + 1);
	if (!pipe($pipeFHs[$rd], $pipeFHs[$wr])) { 
	    &ErrorExit("ERROR: unable to create pipe $j for child $i ($!)");
	}
	&unBufferFH($pipeFHs[$wr]);
    }
}

for ($i = 0; $i < $childCnt; $i++) {
    $currentTest = $testsToRun[$i];
    ($testUser, $testGroup) = split(':', $testIds{$currentTest});

    $childRFh = $pipeFHs[$i * 4];
    $childWFh = $pipeFHs[$i * 4 + 3];
    $childRFd = fileno($childRFh);
    $childWFd = fileno($childWFh);

    $childLogFile = "$TESTACL_LOGDIR/accheck_$currentTest" . "LOG"; 
    $testCategory[$i] = $currentTest;
    &Log("Forking child $i to perform $currentTest category access checks");
    $pid = fork();
    if (!$pid) {
       	&SetLogFile($childLogFile, 0);
	# Close unnecessary pipes
	close($pipeFHs[$i * 4 + 1]);
	close($pipeFHs[$i * 4 + 2]);
	# Also close all other pipes
	foreach $i (0..$i - 1, $i + 1 .. $childCnt - 1) {
	    close($pipeFHs[$i * 4]);
	    close($pipeFHs[$i * 4 + 1]);
	    close($pipeFHs[$i * 4 + 2]);
	    close($pipeFHs[$i * 4 + 3]);
	}

	$ENV{TESTACL_DRIVER_TEST} = $currentTest;
	$ENV{TESTACL_DRIVER_USER} = $testUser;
	$ENV{TESTACL_DRIVER_GROUP} = $testGroup;
	$ENV{TESTACL_DRIVER_INDEX} = $i;
	$ENV{TESTACL_DRIVER_CNT} = $childCnt;
	$ENV{TESTACL_DRIVER_READFD} = $childRFd;
	$ENV{TESTACL_DRIVER_WRITEFD} = $childWFd;
	&Log("CHECKPOINT: Accheck test child $i, pid $$ starting");

	if ($ENV{TESTACL_DEBUG}) {
	    $prog = "perl -d $ACCHECK";
	} else {
	    $prog = $ACCHECK;
	}
	
	&Log("Acquire desired identity of $testUser");
	if ($PATH_TYPE eq "localpath") {
	    &Exec("su $testUser -c $DCE_LOGIN $testUser $COMMON_USER_PASSWD -e $prog");
	    # XXX Need some way of communicating this error to parent.
	    # Signals ??
	    &ErrorExit("ERROR: (local) Test $testType failed with exit code $code", $code);
	} else {
	    &Exec("$DCE_LOGIN $testUser $COMMON_USER_PASSWD -e $prog");
	    &ErrorExit("ERROR: (cm) Test $testType failed with exit code $code", $code);
	}
    } 
    push(@pids, $pid);
    #parent
    &Log("Forked child $i, $currentTest, pid $pid, log in $childLogFile");
}

&Log("CHECKPOINT: Forked all necessary identity processes");

# Parent should close read on pipe 1 and write on pipe2 for each child
for ($i = 0; $i < $childCnt; $i++) {
    close($pipeFHs[$i * 4]);
    close($pipeFHs[$i * 4 + 3]);
}

# Form array of file descriptors for reads ends of the pipe 2 of each child 
# from which the parent reads. Also form the file descriptor bitset for
# select simultaneously.
$rin = '';
undef(@parRdFds);
for ($i = 0; $i < $childCnt; $i++) {
    $fd = fileno($pipeFHs[$i * 4 + 2]);
#    print "Child $i fileno is $fd\n";
    $parRdFds[$i] = $fd;
    vec($rin, $fd, 1) = 1;
#    printf("Updated select bit vector to %s for child $i, fileno $fd\n", 
#	   unpack("b*", $rin));
}
#printf("Bit vector for select is %s\n", unpack("b*", $rin));

$end = 0;
maintest:
while (!$end) { 
    $code = 0;
    $end = 1;

    # XXX The original sh tests did not have access checks for ACLs w/o
    # mask_obj. They need to be added.
    # Check access without mask_obj.

    # Check access with mask_obj
    &Log("CHECKPOINT: Starting access checks with mask_obj entry");    
    #Check access with various values of mask_obj
    foreach $mask (@MASK_PERMS) {
	&Log("CHECKPOINT: Setting mask_obj to $mask");
	foreach $object (@files, @dirs) {		
	    $code = &changeAclEntry($object, "mask_obj", $mask);
	    last maintest if ($code);
	} # object
	foreach $entry (@entriesToModify) {
	    foreach $permVar (@ENTRY_PERMS) {
	        $label = "$entry" . ":" . "$permVar";
		&Log("CHECKPOINT: Starting mask_obj access tests for $label");

		# Insert a file per child in each directory to enable each 
		# child to test delete permission access.
		foreach $dir (@dirs) {
		    &Log("Temporarily changing ACL to enable creation of deletion files");
		    $code = &getAcl($dir, "o", 0, *dirAcl);
		    if ($code) {
			&Log("ERROR: Unable to get object ACL for $dir");
			last maintest;
		    }
		    $otherObjEntry = &getAssocAclEntryPermStr("other_obj",
							      %acl);
		    $code = &changeAclEntry($dir, "other_obj", "rwxi");
		    if ($code) {
			&Log("ERROR: Could not change other_obj entry to allow creation of delete files");
			last maintest;
		    }
		    foreach $i (0..$childCnt) {
			$file = "$dir"."/deleteFile."."$i";
			if (!-e $file) {
			    $code = &createFile($file, *error);
			    if (!$result) {
				&Log("ERROR: Could not create delete file $file ($error)");
				last maintest;
			    }
			}
		    }
		    $code = &changeAclEntry($dir, "other_obj", $otherObjEntry);
		    if ($code) {
			&Log("ERROR: Could not reset other_obj entry, changed to allow creation of delete files, back");
			last maintest;
		    }
		}

		foreach $object (@files, @dirs) {		
		    $code = &changeAclEntry($object, $entry, $permVar);
		    last maintest if ($code);
		} # object
		$code = &signalProbeProcesses($childCnt, $rin, $label);
		if ($code) {
		    &Log("ERROR: in mask_obj access tests for $label, exiting");
		    last maintest;
		}
		&Log("CHECKPOINT: Completed mask_obj access tests for $label");
	    } #permVar
	    # Reset ACLs on files and dirs to a known state
	    &Log("Reset ACLs");
	    foreach $object (@files, @dirs) {
		$code = &changeAclEntry($object, $entry, "c");
		if ($code) {
		    &Log("ERROR: Unable to reset $object ACL entry $entry to \"c");
		    last maintest;
		}
	    }
	} #entry
    } #mask
} #end

if ($code) {
    &Log("ACCHECK Test FAILED");
    &Log("Will skip test cleanup due to test failure");
} else {
    &Log("ACCHECK Test completed successfully");
}
$skipCleanup = $code;
$code = &killChildren;
if (!$code && !$skipCleanup) {
    $code = &privateCleanup();
    if (!$code) {
	&commonCleanup;
    }
}

if ($skipCleanup) {
    exit($skipCleanup);
} else {
    exit($code);
}

#
# End of main section
#

#
# Subroutines
#
sub setup {
    $USER_OBJ_USER = "acluou";
    $ENV{USER_OBJ_USER} = $USER_OBJ_USER;

    $GROUP_OBJ_USER = "aclgou";
    $OTHER_OBJ_USER =  "acloou";
    $USER_USER = "acluu";
    $GROUP_USER  = "aclgu";
    $GROUP_OBJ_GROUP = "aclgog";
    $NOT_GROUP_OBJ_GROUP = "aclngog";
    $NO_GROUP_GROUP = "aclnog";

    $TESTACL_RESET_STRING = "-s user_obj:c group_obj:c other_obj:c mask_obj:$TESTACL_ALLPERMS user:$USER_USER:c group:$NOT_GROUP_OBJ_GROUP:c group:$GROUP_OBJ_GROUP:c";
    $ENV{TESTACL_RESET_STRING} = $TESTACL_RESET_STRING;

    &makeTestFiles;

    # setup complete
    &Log("CHECKPOINT: local setup complete");
}

# This routine makes sure the secd database has been setup appropriately 
# and sets up the user and group variables based on it.
sub selectTestUsersAndGroups {
    
    $USER_OBJ_USER = "acluou";
    $ENV{USER_OBJ_USER} = $USER_OBJ_USER;

    # XXX Export these into the environment
    $GROUP_OBJ_USER = "aclgou";
    $OTHER_OBJ_USER =  "acloou";
    $USER_USER = "acluu";
    $GROUP_USER  = "aclgu";
    $GROUP_OBJ_GROUP = "aclgog";
    $NOT_GROUP_OBJ_GROUP = "aclngog";
}

sub makeTestFiles {
    local($prog);

    &Log("Create all files and directories for the access checks");
    if ($ENV{TESTACL_DEBUG}) {
	$prog = "perl -d $MAKETESTFILES";
    } else {
	$prog = $MAKETESTFILES;
    }
    # Let $USER_OBJ_USER be owner for all files. This is sufficient
    # for the existing tests cases.

    $ENV{TESTACL_TEST_FILES_OWNER} = $USER_OBJ_USER;
    $ENV{TESTACL_TEST_FILES_OWNINGGROUP} = $GROUP_OBJ_GROUP;

    if ($PATH_TYPE eq "localpath") {
	$code = &Sys("su acluou -c $DCE_LOGIN $USER_OBJ_USER $COMMON_USER_PASSWD -e $prog");
	if ($code) {
	    &ErrorExit("ERROR: Could not create test files (local) via $MAKETESTFILES, exit code $code") ;
	}
    } else {
        $code = &Sys("$DCE_LOGIN $USER_OBJ_USER $COMMON_USER_PASSWD -e $prog");
	if ($code) {
	    &ErrorExit("ERROR: Could not create test files (cm) via $MAKETESTFILES, exit code $code") ;
	}
    }
}

# Cleanup all dirs
# Accessses globals @files, @dirs
sub privateCleanup {
    local($warnings, $code, $error);

    &Log("Do accheck tests specific cleanup");
    $warnings = 0;
    foreach $dir (@dirs) {
	while (<"${dir}/insertFile*">) {
	    &unlinkObject($_, *error);
	}
	while (<"${dir}/deleteFile*">) {
	    &unlinkObject($_, *error);
	}
	&removeDir($dir, *error);
    }
    foreach $file (@files) {
	&unlinkObject($file, *error);
    }
    $warnings;
}

sub signalProbeProcesses {
    local($childCnt, $rin, $signalStr) = @_;
    local($inProgressChildCnt, $nFound, $nFdRead, $rout, $code);

    $code = 0;
    &Log("Inform all children to start access checks");
    for ($i = 0; $i < $childCnt; $i++) {
	$code = &pipeWriteStr($pipeFHs[$i * 4 + 1], $signalStr);
	if ($code) {
	    &Log(sprintf("ERROR $code writing to child $child's pipe %d"),
		 $i * 4 + 1);
	    return $code;
	}
    }

    $inProgressChildCnt = $childCnt;    

doTestMainLoop:    
    while ($inProgressChildCnt) {
	$nFound = select($rout = $rin, undef, undef, 60);
	#$nFound = select($rout = $rin, undef, undef, undef);
#	printf("nFound is %d, rin is %s, rout is %s \n", $nFound,
#	       unpack("b*", $rin), unpack("b*", $rout));
	
	if ($nFound) {
	    $nFdRead = 0;
	    for ($i = 0; $i < $childCnt; $i++) {
		if (vec($rout, $parRdFds[$i], 1)) {
		    $nFdRead++;
		    $code = &processChild($i);
		    $inProgressChildCnt--;
		    if ($code) {
			&Log("ERROR from child $i for case $signalStr");
			last doTestMainLoop;
		    }	
		}
	    }
	    if ($nFdRead != $nFound)  {
		&Log("select return value ($nFound) mismatch with ready read fds ($nFdRead)");
	    }
	}
    }
    $code;
}

sub changeAclEntry {
    local($object, $entry, $perm) = @_;
    local($code);

    $code = &aclEdit("-n", $object, "-m $entry:$perm",*aclOut);
    &Log("ERROR: unable to set entry $entry:$perm in object acl of $object") if ($code);
    $code;
}

sub processChild {
    local($childIndex) = @_;
    local($code, $childVal);

    $code = 0;
    $code = &pipeReadInt($pipeFHs[$childIndex * 4 + 2], *childVal);
    if ($code) {
	&Log(sprintf("ERROR reading from child $childIndex pipe %d",
		     $childIndex * 4 + 2));
    } else {
	if ($childVal != 0) {
	    &Log("ERROR failure ($childVal) indicated by child $childIndex\n");
	    $code = 1;
	} else {
	    &Log(sprintf("Success of child $childIndex, %s",
			 $testCategory[$childIndex]));
	}
    }
    $code;
}

sub killChildren {
    &Log("Killing all children");
    foreach (@pids) {
	$code = kill('INT', $_);
	if ($code != 1) {
	    &Log("WARNING unable to kill SIGINT pid $_. Ignore this error if read from child's pipe indicated dead child");
	    return $code;
	}
    }
    &Log("Waiting for all children to die");
    while (wait != -1) {
	#null
    }; 
    &Log("All children have exited");
}

