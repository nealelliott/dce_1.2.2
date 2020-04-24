#!/afs/tr/proj/tools/bin/perl

# Should cover both files and dirs.
# ACL no mask_obj
#     chacl -> chmod
#     chmod -> chacl
# 
# ACL mask_obj
#     chacl group_obj !-> chmod UNIX group
#     chmod UNIX gropup !-> chacl group_obj
# 
#   * chacl -> chmod
#   * chmod -> chacl
# 
# NOTE: Only the last 2 cases marked with * is covered currently by this 
# test.

# verify that changing DACLs changes the mode bits properly & vice-versa

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};
require "$TESTACL_SRCDIR/scripts/common/testInit";
$code = &commonSetup($0);
&ErrorExit("ERROR: Unable to do common setup for test $testName") if ($code);

# Globals specific to this test

# we modify the execution directory in this case
$EXECDIR = "$EXECDIR/testdir2";

# the set below implies -rwxrw-r-- masked by r-x
$UO_PERM = "rwxcid";
$GO_PERM = "rw";
$OO_PERM = "r";
$U_PERM = "rxd";
$G_PERM = "rwi";
$M_PERM = "rx";

# -rwxr-x-w- masked by -wx: -rwx-wx-w-: mode : 0732
$UO_PERMI = "rwxcid";
$GO_PERMI = "rx";
$OO_PERMI = "w";
$U_PERMI = "rwxid";
$G_PERMI = "rx";
$M_PERMI = "wx";
$FMODE = 0732;

# -r--rwx-x- masked by r--: -r--r---x-: mode: 0441
$UO_PERMD = "rcid";
$GO_PERMD = "rwx";
$OO_PERMD = "x";
$U_PERMD = "id";
$G_PERMD = "c";
$M_PERMD = "r";
$DMODE = 0441;

# Globals set in selectTestUsersAndGroups
# $GEN_USER 
# $GEN_GROUP

# Global temporaries
# @aclOut
# %acl
# $permStr

&setup;
&saveAclsAndModes;
# mode bits and DACL computed modes bits have been made in sync. at this point
# for file1.

# Now DACLs are altered and mode bits monitored for being in lockstep.

foreach $object ("file1", "dir1") {
    #  change the acl on the object and make sure the mode tracks
    &Log("CHECKPOINT: Change ACL on $object and ensure mode tracks");
    &chaclTest($object);
    #  now, chmod the object and make sure the acl tracks    
    &Log("CHECKPOINT: Change mode on $object and ensure ACL tracks");
    &chmodTest($object); 
} # object

# XXX This needs fixing as ACL already has mask_obj by inheritance. 
# Now make sure the chmod test works with an acl that doesn't have a mask_obj
# Create new test file and dir
$success = &createFile("file2", *retCode);
if (!$success) {
    &ErrorExit("ERROR: Unable to create file2", $retCode);
}
&MakeDir("dir2");
foreach $object ("file2", "dir2") {
    &chmodTest($object);
} #object

&Log("CHECKPOINT: CHMODTEST test completed successfully");

$code = &commonCleanup;
&ErrorExit("ERROR: Unable to do common cleanup after test CHMODTEST", $code)
    if ($code);

0;

#
# End of main section
#

#
# Subroutines
#

sub setup {
    &selectTestUsersAndGroups;

    # create the test directory and set the acls to a known, initial state
    &MakeDir($EXECDIR);
    $code = &aclEdit("-n", $EXECDIR, "-m mask_obj:$M_PERM user_obj:$UO_PERM group_obj:$GO_PERM other_obj:$OO_PERM user:$GEN_USER:$U_PERM group:$GEN_GROUP:$G_PERM", *aclOut);
    &ErrorExit("ERROR: Unable to set o acl on $EXECDIR", $code) if ($code);
    $code = &aclEdit("-n", $EXECDIR, "-io -m mask_obj:$M_PERMI	user_obj:$UO_PERMI group_obj:$GO_PERMI other_obj:$OO_PERMI user:$GEN_USER:$U_PERMI group:$GEN_GROUP:$G_PERMI", *aclOut);
    &ErrorExit("ERROR: Unable to set io acl on $EXECDIR", $code) if ($code);  
    $code = &aclEdit("-n", $EXECDIR, "-ic -m mask_obj:$M_PERMD	user_obj:$UO_PERMD group_obj:$GO_PERMD other_obj:$OO_PERMD user:$GEN_USER:$U_PERMD group:$GEN_GROUP:$G_PERMD", *aclOut);
    &ErrorExit("ERROR: Unable to set ic acl on $EXECDIR", $code) if ($code);

    &Chdir($EXECDIR);
    &Log("CHECKPOINT: CHMODTEST local setup complete");
}

sub selectTestUsersAndGroups {
    $GEN_USER = "acluu";
    $GEN_GROUP = "aclngog";
}

sub saveAclsAndModes {
    local($aclModeOctalStr, $modeOctalStr);
    local($success, $retCode);

    # create test files and directories
    $success = &createFile("file1", *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file1", $retCode);
    }

    &Chmod("file1", $FMODE);
    &MakeDir("dir1");
    &Chmod("dir1", $DMODE);

    &sameAclModeUnixMode("file1", *modeOctalStr, *aclModeOctalStr) || 
	&ErrorExit("ERROR: after chmod on file file1, acl:$aclModeOctalStr differs from mode:$modeOctalStr");
    &sameAclModeUnixMode("dir1", *modeOctalStr, *aclModeOctalStr) || 
	&ErrorExit("ERROR: after chmod on dir dir1, acl mode:$aclModeOctalStr differs from mode:$modeOctalStr");    
    &Log("CHECKPOINT: Acls and modes agree after initialization");
}

# Return 1 if same, 0 if different
# Reference parameter $modeOctalStr and $aclModeOctalStr contain octal
# strings representing the access bits i.e. "0644"
sub sameAclModeUnixMode {
    local($object, *modeOctalStr, *aclModeOctalStr) = @_;
    local(%acl);
    local($code, $result);
    local($mode, $aclMode);

    $code = &getAcl($object, "o", 1, *acl);
    &ErrorExit("ERROR: Unable to read object ACL of $object", $code)
	if ($code);    
    $aclMode = &getModeFromAcl(%acl);
    $mode = &getMode($object);
    $modeOctalStr = sprintf("0%o", $mode);
    $aclModeOctalStr = sprintf("0%o", $aclMode);
    $result = ($mode == $aclMode);
    $result;
}

# XXX Do we always want mask_obj test ? What if acl has no mask_obj case?
#  chaclTest parameters:
#	$1: file or directory name
sub chaclTest {
    local($object) = @_;
    local($code, @aclOut);
    local($modeOctalStr, $aclModeOctalStr); 

    # modify the user_obj     
    $code = &aclEdit("-n", $object, "-m user_obj:rc", *aclOut);
    &ErrorExit("ERROR: Unable to modify user_obj entry on object ACL of $object") if ($code);
    &sameAclModeUnixMode($object, *modeOctalStr, *aclModeOctalStr) || 
	&ErrorExit("ERROR: after modifying user_obj in object ACL of $object, acl mode:$aclModeOctalStr differs from mode:$modeOctalStr");    	
    &Log("CHECKPOINT: acl and mode agree on $object after user_obj modification");

    # now, modify the group_obj bits
    $code = &aclEdit("-n", $object, "-m group_obj:r", *aclOut);
    &ErrorExit("ERROR: Unable to modify group_obj entry on object ACL of $object") if ($code);
    &sameAclModeUnixMode($object, *modeOctalStr, *aclModeOctalStr) || 
	&ErrorExit("ERROR: after modifying group_obj in object ACL of $object, acl mode:$aclModeOctalStr differs from mode:$modeOctalStr");    	
    &Log("CHECKPOINT: acl and mode agree on $object after group_obj modification");
    
    # now, do the same for the other_obj bits
    $code = &aclEdit("-n", $object, "-m other_obj:r", *aclOut);
    &ErrorExit("ERROR: Unable to modify other_obj entry on object ACL of $object") if ($code);
    &sameAclModeUnixMode($object, *modeOctalStr, *aclModeOctalStr) || 
	&ErrorExit("ERROR: after modifying other_obj in object ACL of $object, acl mode:$aclModeOctalStr differs from mode:$modeOctalStr");    	
    &Log("CHECKPOINT: acl and mode agree on $object after other_obj modification");

    # now, the mask_obj
    $code = &aclEdit("-n", $object, "-m mask_obj:r", *aclOut);
    &ErrorExit("ERROR: Unable to modify mask_obj entry on object ACL of $object") if ($code);
    &sameAclModeUnixMode($object, *modeOctalStr, *aclModeOctalStr) || 
	&ErrorExit("ERROR: after modifying mask_obj in object ACL of $object, acl mode:$aclModeOctalStr differs from mode:$modeOctalStr");    	
    &Log("CHECKPOINT: acl and mode agree on $object after mask_obj modification");
}

#  chmod_test parameters:
#	$1: file or directory name
sub chmodTest {
    local($object) = @_;
    local($code, @aclOut);
    local($modeOctalStr, $aclModeOctalStr);
    local($m, $mOctalStr);
    
    foreach $u (0, 2, 4, 6) {
	foreach $g (1, 3, 5, 7) {
	    foreach $o (0, 3, 6, 7) {
		$m = ($u << 6) | ($g << 3) | ($o);
		$mOctalStr = sprintf("0%o", $m);
		&Chmod($object, $m);
		&sameAclModeUnixMode($object,
				     *modeOctalStr,
				     *aclModeOctalStr) 
		    || &ErrorExit("ERROR: after chmod $mOctalStr on $object, difference between set unix mode and/or read acl mode: $aclModeOctalStr and/or read unix mode: $modeOctalStr");
	    } #o
	} #g
    } #u
    &Log("CHECKPOINT: chmod test on object $object complete");
}    
