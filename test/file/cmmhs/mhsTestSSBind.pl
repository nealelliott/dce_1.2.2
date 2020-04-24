# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Single-server Binding Tests
#
#   This file implements the single-server binding tests:
#     - server status check.

# ----------------------------------------------------------------------------


# SStatusCheck_Test() - test of cm_ConnByHost() that exercises address
#     acquisition, address fail-over, and address revival
#
sub SStatusCheck_Test {
    local($phase);

    print "\n\t\tSingle-server binding test - Server Status Check\n";

    print "\n+ Performing test initialization\n";
    &TraceOn() || &Bailout();

    for ($phase = 0; $phase < 3; $phase++) {
	print "\n+ Beginning test phase $phase\n";
	&CheckFilesets() || &Bailout();  # force addr fetch
	&TraceClear() || &Bailout();

	print "\n+ Opening $rwFile\n";
	open(READFD, "$rwFile") || &Bailout();
	&TraceClear() || &Bailout();

	print "\n+ Checking file-server at $dnA1 - should succeed ";
	print "via $dnA1 ($ipA1)\n";
	&StatServers() || &Bailout();
	&TraceSSBindAddr($ipA1, $fxType) || &Bailout();
	&TraceClear() || &Bailout();

	print "\n+ Configuring $dnA1 ($ipA1,$ifA1) DOWN\n";
#       config A1 down via A2 AFTER resetting A's client-route to A2
	&DelRoute($dnA2, $ipClient, $gwipA1) || &Bailout();
	$bailoutAHostRmRoute = 0;
	&AddRoute($dnA2, $ipClient, $gwipA2) || &Bailout();
	$bailoutAHostRmRoute = 1;

	&ConfigIfDown($dnA2, $ifA1) || &Bailout();
	$bailoutAHostUpIf = 1;

	if ($phase == 1) {
	    print "\n+ Checking file-server at $dnA1 - should indicate that ";
	    print "file-server declared DOWN\n";
	    &StatServers() || &Bailout();
	    &TraceDownServer($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Checking file-server at $dnA1 - should succeed ";
	    print "via $dnA2 ($ipA2) AFTER fail-over from $dnA1 ($ipA1)\n";
	    &StatServers() || &Bailout();
	    &TraceFailedAddr($ipA1) || &Bailout();
	    &TraceSSBindAddr($ipA2, $fxType) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	print "\n+ Configuring $dnA1 ($ipA1,$ifA1) UP\n";
#       config A1 up via A2 THEN reset A's client-route to A1
	&ConfigIfUp($dnA2, $ifA1) || &Bailout();
	$bailoutAHostUpIf = 0;

	&DelRoute($dnA2, $ipClient, $gwipA2) || &Bailout();
	$bailoutAHostRmRoute = 0;
	&AddRoute($dnA2, $ipClient, $gwipA1) || &Bailout();
	$bailoutAHostRmRoute = 1;

	if ($phase == 1) {
	    print "\n+ Sleeping dead-server poll time ";
	    print "($deadPollTime seconds)\n";
	    sleep($deadPollTime);
	    print "\n+ File-server at $dnA1 should now be UP\n";
	    &TraceUpServer($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();

	    print "\n+ Closing and re-opening $rwFile\n";
	    close(READFD);
	    open(READFD, "$rwFile") || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Sleeping address-revival poll time ";
	    print "($livePollTime seconds)\n";
	    sleep($livePollTime);
	    print "\n+ File-server connection $dnA1 ($ipA1) ";
	    print "should now be UP\n";
	    &TraceRevivedAddr($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	print "\n+ Checking file-server at $dnA1 - should succeed ";
	print "via $dnA1 ($ipA1)\n";
	&StatServers() || &Bailout();
	&TraceSSBindAddr($ipA1, $fxType) || &Bailout();
	&TraceClear() || &Bailout();

	print "\n+ Closing $rwFile\n";
	close(READFD);

	if ($phase == 0) {
	    print "\n+ Removing file-server address $dnA2 ($ipA2) from FLDB\n";
	    &RmHostAddrFLDB($ipA2) || &Bailout();
	} elsif ($phase == 1) {
	    print "\n+ Adding file-server address $dnA2 ($ipA2) to FLDB\n";
	    &AddHostAddrFLDB($ipA1, $ipA2) || &Bailout();
	    &SetAddrPrefRank($ipA2, $rankA2) || &Bailout();
	}
    }
}

1; # needed for perl require()
