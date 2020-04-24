package main;

# exercise chmod

sub sysChmodRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($toChmod);
    local($code, @aclOut);
    local($retCode);
    local($success);

    # ("dir1", "dir1/dir1_1"," dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);
    $child = "$par/$dir1Star[0]"; # ./dir1
    $toChmod = "$par/$dir1Star[2]"; # ./dir1/file1_2

    @modeList = (0400, 0200, 0100, 040, 020, 010, 04, 02, 01);

    # check for search (x) disabling. chmod should fail
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:$perm", $code) if ($code);
	    foreach $mode (@modeList) {
		$success = &changeMode($mode, $toChmod, *retCode);
		if ($success) {
		    &ErrorExit("ERROR: chmod: $pathComp, $toChmod; perm $perm; succeeded incorrectly");
		} elsif ($retCode != $EACCES && $retCode != $EPERM) {
		    &ErrorExit("ERROR: chmod: $pathComp, $toChmod; perm $perm; failed with wrong errno: $retCode");
		}
	    } # mode
	} # perm
	
	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:xc",
		   $code) if ($code);
    } # pathComp

    # At this point we have only x and c permissions on all the components, 
    # should not need any more. c is needed to be able to modify the DACLs in 
    # this test.

    foreach $mode (@modeList) {
	$success = &changeMode($mode, $toChmod, *retCode);
	if (!$success) {
	    &ErrorExit("ERROR: chmod: $toChmod $mode failed", $retCode);
	}
    }
    
    $success = &changeMode(0777, $toChmod, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: chmod: $toChmod 077 failed", $retCode);
    }
    0;
}

1;
