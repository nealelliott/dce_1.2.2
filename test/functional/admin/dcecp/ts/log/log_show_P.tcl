#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: log_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:29  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:48  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:20  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:27:44  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:40:49  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:05  melman]
# 
# Revision 1.1.2.8  1994/09/20  15:14:45  salamone
# 	Use new string binding constants for dts server.
# 	[1994/09/20  14:49:17  salamone]
# 
# Revision 1.1.2.7  1994/09/08  14:01:07  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:37  salamone]
# 
# Revision 1.1.2.6  1994/08/19  18:04:19  salamone
# 	CR10683 - Handle 1 string binding in TCL syntax.
# 	[1994/08/19  17:54:48  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:13:57  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:53  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:41:05  salamone
# 	CR10636 - dcecp "log show" and "log list" should display the
# 		  appropriate info if unauthenticated.
# 	[1994/05/13  22:27:09  salamone]
# 
# Revision 1.1.2.3  1994/05/11  19:38:22  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:50  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:41:35  melman
# 	updating to new API path
# 	[1994/05/06  15:41:20  melman]
# 
# Revision 1.1.2.1  1994/05/02  19:26:15  salamone
# 	Initial version.
# 	[1994/04/29  20:25:47  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc log_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Set up variables for string binding tests
    dcp_log_binding_vars
}

proc log_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc log_show_010_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_010_P: Display all production routings for 1 server, which
    tet_infoline log_show_010_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {log modify $DCP_LOG_SRVR_ONE -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log show $DCP_LOG_SRVR_ONE } 

    # Cleanup steps
}

proc log_show_011_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_011_P: Display all production routings for 1 server, which
    tet_infoline log_show_011_P: is enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {log modify $DCP_LOG_SRVR_ONE -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show "$DCP_LOG_SRVR_ONE"}

    # Cleanup steps
}

proc log_show_012_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_012_P: Display all production routings for more than 1 server,
    tet_infoline log_show_012_P: which are not individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {log modify "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log show "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO"}

    # Cleanup steps
}

proc log_show_013_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_013_P: Display all production routings for more than 1 server,
    tet_infoline log_show_013_P: which are individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {log modify "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log show [list "$DCP_LOG_SRVR_ONE" "$DCP_LOG_SRVR_TWO"]}

    # Cleanup steps
}

proc log_show_014_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_014_P: Display all debug routings of 1 specified component 
    tet_infoline log_show_014_P: for 1 server, which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_TWO}] != 0} {return;}
    if {[dcp_wrap {log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_TWO:general.9:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show $DCP_LOG_SRVR_ONE -debug} 

    # Cleanup steps
}

proc log_show_015_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_015_P: Display all debug routings for more than 1 server,
    tet_infoline log_show_015_P: which are not individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_TWO}] != 0} {return;}
    if {[dcp_wrap {log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_TWO:general.9:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -debug}

    # Cleanup steps
}

proc log_show_016_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_016_P: Display production routing specification with no
    tet_infoline log_show_016_P: "where" field and a "private" field specified.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {log modify $DCP_LOG_SRVR_ONE -ch FATAL:DISCARD::private-data}] != 0} {return;}

    # Run the test
    test { log show $DCP_LOG_SRVR_ONE
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" {} [list "FATAL DISCARD {} private-data"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_show_017_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_017_P: Display debug routing specification with no
    tet_infoline log_show_017_P: "where" field and a "private" field specified.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}
    if {[dcp_wrap {log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.1:DISCARD::private-data}] != 0} {return;}

    # Run the test
    test { log show $DCP_LOG_SRVR_ONE -debug
	   dcp_log_find_multi_routings "$DCP_LOG_SRVR_ONE" debug  [list "$DCP_LOG_COMP_ONE [list "general 1"] DISCARD {} private-data"]
	} compare_strings {found}

    # Cleanup steps
}

proc log_show_018_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_018_P: Display all production routings for 1 server specified as a 
    tet_infoline log_show_018_P: string binding with just an endpoint in
    tet_infoline log_show_018_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {log modify "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]"}

    # Cleanup steps
}

proc log_show_019_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_019_P: Display all production routings for 1 server specified as a 
    tet_infoline log_show_019_P: string binding with just an endpoint in
    tet_infoline log_show_019_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {log modify "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show "$DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT"}

    # Cleanup steps
}

proc log_show_020_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_020_P: Display all production routings for 1 server specified as a 
    tet_infoline log_show_020_P: string binding with an endpoint and UUID in
    tet_infoline log_show_020_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {log modify "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show "$DCP_LOG_OBJECT_UUID@$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]"}

    # Cleanup steps
}

proc log_show_021_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_021_P: Display all production routings for 1 server specified as a 
    tet_infoline log_show_021_P: string binding with an endpoint and UUID in
    tet_infoline log_show_021_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {log modify "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]" -ch FATAL:DISCARD:}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log show "$DCP_LOG_OBJECT_UUID $DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT"}

    # Cleanup steps
}

proc log_show_022_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_show_022_P: Display all production routings for 1 server for an
    tet_infoline log_show_022_P: unauthenticated user.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log show $DCP_LOG_SRVR_ONE } 

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

}

proc log_show_023_P {} {
    # Identify the test
    tet_infoline log_show_023_P: Display all debug routings for 1 server since there is only
    tet_infoline log_show_023_P: one option.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log show $DCP_LOG_SRVR_ONE -} 

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 "
set ic1 "log_show_010_P"
set ic2 "log_show_011_P"
set ic3 "log_show_012_P"
set ic4 "log_show_013_P"
set ic5 "log_show_014_P"
set ic6 "log_show_015_P"
set ic7 "log_show_016_P"
set ic8 "log_show_017_P"
set ic9 "log_show_018_P"
set ic10 "log_show_019_P"
set ic11 "log_show_020_P"
set ic12 "log_show_021_P"
set ic13 "log_show_022_P"
set ic14 "log_show_023_P"
set tet_startup log_show_startup
set tet_cleanup log_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
