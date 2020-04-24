#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:46  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:56  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:45  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:43  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:15  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:30  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:14  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:42  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:31  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:24  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:03  melman
# 	updating to new API path
# 	[1994/05/06  17:14:38  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:06  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:32  kevins]
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
proc organization_create_32_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_32_P: Check to see that we can create a organization

    # Setup steps
    set org_name [lindex [info level [info level]] 0]
    # Run the test
    test { organization create $org_name -orgid 3000
         dcp_attrlist getval [organization show $org_name] orgid
         } compare_strings 3000
         
    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_33_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_33_P: Check to see that we can create a organization

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create $org_name -fullname "kevin sullivan"
         dcp_attrlist getval [organization show $org_name] fullname
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_34_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_34_P: Check to see that we can create a organization

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create $org_name -attribute {orgid 1231}
             dcp_attrlist getval [organization show $org_name] orgid
         } compare_strings 1231

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_35_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_35_P: Check to see that we can create a organization

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}
    if {[dcp_wrap {set uuid_value [attrlist getvalues \
                [organization show $org_name] -type uuid]}] != 0 } {return;}
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}


    # Run the test
     test { organization create $org_name -uuid $uuid_value
           if { [lsearch [organization catalog -simplename] $org_name] != -1 } {
           list found} else { list not_found }
         } compare_strings found

        # expected results are:
        # Successful completion.

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_36_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_36_P: Check to see that we can create a organization

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create $org_name  
         organization catalog -simplename } dcp_find_pgo $org_name

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_37_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_37_P: Check to see that we can create a organization with pwdminlen

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create $org_name -pwdminlen 3 
           dcp_multi_values [organization show $org_name -policies] \
            {{pwdminlen 3}}
         } compare_strings found

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_38_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_38_P: Check to see that we can create a organization with pwdminlen and
    tet_infoline organization_create_38_P: pwdalpha acclife pwdlife pwdspaces

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create $org_name -pwdminlen 10 -pwdalpha no -pwdlife 10\
            -pwdspaces no -acctlife 10
           dcp_multi_values [organization show $org_name -policies] \
             {{pwdminlen 10} {pwdalpha no} {pwdlife +0-00:00:10.000I-----} \
              {pwdspaces no} {acctlife +0-00:00:10.000I-----}} 
         } compare_strings found

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_39_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_39_P: Check to see that we can create a organization with pwdminlen and
    tet_infoline organization_create_39_P: pwdalpha acclife pwdlife pwdspaces

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create $org_name -attribute {{pwdminlen 10} \
               {pwdalpha no} {pwdlife 10} {pwdspaces no} {acctlife 10}}
           dcp_multi_values [organization show $org_name -policies] {{pwdalpha no}
              {pwdminlen 10} {pwdlife +0-00:00:10.000I-----} 
              {pwdspaces no} 
              {acctlife +0-00:00:10.000I-----}}} compare_strings found

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_40_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_40_P: Check to see that we can create a organization

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]


    # Run the test
    test { organization create $org_name -attribute {fullname "kevin sullivan"}
         dcp_attrlist getval [organization show $org_name] fullname 
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_create_41_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_create_41_P: Check to see that we can create a organization

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]

    # Run the test
    test { organization create "$org_name 1_$org_name" -attribute {fullname "kevin sullivan"}
         dcp_attrlist getval [organization show $org_name] fullname 
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete "$org_name 1_$org_name"}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10"
set ic1 "organization_create_32_P"
set ic2 "organization_create_33_P"
set ic3 "organization_create_34_P"
set ic4 "organization_create_35_P"
set ic5 "organization_create_36_P"
set ic6 "organization_create_37_P"
set ic7 "organization_create_38_P"
set ic8 "organization_create_39_P"
set ic9 "organization_create_40_P"
set ic10 "organization_create_41_P"

set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
