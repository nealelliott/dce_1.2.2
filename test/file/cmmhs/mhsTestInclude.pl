# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For Multi-homed DFS Server Support - Common include file
#
#   This is the common include file for all tests in the suite.

# ----------------------------------------------------------------------------

# Time interval definitions (from src/file/cm/cm_daemons.c)

$deadPollTime = (3 * 60) + (1 * 60);  # dead-server poll interval + grace
$livePollTime = (15 * 60) + (1 * 60); # addr-revival poll interval + grace

# Numeric server-type identifiers (from src/file/cm/cm_server.h)

$flType  = 1;    # SRT_FL
$fxType  = 2;    # SRT_FX
$repType = 3;    # SRT_REP

1; # needed for perl require()
