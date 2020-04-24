#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: good_test1a.sh,v $
# Revision 1.1.4.2  1996/03/11  02:07:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:38  marty]
#
# Revision 1.1.4.1  1995/12/11  19:52:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:53  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:17  root]
# 
# Revision 1.1.2.2  1994/10/06  20:32:58  agd
# 	expand copyright
# 	[1994/10/06  14:29:05  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:23  mdf
# 	Initial checkin
# 	[1994/08/03  19:20:21  mdf]
# 
# $EndLog$
#

#. $TET_ROOT/lib/ksh/tetapi.ksh
#. $TET_ROOT/tetexec.cfg

#tet_setcontext
this_fail=0

/opt/dcelocal/bin/klist >KOUT
PRIN=`cat KOUT|grep Principal|grep -v Global|awk '{print $3}'`
#tet_infoline "PRIN: $PRIN"

if [ $? != 0 ]
then
       this_fail=1
fi

/opt/dcelocal/bin/kdestroy

if [ $this_fail -ne 0 ]
then
	print "FAILED" > /tmp/invtest.out
        exit 1
else
	print "PASSED" > /tmp/invtest.out
        exit 0
fi
