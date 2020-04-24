#!/afs/tr/proj/tools/bin/perl

# verify that the DACLs are set and read correctly.

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};
require "$TESTACL_SRCDIR/scripts/common/testInit";
$code = &commonSetup($0);
&ErrorExit("ERROR: Unable to do common setup for test $testName") if ($code);

# Globals specific to this test

# possible values of permission bits
@PERMSSET = ("r", "w", "x", "i", "d", "c");
# possible values of permission bits
@USEROBJ_PERMSSET = ("rc", "wc", "xc", "ci", "cd", "c");

# Globals set in selectTestUsersAndGroups()
# @USERSSET
# @GROUPSSET

# Globals set in selectTestObjects()
# $TESTDIR
# $TESTFILE
# @TESTOBJSET = ("$EXECDIR/testdir3", "$EXECDIR/file3");

# Global temporaries
# @aclOut
# %acl
# $permStr

&setup;

foreach $object (@TESTOBJSET) {
    @tflagsSet = ("o");

    if ($object eq $TESTDIR) {
	push(@tflagsSet, "ic");
	push(@tflagsSet, "io");
    }
    # try setting rwxidc bits (one at a time) on user_obj, group_obj other_obj 
    # and mask_obj entries on all the three DACLs on the testdir3 or 
    # one DACL on file1
    foreach (@tflagsSet) {
	$tflags = $_;
	$whichAcl = $_;
	if ($tflags ne "o") {
	    $tflags = "-" . $tflags;
	} else {
	    $tflags = "";
	}
	foreach $type ("user_obj", "group_obj", "other_obj", "mask_obj") {
	    &Log("Checking $type entry");
	    if ($type eq "user_obj") {
		# the userobj permset makes sure that c is always on the list 
		@permsSetToUse = @USEROBJ_PERMSSET;
	    } else {
		@permsSetToUse = @PERMSSET;		
	    }
	    foreach $perm (@permsSetToUse) {
		undef(%acl);

# XXX Passing -c to acl_edit is not really the right thing to do.  You
# want -n but that is not currently working when, on an ACL with only
# user_obj, group_obj, other_obj, an entry affecting mask_obj implicitly
# is added
#
		$code = &aclEdit("-n", $object, "$tflags -m $type:$perm", *aclOut);
		&ErrorExit("Unable to set $whichAcl ACL on $object", $code) 
		    if ($code);
		# Read actual acl entry perms not effective ones.
		$code = &getAcl($object, $whichAcl, 0, *acl);
		&ErrorExit("Unable to read $whichAcl ACL on $object", $code) 
		    if ($code);
		if (&aclPermStrToAclPermSet($perm) != $acl{$type}) {
		    local($permStr) = &aclPermSetToAclPermStr($acl{$type});
		    &ErrorExit("ERROR: In simple entry test for $whichAcl ACL of $object, $type set entry $perm differs from read entry $permStr");
		}
	    } # perm
	} # type
    } # tflags

    # checked mode bits on the usual types
    &Log("CHECKPOINT: simple entry test for $object complete");

    # check insertion of users and groups and changing permissions on each of 
    # the user and group entry, one at a time.
    foreach (@tflagsSet) {
	$tflags = $_;
	$whichAcl = $_;
	if ($tflags ne "o") {
	    $tflags = "-" . $tflags;
	} else {
	    $tflags = "";
	}
	foreach $type ("user", "group") {
	    local(@setToUse);
	    
	    &Log("Checking $type entry");
	    if ($type eq "user") {
		@setToUse = @USERSSET;
	    } else {
		@setToUse = @GROUPSSET;
	    }
	    foreach $user (@setToUse) {
		foreach $perm (@PERMSSET) {
		    undef(%acl);
		    $code = &aclEdit("-n", $object, "$tflags -m $type:$user:$perm",
				     *aclOut);
		    &ErrorExit("ERROR: Unable to set $whichAcl ACL on $object",
			       $code) if ($code);

		    $code = &getAcl($object, $whichAcl, 0, *acl);
		    &ErrorExit("Unable to read $whichAcl ACL on $object",
			       $code) if ($code);		    
		    if (&aclPermStrToAclPermSet($perm) != $acl{"$type:$user"}) {
			local($permStr) =
			    &aclPermSetToAclPermStr($acl{"$type:$user"});
			&ErrorExit("ERROR: In user/group entry test for $whichAcl ACL of $object, $type:$user set entry $perm differs from read entry $permStr");
		    }
		} # perm
	    } # user
	} # type
    } # tflags

    # checked mode bits on the user and group
    &Log("CHECKPOINT: user/group entry test for $object complete");
}				# object

# Change ACLs on object to allow deletion for cleanup
foreach $object (@TESTOBJSET) {
    $code = &aclEdit("-c", $object,
		     "-m user_obj:rwxcid group_obj:rwxcid other_obj:rwxcid",
		     *aclOut);
    if ($code) {
	&ErrorExit("ERROR: Unable to reset ACL on $object to allow cleanup",
		   $code);
    }
}

&Log("CHECKPOINT: GETSET test completed successfully");

$code = &commonCleanup;
&ErrorExit("ERROR: Unable to do common cleanup after test GETSET", $code)
    if ($code);

0;

#
# End of main section
#

#
# Subroutines
#

sub setup {
    local($success, $retCode);

    &selectTestUsersAndGroups;
    &selectTestObjects;
    &MakeDir($TESTDIR);
    if (-f $TESTFILE) {
	$success = &unlinkObject($TESTFILE, *retCode);
	if (!$success) {
	    &ErrorExit("ERROR: unable to cleanup test file $TESTFILE",
		       $retCode);
	}
    }
    $success = &createFile($TESTFILE, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: unable to create test file $TESTFILE", $retCode);
    }

    &Log("CHECKPOINT: local setup complete");
}

sub selectTestUsersAndGroups {
    @USERSSET = ("acluu");
    @GROUPSSET = ("aclngog")
}

sub selectTestObjects {
    $TESTDIR = "$EXECDIR/testdir3";
    $TESTFILE = "$EXECDIR/file3";
    @TESTOBJSET = ($TESTDIR, $TESTFILE);
}
