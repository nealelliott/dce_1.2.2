#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: hd_cleanup.sh,v $
# Revision 1.1.2.3  1996/03/11  02:15:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:30  marty]
#
# Revision 1.1.2.2  1995/12/13  15:26:07  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  15:12 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  15:11 UTC  truitt
# 	New file.
# 	[1995/12/11  15:53:12  root]
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
