#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# @HP_COPYRIGHT@
#
# HISTORY
# $Log: test_dts_provider.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:52:53  marty
# 	Add OSF copyright
# 	[1996/03/09  20:30:20  marty]
#
# Revision 1.1.2.1  1995/12/11  23:06:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  19:06 UTC  mgm
# 	merge HPDCE01 to HPDCE02
# 
# 	HP revision /main/HPDCE01/2  1994/06/22  18:23 UTC  pare
# 	.
# 
# 	HP revision /main/pare_rodan/2  1994/06/22  18:12 UTC  pare
# 	remove /etc/rc.dts reference for rodan
# 
# 	HP revision /main/HPDCE01/1  1994/03/30  19:11 UTC  pare
# 
# 	HP revision /main/pare_rodan/1  1994/03/30  19:10 UTC  pare
# 	initial file creation
# 	[1995/12/11  22:37:21  root]
# 
# $EndLog$
#
# This test checks the command line parameter range checking feature of the dts time
# providers, namely: dts_null_provider, dts_ntp_provider, and dts_spectracom_provider.
# In each case, out-of-range command line parameters are passed to the various time
# providers. The expected behavior is "FAIL". 
#
tet_startup="startup"
tet_cleanup="cleanup"
iclist="ic1 ic2 ic3 ic4"
ic1="test_dts_null_provider"
ic2="test_dts_ntp_provider"
ic3="test_dts_spectracom_provider"
ic4="test_dts_provider_diag"
tet_testlist="test_dts_null_provider test_dts_ntp_provider test_dts_spectracom_provider test_dts_provider_diag"

startup()
{
   tet_infoline "START DCE dts provider functional test: ${TET_PNAME}; DATE: `date`"
   return
}

test_dts_null_provider()
{

   FAIL=0
   tet_infoline "Testing dts_null_provider command line options"

   dts_null_provider -p 1 -i 5 -o 1 -d 1 > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_null_provider -p 1 -i 5 -o 1 -d 1 should have failed"
   fi
   
   dts_null_provider -p 2 -i 5 -o 1 -d 1 > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_null_provider -p 2 -i 5 -o 1 -d 1 should have failed"
   fi

   dts_null_provider -p 2 -i 10 -o 1 -d k > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_null_provider -p 2 -i 10 -o 1 -d k should have failed"
   fi

   dts_null_provider -p 60 -i 5 -o 1 -d 1 -z 1 > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_null_provider -p 60 -i 5 -o 1 -d 1  -z 1 should have failed"
   fi
   
   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 1 "TEST_DTS_NULL_PROVIDER" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 1 "TEST_DTS_NULL_PROVIDER" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}

test_dts_ntp_provider()
{
   FAIL=0
   tet_infoline "Testing dts_ntp_provider command line options"

   dts_ntp_provider -h fric -p 30 -t 1 -c 0 -o 1 -d 1 > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_ntp_provider -h fric -p 30 -t 1 -c 0 -o 1 -d 1 should have failed"
   fi

   dts_ntp_provider -h fric -p 60 -i 30 -t 4 -c 1 -o 1 -d g  > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_ntp_provider -h fric -p 60 -i 30 -t 4 -c 1 -o 1 -d g should have failed"
   fi

   dts_ntp_provider -h snarfblatt -p 6 -i 30 -t 4 -c 0 -o 1 -d 1 -z 1  > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_ntp_provider -h snarfblatt -p 6 -i 30 -t 4 -c 0 -o 1 -d 1 -z 1  should have failed"
   fi

   dts_ntp_provider -h snarfblatt -p 1 -i 30 -t 4 -c 0 -o 1 -d k > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_ntp_provider -h snarfblatt -p 1 -i 30 -t 4 -c 0 -o 1 -d k should have failed"
   fi

   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 2 "TEST_DTS_NTP_PROVIDER" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 2 "TEST_DTS_NTP_PROVIDER" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}

test_dts_spectracom_provider()
{
   FAIL=0
   tet_infoline "Testing dts_spectracom_provider command line options"

   dts_spectracom_provider -d /dev/tty00 -p 1 -i 10 -t 4 -c 1 -o 1  > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_spectracom_provider -d /dev/tty00 -p 1 -i 10 -t 4 -c 1 -o 1 should have failed"
   fi

   dts_spectracom_provider -d /dev/tty00 -p 30 -i 10 -t 4 -c 1 -o k  > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_spectracom_provider -d /dev/tty00 -p 30 -i 10 -t 4 -c 1 -o k should have failed"
   fi

   dts_spectracom_provider -d /dev/tty00 -p 3 -i 10 -t 4 -c 1 -o 1 -z 1  > /dev/null 2>&1
   if [[ $? != 1 ]]; then
      FAIL=1
      tet_infoline "ERROR: dts_spectracom_provider -d /dev/tty00 -p 3 -i 10 -t 4 -c 1 -o 1 -z 1 should have failed"
   fi

   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 3 "TEST_DTS_SPECTRACOM_PROVIDER" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 3 "TEST_DTS_SPECTRACOM_PROVIDER" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}

test_dts_provider_diag()
{
   grep server /opt/dcelocal/etc/dts_commands  >/dev/null 2>&1
   if [[ $? = 0 ]]; then
      FAIL=0
      tet_infoline "Testing time providers in diagnostic mode"

      dts_null_provider >/dev/null 2>&1 <<!
      1
      30
      1
      k
!
      if [[ $? != 1 ]]; then
         FAIL=1
         tet_infoline "ERROR: dts_null_provider -p 1 -i 30 -o 1 -d k should have failed"
      fi

      dts_ntp_provider >/dev/null 2>&1 <<!
      snarfblatt
      1
      30
      4
      0
      1
      k
!
      if [[ $? != 1 ]]; then
         FAIL=1
         tet_infoline "ERROR: dts_ntp_provider -h snarfblatt -p 1 -i 30 -t 4 -c 0 -o 1 -d k should have failed"
      fi

      dts_spectracom_provider >/dev/null 2>&1 <<!
      /dev/tty00
      1
      10
      0
      k
!
      if [[ $? != 1 ]]; then
         FAIL=1
         tet_infoline "ERROR: dts_spectracom_provider -d /dev/tty00 -p 1 -i 10 -t 4 -c 1 -o 1 should have failed"
      fi


      if [[ $FAIL = 0 ]]; then
         print "PASSED, Test 4 "TEST_DTS_PROVIDER_DIAG" : test ran successfully"
         tet_result PASS
      else
         print "FAILED, Test 4 "TEST_DTS_PROVIDER_DIAG" : test ran unsuccessfully"
         tet_result FAIL
      fi
      tet_infoline "Test completed (`date`)"

  else
      tet_infoline "Cannot test time providers in diagnostic mode -- host confgured as dts clerk."
  fi
}

cleanup()
{
   tet_infoline "END DCE dts provider functional test: ${TET_PNAME}; DATE: `date`"
}
. $TET_ROOT/lib/ksh/tcm.ksh
