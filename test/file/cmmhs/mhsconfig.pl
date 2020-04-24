# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Common configuration file
#
#   This is the common configuration file for all tests in the suite.

# ----------------------------------------------------------------------------
# Configuration parameters to be set manually

# Server-host-A's directory in CDS (server-host executing flserver)
$dirA = "";    # e.g. /.:/hosts/hostA

# Server-host-B's directory in CDS (server-host NOT executing flserver)
$dirB = "";    # e.g. /.:/hosts/hostB

# Server-hosts' network connection information
$dnA1 = "";    # server-host-A ntwk-connection-1 domain name
$ifA1 = "";    # server-host-A ntwk-connection-1 interface name (e.g. le0)
$gwA1 = "";    # gateway on server-host-A ntwk-connection-1 subnet

$dnA2 = "";    # server-host-A ntwk-connection-2 domain name
$ifA2 = "";    # server-host-A ntwk-connection-2 interface name (e.g. le1)
$gwA2 = "";    # gateway on server-host-A ntwk-connection-2 subnet

$dnB1 = "";    # server-host-B ntwk-connection-1 domain name
$ifB1 = "";    # server-host-B ntwk-connection-1 interface name (e.g. le0)
$gwB1 = "";    # gateway on server-host-B ntwk-connection-1 subnet

$dnB2 = "";    # server-host-B ntwk-connection-2 domain name
$ifB2 = "";    # server-host-B ntwk-connection-2 interface name (e.g. le1)
$gwB2 = "";    # gateway on server-host-B ntwk-connection-2 subnet

# Fileset name and mount-point (to be created by test)
#   note: mount-point path should not traverse filesets on server-host-B
$ftName = "cmMHSTest";             # fileset name prefix
$mtPath = "/:/.rw/$ftName";            # fileset mount point prefix

# DFS namespace junction in CDS
$dfsNS = "/.:/fs";

# Identity information
$LCL_ROOT   = "root";    # specify local root
$CELL_ADMIN = "cell_admin";    # specify DCE cell-administrator

# Required utilities
$CM       = "cm";
$FTS      = "fts";
$DCECP    = "dcecp";
$DFSTRACE = "dfstrace";
$KLIST    = "klist";
$WHOAMI   = "/usr/ucb/whoami";
$CAT      = "cat";
$UNAME    = "uname";
$RSH      = "rsh -n";
$IFCONFIG = "ifconfig";
$ROUTE    = "route";

1; # needed for perl require()
