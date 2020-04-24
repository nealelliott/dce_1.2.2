package main;

# exercise stat

sub sysStatRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($toStat);
    local($code, @aclOut);

    # ("dir1", "dir1/dir1_1"," dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);
    $child = "$par/$dir1Star[0]"; # ./dir1
    $toStat = "$par/$dir1Star[1]"; # ./dir1/dir1_1

    # on entry x is enabled on all the components of the path. stat should 
    # succeed.

    $success = &statObject($toStat, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: stat: $toStat failed", $retCode);
    }

    #check for search (x) disabling. stat should fail.
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: unable to set acl on $pathComp to user_obj:$perm", $code) if ($code);
	    
	    $success = &statObject($toStat, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: stat: $pathComp, $toStat; perm $perm; incorrectly succeeded");
	    } else {
		if ($retCode != $EACCES && $retCode != $EPERM) {
		    &ErrorExit("ERROR: stat: $pathComp, $toStat; perm $perm; failed with wrong errno: $retCode");
		}
	    }
	} #perm

	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: unable to set acl on $pathComp to user_obj:xc", 
		   $code) if ($code);

    } #pathComp

    0;
}

1;
