#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: log_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:27  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:45  root]
# 
# Revision 1.1.2.3  1994/10/05  17:22:08  salamone
# 	CR12479 - Due to CR12457, change test comparison.
# 	[1994/10/05  15:41:12  salamone]
# 
# Revision 1.1.2.2  1994/10/01  01:33:17  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:27:40  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:40:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:02  melman]
# 
# Revision 1.1.2.7  1994/09/20  15:14:43  salamone
# 	Use new string binding constants for dts server.
# 	[1994/09/20  14:45:05  salamone]
# 
# Revision 1.1.2.6  1994/09/08  14:01:03  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:34  salamone]
# 
# Revision 1.1.2.5  1994/08/19  18:04:20  salamone
# 	CR10683 - Handle 1 string binding in TCL syntax.
# 	[1994/08/19  17:54:49  salamone]
# 
# Revision 1.1.2.4  1994/06/10  20:13:54  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:49  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:19  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:38  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:41:33  melman
# 	updating to new API path
# 	[1994/05/06  15:41:14  melman]
# 
# Revision 1.1.2.1  1994/05/02  19:26:20  salamone
# 	Initial version.
# 	[1994/04/29  20:24:52  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc log_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Set up variables for string binding tests
    dcp_log_binding_vars
}

proc log_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc log_modify_019_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_019_P: Modify a production routing with fatal error messages 
    tet_infoline log_modify_019_P: processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch FATAL:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL BINFILE /tmp/svc"]
	} compare_strings {found}


    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_020_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_020_P: Modify a production routing with fatal error messages 
    tet_infoline log_modify_020_P: processed as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch FATAL:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_021_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_021_P: Modify a production routing with fatal error messages 
    tet_infoline log_modify_021_P: processed as human-readable file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch FATAL:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_022_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_022_P: Modify a production routing with fatal error messages 
    tet_infoline log_modify_022_P: discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch FATAL:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_023_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_023_P: Modify a production routing with fatal error messages 
    tet_infoline log_modify_023_P: written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch FATAL:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_024_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_024_P: Modify a production routing with fatal error messages 
    tet_infoline log_modify_024_P: written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch FATAL:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_025_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_025_P: Modify a production routing with normal exit messages 
    tet_infoline log_modify_025_P: processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch ERROR:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_026_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_026_P: Modify a production routing with normal exit messages 
    tet_infoline log_modify_026_P: processed as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch ERROR:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_027_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_027_P: Modify a production routing with normal exit messages 
    tet_infoline log_modify_027_P: processed as human-readable file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch ERROR:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_028_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_028_P: Modify a production routing with normal exit messages 
    tet_infoline log_modify_028_P: discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch ERROR:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_029_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_029_P: Modify a production routing with normal exit messages 
    tet_infoline log_modify_029_P: written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch ERROR:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_030_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_030_P: Modify a production routing with normal exit messages 
    tet_infoline log_modify_030_P: written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch ERROR:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_031_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_031_P: Modify a production routing with warning messages 
    tet_infoline log_modify_031_P: processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch WARNING:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "WARNING BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_032_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_032_P: Modify a production routing with warning messages 
    tet_infoline log_modify_032_P: processed as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch WARNING:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "WARNING TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_033_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_033_P: Modify a production routing with warning messages 
    tet_infoline log_modify_033_P: processed as human-readable file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch WARNING:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "WARNING FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_034_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_034_P: Modify a production routing with warning messages 
    tet_infoline log_modify_034_P: discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch WARNING:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "WARNING DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_035_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_035_P: Modify a production routing with warning messages 
    tet_infoline log_modify_035_P: written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch WARNING:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "WARNING STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_036_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_036_P: Modify a production routing with warning messages 
    tet_infoline log_modify_036_P: written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch WARNING:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "WARNING STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_037_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_037_P: Modify a production routing with informational messages 
    tet_infoline log_modify_037_P: processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_038_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_038_P: Modify a production routing with informational messages 
    tet_infoline log_modify_038_P: processed as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_039_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_039_P: Modify a production routing with informational messages 
    tet_infoline log_modify_039_P: processed as human-readable file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_040_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_040_P: Modify a production routing with informational messages 
    tet_infoline log_modify_040_P: discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_041_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_041_P: Modify a production routing with informational messages 
    tet_infoline log_modify_041_P: written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_042_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_042_P: Modify a production routing with informational messages 
    tet_infoline log_modify_042_P: written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_043_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_043_P: Modify a production routing with verbose informational 
    tet_infoline log_modify_043_P: messages processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE_VERBOSE:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE_VERBOSE BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_044_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_044_P: Modify a production routing with verbose informational 
    tet_infoline log_modify_044_P: messages processed as human-readable text entries for 
    tet_infoline log_modify_044_P: 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE_VERBOSE:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE_VERBOSE TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_045_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_045_P: Modify a production routing with verbose informational 
    tet_infoline log_modify_045_P: messages processed as human-readable file entries for 1 
    tet_infoline log_modify_045_P: server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE_VERBOSE:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE_VERBOSE FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_046_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_046_P: Modify a production routing with verbose informational 
    tet_infoline log_modify_046_P: messages discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE_VERBOSE:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE_VERBOSE DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_047_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_047_P: Modify a production routing with verbose informational 
    tet_infoline log_modify_047_P: messages written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE_VERBOSE:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE_VERBOSE STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_048_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_048_P: Modify a production routing with verbose informational 
    tet_infoline log_modify_048_P: messages written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch NOTICE_VERBOSE:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "NOTICE_VERBOSE STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_049_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_049_P: Modify a production routing with all messages processed 
    tet_infoline log_modify_049_P: as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch *:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL BINFILE /tmp/svc" "ERROR BINFILE /tmp/svc" "WARNING BINFILE /tmp/svc" "NOTICE BINFILE /tmp/svc" "NOTICE_VERBOSE BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_050_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_050_P: Modify a production routing with all messages processed 
    tet_infoline log_modify_050_P: as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch *:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL TEXTFILE /tmp/svc" "ERROR TEXTFILE /tmp/svc" "WARNING TEXTFILE /tmp/svc" "NOTICE TEXTFILE /tmp/svc" "NOTICE_VERBOSE TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_051_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_051_P: Modify a production routing with all messages processed 
    tet_infoline log_modify_051_P: as human-readable file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch *:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL FILE /tmp/svc" "ERROR FILE /tmp/svc" "WARNING FILE /tmp/svc" "NOTICE FILE /tmp/svc" "NOTICE_VERBOSE FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_052_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_052_P: Modify a production routing with all messages discarded 
    tet_infoline log_modify_052_P: for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch *:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL DISCARD" "ERROR DISCARD" "WARNING DISCARD" "NOTICE DISCARD" "NOTICE_VERBOSE DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_053_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_053_P: Modify a production routing with all messages written to 
    tet_infoline log_modify_053_P: stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch *:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL STDOUT" "ERROR STDOUT" "WARNING STDOUT" "NOTICE STDOUT" "NOTICE_VERBOSE STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_054_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_054_P: Modify a production routing with all messages written to 
    tet_infoline log_modify_054_P: stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch *:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL STDERR" "ERROR STDERR" "WARNING STDERR" "NOTICE STDERR" "NOTICE_VERBOSE STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_055_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_055_P: Modify a debug routing with debug messages for a component
    tet_infoline log_modify_055_P: processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.9:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_056_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_056_P: Modify a debug routing with debug messages for a component 
    tet_infoline log_modify_056_P: processed as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.9:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_057_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_057_P: Modify a debug routing with debug messages for a component 
    tet_infoline log_modify_057_P: processed as human-readable file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.9:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_058_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_058_P: Modify a debug routing with debug messages for a component 
    tet_infoline log_modify_058_P: discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.9:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_059_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_059_P: Modify a debug routing with debug messages for a component 
    tet_infoline log_modify_059_P: written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.9:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_060_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_060_P: Modify a debug routing with debug messages for a component 
    tet_infoline log_modify_060_P: written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.9:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_061_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_061_P: Modify a debug routing with debug messages for all
    tet_infoline log_modify_061_P: sub-components of a component processed as binary log 
    tet_infoline log_modify_061_P: entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:*.9:BINFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "* 9"] BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_062_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_062_P: Modify a debug routing with debug messages for all
    tet_infoline log_modify_062_P: sub-components of a component processed as human-readable 
    tet_infoline log_modify_062_P: text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:*.9:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "* 9"] TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_063_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_063_P: Modify a debug routing with debug messages for all
    tet_infoline log_modify_063_P: sub-components of a component processed as human-readable 
    tet_infoline log_modify_063_P: file entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:*.9:FILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "* 9"] FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_064_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_064_P: Modify a debug routing with debug messages for all
    tet_infoline log_modify_064_P: sub-components of a component discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:*.9:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "* 9"] DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_065_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_065_P: Modify a debug routing with debug messages for all
    tet_infoline log_modify_065_P: sub-components of a component written to stdout for 
    tet_infoline log_modify_065_P: 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:*.9:STDOUT:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "* 9"] STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_066_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_066_P: Modify a debug routing with debug messages for all 
    tet_infoline log_modify_066_P: sub-components of a component written to stderr for 
    tet_infoline log_modify_066_P: 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:*.9:STDERR:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "* 9"] STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_067_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_067_P: Modify a production routing for more than 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -ch FATAL:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" {} [list "FATAL DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_068_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_068_P: Modify a debug routing for more than 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_TWO}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -ch $DCP_LOG_COMP_TWO:general.9:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" debug [list "$DCP_LOG_COMP_TWO [list "general 9"] DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_069_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_069_P: Modify more than 1 production routing for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "FATAL:DISCARD: WARNING:DISCARD:"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL DISCARD" "WARNING DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_070_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_070_P: Modify more than 1 debug routing for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE DCP_LOG_COMP_TWO}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE:general.4:DISCARD: $DCP_LOG_COMP_TWO:general.8:DISCARD:"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 4"] DISCARD" "$DCP_LOG_COMP_TWO [list "general 8"] DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_071_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_071_P: Modify more than 1 production routing for more than 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -ch "ERROR:DISCARD: NOTICE:DISCARD:"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "ERROR DISCARD" "NOTICE DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_072_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_072_P: Modify a production routing in TCL syntax with fatal 
    tet_infoline log_modify_072_P: error messages processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch {FATAL BINFILE /tmp/svc}
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_073_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_073_P: Modify a production routing in TCL syntax with fatal 
    tet_infoline log_modify_073_P: error messages processed as human-readable text entries 
    tet_infoline log_modify_073_P: for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch {FATAL TEXTFILE /tmp/svc}
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_074_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_074_P: Modify a production routing in TCL syntax with fatal 
    tet_infoline log_modify_074_P: error messages processed as human-readable file entries 
    tet_infoline log_modify_074_P: for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch {FATAL FILE /tmp/svc}
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_075_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_075_P: Modify a production routing in TCL syntax with fatal 
    tet_infoline log_modify_075_P: error messages discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch {FATAL DISCARD}
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_076_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_076_P: Modify a production routing in TCL syntax with fatal 
    tet_infoline log_modify_076_P: error messages written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch {FATAL STDOUT}
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_077_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_077_P: Modify a production routing in TCL syntax with fatal 
    tet_infoline log_modify_077_P: error messages written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch {FATAL STDERR}
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_078_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_078_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_078_P: for a component processed as binary log entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE [list "general 9"] BINFILE /tmp/svc"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] BINFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_079_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_079_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_079_P: for a component processed as human-readable text entries 
    tet_infoline log_modify_079_P: for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE [list "general 9"] TEXTFILE /tmp/svc"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_080_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_080_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_080_P: for a component processed as human-readable file entries 
    tet_infoline log_modify_080_P: for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE [list "general 9"] FILE /tmp/svc"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] FILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_081_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_081_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_081_P: for a component discarded for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE [list "general 9"] DISCARD"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_082_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_082_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_082_P: for a component written to stdout for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE [list "general 9"] STDOUT"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] STDOUT"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_083_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_083_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_083_P: for a component written to stderr for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "$DCP_LOG_COMP_ONE [list "general 9"] STDERR"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 9"] STDERR"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_084_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_084_P: Modify a debug routing in TCL syntax with debug messages 
    tet_infoline log_modify_084_P: for a component with multiple sub-components processed 
    tet_infoline log_modify_084_P: as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch [list "$DCP_LOG_COMP_ONE [list "[list "msgs 2" "general 9"]"] TEXTFILE /tmp/svc"]
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "[list "msgs 2" "general 9"]"] TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_085_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_085_P: Modify a debug routing in string syntax with debug messages 
    tet_infoline log_modify_085_P: for a component with multiple sub-components processed 
    tet_infoline log_modify_085_P: as human-readable text entries for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:states.4,events.5:TEXTFILE:/tmp/svc
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "[list "states 4" "events 5"]"] TEXTFILE /tmp/svc"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}

proc log_modify_086_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_086_P: Modify a production routing for 1 server specified as a 
    tet_infoline log_modify_086_P: string binding with just an endpoint in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" -ch FATAL:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" {} [list "FATAL DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_087_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_087_P: Modify a production routing for 1 server specified as a 
    tet_infoline log_modify_087_P: string binding with just an endpoint in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT" -ch ERROR:DISCARD:
	   dcp_log_find_multi_routings [list "$DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT"] {} [list "ERROR DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_088_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_088_P: Modify a production routing for 1 server specified as a 
    tet_infoline log_modify_088_P: string binding with an endpoint and UUID in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_OBJECT_UUID@$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" -ch WARNING:DISCARD:
	   dcp_log_find_multi_routings "$DCP_LOG_OBJECT_UUID@$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" {} [list "WARNING DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_089_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_089_P: Modify a production routing for 1 server specified as a 
    tet_infoline log_modify_089_P: string binding with an endpoint and UUID in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    test { log modify "$DCP_LOG_OBJECT_UUID $DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT" -ch NOTICE:DISCARD:
	   dcp_log_find_multi_routings [list "$DCP_LOG_OBJECT_UUID $DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT"] {} [list "NOTICE DISCARD"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_modify_090_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_modify_090_P: Modify a mixture of production and debug routings for 1 server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE DCP_LOG_COMP_TWO}] != 0} {return;}

    # Run the test
    test { log modify $DCP_LOG_SRVR_ONE -ch "FATAL:DISCARD: $DCP_LOG_COMP_ONE:general.6:TEXTFILE:/tmp/svc WARNING:DISCARD: $DCP_LOG_COMP_TWO:general.3:DISCARD:"
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL DISCARD" "WARNING DISCARD"]
	} compare_strings {found}

    test { dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug [list "$DCP_LOG_COMP_ONE [list "general 6"] TEXTFILE /tmp/svc" "$DCP_LOG_COMP_TWO [list "general 3"] DISCARD"]
	} compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {exec rm -f /tmp/svc}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 "
set ic1 "log_modify_019_P"
set ic2 "log_modify_020_P"
set ic3 "log_modify_021_P"
set ic4 "log_modify_022_P"
set ic5 "log_modify_023_P"
set ic6 "log_modify_024_P"
set ic7 "log_modify_025_P"
set ic8 "log_modify_026_P"
set ic9 "log_modify_027_P"
set ic10 "log_modify_028_P"
set ic11 "log_modify_029_P"
set ic12 "log_modify_030_P"
set ic13 "log_modify_031_P"
set ic14 "log_modify_032_P"
set ic15 "log_modify_033_P"
set ic16 "log_modify_034_P"
set ic17 "log_modify_035_P"
set ic18 "log_modify_036_P"
set ic19 "log_modify_037_P"
set ic20 "log_modify_038_P"
set ic21 "log_modify_039_P"
set ic22 "log_modify_040_P"
set ic23 "log_modify_041_P"
set ic24 "log_modify_042_P"
set ic25 "log_modify_043_P"
set ic26 "log_modify_044_P"
set ic27 "log_modify_045_P"
set ic28 "log_modify_046_P"
set ic29 "log_modify_047_P"
set ic30 "log_modify_048_P"
set ic31 "log_modify_049_P"
set ic32 "log_modify_050_P"
set ic33 "log_modify_051_P"
set ic34 "log_modify_052_P"
set ic35 "log_modify_053_P"
set ic36 "log_modify_054_P"
set ic37 "log_modify_055_P"
set ic38 "log_modify_056_P"
set ic39 "log_modify_057_P"
set ic40 "log_modify_058_P"
set ic41 "log_modify_059_P"
set ic42 "log_modify_060_P"
set ic43 "log_modify_061_P"
set ic44 "log_modify_062_P"
set ic45 "log_modify_063_P"
set ic46 "log_modify_064_P"
set ic47 "log_modify_065_P"
set ic48 "log_modify_066_P"
set ic49 "log_modify_067_P"
set ic50 "log_modify_068_P"
set ic51 "log_modify_069_P"
set ic52 "log_modify_070_P"
set ic53 "log_modify_071_P"
set ic54 "log_modify_072_P"
set ic55 "log_modify_073_P"
set ic56 "log_modify_074_P"
set ic57 "log_modify_075_P"
set ic58 "log_modify_076_P"
set ic59 "log_modify_077_P"
set ic60 "log_modify_078_P"
set ic61 "log_modify_079_P"
set ic62 "log_modify_080_P"
set ic63 "log_modify_081_P"
set ic64 "log_modify_082_P"
set ic65 "log_modify_083_P"
set ic66 "log_modify_084_P"
set ic67 "log_modify_085_P"
set ic68 "log_modify_086_P"
set ic69 "log_modify_087_P"
set ic70 "log_modify_088_P"
set ic71 "log_modify_089_P"
set ic72 "log_modify_090_P"
set tet_startup log_modify_startup
set tet_cleanup log_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
