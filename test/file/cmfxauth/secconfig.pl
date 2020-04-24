# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Configuration file
#
#   Describes the execution environment for all tests in the suite.

# ----------------------------------------------------------------------------
# Configuration parameters to be set manually

# Cell information
$LCL_CELL = "";    # local cell (/.../lcl_cell)
$RMT_CELL = "";    # remote cell (/.../rmt_cell)

# Host information - DNS
$LCL_HOSTA = "";    # local server-A host name
$LCL_HOSTB = "";    # local server-B host name
$RMT_HOSTC = "";    # remote server-C host name

# Host information - CDS
$LCL_HOSTA_DIR = "$LCL_CELL/hosts/$LCL_HOSTA";    # local server-A host dir
$LCL_HOSTB_DIR = "$LCL_CELL/hosts/$LCL_HOSTB";    # local server-B host dir
$RMT_HOSTC_DIR = "$RMT_CELL/hosts/$RMT_HOSTC";    # remote server-C host dir

# Aggregate information
$LCL_HOSTA_AGGR = "";    # local server-A LFS aggregate (to be used)
$LCL_HOSTB_AGGR = "";    # local server-B LFS aggregate (to be used)

# Identity information
$LCL_USER = "";    # local non-root user
$LCL_ROOT = "root";    # local root

$LCL_CELL_ADMIN        = "cell_admin";    # local cell_admin principal
$LCL_CELL_ADMIN_PASSWD = "-dce-";    # local cell_admin password

# Local fileset-name and mount-point (to be created by test)
$LCL_FTNAME = "cmXSecTest";    # fileset name
$LCL_MTPATH = "$LCL_CELL/fs/$LCL_FTNAME";    # mount-point prefix

$LCL_MTPATH_RW = "$LCL_MTPATH.RW";
$LCL_MTPATH_RO = "$LCL_MTPATH.RO";

$LCL_RWFILE = "$LCL_MTPATH_RW/Testfile.txt";
$LCL_ROFILE = "$LCL_MTPATH_RO/Testfile.txt";

# Remote fileset-name and file path (NOT created by test)
#   - file is presumed extant, readable from local cell, and located
#     on $RMT_HOSTC in fileset $RMT_FTNAME (in cell $RMT_CELL)

$RMT_FTNAME = "";    # fileset name
$RMT_RWFILE = "";    # file path

# ICL log-file directories
$LCL_HOSTA_LOG = "/opt/dcelocal/var/dfs/adm";
$LCL_HOSTB_LOG = "/opt/dcelocal/var/dfs/adm";

# Wait time in seconds for various (unbounded-time) events; try increasing
# if a test failure occurs after a sleep.
$SLEEPTIME = 120;

# Required utilities
$DCE_LOGIN   = "dce_login";
$KLIST       = "klist";
$FTS         = "fts";
$CM          = "cm";
$BOS         = "/bin/bos";
$DFSEXPORT   = "dfsexport";
$DCECP       = "dcecp";
$DFSD        = "dfsd";
$FXD         = "fxd";
$DFSTRACE    = "dfstrace";
$SU          = "su";
$WHOAMI      = "/usr/ucb/whoami";
$RSH         = "rsh -n";
$CAT         = "cat";
$TOUCH       = "touch";
$FXSECADVICE = "/bin/fxsecadvice";

1; # need for 'require()'
