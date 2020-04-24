#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Test Driver
#
#   This script is the driver for the test suite described in detail in
#   "Test Plan for OSF-RFC 77.0: Supporting Multi-homed DFS Servers".

# ----------------------------------------------------------------------------
# Some code cribbed from ..../src/test/file/acl

# main()

$| = 1;  # turn off print buffering for the benefit of logging

# Determine source dir for test and specify required files

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($0 =~ m#^/#) {
    # full path specified
    $0 =~ m#^(.*)/[^/]+$#;
    $MHSTEST_SRCDIR = "/" . $1;
    $MHSTEST_SRCDIR =~ s#/+#/#;
} else {
    # relative path specified
    chop($MHSTEST_SRCDIR = `pwd`);

    if ($0 =~ m#^(.+/)[^/]+$#) {
	$relpath = $1;
	$relpath =~ s#^\./##;  # strip any leading "./"

	if ($relpath) {
	    $MHSTEST_SRCDIR .= "/" . $relpath;
	    $MHSTEST_SRCDIR =~ s#/$##;  # strip trailing "/"
	}
    }
}

if (!(-e "$MHSTEST_SRCDIR/$myExec")) {
    print "\n$0: Unable to determine test source directory\n";
    exit(1);
}

unshift(@INC, "$MHSTEST_SRCDIR");
require("mhsconfig.pl");
require("mhsTestInclude.pl");
require("mhsTestInit.pl");
require("mhsTestUtil.pl");
require("mhsTestMSBind.pl");
require("mhsTestSSBind.pl");
require("mhsTestCDSAddr.pl");
require("mhsmach.pl");

# Check that running as root

chop($myLogin = `$WHOAMI`);

if ($myLogin ne $LCL_ROOT) {
    print "\n$0: Must be executed as local identity $LCL_ROOT\n";
    exit(1);
}

# Check that running as cell_admin

@myCred = `$KLIST`;
if (grep(/$CELL_ADMIN/, @myCred) == 0) {
    print "\n$0: Must be executed as DCE principal $CELL_ADMIN\n";
    exit(1);
}

# Determine which tests are to be run

@allTests = ("roaccess", "rwaccess", "sstatus", "ftloc");

if (@ARGV == 0) {
    @testsToRun = @allTests;
} else {
    @testsToRun = @ARGV;
}

# Perform common test initialization/configuration

print "\n---------------------------------------------------------\n";
print "\n+ $myExec: Begin test initialization.\n\n";
&InitializeEnv();

# Execute specified tests

foreach (@testsToRun) {
    local($test) = $_;

    if (/roaccess/) {
	print "\n---------------------------------------------------------\n";
	print "\n+ $myExec: Starting test $test\n\n";
	&ROFilesetAccess_Test();

    } elsif (/rwaccess/) {
	print "\n---------------------------------------------------------\n";
	print "\n+ $myExec: Starting test $test\n\n";
	&RWFilesetAccess_Test();

    } elsif (/sstatus/) {
	print "\n---------------------------------------------------------\n";
	print "\n+ $myExec: Starting test $test\n\n";
	&SStatusCheck_Test();

    } elsif (/ftloc/) {
	print "\n---------------------------------------------------------\n";
	print "\n+ $myExec: Starting test $test\n\n";
	&FilesetLocation_Test();

    } else {
	print "\n---------------------------------------------------------\n";
	print "\n+ $myExec: Ignoring unknown test $test\n\n";
	next;
    }
}

# Restore environment

print "\n---------------------------------------------------------\n";
print "\n+ $myExec: Restoring environment.\n\n";
&RestoreEnv();

# Done

print "\n---------------------------------------------------------\n";
print "\n+ $myExec: Testing complete.\n\n";

exit(0);
