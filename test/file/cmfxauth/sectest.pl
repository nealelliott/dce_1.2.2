#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Test Driver
#
#   This script is the driver for the test suite described in detail in
#   "Test Plan for OSF-RFC 90.0: Security Enhancements for DCE DFS"

# USAGE: sectest.pl [-l | -i | -r] [test-list]
#
#     -l: run local-cell tests only (i.e. no remote cell required)
#
#     -i: initialize environment (for test development)
#     -r: restore environment (for test development)

# ----------------------------------------------------------------------------
# Some code cribbed from ..../src/test/file/acl

# main()
$| = 1;

# Determine source dir for test

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($0 =~ m#^/#) {
    # full path specified
    $0 =~ m#^(.*)/[^/]+$#;
    $SECTEST_SRCDIR = "/" . $1;
    $SECTEST_SRCDIR =~ s#/+#/#;
} else {
    # relative path specified
    chop($SECTEST_SRCDIR = `pwd`);

    if ($0 =~ m#^(.+/)[^/]+$#) {
	$relpath = $1;
	$relpath =~ s#^\./##;  # strip any leading "./"

	if ($relpath) {
	    $SECTEST_SRCDIR .= "/" . $relpath;
	    $SECTEST_SRCDIR =~ s#/$##;  # strip trailing "/"
	}
    }
}

if (!(-e "$SECTEST_SRCDIR/$myExec")) {
    print "\n$0: Unable to determine test source directory\n";
    exit(1);
}

$ENV{DFSXSECTEST_SRCDIR} = $SECTEST_SRCDIR;

unshift(@INC, "$SECTEST_SRCDIR");
require("secconfig.pl");
require("secTestInclude.pl");
require("secTestUtil.pl");


# Check that running as local root

chop($myLogin = `$WHOAMI`);

if ($myLogin ne $LCL_ROOT) {
    print "\n$0: Must be executed as local identity $LCL_ROOT\n";
    exit(1);
}

# Check that running as DCE cell_admin

@myCred = `$KLIST`;
if (grep(/$LCL_CELL_ADMIN/, @myCred) == 0) {
    print "\n$0: Must be executed as DCE principal $LCL_CELL_ADMIN\n";
    exit(1);
}

# Initialize some variables

$gotError = 0;

# Respond to any switches

if (@ARGV > 0) {
    if ($ARGV[0] eq "-l") {
	# tests are to be run in local-cell-only mode
	shift(@ARGV);
	$ENV{DFSXSECTEST_NOREMOTE} = "1";

    } elsif ($ARGV[0] eq "-i") {
	# do initialization and exit
	print "\n$myExec: Initialize test environment only.\n\n";
	if (!(&InitializeEnv())) {
	    print "\n$myExec: Initialization failed; restoring.\n\n";
	    &RestoreEnv();
	    exit(1);
	}
	exit(0);

    } elsif ($ARGV[0] eq "-r") {
	# do restore (uninitialize) and exit
	print "\n$myExec: Restore environment only.\n\n";
	&RestoreEnv(1);  # set force flag
	exit(0);
    }
}

# Determine which tests are to be run

@allTests = ("startup",
	     "setprot",
	     "repauth",
	     "flauth",
	     "revokeauth",
	     "bounds-un", "bounds-ur", "bounds-an", "bounds-ar");

if (@ARGV == 0) {
    @testsToRun = @allTests;
} else {
    @testsToRun = @ARGV;
}

# Perform common test initialization/configuration

print "\n---------------------------------------------------------\n";
print "\n+ $myExec: Begin test initialization.\n\n";
if (!(&InitializeEnv())) {
    print "\n---------------------------------------------------------\n";
    print "\n+ $myExec: Initialization failed; restoring environment.\n\n";
    &RestoreEnv();
    exit(1);
}

# Execute specified tests

foreach (@testsToRun) {
    local($test, $testExecCmd, $testLclUser, $testDceUser, $testDcePasswd,
	  $testDceAuthn);

    $test = $_;

    if (/startup/) {
	$testExecCmd   = "$SECTEST_SRCDIR/secTestStartup.pl";
	$testLclUser   = $LCL_USER;
	$testDceUser   = "";
	$testDcePasswd = "";
	$testDceAuthn  = 0;

    } elsif (/setprot/) {
	$testExecCmd   = "$SECTEST_SRCDIR/secTestSetProt.pl";
	$testLclUser   = $LCL_ROOT;
	$testDceUser   = $LCL_CELL_ADMIN;
	$testDcePasswd = $LCL_CELL_ADMIN_PASSWD;
	$testDceAuthn  = 1;

    } elsif (/repauth/) {
	$testExecCmd   = "$SECTEST_SRCDIR/secTestRepAuth.pl";
	$testLclUser   = $LCL_ROOT;
	$testDceUser   = $LCL_CELL_ADMIN;
	$testDcePasswd = $LCL_CELL_ADMIN_PASSWD;
	$testDceAuthn  = 1;

    } elsif (/flauth/) {
	$testExecCmd   = "$SECTEST_SRCDIR/secTestFlAuth.pl";
	$testLclUser   = $LCL_ROOT;
	$testDceUser   = $LCL_CELL_ADMIN;
	$testDcePasswd = $LCL_CELL_ADMIN_PASSWD;
	$testDceAuthn  = 1;

    } elsif (/revokeauth/) {
	$testExecCmd   = "$SECTEST_SRCDIR/secTestRevokeAuth.pl";
	$testLclUser   = $LCL_ROOT;
	$testDceUser   = $LCL_CELL_ADMIN;
	$testDcePasswd = $LCL_CELL_ADMIN_PASSWD;
	$testDceAuthn  = 1;

    } elsif (/bounds-un|bounds-ur|bounds-an|bounds-ar/) {
	$testExecCmd   = "$SECTEST_SRCDIR/secTestBounds.pl";
	$testLclUser   = $LCL_ROOT;
	$testDceUser   = $LCL_CELL_ADMIN;
	$testDcePasswd = $LCL_CELL_ADMIN_PASSWD;
	$testDceAuthn  = 1;

	if (/bounds-un/) {
	    $testExecCmd .= " UN";
	} elsif (/bounds-ur/) {
	    $testExecCmd .= " UR";
	} elsif (/bounds-an/) {
	    $testExecCmd .= " AN";
	} else {
	    $testExecCmd .= " AR";
	}

    } else {
	print "\n---------------------------------------------------------\n";
	print "\n+ $myExec: Ignoring unknown test $test\n\n";
	next;
    }

    print "\n---------------------------------------------------------\n";
    print "\n+ $myExec: Starting test $test\n\n";

    if (&ForkTestAuthn($myExec,
		       $testExecCmd,
		       $testLclUser,
		       $testDceUser, $testDcePasswd, $testDceAuthn)) {
	wait;

	if ($?) {
	    print "$myExec: Test $test exited with an error\n\n";
	    $gotError = 1;
	} else {
	    print "$myExec: Test $test completed without error\n\n";
	}
    } else {
	print "$myExec: Unable to fork test $test\n\n";
	$gotError = 1;
    }
}


# Restore environment

print "\n---------------------------------------------------------\n";
print "\n+ $myExec: Restoring environment.\n\n";
&RestoreEnv();

# Summarize test results

print "\n---------------------------------------------------------\n";
print "\n+ $myExec: Testing complete.\n\n";

if ($gotError) {
    print "\n+ $myExec: One or more tests resulted in an error\n";
    exit(1);
} else {
    print "\n+ $myExec: All tests completed without error\n";
    exit(0);
}


# ----------------------------------------------------------------------------


# InitializeEnv() - initialize test environment
#
#     Returns: 1 - successful; 0 - failed
#
sub InitializeEnv {

# Initialize global cleanup variables

    $cleanAHostFt  = 0;
    $cleanAHostRep = 0;
    $cleanBHostRep = 0;

    $cleanLclMountRW = 0;
    $cleanLclMountRO = 0;

# Check that remote file exists and is readable
# NOTE: really should be checking unauthenticated

    if (!defined($ENV{DFSXSECTEST_NOREMOTE})) {
	print "\n+ Checking that remote-file $RMT_RWFILE is readable.\n";
	((-e $RMT_RWFILE) && (-r $RMT_RWFILE)) || return 0;
    }

# Create local-cell filesets/files used by all tests

    umask(0);

    print "\n+ Creating local-cell filesets required for tests\n";
    &CheckFilesets() || return 0;
    &CreateFileset($LCL_FTNAME, $LCL_HOSTA, $LCL_HOSTA_AGGR) || return 0;
    $cleanAHostFt = 1;

    &MountFileset($LCL_MTPATH_RW, $LCL_FTNAME, 1) || return 0;
    $cleanLclMountRW = 1;

    &ChmodFileset(0777, $LCL_MTPATH_RW) || return 0;

    &ACLModify($LCL_MTPATH_RW, "-add {any_other rx}") || return 0;

    &SetRepFileset($LCL_FTNAME, "-sched") || return 0;

    &SetRepFileset($LCL_FTNAME,
		   "-minrepdelay ${INIT_MINREPDELAY}s " .
		   "-defaultsiteage ${INIT_DEFSITEAGE}s") || return 0;

    &AddSiteFileset($LCL_FTNAME, $LCL_HOSTA, $LCL_HOSTA_AGGR) || return 0;
    $cleanAHostRep = 1;

    &AddSiteFileset($LCL_FTNAME, $LCL_HOSTB, $LCL_HOSTB_AGGR) || return 0;
    $cleanBHostRep = 1;

    &MountFileset($LCL_MTPATH_RO, "$LCL_FTNAME.readonly", 0) || return 0;
    $cleanLclMountRO = 1;

    print "\n+ Sleeping $SLEEPTIME seconds to ensure sites are created\n";
    sleep($SLEEPTIME);

    print "\n+ Creating local-cell files required for tests\n";

    &CreateFile($LCL_RWFILE) || return 0;

    &ChmodFileset(0666, $LCL_RWFILE) || return 0;

    &ACLModify($LCL_RWFILE, "-add {any_other r}") || return 0;

    &UpdateFileset($LCL_FTNAME) || return 0;

    print "\n+ Sleeping $SLEEPTIME seconds to ensure replicas are updated\n";
    sleep($SLEEPTIME);

    1; # return result
}



# RestoreEnv() - restore environment to original state
#
sub RestoreEnv {
    local($force);

    if (@_ == 0) {
	$force = 0;
    } else {
	($force) = @_;
    }

    print "\n+ Removing files, filesets and mount-points created for tests.\n";

    if ($force || $cleanLclMountRO) {
	&UmountFileset($LCL_MTPATH_RO);
    }

    if ($force || $cleanBHostRep) {
	&RmSiteFileset($LCL_FTNAME, $LCL_HOSTB, $LCL_HOSTB_AGGR);
    }

    if ($force || $cleanAHostRep) {
	&RmSiteFileset($LCL_FTNAME, $LCL_HOSTA, $LCL_HOSTA_AGGR);
    }

    if ($force || $cleanLclMountRW) {
	&UmountFileset($LCL_MTPATH_RW);
    }

    if ($force || $cleanAHostFt) {
	&DeleteFileset($LCL_FTNAME, $LCL_HOSTA, $LCL_HOSTA_AGGR);
    }
}
