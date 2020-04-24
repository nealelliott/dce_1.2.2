#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test_dtsd.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:52:54  marty
# 	Add OSF copyright
# 	[1996/03/09  20:30:21  marty]
#
# Revision 1.1.2.1  1995/12/11  23:06:17  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  19:06 UTC  mgm
# 	merge HPDCE01 to HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/03/30  19:08 UTC  pare
# 
# 	HP revision /main/pare_rodan/1  1994/03/30  19:04 UTC  pare
# 	initial file creaton
# 	[1995/12/11  22:37:21  root]
# 
# $EndLog$
#
tet_startup="startup"
tet_cleanup="cleanup"
iclist="ic1 ic2"
ic1="test_dtsd"
ic2="test_dtstimed"
tet_testlist="test_dtsd test_dtstimed"

startup()
{
   tet_infoline "START DCE dtsd/dtstimed functional test: ${TET_PNAME}; DATE: `date`"
   return
}

test_dtsd()
{

   FAIL=0
   tet_infoline "Testing dtsd command line options"

   dtsd -ds > /dev/null 2>&1
   if [[ $? != 2 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsd -ds should have failed"
   fi
   
   dtsd -x > /dev/null 2>&1
   if [[ $? != 2 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtsd -x should have failed"
   fi

   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 1 "TEST_DTSD" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 1 "TEST_DTSD" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}

test_dtstimed()
{
   FAIL=0
   tet_infoline "Testing dtstimed command line options"

   dtstimed -x > /dev/null 2>&1
   if [[ $? = 0 ]]; then
      FAIL=1
      tet_infoline "ERROR: dtstimed -x should have failed"
   fi

   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 2 "TEST_DTSTIMED" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 2 "TEST_DTSTIMED" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}


cleanup()
{
   tet_infoline "END DCE dtsd/dtstimed test: ${TET_PNAME}; DATE: `date`"
}
. $TET_ROOT/lib/ksh/tcm.ksh
