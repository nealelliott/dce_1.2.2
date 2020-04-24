# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Multi-server Binding Tests
#
#   This file implements the multi-server binding tests:
#     - replicated (RO) fileset access, and
#     - read-write fileset access.

# ----------------------------------------------------------------------------


# ROFilesetAccess_Test() - two server test of cm_ConnByMHosts() (cm_Conn())
#     that exercises addresses acquisition, address fail-over, and address
#     revival
#
sub ROFilesetAccess_Test {
    local($phase);

    print "\n\t\tMulti-server binding test - Replicated Fileset Access\n";

    print "\n+ Performing test initialization\n";
    &TraceOn() || &Bailout();
    &FlushFile($roFile) || &Bailout();

    for ($phase = 0; $phase < 3; $phase++) {
	print "\n+ Beginning test phase $phase\n";
	&CheckFilesets() || &Bailout(); # force addr fetch
	&TraceClear() || &Bailout();

	if ($phase == 1) {
	    print "\n+ Reading $roFile - should succeed via $dnB1 ($ipB1)\n";
	    &CatFile($roFile) || &Bailout();
	    &TraceMSBindAddr($ipB1, $fxType) || &Bailout();

	    &FlushFile($roFile) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Reading $roFile - should succeed via $dnA1 ($ipA1)\n";
	    &CatFile($roFile) || &Bailout();
	    &TraceMSBindAddr($ipA1, $fxType) || &Bailout();

	    &FlushFile($roFile) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	print "\n+ Configuring $dnA1 ($ipA1,$ifA1) DOWN\n";
#       config A1 down via A2 AFTER resetting A's client-route to A2
	&DelRoute($dnA2, $ipClient, $gwipA1) || &Bailout();
	$bailoutAHostRmRoute = 0;
	&AddRoute($dnA2, $ipClient, $gwipA2) || &Bailout();
	$bailoutAHostRmRoute = 1;

	&ConfigIfDown($dnA2, $ifA1) || &Bailout();
	$bailoutAHostUpIf = 1;

	if ($phase == 1) {
	    print "\n+ Reading $roFile - should succeed via $dnB1 ($ipB1)\n";
	    &CatFile($roFile) || &Bailout();
	    &TraceMSBindAddr($ipB1, $fxType) || &Bailout();

	    &FlushFile($roFile) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Reading $roFile - should succeed via $dnB1 ($ipB1) ";
	    print "AFTER fail-over from $dnA1 ($ipA1)\n";
	    &CatFile($roFile) || &Bailout();
	    &TraceFailedAddr($ipA1) || &Bailout();
	    &TraceMSBindAddr($ipB1, $fxType) || &Bailout();

	    &FlushFile($roFile) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	print "\n+ Configuring $dnB1 ($ipB1,$ifB1) DOWN\n";
#       config B1 down via B2 AFTER resetting B's client-route to B2
	&DelRoute($dnB2, $ipClient, $gwipB1) || &Bailout();
	$bailoutBHostRmRoute = 0;
	&AddRoute($dnB2, $ipClient, $gwipB2) || &Bailout();
	$bailoutBHostRmRoute = 1;

	&ConfigIfDown($dnB2, $ifB1) || &Bailout();
	$bailoutBHostUpIf = 1;

	print "\n+ Reading $roFile - should succeed via $dnA2 ($ipA2) ";
	print "AFTER file-server at $dnB1 is declared DOWN\n";
	&CatFile($roFile) || &Bailout();
	&TraceDownServer($ipB1) || &Bailout();
	&TraceMSBindAddr($ipA2, $fxType) || &Bailout();

	&FlushFile($roFile) || &Bailout();
	&TraceClear() || &Bailout();

	print "\n+ Configuring $dnB1 ($ipB1,$ifB1) UP\n";
#       config B1 up via B2 THEN reset B's client-route to B1
	&ConfigIfUp($dnB2, $ifB1) || &Bailout();
	$bailoutBHostUpIf = 0;

	&DelRoute($dnB2, $ipClient, $gwipB2) || &Bailout();
	$bailoutBHostRmRoute = 0;
	&AddRoute($dnB2, $ipClient, $gwipB1) || &Bailout();
	$bailoutBHostRmRoute = 1;

	print "\n+ Sleeping dead-server poll time ($deadPollTime seconds)\n";
	sleep($deadPollTime);
	print "\n+ File-server at $dnB1 should now be UP\n";
	&TraceUpServer($ipB1) || &Bailout();
	&TraceClear() || &Bailout();

	print "\n+ Reading $roFile - should succeed via $dnB1 ($ipB1)\n";
	&CatFile($roFile) || &Bailout();
	&TraceMSBindAddr($ipB1, $fxType) || &Bailout();

	&FlushFile($roFile) || &Bailout();
	&TraceClear() || &Bailout();

	print "\n+ Configuring $dnA1 ($ipA1,$ifA1) UP\n";
#       config A1 up via A2 THEN reset A's client-route to A1
	&ConfigIfUp($dnA2, $ifA1) || &Bailout();
	$bailoutAHostUpIf = 0;

	&DelRoute($dnA2, $ipClient, $gwipA2) || &Bailout();
	$bailoutAHostRmRoute = 0;
	&AddRoute($dnA2, $ipClient, $gwipA1) || &Bailout();
	$bailoutAHostRmRoute = 1;

	if ($phase == 1) {
	    print "\n+ Reading $roFile - should succeed via $dnB1 ($ipB1)\n";
	    &CatFile($roFile) || &Bailout();
	    &TraceMSBindAddr($ipB1, $fxType) || &Bailout();

	    &FlushFile($roFile) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Sleeping address-revival poll time ";
	    print "($livePollTime seconds)\n";
	    sleep($livePollTime);
	    print "\n+ File-server connection $dnA1 ($ipA1) ";
	    print "should now be UP\n";
	    &TraceRevivedAddr($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();

	    print "\n+ Reading $roFile - should succeed via $dnA1 ($ipA1)\n";
	    &CatFile($roFile) || &Bailout();
	    &TraceMSBindAddr($ipA1, $fxType) || &Bailout();

	    &FlushFile($roFile) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	if ($phase == 0) {
	    print "\n+ Removing file-server address $dnA1 ($ipA1) from FLDB\n";
	    &RmHostAddrFLDB($ipA1) || &Bailout();
	} elsif ($phase == 1) {
	    print "\n+ Adding file-server address $dnA1 ($ipA1) to FLDB\n";
	    &AddHostAddrFLDB($ipA2, $ipA1) || &Bailout();
	    &SetAddrPrefRank($ipA1, $rankA1) || &Bailout();
	}
    }
}



# RWFilesetAccess_Test() - single server test of cm_ConnByMHosts() (cm_Conn())
#     that exercises addresses acquisition, address fail-over, and address
#     revival
#
sub RWFilesetAccess_Test {
    local($phase);

    print "\n\t\tMulti-server binding test - Read-write Fileset Access\n";

    print "\n+ Performing test initialization\n";
    &TraceOn() || &Bailout();
    &FlushFile($rwFile) || &Bailout();

    for ($phase = 0; $phase < 3; $phase++) {
	print "\n+ Beginning test phase $phase\n";
	&CheckFilesets() || &Bailout();  # force addr fetch
	&TraceClear() || &Bailout();

	print "\n+ Reading $rwFile - should succeed via $dnA1 ($ipA1)\n";
	&CatFile($rwFile) || &Bailout();
	&TraceMSBindAddr($ipA1, $fxType) || &Bailout();

	&FlushFile($rwFile) || &Bailout();
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
	    print "\n+ Reading $rwFile - should fail with ";
	    print "file-server at $dnA1 declared DOWN\n";
	    !(&CatFile($rwFile)) || &Bailout();
	    &TraceDownServer($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	} else {
	    print "\n+ Reading $rwFile - should succeed via $dnA2 ($ipA2) ";
	    print "AFTER fail-over from $dnA1 ($ipA1)\n";
	    &CatFile($rwFile) || &Bailout();
	    &TraceFailedAddr($ipA1) || &Bailout();
	    &TraceMSBindAddr($ipA2, $fxType) || &Bailout();

	    &FlushFile($rwFile) || &Bailout();
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
	} else {
	    print "\n+ Sleeping address-revival poll time ";
	    print "($livePollTime seconds)\n";
	    sleep($livePollTime);
	    print "\n+ File-server connection $dnA1 ($ipA1) ";
	    print "should now be UP\n";
	    &TraceRevivedAddr($ipA1) || &Bailout();
	    &TraceClear() || &Bailout();
	}

	print "\n+ Reading $rwFile - should succeed via $dnA1 ($ipA1)\n";
	&CatFile($rwFile) || &Bailout();
	&TraceMSBindAddr($ipA1, $fxType) || &Bailout();

	&FlushFile($rwFile) || &Bailout();
	&TraceClear() || &Bailout();

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
