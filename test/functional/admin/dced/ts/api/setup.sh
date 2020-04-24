#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# @HP_COPYRIGHT@
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: setup.sh,v $
# Revision 1.1.2.2  1996/10/03  18:39:58  arvind
# 	Add OSF copyright
# 	[1996/08/20  19:52 UTC  mullan_s  /main/DCE_1.2.2/3]
#
# 	AIX does not support "kill -SIGUSR1", use "kill -s USR1" instead.
# 	Capture pid of dced differently (AIX doesn't like it).
# 	[1996/08/19  23:01 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
#
# 	Submit fix for CHFts19700
# 	[1996/08/14  19:46 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
#
# 	Merge test changes/fixes back to mainline
# 	[1996/08/05  18:00 UTC  gaz  /main/DCE_1.2.2/1]
#
# 	merge from latest test versions
# 	[1996/08/01  20:31 UTC  gaz  /main/gaz_dce1.2/1]
#
# 	Fix problems killing dced
# 	[1996/03/05  16:40 UTC  jss  /main/HPDCE02/3]
#
# 	fixing path and name for dced
# 	[1996/03/04  15:42 UTC  root  /main/HPDCE02/2]
#
# 	Fixed problem with tests killing dced
# 	[1996/03/20  00:44 UTC  jss  /main/HPDCE02/bissen_cma_work/1]
#
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  23:10 UTC  jss  /main/HPDCE02/1]
#
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  22:56 UTC  jss  /main/jss_dced_acl/1]
#
# 	Initial Revision.
# 	[1996/01/15  22:56 UTC  jss  /main/jss_dced_acl/1]
#
# 	Initial version
# 	[1995/10/26  21:26:59  jss  1.0]
#
# Revision 1.1.2.1  1996/08/09  20:11:15  arvind
# 	Merge from DCE_1.2
# 	[1996/08/09  19:56:58  arvind]
# 
# $EndLog$
#
#
#  Module:      setup.sh
#
#  FACILITY:    DCE Host Daemon API
#
#  ABSTRACT:    This module implements the tests for dced api
#               setup.
#
################################################################################

tet_startup="startup"
tet_cleanup="cleanup"
iclist="ic1"
ic1="setup"
testlist="$ic1"

startup ()
{
	rm -f $DCED_ACL_SAVE
	return
}

cleanup ()
{
	rm -f $DCECP_OUTPUT
	return
}

#===============================================================================
# Function:     setup
# Purpose:      Perform setup for DCED tests
#
#===============================================================================
setup ()
{
	# PERFORM SETUP

        fail=0
        tet_infoline "Begin DCED test setup."

        # Kill dced and restart to open acls

	ps -ef | grep -v grep | grep "dced.*-r" 2>&1 > /dev/null
        rc=$?
        if [[ $rc != 0 ]]
        then
	        integer pid=0
		while (true); do
        		pid=`ps -e | awk '/dced/ {print $1}'`
        		if [[ $pid -ne 0 ]]; then
                		kill $pid
		                sleep 10		
				pid=0
			else
				break
        		fi
	        done

        	# Restart dced with -r
	        ${DCELOCALBIN}/dced -r

        	rc=$?
	        if [[ $rc != 0 ]]
        	then
        		tet_infoline "ERROR: dced -r failed to restart"
	                tet_infoline "ERROR: during DCED test setup."
                	tet_result FAIL
                	fail=1
                	return
		fi
	fi

	#Put dced into local mode

        integer pid=0
        pid=`ps -ef | grep "dced.*-r" | grep -v grep | cut -c10-14`
        if [[ $pid -ne 0 ]]; then
               	kill -s USR1 $pid
	        sleep 5
	else
        	tet_infoline "ERROR: Could not find pid for dced -r"
                tet_infoline "ERROR: during DCED test setup."
                tet_result FAIL
		fail=1
		return
        fi
	
	# Modify acls

	acl_save_and_modify hostdata hosts/${LOCAL_HOST}/self \
		"{user hosts/${LOCAL_HOST}/self -riI}" ""
	acl_save_and_modify srvrconf hosts/${LOCAL_HOST}/self \
		"{user hosts/${LOCAL_HOST}/self -riI}" ""
	acl_save_and_modify srvrconf hosts/${LOCAL_HOST}/self \
		"{user hosts/${LOCAL_HOST}/self -dfrwx}" "-io"
	acl_save_and_modify srvrexec hosts/${LOCAL_HOST}/self \
		"{user hosts/${LOCAL_HOST}/self -riI}" ""

	#Put dced back into normal (non-local) mode

        kill -s USR1 $pid
        sleep 5

	# CHECK RESULTS

        if [ $fail -eq 0 ]
        then
                tet_infoline "SUCCESSFULLY completed DCED test setup."
      		tet_result PASS
        else
                tet_infoline "ERROR: during DCED test setup."
      		tet_result FAIL
        fi
}

# tcc will fail if these files don't exist and get sourced
# so there is no need to check that this happens
. $TET_ROOT/test/functional/admin/dced/ts/dced.data
. $TET_ROOT/test/functional/admin/dced/ts/dced.functions
. $TET_ROOT/lib/ksh/tcm.ksh 
