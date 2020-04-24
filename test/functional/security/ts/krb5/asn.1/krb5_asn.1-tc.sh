#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
# HISTORY
# $Log: krb5_asn.1-tc.sh,v $
# Revision 1.1.2.1  1996/07/09  15:01:52  arvind
# 	OSF DCE 1.2.2 Drop 3
# 	[1996/07/09  14:55:36  arvind]
#
# Revision /main/DCE_1.2.2/2  1996/06/25  19:57 UTC  mullan_s
# 	Merge to DCE_1.2.2
# 	[1996/06/20  18:19 UTC  mullan_s  /main/DCE_1.2.2/1]
# 
# Revision /main/mullan_dce_krb5_der_work/2  1996/06/25  19:55 UTC  mullan_s
# 	Diff against ref_encode.out instead of reference_encode.out
# 
# Revision /main/mullan_dce_krb5_der_work/1  1996/06/10  19:30 UTC  mullan_s
# 	TET Shell Script for ASN.1 test cases.
# 
# $EndLog$
#
# NOTE: This test works best if TET_EXEC_IN_PLACE=True so that
# temporary files used to generate test results (encode.out and
# encode.diffs) are not removed from the currrent working directory.
# If TET_EXEC_IN_PLACE=False, then in addition, set
# TET_SAVE_FILES="./encode.out ./encode.diffs" in your tetexec.cfg
# file.

tet_startup=""
tet_cleanup=""

iclist="ic1 ic2"
ic1="krb5_encode_test"
ic2="krb5_decode_test"
tet_testlist="krb5_encode_test krb5_decode_test"

myecho()
{
    tet_infoline $1
    print $1
}

krb5_encode_test()
{
    myecho "DCE Krb5 ASN.1 DER Encoding Test"

    ./krb5_encode_test > ./encode.out 2>&1
    if [ $? -ne 0 ]
    then
	myecho "FAILED: krb5_encode_test: exec of krb5_encode_test; see ./encode.out"
	tet_result FAIL
	return
    fi

    diff ./ref_encode.out ./encode.out > ./encode.diffs 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_encode_test: unexpected results; see ./encode.diffs"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_encode_test"
    tet_result PASS
}

krb5_decode_test()
{
    myecho "DCE Krb5 ASN.1 DER Decoding Test"

    ./krb5_decode_test > ./decode.out 2>&1
    if [ $? -ne 0 ]
    then
	myecho "FAILED: krb5_decode_test: exec of krb5_decode_test; see ./decode.out"
	tet_result FAIL
	return
    fi

    grep "^ERROR" ./decode.out > /dev/null 2>&1
    if [ $? -ne 1 ]
    then
        myecho "FAILED: krb5_decode_test: unexpected results; see ./decode.out"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_decode_test"
    tet_result PASS
}

. $TET_ROOT/lib/ksh/tcm.ksh
