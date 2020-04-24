#!/bin/ksh

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: preauth-tc.sh,v $
# Revision 1.1.4.1  1996/11/14  16:15:15  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:30:08  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/31  19:36 UTC  mullan_s
# 	Merge to DCE 1.2.2
# 
# Revision /main/mullan_dce12_tp_preauth2/1  1996/10/30  22:55 UTC  mullan_s
# 	Add test for non-default salt.
# 
# Revision 1.1.2.2  1996/03/09  20:47:33  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:24  marty]
# 
# Revision 1.1.2.1  1995/12/11  19:52:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:56  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:21  root]
# 
# $EndLog$

tet_startup="startup"
tet_cleanup="cleanup"

iclist="ic1"
ic1="preauth_tp1 preauth_tp2"
tet_testlist="preauth_tp1 preauth_tp2"

#
# NAME : this is the name of the test user
#
NAME=preauth
NEW_NAME=preauth2
# CELL_ADMIN : this is the name of the principal who will be able
# to run rgy_edit
#
CELL_ADMIN=cell_admin
#
# CELL_PW : this is the password of the principal who will be able
# to run rgy_edit
#
CELL_PW=-dce-
export NAME CELL_ADMIN CELL_PW

myecho()
{
    tet_infoline $1
    print $1
}

startup()
{
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "principal create $NAME; \
              group add none -member $NAME; \
              organization add none -member $NAME"
              > ./startup_errors 2>&1

    if [ $? -ne 0 ]
    then
        myecho "FAILED: startup. See startup_errors file"
	for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        return
    fi

#
# !!! NOTE !!!
#
# dcecp does not allow the user to input passwords on the command
# line via the -c switch so we have to go back to using rgy_edit
# until we figure out a way around this.
#
    dce_login $CELL_ADMIN  $CELL_PW -e \
    rgy_edit -update << EOF >> ./startup_errors 2>&1
do account
add $NAME -g none -o none -pw test -mp $CELL_PW
exit
EOF

    if [ $? -ne 0 ]
    then
        myecho "FAILED: startup. See startup_errors file"
        for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        return
    fi
}

preauth_tp1()
{
    myecho "DCE Preauthentication test purpose 1: successful login"
    myecho "\texpecting THIRD PARTY preauth"

    dce_login $CELL_ADMIN $CELL_PW -e \
    dcecp -c "principal modify $NAME -add {pre_auth_req 2}" > ./tp1_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: preauth_tp1: see tp1_errors file"
        tet_result FAIL
        return
    fi

    dce_login $NAME test -e kdestroy >> ./tp1_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: preauth_tp1: see tp1_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: preauth_tp1"
    tet_result PASS
}

preauth_tp2()
{
    myecho "DCE Preauthentication test purpose 2: non-default salt"

    dce_login $CELL_ADMIN $CELL_PW -e \
    dcecp -c "principal rename $NAME -to $NEW_NAME" > ./tp2_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: preauth_tp2: see tp2_errors file"
        tet_result FAIL
        return
    fi

    dce_login $NEW_NAME test -e kdestroy >> ./tp2_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: preauth_tp2: see tp2_errors file"
        tet_result FAIL
        return
    fi

    dce_login $CELL_ADMIN $CELL_PW -e \
    dcecp -c "principal rename $NEW_NAME -to $NAME" >> ./tp2_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: preauth_tp2: see tp2_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: preauth_tp2"
    tet_result PASS
}

cleanup()
{
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "account delete $NAME; \
              principal delete $NAME" > ./cleanup_errors 2>&1

    if [ $? -ne 0 ]
    then
        myecho "FAILED: cleanup. See cleanup_errors file"
    fi
}

. $TET_ROOT/lib/ksh/tcm.ksh
