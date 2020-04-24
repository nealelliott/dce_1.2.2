#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: po_setup.sh,v $
# Revision 1.1.7.2  1996/03/11  02:35:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:49  marty]
#
# Revision 1.1.7.1  1995/12/11  21:49:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:40  root]
# 
# Revision 1.1.5.4  1993/02/25  22:07:22  cuti
# 	Add "site -u" to rgy_edit commands to make sure working on master site.
# 	[1993/02/25  19:05:04  cuti]
# 
# Revision 1.1.5.3  1993/01/22  21:57:03  sekhar
# 	[OT 4887]  remove /tmp/rgy_edit.script first before usage.
# 	[1993/01/22  21:44:09  sekhar]
# 
# Revision 1.1.5.2  1992/12/31  17:31:21  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:56:41  htf]
# 
# Revision 1.1.2.3  1992/05/22  20:21:51  cuti
# 	removed /sys5.3 from /bin/sh
# 	[1992/05/22  20:10:40  cuti]
# 
# Revision 1.1.2.2  1992/05/21  21:32:57  cuti
# 	 Setup some accounts for passwd_override test.
# 
# 	 Setup some accounts for testing passwd_override
# 	[1992/05/21  21:13:53  cuti]
# 
# $EndLog$

#set -x 
   
rgy_setup_init()
{
echo "site -u" >> /tmp/rgy_edit.script
echo "do p" >> /tmp/rgy_edit.script
echo "add $1 $2 -f \"$3\"" >> /tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add $1 -g $4 -o none -mp -dce- -pw $5 " >>/tmp/rgy_edit.script
}


/bin/rm -f /tmp/rgy_edit.script
touch /tmp/rgy_edit.script

rgy_setup_init test1 1050 "fred flintstone" tcb -dce-
rgy_setup_init test2 1051 "fred flintstone" tcb -dce-
rgy_setup_init test3 1052 "fred flintstone" tcb -dce-
rgy_setup_init test4 1053 "fred flintstone" none -dce-
rgy_setup_init test5 1054 "fred flintstone" none -dce-
rgy_setup_init zxcvb 1055 "fred flintstone" none -dce-
rgy_setup_init asdf  1056 "fred flintstone" none -dce-
rgy_setup_init poiuy 1057 "fred flintstone" none -dce-
rgy_setup_init asasdfhgjl 1058 "fred flintstone" none -dce-
echo "q" >>/tmp/rgy_edit.script


/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script
/bin/rm -f /tmp/rgy_edit.script


EXIT=$?

exit $EXIT

