#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:11  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:16  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:08  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:34  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:51  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:54  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:16  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:56  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:49  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:25  melman
# 	updating to new API path
# 	[1994/05/06  17:15:04  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:33  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:54  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc principal_modify_13_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_13_P: Check to see that we can modify an alias on a principal 
    tet_infoline principal_modify_13_P: from a option 

    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
    test { principal modify foo -alias yes 
	   dcp_attrlist getval [principal show foo] alias } compare_strings yes

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_14_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_14_P: Check to see that we can modify an alias from an attribute list 

    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
    test { principal modify foo -change {alias yes} 
         dcp_attrlist getval [principal show foo] alias } compare_strings yes

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_15_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_15_P: Check to see that we can modify the fullname on a principal 
    tet_infoline principal_modify_15_P: from a option

    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
    test { principal modify foo -fullname "kevin sullivan" 
        dcp_attrlist getval [principal show foo] fullname
     } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_16_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_16_P: Check to see that we can modify the fullname on a principal 

    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
     test { principal modify foo -change {fullname "kevin sullivan"}
        dcp_attrlist getval [principal show foo] fullname
        } compare_strings {{kevin sullivan}}



    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_17_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_17_P: Check to see that we can modify the quota on a principal from a option


    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
    test { principal modify foo -quota unlimited
        dcp_attrlist getval [principal show foo] quota
    } compare_strings unlimited	

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_18_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_18_P: Check to see that we can modify the quota on a principal from an attribute list.


    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
    test { principal modify foo -change {quota unlimited}
        dcp_attrlist getval [principal show foo] quota
    } compare_strings unlimited	

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_19_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_19_P: Check to see that we can modify the quota on a list of principals from an attribute list.

    # Setup steps
        # Must be running as cell administrator. 
        # The principals foo and foo1 must exist.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create {foo foo1}}] != 0} {return;}

    # Run the test
    test { principal modify {foo foo1} -change {quota unlimited} 
        dcp_attrlist getval [principal show foo] quota
    } compare_strings unlimited	

    # Cleanup steps
        # The principals foo and foo1 needs to be removed from the registry.
    if {[dcp_wrap {principal delete {foo foo1}}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "principal_modify_13_P"
set ic2 "principal_modify_14_P"
set ic3 "principal_modify_15_P"
set ic4 "principal_modify_16_P"
set ic5 "principal_modify_17_P"
set ic6 "principal_modify_18_P"
set ic7 "principal_modify_19_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
