package main;

#
# Subroutines
#

# Assumes $TESTACL_SRCDIR is already set in the main test script.

# Sets 
# $testName
# $TESTACL_DEBUG
# $TESTACL_TESTDIR
# $TESTACL_LOGDIR 
# $TESTACL_SEC_SETUP
# $TESTACL_SEC_CLEANUP
# $EXECDIR 
# $TESTACL_GROUP_LIST 
# $TESTACL_GROUP_COUNT 
# $TESTACL_USER_LIST 
# $TESTACL_USER_COUNT 
# $TESTACL_GROUP_MEMBERS 
# $CELL_ADMIN_PASSWD 
# $COMMON_USER_PASSWD 
# $ALLPERMS
# $PATH_TYPE 

# common variables and setup done by all the tests below the top-level driver

sub commonSetup {
    local($testPath) = @_;
    local($code) = 0;

    if ($testPath =~ m#^.*/([^/]*)$#) {
	$testName = $1;
    } else {
	$testName = $testPath;
    }

    # The following 4 lines should be before the Log call.
    $TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};    

    require "$TESTACL_SRCDIR/scripts/common/util";
    require "$TESTACL_SRCDIR/scripts/common/sysUtil";
    require "$TESTACL_SRCDIR/scripts/common/aclUtil";
    require "$TESTACL_SRCDIR/scripts/common/rgyUtil";

    &unBufferFH(STDOUT);
    &unBufferFH(STDERR);

    &Log("CHECKPOINT: common test setup for test $testName starting");
    
    $TESTACL_DEBUG = $ENV{TESTACL_DEBUG};
    $TESTACL_TESTDIR = $ENV{TESTACL_TESTDIR};
    $TESTACL_LOGDIR = $ENV{TESTACL_LOGDIR};
    $TESTACL_SEC_SETUP = $ENV{TESTACL_SEC_SETUP};
    $TESTACL_SEC_CLEANUP = $ENV{TESTACL_SEC_CLEANUP};
    
    $PAREXECDIR = "$TESTACL_TESTDIR/scratch";
    $EXECDIR = "$PAREXECDIR/$testName";
    $SAVEDEXECDIR = $EXECDIR;
    $TESTACL_GROUP_LIST = $ENV{TESTACL_GROUP_LIST};
    $TESTACL_GROUP_COUNT = $ENV{TESTACL_GROUP_COUNT};
    $TESTACL_USER_LIST = $ENV{TESTACL_USER_LIST};
    $TESTACL_USER_COUNT = $ENV{TESTACL_USER_COUNT};
    $TESTACL_GROUP_MEMBERS = $ENV{TESTACL_GROUP_MEMBERS};
    
    $CELL_ADMIN_PASSWD = $ENV{CELL_ADMIN_PASSWD};
    $COMMON_USER_PASSWD = $ENV{COMMON_USER_PASSWD};
    
    $ALLPERMS = $ENV{ALLPERMS};

    $PATH_TYPE = $ENV{PATH_TYPE};
    
    $ENV{EXECDIR} = $EXECDIR;
    $ENV{PAREXECDIR} = $PAREXECDIR;

    if (-d $EXECDIR) {
	$code = &Sys("rm -rf $EXECDIR");
	if ($code) {
	    &ErrorExit("Unable to remove execution dir $EXECDIR");
	}
    } 
    &MakeDir($EXECDIR, 0777);
    &Chdir($EXECDIR);

    &Log("CHECKPOINT: common setup for $testName complete");
    $code;
}

sub commonCleanup {
    local($code);

    &Log("Start common cleanup for test $testName");
    &Chdir($PAREXECDIR);
    $code = &Sys("find $SAVEDEXECDIR -exec $ACL_EDIT {} -c -m user_obj:rwxcid group_obj:rwxcid other_obj:rwxcid \\;");
    &ErrorExit("Unable to cleanup execution dir $EXECDIR for deletion", $code)
	if ($code);
    $code = &Sys("rm -rf $SAVEDEXECDIR");
    &ErrorExit("Unable to delete execution dir $SAVEDEXECDIR", $code) if ($code);
    &Log("CHECKPOINT: common cleanup for test $testName complete");
    $code;
}

1;
