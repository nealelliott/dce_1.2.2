#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_cleanup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:27:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:26  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:17  root
# 	Submit
# 	[1995/12/11  14:32:23  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:22  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:25  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:27  melman]
# 
# Revision 1.1.2.3  1994/09/20  18:22:33  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:09:39  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:08  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:01  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:30  pwang
# 	Initial Implementation
# 	[1994/06/10  21:25:34  pwang]
# 
# $EndLog$

if [ "$#" -ne 1 ]
then 
    echo "Usage: $0 hostname"
    exit 1
fi
acl_edit /.:/$1/config/srvrconf -l | grep "user:cell_admin:cri" > /dev/null
if [ "$?" -eq 0 ]
then
    acl_edit /.:/$1/config/srvrconf -d user:cell_admin
    acl_edit /.:/$1/config/srvrconf -l | grep cell_admin > /dev/null
    if [ "$?" -eq 0 ]
    then
        echo "Failed to remove acl entry user:cell_admin from srvrconf"
        exit 1
    fi
fi
acl_edit /.:/$1/config/srvrexec -l | grep "user:cell_admin:cri" > /dev/null
if [ "$?" -eq 0 ]
then
    acl_edit /.:/$1/config/srvrexec -d user:cell_admin
    acl_edit /.:/$1/config/srvrexec -l | grep cell_admin > /dev/null
    if [ "$?" -eq 0 ]
    then
        echo "Failed to remove acl entry user:cell_admin from srvrexec"
        exit 1
    fi
fi
