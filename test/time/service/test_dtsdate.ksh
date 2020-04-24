#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test_dtsdate.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:52:56  marty
# 	Add OSF copyright
# 	[1996/03/09  20:30:22  marty]
#
# Revision 1.1.2.1  1995/12/11  23:06:18  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/06/29  11:42 UTC  mgm
# 	change SVR_NODE to svr_node
# 
# 	HP revision /main/HPDCE02/3  1995/06/27  13:50 UTC  pare
# 	Merge changes
# 
# 	HP revision /main/pare_mothra/2  1995/06/27  13:48 UTC  pare
# 	Replace server=fric with server=$SVR_NODE
# 
# 	HP revision /main/HPDCE02/2  1995/06/27  12:20 UTC  root
# 	remove hard-coded name
# 
# 	HP revision /main/HPDCE02/1  1995/06/07  18:31 UTC  pare
# 	Merge directory
# 
# 	HP revision /main/pare_mothra/1  1995/06/07  17:26 UTC  pare
# 	create initial version
# 	[1995/12/11  22:44:44  root]
# 
# $EndLog$
#
tet_startup="startup"
tet_cleanup="cleanup"
iclist="ic1"
ic1="test_dtsdate"
tet_testlist="test_dtsdate"
host=`hostname`
server=$svr_node
startup()
{
   tet_infoline "START DCE dtsdate: ${TET_PNAME}; DATE: `date`"
   return
}

test_dtsdate()
{

   FAIL=0
   tet_infoline "Testing dtsdate command line options"

   dtsdate > /dev/null 2>&1
   if [[ $? != 0  ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate failed"
   fi
   
   dtsdate -s $server  > /dev/null 2>&1
   if [[ $? != 0  ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -s $server failed"
   fi
   
   dtsdate -q $server  > /dev/null 2>&1
   if [[ $? != 0  ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -q $server failed"
   fi
   
   dtsdate -u $server  > /dev/null 2>&1
   if [[ $? != 0  ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -u $server failed"
   fi
   
   dtsdate $server  > /dev/null 2>&1
   if [[ $? != 0  ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate $server failed"
   fi
   
   dtsdate -w > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -w should have failed"
   fi

   dtsdate -s > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -s should have failed"
   fi

   dtsdate -u > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -u should have failed"
   fi

   dtsdate -q > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -q should have failed"
   fi

   dtsdate -qt $server > /dev/null 2>&1
   if [[ $? != 0 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -qt $server failed"
   fi

   dtsdate -st $server > /dev/null 2>&1
   if [[ $? != 0 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -st $server failed"
   fi

   dtsdate -t $server > /dev/null 2>&1
   if [[ $? != 0 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -t $server failed"
   fi

   dtsdate -ut $server > /dev/null 2>&1
   if [[ $? != 0 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -ut $server failed"
   fi

   dtsdate -t junk > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsdate -t junk should have failed"
   fi

   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 1 "TEST_DTSDATE" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 1 "TEST_DTSDATE" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}

cleanup()
{
   tet_infoline "END DCE dtsdate test: ${TET_PNAME}; DATE: `date`"
}
. $TET_ROOT/lib/ksh/tcm.ksh
