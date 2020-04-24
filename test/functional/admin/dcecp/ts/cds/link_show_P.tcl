#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:15  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:46  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:18  melman]
# 
# Revision 1.1.2.5  1994/09/02  18:42:13  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:23  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:00  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:45  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:16  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:18  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:29  melman
# 	updating to new API path
# 	[1994/05/06  15:20:22  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:16  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:54  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc link_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc link_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}
## Test purposes
proc link_show_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline link_show_3_P: Check that we can show the attributes of a softlink in cds 
    tet_infoline link_show_3_P: namespace.

    # Setup steps
    if {[dcp_wrap {link create /.:/linksh -to /.:/hosts }] != 0} {return;}

    # Run the test
    test { dcp_attrlist getval [link show /.:/linksh] CDS_LinkTarget
         } dcp_find_pgo hosts

    # Cleanup steps
    if {[dcp_wrap {link delete /.:/linksh}] != 0} {return;}
}

proc link_show_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW 

    # Identify the test
    tet_infoline link_show_4_P: Check that we can show the attributes from a list of softlinks
    tet_infoline link_show_4_P: in the cds namespace.

    # Setup steps
    set LINKONE [name expand /.:/hosts]
    set LINKTWO [name expand /.:/subsys]
    if {[dcp_wrap {link create /.:/linksh_2 -to $LINKONE }] != 0} {return;}
    if {[dcp_wrap {link create /.:/linksh_1 -to $LINKTWO}] != 0} {return;}

    # Run the test
    test { attrlist getvalues [link show {/.:/linksh_1 /.:/linksh_2}] -type CDS_LinkTarget 
         } compare_strings "$LINKTWO\n$LINKONE"

    # Cleanup steps
    if {[dcp_wrap {link delete {/.:/linksh_1 /.:/linksh_2}}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "link_show_3_P"
set ic2 "link_show_4_P"

set tet_startup link_startup
set tet_cleanup link_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
