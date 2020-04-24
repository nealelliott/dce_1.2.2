#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_modify_generic_P.tcl,v $
# Revision 1.1.6.1  1996/11/14  16:09:51  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:25:56  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/21  18:16 UTC  truitt
# 	Initial branch creation for test changes.
# 
# Revision /main/truitt_dcecp_pk7/1  1996/10/18  14:46 UTC  truitt
# 	CHFts20285: Update schema tests to reflect new aclmgr functionality.
# 
# Revision 1.1.4.2  1996/03/11  02:24:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:01  marty]
# 
# Revision 1.1.4.1  1995/12/11  15:13:29  root
# 	Submit
# 	[1995/12/11  14:31:15  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:52  root]
# 
# Revision 1.1.2.2  1994/09/30  17:12:17  truitt
# 	OT12414: Modify some of the test cases to take into account
# 	the legal permission set allowed on the srvrconf acl manager.
# 	[1994/09/30  17:04:10  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:44:29  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:58  melman]
# 
# 	Initial creation.
# 	[1994/08/25  16:15:39  nav]
# 
# Revision 1.1.2.5  1994/09/23  17:01:49  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:59:14  truitt]
# 
# Revision 1.1.2.4  1994/09/21  15:26:10  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:58:18  truitt]
# 
# Revision 1.1.2.3  1994/09/07  16:13:24  truitt
# 	Fix a mistake in one of the proc names.
# 	[1994/09/07  16:12:24  truitt]
# 
# Revision 1.1.2.2  1994/09/07  12:56:16  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:45:42  truitt]
# 
# Revision 1.1.2.1  1994/08/25  16:16:36  nav
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


## Test purposes
proc schema_modify_generic_1_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_1_P: Modify aclmgr set in the schema by adding a single aclmgr using switches.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_1_P -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_1_P -aclmgr {srvrconf r d r d}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_1_P] aclmgr
    } compare_lists {{srvrconf {query r} {update d} {test r} {delete d}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_1_P}] != 0} {return;}
}

proc schema_modify_generic_2_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_2_P: Modify aclmgr set in the schema by adding a single aclmgr using attributes.

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_2_P -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_2_P -change {aclmgr {srvrconf r d r d}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_2_P] aclmgr
    } compare_lists {{srvrconf {query r} {update d} {test r} {delete d}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_2_P}] != 0} {return;}
}

proc schema_modify_generic_3_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_3_P: Modify aclmgr set with a different srvrconf aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_3_P -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_3_P -aclmgr {srvrconf rc rc rc rc}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_3_P] aclmgr
    } compare_lists {{srvrconf {query rc} {update rc} {test rc} {delete rc}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_3_P}] != 0} {return;}
}

proc schema_modify_generic_4_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_4_P: Modify aclmgr set with a different srvrconf aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_4_P -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_4_P -change {aclmgr {srvrconf rc rc rc rc}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_4_P] aclmgr
    } compare_lists {{srvrconf {query rc} {update rc} {test rc} {delete rc}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_4_P}] != 0} {return;}
}

proc schema_modify_generic_5_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_5_P: Modify aclmgr set with a different srvrconf aclmgr using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_5_P -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_5_P -aclmgr {srvrconf w w w w}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_5_P] aclmgr
    } compare_lists {{srvrconf {query w} {update w} {test w} {delete w}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_5_P}] != 0} {return;}
}

proc schema_modify_generic_6_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_6_P: Modify aclmgr set with a different srvrconf aclmgr using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_6_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_6_P -change {aclmgr {srvrconf w w w w}} 
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_6_P] aclmgr
    } compare_lists {{srvrconf {query w} {update w} {test w} {delete w}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_6_P}] != 0} {return;}
}

proc schema_modify_generic_7_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_7_P: Add an aclmgr by specifying it by uuid using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_7_P -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_7_P -aclmgr {004d3731-c9b5-1d6c-9adb-0000c09ce054 r r r r }
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_7_P] aclmgr
    } compare_lists {{srvrconf {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_7_P}] != 0} {return;}
}

proc schema_modify_generic_8_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_8_P: Add an aclmgr by specifying it by uuid using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_8_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_8_P -change {aclmgr {004d3731-c9b5-1d6c-9adb-0000c09ce054 r r r r }}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_8_P] aclmgr
    } compare_lists {{srvrconf {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_8_P}] != 0} {return;}
}

proc schema_modify_generic_9_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_9_P: Modify the applydefs field from yes to no using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_9_P -encoding integer -aclmgr {srvrconf r r r r} -unique yes -multivalued no -reserved yes -applydefs yes}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_9_P -applydefs no
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_9_P] applydefs
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_9_P}] != 0} {return;}
}

proc schema_modify_generic_10_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_10_P: Modify the reserved field from yes to no using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_10_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {unique yes} {multivalued no} {reserved yes} {applydefs yes}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_10_P -change {reserved no}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_10_P] reserved
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_10_P}] != 0} {return;}
}

proc schema_modify_generic_11_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_11_P: Modify the reserved field from no to yes using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_11_P -encoding integer -aclmgr {srvrconf r r r r} -unique no -multivalued no -reserved no -applydefs no}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_11_P -reserved yes
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_11_P] reserved
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_11_P}] != 0} {return;}
}

proc schema_modify_generic_12_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_12_P: Modify intercell type to reject using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_12_P -encoding integer -aclmgr {srvrconf r r r r} -intercell accept}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_12_P -intercell reject
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_12_P] intercell
    } compare_strings reject

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_12_P}] != 0} {return;}
}

proc schema_modify_generic_13_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_13_P: Modify intercell type to reject using attributes 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_13_P -encoding integer -aclmgr {srvrconf r r r r} -intercell accept}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_13_P -change {intercell reject}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_13_P] intercell
    } compare_strings reject

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_13_P}] != 0} {return;}
}

proc schema_modify_generic_14_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_14_P: Modify intercell type to accept using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_14_P -encoding integer -aclmgr {srvrconf r r r r} -intercell reject}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_14_P -intercell accept
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_14_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_14_P}] != 0} {return;}
}

proc schema_modify_generic_15_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_15_P: Modify intercell type to accept using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_15_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {intercell reject}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_15_P -change {intercell accept}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_15_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_15_P}] != 0} {return;}
}

proc schema_modify_generic_16_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_16_P: Modify the intercell type from evaluate to accept using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_16_P -encoding integer -aclmgr {srvrconf r r r r} -intercell evaluate -trigbind {none {/.:/blech}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_16_P -intercell accept
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_16_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_16_P}] != 0} {return;}
}

proc schema_modify_generic_17_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_17_P: Modify the intercell type from evaluate to accept using attributes 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_17_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {intercell evaluate} {trigbind {none {/.:/blech}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_17_P -change {intercell accept}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_17_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_17_P}] != 0} {return;}
}

proc schema_modify_generic_18_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_18_P: Modify the server name of a none type trig bind using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_18_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {none {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_18_P -trigbind {none {/.:/gumby2}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_18_P] trigbind
    } compare_lists {{none /.:/gumby2}} 

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_18_P}] != 0} {return;}
}

proc schema_modify_generic_19_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_19_P: Modify the server name of a none type trig bind using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_19_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {none {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_19_P -change {trigbind {none {/.:/gumby2}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_19_P] trigbind
    } compare_lists {{none /.:/gumby2}} 

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_19_P}] != 0} {return;}
}

proc schema_modify_generic_20_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_20_P: Modify the server name by specifying it in a tower set for a none type trig bind using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_20_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {none {ncacn_ip_tcp 10.29.58.00 2001}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_20_P -trigbind {none {ncacn_ip_tcp 10.30.58.00 2001}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_20_P] trigbind
    } compare_lists {{none {ncacn_ip_tcp 10.30.58.00 2001}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_20_P}] != 0} {return;}
}

proc schema_modify_generic_21_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_21_P: Modify the server name by specifying it in a tower set for a none type trig bind using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_21_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {none {ncacn_ip_tcp 10.29.58.00 2001}}}}}] != 0} {return;}

    # Run the test
    test { xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_21_P -change {trigbind {none {ncacn_ip_tcp 10.30.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_21_P] trigbind
    } compare_lists {{none {ncacn_ip_tcp 10.30.58.00 2001}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_21_P}] != 0} {return;}
}

proc schema_modify_generic_22_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_22_P: Modify the trig bind of none with multiple server names using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_22_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {none {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_22_P -trigbind {none {{/.:/gumby} {ncacn_ip_tcp 10.29.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_22_P] trigbind
    } compare_lists {{none {/.:/gumby {ncacn_ip_tcp 10.29.58.00 2001}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_22_P}] != 0} {return;}
}

proc schema_modify_generic_23_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_23_P: Modify the trig bind of none with multiple server names using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_23_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {none {/.:/gumby}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_23_P -change {trigbind {none {{/.:/gumby} {ncacn_ip_tcp 10.29.58.00 2001}}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_23_P] trigbind
    } compare_lists {{none {/.:/gumby {ncacn_ip_tcp 10.29.58.00 2001}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_23_P}] != 0} {return;}
}

proc schema_modify_generic_24_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_24_P: Modify the server name part of trig bind of type dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_24_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech default default none} {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_24_P -trigbind {{dce blech default default none} {/.:/gumby2}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_24_P] trigbind
    } compare_lists {{{dce blech default default none}   /.:/gumby2}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_24_P}] != 0} {return;}
}

proc schema_modify_generic_25_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_25_P: Modify the server name part of trig bind of type dce using attributes 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_25_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {{dce blech default default none} {/.:/gumby}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_25_P -change {trigbind {{dce blech default default none} {/.:/gumby2}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_25_P] trigbind
    } compare_lists {{{dce blech default default none}   /.:/gumby2}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_25_P}] != 0} {return;}
}

proc schema_modify_generic_26_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_26_P: Modify the server name part of trig bind of type dce using switches 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_26_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech none none none} {{/.:/gumby} {/.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_26_P -trigbind {{dce blech none none none} {{/.:/gumby2} {/.:/pokey2}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_26_P] trigbind
    } compare_lists {{{dce blech none none none} {/.:/gumby2 /.:/pokey2}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_26_P}] != 0} {return;}
}

proc schema_modify_generic_27_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_27_P: Modify the server name part of trig bind of type dce using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_27_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {{dce blech none none none} {{/.:/gumby} {/.:/pokey}}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_27_P -change {trigbind {{dce blech none none none} {{/.:/gumby2} {/.:/pokey2}}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_27_P] trigbind
    } compare_lists {{{dce blech none none none} {/.:/gumby2 /.:/pokey2}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_27_P}] != 0} {return;}
}

proc schema_modify_generic_28_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_28_P: Modify the auth info part of trig bind of type dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_28_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech connect public dce} {ncacn_ip_tcp 10.29.58.00 2001}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_28_P -trigbind {{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_28_P] trigbind
    } compare_lists {{{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}}  

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_28_P}] != 0} {return;}
}

proc schema_modify_generic_29_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_29_P: Modify the auth info part of trig bind of type dce using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_29_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech connect public dce} {ncacn_ip_tcp 10.29.58.00 2001}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_29_P -change {trigbind {{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_29_P] trigbind
    } compare_lists {{{dce blech2 pktprivacy secret dce} {ncacn_ip_tcp 10.29.58.00 2001}}}  

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_29_P}] != 0} {return;}
}

proc schema_modify_generic_30_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_30_P: Modify multiple server names in trig bind of dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_30_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2001} {ncacn_ip_tcp 03.19.61.00 2010}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_30_P -trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_30_P] trigbind
    } compare_lists {{{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_30_P}] != 0} {return;}
}

proc schema_modify_generic_31_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_31_P: Modify multiple server names in trig bind of dce using attribute names

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_31_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2001} {ncacn_ip_tcp 03.19.61.00 2010}}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_31_P -change {trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_31_P] trigbind
    } compare_lists {{{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2002} {ncacn_ip_tcp 03.19.60.00 2010}}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_31_P}] != 0} {return;}
}

proc schema_modify_generic_32_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_32_P: Modify the auth info part of trig bind of type dce using switches 

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_32_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech pkt secret name} {/.:/gumby}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_32_P -trigbind {{dce blech pktinteg none name} {/.:/gumby}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_32_P] trigbind
    } compare_lists {{{dce blech pktinteg none name}   /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_32_P}] != 0} {return;}
}

proc schema_modify_generic_33_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_33_P: Modify the auth info part of trig bind of type dce using attribute names

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_33_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {{dce blech pkt secret name} {/.:/gumby}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_33_P -change {trigbind {{dce blech pktinteg none name} {/.:/gumby}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_33_P] trigbind
    } compare_lists {{{dce blech pktinteg none name}   /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_33_P}] != 0} {return;}
}

proc schema_modify_generic_34_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_34_P: Modify a basic schema name with trig bind of dce using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_34_P -encoding integer -aclmgr {srvrconf r r r r} -trigbind {{dce blech pktprivacy secret name} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_34_P -trigbind {{dce blech pkt secret dce} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_34_P] trigbind
    } compare_lists {{{dce blech pkt secret dce} {/.:/gumby /.:/pokey /.:/daffy}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_34_P}] != 0} {return;}
}

proc schema_modify_generic_35_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_35_P: Modify a basic schema name with trig bind of dce using attribute names

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_35_P -attribute {{encoding integer} {aclmgr {srvrconf r r r r}} {trigbind {{dce blech pktprivacy secret name} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}}}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_35_P -change {trigbind {{dce blech pkt secret none} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_35_P] trigbind
    } compare_lists {{{dce blech pkt secret none} {/.:/gumby /.:/pokey /.:/daffy}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_35_P}] != 0} {return;}
}

proc schema_modify_generic_36_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_36_P: Modify the comment string using switches

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_36_P -encoding integer -aclmgr {srvrconf r r r r} -annotation "this software, by its very complexity, defends itself against being used"}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_36_P -annotation "this software, by its very complexity, protects itself from being used"
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_36_P] annotation
    } compare_strings {{this software, by its very complexity, protects itself from being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_36_P}] != 0} {return;}
}

proc schema_modify_generic_37_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_generic_37_P: Modify the comment string using attributes

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_37_P -encoding integer -aclmgr {srvrconf r r r r} -annotation "this software, by its very complexity, defends itself against being used"}] != 0} {return;}

    # Run the test
    test {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_37_P -change {annotation "this software, by its very complexity, protects itself from being used"}
        dcp_attrlist getval [xattrschema show /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_37_P] annotation
    } compare_strings {{this software, by its very complexity, protects itself from being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_generic_37_P}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 "
set ic1 "schema_modify_generic_1_P"
set ic2 "schema_modify_generic_2_P"
set ic3 "schema_modify_generic_3_P"
set ic4 "schema_modify_generic_4_P"
set ic5 "schema_modify_generic_5_P"
set ic6 "schema_modify_generic_6_P"
set ic7 "schema_modify_generic_7_P"
set ic8 "schema_modify_generic_8_P"
set ic9 "schema_modify_generic_9_P"
set ic10 "schema_modify_generic_10_P"
set ic11 "schema_modify_generic_11_P"
set ic12 "schema_modify_generic_12_P"
set ic13 "schema_modify_generic_13_P"
set ic14 "schema_modify_generic_14_P"
set ic15 "schema_modify_generic_15_P"
set ic16 "schema_modify_generic_16_P"
set ic17 "schema_modify_generic_17_P"
set ic18 "schema_modify_generic_18_P"
set ic19 "schema_modify_generic_19_P"
set ic20 "schema_modify_generic_20_P"
set ic21 "schema_modify_generic_21_P"
set ic22 "schema_modify_generic_22_P"
set ic23 "schema_modify_generic_23_P"
set ic24 "schema_modify_generic_24_P"
set ic25 "schema_modify_generic_25_P"
set ic26 "schema_modify_generic_26_P"
set ic27 "schema_modify_generic_27_P"
set ic28 "schema_modify_generic_28_P"
set ic29 "schema_modify_generic_29_P"
set ic30 "schema_modify_generic_30_P"
set ic31 "schema_modify_generic_31_P"
set ic32 "schema_modify_generic_32_P"
set ic33 "schema_modify_generic_33_P"
set ic34 "schema_modify_generic_34_P"
set ic35 "schema_modify_generic_35_P"
set ic36 "schema_modify_generic_36_P"
set ic37 "schema_modify_generic_37_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
