#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:04  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:10  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:08  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:01  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:31  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:47  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:50  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:11  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:53  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:34  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:22  melman
# 	updating to new API path
# 	[1994/05/06  17:14:59  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:28  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:50  kevins]
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
proc principal_create_14_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_14_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {principal create foo -quota 10
     dcp_attrlist getval [principal show foo] quota } compare_strings 10 

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc principal_create_15_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_15_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {principal create foo -quota unlimited
	  dcp_attrlist getval [principal show foo] quota } compare_strings unlimited
	 

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc principal_create_16_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_16_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { principal create foo -uid 1231
	  dcp_attrlist getval [principal show foo] uid } compare_strings 1231

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_create_17_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_17_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { principal create foo -fullname "kevin sullivan"
          dcp_attrlist getval [principal show foo] fullname
          } compare_strings {{kevin sullivan}}

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc principal_create_18_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_18_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
     test {principal create foo -attribute {uid 1231}
	  dcp_attrlist getval [principal show foo] uid } compare_strings 1231

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc principal_create_19_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_19_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    set pr_name [lindex [info level [info level]] 0]
    # Run the test
        test {principal create $pr_name 
	      if {[lsearch [principal catalog -simplename] $pr_name] != -1 } {
              set a "found" } else {set a "not_found"} } compare_strings found
	      

    # Cleanup steps
    if {[dcp_wrap {principal delete $pr_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc principal_create_20_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_20_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {principal create foo -attribute {fullname "kevin sullivan"}
	  dcp_attrlist getval [principal show foo] fullname
    } compare_strings {{kevin sullivan}}

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_create_21_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_21_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { principal create {foo foo1} -attribute {fullname "kevin sullivan"}
           dcp_attrlist getval [principal show {foo foo1}] fullname 
    } compare_strings {{kevin sullivan}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foo foo1}}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_create_22_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_22_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { principal create foo -attribute {{alias yes} {uid 100}}
	   dcp_multi_values [principal show foo] {{alias yes} {uid 100}}
    } compare_strings found

    # Cleanup steps
     if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc principal_create_23_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_create_23_P: Check to see that we can create a principal

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { principal create foo -alias yes -uid 100 
	   dcp_multi_values [principal show foo] {{alias yes} {uid 100}}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {principal delete foo}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10"
set ic1 "principal_create_14_P"
set ic2 "principal_create_15_P"
set ic3 "principal_create_16_P"
set ic4 "principal_create_17_P"
set ic5 "principal_create_18_P"
set ic6 "principal_create_19_P"
set ic7 "principal_create_20_P"
set ic8 "principal_create_21_P"
set ic9 "principal_create_22_P"
set ic10 "principal_create_23_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
