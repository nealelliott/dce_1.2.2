# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Generic utility routines
#
#   This file contains generic utilities for test implementation.

# ----------------------------------------------------------------------------


# ForkTestAuthn() - fork/exec test with specified local and DCE authn.
#
#  Args (all IN):
#     caller        - caller id string
#     testExecCmd   - command to execute test
#     testLclUser   - local identity for executing test
#     testDceUser   - DCE identity (principal) for executing test
#     testDcePasswd - DCE password for specified principal
#     testDceAuthn  - flag indicating if DCE identity specified/required
#
#  NOTE: presumes caller is root
#
#  Returns: 1 - successful; 0 - failed
#
sub ForkTestAuthn {
    local($caller,
	  $testExecCmd,
	  $testLclUser, $testDceUser, $testDcePasswd, $testDceAuthn) = @_;
    local($pid, $cmd);

    if ($pid = fork()) {
	# parent process
	return ($pid != -1);
    } else {
	# child process

	# Really want to make a resetpag() system call here so that child
	# loses (parent's) DCE creds without affecting parent.  Unfortunately,
	# this is not possible on systems which don't implement syscall(),
	# e.g. AIX.
	#
	# When exec-ing an authenticated test via dce_login, this is not an
	# issue since dce_login gets a new login context.
	#
	# When exec-ing an unauthenticated test, we work around this problem
	# by exec-ing the intermidiary command execunauth, which performs a
	# resetpag() and then execs the required test.

	$cmd = "$SU $testLclUser -c ";

	if ($testDceAuthn) {
	    $cmd .=
		"'$DCE_LOGIN $testDceUser $testDcePasswd -exec $testExecCmd'";
	} else {
	    $cmd .= "'$SECTEST_SRCDIR/execunauth $testExecCmd'";
	}

	exec("$cmd");

	# never reach here unless exec failed
	print "$caller: Unable to exec $testExecCmd\n";
	exit(1);
    }
}


# Backtick([IN] command) -
#     Attempts to provide the same functionality as the standard backtick
#     operator (`command`), except that the value returned includes BOTH
#     the stdout and stderr produced by command.
#
#     As with the standard backtick operator, the result of command is
#     returned in $?.  If a pipe(), fork(), open() or exec() fails, then $?
#     will be set to 256 (exit(1)), and the value returned will be "".
#
sub Backtick {
    local($cmd) = @_;
    local($pid, $cmdout);

    if (!pipe(BACKTICK_INPIPE, BACKTICK_OUTPIPE)) {
	# pipe failed
	$? = 256;
	return "";
    }

    if ($pid = fork()) {
	# parent process
	close(BACKTICK_OUTPIPE);

	if ($pid == -1) {
	    # fork failed
	    close(BACKTICK_INPIPE);
	    $? = 256;
	    return "";
	}

	$cmdout = "";

	# read output from child
	while (<BACKTICK_INPIPE>) {
	    $cmdout .= $_;
	}
	close(BACKTICK_INPIPE);

	wait;  # sets $?

	return $cmdout;

    } else {
	# child process
	close(BACKTICK_INPIPE);
	open(STDOUT, ">&BACKTICK_OUTPIPE") || exit(1);
	open(STDERR, ">&BACKTICK_OUTPIPE") || exit(1);

	exec("$cmd");

	# never reach here unless exec failed
	exit(1);
    }
}


# CreateFile([IN] filename) - create and write specified file
#
#     Returns: 1 - successful; 0 - failed
#
sub CreateFile {
    local($filename) = @_;
    local($goterror);

    $goterror = 0;

    print "Creating file $filename\n";

    open(OUTFILE, "> $filename") || ($goterror = 1);

    if (!($goterror)) {
	print OUTFILE <<FINIS;
Be most excellent to each other.  Party on, dudes. [Bill and Ted]
FINIS
        close(OUTFILE);
    }

    !($goterror); # return result
}


# CheckFilesets() - force update of fileset-related info from FLDB/CDS
#
#     Returns: 1 - successful; 0 - failed
#
sub CheckFilesets {
    local($goterror);

    print "$CM checkfilesets\n";
    $goterror = system("$CM checkfilesets");
    !($goterror); # return result
}


# CreateFileset([IN] ftname, [IN] host, [IN] aggr) - create fileset on
#     specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub CreateFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS create -ftname $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS create $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# DeleteFileset([IN] ftname, [IN] host, [IN] aggr) - delete fileset on
#     specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub DeleteFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS delete -fileset $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS delete -fileset $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# SetRepFileset([IN] ftname, [IN] repvals) - set replication info for fileset
#
#     Returns: 1 - successful; 0 - failed
#
sub SetRepFileset {
    local($ftname, $repvals) = @_;
    local($goterror);

    print "$FTS setrepinfo -fileset $ftname $repvals\n";
    $goterror = system("$FTS setrepinfo -fileset $ftname $repvals");

    !($goterror); # return result
}


# AddSiteFileset([IN] ftname, [IN] host, [IN] aggr) - add replication site
#     for fileset on specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub AddSiteFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS addsite -fileset $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS addsite -fileset $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# RmSiteFileset([IN] ftname, [IN] host, [IN] aggr) - remove replication site
#     for fileset on specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub RmSiteFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS rmsite -fileset $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS rmsite -fileset $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# MountFileset([IN] mtdir, [IN] ftname, [IN] rwflag) - create mount point for
#     fileset; a RW mount point is created if rwflag is set.
#
#     Returns: 1 - successful; 0 - failed
#
sub MountFileset {
    local($mtdir, $ftname, $rwflag) = @_;
    local($goterror, $sysarg);

    print "$FTS crmount -dir $mtdir -fileset $ftname ";
    $sysarg = "$FTS crmount -dir $mtdir -fileset $ftname ";

    if ($rwflag) {
	print "-rw\n";
	$sysarg .= "-rw";
    } else {
	print "\n";
    }

    $goterror = system("$sysarg");

    !($goterror); # return result
}


# UmountFileset([IN] mtdir) - delete mount point for fileset
#
#     Returns: 1 - successful; 0 - failed
#
sub UmountFileset {
    local($mtdir) = @_;
    local($goterror);

    print "$FTS delmount -dir $mtdir\n";
    $goterror = system("$FTS delmount -dir $mtdir");

    !($goterror); # return result
}


# ChmodFileset([IN] prot, [IN] mtdir) - set file/dir/mount-point protection
#
#     Returns: 1 - successful; 0 - failed
#
sub ChmodFileset {
    local($prot, $mtdir) = @_;
    local($goterror);

    printf "chmod %o %s\n", $prot, $mtdir;
    $goterror = !(chmod($prot, $mtdir));

    !($goterror); # return result
}


# UpdateFileset([IN] ftname) - force immediate update of fileset replicas
#
#     Returns: 1 - successful; 0 - failed
#
sub UpdateFileset {
    local($ftname) = @_;
    local($goterror);

    print "$FTS update -fileset $ftname -all\n";
    $goterror = system("$FTS update -fileset $ftname -all");

    !($goterror); # return result
}


# ACLModify([IN] path, [IN] options) - modify ACL according to options
#
#     Returns: 1 - successful; 0 - failed
#
sub ACLModify {
    local($path, $options) = @_;
    local($goterror);

    print "$DCECP -c acl modify $path $options\n";
    $goterror = system("$DCECP -c acl modify $path $options");

    !($goterror);  # return result
}


# FXSetProtect([IN] host,
#              [IN] minlocal, [IN] maxlocal, [IN] minremote, [IN] maxremote)
#
#   Set the file-exporter protection-bounds on the specified host to
#   the indicated values.
#
#   NOTE: presumes file-exporter is already executing.
#
#     Returns: 1 - successful; 0 - failed
#
sub FXSetProtect {
    local($host, $minlocal, $maxlocal, $minremote, $maxremote) = @_;
    local($goterror, $cmd);

    $cmd  = "$RSH $host ";
    $cmd .= "$FXSECADVICE $minlocal $maxlocal $minremote $maxremote";

    print "$cmd\n";
    $goterror = system("$cmd");

    !($goterror);  # return result
}


1; # needed for perl require()
