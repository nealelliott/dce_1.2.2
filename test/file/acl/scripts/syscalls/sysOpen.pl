package main;

# exercise opendir

sub sysOpenRun {
    local($dir1StarStr, $dir2StarStr) = @_;
    local(@dir1Star, @dir2Star);
    local($par);  # current dir
    local($child); # pre-existing dir
    local($toOpen); # pre-existing file
    local($newFile); # a new file that is created by this test
    local($code, @aclOut);
    local($retCode);

    # for some reason, this one is different from that used by the other test
    # x is still needed:
    local(@LOCAL_NO_W_PERMS) = ("rxcid", "rxci", "rxcd", "xcid", "rxc", "xci",
				"xcd", "xc");

    # ("dir1", "dir1/dir1_1", "dir1/file1_2");
    @dir1Star = split(' ', $dir1StarStr);
    # do not really need @dir2Star
    # ("dir2", "dir2/dir2_1", "dir2/file2_2");
    @dir2Star = split(' ', $dir2StarStr);

    $par = `pwd`; chop($par);
    $child = "$par/$dir1Star[0]"; # ./dir1
    $toOpen = "$par/$dir1Star[2]"; # ./dir1/file1_2
    $newFile = "$par/newfile";	# ./newfile

    # check for search (x) disabling. opendir should fail.

    foreach $pathComp ($par, $child) {
	foreach $perm (@NO_X_PERMS) {
	    $code = &aclEdit("-n", $pathComp, "-m user_obj:$perm", *aclOut);
	    &ErrorExit("ERROR: failed to set object acl on $pathComp to user_obj:$perm", $code) if ($code);
#	    `"$TESTACL_SRCDIR/utils/mopen" $toOpen 0777 DEFAULT`;
	    $success = open(OPENFILE, "+<$toOpen");
	    $retCode = $!;
	    &Log("open +< $toOpen, result $success, errno $retCode");
	    if ($success) {
		&ErrorExit("ERROR: open (rdwr): $pathComp, $toOpen; perm $perm; incorrectly succeeded");
	    } elsif ($retCode != $EACCES && $retCode != $EPERM) {
		&ErrorExit("ERROR: open (rdwr): $pathComp, $toOpen; perm $perm; failed with wrong errno: $retCode", $retCode);		
	    }
	} # perm
	$code = &aclEdit("-n", $pathComp, "-m user_obj:xc", *aclOut);
	&Error_Exit("ERROR: failed to set acl on $pathComp to user_obj:xc", $code) if ($code);
    } #pathComp

    # at this point we have only x and c permissions on all the components
    # should be able to open a file for reading if it already exists.
#   `$TESTACL_SRCDIR/utils/mopen $toOpen 0777 RDONLY`;
    $success = open(OPENFILE, "$toOpen");
    $retCode = $!;
    &Log("open $toOpen, result $success, errno $retCode");
    if (!$success) {
	&ErrorExit("ERROR: open (rd): $toOpen, failed incorrectly with errno $retCode", $retCode);
    }
    close(OPENFILE);

    # creating a new file should fail since no "wi" is enabled on the target 
    # directory.
    foreach $perm (@LOCAL_NO_W_PERMS) {
	$code = &aclEdit("-n", $par, "-m user_obj:$perm", *aclOut);
	&ErrorExit("ERROR: failed to set acl on $par to user_obj:$perm", $code)
	    if ($code);
#	`$TESTACL_SRCDIR/utils/mopen $newFile 0777 CREAT`;
	$success = open(OPENFILE, ">$newFile");
	$retCode = $!;
	&Log("open >$newFile, result $success, errno $retCode");
	if ($success) {
	    &ErrorExit("ERROR: open (create): $newFile, $par perm $perm; incorrectly succeeded");
	} elsif ($retCode != $EACCES && $retCode != $EPERM) {
	    &ErrorExit("ERROR: open (create): $newFile, $par perm $perm; failed with wrong errno: $retCode");		
	}
    } #perm

    # enable all permissions on the parent directory

    $code = &aclEdit("-n", $par, "-m user_obj:$ALLPERMS", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $par to user_obj:$ALLPERMS", $code)
	if ($code);
    $code = &aclEdit("-n", $child, "-m user_obj:$ALLPERMS", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $child to user_obj:$ALLPERMS",
	       $code)
	if ($code);

    $code = &aclEdit("-n", $toOpen, "-m user_obj:wc", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $toOpen to user_obj:wc",
	       $code)
	if ($code);

    $code = &aclEdit("-n", $toOpen, "-m user_obj:rwc", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $toOpen to user_obj:rwc",
	       $code)
	if ($code);
#    `$TESTACL_SRCDIR/utils/mopen $toOpen 0777 RDWR`;
    $success = open(OPENFILE, "+<$toOpen");
    &Log("open +< $toOpen, result $success, errno $retCode");
    if (!$success) {
	&ErrorExit("ERROR: open (rdwr): $toOpen failed incorrectly with errno $retCode", $retCode);
    }
    close(OPENFILE);

    $code = &aclEdit("-n", $toOpen, "-m user_obj:rc", *aclOut);
    &ErrorExit("ERROR: failed to set acl on $toOpen to user_obj:rc",
	       $code)
	if ($code);
#   `$TESTACL_SRCDIR/utils/mopen $toOpen 0777 RDONLY`;
    $success = open(OPENFILE, "$toOpen");
    $retCode = $!;
    &Log("open $toOpen, result $success, errno $retCode");
    if (!$success) {
	&ErrorExit("ERROR: open (rd): $toOpen, failed incorrectly with errno $retCode", $retCode);
    }
    close(OPENFILE);

    0;
}

1;
