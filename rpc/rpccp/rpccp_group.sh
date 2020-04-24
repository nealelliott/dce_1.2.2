#!/bin/csh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: rpccp_group.sh,v $
# Revision 1.1.4.2  1996/03/09  23:26:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:00  marty]
#
# Revision 1.1.4.1  1995/12/08  00:15:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:44  root]
# 
# Revision 1.1.2.3  1993/01/03  22:37:26  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:54:25  bbelch]
# 
# Revision 1.1.2.2  1992/12/23  19:41:36  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:55:15  zeliff]
# 
# Revision 1.1  1992/01/19  03:14:14  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# Set up some environment variables
#
setenv RPC_DEFAULT_ENTRY_SYNTAX $1
setenv NLSPATH $2/%N
#
# set up the tests
#
rpccp remove entry nstest/entry1 >& /dev/null
rpccp remove entry nstest/entry2 >& /dev/null
rpccp remove entry nstest/entry3 >& /dev/null
#
# run the tests
#
rpccp add_member nstest/group1 -m nstest/entry1
rpccp add_member nstest/group1 -m nstest/entry2
rpccp add_member nstest/group1 -m nstest/entry3
rpccp add_member nstest/group2 -m nstest/entry1
rpccp add_member nstest/group -m nstest/group1
rpccp add_member nstest/group -m nstest/group2
rpccp show group nstest/group1 -m nstest/entry2
rpccp show group nstest/group1 -m nstest/entry1
rpccp show group nstest/group1 -m nstest/entry3
rpccp show group -m nstest/entry2 nstest/group1 
rpccp show group -m nstest/group1 nstest/group1 
rpccp show group -r2 nstest/group
rpccp add entry nstest/entry1
rpccp add entry nstest/entry2
rpccp add entry nstest/entry3
rpccp show group -r2 nstest/group
