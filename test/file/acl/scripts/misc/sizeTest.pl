#!/afs/tr/proj/tools/bin/perl

# try to grow and shrink DACLs to some large size.
# It is a good idea to grow /etc/passwd and /etc/group artificially before 
# running this test, to control the degree of stress.

# XXX Modify tests to be independent of existing entries in the registry.

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};
require "$TESTACL_SRCDIR/scripts/common/testInit";
$code = &commonSetup($0);
&ErrorExit("ERROR: Unable to do common setup for test $testName") if ($code);

# Globals specific to this test
$NEWPERMSTR = "rc";
$NEWPERMSET = &aclPermStrToAclPermSet($NEWPERMSTR);

# temporaries
# @aclOut
#

# Sets @rgyPrincipals and @rgyGroups
&setup;

foreach $category ("user", "group") {
    local(@nameList);

    &Log("CHECKPOINT: beginning $category growth execution");
    if ($category eq "user") {
	@nameList = @rgyPrincipals;
    } else {
	@nameList = @rgyGroups;
    }	
    
    foreach $name (@nameList) {
	undef(%acl);
	next if ($name eq "rootl" || $name eq "adduser" ||
		 $name eq "netmail" || $name eq "su");
	&Log("CHECKPOINT: about to add $category:$name:$NEWPERMSTR to object ACL of $EXECDIR");
	$code = &aclEdit("-n", $EXECDIR, "-m $category:$name:$NEWPERMSTR", *aclOut);
	&ErrorExit("ERROR: unable to add entry \"$category:$name:$NEWPERMSTR\" to object ACL of $EXECDIR", $code) if ($code);
	# Get actual acl entry perm not effective one.
	$code = &getAcl($EXECDIR, 'o', 0, *acl);
	&ErrorExit("Unable to read object ACL of $EXECDIR", $code) if ($code);
	if ($NEWPERMSET != $acl{"$category:$name"}) {
	    local($permStr) = &aclPermSetToAclPermStr($acl{"$category:$name"});
	    &ErrorExit("ERROR: for $category:$name, new added entry \"$NEWPERMSTR\" differs from that read \"$permStr\" on object ACL of $EXECDIR");
	}
    }
}

&Log("CHECKPOINT: ACL growth phase completed successfully");

# now shrink the DACL to the original basic DACL.
# Verify that listing and modification is still possible and that the 
# rights are interpreted correctly. This is an indirect verification of 
# DACL correctness.

foreach $category ("user", "group") {
    local(@nameList);

    &Log("CHECKPOINT: beginning $category shrinkage execution");
    if ($category eq "user") {
	@nameList = @rgyPrincipals;
    } else {
	@nameList = @rgyGroups;
    }	
    
    foreach $name (@nameList) {
	undef(%acl);
	next if ($name eq "rootl" || $name eq "adduser" ||
		 $name eq "netmail" || $name eq "su");
	&Log("CHECKPOINT: about to delete $category:$name from object ACL of $EXECDIR");
	$code = &aclEdit("-n", $EXECDIR, "-d $category:$name", *aclOut);
	&ErrorExit("ERROR: unable to delete entry for \"$category:$name\" from object ACL of $EXECDIR", $code) if ($code);
	$code = &getAcl($EXECDIR, 'o', 0, *acl);
	&ErrorExit("Unable to read object ACL of $EXECDIR", $code) if ($code);
	if ($acl{"$category:$name"}) {
	    &ErrorExit("ERROR: deleted entry \"$category:$name\" still exists on object ACL of $name");
	}
    }
}

&Log("CHECKPOINT: sizetest test completed successfully");

$code = &commonCleanup;
&ErrorExit("ERROR: Unable to do common cleanup after test sizetest", $code)
    if ($code);

0;

#
# End of main section
#

#
# Subroutines
#

sub setup {
    local(@aclOut);
    local($code);
    local($obj);
    
    # Generate the basic DACLs on sizetest
    $obj = $EXECDIR;
    $code =
	&aclEdit("-n", $obj, "-m user_obj:$ALLPERMS group_obj:- other_obj:- mask_obj:$ALLPERMS", *aclOut);
    &ErrorExit("ERROR: unable to generate object ACL on $obj", $code) 
	if ($code);
    
    $code =
	&aclEdit("-n", $obj, "-io -m user_obj:$ALLPERMS group_obj:- other_obj:- mask_obj:$ALLPERMS", *aclOut);
    &ErrorExit("ERROR: unable to generate IO ACL on $obj", $code)
	if ($code);

    $code =
	&aclEdit("-n", $obj, "-ic -m user_obj:$ALLPERMS group_obj:- other_obj:- mask_obj:$ALLPERMS", *aclOut);
    &ErrorExit("ERROR: unable to generate IC ACL on $obj", $code)
	if ($code);
    
    $code = &getRgyPrincipals(*rgyPrincipals);
    &ErrorExit("ERROR: unable to get registry principals", $code) if ($code);
    $code = &getRgyGroups(*rgyGroups);
    &ErrorExit("ERROR: unable to get registry groups", $code) if ($code);
    &Log("CHECKPOINT: sizetest local setup complete");
}
