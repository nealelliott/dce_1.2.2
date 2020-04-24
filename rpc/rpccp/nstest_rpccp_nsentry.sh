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
# $Log: nstest_rpccp_nsentry.sh,v $
# Revision 1.1.4.2  1996/03/09  23:26:19  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:56  marty]
#
# Revision 1.1.4.1  1995/12/08  00:15:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:39  root]
# 
# Revision 1.1.2.3  1993/01/03  22:37:13  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:54:04  bbelch]
# 
# Revision 1.1.2.2  1992/12/23  19:41:01  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:54:53  zeliff]
# 
# Revision 1.1  1992/01/19  03:14:12  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# Set up some environment variables
#
setenv RPC_DEFAULT_ENTRY_SYNTAX 3
setenv NLSPATH $2/%N
setenv obj1 fbe696e0-6dae-11c9-b093-08002b102989
setenv obj2 02d52fc0-6daf-11c9-b958-08002b102989
setenv obj3 f62512c0-c747-11c9-9884-08002b13ec4e
setenv b1 ncacn_dnet_nsp:4.133'[22]'
setenv if11 ec658c20-c9a3-11c9-905e-08002b13ec4e,0000.0000

# export objects, so that inquires can be done on them"
echo "SETUP: export objects, so that inquires can be done on them"
rpccp export nstest/entry_test1  -o obj1 -o obj2
rpccp export nstest/entry_test2  -o obj3
rpccp export nstest/entry_test3  -i if11 -b b1


# TEST 1: for rpc_ns_entry_object_inq_begin where the RPC_DEFAULT_ENTRY_SYNTAX
#         is used for the entry_name_syntax.
#
echo "Test 1:"
rpccp show_server nstest/entry_test1 -o

#
# TEST 2: for rpc_ns_entry_object_inq_begin where the object_uuid is not found
#         NOTE *  rpccp will not return the status rpc_s_obj_uuid_not_found, 
#         because that status is translated in the actual shell to 
#         >>> no objects
#
echo "Test 2:"
rpccp show_server nstest/entry_test3 -o 

#
# TEST 3: for rpc_ns_entry_object_inq_begin where the name service entry is 
#         not found
#
echo "Test 3:"
rpccp show_server nstest/entry_test99

#
# TEST 4: for rpc_ns_entry_object_inq_begin where the there are no more
#         members. Note- you will not see the status no more members,
#         the operation will just show any members present, and stop.
#

echo "Test 4:"
rpccp show_server nstest/entry_test2 -o

#
# TEST 5: demonstrate use of unexport with -o option
#
echo "Test 5:"
rpccp show server nstest/entry_test1 -o 
rpccp unexport nstest/entry_test1 -o obj2
#
# we expect no errors here and obj2 should be gone
#
rpccp show server nstest/entry_test1 -o 
rpccp show server nstest/entry_test1 -o obj1
# end of tests
# Cleanup-  Rather than unexport the entries, we'll just delete them 
#          (remove_entry)
#
rpccp remove_entry nstest/entry_test1  
rpccp remove_entry nstest/entry_test2  
rpccp remove_entry nstest/entry_test3
# end of file
