#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_modify_P.tcl,v $
# Revision 1.1.5.2  1996/03/11  02:23:01  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:39  marty]
#
# Revision 1.1.5.1  1995/12/11  15:12:58  root
# 	Submit
# 	[1995/12/11  14:30:53  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:34  melman]
# 
# Revision 1.1.2.6  1994/09/27  19:24:11  kevins
# 	OT 12361 Modified pwdexpdate value in pos test.
# 	[1994/09/27  19:23:18  kevins]
# 
# Revision 1.1.2.5  1994/08/26  12:43:30  kevins
# 	OT 11598 added tests for registry -policies ops
# 	[1994/08/26  11:06:22  kevins]
# 
# Revision 1.1.2.4  1994/06/29  21:39:28  salamone
# 	Add -key option support.
# 	[1994/06/29  20:11:10  salamone]
# 
# Revision 1.1.2.3  1994/06/10  20:16:00  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:22  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  19:40:06  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:13  rousseau]
# 
# Revision 1.1.2.1  1994/05/11  13:48:04  kevins
# 	Initial submission
# 	[1994/05/11  13:43:52  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_modify_052_P {} {
    # Identify the test
    tet_infoline registry_modify_052_P: Check that the mintktlife can be set.

    # Run the test
    test {
        registry modify -mintktlife 15:0
        dcp_attrlist getval [registry show] mintktlife
    } compare_strings {+0-00:15:00.000I-----}

    # Cleanup steps
    if {[dcp_wrap {registry modify -mintktlife 5:0}] != 0} {return;}
}

proc registry_modify_053_P {} {
    # Identify the test
    tet_infoline registry_modify_053_P: Check that the deftktlife can be set.

    # Run the test
    test {
        registry modify -deftktlife 20:0:0
        dcp_attrlist getval [registry show] deftktlife
    } compare_strings {+0-20:00:00.000I-----}

    # Cleanup steps
    if {[dcp_wrap {registry modify -deftktlife 10:0:0}] != 0} {return;}
}

proc registry_modify_054_P {} {
    # Identify the test
    tet_infoline registry_modify_054_P: Check that the hidepwd policy can be set.

    # Run the test
    test {
        registry modify -hidepwd no
        dcp_attrlist getval [registry show] hidepwd
    } compare_strings {no}

    # Cleanup steps
    if {[dcp_wrap {registry modify -hidepwd yes}] != 0} {return;}
}

proc registry_modify_055_P {} {
    # Identify the test
    tet_infoline registry_modify_055_P: Check that the maxuid can be set.

    # Run the test
    test {
        registry modify -maxuid 32700
        dcp_attrlist getval [registry show] maxuid
    } compare_strings 32700

    # Cleanup steps
    if {[dcp_wrap {registry modify -maxuid 32767}] != 0} {return;}
}

proc registry_modify_056_P {} {
    # Identify the test
    tet_infoline registry_modify_056_P: Check that the mingid attribute can be set. 

    # Run the test
    test {
        registry modify -mingid 5000
        dcp_attrlist getval [registry show] mingid
    } compare_strings 5000

    # Cleanup steps
    if {[dcp_wrap {registry modify -mingid 5100}] != 0} {return;}
}

proc registry_modify_057_P {} {
    # Identify the test
    tet_infoline registry_modify_057_P: Check that the minorgid can be set.

    # Run the test
    test {
        registry modify -minorgid 1
        dcp_attrlist getval [registry show] minorgid
    } compare_strings 1

    # Cleanup steps
    if {[dcp_wrap {registry modify -minorgid 100}] != 0} {return;}
}

proc registry_modify_058_P {} {
    # Identify the test
    tet_infoline registry_modify_058_P: Check that the minuid can be set.

    # Run the test
    test {
        registry modify -minuid 10000
        dcp_attrlist getval [registry show] minuid
    } compare_strings 10000

    # Cleanup steps
    if {[dcp_wrap {registry modify -minuid 12427}] != 0} {return;}
}

proc registry_modify_059_P {} {
    # Identify the test
    tet_infoline registry_modify_059_P: Check that the -change works for an attribute list.

    # Run the test
    test {
        registry modify -change {{minuid 10000} {minorgid 1}}
        dcp_multi_values [registry show] {{minuid 10000} {minorgid 1}}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {registry modify -change {{minuid 12427} {minorgid 100}}}] != 0} {return;}
}

proc registry_modify_060_P {} {
    # Identify the test
    tet_infoline registry_modify_060_P: Check that the -change works for an attribute list.

    # Run the test
    test {
        registry modify -change {{maxuid 32000} {mingid 5000}}
        dcp_multi_values [registry show] {{maxuid 32000} {mingid 5000}}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {registry modify -change {{maxuid 32767} {mingid 5100}}}] != 0} {return;}
}

proc registry_modify_061_P {} {
    # Identify the test
    tet_infoline registry_modify_061_P: Check that the -change works for an attribute list.

    # Run the test
    test {
        registry modify -change {hidepwd no}
        dcp_attrlist getval [registry show] hidepwd
    } compare_strings {no}

    # Cleanup steps
    if {[dcp_wrap {registry modify -change {hidepwd yes}}] != 0} {return;}
}

proc registry_modify_062_P {} {
    # Identify the test
    tet_infoline registry_modify_062_P: Check that the -change works for an attribute list.

    test {
        registry modify -change {{deftktlife 20:0:0} {mintktlife 15:0}}
        dcp_multi_values [registry show] {{deftktlife +0-20:00:00.000I-----} {mintktlife +0-00:15:00.000I-----}}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {registry modify -change {{deftktlife 10:0:0} {mintktlife 5:0}}}] != 0} {return;}
}

proc registry_modify_063_P {} {
    # Identify the test
    tet_infoline registry_modify_063_P: Check that attribute and policy changes are not
    tet_infoline registry_modify_063_P: mutually exclusive.

    # Run the test
    test {
        registry modify -mingid 5000 -hidepwd no
        dcp_attrlist getval [registry show] hidepwd
    } compare_strings {no}

    test {
        dcp_attrlist getval [registry show] mingid
    } compare_strings {5000}

    # Cleanup steps
    if {[dcp_wrap {registry modify -mingid 5100}] != 0} {return;}
    if {[dcp_wrap {registry modify -hidepwd yes}] != 0} {return;}
}

proc registry_modify_064_P {} {
    # Identify the test
    tet_infoline registry_modify_064_P: Check that the argument can be specified as the local
    tet_infoline registry_modify_064_P: cell when changing attributes and/or policies.

    # Run the test
    test {
        registry modify /.: -mingid 5000 -hidepwd no
        dcp_attrlist getval [registry show] hidepwd
    } compare_strings {no}

    test {
        dcp_attrlist getval  [registry show] mingid
    } compare_strings {5000}

    # Cleanup steps
    if {[dcp_wrap {registry modify -mingid 5100}] != 0} {return;}
    if {[dcp_wrap {registry modify -hidepwd yes}] != 0} {return;}
}

proc registry_modify_065_P {} {
    # Identify the test
    tet_infoline registry_modify_065_P: Check that the argument can be specified as a cell
    tet_infoline registry_modify_065_P: name when changing attributes and/or policies.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        registry modify $DCP_CELLNAME_ONE -mingid 5000 -hidepwd no
        dcp_attrlist getval  [registry show] hidepwd
    } compare_strings {no}

    test {
        dcp_attrlist getval  [registry show] mingid
    } compare_strings {5000}

    # Cleanup steps
    if {[dcp_wrap {registry modify -mingid 5100}] != 0} {return;}
    if {[dcp_wrap {registry modify -hidepwd yes}] != 0} {return;}
}

proc registry_modify_066_P {} {
    # Identify the test
    tet_infoline registry_modify_066_P: Modify the keyseed of the security server specified 
    tet_infoline registry_modify_066_P: as the global name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry modify $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1 -key}

    # Cleanup steps
}

proc registry_modify_067_P {} {
    # Identify the test
    tet_infoline registry_modify_067_P: Modify the keyseed of the security server specified 
    tet_infoline registry_modify_067_P: as the local name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry modify /.:/$DCP_RGY_SA_REP_NAME_SLAVE1 -key}

    # Cleanup steps
}

proc registry_modify_068_P {} {
    # Identify the test
    tet_infoline registry_modify_068_P: Modify the keyseed of the security server specified 
    tet_infoline registry_modify_068_P: as a replica name as found in the master replica list.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry modify $DCP_RGY_SA_REP_NAME_SLAVE1 -key}

    # Cleanup steps
}

proc registry_modify_069_P {} {
    # Identify the test
    tet_infoline registry_modify_069_P: Modify the keyseed of the security server specified 
    tet_infoline registry_modify_069_P: as a string binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry modify $DCP_RGY_SA_PROTSEQ_SLAVE1:$DCP_RGY_SA_NETADDR_SLAVE1 -key}

    # Cleanup steps
}

proc registry_modify_070_P {} {
    # Identify the test
    tet_infoline registry_modify_070_P: Modify the keyseed of the security server specified 
    tet_infoline registry_modify_070_P: as a string binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry modify "$DCP_RGY_SA_PROTSEQ_SLAVE1 $DCP_RGY_SA_NETADDR_SLAVE1" -key}

    # Cleanup steps
}

proc registry_modify_071_P {} {
    # Identify the test
    tet_infoline registry_modify_071_P: Modify the keyseed of the master security server.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry modify $DCP_RGY_SA_REP_NAME_MASTER -key}

    # Cleanup steps
}

proc registry_modify_72_P {} {
    # Identify the test
    tet_infoline registry_modify_72_P: Modify the acctlife policy attribute.

    # Run the test
    test {
        registry modify -acctlife 1995 
        attrlist getvalues [registry show -policies] -type acctlife 
    } compare_strings +0-00:33:15.000I-----

    # Cleanup steps
    if {[dcp_wrap {registry modify -acctlife unlimited}] != 0} {return;}
}

proc registry_modify_73_P {} {
    # Identify the test
    tet_infoline registry_modify_73_P: Modify the maxtktlife policy attribute

    # Run the test
    test {
        registry modify -maxtktlife 20-0:0:0
        attrlist getvalues [registry show -policies] -type maxtktlife
    } compare_strings +20-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {registry modify -maxtktlife +1-00:00:00}] != 0} {return;}
}

proc registry_modify_74_P {} {
    # Identify the test
    tet_infoline registry_modify_74_P: Modify the maxtktrenew policy attribute

    # Run the test
    test {
        registry modify -maxtktrenew 21-0:0:0
        attrlist getvalues [registry show -policies] -type maxtktrenew
    } compare_strings +21-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {registry modify -maxtktrenew +28-00:00:00}] != 0} {return;}
}

proc registry_modify_75_P {} {
    # Identify the test
    tet_infoline registry_modify_75_P: Modify the pwdalpha policy attribute

    # Run the test
    test {
        registry modify -pwdalpha no 
        attrlist getvalues [registry show -policies] -type pwdalpha
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {registry modify -pwdalpha yes}] != 0} {return;}
}

proc registry_modify_76_P {} {
    # Identify the test
    tet_infoline registry_modify_76_P: Modify the pwdexpdate policy attribute

    # Run the test
    test {
        registry modify -pwdexpdate 1995-09-25-20:00:00.000-04:00I-----
        attrlist getvalues [registry show -policies] -type pwdexpdate
    } compare_strings 1995-09-25-20:00:00.000-04:00I-----

    # Cleanup steps
    if {[dcp_wrap {registry modify -pwdexpdate none }] != 0} {return;}
}

proc registry_modify_77_P {} {
    # Identify the test
    tet_infoline registry_modify_77_P: Modify the pwdlife policy attribute

    # Run the test
    test {
        registry modify -pwdlife 20-0:0:0
        attrlist getvalues [registry show -policies] -type pwdlife
    } compare_strings +20-00:00:00.000I-----

    # Cleanup steps
    if {[dcp_wrap {registry modify -pwdlife unlimited}] != 0} {return;}
}

proc registry_modify_78_P {} {
    # Identify the test
    tet_infoline registry_modify_78_P: Modify the pwdminlen policy attribute.

    # Run the test
    test {
        registry modify -pwdminlen 6
        attrlist  getvalues [registry show -policies] -type pwdminlen
    } compare_strings 6

    # Cleanup steps
    if {[dcp_wrap {registry modify -pwdminlen 0 }] != 0} {return;}
}

proc registry_modify_79_P {} {
    # Identify the test
    tet_infoline registry_modify_79_P: Modify the pwdspaces policy attribute

    # Run the test
    test {
        registry modify -pwdspaces no
        attrlist getvalues [registry show -policies] -type pwdspaces
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {registry modify -pwdspaces yes }] != 0} {return;}
}

proc registry_modify_80_P {} {
    # Identify the test
    tet_infoline registry_modify_80_P: Modify multiple attributes at the same time

    # Run the test
    test {
        registry modify -change {{pwdminlen 6} {pwdalpha no}}
        dcp_multi_values [registry show -policies] {{pwdminlen 6} {pwdalpha no}}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {registry modify -pwdminlen 0}] != 0} {return;}
    if {[dcp_wrap {registry modify -pwdalpha yes}] != 0} {return;}
}

proc registry_modify_81_P {} {
    # Identify the test
    tet_infoline registry_modify_81_P: Modify the version attribute

    # Run the test
    test {
        registry modify -version secd.dce.1.1
        attrlist getvalues [registry show] -type version
    } compare_strings secd.dce.1.1

    # Cleanup steps
}

proc registry_modify_82_P {} {
    # Identify the test
    tet_infoline registry_modify_82_P: Modify the version attribute

    # Run the test
    test {
        registry modify -change {version secd.dce.1.1}
        attrlist getvalues [registry show] -type version
    } compare_strings secd.dce.1.1

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 "
set ic1 "registry_modify_052_P"
set ic2 "registry_modify_053_P"
set ic3 "registry_modify_054_P"
set ic4 "registry_modify_055_P"
set ic5 "registry_modify_056_P"
set ic6 "registry_modify_057_P"
set ic7 "registry_modify_058_P"
set ic8 "registry_modify_059_P"
set ic9 "registry_modify_060_P"
set ic10 "registry_modify_061_P"
set ic11 "registry_modify_062_P"
set ic12 "registry_modify_063_P"
set ic13 "registry_modify_064_P"
set ic14 "registry_modify_065_P"
set ic15 "registry_modify_066_P"
set ic16 "registry_modify_067_P"
set ic17 "registry_modify_068_P"
set ic18 "registry_modify_069_P"
set ic19 "registry_modify_070_P"
set ic20 "registry_modify_071_P"
set ic21 "registry_modify_72_P"
set ic22 "registry_modify_73_P"
set ic23 "registry_modify_74_P"
set ic24 "registry_modify_75_P"
set ic25 "registry_modify_76_P"
set ic26 "registry_modify_77_P"
set ic27 "registry_modify_78_P"
set ic28 "registry_modify_79_P"
set ic29 "registry_modify_80_P"
set ic30 "registry_modify_81_P"
set ic31 "registry_modify_82_P"

set tet_startup registry_modify_startup
set tet_cleanup registry_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
