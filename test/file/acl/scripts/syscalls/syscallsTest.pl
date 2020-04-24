#!/afs/tr/proj/tools/bin/perl

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};
require "$TESTACL_SRCDIR/scripts/common/testInit";
$code = &commonSetup($0);
&ErrorExit("ERROR: Unable to do common setup for test $testName") if ($code);

# Globals specific to this test
@ALLTESTSUITES = ("sysUtime", "sysRename", "sysUnlink", "sysChmod", 
		  "sysMkrmdir", "sysOpendir", "sysStat", "sysOpen", );

if ($ENV{TESTACL_SYSCALLS_TESTS}) {
    @TESTSUITES = split(' ', $ENV{TESTACL_SYSCALLS_TESTS});
} else {
    @TESTSUITES = @ALLTESTSUITES;
}

@TEST_DIRS = ("dir1", "dir2", "dir1/dir1_1", "dir2/dir2_1");
@TEST_FILES =("dir1/file1_2", "dir2/file2_2");

@DIR1_STAR = ("dir1", "dir1/dir1_1","dir1/file1_2");
@DIR2_STAR = ("dir2", "dir2/dir2_1", "dir2/file2_2");

# interesting combinations of permissions used by many tests
@NO_X_PERMS = ("rwidc", "rwic", "rwdc", "widc", "ridc", "rwc", "ric", "rdc",
	       "wic", "wdc", "idc", "rc", "wc", "ic", "dc", "c");
@NO_R_PERMS = ("xwidc", "xwic", "xwdc", "widc", "xidc", "xwc", "xic", "xdc",
	       "wic", "wdc", "idc", "xc", "wc", "ic", "dc", "c");
@NO_W_PERMS = ("rxidc", "rxic", "rxdc", "xidc", "ridc", "rxc", "ric", "rdc",
	       "xic", "xdc", "idc", "rc", "xc", "ic", "dc", "c");

# This may not be necessary
#export ALLPERMS NO_X_PERMS NO_R_PERMS NO_W_PERMS
#export EPERM EACCES

# interesting error codes used by all tests
$EPERM = 1;
$EACCES = 13;

&setup;
for $test (@TESTSUITES) {
    require "$TESTACL_SRCDIR/scripts/syscalls/$test";
    &Log("CHECKPOINT: syscalls sub-test $test beginning execution");
    # XXX Check this out

    $code =
	eval("\&${test}Run(\join(' ', \@DIR1_STAR), \join(' ', \@DIR2_STAR))");
    if (!defined($code)) {
	&ErrorExit("ERROR: Sub-test $test FAILED with error message \"$@\"");
    } elsif ($code) {
	&ErrorExit("ERROR: Sub-test $test FAILED with error code $code",
		   $code);
    }
    &Log("CHECKPOINT: syscalls sub-test $test execution completed successfully");
    &restoreAclState;
    &Log("CHECKPOINT: re-initialized state for next syscalls sub-test");
}

&Log("CHECKPOINT: syscalls test completed successfully");

$code = &commonCleanup;
&ErrorExit("ERROR: Unable to do common cleanup after test syscalls", $code)
    if ($code);

0;

#
# End of main section
#

#
# Subroutines
#

sub setup {
    &makeTestFilesAndDirs;
    &Log("CHECKPOINT: test syscalls local setup complete");
}

sub makeTestFilesAndDirs {
    local($code, @aclOut);
    local($success, $retCode);

    # treat this as the root dir. Setup default DACLs on it.
    $code = &aclEdit("-n", $EXECDIR, "-m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set object ACL on $EXECDIR", $code)
	if ($code);
    $code =
	&aclEdit("-n", $EXECDIR, "-io -m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set io ACL on $EXECDIR", $code) if ($code);
    $code =
	&aclEdit("-n", $EXECDIR, "-ic -m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set ic ACL on $EXECDIR", $code) if ($code);

    # Do we really need to set the ACLs below. No but we do not want to
    # rely on inheritance working.
    # setup the working tree. The files and directories inherit the default
    # DACLs of the parent

# if inheritance worked correctly, the first line following would work
#  mkdir $TEST_DIRS || exit 1

    &MakeDir("dir1");
    &MakeDir("dir2");    
    $code =
	&aclEdit("-n", "$EXECDIR/dir1", "-m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set object ACL on $EXECDIR/dir1", $code)
	if ($code);
    $code =
	&aclEdit("-n", "$EXECDIR/dir2", "-m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set object ACL on $EXECDIR/dir2", $code)
	if ($code);

    &MakeDir("dir1/dir1_1");
    &MakeDir("dir2/dir2_1");    
    $code =
	&aclEdit("-n", "$EXECDIR/dir1/dir1_1", "-m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set object ACL on $EXECDIR/dir1/dir1_1",
	       $code) if ($code);
    $code =
	&aclEdit("-n", "$EXECDIR/dir2/dir2_1", "-m user_obj:$ALLPERMS group_obj:- other_obj:-", *aclOut);
    &ErrorExit("ERROR: Unable to set object ACL on $EXECDIR/dir2/dir2_1",
	       $code) if ($code);

    foreach (@TEST_FILES) {
	$success = &createFile("$_", *retCode);
	if (!$success) {
	    &ErrorExit("ERROR: Unable to create file $_", $retCode);
	}
    }
}

sub restoreAclState {
    local($code, @aclOut);
    foreach $object ($EXECDIR, @TEST_DIRS, @TEST_FILES) {
	# XXX Does mask_obj really have to be set here.
	$code = &aclEdit("-n", $object, "-m user_obj:$ALLPERMS group_obj:- other_obj:- mask_obj:$ALLPERMS", *aclOut);
	&ErrorExit("ERROR: Unable to restore state of object ACL on $object",
		   $code) if $code;
    }
}
