package main;

# exercise utime

sub sysUtimeRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($toOpen); # pre-existing file
    local($code, @aclOut);
    local($retCode);
    local($success);


    # ("dir1", "dir1/dir1_1"," dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # do not need @dir2Star really
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);
    $child = "$par/$dir1Star[0]";
    $toOpen = "$par/$dir1Star[2]";
    
    # check for search (x) disabling. utime should fail.
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set object acl on $pathComp to user_obj:$perm", $code) if ($code);
    	    $success = &utimeObject($toOpen, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: utime: $pathComp, $toOpen; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: utime: $pathComp, $toOpen; perm $perm; failed with wrong errno: $retCode");		
	    }
	} # perm
	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&Error_Exit("ERROR: failed to set acl on $pathComp to user_obj:xc", $code)
	    if ($code);
    } #pathComp

    # at this point we have only x and c permissions on all the components
    # enable write and try and call utime
    $code = &aclEdit("-n", $toOpen, "-m user_obj:wc", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $toOpen to user_obj:wc", $code) 
	if ($code);

    $success = &utimeObject($toOpen, *retCode);
    if (!$success) {
	&ErrorExit(sprintf("ERROR: utime: $toOpen, failed with errno %d", $retCode));
    }
    0;
}

1;
