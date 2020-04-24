#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: inv_test1.sh,v $
# Revision 1.1.10.1  1996/10/15  21:19:22  arvind
# 	merge tp3()  ~ loop count fix.
# 	[1996/10/14  18:56 UTC  bartf  /main/bartf_pk_fix/1]
#
# 	fix tp3() count
# 	[1996/03/11  02:07:17  marty  1.1.8.3]
#
# Revision 1.1.8.3  1996/03/11  02:07:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:40  marty]
# 
# Revision 1.1.8.2  1995/12/11  19:52:40  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:54  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:18  root]
# 
# Revision 1.1.4.1  1994/10/19  20:50:51  mob
# 	OT 12525 - Use dcecp to add ERA's to principals (instead of obsolete tool)
# 	[1994/10/19  20:44:48  mob]
# 
# Revision 1.1.2.2  1994/10/06  20:33:22  agd
# 	expand copyright
# 	[1994/10/06  14:29:13  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:27  mdf
# 	Initial checkin
# 	[1994/08/03  19:20:31  mdf]
# 
# $EndLog$
#
#

tet_startup="startup"
tet_cleanup="cleanup"

# Config variables
#
# NUM_ATTEMPTS : this is the number of invalid attempts to try 
# This is the value set in the ERA during setup
#
NUM_ATTEMPTS=5
#
# DISABLE_LOGIN_INTERVAL : this is the amount of time in seconds
# to wait before trying to login again
# This is the value set in the ERA during setup
#
DISABLE_LOGIN_INTERVAL=300
#
# NAME : this is the name of the test user
# This is the name of the principal added by rgy_edit during setup
#
NAME=testlogin
#
# GOOD_PASSWD : this is the passwd of the test user
# This is the passwd of the principal added by rgy_edit during setup
#
GOOD_PASSWD=testlogin
#
# BAD_PASSWD : this is a bad variation of the
# passwd of the principal added by rgy_edit during setup
#
BAD_PASSWD=test
#
# CELL_ADMIN : this is the name of the principal who will be able
# to run rgy_edit
#
CELL_ADMIN=cell_admin
#
# CELL_PW : this is the password of the principal who will be able
# to run rgy_edit
#
CELL_PW=-dce-
export NAME GOOD_PASSWD BAD_PASSWD DISABLE_LOGIN_INTERVAL NUM_ATTEMPTS

iclist="ic1 ic2 ic3"
ic1="inv_tp1"
ic2="inv_tp2"
ic3="inv_tp3"
tet_testlist="inv_tp1 inv_tp2 inv_tp3"

startup()
{
        MACHNAME=`uname -n`
	tet_infoline "DCE invalid login tests starting on client $MACHNAME"

	# If there is a secd replica in the cell, do not test, because it
	# may take longer to lock out the account.
	if [[ $SEC_TEST_REPLICATION = [Tt]* ]]
	then
	    tet_infoline "SEC_TEST_REPLICATION is true, tests are currently unsupported with a replica"
	    for test_purp in $tet_testlist
	    do
		tet_delete $test_purp "UNSUPPORTED: testing with replica"
	    done
	    return
	fi

        HOSTNAME=`hostname`
	export HOSTNAME 
	dce_login $CELL_ADMIN  $CELL_PW -e \
	rgy_edit -update << EOF > ./rgy.errors 2>&1
do principal
add $NAME
do group
add testgroup
do org
add testorg
do account
add $NAME -g testgroup -o testorg -pw $GOOD_PASSWD -mp $CELL_PW
exit
EOF

	dce_login $CELL_ADMIN  $CELL_PW -e \
	dcecp -c "principal modify $NAME -add {disable_time_interval $DISABLE_LOGIN_INTERVAL}"

	dce_login $CELL_ADMIN  $CELL_PW -e \
        dcecp -c "principal modify $NAME -add {max_invalid_attempts $NUM_ATTEMPTS}"

}

cleanup()
{
	tet_infoline "DCE invalid login tests done, cleanup begun on client $MACHNAME"
	dce_login $CELL_ADMIN  $CELL_PW -e\
	rgy_edit -update << EOF > ./rgy.errors 2>&1
domain principal
delete $NAME
y
domain group
delete testgroup
y
domain org
delete testorg
y
exit
EOF
	
}


inv_tp1()
{
	tet_setblock
	tet_infoline "DCE invalid login tests: test successful login"
	dce_login $NAME $GOOD_PASSWD -e ./good_test1a
	grep "FAILED" /tmp/invtest.out
        if [ $? -eq 0 ]
        then
                tet_infoline "ERROR: Login of client $NAME failed UNEXPECTEDLY"
	 	tet_infoline "FAILED: Login activity 1: not able to login"
	 	print "FAILED: Login activity 1: not able to login"
		tet_result FAIL
        else
	 	tet_infoline "Login  for $NAME passed the first time"
	 	tet_infoline "PASSED: Login activity 1: good login attempt"
	 	print "PASSED: Login activity 1: good login attempt"
		tet_result PASS
        fi
	rm /tmp/invtest.out >/dev/null 2>&1
	tet_setblock	
}

inv_tp2()
{
	tet_setblock
	tet_infoline "DCE invalid login tests: test unsuccessful login"

	dce_login $NAME $BAD_PASSWD | grep "Sorry" > /dev/null 2>&1
        if [ $? -ne 0 ]
           then
                tet_infoline "ERROR: Was able to login to account of client $NAME UNEXPECTEDLY "
	 	tet_infoline "FAILED: Login activity 2: able to login"
	 	print "FAILED: Login activity 2: able to login"
		tet_result FAIL
        else
                tet_infoline "Login of client $NAME failed as expected"
	 	tet_infoline "PASSED: Login activity 2: bad login attempt"
	 	print "PASSED: Login activity 2: bad login attempt"
		tet_result PASS
        fi
	rm /tmp/invtest.out >/dev/null 2>&1
	tet_setblock	
}


inv_tp3()
{
integer sleeptime=0
integer result=0
integer count=0
sleeptime=$DISABLE_LOGIN_INTERVAL
	tet_infoline "DCE invalid login tests starting on client $NAME"
	tet_infoline "DCE invalid login tests: Disable time is $sleeptime after $NUM_ATTEMPTS bad attempts"

	rm /tmp/invtest.out >/dev/null 2>&1
        if [[ $NUM_ATTEMPTS > 0 ]]
        then
                total=$NUM_ATTEMPTS
                while (($count <= $total))
                do
			tet_setblock
		        ((count=count+1))
			dce_login $NAME $BAD_PASSWD -e ./good_test1a |grep "Sorry" > /dev/null 2>&1
			if [ $? -eq 0 ]
	                then
                		tet_infoline "unsuccessful login count = $count"
			else
                		tet_infoline "ERROR: Was able to login to account of client $NAME UNEXPECTEDLY count = $count"
				result=result+1
			fi
			tet_setblock
                done
		tet_setblock
		dce_login $NAME $GOOD_PASSWD -e ./good_test1a |grep "Sorry" > /dev/null 2>&1
	        if [ $? -eq 0 ]
	        then
	           tet_infoline "Login of client $NAME failed as expected account is locked"
	        else
		   tet_infoline "ERROR: Login for $NAME  UNEXPECTEDLY account is not locked"
		   result=result+1
	        fi
		tet_setblock
        fi
	sleep $sleeptime
	tet_setblock
	dce_login $NAME $GOOD_PASSWD -e ./good_test1a
	grep "FAILED" /tmp/invtest.out
	if [ $? -eq 0 ]
	then
		result=result+1
	        tet_infoline "ERROR: Login of client $NAME failed UNEXPECTEDLY, after $sleeptime seconds"
	 	tet_infoline "FAILED: Login activity 3: no reset"
	 	print "FAILED: Login activity 3: no reset"
	else
		tet_infoline "Login for $NAME passed after $sleeptime seconds, account successfully reset"
	 	tet_infoline "PASSED: Login activity 3: reset account, good login"
	 	print "PASSED: Login activity 3: reset account, good login"
	fi
	if [ result -eq 0 ]
	then
		tet_result PASS
	else
		tet_result FAIL
	fi
	rm /tmp/invtest.out >/dev/null 2>&1

}

. $TET_ROOT/lib/ksh/tcm.ksh
#. ./inv_data
