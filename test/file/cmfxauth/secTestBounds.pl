#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Protection-bounds Tests
#
#   This test implements three sub-tests:
#     1) Non-intersecting protection-bounds - verifies that files can not be
#        accessed when CM-FX have non-intersecting protection bounds.
#
#     2) Intersecting protection-bounds - verifies that files can be accessed
#        when CM-FX have intersecting protection bounds; checks that
#        expected protection-level utilized.
#
#     3) Fileset protection-bounds - verifies that fileset protection bounds
#        influence the CM's initial protection-level choice appropriately.
#
#   The test takes as an argument a two letter code identifying the
#   credential-category for file accesses:
#     a) UN - Unauthenticated Non-local-root
#     b) UR - Unauthenticated local-Root
#     c) AN - dce-Authenticated Non-local-root
#     d) AR - dce-Authenticated local-Root
#
#   Local-identity: root
#   DCE-identity  : cell_admin
# ----------------------------------------------------------------------------

# main(credcat)
$| = 1;

print "\n\t\tCM-FX Authentication Negotiation Tests\n\n";

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($ENV{DFSXSECTEST_SRCDIR}) {
    unshift(@INC, "$ENV{DFSXSECTEST_SRCDIR}");
    $SECTEST_SRCDIR = $ENV{DFSXSECTEST_SRCDIR};
} else {
    unshift(@INC, ".");
    $SECTEST_SRCDIR = ".";
}

require("secconfig.pl");
require("secTestInclude.pl");
require("secTestUtil.pl");

# check arguments

if (@ARGV != 1) {
    print "$myExec: Incorrect argument count; error in test script?\n";
    exit(1);
}

$credcat = $ARGV[0];

if ($credcat eq "UN") {
    $readDceAuthn = 0;
    $readLclUser  = $LCL_USER;
    $authz_expect = $AUTHZ_SVC{"name"};    # unauthenticated access

    print "\t\t(Unauthenticated non-local-root)\n\n";

} elsif ($credcat eq "UR") {
    $readDceAuthn = 0;
    $readLclUser  = $LCL_ROOT;
    $authz_expect = $AUTHZ_SVC{"dce"};    # root authenticated as ..../self

    print "\t\t(Unauthenticated local-root)\n\n";

} elsif ($credcat eq "AN") {
    $readDceAuthn = 1;
    $readLclUser  = $LCL_USER;
    $authz_expect = $AUTHZ_SVC{"dce"};    # authenticated access

    print "\t\t(Authenticated non-local-root)\n\n";

} elsif ($credcat eq "AR") {
    $readDceAuthn = 1;
    $readLclUser  = $LCL_ROOT;
    $authz_expect = $AUTHZ_SVC{"dce"};    # authenticated access

    print "\t\t(Authenticated local-root)\n\n";

} else {
    print "$myExec: Invalid argument; error in test script?\n";
    exit(1);
}

$myExec = "$myExec($credcat)";



# 1) Non-intersecting protection-bounds test

print "\n\t\tNon-intersecting protection-bounds test\n\n";

# set per-fileset bounds wide-open and do a checkf to load into CM on access

$minl = $minr = $PROT_LVL_MIN;
$maxl = $maxr = $PROT_LVL_MAX;

print "$myExec: $FTS setprotect $LCL_FTNAME $minl $maxl $minr $maxr\n";
	
if (system("$FTS setprotect $LCL_FTNAME $minl $maxl $minr $maxr")) {
    print "$myExec: Error setting fileset protection levels\n";
    exit(1);
}

if (!defined($ENV{DFSXSECTEST_NOREMOTE})) {
    $cmd =
	"$FTS setprotect $RMT_FTNAME $minl $maxl $minr $maxr -cell $RMT_CELL";

    print "$myExec: $cmd\n";
	
    if (system("$cmd")) {
	print "$myExec: Error setting fileset protection levels\n";
	exit(1);
    }
}

print "$myExec: $CM checkf\n";

if (system("$CM checkf")) {
    print "$myExec: $CM checkf failed\n";
    exit(1);
}

print "\n";


# flush file data

print "$myExec: $CM flush $LCL_RWFILE\n";

if (system("$CM flush $LCL_RWFILE")) {
    print "$myExec: Error flushing $LCL_RWFILE\n";
    exit(1);
}

if (!defined($ENV{DFSXSECTEST_NOREMOTE})) {
    print "$myExec: $CM flush $RMT_RWFILE\n";

    if (system("$CM flush $RMT_RWFILE")) {
	print "$myExec: Error flushing $RMT_RWFILE\n";
	exit(1);
    }
}

print "\n";


# try all CM-FX non-overlapping protection bounds

for ($phase = 0; $phase <= 1; $phase++) {

    if ($phase == 0) {
	# local-cell access
	$filename = $LCL_RWFILE;
	$fxhost   = $LCL_HOSTA;

    } else {
	# remote-cell access
	$filename = $RMT_RWFILE;
	$fxhost   = $RMT_HOSTC;
    }

    for ($i = $PROT_LVL_MIN; $PROT_LVL_MAP{$i} < $PROT_LVL_MAX; $i++) {
	# skip if same as last iteration
	if ($i > $PROT_LVL_MIN && $PROT_LVL_MAP{$i - 1} == $PROT_LVL_MAP{$i}) {
	    next;
	}

	# set FX protection bounds
	$minl = $minr = $PROT_LVL_MAP{$PROT_LVL_MIN};
	$maxl = $maxr = $PROT_LVL_MAP{$i};

	print "$myExec: ";
	print "fx setprotectlevels $minl $maxl $minr $maxr (on $fxhost)\n";

	if (!(&FXSetProtect($fxhost, $minl, $maxl, $minr, $maxr))) {
	    print "$myExec: Error setting FX protection levels\n";
	    exit(1);
	}

	# set CM protection bounds
	$minl = $inil = $minr = $inir = $PROT_LVL_MAP{$i} + 1;

	print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

	if (system("$CM setprotectlevels $inil $minl $inir $minr")) {
	    print "$myExec: Error setting CM protection levels\n";
	    &Bailout();
	}

	print "\n";

	# sleep for fileset badness info timeout
	print "$myExec: Sleeping $FT_BADTIME seconds to let fileset badness ";
	print "info clear\n";

	sleep($FT_BADTIME);

	print "\n";

	# attempt to access file
	print "$myExec: $CAT $filename > /dev/null\n";

	$code = &ReadAuthn($readDceAuthn, $readLclUser, $filename);

	if ($code != 1) {
	    if ($code == 0) {
		print "$myExec: Error, should NOT be able to read $filename\n";
	    } else {
		print "$myExec: Error in fork/exec of worker\n";
	    }
	    &Bailout();
	}

	print "\n";
    }

    # reset FX protection bounds to be wide open
    $minl = $minr = $PROT_LVL_MIN;
    $maxl = $maxr = $PROT_LVL_MAX;

    print "$myExec: fx setprotectlevels $minl $maxl $minr $maxr ($fxhost)\n";

    if (!(&FXSetProtect($fxhost, $minl, $maxl, $minr, $maxr))) {
	print "$myExec: Error setting FX protection levels\n";
	exit(1);
    }
    print "\n";

    # determine if remote-cell access testing should be performed
    if (defined($ENV{DFSXSECTEST_NOREMOTE})) {
	last;
    }
}

# sleep for fileset badness info timeout

print "$myExec: Sleeping $FT_BADTIME seconds to let fileset badness ";
print "info clear\n";

sleep($FT_BADTIME);

print "\n";

# reset CM protection bounds to be wide open

$minl = $minr = $PROT_LVL_MIN;
$inil = $inir = $PROT_LVL_DEF;

print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

if (system("$CM setprotectlevels $inil $minl $inir $minr")) {
    print "$myExec: Error setting CM protection levels\n";
    exit(1);
}

print "\n";



# 2) Intersecting protection-bounds test

print "\n\t\tIntersecting protection-bounds test\n\n";

# set per-fileset bounds wide-open and do a checkf to load into CM on access

$minl = $minr = $PROT_LVL_MIN;
$maxl = $maxr = $PROT_LVL_MAX;

print "$myExec: $FTS setprotect $LCL_FTNAME $minl $maxl $minr $maxr\n";
	
if (system("$FTS setprotect $LCL_FTNAME $minl $maxl $minr $maxr")) {
    print "$myExec: Error setting fileset protection levels\n";
    exit(1);
}

if (!defined($ENV{DFSXSECTEST_NOREMOTE})) {
    $cmd =
	"$FTS setprotect $RMT_FTNAME $minl $maxl $minr $maxr -cell $RMT_CELL";

    print "$myExec: $cmd\n";
	
    if (system("$cmd")) {
	print "$myExec: Error setting fileset protection levels\n";
	exit(1);
    }
}

print "$myExec: $CM checkf\n";

if (system("$CM checkf")) {
    print "$myExec: $CM checkf failed\n";
    exit(1);
}

print "\n";

# set CM trace log

print "$myExec: $DFSTRACE setset -set cm -active\n";

if (system("$DFSTRACE setset -set cm -active")) {
    print "$myExec: Unable to manipulate CM trace logs\n";
    exit(1);
}

print "\n";

# try many CM-FX overlapping protection bounds

for ($phase = 0; $phase <= 1; $phase++) {

    if ($phase == 0) {
	# local-cell access
	$filename = $LCL_RWFILE;
	$fxhost   = $LCL_HOSTA;

    } else {
	# remote-cell access
	$filename = $RMT_RWFILE;
	$fxhost   = $RMT_HOSTC;
    }

    for ($i = $PROT_LVL_MIN; $i <= $PROT_LVL_MAX; $i++) {
	# skip if same as last iteration
	if ($i > $PROT_LVL_MIN && $PROT_LVL_MAP{$i - 1} == $PROT_LVL_MAP{$i}) {
	    next;
	}

	# set FX protection bounds
	$minl = $minr = $PROT_LVL_MAP{$PROT_LVL_MIN};
	$maxl = $maxr = $PROT_LVL_MAP{$i};

	print "$myExec: ";
	print "fx setprotectlevels $minl $maxl $minr $maxr (on $fxhost)\n";

	if (!(&FXSetProtect($fxhost, $minl, $maxl, $minr, $maxr))) {
	    print "$myExec: Error setting FX protection levels\n";
	    exit(1);
	}

	# set CM protection bounds
	$minl = $minr = $PROT_LVL_MAP{$PROT_LVL_MIN};
	$inil = $inir = $PROT_LVL_MAP{$i};

	if ($PROT_LVL_MAP{$i} < $PROT_LVL_MAX) {
	    # start out too high, if can
	    $inil++;
	    $inir++;
	}

	print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

	if (system("$CM setprotectlevels $inil $minl $inir $minr")) {
	    print "$myExec: Error setting CM protection levels\n";
	    &Bailout();
	}

	print "\n";

        # flush file data
	print "$myExec: $CM flush $filename\n";

	if (system("$CM flush $filename")) {
	    print "$myExec: Error flushing $filename\n";
	    &Bailout();
	}

	print "\n";

	# clear CM trace log
	print "$myExec: $DFSTRACE clear\n";

	if (system("$DFSTRACE clear")) {
	    print "$myExec: Unable to manipulate CM trace logs\n";
	    &Bailout();
	}

	print "\n";

	# attempt to access file
	print "$myExec: $CAT $filename > /dev/null\n";

	$code = &ReadAuthn($readDceAuthn, $readLclUser, $filename);

	if ($code != 0) {
	    if ($code == 1) {
		print "$myExec: Error, should be able to read $filename\n";
	    } else {
		print "$myExec: Error in fork/exec of worker\n";
	    }
	    &Bailout();
	}

	print "\n";

	# dump CM trace log and check that access occur at expected level
	# WARNING: heuristic; can be fooled; need better tracing
	print "$myExec: Searching CM log for access at expected level\n";

	if (!(open(INPIPE, "$DFSTRACE dump -set cm |"))) {
	    print "$myExec: Unable to dump CM log\n";
	    &Bailout();
	}

	$found = 0;

	while (<INPIPE>) {
	    if (/cm_ConnByHost.*service.*$FX_ID.*authn.*authz/) {
		($authn, $authz) = /authn\s*(\d*),.*authz\s*(\d*)/;

		if ($authn == $PROT_LVL_MAP{$i} &&
		    $authz == $authz_expect) {
		    $found = 1;
		    last;
		}
	    }
	}
	close(INPIPE);

	if ($found) {
	    print "$myExec: Found authenticated RPC to FX at expected level ";
	    print "($authn, $authz)\n";
	} else {
	    print "$myExec: Did not find authenticated RPC to FX ";
	    print "at expected level in log\n";
	    &Bailout();
	}

	print "\n";
    }

    # reset FX protection bounds to be wide open
    $minl = $minr = $PROT_LVL_MIN;
    $maxl = $maxr = $PROT_LVL_MAX;

    print "$myExec: fx setprotectlevels $minl $maxl $minr $maxr ($fxhost)\n";

    if (!(&FXSetProtect($fxhost, $minl, $maxl, $minr, $maxr))) {
	print "$myExec: Error setting FX protection levels\n";
	exit(1);
    }
    print "\n";

    # determine if remote-cell access testing should be performed
    if (defined($ENV{DFSXSECTEST_NOREMOTE})) {
	last;
    }
}

# reset CM protection bounds to be wide open

$minl = $minr = $PROT_LVL_MIN;
$inil = $inir = $PROT_LVL_DEF;

print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

if (system("$CM setprotectlevels $inil $minl $inir $minr")) {
    print "$myExec: Error setting CM protection levels\n";
    exit(1);
}

print "\n";



# 3) Fileset protection-bounds test

print "\n\t\tFileset protection-bounds test\n\n";

# set CM trace log

print "$myExec: $DFSTRACE setset -set cm -active\n";

if (system("$DFSTRACE setset -set cm -active")) {
    print "$myExec: Unable to manipulate CM trace logs\n";
    exit(1);
}

print "\n";

# try full range of fileset protection bounds

for ($phase = 0; $phase <= 1; $phase++) {

    if ($phase == 0) {
	# local-cell access
	$filecell = $LCL_CELL;
	$filename = $LCL_RWFILE;
	$fileset  = $LCL_FTNAME;
	$fxhost   = $LCL_HOSTA;

    } else {
	# remote-cell access
	$filecell = $RMT_CELL;
	$filename = $RMT_RWFILE;
	$fileset  = $RMT_FTNAME;
	$fxhost   = $RMT_HOSTC;
    }

    # set FX protection bounds wide-open
    $minl = $minr = $PROT_LVL_MIN;
    $maxl = $maxr = $PROT_LVL_MAX;

    print "$myExec: ";
    print "fx setprotectlevels $minl $maxl $minr $maxr (on $fxhost)\n";

    if (!(&FXSetProtect($fxhost, $minl, $maxl, $minr, $maxr))) {
	print "$myExec: Error setting FX protection levels\n";
	exit(1);
    }

    # set CM protection bounds wide-open
    $minl = $minr = $PROT_LVL_MIN;
    $inil = $inir = $PROT_LVL_DEF;

    print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

    if (system("$CM setprotectlevels $inil $minl $inir $minr")) {
	print "$myExec: Error setting CM protection levels\n";
	exit(1);
    }

    print "\n";

    for ($i = $PROT_LVL_MIN; $i <= $PROT_LVL_MAX; $i++) {
	# skip if same as last iteration
	if ($i > $PROT_LVL_MIN && $PROT_LVL_MAP{$i - 1} == $PROT_LVL_MAP{$i}) {
	    next;
	}

	# set fileset bounds and do a checkf to load into CM on access
	$minl = $maxl = $minr = $maxr = $PROT_LVL_MAP{$i};

	$cmd =
	    "$FTS setprotect $fileset $minl $maxl $minr $maxr -cell $filecell";

	print "$myExec: $cmd\n";

	if (system("$cmd")) {
	    print "$myExec: Error setting fileset protection levels\n";
	    exit(1);
	}

	print "$myExec: $CM checkf\n";

	if (system("$CM checkf")) {
	    print "$myExec: $CM checkf failed\n";
	    exit(1);
	}

	print "\n";

        # flush file data
	print "$myExec: $CM flush $filename\n";

	if (system("$CM flush $filename")) {
	    print "$myExec: Error flushing $filename\n";
	    exit(1);
	}

	print "\n";

	# clear CM trace log
	print "$myExec: $DFSTRACE clear\n";

	if (system("$DFSTRACE clear")) {
	    print "$myExec: Unable to manipulate CM trace logs\n";
	    exit(1);
	}

	print "\n";

	# attempt to access file
	print "$myExec: $CAT $filename > /dev/null\n";

	$code = &ReadAuthn($readDceAuthn, $readLclUser, $filename);

	if ($code != 0) {
	    if ($code == 1) {
		print "$myExec: Error, should be able to read $filename\n";
	    } else {
		print "$myExec: Error in fork/exec of worker\n";
	    }
	    exit(1);
	}

	print "\n";

	# dump CM trace log and check that access occur at expected level
	# WARNING: heuristic; can be fooled; need better tracing
	print "$myExec: Searching CM log for access at expected level\n";

	if (!(open(INPIPE, "$DFSTRACE dump -set cm |"))) {
	    print "$myExec: Unable to dump CM log\n";
	    exit(1);
	}

	$found = 0;

	while (<INPIPE>) {
	    if (/cm_ConnByHost.*service.*$FX_ID.*authn.*authz/) {
		($authn, $authz) = /authn\s*(\d*),.*authz\s*(\d*)/;

		if ($authn == $PROT_LVL_MAP{$i} &&
		    $authz == $authz_expect) {
		    $found = 1;
		    last;
		}
	    }
	}
	close(INPIPE);

	if ($found) {
	    print "$myExec: Found authenticated RPC to FX at expected level ";
	    print "($authn, $authz)\n";
	} else {
	    print "$myExec: Did not find authenticated RPC to FX ";
	    print "at expected level in log\n";
	    exit(1);
	}

	print "\n";
    }

    # determine if remote-cell access testing should be performed
    if (defined($ENV{DFSXSECTEST_NOREMOTE})) {
	last;
    }
}

print "\n";


exit(0);


# ---------------------------------------------------------------------------


# ReadAuthn(dceAuthn, lclUser, filepath) - read file in context of specified
#     credential-category.
#
#  Returns: 0 - read successful; 1 - read failed; 2 - fork/exec failed
#
sub ReadAuthn {
    local($dceAuthn, $lclUser, $filepath) = @_;
    local($readCmd, $dceUser, $dcePasswd);

    $readCmd = "$SECTEST_SRCDIR/secTestBoundsWorker.pl $filepath";

    if ($dceAuthn) {
	$dceUser   = $LCL_CELL_ADMIN;
	$dcePasswd = $LCL_CELL_ADMIN_PASSWD;
    } else {
	$dceUser   = "";
	$dcePasswd = "";
    }

    if (&ForkTestAuthn($myExec,
		       $readCmd,
		       $lclUser, $dceUser, $dcePasswd, $dceAuthn)) {
	# fork successful
	wait;  # sets $?

	if (($? / 256) == 0) {
	    # read successful
	    return 0;
	} elsif (($? / 256) == 1) {
	    # failed to execute worker correctly
	    return 2;
	} else {
	    # read failed
	    return 1;
	}

    } else {
	# fork failed
	return 2;
    }
}


# Bailout() - do some cleanup and exit with an error status
#
sub Bailout {
    local($minl, $minr, $maxl, $maxr, $inil, $inir);

    # set FX protection bounds wide-open
    $minl = $minr = $PROT_LVL_MIN;
    $maxl = $maxr = $PROT_LVL_MAX;

    print "$myExec: ";
    print "fx setprotectlevels $minl $maxl $minr $maxr (on $LCL_HOSTA)\n";

    &FXSetProtect($LCL_HOSTA, $minl, $maxl, $minr, $maxr);

    if (!defined($ENV{DFSXSECTEST_NOREMOTE})) {
	print "$myExec: ";
	print "fx setprotectlevels $minl $maxl $minr $maxr (on $RMT_HOSTC)\n";

	&FXSetProtect($RMT_HOSTC, $minl, $maxl, $minr, $maxr);
    }

    # set CM protection bounds wide-open
    $minl = $minr = $PROT_LVL_MIN;
    $inil = $inir = $PROT_LVL_DEF;

    print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

    system("$CM setprotectlevels $inil $minl $inir $minr");

    # done
    exit(1);
}
