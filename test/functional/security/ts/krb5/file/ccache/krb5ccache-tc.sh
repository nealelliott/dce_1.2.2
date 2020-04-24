#!/bin/ksh

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: krb5ccache-tc.sh,v $
# Revision 1.1.2.1  1996/08/09  20:17:43  arvind
# 	Merge to DCE 1.2.2
# 	[1996/07/26  19:25 UTC  mullan_s  /main/mullan_dce122_k5tests_1/2]
#
# 	Add some comments explaining test.
# 	[1996/07/01  19:34 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# 	Krb5 ccache compatibility tests.
# 	[1996/07/01  19:34 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# $EndLog$
#
# The purpose of this test is to check that a krb5 application using the 
# DCE libraries can parse a krb5 credentials file in the 3 possible formats.
#

tet_startup=""
tet_cleanup=""

iclist="ic1 ic2 ic3"
ic1="krb5_cc_v1"
ic2="krb5_cc_v2"
ic3="krb5_cc_v3"
tet_testlist="krb5_cc_v1 krb5_cc_v2 krb5_cc_v3"

myecho()
{
    tet_infoline $1
    print $1
}

krb5_cc_v1()
{
    myecho "DCE Krb5 ccache compatibility test: version 1 format"

    KRB5CCNAME="FILE:./krb5ccfilename.1"
    klist > ./krb5_cc_v1_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_cc_v1: see krb5_cc_v1_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_cc_v1"
    tet_result PASS
}

krb5_cc_v2()
{
    myecho "DCE Krb5 ccache compatibility test: version 2 format"

    KRB5CCNAME="FILE:./krb5ccfilename.2"
    klist > ./krb5_cc_v2_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_cc_v2: see krb5_cc_v2_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_cc_v2"
    tet_result PASS
}

krb5_cc_v3()
{
    myecho "DCE Krb5 ccache compatibility test: version 3 format"

    KRB5CCNAME="FILE:./krb5ccfilename.3"
    klist > ./krb5_cc_v3_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_cc_v3: see krb5_cc_v3_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_cc_v3"
    tet_result PASS
}

. $TET_ROOT/lib/ksh/tcm.ksh
