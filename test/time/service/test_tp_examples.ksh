#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# @OSF_COPYRIGHT@
# 
# HISTORY
# $Log: test_tp_examples.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:52:58  marty
# 	Add OSF copyright
# 	[1996/03/09  20:30:23  marty]
#
# Revision 1.1.2.1  1995/12/11  23:06:19  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/08/28  20:13 UTC  pare
# 	Merge service directory files
# 
# 	HP revision /main/pare_MOTHRA/1  1995/08/28  13:30 UTC  pare
# 	Create new element
# 	[1995/12/11  22:45:22  root]
# 
# $EndLog$
#
tet_startup="startup"
tet_cleanup="cleanup"
iclist="ic1"
ic1="test_tp_examples"
tet_testlist="test_tp_examples"

examples_dir=/opt/dcelocal/usr/examples/dts

if [[ ! -d ${examples_dir} ]]; then
     examples_dir=/opt/dce/examples/dts
fi

startup()
{
   tet_infoline "START TEST test_tp_examples: ${TET_PNAME}; DATE: `date`"
   return
}

test_tp_examples()
{

   FAIL=0
   tet_infoline "Testing dts time provider examples"
   cd $examples_dir
   /bin/make -fn Makefile.tp > /dev/null 2>&1
   if [[ $? != 0  ]]; then
      FAIL=1
      tet_infoline "ERROR: test_tp_examples failed"
   fi
   cd -

   if [[ $FAIL = 0 ]]; then
      print "PASSED, Test 1 "test_tp_examples" : test ran successfully"
      tet_result PASS
   else
      print "FAILED, Test 1 "test_tp_examples" : test ran unsuccessfully"
      tet_result FAIL
   fi
   tet_infoline "Test completed (`date`)"
}

cleanup()
{
   tet_infoline "END DCE test_tp_examples: ${TET_PNAME}; DATE: `date`"
}
. $TET_ROOT/lib/ksh/tcm.ksh
