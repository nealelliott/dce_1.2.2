#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: dce_login-tc_N.sh,v $
# Revision 1.1.2.1  1996/11/14  16:13:32  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:28:52  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/21  18:19 UTC  bartf
# 	add dce_login tests
# 
# Revision /main/bartf_pk.3/1  1996/10/21  17:27 UTC  bartf
# 	dce_login tests
# 
# $EndLog$
#

# 
# Error condition tests for PK dce_login:
# 

tet_startup="startup"
tet_cleanup="cleanup"

iclist="ic1"
ic1="dce_login_tp1 dce_login_tp2 dce_login_tp3"
tet_testlist="dce_login_tp1 dce_login_tp2 dce_login_tp3"

PWD=pk_passwd
CELL_ADMIN=cell_admin
CELL_PW=-dce-
export NAME PWD CELL_ADMIN CELL_PW
export PVTKEY_FILE_PREFIX="/opt/dcelocal/var/security/pk_file/"

mtet_echo()
{
    tet_infoline $1
    print $1
}

startup()
{
    mtet_echo "dce_login-tc_N startup."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "principal create { pk_user1 pk_user2 pk_user3 }; \
              group add none -member { pk_user1 pk_user2 pk_user3 }; \
              organization add none -member { pk_user1 pk_user2 pk_user3 }"
              > ./startup_errors_N 2>&1

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: startup. See startup_errors_N file"
	for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        return
    fi

    cellname=$(getcellname)
    SIMPCELLNAME=${cellname#/.../}

    sleep 300
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./startup_errors_P 2>&1
account modify krbtgt/$SIMPCELLNAME -pkgenprivkey 512 -mypwd $CELL_PW 
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: startup acc mod krbtgt. See startup_errors_N file"
	for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        tet_result FAIL
        return
    fi


}

# 1.Issue dce_login with an invalid asymmetric key pair for a user that
#   is required to be a public key user.
#   set the pre_auth_req ERA for
#   pk_user to "3" PADATA_ENC_PUBLIC_KEY".   
#   Expected result: should fail (since using secret key password,
#   but user is required to use public key encryption, so no
#   fall-back).   Test for correct error message.

dce_login_tp1()
{
    mtet_echo "dce_login tp1: Check Unsuccessful login"

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp1_errors_N 2>&1
account create pk_user1 -group none -organization none -mypwd $CELL_PW \
-password pk_passwd -pkmechanism file \
-pksignatureusage { {gen 512} {newpassphrase pk_passphrase} } \
-pkkeycipherusage { {gen 512} {newpassphrase pk_passphrase} }
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: account create : see tp1_errors_N file"
        return
    fi

    mtet_echo "dce_login_tp1: N1: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY to princ."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp1_errors_N 2>&1    
    principal modify pk_user1 -add {pre_auth_req 3}
exit
EOF


    dce_login pk_user1 incorrect_passphrase | grep "Sorry" > /dev/null 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: tp1: N1: dce_login Succeeded Unexpectedly."
        tet_result FAIL
        return
    fi


    mtet_echo "PASSED: dce_login_tp1"
    tet_result PASS
}


# Try "account create pk_user" using 3 different passwords: one
#   for -password, another for first newpassphrase instance, third
#   for second newpassphrase instance.
#   Expected result: should fail, but I'm not sure at what point it
#   will fail.

dce_login_tp2()
{
    mtet_echo "dce_login_tp2: Check Unsuccessful login with 3 different passwd/phrases."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp2_errors_N 2>&1
account create pk_user2 -group none -organization none -mypwd $CELL_PW \
-password pk_passwd1 -pkmechanism file \
-pksignatureusage { {gen 512} {newpassphrase pk_passphrase2} } \
-pkkeycipherusage { {gen 512} {newpassphrase pk_passphrase3} }
exit
EOF

    mtet_echo "dce_login_tp2: N1: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY to princ."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp2_errors_N 2>&1    
    principal modify pk_user2 -add {pre_auth_req 3}
exit
EOF


    dce_login pk_user2 $PWD | grep "Sorry" > /dev/null 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: tp2: N1: dce_login Succeeded Unexpectedly."
        tet_result FAIL
        return
    fi

    mtet_echo "PASSED: dce_login_tp2"
    tet_result PASS
}


 
# 3. attempt to dce_login as a public key user but delete or rename the
#   user's Personal Security Module.  Test for correct error message.

dce_login_tp3()
{
    mtet_echo "dce_login tp3: Check Unsuccessful login with missing PSM."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp3_errors_N 2>&1
account create pk_user3 -group none -organization none -mypwd $CELL_PW \
-password pk_passwd -pkmechanism file \
-pksignatureusage { {gen 512} {newpassphrase pk_passphrase} } \
-pkkeycipherusage { {gen 512} {newpassphrase pk_passphrase} }
exit
EOF

    mtet_echo "dce_login_tp3: N1: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY to princ."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp3_errors_N 2>&1    
    principal modify pk_user3 -add {pre_auth_req 3}
exit
EOF

    pk_user_idstr=$(dcecp -c pri show pk_user3 | grep "{uuid ")
    pk_user_id="${pk_user_idstr#{uuid }"
    UUID_NUM="${pk_user_id%?}"
    rm $PVTKEY_FILE_PREFIX/$UUID_NUM > /dev/null 2>&1
    dce_login pk_user3 pk_passphrase | grep "Sorry" > /dev/null 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: tp3: N1: dce_login succeeded Unexpectedly."
        tet_result FAIL
        return
    fi

    mtet_echo "PASSED: dce_login_tp3"
    tet_result PASS
}


cleanup()
{
    mtet_echo "dce_login-tc_N cleanup."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "account delete { pk_user1 pk_user2 pk_user3 }; \
              principal delete {pk_user1 pk_user2 pk_user3 }" > ./cleanup_errors_N 2>&1

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: cleanup. See cleanup_errors_N file"
    fi
}

. $TET_ROOT/lib/ksh/tcm.ksh











