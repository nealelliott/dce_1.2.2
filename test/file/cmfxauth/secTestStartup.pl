#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Startup Test
#
#   This test checks that fxd and dfsd protection-level options are
#   handled correctly in terms of bounds checking, etc.

#   Local-identity: non-root
#   DCE-identity  : none required

# ----------------------------------------------------------------------------

# main()
$| = 1;

print "\n\t\tUI Extensions Test - Startup Options\n\n";

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($ENV{DFSXSECTEST_SRCDIR}) {
    unshift(@INC, "$ENV{DFSXSECTEST_SRCDIR}");
} else {
    unshift(@INC, ".");
}

require("secconfig.pl");
require("secTestInclude.pl");
require("secTestUtil.pl");

# check that unable to really execute fxd and dfsd

$output = &Backtick("$FXD -admin subsys/dce/dfs-admin");

if (!($?)) {
    print "$myExec: Able to really execute $FXD; error in test script?\n";
    exit(1);
}

$output = &Backtick("$DFSD");

if (!($?)) {
    print "$myExec: Able to really execute $DFSD; error in test script?\n";
    exit(1);
}



# 1) test fxd protection-level options

# check that can set levels to all valid values, for all three name-forms

for ($form = 0; $form < 3; $form++) {
    for ($i = $PROT_LVL_MIN; $i <= $PROT_LVL_MAX; $i++) {
	if ($form == 0) {
	    # numeric name-form
	    $minl = $maxl = $minr = $maxr = $i;
	} elsif ($form == 1) {
	    # abbreviated name-form
	    $minl = $maxl = $minr = $maxr = $PROT_LVL{$i};
	} else {
	    # full name-form
	    $minl = $maxl = $minr = $maxr = "$PROT_LVL_PREFIX$PROT_LVL{$i}";
	}

	$cmd  = "$FXD -admin subsys/dce/dfs-admin ";
	$cmd .= "-minlocal $minl -maxlocal $maxl ";
	$cmd .= "-minremote $minr -maxremote $maxr";

	print "$myExec: $cmd\n";

	$output = &Backtick("$cmd");

	if (!($output =~ /requires root access/)) {
	    print "$myExec: Error setting valid $FXD protection levels\n";
	    exit(1);
	}
    }
    print "\n";
}

# check that can not set min level greater than max level

print "\n$myExec: $FXD -admin subsys/dce/dfs-admin -minlocal 2 -maxlocal 1\n";

$output =
    &Backtick("$FXD -admin subsys/dce/dfs-admin -minlocal 2 -maxlocal 1");

if (!($output =~ /minlocal.*larger.*maxlocal/)) {
    print "$myExec: Invalid $FXD protection level not detected\n";
    exit(1);
}

print "\n$myExec: ";
print "$FXD -admin subsys/dce/dfs-admin -minremote 2 -maxremote 1\n";

$output =
    &Backtick("$FXD -admin subsys/dce/dfs-admin -minremote 2 -maxremote 1");

if (!($output =~ /minremote.*larger.*maxremote/)) {
    print "$myExec: Invalid $FXD protection level not detected\n";
    exit(1);
}

# check that can not specify an out-of-range level

print "\n$myExec: $FXD -admin subsys/dce/dfs-admin -minlocal -1\n";

$output = &Backtick("$FXD -admin subsys/dce/dfs-admin -minlocal -1");

if (!($output =~ /not.*legal.*minlocal/)) {
    print "$myExec: Invalid $FXD protection level not detected\n";
    exit(1);
}

$maxl = $PROT_LVL_MAX + 1;
print "\n$myExec: $FXD -admin subsys/dce/dfs-admin -maxlocal $maxl\n";

$output = &Backtick("$FXD -admin subsys/dce/dfs-admin -maxlocal $maxl");

if (!($output =~ /not.*legal.*maxlocal/)) {
    print "$myExec: Invalid $FXD protection level not detected\n";
    exit(1);
}

print "\n$myExec: $FXD -admin subsys/dce/dfs-admin -minremote -1\n";

$output = &Backtick("$FXD -admin subsys/dce/dfs-admin -minremote -1");

if (!($output =~ /not.*legal.*minremote/)) {
    print "$myExec: Invalid $FXD protection level not detected\n";
    exit(1);
}

$maxr = $PROT_LVL_MAX + 1;
print "\n$myExec: $FXD -admin subsys/dce/dfs-admin -maxremote $maxr\n";

$output = &Backtick("$FXD -admin subsys/dce/dfs-admin -maxremote $maxr");

if (!($output =~ /not.*legal.*maxremote/)) {
    print "$myExec: Invalid $FXD protection level not detected\n";
    exit(1);
}

print "\n\n";



# 2) test dfsd protection-level options

# check that can set levels to all valid values, for all three name-forms

for ($form = 0; $form < 3; $form++) {
    for ($i = $PROT_LVL_MIN; $i <= $PROT_LVL_MAX; $i++) {
	if ($form == 0) {
	    # numeric name-form
	    $minl = $inil = $minr = $inir = $i;
	} elsif ($form == 1) {
	    # abbreviated name-form
	    $minl = $inil = $minr = $inir = $PROT_LVL{$i};
	} else {
	    # full name-form
	    $minl = $inil = $minr = $inir = "$PROT_LVL_PREFIX$PROT_LVL{$i}";
	}

	$cmd  = "$DFSD -minlocal $minl -initiallocal $inil ";
	$cmd .= "-minremote $minr -initialremote $inir";

	print "$myExec: $cmd\n";

	$output = &Backtick("$cmd");

	if (!($output =~ /failed to invoke/)) {
	    print "$myExec: Error setting valid $DFSD protection levels\n";
	    exit(1);
	}
    }
    print "\n";
}

# check that can not set min level greater than initial level

print "\n$myExec: $DFSD -minlocal 2 -initiallocal 1\n";

$output = &Backtick("$DFSD -minlocal 2 -initiallocal 1");

if (!($output =~ /minlocal.*exceed.*initiallocal/)) {
    print "$myExec: Invalid $DFSD protection level not detected\n";
    exit(1);
}

print "\n$myExec: $DFSD -minremote 2 -initialremote 1\n";

$output = &Backtick("$DFSD -minremote 2 -initialremote 1");

if (!($output =~ /minremote.*exceed.*initialremote/)) {
    print "$myExec: Invalid $DFSD protection level not detected\n";
    exit(1);
}

# check that can not specify an out-of-range level

print "\n$myExec: $DFSD -minlocal -1\n";

$output = &Backtick("$DFSD -minlocal -1");

if (!($output =~ /not.*valid.*minlocal/)) {
    print "$myExec: Invalid $DFSD protection level not detected\n";
    exit(1);
}

$inil = $PROT_LVL_MAX + 1;
print "\n$myExec: $DFSD -initiallocal $inil\n";

$output = &Backtick("$DFSD -initiallocal $inil");

if (!($output =~ /not.*valid.*initiallocal/)) {
    print "$myExec: Invalid $DFSD protection level not detected\n";
    exit(1);
}

print "\n$myExec: $DFSD -minremote -1\n";

$output = &Backtick("$DFSD -minremote -1");

if (!($output =~ /not.*valid.*minremote/)) {
    print "$myExec: Invalid $DFSD protection level not detected\n";
    exit(1);
}

$inir = $PROT_LVL_MAX + 1;
print "\n$myExec: $DFSD -initialremote $inir\n";

$output = &Backtick("$DFSD -initialremote $inir");

if (!($output =~ /not.*valid.*initialremote/)) {
    print "$myExec: Invalid $DFSD protection level not detected\n";
    exit(1);
}

print "\n";

exit(0);
