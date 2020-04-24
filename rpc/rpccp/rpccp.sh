#! /bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: rpccp.sh,v $
# Revision 1.1.4.2  1996/03/09  23:26:21  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:58  marty]
#
# Revision 1.1.4.1  1995/12/08  00:15:53  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:43  root]
# 
# Revision 1.1.2.3  1993/01/03  22:37:22  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:54:18  bbelch]
# 
# Revision 1.1.2.2  1992/12/23  19:41:25  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:55:08  zeliff]
# 
# Revision 1.1  1992/01/19  03:14:06  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#    $1 is RPCCP_PATH 
#    $2 is TEST_NAME.sh
#    $3 is the value of NS_ENV 
#    $4 is the value of DCE_CAT_PATH 
# where
#    NS_PATH -- one of the variable defined in DTM 
#    TEST_NAME -- a VMS DCL symbol defined in ns_ult_cmd:rpccp_setup.com
#                 at DTM control node -- lanz
#
#    NS_ENV  -- value defined for RPC_DEFAULT_ENTRY_SYNTAX
#    DCE_CAT_PATH  -- whereabouts of dcerpc.cat
cd $1
$2 $3 $4
