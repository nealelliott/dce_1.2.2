package main;

# exercise link and unlink

sub sysUnlinkRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($testDir, $testFile, $linkTo, $newLink);
    local($par2);
    local($code, @aclOut);
    local($retCode);
    local($success);

    # ("dir1", "dir1/dir1_1"," dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);
    $par2 = "$par/$dir1Star[1]"; # ./dir1/dir1_1

    $child = "$par/$dir1Star[0]"; # ./dir1
    $testDir = "$par2/tempdir";	# ./dir1/dir1_1/tempdir
    $testFile = "$par2/tempfile"; # ./dir1/dir1_1/tempfile
    $linkTo = "$par/$dir1Star[2]"; # ./dir1/file1_2
    $newLink = "$par2/newlink";	# ./dir1/dir1_1/newlink

    $success = &createFile($testFile, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create $testFile", $retCode);
    }

    # check for search (x) disabling. unlink and link should fail
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:$perm", $code) if ($code);
	    
	    $success = &unlinkObject($testFile, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: unlink: $pathComp, $testFile; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: unlink: $pathComp, $testFile; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &linkObject($linkTo, $newLink, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: link: $pathComp, $linkTo, $newLink; perm $perm; incorrectly succeeded"); 
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: link: $pathComp, $linkTo, $newLink; perm $perm; failed with wrong errno: $retCode");
	    }
	} # perm

	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:xc",
		   $code) if ($code);
    } # pathComp

    # at this point we have only x and c permissions on all the components,
    # should not need any more. c is needed to be able to modify the DACLs in 
    # this test.

    # unlink and link should succeed only if w is enabled on its parent
    foreach $perm (@NO_W_PERMS) {
	$code = &aclEdit("-n", $par2, "-m user_obj:$perm", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $par2 to user_obj:$perm",
		   $code) if ($code);

	$success = &unlinkObject($testFile, *retCode);
	if ($success) {
	    &ErrorExit("ERROR: unlink: parent $par2 ($testFile); perm $perm; incorrectly succeeded");
	} elsif ( $retCode != $EACCES && $retCode != $EPERM) {
	    &ErrorExit("ERROR: unlink: parent $par2 ($testFile); perm $perm; failed with wrong errno: $retCode");
	}

	$success = &linkObject($linkTo, $newLink, *retCode);
	if ($success) {
	    &ErrorExit("ERROR: link: parent $par2 ($linkTo, $newLink); perm $perm; incorrectly succeeded");
	} elsif ( $retCode != $EACCES && $retCode != $EPERM) {
	    &ErrorExit("ERROR: link: parent $par2 ($linkTo, $newLink); perm $perm; failed with wrong errno: $retCode");
	}
    } #perm

    # enable w (actually wid and x (to search))
    $code = &aclEdit("-n", $par2, "-m user_obj:wxcid", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $par2 to user_obj:wxcid", $code)
	if ($code);
    $success = &unlinkObject($testFile, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: unlink: $testFile", $retCode);
    }
    $success = &linkObject($linkTo, $newLink, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: link: $linkTo $newLink", $retCode);
    }
    0;
}

1;
