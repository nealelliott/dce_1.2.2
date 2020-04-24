#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Token-revoke Test
#
#   This test verifies that the fts, dfsexport, CM and repserver clients
#   supply a principal name in AFS_SetContext calls so that the FX can
#   authenticate token revoke RPCs.
#
#   Local-identity: root
#   DCE-identity  : cell_admin
# ----------------------------------------------------------------------------

# main()
$| = 1;

print "\n\t\tToken-revoke RPC Authentication Test\n\n";

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


# set and clear the fshost trace logs

print "$myExec: $DFSTRACE setset -set fshost -active (on $LCL_HOSTA)\n";

$output = &Backtick("$RSH $LCL_HOSTA $DFSTRACE setset -set fshost -active");

if (($?) || ($output ne "")) {
    print "$myExec: Unable to manipulate CM trace logs (on $LCL_HOSTA)\n";
    exit(1);
}

print "$myExec: $DFSTRACE clear (on $LCL_HOSTA)\n";

$output = &Backtick("$RSH $LCL_HOSTA $DFSTRACE clear");

if (($?) || ($output ne "")) {
    print "$myExec: Unable to manipulate CM trace logs (on $LCL_HOSTA)\n";
    exit(1);
}

print "\n";


# issue a clone command to generate an fts-->FX set-context RPC (new)

print "$myExec: $FTS clone $LCL_FTNAME\n";

if (system("$FTS clone $LCL_FTNAME")) {
    print "$myExec: Unable to clone $LCL_FTNAME\n";
    exit(1);
}

print "\n";


# dump fshost logs and check that fts set-context passed in principal

print "$myExec: Searching FX log for set-context principal (on $LCL_HOSTA)\n";

if (&TraceCreateHostAuth($LCL_HOSTA, *princ)) {
    print "$myExec: Found set-context principal ($princ)\n";
} else {
    print "$myExec: Unable to dump FX log (on $LCL_HOSTA), ";
    print "or did not find set-context principal in log\n";
    exit(1);
}

print "\n";


# issue export command to generate a dfsexport-->FX set-context RPC (new)

print "$myExec: $DFSTRACE clear (on $LCL_HOSTA)\n";

$output = &Backtick("$RSH $LCL_HOSTA $DFSTRACE clear");

if (($?) || ($output ne "")) {
    print "$myExec: Unable to manipulate CM trace logs (on $LCL_HOSTA)\n";
    exit(1);
}

print "\n";

print "$myExec: ";
print "$DFSEXPORT -aggr $LCL_HOSTA_AGGR -detach (on $LCL_HOSTA)\n";

$output = &Backtick("$RSH $LCL_HOSTA $DFSEXPORT " .
		    "-aggr $LCL_HOSTA_AGGR -detach");

# note: don't care if detach fails; in fact, it likely will

print "$myExec: $DFSEXPORT -aggr $LCL_HOSTA_AGGR (on $LCL_HOSTA)\n";

$output = &Backtick("$RSH $LCL_HOSTA $DFSEXPORT -aggr $LCL_HOSTA_AGGR");

# note: don't care if attach fails; in fact, it likely will
print "\n";


# dump fshost logs and check that dfsexport set-context passed in principal

print "$myExec: Searching FX log for set-context principal (on $LCL_HOSTA)\n";

if (&TraceCreateHostAuth($LCL_HOSTA, *princ)) {
    print "$myExec: Found set-context principal ($princ)\n";
} else {
    print "$myExec: Unable to dump FX log (on $LCL_HOSTA), ";
    print "or did not find set-context principal in log\n";
    exit(1);
}

print "\n";


# issue repserver restart to generate a repserver-->FX set-context RPC (new)

print "$myExec: $DFSTRACE clear (on $LCL_HOSTA)\n";

$output = &Backtick("$RSH $LCL_HOSTA $DFSTRACE clear");

if (($?) || ($output ne "")) {
    print "$myExec: Unable to manipulate CM trace logs (on $LCL_HOSTA)\n";
    exit(1);
}

print "\n";

print "$myExec: $BOS restart $LCL_HOSTA_DIR -proc repserver\n";

if (system("$BOS restart $LCL_HOSTA_DIR -proc repserver")) {
    print "$myExec: Unable to restart repserver (on $LCL_HOSTA)\n";
    exit(1);
}

print "$myExec: ";
print "Sleeping $SLEEPTIME secs to let repserver start (on $LCL_HOSTA)\n";
sleep($SLEEPTIME);

print "\n";


# dump fshost logs and check that repserver set-context passed in principal

print "$myExec: Searching FX log for set-context principal (on $LCL_HOSTA)\n";

if (&TraceCreateHostAuth($LCL_HOSTA, *princ)) {
    print "$myExec: Found set-context principal ($princ)\n";
} else {
    print "$myExec: Unable to dump FX log (on $LCL_HOSTA), ";
    print "or did not find set-context principal in log\n";
    exit(1);
}

print "\n";


# need to somehow generate a CM-->FX set-context RPC that will create a host

print "\n$myExec: Test to generate CM set-context RPC not implemented\n";

print "\n";
exit(0);


# ----------------------------------------------------------------------------


# TraceCreateHostAuth([IN] host, [OUT] *princ)
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceCreateHostAuth {
    local($host, *princ) = @_;
    local($goterror);

    $goterror = $found = 0;

    open(INPIPE, "$RSH $host $DFSTRACE dump -set fshost |") || ($goterror = 1);

    if (!($goterror)) {
	while (<INPIPE>) {
	    if (/FSHS.*CreateHost.*Authenticating/) {
		($princ) = /as self to\s*(\S*):/;
		$found = 1;
		last;
	    }
	}
	close(INPIPE);
    }

    $goterror = ($goterror || !($found));

    !($goterror);  # return result
}
