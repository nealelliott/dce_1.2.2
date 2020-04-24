# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Common include file
#
#   This is the common include file for all tests in the suite.

# ----------------------------------------------------------------------------

# Local fileset replication parameters (in seconds)

$INIT_MINREPDELAY = 60;
$INIT_DEFSITEAGE  = 180;

$REPAUTH_MAXAGE = 240;

# Numeric server-type identifiers (from src/file/cm/cm_server.h)

$FL_ID  = 1;    # SRT_FL
$FX_ID  = 2;    # SRT_FX
$REP_ID = 3;    # SRT_REP

# Volume badness info timeout (from src/file/cm/cm_rrequest.h)

$FT_BADTIME = 20 + 5;    # 20 second timeout (CM_RREQ_SLEEP) + grace period

# RPC authorization-service values

%AUTHZ_SVC = ("none", 0,
	      "name", 1,
	      "dce",  2);


# RPC protection-level values

%PROT_LVL = (1, "none",
	     2, "connect",
	     3, "call",
	     4, "pkt",
	     5, "pkt_integ",
	     6, "pkt_privacy");

$PROT_LVL_PREFIX = "rpc_c_protect_level_";

$PROT_LVL_MIN = 1;
$PROT_LVL_MAX = 6;
$PROT_LVL_DEF = 0;


# RPC protection-level responses from 'fts' and 'cm' commands

$FLDB_MINLCL_STR = "Minimum local protection level:";
$FLDB_MAXLCL_STR = "Maximum local protection level:";

$FLDB_MINRMT_STR = "Minimum remote protection level:";
$FLDB_MAXRMT_STR = "Maximum remote protection level:";

$CM_MINLCL_STR = "Minimum protection level in the local cell:";
$CM_INILCL_STR = "Initial protection level in the local cell:";

$CM_MINRMT_STR = "Minimum protection level in non-local cells:";
$CM_INILCL_STR = "Initial protection level in non-local cells:";


# RPC protection-level map vector
#   The CM currently uses only UDP.  Because only four protection-levels
#   exist for UDP transport, the RPC runtime remaps two levels transparently.
#   Note: code presumes that remapping results in a higher protection level.

%PROT_LVL_MAP = (1, 1,
		 2, 4,
		 3, 4,
		 4, 4,
		 5, 5,
		 6, 6);


1; # need for 'require()'
