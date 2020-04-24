#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: hd_setup.sh,v $
# Revision 1.1.2.3  1996/03/11  02:15:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:36  marty]
#
# Revision 1.1.2.2  1995/12/13  15:26:10  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  20:27 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  20:21 UTC  truitt
# 	New file to help hostdata setup.
# 	[1995/12/11  15:53:15  root]
# 
# $EndLog$

if [ "$#" -ne 2 ]
then
    echo "Usage: $0 cellname hostname"
    exit 1
fi

acl_edit $1/hosts/$2/config/hostdata -io -m any_other:cdprw
if [ "$?" -ne 0 ]
then
    echo "Failed to modify acl entry any_other:cdprw"
    exit 1
fi
