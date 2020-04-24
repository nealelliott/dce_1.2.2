#!/bin/ksh

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: krb5kinit-tc.sh,v $
# Revision 1.1.2.1  1996/08/09  20:18:33  arvind
# 	Merge to DCE 1.2.2
# 	[1996/07/26  19:23 UTC  mullan_s  /main/mullan_dce122_k5tests_1/4]
#
# 	Add some comments explaining test.
# 	[1996/07/26  19:20 UTC  mullan_s  /main/mullan_dce122_k5tests_1/3]
#
# 	Delete keytab file in cleanup.
# 	[1996/07/10  18:07 UTC  mullan_s  /main/mullan_dce122_k5tests_1/2]
#
# 	Add more comments.
# 	[1996/07/10  14:50 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# 	TET TestCase for kinit tests.
# 	[1996/07/10  14:50 UTC  mullan_s  /main/mullan_dce122_k5tests_1/1]
#
# $EndLog$
#
# The purpose of this test is to check that renewable and proxiable TGTs can
# be obtained from a DCE KDC. It does this by using a stock MIT krb5 kinit
# application and checking the ticket flags displayed by the klist program.
#
# To Do:
#	Extend tests so that they use the renewable/proxiable TGTs.
#

tet_startup="krb5_kinit_startup"
tet_cleanup="krb5_kinit_cleanup"

iclist="ic1 ic2"
ic1="krb5_kinit_renew"
ic2="krb5_kinit_prox"
tet_testlist="krb5_kinit_renew krb5_kinit_prox"

myecho()
{
    tet_infoline $1
    print $1
}

krb5_kinit_startup()
{
    if [ ! -f /usr/bin/dce_login_noexec ]
    then
        ln -s /usr/bin/dce_login /usr/bin/dce_login_noexec
    fi

    export KRB5CCNAME=`dce_login_noexec cell_admin -dce- 2>./krb5_kinit_startup_errors`
    if [ $? -ne 0 ]
    then
	myecho "krb5_kinit_startup failed: see krb5_kinit_startup_errors file for more info"
        for test_purp in $tet_testlist
        do
            tet_delete $test_purp "krb5_kinit_startup(): dce_login has failed."
        done
	return
    fi

    # Add a keytable key for cell_admin to avoid kinit password prompting
    rgy_edit -update >./krb5_kinit_startup_errors 2>&1 <<EOF
ktadd -f ./krb5ktfilename -p cell_admin -pw -dce-
quit
EOF
    if [ $? -ne 0 ]
    then
	myecho "krb5_kinit_startup failed: see krb5_kinit_startup_errors file for more info"
        for test_purp in $tet_testlist
        do
            tet_delete $test_purp "krb5_kinit_startup(): adding key has failed."
        done
	return
    fi
}

krb5_kinit_cleanup()
{
    # Delete keytable
    rm -f ./krb5ktfilename
    kdestroy
}

krb5_kinit_renew()
{
    myecho "MIT Krb5 kinit w/DCE KDC test: renewable tickets"

    ./kinit -r 8h -k -t ./krb5ktfilename cell_admin >./krb5_kinit_renew_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_kinit_renew: see krb5_kinit_renew_errors file"
        tet_result FAIL
        return
    fi
    ./klist -f | grep "Flags: .*R.*" >./krb5_kinit_renew_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_kinit_renew: see krb5_kinit_renew_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_kinit_renew"
    tet_result PASS
}

krb5_kinit_prox()
{
    myecho "MIT Krb5 kinit w/DCE KDC test: proxiable tickets"

    # Allow cell_admin to acquire proxiable tickets
    echo "account modify cell_admin -proxiabletkt yes" | dcecp >./krb5_kinit_prox_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_kinit_prox: see krb5_kinit_prox_errors file"
        tet_result FAIL
        return
    fi

    ./kinit -p -k -t ./krb5ktfilename cell_admin >./krb5_kinit_prox_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_kinit_prox: see krb5_kinit_prox_errors file"
        tet_result FAIL
        return
    fi
    ./klist -f | grep "Flags: .*P.*" >./krb5_kinit_prox_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: krb5_kinit_prox: see krb5_kinit_prox_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: krb5_kinit_prox"
    tet_result PASS
}

. $TET_ROOT/lib/ksh/tcm.ksh
