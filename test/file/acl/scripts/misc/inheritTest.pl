#!/afs/tr/proj/tools/bin/perl

# Things to remember: have idc bits ON to enable subdir/subfile creation
# and manipulation of DACLs (when running as a non super-user).
# Create mask_obj before adding any types like user, group etc.

# XXX Clear umask to avoid existing bugs.
umask(0);

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};
require "$TESTACL_SRCDIR/scripts/common/testInit";
$code = &commonSetup($0);
&ErrorExit("ERROR: Unable to do common setup for test $testName") if ($code);

# Globals specific to this test
# in this case, we modify the execution directory
$EXECDIR = "$EXECDIR/testdir1";

#
#  Permissions for the execution directory, itself.
#
$UO_PERM = "rwxcid";
$GO_PERM = "rw";
$OO_PERM = "r";
$U_PERM = "rxd";
$G_PERM = "rwi";
$M_PERM = "rx";

#
# Permissions in initial file ACL.
# NOTE: Exclude "x" for things that map to mode bits, since 
# the subroutine perl's open(FILE, ">$file") also does so by specifying
# 0666 as the new file mode bits.
#
$UO_PERMI = "rwcid";
$GO_PERMI = "rx";
$OO_PERMI = "w";
$U_PERMI = "rwxid";
$G_PERMI = "rx";
$M_PERMI = "w";

#
# Permissions for the initial directory ACL.
#
$UO_PERMD = "rwxcid";
$GO_PERMD = "rwx";
$OO_PERMD = "x";
$U_PERMD = "id";
$G_PERMD = "c";
$M_PERMD = "r";

$UO_PERMT = "rwxcid";
$GO_PERMT = "x";
$OO_PERMT = "rwx";
$U_PERMT = "idc";
$G_PERMT = "c";
$M_PERMT = "r";

# Globals set in selectTestUsersAndGroups
# $GEN_USER 
# $GEN_GROUP

# Globals that hold state of ACLs for later comparsion
# ACLs %inDir, %inDirIf, %inDirId set in setup.

&setup;
&checkDirAclInheritance;
&checkFileAclInheritance;
&checkCowLinkBreakage;

&Log("CHECKPOINT: inherit test completed successfully");

$code = &commonCleanup;
&ErrorExit("ERROR: Unable to do common cleanup after test inherit", $code)
    if ($code);

0;

#
# End of main section
#

#
# Subroutines
#

# Sets ACLs %inDir, %inDirIf, %inDirId
sub setup {
    local($code, @aclOut);
    
    &selectTestUsersAndGroups;
    
    &MakeDir($EXECDIR, 0777);

    # setup the ACLs and save them for future comparision
    # object ACL
    $code = &aclEdit("-n", $EXECDIR, "-m mask_obj:$M_PERM user_obj:$UO_PERM group_obj:$GO_PERM other_obj:$OO_PERM user:$GEN_USER:$U_PERM group:$GEN_GROUP:$G_PERM", *aclOut);
    &ErrorExit("ERROR: Unable to set o ACL on $EXECDIR", $code)
	if ($code);

    # XXX inDir is not used anywhere and is not really needed.
    # Get actual ACL entry perms not effective perms.
    $code = &getAcl($EXECDIR, "o", 0, *inDir);
    &ErrorExit("ERROR: Unable to list o ACL on $EXECDIR", $code)
	if ($code);

    # initial file ACL
    $code = &aclEdit("-n", $EXECDIR, "-io -m mask_obj:$M_PERMI user_obj:$UO_PERMI group_obj:$GO_PERMI other_obj:$OO_PERMI user:$GEN_USER:$U_PERMI group:$GEN_GROUP:$G_PERMI", *aclOut);
    &ErrorExit("ERROR: Unable to set io ACL on $EXECDIR", $code) if ($code);
    $code = &getAcl($EXECDIR, "io", 0, *inDirIf);
    &ErrorExit("ERROR: Unable to list io ACL on $EXECDIR", $code) if ($code);

    # initial dir ACL
    $code = &aclEdit("-n", $EXECDIR, "-ic -m mask_obj:$M_PERMD user_obj:$UO_PERMD group_obj:$GO_PERMD other_obj:$OO_PERMD user:$GEN_USER:$U_PERMD group:$GEN_GROUP:$G_PERMD", *aclOut);
    &ErrorExit("ERROR: Unable to set ic ACL on $EXECDIR", $code) if ($code); 
    $code = &getAcl($EXECDIR, "ic", 0, *inDirId);
    &ErrorExit("ERROR: Unable to list io ACL on $EXECDIR", $code) if ($code);

    &Chdir($EXECDIR);
    
    # at this point the three DACLs on testdir1 are in a known state
    # ACLs %inDir, %inDirIf and %inDirId capture this state.
    &Log("CHECKPOINT: local setup complete");
}

# This routine picks user and group variables out of the secd database. We 
# assume that the database has been set up for us by the initialization 
# routine.The GEN_USER should be different from the uid under which the test 
# is running. Similarly, the GEN_GROUP should be different from the gid under
# which the test is running.

sub selectTestUsersAndGroups {
    $GEN_USER = "acluu";
    $GEN_GROUP = "aclngog";
}

sub checkDirAclInheritance {
    local($code, @aclOut);
    local(%inDirP, %inDirIfP, %inDirIdP);

    $code = &MakeDir2("dir1", 0777);
    `ls -l`;
    &ErrorExit("ERROR: unable to create dir $EXECDIR/dir1 ($code)", $code) 
	if ($code);

    $code = &getAcl(dir1, "o", 0, *inDirP);
    &ErrorExit("ERROR: unable to list o ACL on $EXECDIR/dir1", $code) 
	if ($code);
    $code = &getAcl(dir1, "io", 0, *inDirIfP);
    &ErrorExit("ERROR: unable to list io ACL on $EXECDIR/dir1", $code)
	if ($code);
    $code = &getAcl(dir1, "ic", 0, *inDirIdP);
    &ErrorExit("ERROR: unable to list ic ACL on $EXECDIR/dir1", $code) 
	if ($code);
    
    &sameAcls(%inDirId, %inDirP) || 
	&ErrorExit("ERROR: o ACL on $EXECDIR/dir1 is different than ic ACL on parent");

    &sameAcls(%inDirIf, %inDirIfP) || 
	&ErrorExit("ERROR: io ACL on $EXECDIR/dir1 is different than io ACL on parent");

    &sameAcls(%inDirId, %inDirIdP) || 
	&ErrorExit("ERROR: ic ACL on $EXECDIR/dir1 is different than ic ACL on parent");

    &Log("CHECKPOINT: directory ACL inheritance OK");
}

sub checkFileAclInheritance {
    local($code);
    local(%inFileP);
    local($success, $retCode);

    $success = &createFile("file1", *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file $EXECDIR/file1", $retCode);
    }

    $code = &getAcl(file1, "o", 0, *inFileP);
    &ErrorExit("ERROR: error listing o ACL on $EXECDIR/file1", $code)
	if ($code);
    &sameAcls(%inDirIf, %inFileP) || 
	&ErrorExit("ERROR: o ACL on $EXECDIR/file1 is different than io ACL on parent");

    &Log("CHECKPOINT: file ACL inheritance OK");
}

sub checkCowLinkBreakage {
    local($code, @aclOut);
    local(*inFileP, *inDirP, *inDirIfP, *inDirIdP);
    local($success, $retCode);

    # ensure that copy-on-write sharing on DACLs works correctly
    $code = &MakeDir2("dir2", 0777);
    &ErrorExit("ERROR: unable to create dir $EXECDIR/dir2 ($code)", $code) 
	if ($code);

    $success = &createFile("file2", *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file $EXECDIR/file2", $retCode);   
    }

    $success = &createFile("file3", *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file $EXECDIR/file3", $retCode);
    }

    $code = &getAcl(file1, "o", 0, *inFileP);
    &ErrorExit("ERROR: Unable to list o ACL on $EXECDIR/file1 (1)", $code)
	if ($code);

    $code = &getAcl(dir1, "o", 0, *inDirP);
    &ErrorExit("ERROR: Unable to list o ACL on $EXECDIR/dir1 (1)", $code)
	if ($code);

    $code = &getAcl(dir1, "io", 0, *inDirIfP);
    &ErrorExit("ERROR: Unable to list io ACL on $EXECDIR/dir1 (1)", $code)
	if ($code);

    $code = &getAcl(dir1, "ic", 0, *inDirIdP);
    &ErrorExit("ERROR: Unable to list ic ACL on $EXECDIR/dir1 (1)", $code)
	if ($code);

    &Log("CHECKPOINT: first set of ACLs (1) saved");

    # Modify ACLs on file2, file3 and dir2 making them different from that of 
    # dir1 and file1.  This should force a cow link breakage.

    $code = &aclEdit("-n", file2, "-m mask_obj:$M_PERMT user_obj:$UO_PERMT group_obj:$GO_PERMT other_obj:$OO_PERMT user:$GEN_USER:$U_PERMT group:$GEN_GROUP:$G_PERMT", *aclOut);
    &ErrorExit("ERROR: unable to set o ACL on $EXECDIR/file2", $code)
	if ($code);

    $code = &aclEdit("-n", file3, "-m mask_obj:$M_PERMT user_obj:$UO_PERMT group_obj:$GO_PERMT other_obj:$OO_PERMT user:$GEN_USER:$U_PERMT group:$GEN_GROUP:$G_PERMT", *aclOut);
    &ErrorExit("ERROR: unable to set o ACL on $EXECDIR/file3", $code)
	if ($code);

    $code = &aclEdit("-n", dir2, "-m mask_obj:$M_PERMT	user_obj:$UO_PERMT group_obj:$GO_PERMT other_obj:$OO_PERMT user:$GEN_USER:$U_PERMT group:$GEN_GROUP:$G_PERMT", *aclOut);
    &ErrorExit("ERROR: unable to set o ACL on $EXECDIR/dir2", $code)
	if ($code); 

    $code = &aclEdit("-n", dir2, "-io -m mask_obj:$M_PERMT user_obj:$UO_PERMT group_obj:$GO_PERMT other_obj:$OO_PERMT user:$GEN_USER:$U_PERMT group:$GEN_GROUP:$G_PERMT", *aclOut);
    &ErrorExit("ERROR: unable to set io ACL on $EXECDIR/dir2", $code)
	if ($code); 

    $code = &aclEdit("-n", dir2, "-ic -m mask_obj:$M_PERMT user_obj:$UO_PERMT group_obj:$GO_PERMT other_obj:$OO_PERMT user:$GEN_USER:$U_PERMT group:$GEN_GROUP:$G_PERMT", *aclOut);
    &ErrorExit("ERROR: unable to set ic ACL on $EXECDIR/dir2", $code)
	if ($code); 

    &Log("CHECKPOINT: second set of ACLs set");

    # create fresh listings of DACLs on file1 and dir1    
    $code = &getAcl(file1, "o", 0, *inFileP2);
    &ErrorExit("ERROR: Unable to list o ACL on $EXECDIR/file1 (2)", $code)
	if ($code);    

    $code = &getAcl(dir1, "o", 0, *inDirP2);
    &ErrorExit("ERROR: Unable to list o ACL on $EXECDIR/dir1 (2)", $code)
	if ($code);    

    $code = &getAcl(dir1, "io", 0, *inDirIfP2);
    &ErrorExit("ERROR: Unable to list io ACL on $EXECDIR/dir1 (2)", $code)
	if ($code);

    $code = &getAcl(dir1, "ic", 0, *inDirIdP2);
    &ErrorExit("ERROR: Unable to list ic ACL on $EXECDIR/dir1 (2)", $code)
	if ($code);
    &Log("CHECKPOINT: first set of ACLs (2) saved");
    
    # check to see that the new listings match with the old listings prior 
    # to modification of DACLs on file2, file3, dir2
    if (!&sameAcls(*inFileP, *inFileP2)) {
	&Log("Listing original o ACL on $EXECDIR/file1");
	&listAcl(%inFileP);
	&Log("Listing after sharing breakage o ACL on $EXECDIR/file1");
	&listAcl(%inFileP);
	&ErrorExit("ERROR: unexpected change in o ACL on $EXECDIR/file1");
    }

    if (!&sameAcls(*inDirP, *inDirP2)) {
	&Log("Listing original o ACL on $EXECDIR/dir1");
	&listAcl(%inDirP);
	&Log("Listing after sharing breakage o ACL on $EXECDIR/dir1");
	&listAcl(%inDirP2);
	&ErrorExit("ERROR: unexpected change in o ACL on $EXECDIR/dir1");
    }

    if (!&sameAcls(*inDirIfP, *inDirIfP2)) {
	&Log("Listing original io ACL on $EXECDIR/dir1");
	&listAcl(%inDirIfP);
	&Log("Listing after sharing breakage io ACL on $EXECDIR/dir1");
	&listAcl(%inDirIfP2);
	&ErrorExit("ERROR: unexpected change in io ACL on $EXECDIR/dir1");
    }

    if (!&sameAcls(*inDirIdP, *inDirIdP2)) {
	&Log("Listing original ic ACL on $EXECDIR/dir1");
	&listAcl(%inDirIdP);
	&Log("Listing after sharing breakage ic ACL on $EXECDIR/dir1");
	&listAcl(%inDirIdP2);
	&ErrorExit("ERROR: unexpected change in ic ACL on $EXECDIR/dir1");
    }

    &Log("CHECKPOINT: no unexpected ACL changes");

    # XXX Need enhancement to ensure that cow linkage is broken CORRECTLY.
    # Check that the objects with the modified ACLs have correctly
    # modified ACLs
    &Log("CHECKPOINT: copy-on-write link breakage OK");    
}

