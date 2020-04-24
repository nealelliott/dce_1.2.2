#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: rpccp_clean.sh,v $
# Revision 1.1.4.2  1996/03/11  02:20:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:56:36  marty]
#
# Revision 1.1.4.1  1995/12/11  19:36:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:19:11  root]
# 
# Revision 1.1.2.2  1993/01/11  21:15:04  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  14:55:41  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:14  devrcs
# 	Initial revision
# 
# $EndLog$
#
RPC_DEFAULT_ENTRY_SYNTAX=${RPC_DEFAULT_ENTRY_SYNTAX:-3}
export RPC_DEFAULT_ENTRY_SYNTAX
rpccp remove_entry nstest/entry1
rpccp remove_entry nstest/entry2
rpccp remove_entry nstest/entry3
rpccp remove_entry nstest/entry4
rpccp remove_entry nstest/group1
rpccp remove_entry nstest/group2
rpccp remove_entry nstest/group
rpccp remove_entry nstest/profile1
rpccp remove_entry nstest/profile2
rpccp remove_entry nstest/profile3
rpccp remove_entry nstest/profile4
rpccp remove_entry nstest/profile

