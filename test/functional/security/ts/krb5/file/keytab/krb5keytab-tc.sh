#!/bin/ksh

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: krb5keytab-tc.sh,v $
# Revision 1.1.2.1  1996/08/09  20:18:08  arvind
# 	Merge to DCE 1.2.2
# 	[1996/07/26  21:19 UTC  mullan_s  /main/DCE_1.2.2/1]
#
# 	Check erroIImprove error checking from rgy_edit.
# 	[1996/07/26  19:27 UTC  mullan_s  /main/mullan_dce122_k5tests_1/2]
#
# 	Add some comments explaining test.
# 	[1996/07/10  15:30 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# 	TET test script for keytab compatibility tests
# 	[1996/07/10  15:30 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# $EndLog$
#
# The purpose of this test is to check that a DCE application using the
# DCE krb5 libraries can parse a krb5 keytab file in the 2 possible formats.
#

tet_startup=""
tet_cleanup=""

iclist="ic1 ic2"
ic1="krb5_kt_v1"
ic2="krb5_kt_v2"
tet_testlist="krb5_kt_v1 krb5_kt_v2"

myecho()
{
    tet_infoline $1
    print $1
}

chk_rgy_edit()
{
    grep "?(rgy_edit)" $1 >/dev/null
    if [ $? -eq 0  -o  $rgy_edit_status -ne 0 ]
    then	
	myecho "$2"
	tet_result FAIL
	return 1
    fi
    return 0
}

krb5_kt_v1()
{
    myecho "DCE Krb5 keytab compatibility test: version 1 format"

    rgy_edit >./krb5_kt_v1_errors 2>&1 <<EOF
ktlist -f ./krb5ktfilename.1
quit
EOF
    rgy_edit_status=$?
    chk_rgy_edit ./krb5_kt_v1_errors "FAILED: krb5_kt_v1: see krb5_kt_v1_errors file"
    if [ $? -ne 0 ]
    then
	return
    fi

    myecho "PASSED: krb5_kt_v1"
    tet_result PASS
}

krb5_kt_v2()
{
    myecho "DCE Krb5 keytab compatibility test: version 2 format"

    rgy_edit >./krb5_kt_v2_errors 2>&1 <<EOF
ktlist -f ./krb5ktfilename.2
quit
EOF
    rgy_edit_status=$?
    chk_rgy_edit ./krb5_kt_v2_errors "FAILED: krb5_kt_v2: see krb5_kt_v2_errors file"
    if [ $? -ne 0 ]
    then
	return
    fi

    myecho "PASSED: krb5_kt_v2"
    tet_result PASS
}

. $TET_ROOT/lib/ksh/tcm.ksh
