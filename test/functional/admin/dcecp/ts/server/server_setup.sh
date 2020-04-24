#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_setup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:28:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:44  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:51  root
# 	Submit
# 	[1995/12/11  14:32:35  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:38  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:48  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:43  melman]
# 
# Revision 1.1.2.4  1994/09/21  16:37:35  pwang
# 	Fixed typo.
# 	[1994/09/21  16:36:30  pwang]
# 
# Revision 1.1.2.3  1994/09/20  18:23:05  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:15:40  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:25  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:57  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:32  pwang
# 	Initial implementation
# 	[1994/06/10  21:32:21  pwang]
# 
# $EndLog$

if [ "$#" -ne 1 ]
then
    echo "Usage: $0 hostname"
    exit 1
fi
acl_edit /.:/$1/config/srvrconf -m user:cell_admin:criI
acl_edit /.:/$1/config/srvrconf -l | grep "user:cell_admin:cri" > /dev/null
if [ "$?" -ne 0 ]
then
    echo "Failed to add acl entry user:cell_admin:criI to srvrconf"
    exit 1
fi
acl_edit /.:/$1/config/srvrexec -m user:cell_admin:criI
acl_edit /.:/$1/config/srvrexec -l | grep "user:cell_admin:cri" > /dev/null
if [ "$?" -ne 0 ]
then
    echo "Failed to add acl entry user:cell_admin:criI to srvrexec"
    exit 1
fi
