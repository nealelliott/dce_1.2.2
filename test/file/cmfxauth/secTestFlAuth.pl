#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Flserver-authentication Test
#
#   This test verifies that the CM client makes authenticated flserver RPCs.
#
#   Local-identity: root
#   DCE-identity  : cell_admin
# ----------------------------------------------------------------------------

# main()
$| = 1;

print "\n\t\tServer RPC Authentication Test - Flserver\n\n";

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($ENV{DFSXSECTEST_SRCDIR}) {
    unshift(@INC, "$ENV{DFSXSECTEST_SRCDIR}");
} else {
    unshift(@INC, ".");
}

require("secconfig.pl");
require("secTestInclude.pl");


# set and clear the CM trace log

print "$myExec: $DFSTRACE setset -set cm -active\n";

if (system("$DFSTRACE setset -set cm -active")) {
    print "$myExec: Unable to manipulate CM trace logs\n";
    exit(1);
}

print "$myExec: $DFSTRACE clear\n";

if (system("$DFSTRACE clear")) {
    print "$myExec: Unable to manipulate CM trace logs\n";
    exit(1);
}

print "\n";


# issue checkf and read file in order to generate CM-->flserver RPC

print "$myExec: $CM checkf\n";

if (system("$CM checkf")) {
    print "$myExec: $CM checkf failed\n";
    exit(1);
}

print "$myExec: $CAT $LCL_ROFILE > /dev/null\n";

if (system("$CAT $LCL_ROFILE > /dev/null")) {
    print "$myExec: Unable to read file $LCL_ROFILE\n";
    exit(1);
}

print "\n";


# dump CM trace logs and check that flserver RPC was authenticated
# WARNING: heuristic; can be fooled; need better tracing

print "$myExec: Searching CM log for authenticated VL_GetEntryByName ";
print "to any flserver\n";

if (!(open(INPIPE, "$DFSTRACE dump -set cm |"))) {
    print "$myExec: Unable to dump CM log\n";
    exit(1);
}

$found = $phase = 0;

while (<INPIPE>) {
    if ($phase == 0) {
	# getting auth flserver binding should appear first...
	if (/cm_ConnByHost.*service.*$FL_ID.*authn.*authz/) {
	    ($authn, $authz) = /authn\s*(\d*),.*authz\s*(\d*)/;

	    if ($authz == $AUTHZ_SVC{"dce"}) {
		$phase = 1;
	    }
	}
    } else {
	# ...followed by indication of a VL_GetEntryByName RPC
	if (/CM\s*end\s*VL_GetVolByName/) {
	    $found = 1;
	    last;
	}
    }
}
close(INPIPE);

if ($found) {
    print "$myExec: Found authn RPC to flserver ($authn, $authz)\n";
} else {
    print "$myExec: Did not find authenticated flserver RPC in log\n";
    exit(1);
}

print "\n";
exit(0);
