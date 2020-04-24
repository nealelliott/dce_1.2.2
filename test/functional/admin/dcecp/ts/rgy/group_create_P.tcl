#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:09  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:22  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:48  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:58  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:03:58  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:05  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:34  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:20  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:02  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:09  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:45  melman
# 	updating to new API path
# 	[1994/05/06  17:14:14  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:45  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:10  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc group_create_21_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_21_P: Check to see that we can create a group

    # Setup steps

    # Run the test
    test { group create foo -gid 3000 
           dcp_attrlist getval [group show foo] gid } compare_strings 3000

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_create_22_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_22_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 

    # Run the test
    test { group create foo -fullname "kevin sullivan" 
	   dcp_attrlist getval [group show foo] fullname 
    } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete foo}] != 0} {return;}

}

proc group_create_23_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_23_P: Check to see that we can create a group

    # Setup steps

    # Run the test
    test { group create foo -attribute {gid 1231}
           dcp_attrlist getval [group show foo] gid } compare_strings 1231

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_create_24_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_24_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 
    set gr_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {group create $gr_name}] != 0} {return;}
    if {[dcp_wrap {set uuid_value [attrlist getvalues [group show $gr_name] \
                -type uuid]}] != 0 } {return;}
    if {[dcp_wrap {group delete $gr_name}] != 0} {return;}

    # Run the test
    test { group create $gr_name -uuid $uuid_value
           if { [lsearch [group catalog -simplename] $gr_name] != -1 } {
           set a "found"} else { set a "not_found" }
         } compare_strings found

        # Successful completion.

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete $gr_name}] != 0} {return;}
}

proc group_create_25_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_25_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 
    set gr_name [lindex [info level [info level]] 0]

    # Run the test
    test { group create $gr_name
           if { [lsearch [group catalog -simplename] $gr_name] != -1 } {
           set a "found" } else { set a "not_found" } } compare_strings found

        # Expected results are:
        # Successful completion.

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete $gr_name}] != 0} {return;}

}

proc group_create_26_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_26_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 


    # Run the test
     test { group create foo -attribute {fullname "kevin sullivan"}
          dcp_attrlist getval [group show foo] fullname 
          } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_create_27_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_27_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 
    set gr_name [lindex [info level [info level]] 0]


    # Run the test
    test { group create {$gr_name foo_$gr_name} 
         dcp_found [group catalog -simplename] {$gr_name foo_$gr_name}
       } compare_strings found

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete {$gr_name foo_$gr_name}}] != 0} {return;}
}

proc group_create_28_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_28_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 

    # Run the test
    test { group create foo -attribute {{alias yes} {gid 100}}
           dcp_multi_values [group show foo] {{alias yes} {gid 100}} 
          } compare_strings found

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete foo}] != 0} {return;}

}

proc group_create_29_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_create_29_P: Check to see that we can create a group

    # Setup steps
        # Must be running as cell administrator. 

    # Run the test
    test { group create foo -alias yes -gid 100
           dcp_multi_values [group show foo] {{alias yes} {gid 100}} 
          } compare_strings found

    # Cleanup steps
        # Group needs to be deleted.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "group_create_21_P"
set ic2 "group_create_22_P"
set ic3 "group_create_23_P"
set ic4 "group_create_24_P"
set ic5 "group_create_25_P"
set ic6 "group_create_26_P"
set ic7 "group_create_27_P"
set ic8 "group_create_28_P"
set ic9 "group_create_29_P"

set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
