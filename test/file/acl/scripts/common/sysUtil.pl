package main;

# return result of syscall
sub createFile {
    local($file, *error) = @_;
    local($result);

    $error = 0;
    $result = open(SYSTMPFILE, ">$file");
    if (!$result) {
	$error = $!;
    } else {
	close(SYSTMPFILE);
    }
    &Log("createFile: $file, result $result, error $error");
    $result;
}


sub makeDir {
    local($dir, $perm, *error) = @_;
    local($result);

    $error = 0;
    $result = mkdir($dir, $perm);
    if (!$result) {
	$error = $!;
    }
    &Log("makeDir: $dir, result $result, error $error");
    $result;
}

sub removeDir {
    local($dir, *error) = @_;
    local($result);

    $error = 0;
    $result = rmdir($dir);
    if (!$result) {
	$error = $!;
    }
    &Log("removeDir: $dir, result $result, error $error");
    $result;
}

sub changeMode {
    local($mode, $object, *error) = @_;
    local($result);

    $error = 0;
    $result = chmod($mode, $object);
    if (!$result) {
	$error = $!;
    }
    &Log(sprintf("changeMode: %#o $object, result $result, error $error",
		 $mode));
    $result;
}

sub openDirectory {
    local($dir, *error) = @_;
    local($result, $resultStr);

    $error = 0;
    $result = opendir(SYSTMPDIR, $dir);
    if (!$result) {
	$error = $!;
    } else {
	closedir(SYSTMPDIR);
    }
    if (!defined($result)) {
	$resultStr = "UNDEF";;
    } else {
	$resultStr = $result;
    }

    &Log("openDirectory: $dir, result $resultStr, error $error");
    $result;
}

sub renameObject {
    local($from, $to, *error) = @_;
    local($result);

    $error = 0;
    $result = rename($from, $to);
    if (!$result) {
	$error = $!;
    }
    &Log("renameObject: $from to $to, result $result, error $error");
    $result;
}

sub statObject {
    local($object, *error) = @_;
    local(@s, $result);

    $error = 0;
    @s = stat($object);
    if (!defined(@s)) {
	$result = 0;
	$error = $!;
    } else {
	$result = 1;
    }
    &Log("statObject: $object, result $result, error $error");
    $result;
}

sub unlinkObject {
    local($object, *error) = @_;
    local($result);

    $error = 0;
    $result = unlink($object);
    if (!$result) {
	$error = $!;
    }
    &Log("unlinkObject: $object, result $result, error $error");
    $result;
}

sub linkObject {
    local($oldObject, $newObject, *error) = @_;
    local($result);

    $error = 0;
    $result = link($oldObject, $newObject);
    if (!$result) {
	$error = $!;
    }
    &Log("linkObject: oldObject $oldObject, newObject $newObject, result $result, error $error");
    $result;
}

sub utimeObject {
    local($object, *error) = @_;
    local($result);
    local($time);

    $time = time;
    $error = 0;
    $result = utime($time, $time, $object);
    if (!$result) {
	$error = $!;
    }
    &Log("utimeObject: $object, result $result, error $error");
    $result;
}

sub unBufferFH {
    local($fh) = @_;
    local($oldFh);

    $oldFh = select($fh);
    $| = 1;
    select($oldFh);
}

sub clearCloseOnExec {
    local($fileH) = @_;
    local($fd, $r, $f);

    $fd = fileno($fileH);

    $r = fcntl($fileH, 1, $f);
    if (!defined($r)) {
	print "fcntl failed ($r) [$!]\n";
	exit 0;
    } else {
	print "flag is $r\n";
	if ($r eq "0 but true") {
	    return;
	}
    }

    $r = fcntl($fileH, 2, 0);
    if (!defined($r)) {
	print "fcntl clear close-on-exec failed on fileno $fd, ($r) [$!]\n";
	exit 0;
    } 

    $r = fcntl($fileH, 1, $f);
    if (!defined($r)) {
	print "fcntl failed ($r) [$!]\n";
	exit 0;
    } else {
	print "flag is $r\n";
	if ($r ne "0 but true") {
	    print "fcntl, get clear close-on-exec on fileno $fd shows ($r)\n";
	    exit 0;
	}
    }
}

sub setCloseOnExec {
    local($fileH) = @_;
    local($fd, $r, $f);

    $fd = fileno($fileH);

    $r = fcntl($fileH, 1, $f);
    if (!defined($r)) {
	print "fcntl failed ($r) [$!]\n";
	exit 0;
    } else {
	print "flag is $r\n";
	if ($r == 1) {
	    return;
	}
    }

    $r = fcntl($fileH, 2, 1);
    if (!defined($r)) {
	print "fcntl set close-on-exec failed on fileno $fd, ($r) [$!]\n";
	exit 0;
    } 

    $r = fcntl($fileH, 1, $f);
    if (!defined($r)) {
	print "fcntl failed ($r) [$!]\n";
	exit 0;
    } else {
	print "flag is $r\n";
	if ($r != 1) {
	    print "fcntl, set clear close-on-exec on fileno $fd shows ($r)\n";
	    exit 0;
	}
    }
}

# sub  {
#     local(*error) = @_;
#     local($result);
# 
#     $error = 0;
#     $result = 
#     if (!$result) {
# 	$error = $!;
#     }
#     &Log(", result $result, error $error");
#     $result;
# }

1;
