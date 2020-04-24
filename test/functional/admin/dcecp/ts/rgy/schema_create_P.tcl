#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_create_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:51  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:18  root
# 	Submit
# 	[1995/12/11  14:31:08  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:44  root]
# 
# Revision 1.1.2.2  1994/09/30  16:22:02  truitt
# 	Add some new test cases.
# 	[1994/09/30  15:44:55  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:44:18  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:51  melman]
# 
# Revision 1.1.2.6  1994/09/23  17:01:44  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:58:22  truitt]
# 
# Revision 1.1.2.5  1994/09/21  15:26:06  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:56:17  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:44  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:43:49  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:47  truitt
# 	OT11693 & others: update tests to reflect new functionality.
# 	[1994/08/17  18:43:26  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:23  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:41  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:23:14  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:32:34  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_create_1_P {} {
    # Identify the test
    tet_infoline schema_create_1_P: Create schema name of type integer in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_1_P -encoding integer -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_1_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_1_P}] != 0} {return;}
}

proc schema_create_2_P {} {
    # Identify the test
    tet_infoline schema_create_2_P: Create schema name of type integer in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_2_P -attribute {{encoding integer} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_2_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_2_P}] != 0} {return;}
}

proc schema_create_3_P {} {
    # Identify the test
    tet_infoline schema_create_3_P: Create schema name of type any in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_3_P -encoding any -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_3_P] encoding
    } compare_strings any

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_3_P}] != 0} {return;}
}

proc schema_create_4_P {} {
    # Identify the test
    tet_infoline schema_create_4_P: Create schema name of type any in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_4_P -attribute {{encoding any} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_4_P] encoding
    } compare_strings any

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_4_P}] != 0} {return;}
}

proc schema_create_5_P {} {
    # Identify the test
    tet_infoline schema_create_5_P: Create schema name of type void in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_5_P -encoding void -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_5_P] encoding
    } compare_strings void

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_5_P}] != 0} {return;}
}

proc schema_create_6_P {} {
    # Identify the test
    tet_infoline schema_create_6_P: Create schema name of type void in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_6_P -attribute {{encoding void} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_6_P] encoding
    } compare_strings void

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_6_P}] != 0} {return;}
}

proc schema_create_7_P {} {
    # Identify the test
    tet_infoline schema_create_7_P: Create schema name of type printstring in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_7_P -encoding printstring -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_7_P] encoding
    } compare_strings printstring

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_7_P}] != 0} {return;}
}

proc schema_create_8_P {} {
    # Identify the test
    tet_infoline schema_create_8_P: Create schema name of type printstring in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_8_P -attribute {{encoding printstring} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_8_P] encoding
    } compare_strings printstring

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_8_P}] != 0} {return;}
}

proc schema_create_9_P {} {
    # Identify the test
    tet_infoline schema_create_9_P: Create schema name of type stringarray in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_9_P -encoding stringarray -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_9_P] encoding
    } compare_strings stringarray

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_9_P}] != 0} {return;}
}

proc schema_create_10_P {} {
    # Identify the test
    tet_infoline schema_create_10_P: Create schema name of type stringarray in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_10_P -attribute {{encoding stringarray} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_10_P] encoding
    } compare_strings stringarray

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_10_P}] != 0} {return;}
}

proc schema_create_11_P {} {
    # Identify the test
    tet_infoline schema_create_11_P: Create schema name of type byte in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_11_P -encoding byte -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_11_P] encoding
    } compare_strings byte

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_11_P}] != 0} {return;}
}

proc schema_create_12_P {} {
    # Identify the test
    tet_infoline schema_create_12_P: Create schema name of type byte in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_12_P -attribute {{encoding byte} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_12_P] encoding
    } compare_strings byte

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_12_P}] != 0} {return;}
}

proc schema_create_13_P {} {
    # Identify the test
    tet_infoline schema_create_13_P: Create schema name of type confidential in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_13_P -encoding confidential -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_13_P] encoding
    } compare_strings confidential

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_13_P}] != 0} {return;}
}

proc schema_create_14_P {} {
    # Identify the test
    tet_infoline schema_create_14_P: Create schema name of type confidential in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_14_P -attribute {{encoding confidential} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_14_P] encoding
    } compare_strings confidential

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_14_P}] != 0} {return;}
}

proc schema_create_15_P {} {
    # Identify the test
    tet_infoline schema_create_15_P: Create schema name of type i18ndata in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_15_P -encoding i18ndata -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_15_P] encoding
    } compare_strings i18ndata

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_15_P}] != 0} {return;}
}

proc schema_create_16_P {} {
    # Identify the test
    tet_infoline schema_create_16_P: Create schema name of type i18ndata in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_16_P -attribute {{encoding i18ndata} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_16_P] encoding
    } compare_strings i18ndata

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_16_P}] != 0} {return;}
}

proc schema_create_17_P {} {
    # Identify the test
    tet_infoline schema_create_17_P: Create schema name of type uuid in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_17_P -encoding uuid -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_17_P] encoding
    } compare_strings uuid

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_17_P}] != 0} {return;}
}

proc schema_create_18_P {} {
    # Identify the test
    tet_infoline schema_create_18_P: Create schema name of type uuid in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_18_P -attribute {{encoding uuid} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_18_P] encoding
    } compare_strings uuid

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_18_P}] != 0} {return;}
}

proc schema_create_19_P {} {
    # Identify the test
    tet_infoline schema_create_19_P: Create schema name of type attrset in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_19_P -encoding attrset -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_19_P] encoding
    } compare_strings attrset

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_19_P}] != 0} {return;}
}

proc schema_create_20_P {} {
    # Identify the test
    tet_infoline schema_create_20_P: Create schema name of type attrset in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_20_P -attribute {{encoding attrset} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_20_P] encoding
    } compare_strings attrset

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_20_P}] != 0} {return;}
}

proc schema_create_21_P {} {
    # Identify the test
    tet_infoline schema_create_21_P: Create schema name of type binding in the registry schema using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_21_P -encoding binding -aclmgr {group r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_21_P] encoding
    } compare_strings binding

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_21_P}] != 0} {return;}
}

proc schema_create_22_P {} {
    # Identify the test
    tet_infoline schema_create_22_P: Create schema name of type binding in the registry schema using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_22_P -attribute {{encoding binding} {aclmgr {group r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_22_P] encoding
    } compare_strings binding

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_22_P}] != 0} {return;}
}

proc schema_create_23_P {} {
    # Identify the test
    tet_infoline schema_create_23_P: Create a basic schema name plus uuid using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_23_P -encoding integer -aclmgr {group r r r r} -uuid 004fdb0c-a4c3-11cd-8790-080009247461
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_23_P] uuid
    } compare_strings 004fdb0c-a4c3-11cd-8790-080009247461

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_23_P}] != 0} {return;}
}

proc schema_create_24_P {} {
    # Identify the test
    tet_infoline schema_create_24_P: Create a basic schema name plus uuid using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_24_P -attribute {{encoding integer} {aclmgr {group r r r r}} {uuid 16e6ed74-a4c3-11cd-8369-090009247461}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_24_P] uuid
    } compare_strings 16e6ed74-a4c3-11cd-8369-090009247461

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_24_P}] != 0} {return;}
}

proc schema_create_25_P {} {
    # Identify the test
    tet_infoline schema_create_25_P: Create a basic schema name plus a different group aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_25_P -encoding integer -aclmgr {group rc rc rc rc}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_25_P] aclmgr
    } compare_strings {{group {query rc} {update rc} {test rc} {delete rc}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_25_P}] != 0} {return;}
}

proc schema_create_26_P {} {
    # Identify the test
    tet_infoline schema_create_26_P: Create a basic schema name plus a different group aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_26_P -attribute {{encoding integer} {aclmgr {group rc rc rc rc}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_26_P] aclmgr
    } compare_strings {{group {query rc} {update rc} {test rc} {delete rc}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_26_P}] != 0} {return;}
}

proc schema_create_27_P {} {
    # Identify the test
    tet_infoline schema_create_27_P: Create a basic schema name plus a different group aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_27_P -encoding integer -aclmgr {group n - n -}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_27_P] aclmgr
    } compare_strings {{group {query n} {update -} {test n} {delete -}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_27_P}] != 0} {return;}
}

proc schema_create_28_P {} {
    # Identify the test
    tet_infoline schema_create_28_P: Create a basic schema name plus a different group aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_28_P -attribute {{encoding integer} {aclmgr {group n - n -}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_28_P] aclmgr
    } compare_strings {{group {query n} {update -} {test n} {delete -}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_28_P}] != 0} {return;}
}

proc schema_create_29_P {} {
    # Identify the test
    tet_infoline schema_create_29_P: Create a basic schema name plus a different group aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_29_P -encoding integer -aclmgr {group r r M r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_29_P] aclmgr
    } compare_strings {{group {query r} {update r} {test M} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_29_P}] != 0} {return;}
}

proc schema_create_30_P {} {
    # Identify the test
    tet_infoline schema_create_30_P: Create a basic schema name plus a different group aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_30_P -attribute {{encoding integer} {aclmgr {group r r M r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_30_P] aclmgr
    } compare_strings {{group {query r} {update r} {test M} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_30_P}] != 0} {return;}
}

proc schema_create_31_P {} {
    # Identify the test
    tet_infoline schema_create_31_P: Create a basic schema name plus a different principal aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_31_P -encoding integer -aclmgr {principal r r r D}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_31_P] aclmgr
    } compare_strings {{principal {query r} {update r} {test r} {delete D}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_31_P}] != 0} {return;}
}

proc schema_create_32_P {} {
    # Identify the test
    tet_infoline schema_create_32_P: Create a basic schema name plus a different principal aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_32_P -attribute {{encoding integer} {aclmgr {principal r r r D}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_32_P] aclmgr
    } compare_strings {{principal {query r} {update r} {test r} {delete D}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_32_P}] != 0} {return;}
}

proc schema_create_33_P {} {
    # Identify the test
    tet_infoline schema_create_33_P: Create a basic schema name plus a different principal aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_33_P -encoding integer -aclmgr {principal gruf rf r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_33_P] aclmgr
    } compare_strings {{principal {query rfug} {update rf} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_33_P}] != 0} {return;}
}

proc schema_create_34_P {} {
    # Identify the test
    tet_infoline schema_create_34_P: Create a basic schema name plus a different principal aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_34_P -attribute {{encoding integer} {aclmgr {principal gruf rf r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_34_P] aclmgr
    } compare_strings {{principal {query rfug} {update rf} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_34_P}] != 0} {return;}
}

proc schema_create_35_P {} {
    # Identify the test
    tet_infoline schema_create_35_P: Create a basic schema name plus a different organization aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_35_P -encoding integer -aclmgr {organization t t t t}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_35_P] aclmgr
    } compare_strings {{organization {query t} {update t} {test t} {delete t}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_35_P}] != 0} {return;}
}

proc schema_create_36_P {} {
    # Identify the test
    tet_infoline schema_create_36_P: Create a basic schema name plus a different principal aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_36_P -attribute {{encoding integer} {aclmgr {organization t t t t}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_36_P] aclmgr
    } compare_strings {{organization {query t} {update t} {test t} {delete t}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_36_P}] != 0} {return;}
}

proc schema_create_37_P {} {
    # Identify the test
    tet_infoline schema_create_37_P: Create a basic schema name plus a different secdirectory aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_37_P -encoding integer -aclmgr {secdirectory id i i i}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_37_P] aclmgr
    } compare_strings {{secdirectory {query id} {update i} {test i} {delete i}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_37_P}] != 0} {return;}
}

proc schema_create_38_P {} {
    # Identify the test
    tet_infoline schema_create_38_P: Create a basic schema name plus a different secdirectory aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_38_P -attribute {{encoding integer} {aclmgr {secdirectory id i i i}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_38_P] aclmgr
    } compare_strings {{secdirectory {query id} {update i} {test i} {delete i}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_38_P}] != 0} {return;}
}

proc schema_create_39_P {} {
    # Identify the test
    tet_infoline schema_create_39_P: Create a basic schema name plus a different policy aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_39_P -encoding integer -aclmgr {policy ar m ar m}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_39_P] aclmgr
    } compare_strings {{policy {query ra} {update m} {test ra} {delete m}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_39_P}] != 0} {return;}
}

proc schema_create_40_P {} {
    # Identify the test
    tet_infoline schema_create_40_P: Create a basic schema name plus a different policy aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_40_P -attribute {{encoding integer} {aclmgr {policy ar m ar m}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_40_P] aclmgr
    } compare_strings {{policy {query ra} {update m} {test ra} {delete m}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_40_P}] != 0} {return;}
}

proc schema_create_41_P {} {
    # Identify the test
    tet_infoline schema_create_41_P: Create a basic schema name plus a different replist aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_41_P -encoding integer -aclmgr {replist AI AI AI AI}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_41_P] aclmgr
    } compare_strings {{replist {query AI} {update AI} {test AI} {delete AI}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_41_P}] != 0} {return;}
}

proc schema_create_42_P {} {
    # Identify the test
    tet_infoline schema_create_42_P: Create a basic schema name plus a different replist aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_42_P -attribute {{encoding integer} {aclmgr {replist AI AI AI AI}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_42_P] aclmgr
    } compare_strings {{replist {query AI} {update AI} {test AI} {delete AI}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_42_P}] != 0} {return;}
}

proc schema_create_43_P {} {
    # Identify the test
    tet_infoline schema_create_43_P: Create a basic schema name plus a different xattrschema aclmgr using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_43_P -encoding integer -aclmgr {xattrschema r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_43_P] aclmgr
    } compare_strings {{xattrschema {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_43_P}] != 0} {return;}
}

proc schema_create_44_P {} {
    # Identify the test
    tet_infoline schema_create_44_P: Create a basic schema name plus a different xattrschema aclmgr using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_44_P -attribute {{encoding integer} {aclmgr {xattrschema r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_44_P] aclmgr
    } compare_strings {{xattrschema {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_44_P}] != 0} {return;}
}

proc schema_create_45_P {} {
    # Identify the test
    tet_infoline schema_create_45_P: Create a basic schema name specifying the aclmgr by uuid using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_45_P -encoding integer -aclmgr {06ab8f10-0191-11ca-a9e8-08001e039d7d r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_45_P] aclmgr
    } compare_strings {{policy {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_45_P}] != 0} {return;}
}

proc schema_create_46_P {} {
    # Identify the test
    tet_infoline schema_create_46_P: Create a basic schema name specifying the aclmgr by uuid using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_46_P -attribute {{encoding integer} {aclmgr {06ab8f10-0191-11ca-a9e8-08001e039d7d r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_46_P] aclmgr
    } compare_strings {{policy {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_46_P}] != 0} {return;}
}

proc schema_create_47_P {} {
    # Identify the test
    tet_infoline schema_create_47_P: Create a basic schema name with multiple aclmgr's using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_47_P -encoding integer -aclmgr {{principal r r r D} {group rc rc rc rc} {organization t t t t} {secdirectory id i i i} {policy ar m ar m}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_47_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_47_P}] != 0} {return;}
}

proc schema_create_48_P {} {
    # Identify the test
    tet_infoline schema_create_48_P: Create a basic schema name with multiple aclmgr's using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_48_P -attribute {{encoding integer} {aclmgr {{principal r r r D} {group rc rc rc rc} {organization t t t t} {secdirectory id i i i} {policy ar m ar m}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_48_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_48_P}] != 0} {return;}
}

proc schema_create_49_P {} {
    # Identify the test
    tet_infoline schema_create_49_P: Create a basic schema name with all flags set yes using switches.
    tet_infoline schema_create_49_P: (except reserved, because you cannot delete it).

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_49_P -encoding integer -aclmgr {group r r r r} -unique yes -multivalued yes -reserved no -applydefs yes
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_49_P] unique
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_49_P}] != 0} {return;}
}

proc schema_create_50_P {} {
    # Identify the test
    tet_infoline schema_create_50_P: Create a basic schema name with all flags set yes using attribute names
    tet_infoline schema_create_50_P: (except reserved, because you cannot delete it).

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_50_P -attribute {{encoding integer} {aclmgr {group r r r r}} {unique yes} {multivalued yes} {reserved no} {applydefs yes}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_50_P] multivalued
    } compare_strings yes

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_50_P}] != 0} {return;}
}

proc schema_create_51_P {} {
    # Identify the test
    tet_infoline schema_create_51_P: Create a basic schema name with all flags set no using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_51_P -encoding integer -aclmgr {group r r r r} -unique no -multivalued no -reserved no -applydefs no
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_51_P] reserved
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_51_P}] != 0} {return;}
}

proc schema_create_52_P {} {
    # Identify the test
    tet_infoline schema_create_52_P: Create a basic schema name with all flags set no using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_52_P -attribute {{encoding integer} {aclmgr {group r r r r}} {unique no} {multivalued no} {reserved no} {applydefs no}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_52_P] applydefs
    } compare_strings no

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_52_P}] != 0} {return;}
}

proc schema_create_53_P {} {
    # Identify the test
    tet_infoline schema_create_53_P: Create a basic schema name with intercell type accept using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_53_P -encoding integer -aclmgr {group r r r r} -intercell accept
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_53_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_53_P}] != 0} {return;}
}

proc schema_create_54_P {} {
    # Identify the test
    tet_infoline schema_create_54_P: Create a basic schema name with intercell type accept using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_54_P -attribute {{encoding integer} {aclmgr {group r r r r}} {intercell accept}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_54_P] intercell
    } compare_strings accept

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_54_P}] != 0} {return;}
}

proc schema_create_55_P {} {
    # Identify the test
    tet_infoline schema_create_55_P: Create a basic schema name with intercell type reject using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_55_P -encoding integer -aclmgr {group r r r r} -intercell reject
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_55_P] intercell
    } compare_strings reject

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_55_P}] != 0} {return;}
}

proc schema_create_56_P {} {
    # Identify the test
    tet_infoline schema_create_56_P: Create a basic schema name with intercell type reject using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_56_P -attribute {{encoding integer} {aclmgr {group r r r r}} {intercell reject}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_56_P] intercell
    } compare_strings reject

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_56_P}] != 0} {return;}
}

proc schema_create_57_P {} {
    # Identify the test
    tet_infoline schema_create_57_P: Create a basic schema name with intercell type evaluate using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_57_P -encoding integer -aclmgr {group r r r r} -intercell evaluate -trigbind {none {/.:/blech}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_57_P] intercell
    } compare_strings evaluate

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_57_P}] != 0} {return;}
}

proc schema_create_58_P {} {
    # Identify the test
    tet_infoline schema_create_58_P: Create a basic schema name with intercell type evaluate using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_58_P -attribute {{encoding integer} {aclmgr {group r r r r}} {intercell evaluate} {trigbind {none {/.:/blech}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_58_P] intercell
    } compare_strings evaluate

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_58_P}] != 0} {return;}
}

proc schema_create_59_P {} {
    # Identify the test
    tet_infoline schema_create_59_P: Create a basic schema name with trig type none using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_59_P -encoding integer -aclmgr {group r r r r} -trigtype none
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_59_P] trigtype
    } compare_strings none

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_59_P}] != 0} {return;}
}

proc schema_create_60_P {} {
    # Identify the test
    tet_infoline schema_create_60_P: Create a basic schema name with trig type none using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_60_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigtype none}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_60_P] trigtype
    } compare_strings none

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_60_P}] != 0} {return;}
}

proc schema_create_61_P {} {
    # Identify the test
    tet_infoline schema_create_61_P: Create a basic schema name with trig type query using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_61_P -encoding integer -aclmgr {group r r r r} -trigtype query -trigbind {none {/.:/blech}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_61_P] trigtype
    } compare_strings query

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_61_P}] != 0} {return;}
}

proc schema_create_62_P {} {
    # Identify the test
    tet_infoline schema_create_62_P: Create a basic schema name with trig type query using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_62_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigtype query} {trigbind {none {/.:/blech}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_62_P] trigtype
    } compare_strings query

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_62_P}] != 0} {return;}
}

proc schema_create_63_P {} {
    # Identify the test
    tet_infoline schema_create_63_P: Create a basic schema name with trig type update using switches
    tet_infoline schema_create_63_P: Note: at this time security does not support trig type update.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_63_P -encoding integer -aclmgr {group r r r r} -trigtype query -trigbind {none {/.:/blech}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_63_P] trigtype
    } compare_strings query

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_63_P}] != 0} {return;}
}

proc schema_create_64_P {} {
    # Identify the test
    tet_infoline schema_create_64_P: Create a basic schema name with trig type update using attribute names
    tet_infoline schema_create_64_P: Note: at this time security does not support trig type update.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_64_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigtype query} {trigbind {none {/.:/blech}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_64_P] trigtype
    } compare_strings query

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_64_P}] != 0} {return;}
}

proc schema_create_65_P {} {
    # Identify the test
    tet_infoline schema_create_65_P: Create a basic schema name with trig bind of none using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_65_P -encoding integer -aclmgr {group r r r r} -trigbind {none {/.:/gumby}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_65_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_65_P}] != 0} {return;}
}

proc schema_create_66_P {} {
    # Identify the test
    tet_infoline schema_create_66_P: Create a basic schema name with trig bind of none using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_66_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {none {/.:/gumby}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_66_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_66_P}] != 0} {return;}
}

proc schema_create_67_P {} {
    # Identify the test
    tet_infoline schema_create_67_P: Create a basic schema name with trig bind of none using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_67_P -encoding integer -aclmgr {group r r r r} -trigbind {none {ncacn_ip_tcp 10.29.58.00 2001}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_67_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_67_P}] != 0} {return;}
}

proc schema_create_68_P {} {
    # Identify the test
    tet_infoline schema_create_68_P: Create a basic schema name with trig bind of none using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_68_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {none {ncacn_ip_tcp 10.29.58.00 2001}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_68_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_68_P}] != 0} {return;}
}

proc schema_create_69_P {} {
    # Identify the test
    tet_infoline schema_create_69_P: Create a basic schema name with trig bind of none using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_69_P -encoding integer -aclmgr {group r r r r} -trigbind {none {{/.:/gumby} {ncacn_ip_tcp 10.29.58.00 2001}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_69_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_69_P}] != 0} {return;}
}

proc schema_create_70_P {} {
    # Identify the test
    tet_infoline schema_create_70_P: Create a basic schema name with trig bind of none using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_70_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {none {{/.:/gumby} {ncacn_ip_tcp 10.29.58.00 2001}}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_70_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_70_P}] != 0} {return;}
}

proc schema_create_71_P {} {
    # Identify the test
    tet_infoline schema_create_71_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_71_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech default default none} {/.:/gumby}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_71_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_71_P}] != 0} {return;}
}

proc schema_create_72_P {} {
    # Identify the test
    tet_infoline schema_create_72_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_72_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech default default none} {/.:/gumby}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_72_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_72_P}] != 0} {return;}
}

proc schema_create_73_P {} {
    # Identify the test
    tet_infoline schema_create_73_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_73_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech none none none} {{/.:/gumby} {/.:/pokey}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_73_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_73_P}] != 0} {return;}
}

proc schema_create_74_P {} {
    # Identify the test
    tet_infoline schema_create_74_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_74_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech none none none} {{/.:/gumby} {/.:/pokey}}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_74_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_74_P}] != 0} {return;}
}

proc schema_create_75_P {} {
    # Identify the test
    tet_infoline schema_create_75_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_75_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech connect public dce} {ncacn_ip_tcp 10.29.58.00 2001}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_75_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_75_P}] != 0} {return;}
}

proc schema_create_76_P {} {
    # Identify the test
    tet_infoline schema_create_76_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_76_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech connect public dce} {ncacn_ip_tcp 10.29.58.00 2001}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_76_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_76_P}] != 0} {return;}
}

proc schema_create_77_P {} {
    # Identify the test
    tet_infoline schema_create_77_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_77_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2001} {ncacn_ip_tcp 03.19.61.00 2010}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_77_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_77_P}] != 0} {return;}
}

proc schema_create_78_P {} {
    # Identify the test
    tet_infoline schema_create_78_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_78_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech call public name} {{ncacn_ip_tcp 10.29.58.00 2001} {ncacn_ip_tcp 03.19.61.00 2010}}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_78_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_78_P}] != 0} {return;}
}

proc schema_create_79_P {} {
    # Identify the test
    tet_infoline schema_create_79_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_79_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech pkt secret name} {/.:/gumby}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_79_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_79_P}] != 0} {return;}
}

proc schema_create_80_P {} {
    # Identify the test
    tet_infoline schema_create_80_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_80_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech pkt secret name} {/.:/gumby}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_80_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_80_P}] != 0} {return;}
}

proc schema_create_81_P {} {
    # Identify the test
    tet_infoline schema_create_81_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_81_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech pktinteg public none} {/.:/gumby}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_81_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_81_P}] != 0} {return;}
}

proc schema_create_82_P {} {
    # Identify the test
    tet_infoline schema_create_82_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_82_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech pktinteg public none} {/.:/gumby}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_82_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_82_P}] != 0} {return;}
}

proc schema_create_83_P {} {
    # Identify the test
    tet_infoline schema_create_83_P: Create a basic schema name with trig bind of dce using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_83_P -encoding integer -aclmgr {group r r r r} -trigbind {{dce blech pktprivacy secret name} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_83_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_83_P}] != 0} {return;}
}

proc schema_create_84_P {} {
    # Identify the test
    tet_infoline schema_create_84_P: Create a basic schema name with trig bind of dce using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_84_P -attribute {{encoding integer} {aclmgr {group r r r r}} {trigbind {{dce blech pktprivacy secret name} {{/.:/gumby} {/.:/pokey} {/.:/daffy}}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_84_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_84_P}] != 0} {return;}
}

proc schema_create_85_P {} {
    # Identify the test
    tet_infoline schema_create_85_P: Create a basic schema name with scope string using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_85_P -encoding integer -aclmgr {group r r r r} -scope "this software, by its very complexity, defends itself against being used"
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_85_P] scope
    } compare_strings {{this software, by its very complexity, defends itself against being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_85_P}] != 0} {return;}
}

proc schema_create_86_P {} {
    # Identify the test
    tet_infoline schema_create_86_P: Create a basic schema name with scope string using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_86_P -attribute {{encoding integer} {aclmgr {group r r r r}} {scope "this software, by its very complexity, defends itself against being used"}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_86_P] scope
    } compare_strings {{this software, by its very complexity, defends itself against being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_86_P}] != 0} {return;}
}

proc schema_create_87_P {} {
    # Identify the test
    tet_infoline schema_create_87_P: Create a basic schema name with comment string using switches

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_87_P -encoding integer -aclmgr {group r r r r} -annotation "this software, by its very complexity, defends itself against being used"
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_87_P] annotation
    } compare_strings {{this software, by its very complexity, defends itself against being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_87_P}] != 0} {return;}
}

proc schema_create_88_P {} {
    # Identify the test
    tet_infoline schema_create_88_P: Create a basic schema name with comment string using attribute names

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_88_P -attribute {{encoding integer} {aclmgr {group r r r r}} {annotation "this software, by its very complexity, defends itself against being used"}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_88_P] annotation
    } compare_strings {{this software, by its very complexity, defends itself against being used}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_88_P}] != 0} {return;}
}

proc schema_create_89_P {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_create_89_P: Create a new schema name in the hosts schema

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_89_P" -attribute {{encoding integer} {aclmgr {srvrconf r r r r}}}
        dcp_attrlist getval [xattrschema show "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_89_P"] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_89_P"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_create_90_P {} {
    # Identify the test
    tet_infoline schema_create_90_P: Create a basic schema name using abbreviations.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_90_P -encoding int -aclmgr {gr r r r r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_90_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_90_P}] != 0} {return;}
}

proc schema_create_91_P {} {
    # Identify the test
    tet_infoline schema_create_91_P: Create a basic schema name using abbreviations.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_91_P -attribute {{encoding int} {aclmgr {gr r r r r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_91_P] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_91_P}] != 0} {return;}
}

proc schema_create_92_P {} {
    # Identify the test
    tet_infoline schema_create_92_P: Create a schema entry using longhand aclmgr set.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_92_P -encoding int -aclmgr {group {query r} {update r} {test r} {delete r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_92_P] aclmgr
    } compare_strings {{group {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_92_P}] != 0} {return;}
}

proc schema_create_93_P {} {
    # Identify the test
    tet_infoline schema_create_93_P: Create a schema entry using longhand aclmgr set.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_93_P -encoding int -aclmgr {gr {qu r} {upd r} {te r} {del r}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_93_P] aclmgr
    } compare_strings {{group {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_93_P}] != 0} {return;}
}

proc schema_create_94_P {} {
    # Identify the test
    tet_infoline schema_create_94_P: Create a schema entry using longhand aclmgr set.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_94_P -encoding int -aclmgr {group {query r} r {te r} r}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_94_P] aclmgr
    } compare_strings {{group {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_94_P}] != 0} {return;}
}

proc schema_create_95_P {} {
    # Identify the test
    tet_infoline schema_create_95_P: Create a schema entry using longhand aclmgr set.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_95_P -attribute {{encoding int} {aclmgr {group {query r} {update r} {test r} {delete r}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_95_P] aclmgr
    } compare_strings {{group {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_95_P}] != 0} {return;}
}

proc schema_create_96_P {} {
    # Identify the test
    tet_infoline schema_create_96_P: Create a schema entry using longhand aclmgr set.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_96_P -attribute {{encoding int} {aclmgr {gr {qu r} {upd r} {te r} {del r}}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_96_P] aclmgr
    } compare_strings {{group {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_96_P}] != 0} {return;}
}

proc schema_create_97_P {} {
    # Identify the test
    tet_infoline schema_create_97_P: Create a schema entry using longhand aclmgr set.

    # Setup steps

    # Run the test
    test {xattrschema create /.:/sec/xattrschema/schema_create_97_P -attribute {{encoding int} {aclmgr {group {query r} r {te r} r}}}
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_create_97_P] aclmgr
    } compare_strings {{group {query r} {update r} {test r} {delete r}}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_97_P}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 ic97 "
set ic1 "schema_create_1_P"
set ic2 "schema_create_2_P"
set ic2 "schema_create_2_P"
set ic3 "schema_create_3_P"
set ic4 "schema_create_4_P"
set ic5 "schema_create_5_P"
set ic6 "schema_create_6_P"
set ic7 "schema_create_7_P"
set ic8 "schema_create_8_P"
set ic9 "schema_create_9_P"
set ic10 "schema_create_10_P"
set ic11 "schema_create_11_P"
set ic12 "schema_create_12_P"
set ic13 "schema_create_13_P"
set ic14 "schema_create_14_P"
set ic15 "schema_create_15_P"
set ic16 "schema_create_16_P"
set ic17 "schema_create_17_P"
set ic18 "schema_create_18_P"
set ic19 "schema_create_19_P"
set ic20 "schema_create_20_P"
set ic21 "schema_create_21_P"
set ic22 "schema_create_22_P"
set ic23 "schema_create_23_P"
set ic24 "schema_create_24_P"
set ic25 "schema_create_25_P"
set ic26 "schema_create_26_P"
set ic27 "schema_create_27_P"
set ic28 "schema_create_28_P"
set ic29 "schema_create_29_P"
set ic30 "schema_create_30_P"
set ic31 "schema_create_31_P"
set ic32 "schema_create_32_P"
set ic33 "schema_create_33_P"
set ic34 "schema_create_34_P"
set ic35 "schema_create_35_P"
set ic36 "schema_create_36_P"
set ic37 "schema_create_37_P"
set ic38 "schema_create_38_P"
set ic39 "schema_create_39_P"
set ic40 "schema_create_40_P"
set ic41 "schema_create_41_P"
set ic42 "schema_create_42_P"
set ic43 "schema_create_43_P"
set ic44 "schema_create_44_P"
set ic45 "schema_create_45_P"
set ic46 "schema_create_46_P"
set ic47 "schema_create_47_P"
set ic48 "schema_create_48_P"
set ic49 "schema_create_49_P"
set ic50 "schema_create_50_P"
set ic51 "schema_create_51_P"
set ic52 "schema_create_52_P"
set ic53 "schema_create_53_P"
set ic54 "schema_create_54_P"
set ic55 "schema_create_55_P"
set ic56 "schema_create_56_P"
set ic57 "schema_create_57_P"
set ic58 "schema_create_58_P"
set ic59 "schema_create_59_P"
set ic60 "schema_create_60_P"
set ic61 "schema_create_61_P"
set ic62 "schema_create_62_P"
set ic63 "schema_create_63_P"
set ic64 "schema_create_64_P"
set ic65 "schema_create_65_P"
set ic66 "schema_create_66_P"
set ic67 "schema_create_67_P"
set ic68 "schema_create_68_P"
set ic69 "schema_create_69_P"
set ic70 "schema_create_70_P"
set ic71 "schema_create_71_P"
set ic72 "schema_create_72_P"
set ic73 "schema_create_73_P"
set ic74 "schema_create_74_P"
set ic75 "schema_create_75_P"
set ic76 "schema_create_76_P"
set ic77 "schema_create_77_P"
set ic78 "schema_create_78_P"
set ic79 "schema_create_79_P"
set ic80 "schema_create_80_P"
set ic81 "schema_create_81_P"
set ic82 "schema_create_82_P"
set ic83 "schema_create_83_P"
set ic84 "schema_create_84_P"
set ic85 "schema_create_85_P"
set ic86 "schema_create_86_P"
set ic87 "schema_create_87_P"
set ic88 "schema_create_88_P"
set ic89 "schema_create_89_P"
set ic90 "schema_create_90_P"
set ic91 "schema_create_91_P"
set ic92 "schema_create_92_P"
set ic93 "schema_create_93_P"
set ic94 "schema_create_94_P"
set ic95 "schema_create_95_P"
set ic96 "schema_create_96_P"
set ic97 "schema_create_97_P"
set tet_startup schema_create_startup
set tet_cleanup schema_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
