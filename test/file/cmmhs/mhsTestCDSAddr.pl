# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - CDS Address Acquisition Tests
#
#   This file implements the CDS address acquisition tests:
#     - fileset location check.

# ----------------------------------------------------------------------------


# FilesetLocation_Test() - test of flserver address acquistion from CDS
#
sub FilesetLocation_Test {
    local($phase);

    print "\n\t\tCDS Address Acquisition Test - Fileset Location Check\n";

    print "\n+ Performing test initialization\n";
    &TraceOn() || &Bailout();

    for ($phase = 0; $phase < 3; $phase++) {
	print "\n+ Beginning test phase $phase\n";
	&TraceClear() || &Bailout();

	print "\n+ Fetching location of '/:/' from FLDB - should succeed ";
	print "via $dnA1 ($ipA1)\n";
	&CheckFilesets() || &Bailout();
	&WhereisFile("/:/") || &Bailout();
	&TraceMSBindAddr($ipA1, $flType) || &Bailout();
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
	    print "\n+ Fetching location of '/:/' from FLDB - should fail ";
	    print "with flserver at $dnA1 declared DOWN\n";
	    &CheckFilesets() || &Bailout();
	    !(&WhereisFile("/:/")) || &Bailout();
	    &TraceDownServer($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Fetching location of '/:/' from FLDB - should ";
	    print "succeed via $dnA2 ($ipA2) AFTER ";
	    print "fail-over from $dnA1 ($ipA1)\n";
	    &CheckFilesets() || &Bailout();
	    &WhereisFile("/:/") || &Bailout();
	    &TraceFailedAddr($ipA1) || &Bailout();
	    &TraceMSBindAddr($ipA2, $flType) || &Bailout();
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
	    print "\n+ Flserver at $dnA1 should now be UP\n";
	    &TraceUpServer($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Sleeping address-revival poll time ";
	    print "($livePollTime seconds)\n";
	    sleep($livePollTime);
	    print "\n+ Flserver connection $dnA1 ($ipA1) should now be UP\n";
	    &TraceRevivedAddr($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	print "\n+ Fetching location of '/:/' from FLDB - should succeed ";
	print "via $dnA1 ($ipA1)\n";
	&CheckFilesets() || &Bailout();
	&WhereisFile("/:/") || &Bailout();
	&TraceMSBindAddr($ipA1, $flType) || &Bailout();
	&TraceClear() || &Bailout();

	if ($phase == 0) {
	    print "\n+ Removing flserver host address $dnA2 ($ipA2) ";
	    print "from CDS\n";
	    &RmAllHostBindCDS($rpcA, $AHostIfId) || &Bailout();

	    foreach $binding (@bindA1) {
		&AddHostBindCDS($rpcA, $AHostIfId, $binding) || &Bailout();
	    }

	} elsif ($phase == 1) {
	    print "\n+ Adding flserver host address $dnA2 ($ipA2) to CDS\n";
	    foreach $binding (@bindA2) {
		&AddHostBindCDS($rpcA, $AHostIfId, $binding) || &Bailout();
	    }
	    &SetAddrPrefRank($ipA2, $rankA2) || &Bailout();
	}
    }
}

1; # needed for perl require()
