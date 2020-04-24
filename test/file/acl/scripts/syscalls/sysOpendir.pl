package main;

# exercise opendir

sub sysOpendirRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($toOpen);
    local($code, @aclOut);
    local($retCode);
    local($success);

    # ("dir1", "dir1/dir1_1"," dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);	# 
    $child = "$par/$dir1Star[0]"; # ./dir1
    $toOpen = "$par/$dir1Star[1]"; # ./dir1/dir1_1

    #check for search (x) disabling. opendir should fail.
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: unable to set acl on $pathComp to user_obj:$perm", $code) if ($code);

	    $success = &openDirectory($toOpen, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: open: $pathComp, $toOpen; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: open: $pathComp, $toOpen; perm $perm; failed with wrong errno: $retCode");
	    }
	} #perm

	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: unable to set acl on $pathComp to user_obj:xc",
		   $code) if ($code);
    } #pathComp

    # At this point we have only x and c permissions on all the components, 
    # should not need any more. c is needed to be able to modify the DACLs in 
    # this test.

    # search should succeed since r is enabled on TOOPEN
    $success = &openDirectory($toOpen, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: opendir $toOpen failed", $retCode);
    }

    # if r is disabled on TOOPEN opendir should fail
    foreach $perm (@NO_R_PERMS) {
	$code = &aclEdit("-n", $toOpen, "-m user_obj:$perm", *aclOut);
	&ErrorExit("ERROR: unable to set acl on $toOpen to user_obj:$perm",
		   $code) if ($code);

	$success = &openDirectory($toOpen, *retCode);
	if ($success) {
	    &ErrorExit("ERROR: open: $pathComp, $toOpen; perm $perm; incorrectly succeeded");
	} elsif ($retCode != $EACCES && $retCode != $EPERM) {
	    &ErrorExit("ERROR: open: $pathComp, $toOpen; perm $perm; failed with wrong errno: $retCode");
	}
    } #perm

    0;
}

1;
