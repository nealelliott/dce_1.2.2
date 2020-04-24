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
# $Log: key_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:29:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:24  marty]
#
# Revision 1.1.4.1  1995/12/11  15:18:47  root
# 	Submit
# 	[1995/12/11  14:33:16  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:59  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:39  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:44:55  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:33:13  devsrc]
# 
# Revision 1.1.2.1  1994/06/03  14:05:34  ganni
# 	create the principal needed for the tests through scripts
# 	[1994/06/03  14:03:20  ganni]
# 
# $EndLog$
#
#

echo "ktdel -p dced_test_user_1 -v 1" > /tmp/rgy_k.script
echo "del -p dced_test_user_1" >> /tmp/rgy_k.script
echo "do p" >> /tmp/rgy_k.script
echo "del dced_test_user_1" >> /tmp/rgy_k.script
echo "add dced_test_user_1" >> /tmp/rgy_k.script
echo "do a" >> /tmp/rgy_k.script
echo "add dced_test_user_1 -pw dced_test_user_1 -mp $1 -g none -o none" >> /tmp/rgy_k.script
echo "ktadd -p dced_test_user_1 -pw dced_test_user_1" >> /tmp/rgy_k.script
echo "quit" >> /tmp/rgy_k.script
/opt/dcelocal/bin/rgy_edit </tmp/rgy_k.script >/tmp/rgy_k.out

