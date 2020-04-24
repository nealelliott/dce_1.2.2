# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Initialization routines
#
#   This file contains environment initialization routines; all tests in
#   the suite utilize a common configuration.

# ----------------------------------------------------------------------------


# UserAbort() - user abort of test
#
sub UserAbort {
    print "\n+ WARNING - received interrupt signal; calling bailout.\n";

    &Bailout();
}


# InitializeEnv() - initialize test environment
#
sub InitializeEnv {

# Initialize some global variables (based on mhsconfig.pl)

    $rpcA = "$dirA/self"; $rpcA =~ s/\s//g;
    $rpcB = "$dirB/self"; $rpcB =~ s/\s//g;

    $ftName   = "$ftName.$$"; $ftName =~ s/\s//g;
    $ftNameRO = "$ftName.readonly";

    $mtPathRW = "$mtPath.RW"; $mtPathRW =~ s/\s//g;
    $mtPathRO = "$mtPath.RO"; $mtPathRO =~ s/\s//g;

    $rwFile = "$mtPathRW/cmMHSTestfile.txt";
    $roFile = "$mtPathRO/cmMHSTestfile.txt";

    $rankA1 = 1000;
    $rankB1 = 2000;
    $rankA2 = 3000;

    $bailoutAHostFixFLDB = 0;
    $bailoutAHostFixCDS  = 0;
    $bailoutAHostRmFt    = 0;
    $bailoutAHostRmRep   = 0;
    $bailoutAHostUpIf    = 0;
    $bailoutAHostRmRoute = 0;

    $bailoutBHostFixFLDB = 0;
    $bailoutBHostRmRep   = 0;
    $bailoutBHostUpIf    = 0;
    $bailoutBHostRmRoute = 0;

    $bailoutRmMountRW    = 0;
    $bailoutRmMountRO    = 0;

# Setup to catch SIGINT and call UserAbort()
    $SIG{'INT'} = 'UserAbort';

# Obtain and verify environment information

    print "\n+ Looking-up client-host IP addresses.\n";
    &GetMyAddrs(*ClientHostAddr) || &Bailout();
    $ipClient = $ClientHostAddr[0];

    print "\n+ Performing server-host domain-name to IP-address mapping.\n";
    if (!(&HostToIP($dnA1, *ipA1) && &HostToIP($dnA2, *ipA2) &&
	  &HostToIP($dnB1, *ipB1) && &HostToIP($dnB2, *ipB2))) {
	&Bailout();
    }

    print "\n+ Performing gateway domain-name to IP-address mapping.\n";
    if (!(&HostToIP($gwA1, *gwipA1) && &HostToIP($gwA2, *gwipA2) &&
	  &HostToIP($gwB1, *gwipB1) && &HostToIP($gwB2, *gwipB2))) {
	&Bailout();
    }

    print "\n+ Reading server-host binding information from CDS.\n";
    &GetHostInfoCDS($rpcA, *AHostIfId, *AHostBindCDS) || &Bailout();
    &GetHostInfoCDS($rpcB, *BHostIfId, *BHostBindCDS) || &Bailout();

    print "\n+ Reading server-host address information from FLDB.\n";
    &GetHostInfoFLDB($dirA, *AHostAddrFLDB) || &Bailout();
    &GetHostInfoFLDB($dirB, *BHostAddrFLDB) || &Bailout();

    print "\n+ Reading server-host aggregate information.\n";
    &GetHostAggrLFS($dirA, *aggrA) || &Bailout();
    &GetHostAggrLFS($dirB, *aggrB) || &Bailout();

    print "\n+ Reading flserver list from DFS junction in CDS.\n";
    &GetDFSInfoCDS($dfsNS, *flHostsCDS) || &Bailout();

    print "\n+ Verifying that all info meets test config requirements.\n";
    &VerifyHostInfo() || &Bailout();

# Reorder specified connection information if required

    print "\n+ Reordering server-host connection information if required.\n";
    &OrderHostInfo();

# Create filesets/files used by all tests

    umask(0);

    print "\n+ Creating filesets required for tests\n";
    &CreateFileset($ftName, $dirA, $aggrA) || &Bailout();
    $bailoutAHostRmFt = 1;

    &MountFileset($mtPathRW, $ftName, 1) || &Bailout();
    $bailoutRmMountRW = 1;

    &ChmodFileset(0777, $mtPathRW) || &Bailout();

    &SetRepFileset($ftName) || &Bailout();

    &AddSiteFileset($ftName, $dirA, $aggrA) || &Bailout();
    $bailoutAHostRmRep = 1;

    &AddSiteFileset($ftName, $dirB, $aggrB) || &Bailout();
    $bailoutBHostRmRep = 1;

    &MountFileset($mtPathRO, $ftNameRO, 0) || &Bailout();
    $bailoutRmMountRO = 1;

    print "\n+ Sleeping 30 seconds to ensure sites are created\n";
    sleep(30);

    print "\n+ Creating files required for tests\n";

    &CreateFile($rwFile) || &Bailout();

    &UpdateFileset($ftName) || &Bailout();

    print "\n+ Sleeping 30 seconds to ensure replicas are updated\n";
    sleep(30);

# Configure environment as required for tests

    print "\n+ Setting initial server-host client-route configurations.\n";

    # config B1 up via B2 THEN set B's client-route to B1 (now know both up)
    &ConfigIfUp($dnB2, $ifB1) || &Bailout();
    &AddRoute($dnB2, $ipClient, $gwipB1) || &Bailout();
    $bailoutBHostRmRoute = 1;

    # config A1 up via A2 THEN set A's client-route to A1 (now know both up)
    &ConfigIfUp($dnA2, $ifA1) || &Bailout();
    &AddRoute($dnA2, $ipClient, $gwipA1) || &Bailout();
    $bailoutAHostRmRoute = 1;

    print "\n+ Adjusting server-host binding information in CDS.\n";
    #  note: ipA2 guaranteed to be in AHostBindCDS after reordering conn info
    @bindA2 = grep(/$ipA2/, @AHostBindCDS);
    @bindA1 = @bindA2;
    foreach (@bindA1) {s/$ipA2/$ipA1/;}

    &RmAllHostBindCDS($rpcA, $AHostIfId) || &Bailout();
    $bailoutAHostFixCDS = 1;

    foreach $binding (@bindA2) {
	&AddHostBindCDS($rpcA, $AHostIfId, $binding) || &Bailout();
    }

    foreach $binding (@bindA1) {
	&AddHostBindCDS($rpcA, $AHostIfId, $binding) || &Bailout();
    }

    print "\n+ Adjusting server-host address information in FLDB.\n";
    for ($i = 0; $i <= $#AHostAddrFLDB; $i++) {
	if ($AHostAddrFLDB[$i] !~ /$ipA1|$ipA2/) {
	    &RmHostAddrFLDB($AHostAddrFLDB[$i]) || &Bailout();
	    $bailoutAHostFixFLDB = 1;
	}
    }

    if (grep(/$ipA1/, @AHostAddrFLDB) == 0) {
	&AddHostAddrFLDB($ipA2, $ipA1) || &Bailout();
	$bailoutAHostFixFLDB = 1;
    } elsif (grep(/$ipA2/, @AHostAddrFLDB) == 0) {
	&AddHostAddrFLDB($ipA1, $ipA2) || &Bailout();
	$bailoutAHostFixFLDB = 1;
    }

    for ($i = 0; $i <= $#BHostAddrFLDB; $i++) {
	if ($BHostAddrFLDB[$i] !~ /$ipB1|$ipB2/) {
	    &RmHostAddrFLDB($BHostAddrFLDB[$i]) || &Bailout();
	    $bailoutBHostFixFLDB = 1;
	}
    }

    if (grep(/$ipB1/, @BHostAddrFLDB) > 0 &&
	grep(/$ipB2/, @BHostAddrFLDB) > 0) {
	&RmHostAddrFLDB($ipB2) || &Bailout();
	$bailoutBHostFixFLDB = 1;
    } elsif (grep(/$ipB2/, @BHostAddrFLDB) > 0) {
	&AddHostAddrFLDB($ipB2, $ipB1) || &Bailout();
	&RmHostAddrFLDB($ipB2) || &Bailout();
	$bailoutBHostFixFLDB = 1;
    }

    print "\n+ Setting cache-manager address-preference ranks.\n";
    &SetAddrPrefRank($ipA1, $rankA1) || &Bailout();
    &SetAddrPrefRank($ipB1, $rankB1) || &Bailout();
    &SetAddrPrefRank($ipA2, $rankA2) || &Bailout();
    &CheckFilesets() || &Bailout();
}



# RestoreEnv() - restore environment to original state on normal exit
#
sub RestoreEnv {

    print "\n+ Restoring server-host client-route configurations.\n";
    &DelRoute($dnB2, $ipClient, $gwipB1);
    &DelRoute($dnA2, $ipClient, $gwipA1);

    print "\n+ Restoring server-host binding information in CDS.\n";

    &RmAllHostBindCDS($rpcA, $AHostIfId);

    foreach $binding (@AHostBindCDS) {
	&AddHostBindCDS($rpcA, $AHostIfId, $binding);
    }

    print "\n+ Restoring server-host address information in FLDB.\n";

    if (grep(/$ipA1/, @AHostAddrFLDB) > 0) {
	$ipExtant = $ipA1;
	&RmHostAddrFLDB($ipA2);
    } else {
	$ipExtant = $ipA2;
	&RmHostAddrFLDB($ipA1);
    }

    for ($i = 0; $i <= $#AHostAddrFLDB; $i++) {
	if ($AHostAddrFLDB[$i] !~ /$ipExtant/) {
	    &AddHostAddrFLDB($ipExtant, $AHostAddrFLDB[$i]);
	}
    }

    if (grep(/$ipB1/, @BHostAddrFLDB) > 0) {
	$ipExtant = $ipB1;
    } else {
	&AddHostAddrFLDB($ipB1, $ipB2);
	&RmHostAddrFLDB($ipB1);
	$ipExtant = $ipB2;
    }

    for ($i = 0; $i <= $#BHostAddrFLDB; $i++) {
	if ($BHostAddrFLDB[$i] !~ /$ipExtant/) {
	    &AddHostAddrFLDB($ipExtant, $BHostAddrFLDB[$i]);
	}
    }

    print "\n+ Cache-manager address-preference ranks remain altered.\n";

    print "\n+ Removing filesets and files created for tests\n";
    &UmountFileset($mtPathRO);
    &RmSiteFileset($ftName, $dirB, $aggrB);
    &RmSiteFileset($ftName, $dirA, $aggrA);
    &UmountFileset($mtPathRW);
    &DeleteFileset($ftName, $dirA, $aggrA);
}



# VerifyHostInfo() - verify that all specified/derived host information
#     meets test configuration requirements.
#
#     Returns: 1 - successful; 0 - failed
#
sub VerifyHostInfo {
    local($goterror, @iparray, $flprinc);

    $goterror = 0;

    # check IP mappings

    @iparray = ($ipA1, $ipA2, $ipB1, $ipB2);

    if (grep(/$ipA1/, @iparray) != 1 || grep(/$ipA2/, @iparray) != 1 ||
	grep(/$ipB1/, @iparray) != 1 || grep(/$ipB2/, @iparray) != 1) {
	print "Specified server-host domain names did not all map to ";
	print "unique addresses\n";
	$goterror = 1;

    } else {
	# check CDS server entries

	# expect to find ipA1 or ipA2 in rpcA, but not both.  having both in
	# rpcA implies that neither ipA1 nor ipA2 is safe to configure down.
	# having neither in rpcA implies that can not safely replace rpcA with
	# just ipA1 and ipA2, as is required for tests.
	#
	if (!defined(@AHostBindCDS)) {
	    print "The CDS entry $rpcA contains no bindings\n";
	    $goterror = 1;

	} elsif (grep(/$ipA1/, @AHostBindCDS) > 0 &&
		 grep(/$ipA2/, @AHostBindCDS) > 0) {
	    print "Did not expect to find both $ipA1 ($dnA1) and ";
	    print "$ipA2 ($dnA2) in $rpcA\n";
	    $goterror = 1;

	} elsif (grep(/$ipA1/, @AHostBindCDS) == 0 &&
		 grep(/$ipA2/, @AHostBindCDS) == 0) {
	    print "Expected to find either $ipA1 ($dnA1) or ";
	    print "$ipA2 ($dnA2) in $rpcA\n";
	    $goterror = 1;
	}

	if (grep(/$ipB1/, @AHostBindCDS) > 0 ||
	    grep(/$ipB2/, @AHostBindCDS) > 0) {
	    print "Did not expect to find $ipB1 ($dnB1) or ";
	    print "$ipB2 ($dnB2) in $rpcA\n";
	    $goterror = 1;
	}

	# do not expect to find both ipB1 and ipB2 in rpcB (one/none is OK).
	# having both in rpcB implies that neither ipB1 nor ipB2 is safe to
	# configure down.

	if (!defined(@BHostBindCDS)) {
	    print "The CDS entry $rpcB contains no bindings\n";
	    $goterror = 1;

	} elsif (grep(/$ipB1/, @BHostBindCDS) > 0 &&
		 grep(/$ipB2/, @BHostBindCDS) > 0) {
	    print "Did not expect to find both $ipB1 ($dnB1) and ";
	    print "$ipB2 ($dnB2) in $rpcB\n";
	    $goterror = 1;
	}

	if (grep(/$ipA1/, @BHostBindCDS) > 0 ||
	    grep(/$ipA2/, @BHostBindCDS) > 0) {
	    print "Did not expect to find $ipA1 ($dnA1) or ";
	    print "$ipA2 ($dnA2) in $rpcB\n";
	    $goterror = 1;
	}


	if (!defined(@flHostsCDS) || $#flHostsCDS != 0) {
	    print "The DFS junction $dfsNS ";
	    print "must contain exactly one flserver host\n";
	    $goterror = 1;

	} else {
	    ($flprinc) = ($rpcA =~ /\S+\/(\S+\/\S+\/self)/);

	    if (grep(/$flprinc/, @flHostsCDS) != 1) {
		print "Expected to find $flprinc in DFS junction $dfsNS\n";
		$goterror = 1;
	    }
	}

	# check FLDB server entries

	if (grep(/$ipA1/, @AHostAddrFLDB) == 0 &&
	    grep(/$ipA2/, @AHostAddrFLDB) == 0) {
	    print "Expected to find $ipA1 ($dnA1) or $ipA2 ($dnA2) ";
	    print "in FLDB for server $dirA\n";
	    $goterror = 1;
	}

	if (grep(/$ipB1/, @AHostAddrFLDB) > 0 ||
	    grep(/$ipB2/, @AHostAddrFLDB) > 0) {
	    print "Did not expect to find $ipB1 ($dnB1) or $ipB2 ($dnB2) ";
	    print "in FLDB for server $dirA\n";
	    $goterror = 1;
	}

	if (grep(/$ipB1/, @BHostAddrFLDB) == 0 &&
	    grep(/$ipB2/, @BHostAddrFLDB) == 0) {
	    print "Expected to find $ipB1 ($dnB1) or $ipB2 ($dnB2) ";
	    print "in FLDB for server $dirB\n";
	    $goterror = 1;
	}

	if (grep(/$ipA1/, @BHostAddrFLDB) > 0 ||
	    grep(/$ipA2/, @BHostAddrFLDB) > 0) {
	    print "Did not expect to find $ipA1 ($dnA1) or $ipA2 ($dnA2) ";
	    print "in FLDB for server $dirB\n";
	    $goterror = 1;
	}
    }

    # check that test is not being executed on server-host A or B.
    # this is a heuristic, and is not guaranteed to be correct

    if (!($goterror)) {
	if (grep(/$ipA1/, @ClientHostAddr) != 0 ||
	    grep(/$ipA2/, @ClientHostAddr) != 0 ||
	    grep(/$ipB1/, @ClientHostAddr) != 0 ||
	    grep(/$ipB2/, @ClientHostAddr) != 0) {
	    print "Test can not be executed on a designated server-host\n";
	    $goterror = 1;
	}
    }

    !($goterror); # return result
}



# OrderHostInfo() - order host connection information so that tests will
#     not configure down a network connection that is being used by DCE.
#
sub OrderHostInfo {
    local($temp);

    # Ensure that A2 is connection used by DCE RPC for host A
    if (grep(/$ipA1/, @AHostBindCDS) > 0) {
	print "Swapping server-host network connection info for A1 and A2\n";
	$temp = $dnA1;
	$dnA1 = $dnA2;
	$dnA2 = $temp;

	$temp = $ifA1;
	$ifA1 = $ifA2;
	$ifA2 = $temp;

	$temp = $gwA1;
	$gwA1 = $gwA2;
	$gwA2 = $temp;

	$temp = $ipA1;
	$ipA1 = $ipA2;
	$ipA2 = $temp;

	$temp   = $gwipA1;
	$gwipA1 = $gwipA2;
	$gwipA2 = $temp;
    }

    # Ensure that B2 is connection used by DCE RPC for host B
    if (grep(/$ipB1/, @BHostBindCDS) > 0) {
	print "Swapping server-host network connection info for B1 and B2\n";
	$temp = $dnB1;
	$dnB1 = $dnB2;
	$dnB2 = $temp;

	$temp = $ifB1;
	$ifB1 = $ifB2;
	$ifB2 = $temp;

	$temp = $gwB1;
	$gwB1 = $gwB2;
	$gwB2 = $temp;

	$temp = $ipB1;
	$ipB1 = $ipB2;
	$ipB2 = $temp;

	$temp   = $gwipB1;
	$gwipB1 = $gwipB2;
	$gwipB2 = $temp;
    }
}



# Bailout() - exit with error status; restore environment as needed.
#
sub Bailout{
    local($i);

    print "\n+ Bailing due to error...will attempt to restore environment, ";
    print "as needed\n";

    if (&TraceBogusFail()) {
	print "\n\n+ WARNING - trace log indicates bogus failure - ";
	print "IGNORE results and rerun test.\n\n";
    }

    if ($bailoutAHostUpIf) {
	&ConfigIfUp($dnA2, $ifA1); # config A1 up via A2
    }

    if ($bailoutBHostUpIf) {
	&ConfigIfUp($dnB2, $ifB1); # config B1 up via B2
    }

    if ($bailoutAHostRmRoute) {
	# don't know which route in table; attempt to remove both
	&DelRoute($dnA2, $ipClient, $gwipA1);
	&DelRoute($dnA2, $ipClient, $gwipA2);
    }

    if ($bailoutBHostRmRoute) {
	# don't know which route in table; attempt to remove both
	&DelRoute($dnB2, $ipClient, $gwipB1);
	&DelRoute($dnB2, $ipClient, $gwipB2);
    }

    if ($bailoutAHostFixCDS) {
	&RmAllHostBindCDS($rpcA, $AHostIfId);

	foreach $binding (@AHostBindCDS) {
	    &AddHostBindCDS($rpcA, $AHostIfId, $binding);
	}
    }

    if ($bailoutAHostFixFLDB) {
	# don't know state; very brute force approach
	&AddHostAddrFLDB($ipA1, $ipA2);
	&AddHostAddrFLDB($ipA2, $ipA1);

	if (grep(/$ipA1/, @AHostAddrFLDB) > 0) {
	    $ipExtant = $ipA1;
	    &RmHostAddrFLDB($ipA2);
	} else {
	    $ipExtant = $ipA2;
	    &RmHostAddrFLDB($ipA1);
	}

	for ($i = 0; $i <= $#AHostAddrFLDB; $i++) {
	    if ($AHostAddrFLDB[$i] !~ /$ipExtant/) {
		&AddHostAddrFLDB($ipExtant, $AHostAddrFLDB[$i]);
	    }
	}
    }

    if ($bailoutBHostFixFLDB) {
	# don't know state; very brute force approach
	&AddHostAddrFLDB($ipB1, $ipB2);
	&AddHostAddrFLDB($ipB2, $ipB1);

	if (grep(/$ipB1/, @BHostAddrFLDB) > 0) {
	    $ipExtant = $ipB1;
	    &RmHostAddrFLDB($ipB2);
	} else {
	    $ipExtant = $ipB2;
	    &RmHostAddrFLDB($ipB1);
	}

	for ($i = 0; $i <= $#BHostAddrFLDB; $i++) {
	    if ($BHostAddrFLDB[$i] !~ /$ipExtant/) {
		&AddHostAddrFLDB($ipExtant, $BHostAddrFLDB[$i]);
	    }
	}
    }

    if ($bailoutRmMountRO) {
	&UmountFileset($mtPathRO);
    }

    if ($bailoutBHostRmRep) {
	&RmSiteFileset($ftName, $dirB, $aggrB);
    }

    if ($bailoutAHostRmRep) {
	&RmSiteFileset($ftName, $dirA, $aggrA);
    }

    if ($bailoutRmMountRW) {
	&UmountFileset($mtPathRW);
    }

    if ($bailoutAHostRmFt) {
	&DeleteFileset($ftName, $dirA, $aggrA);
    }

    exit(1);
}


1; # need for perl require()
