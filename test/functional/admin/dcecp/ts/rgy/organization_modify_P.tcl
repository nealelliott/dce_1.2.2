#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:54  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:08  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:01  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:52  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:49  melman]
# 
# Revision 1.1.2.7  1994/09/21  17:11:12  kevins
# 	OT 9938 Modified pgo functional tests
# 	[1994/09/21  17:10:51  kevins]
# 
# Revision 1.1.2.6  1994/08/30  18:28:56  kevins
# 	OT 10617 modified messages/output for functional tests
# 	[1994/08/30  18:28:29  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:21  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:36  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:19  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:59  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:39  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:48  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:10  melman
# 	updating to new API path
# 	[1994/05/06  17:14:46  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:14  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:38  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc org_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc org_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc organization_modify_34_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_34_P: Check to see that we can modify a organization

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create $name}] != 0} {return;}

    # Run the test
    test { organization modify $name -fullname "kevin sullivan"
	dcp_attrlist getval [organization show $name] fullname
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}

proc organization_modify_35_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_35_P: Check to see that we can modify a organization with pwdminlen

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create $name}] != 0} {return;}


    # Run the test
    test { organization modify $name -pwdminlen 6
          dcp_attrlist getval [organization show $name -policies] pwdminlen
         } compare_strings 6

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}

proc organization_modify_36_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_36_P: Check to see that we can modify a organization with pwdminlen and
    tet_infoline organization_modify_36_P: pwdalpha acclife pwdlife pwdspaces

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create $name}] != 0} {return;}

    # Run the test
     test { organization modify $name -pwdminlen 10 -pwdalpha no \
                   -pwdlife 10 -pwdspaces no -acctlife 10
           dcp_multi_values [organization show $name -policies] {{pwdminlen 10} \
                            {pwdalpha no} {pwdlife +0-00:00:10.000I-----} \
                            {pwdspaces no} {acctlife +0-00:00:10.000I-----}}
           } compare_strings found

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}

proc organization_modify_37_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_37_P: Check to see that we can modify a organization with pwdminlen and
    tet_infoline organization_modify_37_P: pwdalpha acclife pwdlife pwdspaces

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create $name}] != 0} {return;}

    # Run the test
    test { organization modify $name -change {{pwdminlen 10} {pwdalpha no} \
                                {pwdlife 10} {pwdspaces no} {acctlife 10}}
           dcp_multi_values [organization show $name -policies] {{pwdminlen 10}\
                              {pwdalpha no} {pwdlife +0-00:00:10.000I-----} \
                              {pwdspaces no} {acctlife +0-00:00:10.000I-----}}
           } compare_strings found

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}

proc organization_modify_38_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_38_P: Check to see that we can modify a organization

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create $name}] != 0} {return;}

    # Run the test
    test { organization modify $name -change {fullname "kevin sullivan"}
	dcp_attrlist getval [organization show $name] fullname 
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # Organization needs to be deleted.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}

proc organization_modify_39_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_39_P: Check to see that we can modify an alias on a organization 
    tet_infoline organization_modify_39_P: from a option 

    # Setup steps
     if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                    organization create $name}] != 0} {return;}

    # Run the test
    test { organization modify $name -fullname "kevin sullivan"
	dcp_attrlist getval [organization show $name] fullname 
         } compare_strings {{kevin sullivan}}

     # Cleanup steps
        # The organization $name needs to be removed from the registry.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}


proc organization_modify_40_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_40_P: Check to see that we can modify the fullname on a organization 
    tet_infoline organization_modify_40_P: from an attribute list.

    # Setup steps
     if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                    organization create $name}] != 0} {return;}

    # Run the test
    test { organization modify $name -change {{fullname "kevin sullivan"} \
                               {pwdalpha no} {pwdminlen 10} {pwdlife 10}\
                               {acctlife 10} {pwdspaces no}}
           dcp_multi_values [organization show $name -policies] {{pwdminlen 10}\
                                {pwdalpha no} {pwdlife +0-00:00:10.000I-----}\
                                {pwdspaces no} {acctlife +0-00:00:10.000I-----}}
           } compare_strings found

    # Cleanup steps
        # The organization $name needs to be removed from the registry.
    if {[dcp_wrap {organization delete $name}] != 0} {return;}
}

proc organization_modify_41_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_41_P: Check to see that we can modify the fullname on a list of 
    tet_infoline organization_modify_41_P: organizations from an attribute list.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create "$name bar_$name"}] != 0} {return;}


    # Run the test
    test { organization modify "$name bar_$name" -change \
                   {fullname "kevin sullivan"}
       dcp_attrlist getval [organization show "$name bar_$name"] fullname 
         } compare_strings {{kevin sullivan}}

        # Expected results are:
        # Successful completion

    # Cleanup steps
    if {[dcp_wrap {organization delete "$name bar_$name"}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8"
set ic1 "organization_modify_34_P"
set ic2 "organization_modify_35_P"
set ic3 "organization_modify_36_P"
set ic4 "organization_modify_37_P"
set ic5 "organization_modify_38_P"
set ic6 "organization_modify_39_P"
set ic7 "organization_modify_40_P"
set ic8 "organization_modify_41_P"

set tet_startup org_startup
set tet_cleanup org_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
