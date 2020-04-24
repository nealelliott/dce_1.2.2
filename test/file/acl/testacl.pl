#!/afs/tr/proj/tools/bin/perl

# this script drives the whole testing effort.
# It resides under the top level directory tsuite or the current
# working directory. It should have the following directories 
# along side: utils scripts results scratch
#
# scripts contains the perl scripts that execute each test group.
# each of these test groups are orthogonal to each other and can be
# executed in parallel.
#
# results has subdirectories for each of the test groups. The subdirectories
# store the result of executions and any files that are created to store
# intermediate state.
#
# all the directories and the files created by the test scripts are 
# created in scratch.
#

$TESTACL_DEBUG = $ENV{TESTACL_DEBUG};

# Determine source dir
if ($0 =~ m#^(/.*)/[^/]*$#) {
    $TESTACL_SRCDIR = $1;
} else {
    $TESTACL_SRCDIR = `pwd`;
    chop($TESTACL_SRCDIR);
    if ($0 =~ m#^(.*)/[^/]*$#) {
	# Strip any leading "./"
	$match = $1 . "/";
	$match =~ s#^./##;
	if ($match) {
	    # Strip last "/"
	    $match =~ s#/$##;
	    $TESTACL_SRCDIR .= "/" . $match;
	}
    }
}
$ENV{TESTACL_SRCDIR} = $TESTACL_SRCDIR;

# Determine test dir
if (defined($ENV{TESTACL_TESTDIR})) {
    $TESTACL_TESTDIR = $ENV{TESTACL_TESTDIR};
} else {
    $TESTACL_TESTDIR = `pwd`;
    chop($TESTACL_TESTDIR);
    $ENV{TESTACL_TESTDIR} = $TESTACL_TESTDIR;
}

# Determine log dir
if (defined($ENV{TESTACL_LOGDIR})) {
    $TESTACL_LOGDIR = $ENV{TESTACL_LOGDIR};
} else {
    $TESTACL_LOGDIR = `pwd`;
    chop($TESTACL_LOGDIR);
    $TESTACL_LOGDIR .= "/results";
    $ENV{TESTACL_LOGDIR} = $TESTACL_LOGDIR;
}

# Handle setup/cleanup
# Default:  setup and cleanup of security accounts is done
if (@ARGV) {
    if ($ARGV[0] =~ /setuponly/) {
	$TESTACL_SEC_SETUP = "do";
	$ENV{TESTACL_SEC_SETUP} = $TESTACL_SEC_SETUP; 
	$TESTACL_SEC_SETUP_ONLY  = 1;
    } elsif ($ARGV[0] =~ /cleanuponly/) {
	$TESTACL_SEC_CLEANUP = "do";
	$ENV{TESTACL_SEC_CLEANUP} = $TESTACL_SEC_CLEANUP;
	$TESTACL_SEC_CLEANUP_ONLY  = 1;
    }
}

if (!$TESTACL_SEC_SETUP_ONLY && $ENV{TESTACL_SEC_SETUP_ONLY}) {
    $TESTACL_SEC_SETUP_ONLY = 1;
    $TESTACL_SEC_SETUP = "do";
} 

if (!$TESTACL_SEC_CLEANUP_ONLY && $ENV{TESTACL_SEC_CLEANUP_ONLY}) {
    $TESTACL_SEC_CLEANUP_ONLY = 1;
    $TESTACL_SEC_CLEANUP = "do";
} 

if (!$TESTACL_SEC_CLEANUP_ONLY && !$TESTACL_SEC_SETUP) {
    if (!defined($ENV{TESTACL_SEC_SETUP})) {
	$TESTACL_SEC_SETUP = "do";
	$ENV{TESTACL_SEC_SETUP} = $TESTACL_SEC_SETUP;
    } else {
	$TESTACL_SEC_SETUP = $ENV{TESTACL_SEC_SETUP};
    }
}

if (!$TESTACL_SEC_SETUP_ONLY && !$TESTACL_SEC_CLEANUP) {
    if (!defined($ENV{TESTACL_SEC_CLEANUP})) {
	$TESTACL_SEC_CLEANUP = "do";
	$ENV{TESTACL_SEC_CLEANUP} = $TESTACL_SEC_CLEANUP;
    } else {
	$TESTACL_SEC_CLEANUP = $ENV{TESTACL_SEC_CLEANUP};
    }
}

if ($TESTACL_SEC_SETUP_ONLY && $TESTACL_SEC_CLEANUP_ONLY) {
    &ErrorExit("Cannot specify only setup and only cleanup", 1);
}

$SETUP_SEC_ACCOUNTS = "$TESTACL_SRCDIR/scripts/common/setupSecAccounts";

require "$TESTACL_SRCDIR/scripts/common/util";
require "$TESTACL_SRCDIR/scripts/common/sysUtil";
require "$TESTACL_SRCDIR/scripts/common/aclUtil";
require "$TESTACL_SRCDIR/scripts/common/rgyUtil";

&unBufferFH(STDOUT);
&unBufferFH(STDERR);

# the users and groups we need, in list format
# $TESTACL_GROUP_LIST = "group_obj_group not_group_obj_group"}
# $TESTACL_USER_LIST = "user_obj_user group_obj_group 	\
#                        group_obj_user group_obj_group	\
#			 other_obj_user none		\
#			 user_user none 		\
#			 group_user not_group_obj_group"}
#
#  We have to keep the names of the users below eight characters for su to work
# for the local path tests.
#
# key to reading the short names:
#  all names prefixed with "acl"
#  user names end with "u"
#  group names end with "g"
#  [ugo]o => {user,group,other}_obj
#  [ug] => {user,group}  (generic user/group)
#  ngog => not group_obj group
#  nog => no other group (a group that does not match any groups in the ACL).
#  
#  Users/groups
# acluou [aclgog]
# aclgou [aclgog]
# acluu [aclnog]
# aclgu [aclngog]
# acloou [aclnog]
#

$TESTACL_GROUP_LIST = "aclnog aclgog aclngog";
$ENV{TESTACL_GROUP_LIST} = $TESTACL_GROUP_LIST;

$TESTACL_GROUP_COUNT = &calcNumEntriesInList($TESTACL_GROUP_LIST);
$ENV{TESTACL_GROUP_COUNT} = $TESTACL_GROUP_COUNT;

$TESTACL_USER_LIST = "acluou aclgog \
                      aclgou aclgog \
                      acloou aclnog \
                      acluu aclnog  \
                      aclgu aclngog";
$ENV{TESTACL_USER_LIST} = $TESTACL_USER_LIST;

$TESTACL_USER_COUNT = &calcNumEntriesInList($TESTACL_USER_LIST)/2;
$ENV{TESTACL_USER_COUNT} = $TESTACL_USER_COUNT;

$TESTACL_GROUP_MEMBERS = "acloou,acluu acluou,aclgou aclgu";
$ENV{TESTACL_GROUP_MEMBERS} = $TESTACL_GROUP_MEMBERS;

# interesting passwords
if (!defined($ENV{CELL_ADMIN_PASSWD})) {
    $CELL_ADMIN_PASSWD = "-dce-";
    $ENV{CELL_ADMIN_PASSWD} = $CELL_ADMIN_PASSWD; ;
} else {
    $CELL_ADMIN_PASSWD = $ENV{CELL_ADMIN_PASSWD};
}
if (!defined($ENV{COMMON_USER_PASSWD})) {
    $COMMON_USER_PASSWD = "-dce-";
    $ENV{COMMON_USER_PASSWD} = $COMMON_USER_PASSWD;
} else {
    $COMMON_USER_PASSWD = $ENV{COMMON_USER_PASSWD};
}

$ALLPERMS = "rwxcid";
$ENV{ALLPERMS} = $ALLPERMS;

$PATH_TYPE = "localpath";
if ($TESTACL_TESTDIR =~ m#^/\.\.\./.*#) {
    $PATH_TYPE = "cmpath";    
}
$ENV{PATH_TYPE} = $PATH_TYPE;

# Create log dir if it does not exist yet.
&MakeDir($TESTACL_LOGDIR);

# Log file creation and initialization
$LOGFILE = $TESTACL_LOGDIR . "/testaclLOG";

# Save handle to stdout. Used by subroutine Log.
open(CONSOLE, ">&STDOUT");
$code = &SetLogFile($LOGFILE, CONSOLE);
die("$0 - Unable to initialize log file $LOGFILE") if ($code);

&Log("Beginning DACL tests.");
&LogEnv;

if ($TESTACL_SEC_CLEANUP_ONLY) {
    &Log("Only cleanup of the security accounts specified");
    $code = &cleanupSecAccounts();
    if ($code) {
	&ErrorExit("Unable to clean up the security accounts", $code);
    } else {
	&Log("CHECKPOINT: Completed cleanup of security accounts, exiting");
	exit(0);
    }
}

# Setup the security accounts if so specified
if ($TESTACL_SEC_SETUP ne "skip") {
    &Log("Setting up the security accounts");
    $code = &setupSecAccounts;
    &ErrorExit("Unable to setup the security accounts") if ($code);
    if ($TESTACL_SEC_SETUP_ONLY) {
	&Log("Only setup of security accounts specified, done, exiting ");
	exit(0);
    }
    &Log("CHECKPOINT: Completed setup of security accounts");
} else {
    &Log("Setup of security accounts prevented by tester. Assuming already setup");
}

&MakeDir("$TESTACL_TESTDIR/scratch");
&Log("CHECKPOINT: common test suite setup complete");

@allTests = ("getset", "chmodtest", "inherit", "accheck", "syscalls",
	     "sizetest");

if (@ARGV == 0) {
    @testsToRun = @allTests;
} else {
    @testsToRun = @ARGV;
}

# start the tests. 
&Log("CHECKPOINT: starting the functionality tests");

foreach (@testsToRun) {
    local($test, $testExecCmd, $testLogFile, $testUser, $testUserPasswd);
    if (/syscalls/) {
	$test = "syscalls";
	$testExecCmd = "$TESTACL_SRCDIR/scripts/syscalls/syscallsTest";
	$testLogFile = "$TESTACL_LOGDIR/$test" . "LOG";
	$testUser= "acluou"; 
	$testUserPasswd = $COMMON_USER_PASSWD;
	&Log("CHECKPOINT: starting test: syscalls to verify interaction with system calls");
    } elsif (/sizetest/) {
	$test = "sizetest";
	$testExecCmd = "$TESTACL_SRCDIR/scripts/misc/sizeTest";
	$testLogFile = "$TESTACL_LOGDIR/$test" . "LOG";
	$testUser= "acluou";
	$testUserPasswd = $COMMON_USER_PASSWD;
	&Log("CHECKPOINT: starting test: sizetest to verify growing and shrinking DACLs");
    } elsif (/accheck/) {
	$test = "accheck";
	$testExecCmd = "$TESTACL_SRCDIR/scripts/accheck/accheckTest";
	$testLogFile = "$TESTACL_LOGDIR/$test" . "LOG";
	$testUser= "cell_admin";
	$testUserPasswd = $CELL_ADMIN_PASSWD;
	&Log("CHECKPOINT: starting test: accheck to verify access check algorithm");
    } elsif (/inherit/) {
	$test = "inherit";
	$testExecCmd = "$TESTACL_SRCDIR/scripts/misc/inheritTest";
	$testLogFile = "$TESTACL_LOGDIR/$test" . "LOG";
	$testUser= "acluou";
	$testUserPasswd = $COMMON_USER_PASSWD;
	&Log("CHECKPOINT: starting test: inherit to verify inheritance");
    } elsif (/getset/) {
	$test = "getset";
	$testExecCmd = "$TESTACL_SRCDIR/scripts/misc/getsetTest";
	$testLogFile = "$TESTACL_LOGDIR/$test" . "LOG";
	$testUser= "acluou";
	$testUserPasswd = $COMMON_USER_PASSWD;
	&Log("CHECKPOINT: starting test: getset to verify DACL setting and reading");
    } elsif (/chmodtest/) {
	$test = "chmodtest";
	$testExecCmd = "$TESTACL_SRCDIR/scripts/misc/chmodTest";
	$testLogFile = "$TESTACL_LOGDIR/$test" . "LOG";
	$testUser= "acluou";
	$testUserPasswd = $COMMON_USER_PASSWD;
	&Log("CHECKPOINT: starting test: chmodtest to verify that mode bits are in sync with DACL changes");
    } else {
	&Log("Ignored unknown test $_ specified");
	next;
    }
    if (!$TESTACL_DEBUG) {
	$pid = &forkTest($test, $testExecCmd, $testLogFile, $testUser,
			 $testUserPasswd);
	$test{$pid} = $test;
    } else {
	$code = &Sys("perl -d $testExecCmd");
	if (!$code) {
	    &Log("CHECKPOINT: Test $test passed");
	} else {
	    &ErrorExit("ERROR: test $test, cmd $testExecCmd failed with error $code");
	}
    }
}

&Log("CHECKPOINT: waiting for requested tests to complete");
&Log("Look at files $TESTACL_LOGDIR/<testname>LOG to monitor progress");

$fail = 0;
while (($pid = wait) != -1) {
    $failure = $?;
    $test = $test{$pid};
    if ($failure) {
	&Log(sprintf("ERROR: Test $test, pid $pid FAILED, exit status %#x",
		     $?));
	$fail++;
    } else {
	&Log("CHECKPOINT: Test $test passed");	
    }
    undef($test{$pid});
}

if ($fail == 0) {
    &Log("All DACL tests succeeded");
    if ($TESTACL_SEC_CLEANUP ne "skip") {
	# Cleanup the registry if specified
	&Log("Cleaning up the security accounts");
	$code = &cleanupSecAccounts();
	&ErrorExit("Unable to clean up the security accounts") if ($code);
	&Log("CHECKPOINT: Completed cleanup of security accounts");
    } else {
	&Log("Cleanup of security accounts prevented by tester");
    }
} else {
    &Log("$fail DACL tests FAILED");
    &Log("Punting on cleaning up security accounts");
}

&Log("Exiting DACL test, Ciao");
exit $fail;

#
# End of main section
#

#
# Subroutines
#

# return child process id
sub forkTest {
    local($test, $testExecCmd, $testLog, $testUser, $testUserPasswd) = @_;
    local($pid);

    if ($pid = fork()) {
	&Log("Forked test $test, pid = $pid, log is $testLog");
	return $pid;
    } else {
	&SetLogFile($testLog, 0);
	&Log("CHECKPOINT: Test $test started");
	&Sleep(10);
	&Exec("$DCE_LOGIN $testUser $testUserPasswd -exec $testExecCmd");
	# Never reach here unless exec error;
	&ErrorExit("Test $test execution FAILED", $!);
    }
}

sub setupSecAccounts {
    local($code) = 0;
    local($prog);

    $ENV{TESTACL_SEC_DATABASE_CMD} = "setup";
    if ($TESTACL_DEBUG) {
	$prog = "perl -d $SETUP_SEC_ACCOUNTS";
    } else {
	$prog = $SETUP_SEC_ACCOUNTS;
    }
    $code = &Sys("$DCE_LOGIN cell_admin $CELL_ADMIN_PASSWD -exec $prog");
    &ErrorExit("Unable to setup the security accounts", $code) if ($code);
    $code;
}

sub cleanupSecAccounts {
    local($code) = 0;
    local($prog);

    $ENV{TESTACL_SEC_DATABASE_CMD} = "cleanup";
    if ($TESTACL_DEBUG) {
	$prog = "perl -d $SETUP_SEC_ACCOUNTS";
    } else {
	$prog = $SETUP_SEC_ACCOUNTS;
    }
    $code = &Sys("$DCE_LOGIN cell_admin $CELL_ADMIN_PASSWD -exec $prog");
    &ErrorExit("Unable to cleanup the security accounts", $code) if ($code);
    $code;
}

sub calcNumEntriesInList {
    local($list) = @_;
    local(@list);
    local($cnt);

    @list = split(' ', $list);
    $cnt = @list;
    $cnt;
}
