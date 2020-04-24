#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# @HP_COPYRIGHT@
#
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: cleanup.sh,v $
# Revision 1.1.2.2  1996/10/03  18:38:37  arvind
# 	Add OSF copyright
# 	[1996/08/20  19:52 UTC  mullan_s  /main/DCE_1.2.2/3]
#
# 	Submit fix for CHFts19700
# 	[1996/08/15  22:22 UTC  mullan_s  /main/DCE_1.2.2/2]
#
# 	AIX does not support "kill -SIGUSR1", use "kill -s USR1" instead.
# 	[1996/08/14  19:45 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
#
# 	Merge test changes/fixes back to mainline
# 	[1996/08/05  18:00 UTC  gaz  /main/DCE_1.2.2/1]
#
# 	merge from latest test versions
# 	[1996/08/01  20:31 UTC  gaz  /main/gaz_dce1.2/1]
#
# 	Merge fix for international scenario and test killing dced.
# 	[1996/03/20  01:21 UTC  jss  /main/HPDCE02/2]
#
# 	Fixed problem with tests killing dced
# 	[1996/03/20  00:46 UTC  jss  /main/HPDCE02/bissen_cma_work/1]
#
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  23:10 UTC  jss  /main/HPDCE02/1]
#
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  22:55 UTC  jss  /main/jss_dced_acl/1]
#
# 	Initial Revision.
# 	[1996/01/15  22:55 UTC  jss  /main/jss_dced_acl/1]
#
# 	Initial version
# 	[1995/10/26  21:26:59  jss  1.0]
#
# Revision 1.1.2.1  1996/08/09  20:10:54  arvind
# 	Merge from DCE_1.2
# 	[1996/08/09  19:56:48  arvind]
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
ic1="dced_cleanup"
testlist="$ic1"

startup ()
{
	return
}

cleanup ()
{
	rm -f $DCECP_OUTPUT
	rm -f $DCED_ACL_SAVE
	return
}

#===============================================================================
# Function:     dced_cleanup
# Purpose:      Perform cleanup for DCED tests
#
#===============================================================================
dced_cleanup ()
{

	# PERFORM CLEANUP

        fail=0
        tet_infoline "Begin DCED test cleanup."

	#Put dced into local mode

        integer pid=0
        pid=`ps -ef | grep "dced.*-r" | grep -v grep | cut -c10-14`
        if [[ $pid -ne 0 ]]; then
               	kill -s USR1 $pid
	        sleep 10
	else
        	tet_infoline "ERROR: Could not find pid for dced -r"
                tet_infoline "ERROR: during DCED test cleanup."
                tet_result FAIL
		fail=1
		return
        fi
	
	# Restore acls to their pre-test state

	acl_restore

	#Put dced back into normal (non-local) mode

        kill -s USR1 $pid

	# CHECK RESULTS

        if [ $fail -eq 0 ]
        then
                tet_infoline "SUCCESSFULLY completed DCED test cleanup."
      		tet_result PASS
        else
                tet_infoline "ERROR: during DCED test cleanup."
      		tet_result FAIL
        fi
}

# tcc will fail if these files don't exist and get sourced
# so there is no need to check that this happens
. $TET_ROOT/test/functional/admin/dced/ts/dced.data
. $TET_ROOT/test/functional/admin/dced/ts/dced.functions
. $TET_ROOT/lib/ksh/tcm.ksh 
