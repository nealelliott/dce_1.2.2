package main;

# exercise rename

sub sysRenameRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($fromPar, $toPar);
    local($from, $toNew, $toSame, $fromDir, $toNewDir, $toSameDir);
    local($dir);
    local($code, @aclOut);
    local($retCode);
    local($success);

    # ("dir1", "dir1/dir1_1"," dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);
    $fromPar = "$par/$dir1Star[1]"; # ./dir1/dir1_1
    $toPar = "$par/$dir2Star[1]"; # ./dir2/dir2_1

    $child = "$par/$dir1Star[0]"; # ./dir1

    $from = "$fromPar/tfile1";	# ./dir1/dir1_1/tfile1
    $toNew = "$toPar/tfile2";	# ./dir2/dir2_1/tfile2
    $toSame = "$toPar/tfile1";	# ./dir2/dir2_1/tfile1
    $fromDir = "$fromPar/tdir1"; # ./dir1/dir1_1/tdir1
    $toNewDir = "$toPar/tdir2";	# ./dir2/dir2_1/tdir2
    $toSameDir = "$toPar/tdir1"; # ./dir2/dir2_1/tdir1

    $success = &createFile($from, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file $from", $retCode);
    }

    $success = &createFile($toSame, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file $toSame", $retCode);
    }
    
    &MakeDir($fromDir);
    &MakeDir($toSameDir);

    # check for search (x) disabling. rename should fail
    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:$perm", $code) if ($code);
	    $success = &renameObject($from, $toNew, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $pathComp, $from, $toNew; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $pathComp, $from, $toNew; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &renameObject($from, $toSame, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $pathComp, $from, $toSame; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $pathComp, $from, $toSame; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &renameObject($fromDir, $toSameDir, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $pathComp, $fromDir, $toSameDir; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $pathComp, $fromDir, $toSameDir; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &renameObject($fromDir, $toNewDir, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $pathComp, $fromDir, $toNewDir; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $pathComp, $fromDir, $toNewDir; perm $perm; failed with wrong errno: $retCode");
	    }
	} #perm

	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $pathComp to user_obj:xc",
		   $code) if ($code);
    } # pathComp

    # at this point we have only x and c permissions on all the components, 
    # should not need any more. c is needed to be able to modify the DACLs in 
    # this test.
    # rename should succeed only if w is enabled on its parent/destination 
    # directory when required.
    foreach $dir ($fromPar, $toPar) {
	foreach $perm (@NO_W_PERMS) {
	    $code = &aclEdit("-n", $dir, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set acl on $dir to user_obj:$perm",
		       $code) if ($code);

	    $success = &renameObject($from, $toNew, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $dir, $from, $toNew; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $dir, $from, $toNew; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &renameObject($from, $toSame, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $dir, $from, $toSame; perm $perm; succeeded incorrectly");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $dir, $from, $toSame; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &renameObject($fromDir, $toNewDir, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $dir, $fromDir, $toNewDir; perm $perm; succeeded incorrectly");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $dir, $fromDir, $toNewDir; perm $perm; failed with wrong errno: $retCode");
	    }

	    $success = &renameObject($fromDir, $toSameDir, *retCode);
	    if ($success) {
		&ErrorExit("ERROR: rename: $dir, $fromDir, $toSameDir; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: rename: $dir, $fromDir, $toSameDir; perm $perm; failed with wrong errno: $retCode");
	    }
	} #perm
	
	$code = &aclEdit("-n", $dir, "-m user_obj:wxcid", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $dir to user_obj:wxcid", 
		   $code) if ($code);
    } # dir

    # got all the required permissions, check the operations afresh. They 
    # should succeed.

    $success = &renameObject($from, $toNew, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: rename: from $from to $toNew", $retCode);
    }
    
    $success = &createFile($from, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: Unable to create file $from", $retCode);
    }

    $success = &renameObject($from, $toSame, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: rename: from $from to $toSame", $retCode);
    }

    $success = &renameObject($fromDir, $toNewDir, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: rename: from $fromDir to $toNewDir", $retCode);
    }
    
    &MakeDir($fromDir);
    $success = &renameObject($fromDir, $toSameDir, *retCode);
    if (!$success) {
	&ErrorExit("ERROR: rename: from $fromDir to $toSameDir", $retCode);
    }

    0;
}

1;
