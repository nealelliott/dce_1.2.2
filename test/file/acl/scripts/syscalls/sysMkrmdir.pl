package main;

# exercise rmdir and mkdir

sub sysMkrmdirRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($toMkDir);
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
    $toMkDir = "$par2/tempdir";	# ./dir1/dir1_1/tempdir

    # check for search (x) disabling. mkdir should fail.
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:$perm", $code) if ($code);

	    $success = &makeDir($toMkDir, 0777, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: mkdir: $pathComp, $toMkDir; perm $perm; succeeded incorrectly");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: mkdir: $pathComp, $toMkDir; perm $perm; failed with wrong errno: $retCode");
	    }
	} #perm

	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:xc",
		   $code) if ($code);
    } #pathComp


    # At this point we have only x and c permissions on all the components, 
    # should not need any more. c is needed to be able to modify the DACLs in 
    # this test. mkdir should succeed. This dir is needed to test rmdir

    $success = &makeDir($toMkDir, 0777, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create dir $toMkDir", $retCode);
    }
    $success = &removeDir($toMkDir, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to remove dir $toMkDir", $retCode);
    }

    # mkdir should succeed only if w is enabled on its parent
    foreach $perm (@NO_W_PERMS) {
	$code = &aclEdit("-n", $par2, "-m user_obj:$perm", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $par2 to user_obj:$perm",
		   $code) if ($code);

	$success = &makeDir($toMkDir, 0777, *retCode);
	if ($success) {
	    &ErrorExit("ERROR: mkdir: $par2, $toMkDir; perm $perm; incorrectly succeeded");
	} elsif ($retCode != $EACCES && $retCode != $EPERM) {
	    &ErrorExit("ERROR: mkdir: $par2, $toMkDir; perm $perm; failed with wrong errno: $retCode");
	}
    } # perm

    # enable w (actually wi and x (to search))
    $code = &aclEdit("-n", $par2, "-m user_obj:wxci", *aclOut);
    &ErrorExit("ERROR: unable to set acl on $par2 to user_obj:wxci", $code)
	if ($code);

    $success = &makeDir($toMkDir, 0777, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: mkdir: $toMkDir", $retCode);
    }

    # check for search (x) disabling. rmdir should fail.
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: unable to set acl on $pathComp to user_obj:$perm", $code) if ($code);

	    $success = &removeDir($toMkDir, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rmdir: $pathComp, $toMkDir; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rmdir: $pathComp, $toMkDir; perm $perm; failed with wrong errno: $retCode");
	    }
	} #perm

	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: unable to set acl on $pathComp to user_obj:xc", 
		   $code) if ($code);
    } #pathComp

    # if w is disabled on parent of TOMKDIR, rmdir should fail.
    foreach $perm (@NO_W_PERMS) {
	$code = &aclEdit("-n", $par2, "-m user_obj:$perm", *aclOut);
	&ErrorExit("ERROR: unable to set acl on $par2 to user_obj:$perm",
		   $code) if ($code);

	$success = &removeDir($toMkDir, *retCode);
	if ($success) {
	    &ErrorExit("ERROR: rmdir: parent $par2, $toMkDir; perm $perm; incorrectly succeeded"); 
	} elsif ( $retCode != $EACCES && $retCode != $EPERM ) {
	    &ErrorExit("ERROR: rmdir: parent $par2, $toMkDir; perm $perm; failed with wrong errno: $retCode");
	}
    } #perm

    # enable wd and x
    $code = &aclEdit("-n", $par2, "-m user_obj:wxcd", *aclOut);
    &ErrorExit("ERROR: unable to set acl on $par to user_obj:wxcd",
	       $code) if ($code);
    $success = &removeDir($toMkDir, *retCode); 
    if (!$success) {
	&ErrorExit("ERROR: rmdir $toMkDir failed", $retCode);
    }

    0;
}

1;
