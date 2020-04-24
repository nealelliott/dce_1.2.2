#!/afs/tr/proj/tools/bin/perl

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};

require "$TESTACL_SRCDIR/scripts/common/util";
require "$TESTACL_SRCDIR/scripts/common/sysUtil";
require "$TESTACL_SRCDIR/scripts/common/aclUtil";
require "$TESTACL_SRCDIR/scripts/common/rgyUtil";

$TESTACL_RESET_STRING = $ENV{TESTACL_RESET_STRING};

&unBufferFH(STDOUT);
&unBufferFH(STDERR);

if (defined($ENV{TESTACL_TEST_FILES})) {
    $TESTACL_TEST_FILES = $ENV{TESTACL_TEST_FILES};
    @files = split(' ', $TESTACL_TEST_FILES);
    foreach $file (@files) {
	$success = &createFile($file, *retCode);
	if (!$success) {
	    &ErrorExit("ERROR: Unable to create file $file", $retCode);
	}
    }
    foreach $file (@files) {
	$code = &aclEdit("", $file, "$TESTACL_RESET_STRING", *aclOut);
	&ErrorExit("ERROR: Unable to set object acl $TESTACL_RESET_STRING on $file", $code) if ($code);
    }
}

if (defined($ENV{TESTACL_TEST_DIRS})) {
    $TESTACL_TEST_DIRS = $ENV{TESTACL_TEST_DIRS};
    @dirs = split(' ', $TESTACL_TEST_DIRS);
    foreach $dir (@dirs) {
	&MakeDir($dir);
    }
    foreach $dir (@dirs) {
	$code = &aclEdit("", $dir, "$TESTACL_RESET_STRING", *aclOut);
	&ErrorExit("ERROR: Unable to set object acl $TESTACL_RESET_STRING on $dir", $code) if ($code);
    }
}

0;

