#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: secval_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:29:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:25  marty]
#
# Revision 1.1.4.1  1995/12/11  15:19:22  root
# 	Submit
# 	[1995/12/11  14:33:25  root]
# 
# Revision 1.1.2.1  1994/09/28  21:48:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:51  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:45:13  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:34:03  devsrc]
# 
# Revision 1.1.2.1  1994/06/03  14:05:37  ganni
# 	create the principal needed for the tests through scripts
# 	[1994/06/03  14:03:21  ganni]
# 
# $EndLog$
#
echo "del -p dced_test_user" > /tmp/rgy_s.script
echo "do p" >> /tmp/rgy_s.script
echo "del dced_test_user" >> /tmp/rgy_s.script
echo "add dced_test_user" >> /tmp/rgy_s.script
echo "do a" >> /tmp/rgy_s.script
echo "add dced_test_user -pw dced_test_user -mp $1 -g none -o none" >> /tmp/rgy_s.script
echo "quit" >> /tmp/rgy_s.script
/opt/dcelocal/bin/rgy_edit </tmp/rgy_s.script >/tmp/rgy_s.out

