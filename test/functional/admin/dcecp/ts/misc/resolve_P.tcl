#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: resolve_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:43  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:59  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/29  13:21 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts47/1  1995/09/29  13:18 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 	[1995/12/11  15:53:59  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:11  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:24  melman]
# 
# 	OT 9938 modified string binding aquisition sequence
# 	[1994/09/22  19:40:21  kevins]
# 
# 	OT 9938 Initial submission of resolve positive test
# 	[1994/06/30  19:10:14  kevins]
# 
# Revision 1.1.2.2  1994/09/22  20:04:06  kevins
# 	OT 9938 modified binding aquisition
# 	[1994/09/22  20:01:09  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

#
# This special proc queries the endpoint map to find binding/interface 
# and endpoint information required by the resolve positive testcase.
#

proc fri {protocol_sequence interface ip endpnt} {
    upvar 1 protocol_sequence prot_seq
    upvar 1 interface inter
    upvar 1 endpnt ep 
    upvar 1 ip inter_prot

    foreach working [endpoint show] {
        set binding [lindex [attrlist getvalues $working -type binding] 0]
        set inter [lindex [attrlist getvalues $working -type interface] 0]
        set annot [lindex [attrlist getvalues $working -type annotation] 0]
        if {$annot == "Endpoint Resolution"} {
            set prot_seq [lindex $binding 0]
            set inter_prot [lindex $binding 1]
            set ep [lindex $binding 2]
            return
        }
    }
}


## Test purposes
proc resolve_5_P {} {
    set protocol_sequence ""
    set ip ""
    set interface ""
    set endpnt ""

    # Identify the test
    tet_infoline resolve_5_P: Verify that we can get a fully bound binding 
    tet_infoline resolve_5_P: handle from a partially bound one.

    # Setup steps
    fri protocol_sequence interface ip endpnt
    set resolve [concat $protocol_sequence $ip $endpnt]

    # Run the test
    test {
        resolve "$protocol_sequence $ip" -interface $interface
    } compare_strings $resolve
}


set iclist "ic1 "
set ic1 "resolve_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
