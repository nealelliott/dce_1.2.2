#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Repserver-authentication Test
#
#   This test verifies that the fts, CM, and repserver clients all make
#   authenticated repserver RPCs.
#
#   Local-identity: root
#   DCE-identity  : cell_admin
# ----------------------------------------------------------------------------

# main()
$| = 1;

print "\n\t\tServer RPC Authentication Test - Repserver\n\n";

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($ENV{DFSXSECTEST_SRCDIR}) {
    unshift(@INC, "$ENV{DFSXSECTEST_SRCDIR}");
} else {
    unshift(@INC, ".");
}

require("secconfig.pl");
require("secTestInclude.pl");


# clear the repservers' trace logs

$repA = "$LCL_HOSTA_DIR/repserver";
$repB = "$LCL_HOSTB_DIR/repserver";

print "$myExec: $DFSTRACE clear -cds $repA\n";

if (system("$DFSTRACE clear -cds $repA")) {
    print "$myExec: Unable to manipulate $repA trace logs\n";
    exit(1);
}

print "$myExec: $DFSTRACE clear -cds $repB\n";

if (system("$DFSTRACE clear -cds $repB")) {
    print "$myExec: Unable to manipulate $repB trace logs\n";
    exit(1);
}

print "\n";


# issue a setrepinfo command to generate fts-->rep and rep-->rep RPCs

print "$myExec: ";
print "$FTS setrepinfo -fileset $LCL_FTNAME -maxage ${REPAUTH_MAXAGE}s\n";

if (system("$FTS setrepinfo -file $LCL_FTNAME -maxage ${REPAUTH_MAXAGE}s")) {
    print "$myExec: Unable to perform setrepinfo\n";
    exit(1);
}

print "$myExec: Sleeping $SLEEPTIME secs to ensure rep info is propagated\n";
sleep($SLEEPTIME);

print "\n";


# dump rep trace logs and check that RPCs were authenticated

print "$myExec: $DFSTRACE dump -cds $repA\n";

if (system("$DFSTRACE dump -cds $repA")) {
    print "$myExec: Unable to manipulate $repA trace logs\n";
    exit(1);
}

print "$myExec: $DFSTRACE dump -cds $repB\n";

if (system("$DFSTRACE dump -cds $repB")) {
    print "$myExec: Unable to manipulate $repB trace logs\n";
    exit(1);
}

print "\n";

print "$myExec: Searching for authenticated REP_{All}CheckReplicationConfig ";
print "to $repA\n";

if (!(open(INPIPE, "$RSH $LCL_HOSTA $CAT $LCL_HOSTA_LOG/icl.repserver |"))) {
    print "$myExec: Can't read $LCL_HOSTA_LOG/icl.repserver on $LCL_HOSTA\n";
    exit(1);
}

$found = 0;

while (<INPIPE>) {
    if (/SREP_.*CheckReplicationConfig.*called us/) {
	($princ, $authnlvl) = /us\s*(\S*),.*level\s*(\d*)/;
	$found = 1;
	last;
    }
}
close(INPIPE);

if ($found) {
    print "$myExec: Found authn RPC to $repA ($princ, $authnlvl)\n";
} else {
    print "$myExec: Can't read $LCL_HOSTA_LOG/icl.repserver on $LCL_HOSTA, ";
    print "or did not find authenticated RPC in log\n";
    exit(1);
}

print "$myExec: Searching for authenticated REP_{All}CheckReplicationConfig ";
print "to $repB\n";

if (!(open(INPIPE, "$RSH $LCL_HOSTB $CAT $LCL_HOSTB_LOG/icl.repserver |"))) {
    print "$myExec: Can't read $LCL_HOSTB_LOG/icl.repserver on $LCL_HOSTB\n";
    exit(1);
}

$found = 0;

while (<INPIPE>) {
    if (/SREP_.*CheckReplicationConfig.*called us/) {
	($princ, $authnlvl) = /us\s*(\S*),.*level\s*(\d*)/;
	$found = 1;
	last;
    }
}
close(INPIPE);

if ($found) {
    print "$myExec: Found authn RPC to $repB ($princ, $authnlvl)\n";
} else {
    print "$myExec: Can't read $LCL_HOSTB_LOG/icl.repserver on $LCL_HOSTB, ";
    print "or did not find authenticated RPC in log\n";
    exit(1);
}

print "\n";


# read file, update, and wait for maxage to expire

print "$myExec: $DFSTRACE clear -cds $repA\n";

if (system("$DFSTRACE clear -cds $repA")) {
    print "$myExec: Unable to manipulate $repA trace logs\n";
    exit(1);
}

print "$myExec: $DFSTRACE clear -cds $repB\n";

if (system("$DFSTRACE clear -cds $repB")) {
    print "$myExec: Unable to manipulate $repB trace logs\n";
    exit(1);
}

print "\n";

print "$myExec: $CAT $LCL_ROFILE > /dev/null\n";

if (system("$CAT $LCL_ROFILE > /dev/null")) {
    print "$myExec: Unable to read file $LCL_ROFILE\n";
    exit(1);
}

print "$myExec: $TOUCH $LCL_RWFILE\n";

if (system("$TOUCH $LCL_RWFILE")) {
    print "$myExec: Unable to update file $LCL_RWFILE\n";
    exit(1);
}

print "$myExec: Sleeping maxage of $LCL_FTNAME ($REPAUTH_MAXAGE seconds)\n";
sleep($REPAUTH_MAXAGE);

print "\n";


# read file to generate CM-->rep RPC

print "$myExec: $CAT $LCL_ROFILE > /dev/null\n";

if (system("$CAT $LCL_ROFILE > /dev/null")) {
    print "$myExec: Unable to read file $LCL_ROFILE\n";
    exit(1);
}

print "\n";


# dump rep trace logs and check that RPC was authenticated

print "$myExec: $DFSTRACE dump -cds $repA\n";

if (system("$DFSTRACE dump -cds $repA")) {
    print "$myExec: Unable to manipulate $repA trace logs\n";
    exit(1);
}

print "$myExec: $DFSTRACE dump -cds $repB\n";

if (system("$DFSTRACE dump -cds $repB")) {
    print "$myExec: Unable to manipulate $repB trace logs\n";
    exit(1);
}

print "\n";

print "$myExec: Searching for authenticated REP_GetVolChangedFiles ";
print "to $repA or $repB\n";

if (!(open(INPIPE, "$RSH $LCL_HOSTA $CAT $LCL_HOSTA_LOG/icl.repserver |"))) {
    print "$myExec: Can't read $LCL_HOSTA_LOG/icl.repserver on $LCL_HOSTA\n";
    exit(1);
}

$found = 0;

while (<INPIPE>) {
    if (/SREP_GetVolChangedFiles.*called us/) {
	($princ, $authnlvl) = /us\s*(\S*),.*level\s*(\d*)/;
	$found = 1;
	$foundat = $repA;
	last;
    }
}
close(INPIPE);

if (!($found)) {
    if (!(open(INPIPE,
	       "$RSH $LCL_HOSTB $CAT $LCL_HOSTB_LOG/icl.repserver |"))) {
	print "$myExec: ";
	print "Can't read $LCL_HOSTB_LOG/icl.repserver on $LCL_HOSTB\n";
	exit(1);
    }

    while (<INPIPE>) {
	if (/SREP_GetVolChangedFiles.*called us/) {
	    ($princ, $authnlvl) = /us\s*(\S*),.*level\s*(\d*)/;
	    $found = 1;
	    $foundat = $repB;
	    last;
	}
    }
    close(INPIPE);
}

if ($found) {
    print "$myExec: Found authn RPC to $foundat ($princ, $authnlvl)\n";
} else {
    print "$myExec: Can't read $LCL_HOSTA_LOG/icl.repserver on $LCL_HOSTA, ";
    print "or can't read $LCL_HOSTB_LOG/icl.repserver on $LCL_HOSTB, ";
    print "or did not find authenticated RPC in logs\n";
    exit(1);
}

print "\n";
exit(0);
