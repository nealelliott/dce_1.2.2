# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Generic utility routines
#
#   This file contains generic utilities for test implementation.

# ----------------------------------------------------------------------------


# FlushFile([IN] filename) - flush specified file
#
#     Returns: 1 - successful; 0 - failed
#
sub FlushFile {
    local($filename) = @_;
    local($goterror);

    print "$CM flush $filename\n";
    $goterror = system("$CM flush $filename");
    !($goterror); # return result
}


# CatFile([IN] filename) - cat specified file
#
#     Returns: 1 - successful; 0 - failed
#
sub CatFile {
    local($filename) = @_;
    local($goterror);

    print "$CAT $filename > /dev/null\n";
    $goterror = system("$CAT $filename > /dev/null");
    !($goterror); # return result
}


# CreateFile([IN] filename) - create and write specified file
#
#     Returns: 1 - successful; 0 - failed
#
sub CreateFile {
    local($filename) = @_;
    local($goterror);

    $goterror = 0;

    print "Creating file $filename\n";

    open(OUTFILE, "> $filename") || ($goterror = 1);

    if (!($goterror)) {
	print OUTFILE <<FINIS;
Be most excellent to each other.  Party on, dudes. [Bill and Ted]
FINIS
        close(OUTFILE);
    }

    !($goterror); # return result
}


# HostToIP([IN] hostname, [OUT] *ipaddr) - convert hostname to its
#     corresponding IP address and store (in a.b.c.d form) in ipaddr
#
#     Returns: 1 - successful; 0 - failed
#
sub HostToIP {
    local($hostname, *ipaddr) = @_;
    local($goterror);
    local(@octets);

    local($name, $aliases, $atype, $length, @addrs) =
	gethostbyname("$hostname");

    if (!defined(@addrs)) {
	$goterror = 1;
    } else {
	$goterror = 0;

	@octets = unpack('C4', $addrs[0]);
	$ipaddr = "$octets[0].$octets[1].$octets[2].$octets[3]"; # return addr
    }
    !($goterror); # return result
}


# CheckFilesets() - force update of fileset-related info from FLDB/CDS
#
#     Returns: 1 - successful; 0 - failed
#
sub CheckFilesets {
    local($goterror);

    print "$CM checkfilesets\n";
    $goterror = system("$CM checkfilesets");
    !($goterror); # return result
}


# StatServers() - ping servers from which CM holds tokens
#
#     Returns: 1 - successful; 0 - failed
#
sub StatServers {
    local($goterror);

    print "$CM statservers\n";
    $goterror = system("$CM statservers");
    !($goterror); # return result
}


# WhereisFile([IN] filename) - determine location of specified file
#
#     Returns: 1 - successful; 0 - failed
#
sub WhereisFile {
    local($goterror);
    local($filename) = $_[0];

    print "$CM whereis $filename\n";
    $goterror = system("$CM whereis $filename");
    !($goterror); # return result
}


# GetHostInfoFLDB([IN] host, [OUT] *ipaddrList) - read/parse FLDB info for
#     specified host, returning all addrs in FLDB
#
#     Returns: 1 - successful; 0 - failed
#
sub GetHostInfoFLDB {
    local($host, *ipaddrList) = @_;
    local($goterror);
    local($addridx);

    $goterror = $addridx = 0;

    print "$FTS lsserv -server $host\n";
    open(INPIPE, "$FTS lsserv -server $host |") || ($goterror = 1);

    if (!($goterror)) {
	while (<INPIPE>) {
	    if (/\d+\.\d+\.\d+\.\d+\s*\)/) {
		# get host address
		($ipaddrList[$addridx++]) = /(\d+\.\d+\.\d+\.\d+)\s*\)/;
	    }
	}
	close(INPIPE);

	if (!defined(@ipaddrList)) {
	    print "Unable to get information about host $host\n";
	    $goterror = 1;
	}
    }
    !($goterror); # return result
}


# RmHostAddrFLDB([IN] ipaddr) - remove the host address from the FLDB
#
#     Returns: 1 - successful; 0 - failed
#
sub RmHostAddrFLDB {
    local($goterror);
    local($ipaddr) = $_[0];

    print "$FTS edserv -server $ipaddr -rmaddr\n";
    $goterror = system("$FTS edserv -server $ipaddr -rmaddr");
    !($goterror); # return result
}


# AddHostAddrFLDB([IN] ipextant, [IN] ipnew) - add the host addr to the FLDB
#
#     Returns: 1 - successful; 0 - failed
#
sub AddHostAddrFLDB {
    local($goterror);
    local($ipextant) = $_[0];
    local($ipnew)    = $_[1];

    print "$FTS edserv -server $ipextant -addaddr $ipnew\n";
    $goterror = system("$FTS edserv -server $ipextant -addaddr $ipnew");
    !($goterror); # return result
}


# SetAddrPrefRank([IN] ipaddr, [IN] rank) - set address-preference rank to
#     specified value.
#
#     Returns: 1 - successful; 0 - failed
#
sub SetAddrPrefRank {
    local($ipaddr, $rank) = @_;
    local($goterror);

    print "$CM setpreferences -server $ipaddr $rank\n";
    if (!($goterror = system("$CM setpreferences -server $ipaddr $rank"))) {
	print "$CM setpreferences -server $ipaddr $rank -fldb\n";
	$goterror = system("$CM setpreferences -server $ipaddr $rank -fldb");
    }

    !($goterror); # return result
}


# GetHostInfoCDS([IN] hostRPC, [OUT] *hostIfId, [OUT] *hostBind) -
#     Read/parse CDS info for host, returning interface ID and all bindings
#
#     Returns: 1 - successful; 0 - failed
#
sub GetHostInfoCDS {
    local($hostRPC, *hostIfId, *hostBind) = @_;
    local($goterror);
    local($bindidx);
    local($lineidx);

    $goterror = $bindidx = $lineidx = 0;

    print "$DCECP -c rpcentry show $hostRPC\n";
    open(INPIPE, "$DCECP -c rpcentry show $hostRPC |") || ($goterror = 1);

    if (!($goterror)) {
	while (<INPIPE>) {
	    if (/\S+/) {
		if ($lineidx++ == 0) {
		    # get host interface id; enclose in brackets
		    ($hostIfId) = /\{\s*([a-f0-9-]+\s+\d+.\d+)/i;
		    $hostIfId = "{$hostIfId}";

		} elsif (/\{\s*\w+\s+[0-9.]+\s+\d+\s*\}/) {
		    # get host binding; enclose in brackets
		    ($hostBind[$bindidx]) = /\{\s*(\w+\s+[0-9.]+\s+\d+)/;
		    $hostBind[$bindidx] = "{$hostBind[$bindidx]}";
		    $bindidx++;
		}
	    }
	}
	close(INPIPE);

	if (!defined($hostIfId) || ($hostIfId =~ /\{\s*\}/)) {
	    print "Unable to get host information from $hostRPC\n";
	    $goterror = 1;
	}
    }
    !($goterror); # return result
}


# RmAllHostBindCDS([IN] hostRPC, [IN] hostIfId) - remove all bindings
#     for host from CDS
#
#     Returns: 1 - successful; 0 - failed
#
sub RmAllHostBindCDS {
    local($hostRPC, $hostIfId) = @_;
    local($goterror, $sysarg);

    print "$DCECP -c rpcentry unexport $hostRPC ";
    print "-interface $hostIfId\n";

    $sysarg  = "$DCECP -c rpcentry unexport $hostRPC ";
    $sysarg .= "-interface $hostIfId";

    $goterror = system("$sysarg");

    !($goterror); # return result
}


# AddHostBindCDS([IN] hostRPC, [IN] hostIfId, [IN] hostBind) -
#     Add specified binding for host to CDS
#
#     Returns: 1 - successful; 0 - failed
#
sub AddHostBindCDS {
    local($hostRPC, $hostIfId, $hostBind) = @_;
    local($goterror, $sysarg);

    print "$DCECP -c rpcentry export $hostRPC ";
    print "-interface $hostIfId -binding $hostBind\n";

    $sysarg  = "$DCECP -c rpcentry export $hostRPC ";
    $sysarg .= "-interface $hostIfId -binding $hostBind";

    $goterror = system("$sysarg");

    !($goterror); # return result
}


# GetDFSInfoCDS([IN] dfsNS, [OUT] *flHosts) -  Read/parse DFS junction in CDS,
#     returning list of flserver-host RPC entries
#
#     Returns: 1 - successful; 0 - failed
#
sub GetDFSInfoCDS {
    local($dfsNS, *flHosts) = @_;
    local($goterror);
    local($flidx);

    $goterror = $flidx = 0;

    print "$DCECP -c rpcgroup list $dfsNS\n";
    open(INPIPE, "$DCECP -c rpcgroup list $dfsNS |") || ($goterror = 1);

    if (!($goterror)) {
	while (<INPIPE>) {
	    if (/\S+\/self/) {
		($flHosts[$flidx++]) = /(\S+\/self)/;
	    }
	}

	close(INPIPE);

	if (!defined(@flHosts)) {
	    print "Unable to get DFS information from $dfsNS\n";
	    $goterror = 1;
	}
    }
    !($goterror); # return result
}


# GetMyAddrs([OUT] *ipaddr) - get IP addresses for host executing script.
#
#     Returns: 1 - successful; 0 - failed
#
sub GetMyAddrs{
    local(*ipaddr) = @_;

    local($i, $hostname, $goterror, @octets);

    $goterror = 0;

    $hostname = `$UNAME -n`;
    $hostname =~ s/\s//g;

    if (!defined($hostname) || ($hostname eq "")) {
	$goterror = 1;
    } else {
	local($name, $aliases, $atype, $length, @addrs) =
	    gethostbyname("$hostname");

	if (!defined(@addrs)) {
	    $goterror = 1;
	} else {
	    # return address list
	    for ($i = 0; $i <= $#addrs; $i++) {
		@octets = unpack('C4', $addrs[$i]);
		$ipaddr[$i] = "$octets[0].$octets[1].$octets[2].$octets[3]";
	    }
	}
    }
    !($goterror); # return result
}


# GetHostAggrLFS([IN] host, [OUT] *aggr) - get name of an LFS aggregate on host
#
#     Returns: 1 - successful; 0 - failed
#
sub GetHostAggrLFS {
    local($host, *aggr) = @_;
    local($goterror);

    $goterror = 0;

    print "$FTS lsaggr -server $host\n";
    open(INPIPE, "$FTS lsaggr -server $host |") || ($goterror = 1);

    if (!($goterror)) {
	while (<INPIPE>) {
	    if (/\(LFS\)/) {
		# get host aggregate name
		($aggr) = /\s*(\w+)\s+/;
		last;
	    }
	}
	close(INPIPE);

	if (!defined($aggr)) {
	    print "No LFS aggregate on $host\n";
	    $goterror = 1;
	}
    }
    !($goterror); # return result
}


# CreateFileset([IN] ftname, [IN] host, [IN] aggr) - create fileset on
#     specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub CreateFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS create -ftname $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS create $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# DeleteFileset([IN] ftname, [IN] host, [IN] aggr) - delete fileset on
#     specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub DeleteFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS delete -fileset $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS delete -fileset $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# SetRepFileset([IN] ftname) - set replication info for fileset
#
#     Returns: 1 - successful; 0 - failed
#
sub SetRepFileset {
    local($ftname) = @_;
    local($goterror);

    print "$FTS setrepinfo -fileset $ftname -scheduled\n";
    $goterror = system("$FTS setrepinfo -fileset $ftname -scheduled");

    !($goterror); # return result
}


# AddSiteFileset([IN] ftname, [IN] host, [IN] aggr) - add replication site
#     for fileset on specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub AddSiteFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS addsite -fileset $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS addsite -fileset $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# RmSiteFileset([IN] ftname, [IN] host, [IN] aggr) - remove replication site
#     for fileset on specified machine/aggregate.
#
#     Returns: 1 - successful; 0 - failed
#
sub RmSiteFileset {
    local($ftname, $host, $aggr) = @_;
    local($goterror);

    print "$FTS rmsite -fileset $ftname -server $host -aggregate $aggr\n";
    $goterror =
	system("$FTS rmsite -fileset $ftname -server $host -aggregate $aggr");

    !($goterror); # return result
}


# MountFileset([IN] mtdir, [IN] ftname, [IN] rwflag) - create mount point for
#     fileset; a RW mount point is created if rwflag is set.
#
#     Returns: 1 - successful; 0 - failed
#
sub MountFileset {
    local($mtdir, $ftname, $rwflag) = @_;
    local($goterror, $sysarg);

    print "$FTS crmount -dir $mtdir -fileset $ftname ";
    $sysarg = "$FTS crmount -dir $mtdir -fileset $ftname ";

    if ($rwflag) {
	print "-rw\n";
	$sysarg .= "-rw";
    } else {
	print "\n";
    }

    $goterror = system("$sysarg");

    !($goterror); # return result
}


# UmountFileset([IN] mtdir) - delete mount point for fileset
#
#     Returns: 1 - successful; 0 - failed
#
sub UmountFileset {
    local($mtdir) = @_;
    local($goterror);

    print "$FTS delmount -dir $mtdir\n";
    $goterror = system("$FTS delmount -dir $mtdir");

    !($goterror); # return result
}


# ChmodFileset([IN] prot, [IN] mtdir) - set protection for fileset mount-point
#
#     Returns: 1 - successful; 0 - failed
#
sub ChmodFileset {
    local($prot, $mtdir) = @_;
    local($goterror);

    printf "chmod %o %s\n", $prot, $mtdir;
    $goterror = !(chmod($prot, $mtdir));

    !($goterror); # return result
}


# UpdateFileset([IN] ftname) - force immediate update of fileset replicas
#
#     Returns: 1 - successful; 0 - failed
#
sub UpdateFileset {
    local($ftname) = @_;
    local($goterror);

    print "$FTS update -fileset $ftname -all\n";
    $goterror = system("$FTS update -fileset $ftname -all");

    !($goterror); # return result
}


# TraceOn() - turn on cache-manager tracing
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceOn {
    local($goterror);

    print "$DFSTRACE setset -set cm -active\n";
    $goterror = system("$DFSTRACE setset -set cm -active");

    !($goterror); # return result
}


# TraceClear() - clear trace log
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceClear {
    local($goterror);

    print "$DFSTRACE clear\n";
    $goterror = system("$DFSTRACE clear");

    !($goterror); # return result
}


# TraceMSBindAddr(ipaddr, stype) - search trace log for evidence that
#     multi-server binding function employed specified address for specified
#     server type.
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceMSBindAddr {
    local($ipaddr, $stype) = @_;
    local($goterror, $found, @octets, $addrval);

    $goterror = $found = 0;

    print "$DFSTRACE dump -set cm : searching for multi-server binding to ";
    if ($stype == $flType) {
	print "FL ";
    } elsif ($stype == $fxType) {
	print "FX ";
    } elsif ($stype == $repType) {
	print "REP ";
    }
    print "at $ipaddr\n";

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	(@octets) = ($ipaddr =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/);
	$addrval = pack('C4', @octets);      # recreate address value
	$addrval = unpack('L', $addrval);    # unpack it
	$addrval = sprintf("%x", $addrval);  # convert it to hex

	while (<INPIPE>) {
	    if (/cm_ConnByMHosts.*service=.*$stype.*try-addr=.*$addrval.*got-addr=.*$addrval/) {
		$found = 1;
		last;
	    }
	}
	close(INPIPE);

	if (!($found)) {
	    print "Did not find multi-server binding to $ipaddr\n";
	}
    }

    ($found); # return result
}


# TraceSSBindAddr(ipaddr, stype) - search trace log for evidence that
#     single-server binding function employed specified address for specified
#     server type.
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceSSBindAddr {
    local($ipaddr, $stype) = @_;
    local($goterror, $found, @octets, $addrval, $rstr);

    $goterror = $found = 0;

    print "$DFSTRACE dump -set cm : searching for single-server binding to ";
    if ($stype == $flType) {
	print "FL ";
    } elsif ($stype == $fxType) {
	print "FX ";
    } elsif ($stype == $repType) {
	print "REP ";
    }
    print "at $ipaddr\n";

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	(@octets) = ($ipaddr =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/);
	$addrval = pack('C4', @octets);      # recreate address value
	$addrval = unpack('L', $addrval);    # unpack it
	$addrval = sprintf("%x", $addrval);  # convert it to hex

	while (<INPIPE>) {
	    if (/cm_ConnByHost.*service=.*$stype.*try-addr=.*$addrval.*got-addr=.*$addrval/) {
		($rstr) = /([0-9a-f]*)\s*$/i;
		($rstr) = ($rstr =~ /([1-9a-f]*)/i);

		if (defined($rstr)) {
		    $found = 1;
		    last;
		}
	    }
	}
	close(INPIPE);

	if (!($found)) {
	    print "Did not find single-server binding to $ipaddr\n";
	}
    }

    ($found); # return result
}


# TraceFailedAddr(ipaddr) - search trace log for evidence that address
#     was marked down.
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceFailedAddr {
    local($ipaddr) = @_;
    local($goterror, $found, @octets, $addrval);

    $goterror = $found = 0;

    print "$DFSTRACE dump -set cm : searching for failure of $ipaddr\n";

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	(@octets) = ($ipaddr =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/);
	$addrval = pack('C4', @octets);      # recreate address value
	$addrval = unpack('L', $addrval);    # unpack it
	$addrval = sprintf("%x", $addrval);  # convert it to hex

	while (<INPIPE>) {
	    if (/cm_SiteAddrDown.*addr=.*$addrval/) {
		$found = 1;
		last;
	    }
	}
	close(INPIPE);

	if (!($found)) {
	    print "Did not find failure of $ipaddr\n";
	}
    }

    ($found); # return result
}


# TraceRevivedAddr(ipaddr) - search trace log for evidence that address
#     was marked back up.
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceRevivedAddr {
    local($ipaddr) = @_;
    local($goterror, $found, @octets, $addrval);

    $goterror = $found = 0;

    print "$DFSTRACE dump -set cm : searching for revival of $ipaddr\n";

    # first grep log for standard revival

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	(@octets) = ($ipaddr =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/);
	$addrval = pack('C4', @octets);      # recreate address value
	$addrval = unpack('L', $addrval);    # unpack it
	$addrval = sprintf("%x", $addrval);  # convert it to hex

	while (<INPIPE>) {
	    if (/cm_SiteAddrUp.*addr=.*$addrval/) {
		$found = 1;
		last;
	    }
	}
	close(INPIPE);
    }

    # if didn't find standard revival, grep log for an override

    if (!($goterror) && !($found)) {
	open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

	if (!($goterror)) {
	    while (<INPIPE>) {
		if (/cm_SiteAddrRankOverride.*addr=.*$addrval/) {
		    $found = 1;
		    last;
		}
	    }
	    close(INPIPE);

	    if (!($found)) {
		print "Did not find revival of $ipaddr\n";
	    }
	}
    }

    ($found); # return result
}


# TraceDownServer(ipaddr) - search trace log for evidence that server was
#     marked down due to failure of address.
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceDownServer {
    local($ipaddr) = @_;
    local($goterror, $found, @octets, $addrval, $phase);

    $goterror = $found = $phase = 0;

    print "$DFSTRACE dump -set cm : searching for server down after ";
    print "failure of $ipaddr\n";

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	(@octets) = ($ipaddr =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/);
	$addrval = pack('C4', @octets);      # recreate address value
	$addrval = unpack('L', $addrval);    # unpack it
	$addrval = sprintf("%x", $addrval);  # convert it to hex

	while (<INPIPE>) {
	    if ($phase == 0) {
		# addr failure should appear first...
		if (/cm_SiteAddrDown.*addr=.*$addrval/) {
		    $phase = 1;
		}
	    } else {
		# ...followed by indication that a server was marked down
		if (/CM\s+server.*marked\s+down/) {
		    $found = 1;
		    last;
		}
	    }
	}
	close(INPIPE);

	if (!($found)) {
	    print "Did not find server down after failure of $ipaddr\n";
	}
    }

    ($found); # return result
}


# TraceUpServer(ipaddr) - search trace log for evidence that server was
#     marked back up after successful contact via address
#
#     NOTE: currently uses very weak heuristic; need better tracing in CM;
#           one problem is that server can get marked back up in several ways
#
#     Returns: 1 - successful; 0 - failed
#
sub TraceUpServer {
    local($ipaddr) = @_;
    local($goterror, $found, @octets, $addrval, $phase, $rstr);

    $goterror = $found = $phase = 0;

    print "$DFSTRACE dump -set cm : searching for server up after ";
    print "contact via $ipaddr\n";

    # first grep log for standard mark-up by background daemon

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	(@octets) = ($ipaddr =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/);
	$addrval = pack('C4', @octets);      # recreate address value
	$addrval = unpack('L', $addrval);    # unpack it
	$addrval = sprintf("%x", $addrval);  # convert it to hex

	while (<INPIPE>) {
	    if ($phase == 0) {
		# enter check-server routine should appear first...
		if (/CM\s+checkdownservers\s+server/) {
		    $phase = 1;
		    $found = 1;
		}
	    } else {
		# ...with NO mark-down of address after
		if (/cm_SiteAddrDown.*addr=.*$addrval/) {
		    $found = 0;
		    last;
		}
	    }
	}
	close(INPIPE);
    }

    # if didn't find standard mark-up, grep log for alternates

    if (!($goterror) && !($found)) {
	open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

	if (!($goterror)) {
	    while (<INPIPE>) {
		if ((/cm_ConnByMHosts.*service=.*$fxType.*try-addr=.*$addrval.*got-addr=.*$addrval/) ||
		    (/cm_ConnByHost.*service=.*$fxType.*try-addr=.*$addrval.*got-addr=.*$addrval/)) {
		    ($rstr) = /([0-9a-f]*)\s*$/i;
		    ($rstr) = ($rstr =~ /([1-9a-f]*)/i);

		    if (defined($rstr)) {
			$found = 1;
			last;
		    }
		}
	    }

	    close(INPIPE);

	    if (!($found)) {
		print "Did not find server up after contact via $ipaddr\n";
	    }
	}
    }

    ($found); # return result
}


# TraceBogusFail() - search trace log for evidence that a test failure
#     is due to circumstances beyond our control, and does not really
#     indicate an error in DFS (or the test).
#
#     Returns: 1 - successful (found evidence of bogusness); 0 - failed
#
sub TraceBogusFail {
    local($goterror, $found);

    $goterror = $found = 0;

    open(INPIPE, "$DFSTRACE dump -set cm |") || ($goterror = 1);

    if (!($goterror)) {
	while (<INPIPE>) {
	    if (/cm_SiteAddrRankOverride/) {
		$found = 1;
		last;
	    }
	}
	close(INPIPE);
    }

    ($found); # return result
}


1; # needed for perl require()
