#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:11  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:00  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:06  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:41  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:55  melman]
# 
# Revision 1.1.2.7  1994/09/21  16:59:31  rousseau
# 	Test cleanup.
# 	[1994/09/21  16:59:22  rousseau]
# 
# Revision 1.1.2.6  1994/09/02  22:21:07  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:42  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:41  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:14  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:02  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:10  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:13  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:50  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:56  melman
# 	updating to new API path
# 	[1994/05/06  15:35:26  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:53  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:59  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_modify_6_P {} {
    # Identify the test
    tet_infoline dts_modify_6_P: Check the handling of the -globaltimeout option.

    # Setup steps

    # Run the test
    test {
	dts modify -globaltimeout +0-00:00:20.000I-----
	list [dcp_get_element [dts show] globaltimeout]
    } compare_strings {+0-00:00:20.000I-----}

    # Cleanup steps
}

proc dts_modify_9_P {} {
    # Identify the test
    tet_infoline dts_modify_9_P: Check the handling of the -localtimeout option.

    # Run the test
    test {
	dts modify -localtimeout +0-00:00:10.000I-----
	list [dcp_get_element [dts show] localtimeout]
    } compare_strings {+0-00:00:10.000I-----}

    # Cleanup steps
}

proc dts_modify_12_P {} {
    # Identify the test
    tet_infoline dts_modify_12_P: Check the handling of the -maxinaccuracy option.

    # Setup steps

    # Run the test
    test {
	dts modify -maxinaccuracy +0-00:00:00.200I-----
	list [dcp_get_element [dts show] maxinaccuracy]
    } compare_strings {+0-00:00:00.200I-----}

    # Cleanup steps
}

proc dts_modify_15_P {} {
    # Identify the test
    tet_infoline dts_modify_15_P: Check the handling of the -minservers option.

    # Setup steps

    # Run the test
    test {
	dts modify -minservers 4
	list [dcp_get_element [dts show] minservers]
    } compare_strings {4}

    # Cleanup steps
}

proc dts_modify_18_P {} {
    # Identify the test
    tet_infoline dts_modify_18_P: Check the handling of the -queryattempts option.

    # Setup steps

    # Run the test
    test {
	dts modify -queryattempts 4
	list [dcp_get_element [dts show] queryattempts]
    } compare_strings {4}

    # Cleanup steps
}

proc dts_modify_21_P {} {
    # Identify the test
    tet_infoline dts_modify_21_P: Check the handling of the -syncinterval option.

    # Setup steps

    # Run the test
    test {
	dts modify -syncinterval +0-00:03:00.000I-----
	list [dcp_get_element [dts show] syncinterval]
    } compare_strings {+0-00:03:00.000I-----}

    # Cleanup steps
}

proc dts_modify_24_P {} {
    # Identify the test
    tet_infoline dts_modify_24_P: Check the handling of the -tolerance option.

    # Setup steps

    # Run the test
    test {
	dts modify -tolerance +0-00:15:00.000I-----
	list [dcp_get_element [dts show] tolerance]
    } compare_strings {+0-00:15:00.000I-----}

    # Cleanup steps
}

proc dts_modify_31_P {} {
    # Identify the test
    tet_infoline dts_modify_31_P: Check the handling of the -checkinterval option.

    # Setup steps

    # Run the test
    test {
	dts modify -checkinterval +0-01:35:00.000I-----
	list [dcp_get_element [dts show] checkinterval]
    } compare_strings {+0-01:35:00.000I-----}

    # Cleanup steps
}

proc dts_modify_34_P {} {
    # Identify the test
    tet_infoline dts_modify_34_P: Check the handling of the -courierrole option.

    # Setup steps

    # Run the test
    test {
	dts modify -courierrole backup
	list [dcp_get_element [dts show] courierrole]
    } compare_strings {backup}

    # Cleanup steps
}

proc dts_modify_35_P {} {
    # Identify the test
    tet_infoline dts_modify_35_P: Check the handling of the -courierrole option.

    # Setup steps

    # Run the test
    test {
	dts modify -courierrole courier
	list [dcp_get_element [dts show] courierrole]
    } compare_strings {courier}

    # Cleanup steps
}

proc dts_modify_38_P {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline dts_modify_38_P: Check the handling of the -serverentry option.

    # Setup steps

    # Run the test
    test {
	dts modify -serverentry hosts/$DCP_HOSTNAME_ONE/dts-new
	list [dcp_get_element [dts show] serverentry]
    } compare_strings "hosts/$DCP_HOSTNAME_ONE/dts-new"

    # Cleanup steps
    if {[dcp_wrap {dts modify -serverentry hosts/$DCP_HOSTNAME_ONE/dts-entity
                   object delete /.:/hosts/$DCP_HOSTNAME_ONE/dts-new}] != 0} {return;}
}

proc dts_modify_40_P {} {
    # Identify the test
    tet_infoline dts_modify_40_P: Check the handling of the -servergroup option.

    # Setup steps
    # Must have a local dtsd running with the value of the servergroup
    # attribute set to {subsys/dce/dts-servers}.

    # Run the test
    test {
	dts modify -servergroup none
	list [dcp_get_element [dts show] servergroup]
    } compare_strings {none}

    # Cleanup steps
    if {[dcp_wrap {dts modify -servergroup subsys/dce/dts-servers}] != 0} {return;}
}

proc dts_modify_48_P {} {
    # Identify the test
    tet_infoline dts_modify_48_P: Check that signle pair list formats are handled correctly.

    # Setup steps
        # Must have a local dtsd running with the value of the queryattempts
        # attribute set to '3'.

    # Run the test
    test {
	dts modify -change {queryattempts 4}
	list [dcp_get_element [dts show] queryattempts]
    } compare_strings {4}

    # Cleanup steps
}

proc dts_modify_49_P {} {
    # Identify the test
    tet_infoline dts_modify_49_P: Check the handling of -change switch.

    # Setup steps
    if {[dcp_wrap {dts modify -servergroup subsys/dce/dts-servers}] != 0} {return;}
    if {[dcp_wrap {dts modify -checkinterval +0-01:30:00.000I-----}] != 0} {return;}
    if {[dcp_wrap {dts modify -queryattempts 3}] != 0} {return;}

    # Run the test
    test {
	dts modify -change {{servergroup none} {checkinterval +0-01:35:00.000I-----} {queryattempts 4}}
	list [dcp_get_element [dts show] servergroup] [dcp_get_element [dts show] checkinterval] [dcp_get_element [dts show] queryattempts]
    } compare_strings {none +0-01:35:00.000I----- 4}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14"
set ic1 "dts_modify_6_P"
set ic2 "dts_modify_9_P"
set ic3 "dts_modify_12_P"
set ic4 "dts_modify_15_P"
set ic5 "dts_modify_18_P"
set ic6 "dts_modify_21_P"
set ic7 "dts_modify_24_P"
set ic8 "dts_modify_31_P"
set ic9 "dts_modify_34_P"
set ic10 "dts_modify_35_P"
set ic11 "dts_modify_38_P"
set ic12 "dts_modify_40_P"
set ic13 "dts_modify_48_P"
set ic14 "dts_modify_49_P"
set tet_startup dts_modify_startup
set tet_cleanup dts_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
