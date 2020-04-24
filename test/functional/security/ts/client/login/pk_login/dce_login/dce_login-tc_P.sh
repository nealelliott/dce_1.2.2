#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: dce_login-tc_P.sh,v $
# Revision 1.1.2.1  1996/11/14  16:13:45  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:29:03  arvind]
#
# Revision /main/DCE_1.2.2/5  1996/10/29  19:22 UTC  bartf
# 	merge tp6 fix
# 
# Revision /main/DCE_1.2.2/bartf_pk_fix/1  1996/10/29  19:20 UTC  bartf
# 	fix tp6 filenames
# 
# Revision /main/DCE_1.2.2/4  1996/10/28  18:18 UTC  bartf
# 	merge fix tp3, tp7
# 
# 	tp3 fixes
# 	[1996/10/24  20:52 UTC  bartf  /main/DCE_1.2.2/3]
# 	Revision /main/bartf_pk.3/5  1996/10/28  18:15 UTC  bartf
# 	fix tp3 pwd
# 
# Revision /main/bartf_pk.3/3  1996/10/24  18:08 UTC  bartf
# 	test fixes
# 
# Revision /main/bartf_pk.3/2  1996/10/21  17:27 UTC  bartf
# 	enhance tests
# 
# $EndLog$
#

#
# Valid tests for PK dce_login:
#

tet_startup="startup"
tet_cleanup="cleanup"

iclist="ic1 ic2 ic3 ic4 ic5 ic6 ic7"
ic1="dce_login_tp1"
ic2="dce_login_tp2"
ic3="dce_login_tp3"
ic4="dce_login_tp4"
ic5="dce_login_tp5"
ic6="dce_login_tp6"
ic7="dce_login_tp7"

tet_testlist="dce_login_tp1 dce_login_tp2 dce_login_t3 dce_login_t4 dce_login_tp5 dce_login_tp6 dce_login_tp7" 

NAME=test_dce_login_pk_user
PWD=pk_passwd
CELL_ADMIN=cell_admin
CELL_PW=-dce-
export NAME PWD CELL_ADMIN CELL_PW

mtet_echo()
{
    tet_infoline $1
    print $1
}

startup()
{
    mtet_echo "dce_login test startup."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "principal create { pk_user1 pk_user2 pk_user3 pk_user4 pk_user5 pk_user6 pk_user7}; \
              group add none -member { pk_user1 pk_user2 pk_user3 pk_user4 pk_user5 pk_user6 pk_user7 }; \
              organization add none -member { pk_user1 pk_user2 pk_user3 pk_user4 pk_user5 pk_user6 pk_user7}"
              > ./startup_errors_P 2>&1

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: startup. See startup_errors_P file"
	for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        tet_result FAIL
        return
    fi

    cellname=$(getcellname)
    SIMPCELLNAME=${cellname#/.../}
    mtet_echo "cellname: $SIMPCELLNAME" >> ./startup_errors_P 2>&1
    mtet_echo "waiting 5 minutes for cache refresh..." 
    sleep 300

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./startup_errors_P 2>&1
account modify krbtgt/$SIMPCELLNAME -pkgenprivkey 512 -mypwd $CELL_PW 
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: startup acc mod krbtgt. See startup_errors_P file"
	for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        tet_result FAIL
        return
    fi

}

# 1.Issue dce_login with a valid asymmetric key pair and test for
#  successful login.  Go on to issue basic DCE commands (ie. klist)
#  to ensure further commands function normally.

dce_login_tp1()
{
    mtet_echo "dce_login tp1: Check Successful PK login."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp1_errors_P 2>&1
account create pk_user1 -group none -organization none -mypwd $CELL_PW \
 -password pk_passwd -pkmechanism file \
 -pksignatureusage { {gen 512} {newpassphrase pk_passwd} } \
 -pkkeycipherusage { {gen 512} {newpassphrase pk_passwd} }
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: account create : see tp1_errors_P file"
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp1: principal modify pk_user1 -add {pre_auth_req 3}" >> ./tp1_errors_P 2>&1
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp1_errors_P 2>&1    
    principal modify pk_user1 -add {pre_auth_req 3}
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: principal modify: see tp1_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp1: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY to princ." >> ./tp1_errors_P 2>&1
    dce_login pk_user1 $PWD -e kdestroy >> ./tp1_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: dce_login_tp1 : see tp1_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "PASSED: dce_login_tp1"
    tet_result PASS
}


# 2.Issue dce_login with an invalid asymmetric key pair for a user
#  not required to use public key pre-authentication.  Check to see
#  that sec_login code will fall back to the existing symmetric key
#  password based authentication.

dce_login_tp2()
{
    mtet_echo "dce_login tp2: Check Successful login"

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp1_errors_P 2>&1
account create pk_user2 -group none -organization none -mypwd $CELL_PW -password pk_passwd 
exit
EOF

    dce_login pk_user2 $PWD -e kdestroy >> ./tp2_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: dce_login_tp2 kdestroy : see tp2_errors_P file"
        tet_result FAIL
        return
    fi

    mtet_echo "PASSED: dce_login_tp2"
    tet_result PASS
}


# 3.Issue dce_login for successful login. Go on to modify users account
#    password to ensure it can be done by the user themselves.
# Try dce_login with both "pk_passwd" (secret key password) AND
#   with "new_pk_passwd" (new PK passphrase).  Both cases should work
#   as long as pre_auth_req is not set or is set to a value <
#   PADATA_ENC_PUBLIC_KEY (3).

dce_login_tp3()
{
    mtet_echo "dce_login tp3: P1. Check Successful login and user account modify to changekeys."

    mtet_echo "dce_login tp3: P. cell_admin create account and modify xattr so that user can change their key."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp3_errors_P 2>&1
account create pk_user3 -group none -organization none -mypwd $CELL_PW -password pk_passwd -pkmechanism file -pksignature { {gen 512} {newpassphrase pk_passwd} } -pkkeycipher { {gen 512} {newpassphrase pk_passwd} }
xattrschema modify /.:/sec/xattrschema/DCEPKAuthentication \
     -aclmgr {principal r u r m} 
xattrschema modify /.:/sec/xattrschema/DCEPKKeyEncipherment \
     -aclmgr {principal r u r m} 
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: account create or xattr modify: see tp3_errors_P file."
        tet_result FAIL
        return
    fi

    dce_login pk_user3 $PWD -e \
    dcecp << EOF >> ./tp3_errors_P 2>&1
account modify pk_user3 -mypwd pk_passwd \
-pksignature { {gen 512} {oldpassphrase pk_passwd} {newpassphrase new_pk_passphrase} } \
-pkkeycipher { {gen 512} {oldpassphrase pk_passwd} {newpassphrase new_pk_passphrase} }
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: dce_login_tp3: account modify - see tp3_errors_P file"
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp3: P2. Check Successful login with password."
    dce_login pk_user3 $PWD -e kdestroy >> ./tp3_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: dce_login_tp3: dce_login $PWD - see tp3_errors_P file"
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp3: P3: check successful login with new passphrase."
    dce_login pk_user3 new_pk_passphrase -e kdestroy >> ./tp3_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: dce_login_tp3 for dce_login new_pk_passphrase - see tp3_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "PASSED: dce_login_tp3"
    tet_result PASS
}


#Additional cases for PK Login tests

#  Try "account create pk_userX" using 2 different passwords: one
#   for -password, another for both newpassphrase instances.
#   Log in supplying the "newpassphrase" password.
#   Expected result: should be OK

dce_login_tp4()
{
    mtet_echo "dce_login_tp4: Check Successful acct create and login with different passwd and passphrase."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp4_errors_P 2>&1
account create pk_user4 -group none -organization none -mypwd $CELL_PW \
-password pk_passwd -pkmechanism file \
-pksignatureusage { {gen 512} {newpassphrase pk_passphrase} } \
-pkkeycipherusage { {gen 512} {newpassphrase pk_passphrase} }
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp4: account create - see tp4_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp4: P1: Check Successful login passphrase."
    dce_login pk_user4 pk_passphrase -e kdestroy >> ./tp4_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp4: P1: dce_login pk_pass - see tp4_errors_P file."
        tet_result FAIL
    fi

    mtet_echo "dce_login_tp4: P2: Check Successful login password."
    dce_login pk_user4 pk_passwd -e kdestroy >> ./tp4_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp4: P2: dce_login pk_pass - see tp4_errors_P file."
        tet_result FAIL
    fi

    mtet_echo "PASSED: dce_login_tp4"
    tet_result PASS
}



# 1.Try "account create pk_userX" using 2 different passwords: one
#   for -password, another for both newpassphrase instances.
#3. As in 1., but before logging in, set the pre_auth_req ERA for
#   pk_userX to "3" PADATA_ENC_PUBLIC_KEY".

#5. As in 1., but before logging in, set the pre_auth_req ERA for
#   pk_userX to "2" PADATA_ENC_THIRD_PARTY".
#   
#   Expected result: should be OK (3rd party is lower than public
#   key, so pk login is OK)

dce_login_tp5()
{
    mtet_echo "dce_login_tp5: Check Successful acct create with different passwd and passphrase."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp5_errors_P 2>&1
account create pk_user5 -group none -organization none -mypwd $CELL_PW \
-password pk_passwd -pkmechanism file \
-pksignature { {gen 512} {newpassphrase pk_passphrase} } \
-pkkeycipher { {gen 512} {newpassphrase pk_passphrase} }
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp5: account create - see tp5_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp5: P1: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY."  >> ./tp5_errors_P 2>&1
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp5_errors_P 2>&1    
    principal modify pk_user5 -add {pre_auth_req 3}
exit
EOF

    mtet_echo "dce_login_tp5: P1: Check Successful login passphrase."
    dce_login pk_user5 pk_passphrase -e kdestroy >> ./tp5_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp5: P1: dce_login pk_pass - see tp5_errors_P file."
        tet_result FAIL
    fi


    mtet_echo "dce_login_tp5: P2: add  pre_auth_req ERA 2 PADATA-ENC-THIRD-PARTY."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp5_errors_P 2>&1    
    principal modify pk_user5 -change {pre_auth_req 2}
exit
EOF

    mtet_echo "dce_login_tp5: P2: check successful login passphrase."
    dce_login pk_user5 pk_passphrase -e kdestroy >> ./tp5_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp5: P2: dce_login pk_passphrase - see tp5_errors_P file."
        tet_result FAIL
        
    fi

#   pre_auth_req ERA is set for
#   pk_user to "2" PADATA_ENC_THIRD_PARTY".
#   then login
#   Expected result: should be OK (falls back to secret key, which is
#   OK since 3rd party is allowed)

    mtet_echo "dce_login_tp5: P3: Check Successful login password."
    dce_login pk_user5 pk_passwd -e kdestroy >> ./tp5_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp5: P3: dce_login pk_passwd - see tp5_errors_P file."
        tet_result FAIL
    fi


    tet_result PASS
}


#  Issue dce_login with a valid asymmetric key pair, gotten from key file
#  and test for successful login.
#  Go on to issue basic DCE commands (ie. klist)
#  to ensure further commands function normally.

dce_login_tp6()
{
    mtet_echo "dce_login tp6: Check Successful PK login with pk keyfiles."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp6_errors_P 2>&1
account create pk_user6 -group none -organization none -mypwd $CELL_PW \
 -password pk_passwd -pkmechanism file \
 -pksignatureusage { {public pk_pubkeyfile} {private pk_pvtkeyfile} {newpassphrase pk_passwd} } \
 -pkkeycipherusage { {public pk_pubkeyfile} {private pk_pvtkeyfile} {newpassphrase pk_passwd} }
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: account create : see tp6_errors_P file"
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp6: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY to princ."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp6_errors_P 2>&1    
    principal modify pk_user6 -add {pre_auth_req 3}
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: principal modify: see tp6_errors_P file."
        tet_result FAIL
        return
    fi

    dce_login pk_user6 $PWD -e kdestroy >> ./tp6_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: dce_login_tp6 : see tp6_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "PASSED: dce_login_tp6"
    tet_result PASS
}


#  wait five minutes FIRST, to allow   KDC public key cache to get stale, then
#  Do a valid PK login (verify success),
#  then issue dcecp> account modify krbtgt/<cell> -pkgenprivkey <modulus>
#  to change KDC public key, then try to do valid PK login again.
#  Expected result: should succeed.  On second login, two attempts
#  should be made: first attempt using the stale KDC public key
#  cached by dced on the first login.  that will fail because KDC
#  public key has changed.  sec_login should make second attempt
#  after getting fresh KDC public key.

dce_login_tp7()
{
    mtet_echo "dce_login tp7: Check Successful acct create with pk_pass."
    mtet_echo "wait 5 minutes..."
    sleep 300

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF > ./tp7_errors_P 2>&1
account create pk_user7 -group none -organization none -mypwd $CELL_PW \
-password pk_pass -pkmechanism file \
-pksignature { {gen 512} {newpassphrase pk_pass} } \
-pkkeycipher { {gen 512} {newpassphrase pk_pass} }
exit
EOF
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp7: account create - see tp7_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp7: P1: add  pre_auth_req ERA 3 PADATA-ENC_PUBLIC_KEY to princ."
    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp7_errors_P 2>&1    
    principal modify pk_user7 -add {pre_auth_req 3}
exit
EOF


    mtet_echo "dce_login_tp7: P1: Check Successful 1st login passwd."
    dce_login pk_user7 pk_pass -e kdestroy>> ./tp7_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp7: P1: dce_login pk_pass - see tp7_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp7: P1: account modify krbtgt."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp << EOF >> ./tp7_errors_P 2>&1
account modify krbtgt/${SIMPCELLNAME} -pkgenprivkey 512 
exit
EOF

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp7: P1: account modify - see tp7_errors_P file."
        tet_result FAIL
        return
    fi

    mtet_echo "dce_login_tp7: P1: Check Successful 2nd login passwd."
    dce_login pk_user7 pk_pass -e kdestroy >> ./tp7_errors_P 2>&1
    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED tp7: P1: 2nd dce_login pk_pass - see tp7_errors_P file."
        tet_result FAIL
    fi
 

    mtet_echo "PASSED: dce_login_tp7"
    tet_result PASS
}


cleanup()
{
    mtet_echo "dce_login-tc_P cleanup."

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "account delete { pk_user1 pk_user2 pk_user3 pk_user4 pk_user5 pk_user6 pk_user7 }; \
              principal delete { pk_user1 pk_user2 pk_user3 pk_user4 pk_user5 pk_user6 pk_user7 }" > ./cleanup_errors_P 2>&1

    if [ $? -ne 0 ]
    then
        mtet_echo "FAILED: cleanup. See cleanup_errors_P file"
    fi
}

. $TET_ROOT/lib/ksh/tcm.ksh


