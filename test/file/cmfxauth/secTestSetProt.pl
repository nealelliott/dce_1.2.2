#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Setprotection Test
#
#   This test checks the functionality of the 'fts setprotectlevels',
#   'cm setprotectlevels', and 'cm getprotectlevels' commands.
#   Note that we are only looking at value bounds checking, etc., as well
#   as testing that the values are actually set.
#
#   This test does NOT check that protection levels are actually obeyed.
#
#   Local-identity: root
#   DCE-identity  : cell_admin
# ----------------------------------------------------------------------------

# main()
$| = 1;

print "\n\t\tUI Extensions Test - Set Protection Levels\n\n";

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($ENV{DFSXSECTEST_SRCDIR}) {
    unshift(@INC, "$ENV{DFSXSECTEST_SRCDIR}");
} else {
    unshift(@INC, ".");
}

require("secconfig.pl");
require("secTestInclude.pl");



# 1) test 'fts setprotectlevels' command

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
	
	print "$myExec: ";
	print "$FTS setprotectlevels $LCL_FTNAME $minl $maxl $minr $maxr\n";
	
	if (system("$FTS setprotect $LCL_FTNAME $minl $maxl $minr $maxr")) {
	    print "$myExec: Error setting valid fileset protection levels\n";
	    exit(1);
	}
	$output = `$FTS lsfldb $LCL_FTNAME`;
	
	if ($?) {
	    print "$myExec: ";
	    print "Fileset $LCL_FTNAME not found; error in test script?\n";
	    exit(1);
	}
	
	if (!(&CheckFilesetProt($output, $i, $i, $i, $i))) {
	    print "$myExec: Error setting valid fileset protection levels\n";
	    exit(1);
	}
    }
    print "\n";
}

# check that can not set min level greater than max level

print "\n$myExec: $FTS setprotectlevels $LCL_FTNAME -minlocal 2 -maxlocal 1\n";

if (!system("$FTS setprotectlevels $LCL_FTNAME -minlocal 2 -maxlocal 1")) {
    print "$myExec: Invalid fileset protection level not detected\n";
    exit(1);
}

print "\n$myExec: ";
print "$FTS setprotectlevels $LCL_FTNAME -minremote 2 -maxremote 1\n";

if (!system("$FTS setprotectlevels $LCL_FTNAME -minremote 2 -maxremote 1")) {
    print "$myExec: Invalid fileset protection level not detected\n";
    exit(1);
}

# check that can not specify an out-of-range level

print "\n$myExec: $FTS setprotectlevels $LCL_FTNAME -minlocal -1\n";

if (!system("$FTS setprotectlevels $LCL_FTNAME -minlocal -1")) {
    print "$myExec: Invalid fileset protection level not detected\n";
    exit(1);
}

$maxl = $PROT_LVL_MAX + 1;
print "\n$myExec: $FTS setprotectlevels $LCL_FTNAME -maxlocal $maxl\n";

if (!system("$FTS setprotectlevels $LCL_FTNAME -maxlocal $maxl")) {
    print "$myExec: Invalid fileset protection level not detected\n";
    exit(1);
}

print "\n$myExec: $FTS setprotectlevels $LCL_FTNAME -minremote -1\n";

if (!system("$FTS setprotectlevels $LCL_FTNAME -minremote -1")) {
    print "$myExec: Invalid fileset protection level not detected\n";
    exit(1);
}

$maxr = $PROT_LVL_MAX + 1;
print "\n$myExec: $FTS setprotectlevels $LCL_FTNAME -maxremote $maxr\n";

if (!system("$FTS setprotectlevels $LCL_FTNAME -maxremote $maxr")) {
    print "$myExec: Invalid fileset protection level not detected\n";
    exit(1);
}

print "\n";


# 2) test 'cm setprotectlevels' and 'cm getprotectlevels' commands

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

	print "$myExec: $CM setprotectlevels $inil $minl $inir $minr\n";

	if (system("$CM setprotectlevels $inil $minl $inir $minr")) {
	    print "$myExec: Error setting valid CM protection levels\n";
	    exit(1);
	}
	$output = `$CM getprotectlevels`;

	if ($?) {
	    print "$myExec: Error getting current CM protection levels\n";
	    exit(1);
	}

	if (!(&CheckCMProt($output, $i, $i, $i, $i))) {
	    print "$myExec: Error setting valid CM protection levels\n";
	    exit(1);
	}
    }
    print "\n";
}


# check that can not set min level greater than initial level

print "\n$myExec: $CM setprotectlevels -initiallocal 1 -minlocal 2\n";

if (!system("$CM setprotectlevels -initiallocal 1 -minlocal 2")) {
    print "$myExec: Invalid CM protection level not detected\n";
    exit(1);
}

print "\n$myExec: $CM setprotectlevels -initialremote 1 -minremote 2\n";

if (!system("$CM setprotectlevels -initialremote 1 -minremote 2")) {
    print "$myExec: Invalid CM protection level not detected\n";
    exit(1);
}

# check that can not specify an out-of-range level

print "\n$myExec: $CM setprotectlevels -minlocal -1\n";

if (!system("$CM setprotectlevels -minlocal -1")) {
    print "$myExec: Invalid CM protection level not detected\n";
    exit(1);
}

$inil = $PROT_LVL_MAX + 1;
print "\n$myExec: $CM setprotectlevels -initiallocal $inil\n";

if (!system("$CM setprotectlevels -initiallocal $inil")) {
    print "$myExec: Invalid CM protection level not detected\n";
    exit(1);
}

print "\n$myExec: $CM setprotectlevels -minremote -1\n";

if (!system("$CM setprotectlevels -minremote -1")) {
    print "$myExec: Invalid CM protection level not detected\n";
    exit(1);
}

$inir = $PROT_LVL_MAX + 1;
print "\n$myExec: $CM setprotectlevels -initialremote $inir\n";

if (!system("$CM setprotectlevels -initialremote $inir")) {
    print "$myExec: Invalid CM protection level not detected\n";
    exit(1);
}

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
exit(0);


# ----------------------------------------------------------------------------

# CheckFilesetProt(output, minlcl, maxlcl, minrmt, maxrmt)
#     Check FLDB output to see that all prot levels are set as specified.
#
#     Returns: 1 - successful; 0 - failed
#
sub CheckFilesetProt{
    local($output, $minl, $maxl, $minr, $maxr) = @_;
    local($goterror, $pl_minl, $pl_maxl, $pl_minr, $pl_maxr);

    $goterror = 0;

    $pl_minl = "$PROT_LVL_PREFIX$PROT_LVL{$minl}";
    $pl_maxl = "$PROT_LVL_PREFIX$PROT_LVL{$maxl}";
    $pl_minr = "$PROT_LVL_PREFIX$PROT_LVL{$minr}";
    $pl_maxr = "$PROT_LVL_PREFIX$PROT_LVL{$maxr}";

    if ($output !~ /$FLDB_MINLCL_STR\s*$pl_minl/) {
	print "$myExec: Error setting min local protection level\n";
	$goterror = 1;
    } elsif ($output !~ /$FLDB_MAXLCL_STR\s*$pl_maxl/) {
	print "$myExec: Error setting max local protection level\n";
	$goterror = 1;
    } elsif ($output !~ /$FLDB_MINRMT_STR\s*$pl_minr/) {
	print "$myExec: Error setting min remote protection level\n";
	$goterror = 1;
    } elsif ($output !~ /$FLDB_MAXRMT_STR\s*$pl_maxr/) {
	print "$myExec: Error setting max remote protection level\n";
	$goterror = 1;
    }

    !($goterror);
}


# CheckCMProt(output, inilcl, minlcl, inirmt, minrmt)
#     Check CM output to see that all prot levels are set as specified.
#
#     Returns: 1 - successful; 0 - failed
#
sub CheckCMProt{
    local($output, $inil, $minl, $inir, $minr) = @_;
    local($goterror, $pl_inil, $pl_minl, $pl_inir, $pl_minr);

    $goterror = 0;

    $pl_minl = "$PROT_LVL_PREFIX$PROT_LVL{$minl}";
    $pl_inil = "$PROT_LVL_PREFIX$PROT_LVL{$inil}";
    $pl_minr = "$PROT_LVL_PREFIX$PROT_LVL{$minr}";
    $pl_inir = "$PROT_LVL_PREFIX$PROT_LVL{$inir}";

    if ($output !~ /$CM_MINLCL_STR\s*$pl_minl/) {
	print "$myExec: Error setting min local protection level\n";
	$goterror = 1;
    } elsif ($output !~ /$CM_INILCL_STR\s*$pl_inil/) {
	print "$myExec: Error setting initial local protection level\n";
	$goterror = 1;
    } elsif ($output !~ /$CM_MINRMT_STR\s*$pl_minr/) {
	print "$myExec: Error setting min remote protection level\n";
	$goterror = 1;
    } elsif ($output !~ /$CM_INIRMT_STR\s*$pl_inir/) {
	print "$myExec: Error setting initial remote protection level\n";
	$goterror = 1;
    }

    !($goterror);
}
