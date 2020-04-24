#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_modify_P.tcl,v $
# Revision 1.1.8.3  1996/03/11  02:23:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:58  marty]
#
# Revision 1.1.8.2  1995/12/11  15:13:25  root
# 	Submit
# 	[1995/12/11  14:31:12  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:49  root]
# 
# Revision 1.1.4.1  1994/10/14  19:33:32  cuti
# 	Update to reflect schema_update's work done in security.
# 	[1994/10/14  19:21:17  cuti]
# 
# 	Update to reflect schema_update's work done in security.
# 
# Revision 1.1.2.1  1994/09/28  21:44:27  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:57  melman]
# 
# 	Initial creation.
# 	[1994/07/27  15:54:00  nav]
# 
# Revision 1.1.2.6  1994/09/23  17:01:48  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:58:51  truitt]
# 
# Revision 1.1.2.5  1994/09/07  12:56:14  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:44:56  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:21:07  nav
# 	Real test suite in anticipation of support from the security api.
# 	[1994/08/26  17:19:14  nav]
# 
# Revision 1.1.2.3  1994/08/17  18:44:58  truitt
# 	Fix comment leader.
# 	[1994/08/17  18:31:18  truitt]
# 
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:46  annie]
# 
# Revision 1.1.2.2  1994/08/10  19:22:27  annie
# 	expand copyright with OSF copyright text
# 
# Revision 1.1.2.1  1994/07/27  18:52:06  nav
# 	Initial creation.
# 
# $EndLog$


if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_modify_1_P {} {
    # Identify the test
    tet_infoline schema_modify_1_P: Modify aclmgr set in the schema by adding a single aclmgr using switches.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_1_P -encoding integer -aclmgr {group r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_1_P -aclmgr {organization r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_1_P] aclmgr
    } compare_lists {{{group {query r} {update r} {test r} {delete r}}  {organization {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_1_P}] != 0} {return;}
}

proc schema_modify_2_P {} {
    # Identify the test
    tet_infoline schema_modify_2_P: Modify aclmgr set in the schema by adding a single aclmgr using attributes.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_2_P -encoding integer -aclmgr {group r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_2_P -change {aclmgr {organization r r r r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_2_P] aclmgr
    } compare_lists {{{group {query r} {update r} {test r} {delete r}}  {organization {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_2_P}] != 0} {return;}
}

proc schema_modify_3_P {} {
    # Identify the test
    tet_infoline schema_modify_3_P: Modify aclmgr set in the schema by adding multiple aclmgrs using switches.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_3_P -encoding integer -aclmgr {group r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_3_P -aclmgr {{principal r r r r} {organization r r r r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_3_P] aclmgr
    } compare_lists {{{group {query r} {update r} {test r} {delete r}} {principal {query r} {update r} {test r} {delete r}} {organization {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_3_P}] != 0} {return;}
}

proc schema_modify_4_P {} {
    # Identify the test
    tet_infoline schema_modify_4_P: Modify aclmgr set in the schema by adding multiple aclmgrs using switches.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_4_P -encoding integer -aclmgr {group r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_4_P -change {aclmgr {{principal r r r r} {organization r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_4_P] aclmgr
    } compare_lists {{{group {query r} {update r} {test r} {delete r}} {principal {query r} {update r} {test r} {delete r}} {organization {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_4_P}] != 0} {return;}
}

proc schema_modify_5_P {} {
    # Identify the test
    tet_infoline schema_modify_5_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_5_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_5_P -aclmgr {group rc rc rc rc}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_5_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {group {query rc} {update rc} {test rc} {delete rc}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_5_P}] != 0} {return;}
}

proc schema_modify_6_P {} {
    # Identify the test
    tet_infoline schema_modify_6_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_6_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_6_P -change {aclmgr {group rc rc rc rc}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_6_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {group {query rc} {update rc} {test rc} {delete rc}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_6_P}] != 0} {return;}
}

proc schema_modify_7_P {} {
    # Identify the test
    tet_infoline schema_modify_7_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_7_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_7_P -aclmgr {group n - n -}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_7_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {group {query n} {update -} {test n} {delete -}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_7_P}] != 0} {return;}
}

proc schema_modify_8_P {} {
    # Identify the test
    tet_infoline schema_modify_8_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_8_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_8_P -change {aclmgr {group n - n -}} 
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_8_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {group {query n} {update -} {test n} {delete -}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_8_P}] != 0} {return;}
}

proc schema_modify_9_P {} {
    # Identify the test
    tet_infoline schema_modify_9_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_9_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_9_P -aclmgr {group r r M r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_9_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test M} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_9_P}] != 0} {return;}
}

proc schema_modify_10_P {} {
    # Identify the test
    tet_infoline schema_modify_10_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
        # Schema name must not exist.
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_10_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_10_P -change {aclmgr {group r r M r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_10_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test M} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_10_P}] != 0} {return;}
}

proc schema_modify_11_P {} {
    # Identify the test
    tet_infoline schema_modify_11_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_11_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_11_P -aclmgr {principal r r r D}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_11_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {principal {query r} {update r} {test r} {delete D}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_11_P}] != 0} {return;}
}

proc schema_modify_12_P {} {
    # Identify the test
    tet_infoline schema_modify_12_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_12_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_12_P -change {aclmgr {principal r r r D}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_12_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {principal {query r} {update r} {test r} {delete D}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_12_P}] != 0} {return;}
}

proc schema_modify_13_P {} {
    # Identify the test
    tet_infoline schema_modify_13_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_13_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_13_P -aclmgr {principal gruf rf r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_13_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {principal {query rfug} {update rf} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_13_P}] != 0} {return;}
}

proc schema_modify_14_P {} {
    # Identify the test
    tet_infoline schema_modify_14_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_14_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_14_P -change {aclmgr {principal gruf rf r r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_14_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {principal {query rfug} {update rf} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_14_P}] != 0} {return;}
}

proc schema_modify_15_P {} {
    # Identify the test
    tet_infoline schema_modify_15_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_15_P -encoding integer -aclmgr {group r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_15_P -aclmgr {organization t t t t}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_15_P] aclmgr
    } compare_lists {{{group {query r} {update r} {test r} {delete r}}  {organization {query t} {update t} {test t} {delete t}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_15_P}] != 0} {return;}
}

proc schema_modify_16_P {} {
    # Identify the test
    tet_infoline schema_modify_16_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_16_P -attribute {{encoding integer} {aclmgr {group t t t t}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_16_P -change {aclmgr {organization t t t t}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_16_P] aclmgr
    } compare_lists {{{group {query t} {update t} {test t} {delete t}}  {organization {query t} {update t} {test t} {delete t}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_16_P}] != 0} {return;}
}

proc schema_modify_17_P {} {
    # Identify the test
    tet_infoline schema_modify_17_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_17_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_17_P -aclmgr {secdirectory id i i i}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_17_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {secdirectory {query id} {update i} {test i} {delete i}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_17_P}] != 0} {return;}
}

proc schema_modify_18_P {} {
    # Identify the test
    tet_infoline schema_modify_18_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_18_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_18_P -change {aclmgr {secdirectory id i i i}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_18_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {secdirectory {query id} {update i} {test i} {delete i}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_18_P}] != 0} {return;}
}

proc schema_modify_19_P {} {
    # Identify the test
    tet_infoline schema_modify_19_P: Modify aclmgr set with a different group aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_19_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_19_P -aclmgr {policy ar m ar m}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_19_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {policy {query ra} {update m} {test ra} {delete m}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_19_P}] != 0} {return;}
}

proc schema_modify_20_P {} {
    # Identify the test
    tet_infoline schema_modify_20_P: Modify aclmgr set with a different group aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_20_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_20_P -change {aclmgr {policy ar m ar m}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_20_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {policy {query ra} {update m} {test ra} {delete m}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_20_P}] != 0} {return;}
}

proc schema_modify_21_P {} {
    # Identify the test
    tet_infoline schema_modify_21_P: Add a different replist aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_21_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_21_P  -aclmgr {replist AI AI AI AI}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_21_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {replist {query AI} {update AI} {test AI} {delete AI}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_21_P}] != 0} {return;}
}

proc schema_modify_22_P {} {
    # Identify the test
    tet_infoline schema_modify_22_P: Add a different replist aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_22_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_22_P -change {aclmgr {replist AI AI AI AI}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_22_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {replist {query AI} {update AI} {test AI} {delete AI}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_22_P}] != 0} {return;}
}

proc schema_modify_23_P {} {
    # Identify the test
    tet_infoline schema_modify_23_P: Add a different xattrschema aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_23_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_23_P -aclmgr {xattrschema r r r r} 
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_23_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {xattrschema {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_23_P}] != 0} {return;}
}

proc schema_modify_24_P {} {
    # Identify the test
    tet_infoline schema_modify_24_P: Add a different xattrschema aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_24_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_24_P -change {aclmgr {xattrschema r r r r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_24_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {xattrschema {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_24_P}] != 0} {return;}
}

proc schema_modify_25_P {} {
    # Identify the test
    tet_infoline schema_modify_25_P: Add an aclmgr by specifying it by uuid using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_25_P -encoding integer -aclmgr {organization r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_25_P -aclmgr {06ab8f10-0191-11ca-a9e8-08001e039d7d r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_25_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {policy {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_25_P}] != 0} {return;}
}

proc schema_modify_26_P {} {
    # Identify the test
    tet_infoline schema_modify_26_P: Add an aclmgr by specifying it by uuid using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_26_P -attribute {{encoding integer} {aclmgr {organization r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_26_P -change {aclmgr {06ab8f10-0191-11ca-a9e8-08001e039d7d r r r r }}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_26_P] aclmgr
    } compare_lists {{{organization {query r} {update r} {test r} {delete r}}  {policy {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_26_P}] != 0} {return;}
}

proc schema_modify_27_P {} {
    # Identify the test
    tet_infoline schema_modify_27_P: Modify the applydefs field to no using  switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_27_P -encoding integer -aclmgr {group r r r r} -unique yes -multivalued yes -reserved yes -applydefs yes}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_27_P -applydefs no
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_27_P] applydefs
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {xattrschema modify /.:/sec/xattrschema/schema_modify_27_P -reserved no}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_27_P}] != 0} {return;}
}

proc schema_modify_28_P {} {
    # Identify the test
    tet_infoline schema_modify_28_P: Modify the applydefs field to no using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_28_P -attribute {{encoding integer} {aclmgr {group r r r r}} {unique yes} {multivalued yes} {reserved yes} {applydefs yes}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_28_P -change {applydefs no}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_28_P] applydefs
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {xattrschema modify /.:/sec/xattrschema/schema_modify_28_P -reserved no}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_28_P}] != 0} {return;}
}

proc schema_modify_29_P {} {
    # Identify the test
    tet_infoline schema_modify_29_P: Modify the reserved field to yes using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_29_P -encoding integer -aclmgr {group r r r r} -unique no -multivalued no -reserved no -applydefs no}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_29_P -reserved yes
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_29_P] reserved
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {xattrschema modify /.:/sec/xattrschema/schema_modify_29_P -reserved no}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_29_P}] != 0} {return;}
}

proc schema_modify_30_P {} {
    # Identify the test
    tet_infoline schema_modify_30_P: Modify intercell type to reject using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_30_P -encoding integer -aclmgr {group r r r r} -intercell accept}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_30_P -intercell reject
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_30_P] intercell
    } compare_strings reject

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_30_P}] != 0} {return;}
}

proc schema_modify_31_P {} {
    # Identify the test
    tet_infoline schema_modify_31_P: Modify intercell type to reject using attributes 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_31_P -encoding integer -aclmgr {group r r r r} -intercell accept}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_31_P -change {intercell reject}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_31_P] intercell
    } compare_strings reject

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_31_P}] != 0} {return;}
}

proc schema_modify_32_P {} {
    # Identify the test
    tet_infoline schema_modify_32_P: Modify intercell type to accept using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_32_P -encoding integer -aclmgr {group r r r r} -intercell reject}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_32_P -intercell accept
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_32_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_32_P}] != 0} {return;}
}

proc schema_modify_33_P {} {
    # Identify the test
    tet_infoline schema_modify_33_P: Modify intercell type to accept using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_33_P -attribute {{encoding integer} {aclmgr {group r r r r}} {intercell reject}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_33_P -change {intercell accept}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_33_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_33_P}] != 0} {return;}
}

proc schema_modify_34_P {} {
    # Identify the test
    tet_infoline schema_modify_34_P: Modify the intercell type from evaluate to accept using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_34_P -encoding integer -aclmgr {group r r r r} -intercell evaluate -trigbind {none {/.:/blech}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_34_P -intercell accept
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_34_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_34_P}] != 0} {return;}
}

proc schema_modify_35_P {} {
    # Identify the test
    tet_infoline schema_modify_35_P: Modify the intercell type from evaluate to accept using attributes 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_35_P -attribute {{encoding integer} {aclmgr {group r r r r}} {intercell evaluate} {trigbind {none {/.:/blech}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_35_P -change {intercell accept}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_35_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_35_P}] != 0} {return;}
}

proc schema_modify_36_P {} {
    # Identify the test
    tet_infoline schema_modify_36_P: Modify the server name of a none type trig bind using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_36_P -encoding integer -aclmgr {group r r r r} -trigbind {none {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/sec/xattrschema/schema_modify_36_P -trigbind {none {/.:/gumby2}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_36_P] trigbind
    } compare_lists {{none /.:/gumby2}} 

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_36_P}] != 0} {return;}
}

proc schema_modify_37_P {} {
    # Identify the test
    tet_infoline schema_modify_37_P: Modify the server name of a none type trig bind using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_37_P -encoding integer -aclmgr {group r r r r} -trigbind {none {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/sec/xattrschema/schema_modify_37_P -change {trigbind {none {/.:/gumby2}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_37_P] trigbind
    } compare_lists {{none /.:/gumby2}} 

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_37_P}] != 0} {return;}
}

proc schema_modify_38_P {} {
    # Identify the test
    tet_infoline schema_modify_38_P: Modify the server name by specifying it in a tower set for a none type trig bind using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_38_P -encoding integer -aclmgr {group r r r r} -trigbind {none {ncacn_ip_tcp 10.29.58.00 2001}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/sec/xattrschema/schema_modify_38_P -trigbind {none {ncacn_ip_tcp 10.30.58.00 2001}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_38_P] trigbind
    } compare_lists {{none {ncacn_ip_tcp 10.30.58.00 2001}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_38_P}] != 0} {return;}
}

proc schema_modify_39_P {} {
    # Identify the test
    tet_infoline schema_modify_39_P: Modify the server name by specifying it in a tower set for a none type trig bind using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_39_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {none {ncacn_ip_tcp 10.29.58.00 2001}}}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/sec/xattrschema/schema_modify_39_P -change {trigbind {none {ncacn_ip_tcp 10.30.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_39_P] trigbind
    } compare_lists {{none {ncacn_ip_tcp 10.30.58.00 2001}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_39_P}] != 0} {return;}
}

proc schema_modify_40_P {} {
    # Identify the test
    tet_infoline schema_modify_40_P: Modify the trig bind of none with multiple server names using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_40_P -encoding integer -aclmgr {group r r r r} -trigbind {none {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_40_P -trigbind {none {{/.:/gumby} {ncacn_ip_tcp 10.29.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_40_P] trigbind
    } compare_lists {{none {/.:/gumby {ncacn_ip_tcp 10.29.58.00 2001}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_40_P}] != 0} {return;}
}

proc schema_modify_41_P {} {
    # Identify the test
    tet_infoline schema_modify_41_P: Modify the trig bind of none with multiple server names using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_41_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {none {/.:/gumby}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_41_P -change {trigbind {none {{/.:/gumby} {ncacn_ip_tcp 10.29.58.00 2001}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_41_P] trigbind
    } compare_lists {{none {/.:/gumby {ncacn_ip_tcp 10.29.58.00 2001}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_41_P}] != 0} {return;}
}

proc schema_modify_42_P {} {
    # Identify the test
    tet_infoline schema_modify_42_P: Modify the server name part of trig bind of type dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_42_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech default default none} {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_42_P -trigbind {{dce blech default default none} {/.:/gumby2}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_42_P] trigbind
    } compare_lists {{{dce blech default default none}   /.:/gumby2}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_42_P}] != 0} {return;}
}

proc schema_modify_43_P {} {
    # Identify the test
    tet_infoline schema_modify_43_P: Modify the server name part of trig bind of type dce using attributes 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_43_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech default default none} {/.:/gumby}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_43_P -change {trigbind {{dce blech default default none} {/.:/gumby2}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_43_P] trigbind
    } compare_lists {{{dce blech default default none}   /.:/gumby2}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_43_P}] != 0} {return;}
}

proc schema_modify_44_P {} {
    # Identify the test
    tet_infoline schema_modify_44_P: Modify the server name part of trig bind of type dce using switches 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_44_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech none none none} {{/.:/gumby} {/.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_44_P -trigbind {{dce blech none none none} {{/.:/gumby2} {/.:/pokey2}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_44_P] trigbind
    } compare_lists {{{dce blech none none none} {/.:/gumby2 /.:/pokey2}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_44_P}] != 0} {return;}
}

proc schema_modify_45_P {} {
    # Identify the test
    tet_infoline schema_modify_45_P: Modify the server name part of trig bind of type dce using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_45_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech none none none} {{/.:/gumby} {/.:/pokey}}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_45_P -change {trigbind {{dce blech none none none} {{/.:/gumby2} {/.:/pokey2}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_45_P] trigbind
    } compare_lists {{{dce blech none none none} {/.:/gumby2 /.:/pokey2}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_45_P}] != 0} {return;}
}

proc schema_modify_46_P {} {
    # Identify the test
    tet_infoline schema_modify_46_P: Modify the auth info part of trig bind of type dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_46_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech connect public dce} {ncacn_ip_tcp 10.29.58.00 2001}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_46_P -trigbind {{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_46_P] trigbind
    } compare_lists {{{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}}  

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_46_P}] != 0} {return;}
}

proc schema_modify_47_P {} {
    # Identify the test
    tet_infoline schema_modify_47_P: Modify the auth info part of trig bind of type dce using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_47_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech connect public dce} {ncacn_ip_tcp 10.29.58.00 2001}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_47_P -change {trigbind {{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_47_P] trigbind
    } compare_lists {{{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}}  

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_47_P}] != 0} {return;}
}

proc schema_modify_48_P {} {
    # Identify the test
    tet_infoline schema_modify_48_P: Modify multiple server names in trig bind of dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_48_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2001} {ncacn_ip_tcp 03.19.61.00 2010}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_48_P -trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_48_P] trigbind
    } compare_lists {{{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_48_P}] != 0} {return;}
}

proc schema_modify_49_P {} {
    # Identify the test
    tet_infoline schema_modify_49_P: Modify multiple server names in trig bind of dce using attribute names

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_49_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2001} {ncacn_ip_tcp 03.19.61.00 2010}}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_49_P -change {trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_49_P] trigbind
    } compare_lists {{{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_49_P}] != 0} {return;}
}

proc schema_modify_50_P {} {
    # Identify the test
    tet_infoline schema_modify_50_P: Modify the auth info part of trig bind of type dce using switches 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_50_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech pkt secret name} {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_50_P -trigbind {{dce blech pktinteg none name} {/.:/gumby}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_50_P] trigbind
    } compare_lists {{{dce blech pktinteg none name}   /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_50_P}] != 0} {return;}
}

proc schema_modify_51_P {} {
    # Identify the test
    tet_infoline schema_modify_51_P: Modify the auth info part of trig bind of type dce using attribute names

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_51_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech pkt secret name} {/.:/gumby}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_51_P -change {trigbind {{dce blech pktinteg none name} {/.:/gumby}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_51_P] trigbind
    } compare_lists {{{dce blech pktinteg none name}   /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_51_P}] != 0} {return;}
}

proc schema_modify_52_P {} {
    # Identify the test
    tet_infoline schema_modify_52_P: Modify a basic schema name with trig bind of dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_52_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech pktprivacy secret name} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_52_P -trigbind {{dce blech pkt secret dce} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_52_P] trigbind
    } compare_lists {{{dce blech pkt secret dce} {/.:/gumby /.:/pokey /.:/daffy}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_52_P}] != 0} {return;}
}

proc schema_modify_53_P {} {
    # Identify the test
    tet_infoline schema_modify_53_P: Modify a basic schema name with trig bind of dce using attribute names

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_53_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech pktprivacy secret name} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_53_P -change {trigbind {{dce blech pkt secret none} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_53_P] trigbind
    } compare_lists {{{dce blech pkt secret none} {/.:/gumby /.:/pokey /.:/daffy}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_53_P}] != 0} {return;}
}

proc schema_modify_54_P {} {
    # Identify the test
    tet_infoline schema_modify_54_P: Modify the comment string using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_54_P -encoding integer -aclmgr {group r r r r} -annotation "this software, by its very complexity, defends itself against being used"}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_54_P -annotation "this software, by its very complexity, protects itself from being used"
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_54_P] annotation
    } compare_strings {{this software, by its very complexity, protects itself from being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_54_P}] != 0} {return;}
}

proc schema_modify_55_P {} {
    # Identify the test
    tet_infoline schema_modify_55_P: Modify the comment string using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_55_P -encoding integer -aclmgr {group r r r r} -annotation "this software, by its very complexity, defends itself against being used"}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_55_P -change {annotation "this software, by its very complexity, protects itself from being used"}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_55_P] annotation
    } compare_strings {{this software, by its very complexity, protects itself from being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_55_P}] != 0} {return;}
}

proc schema_modify_56_P {} {
    # Identify the test
    tet_infoline schema_modify_56_P: Create a schema entry using longhand aclmgr set.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_56_P -encoding integer -aclmgr {principal r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_56_P -aclmgr {group {query r} {update r} {test r} {delete r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_56_P] aclmgr
    } compare_lists {{{principal {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_56_P}] != 0} {return;}
}

proc schema_modify_57_P {} {
    # Identify the test
    tet_infoline schema_modify_57_P: Create a schema entry using longhand aclmgr set.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_57_P -encoding integer -aclmgr {principal r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_57_P -aclmgr {gr {qu r} {upd r} {te r} {del r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_57_P] aclmgr
    } compare_lists {{{principal {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_57_P}] != 0} {return;}
}

proc schema_modify_58_P {} {
    # Identify the test
    tet_infoline schema_modify_58_P: Create a schema entry using longhand aclmgr set.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_58_P -encoding integer -aclmgr {principal r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_58_P -aclmgr {group {query r} r {te r} r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_58_P] aclmgr
    } compare_lists {{{principal {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_58_P}] != 0} {return;}
}

proc schema_modify_59_P {} {
    # Identify the test
    tet_infoline schema_modify_59_P: Create a schema entry using longhand aclmgr set.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_59_P -encoding integer -aclmgr {principal r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_59_P -change {aclmgr {group {query r} {update r} {test r} {delete r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_59_P] aclmgr
    } compare_lists {{{principal {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_59_P}] != 0} {return;}
}

proc schema_modify_60_P {} {
    # Identify the test
    tet_infoline schema_modify_60_P: Create a schema entry using longhand aclmgr set.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_60_P -encoding integer -aclmgr {principal r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_60_P -change {aclmgr {gr {qu r} {upd r} {te r} {del r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_60_P] aclmgr
    } compare_lists {{{principal {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_60_P}] != 0} {return;}
}

proc schema_modify_61_P {} {
    # Identify the test
    tet_infoline schema_modify_61_P: Create a schema entry using longhand aclmgr set.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_modify_61_P -encoding integer -aclmgr {principal r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/sec/xattrschema/schema_modify_61_P -change {aclmgr {group {query r} r {te r} r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_modify_61_P] aclmgr
    } compare_lists {{{principal {query r} {update r} {test r} {delete r}}  {group {query r} {update r} {test r} {delete r}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_modify_61_P}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 "
set ic1 "schema_modify_1_P"
set ic2 "schema_modify_2_P"
set ic3 "schema_modify_3_P"
set ic4 "schema_modify_4_P"
set ic5 "schema_modify_5_P"
set ic6 "schema_modify_6_P"
set ic7 "schema_modify_7_P"
set ic8 "schema_modify_8_P"
set ic9 "schema_modify_9_P"
set ic10 "schema_modify_10_P"
set ic11 "schema_modify_11_P"
set ic12 "schema_modify_12_P"
set ic13 "schema_modify_13_P"
set ic14 "schema_modify_14_P"
set ic15 "schema_modify_15_P"
set ic16 "schema_modify_16_P"
set ic17 "schema_modify_17_P"
set ic18 "schema_modify_18_P"
set ic19 "schema_modify_19_P"
set ic20 "schema_modify_20_P"
set ic21 "schema_modify_21_P"
set ic22 "schema_modify_22_P"
set ic23 "schema_modify_23_P"
set ic24 "schema_modify_24_P"
set ic25 "schema_modify_25_P"
set ic26 "schema_modify_26_P"
set ic27 "schema_modify_27_P"
set ic28 "schema_modify_28_P"
set ic29 "schema_modify_29_P"
set ic30 "schema_modify_30_P"
set ic31 "schema_modify_31_P"
set ic32 "schema_modify_32_P"
set ic33 "schema_modify_33_P"
set ic34 "schema_modify_34_P"
set ic35 "schema_modify_35_P"
set ic36 "schema_modify_36_P"
set ic37 "schema_modify_37_P"
set ic38 "schema_modify_38_P"
set ic39 "schema_modify_39_P"
set ic40 "schema_modify_40_P"
set ic41 "schema_modify_41_P"
set ic42 "schema_modify_42_P"
set ic43 "schema_modify_43_P"
set ic44 "schema_modify_44_P"
set ic45 "schema_modify_45_P"
set ic46 "schema_modify_46_P"
set ic47 "schema_modify_47_P"
set ic48 "schema_modify_48_P"
set ic49 "schema_modify_49_P"
set ic50 "schema_modify_50_P"
set ic51 "schema_modify_51_P"
set ic52 "schema_modify_52_P"
set ic53 "schema_modify_53_P"
set ic54 "schema_modify_54_P"
set ic55 "schema_modify_55_P"
set ic56 "schema_modify_56_P"
set ic57 "schema_modify_57_P"
set ic58 "schema_modify_58_P"
set ic59 "schema_modify_59_P"
set ic60 "schema_modify_60_P"
set ic61 "schema_modify_61_P"
set tet_startup schema_modify_startup
set tet_cleanup schema_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
