#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:59  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/15  18:20 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts29/1  1995/05/15  18:20 UTC  truitt
# 	CHFts15220: Additional tests.
# 	[1995/12/11  15:55:07  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:02  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:56  melman]
# 
# Revision 1.1.2.6  1994/08/30  18:28:59  kevins
# 	OT 10617 modified messages/output for functional tests
# 	[1994/08/30  18:28:34  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:27  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:42  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:25  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:06  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:49  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:18  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:17  melman
# 	updating to new API path
# 	[1994/05/06  17:14:53  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:21  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:45  kevins]
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
proc organization_show_1_P {} {
    # Identify the test
    tet_infoline organization_show_1_P: Check that we can show the organization attributes.

    # Setup steps
    if {[dcp_wrap {set org_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test {
        dcp_found [organization show $org_name] orgid
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}

}

proc organization_show_2_P {} {
    # Identify the test
    tet_infoline organization_show_2_P: Check that we can show a list of org attributes.

    # Setup steps
    if {[dcp_wrap {set org_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {organization create "$org_name bar_$org_name"}] != 0} {return;}

    # Run the test
    test {
        dcp_found [organization show "$org_name bar_$org_name"] orgid
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {organization delete "$org_name bar_$org_name"}] != 0} {return;}
}

proc organization_show_3_P {} {
    # Identify the test
    tet_infoline organization_show_3_P: Check that we can show the organization policies.

    # Setup steps
    if {[dcp_wrap {set org_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {organization create $org_name -pwdlife unlimited}] != 0} {return;}

    # Run the test
    test {
        attrlist getvalues [organization show $org_name -policies] -type pwdlife
    } compare_strings unlimited

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_show_4_P {} {
    # Identify the test
    tet_infoline organization_show_4_P: Check that we can show all org attributes

    # Setup steps
    if {[dcp_wrap {set org_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {organization create $org_name -pwdlife unlimited}] != 0} {return;}

    # Run the test
    test {
        dcp_found [organization show $org_name -all] {orgid pwdlife}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_show_5_P {} {
    # Identify the test
    tet_infoline organization_show_5_P: Check the pwdalpha policy field
    tet_infoline organization_show_5_P: org - no, rgy - yes

    # Setup steps
    if {[dcp_wrap {organization create org_show_5_P -pwdalpha no}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_5_P -all] pwdalpha
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_5_P}] != 0} {return;}
}

proc organization_show_6_P {} {
    # Identify the test
    tet_infoline organization_show_6_P: Check the pwdalpha policy field
    tet_infoline organization_show_6_P: org - yes, rgy - yes

    # Setup steps
    if {[dcp_wrap {organization create org_show_6_P -pwdalpha yes}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_6_P -all] pwdalpha
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_6_P}] != 0} {return;}
}

proc organization_show_7_P {} {
    # Identify the test
    tet_infoline organization_show_7_P: Check the pwdalpha policy field
    tet_infoline organization_show_7_P: org - no, rgy - no

    # Setup steps
    if {[dcp_wrap {organization create org_show_7_P -pwdalpha no}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdalpha no}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_7_P -all] pwdalpha
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_7_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdalpha yes}] != 0} {return;}
}

proc organization_show_8_P {} {
    # Identify the test
    tet_infoline organization_show_8_P: Check the pwdalpha policy field
    tet_infoline organization_show_8_P: org - yes, rgy - no

    # Setup steps
    if {[dcp_wrap {organization create org_show_8_P -pwdalpha yes}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdalpha no}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_8_P -all] pwdalpha
    } compare_strings {yes effective no}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_8_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdalpha yes}] != 0} {return;}
}

proc organization_show_9_P {} {
    # Identify the test
    tet_infoline organization_show_9_P: Check the pwdspaces policy field
    tet_infoline organization_show_9_P: org - no, rgy - yes

    # Setup steps
    if {[dcp_wrap {organization create org_show_9_P -pwdspaces no}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_9_P -all] pwdspaces
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_9_P}] != 0} {return;}
}

proc organization_show_10_P {} {
    # Identify the test
    tet_infoline organization_show_10_P: Check the pwdspaces policy field
    tet_infoline organization_show_10_P: org - yes, rgy - yes

    # Setup steps
    if {[dcp_wrap {organization create org_show_10_P -pwdspaces yes}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_10_P -all] pwdspaces
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_10_P}] != 0} {return;}
}

proc organization_show_11_P {} {
    # Identify the test
    tet_infoline organization_show_11_P: Check the pwdspaces policy field
    tet_infoline organization_show_11_P: org - no, rgy - no

    # Setup steps
    if {[dcp_wrap {organization create org_show_11_P -pwdspaces no}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdspaces no}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_11_P -all] pwdspaces
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_11_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdspaces yes}] != 0} {return;}
}

proc organization_show_12_P {} {
    # Identify the test
    tet_infoline organization_show_12_P: Check the pwdspaces policy field
    tet_infoline organization_show_12_P: org - yes, rgy - no

    # Setup steps
    if {[dcp_wrap {organization create org_show_12_P -pwdspaces yes}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdspaces no}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_12_P -all] pwdspaces
    } compare_strings {yes effective no}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_12_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdspaces yes}] != 0} {return;}
}

proc organization_show_13_P {} {
    # Identify the test
    tet_infoline organization_show_13_P: Check the pwdminlen policy field
    tet_infoline organization_show_13_P: org - 0, rgy - 0

    # Setup steps
    if {[dcp_wrap {organization create org_show_13_P -pwdminlen 0}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_13_P -all] pwdminlen
    } compare_strings 0

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_13_P}] != 0} {return;}
}

proc organization_show_14_P {} {
    # Identify the test
    tet_infoline organization_show_14_P: Check the pwdminlen policy field
    tet_infoline organization_show_14_P: org - 3, rgy - 0

    # Setup steps
    if {[dcp_wrap {organization create org_show_14_P -pwdminlen 3}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_14_P -all] pwdminlen
    } compare_strings 3

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_14_P}] != 0} {return;}
}

proc organization_show_15_P {} {
    # Identify the test
    tet_infoline organization_show_15_P: Check the pwdminlen policy field
    tet_infoline organization_show_15_P: org - 5, rgy - 3

    # Setup steps
    if {[dcp_wrap {organization create org_show_15_P -pwdminlen 5}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdminlen 3}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_15_P -all] pwdminlen
    } compare_strings 5

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_15_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdminlen 0}] != 0} {return;}
}

proc organization_show_16_P {} {
    # Identify the test
    tet_infoline organization_show_16_P: Check the pwdminlen policy field
    tet_infoline organization_show_16_P: org - 0, rgy - 3

    # Setup steps
    if {[dcp_wrap {organization create org_show_16_P -pwdminlen 0}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdminlen 3}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_16_P -all] pwdminlen
    } compare_strings {0 effective 3}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_16_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdminlen 0}] != 0} {return;}
}

proc organization_show_17_P {} {
    # Identify the test
    tet_infoline organization_show_17_P: Check the pwdexpdate policy field
    tet_infoline organization_show_17_P: org - none, rgy - none

    # Setup steps
    if {[dcp_wrap {organization create org_show_17_P -pwdexpdate 0}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_17_P -all] pwdexpdate
    } compare_strings none

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_17_P}] != 0} {return;}
}

proc organization_show_18_P {} {
    # Identify the test
    tet_infoline organization_show_18_P: Check the pwdexpdate policy field
    tet_infoline organization_show_18_P: org - 1999-01-02-12:13:14.000-05:00, rgy - none

    # Setup steps
    if {[dcp_wrap {organization create org_show_18_P -pwdexpdate 1999-01-02-12:13:14.000-05:00}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_18_P -all] pwdexpdate
    } compare_strings 1999-01-02-12:13:14.000-05:00I-----

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_18_P}] != 0} {return;}
}

proc organization_show_19_P {} {
    # Identify the test
    tet_infoline organization_show_19_P: Check the pwdexpdate policy field
    tet_infoline organization_show_19_P: org - none, rgy - 1999-01-02-12:13:14.000-05:00

    # Setup steps
    if {[dcp_wrap {organization create org_show_19_P -pwdexpdate none}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdexpdate 1999-01-02-12:13:14.000-05:00}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_19_P -all] pwdexpdate
    } compare_strings {none effective 1999-01-02-12:13:14.000-05:00I-----}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_19_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdexpdate none}] != 0} {return;}
}

proc organization_show_20_P {} {
    # Identify the test
    tet_infoline organization_show_20_P: Check the pwdexpdate policy field
    tet_infoline organization_show_20_P: org - 1999-01-03-00:02:03.000-05:00, rgy - 1999-01-02-12:13:14.000-05:00

    # Setup steps
    if {[dcp_wrap {organization create org_show_20_P -pwdexpdate 1999-01-03-00:02:03.000-05:00}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdexpdate 1999-01-02-12:13:14.000-05:00}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_20_P -all] pwdexpdate
    } compare_strings {1999-01-03-00:02:03.000-05:00I----- effective 1999-01-02-12:13:14.000-05:00I-----}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_20_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdexpdate none}] != 0} {return;}
}

proc organization_show_21_P {} {
    # Identify the test
    tet_infoline organization_show_21_P: Check the pwdlife policy field
    tet_infoline organization_show_21_P: org - unlimited, rgy - unlimited

    # Setup steps
    if {[dcp_wrap {organization create org_show_21_P -pwdlife unlimited}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_21_P -all] pwdlife
    } compare_strings unlimited

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_21_P}] != 0} {return;}
}

proc organization_show_22_P {} {
    # Identify the test
    tet_infoline organization_show_22_P: Check the pwdlife policy field
    tet_infoline organization_show_22_P: org - +7, rgy - unlimited

    # Setup steps
    if {[dcp_wrap {organization create org_show_22_P -pwdlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_22_P -all] pwdlife
    } compare_strings +7-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_22_P}] != 0} {return;}
}

proc organization_show_23_P {} {
    # Identify the test
    tet_infoline organization_show_23_P: Check the pwdlife policy field
    tet_infoline organization_show_23_P: org - unlimited, rgy - +7

    # Setup steps
    if {[dcp_wrap {organization create org_show_23_P -pwdlife unlimited}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_23_P -all] pwdlife
    } compare_strings {unlimited effective +7-00:00:00.000I-----}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_23_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdlife unlimited}] != 0} {return;}
}

proc organization_show_24_P {} {
    # Identify the test
    tet_infoline organization_show_24_P: Check the pwdlife policy field
    tet_infoline organization_show_24_P: org - +6, rgy - +7

    # Setup steps
    if {[dcp_wrap {organization create org_show_24_P -pwdlife +6-00:00:00.000}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_24_P -all] pwdlife
    } compare_strings +6-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_24_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdlife unlimited}] != 0} {return;}
}

proc organization_show_25_P {} {
    # Identify the test
    tet_infoline organization_show_25_P: Check the pwdlife policy field
    tet_infoline organization_show_25_P: org - +8, rgy - +7

    # Setup steps
    if {[dcp_wrap {organization create org_show_25_P -pwdlife +8-00:00:00.000}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_25_P -all] pwdlife
    } compare_strings {+8-00:00:00.000I----- effective +7-00:00:00.000I-----}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_25_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdlife unlimited}] != 0} {return;}
}

proc organization_show_26_P {} {
    # Identify the test
    tet_infoline organization_show_26_P: Check the acctlife policy field
    tet_infoline organization_show_26_P: org - unlimited, rgy - unlimited

    # Setup steps
    if {[dcp_wrap {organization create org_show_26_P -acctlife unlimited}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_26_P -all] acctlife
    } compare_strings unlimited

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_26_P}] != 0} {return;}
}

proc organization_show_27_P {} {
    # Identify the test
    tet_infoline organization_show_27_P: Check the acctlife policy field
    tet_infoline organization_show_27_P: org - +7, rgy - unlimited

    # Setup steps
    if {[dcp_wrap {organization create org_show_27_P -acctlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_27_P -all] acctlife
    } compare_strings +7-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_27_P}] != 0} {return;}
}

proc organization_show_28_P {} {
    # Identify the test
    tet_infoline organization_show_28_P: Check the acctlife policy field
    tet_infoline organization_show_28_P: org - unlimited, rgy - +7

    # Setup steps
    if {[dcp_wrap {organization create org_show_28_P -acctlife unlimited}] != 0} {return;}
    if {[dcp_wrap {registry modify -acctlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_28_P -all] acctlife
    } compare_strings {unlimited effective +7-00:00:00.000I-----}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_28_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -acctlife unlimited}] != 0} {return;}
}

proc organization_show_29_P {} {
    # Identify the test
    tet_infoline organization_show_29_P: Check the acctlife policy field
    tet_infoline organization_show_29_P: org - +6, rgy - +7

    # Setup steps
    if {[dcp_wrap {organization create org_show_29_P -acctlife +6-00:00:00.000}] != 0} {return;}
    if {[dcp_wrap {registry modify -acctlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_29_P -all] acctlife
    } compare_strings +6-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_29_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -acctlife unlimited}] != 0} {return;}
}

proc organization_show_30_P {} {
    # Identify the test
    tet_infoline organization_show_30_P: Check the acctlife policy field
    tet_infoline organization_show_30_P: org - +8, rgy - +7

    # Setup steps
    if {[dcp_wrap {organization create org_show_30_P -acctlife +8-00:00:00.000}] != 0} {return;}
    if {[dcp_wrap {registry modify -acctlife +7-00:00:00.000}] != 0} {return;}

    # Run the test
    test {
        dcp_attrlist getval [organization show org_show_30_P -all] acctlife
    } compare_strings {+8-00:00:00.000I----- effective +7-00:00:00.000I-----}

    # Cleanup steps
    if {[dcp_wrap {organization delete org_show_30_P}] != 0} {return;}
    if {[dcp_wrap {registry modify -acctlife unlimited}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 "
set ic1 "organization_show_1_P"
set ic2 "organization_show_2_P"
set ic3 "organization_show_3_P"
set ic4 "organization_show_4_P"
set ic5 "organization_show_5_P"
set ic6 "organization_show_6_P"
set ic7 "organization_show_7_P"
set ic8 "organization_show_8_P"
set ic9 "organization_show_9_P"
set ic10 "organization_show_10_P"
set ic11 "organization_show_11_P"
set ic12 "organization_show_12_P"
set ic13 "organization_show_13_P"
set ic14 "organization_show_14_P"
set ic15 "organization_show_15_P"
set ic16 "organization_show_16_P"
set ic17 "organization_show_17_P"
set ic18 "organization_show_18_P"
set ic19 "organization_show_19_P"
set ic20 "organization_show_20_P"
set ic21 "organization_show_21_P"
set ic22 "organization_show_22_P"
set ic23 "organization_show_23_P"
set ic24 "organization_show_24_P"
set ic25 "organization_show_25_P"
set ic26 "organization_show_26_P"
set ic27 "organization_show_27_P"
set ic28 "organization_show_28_P"
set ic29 "organization_show_29_P"
set ic30 "organization_show_30_P"

set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
