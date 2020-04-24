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
# $Log: nstest_rpccp_profile.sh,v $
# Revision 1.1.4.2  1996/03/09  23:26:20  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:57  marty]
#
# Revision 1.1.4.1  1995/12/08  00:15:48  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:40  root]
# 
# Revision 1.1.2.3  1993/01/03  22:37:15  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:54:08  bbelch]
# 
# Revision 1.1.2.2  1992/12/23  19:41:06  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:54:57  zeliff]
# 
# Revision 1.1  1992/01/19  03:14:05  devrcs
# 	Initial revision
# 
# $EndLog$
#
# Set up some environment variables
#
setenv RPC_DEFAULT_ENTRY_SYNTAX $1
setenv NLSPATH $2/%N
setenv if11 7581fd80-bed4-11c9-ace1-08002b102989,0000.0000
setenv if22 85172300-bed5-11c9-a49c-08002b102989,0000.0000
setenv if33 a98241c0-bed4-11c9-874c-08002b102989,0001.0000
setenv if44 af0bcc60-bed4-11c9-bf08-08002b102989,0001.0000
setenv if66 39774a80-c6b2-11c9-b723-08002b13ec4e,0001.0000

# add elements, so that inquiries, and removes can be done on them
echo "SETUP: add elements, so that inquiries, and removes can be done on them"
rpccp add_element nstest/profile1 -i if11 -m nstest/entry1 -a IF1 -p 0
rpccp add_element nstest/profile2 -i if22 -m nstest/mbr1 -a IF2 -p 0
rpccp add_element nstest/profile2 -i if22 -m nstest/mbr2 -a IF2 -p 0
rpccp add_element nstest/profile3 -i if33 -m nstest/entry2 -a IF3 -p 0
rpccp add_element nstest/profile5 -i if44 -m nstest/entry5 -a IF5 -p 0
setenv if33 a98241c0-bed4-11c9-874c-08002b102989,0001.0001
rpccp add_element nstest/profile3 -i if33 -m nstest/entry3 -a IF6 -p 0
rpccp add_element nstest/profile99 -i if33 -m nstest/mbr0 -a IF7 -p 0
rpccp add_element nstest/profile6 -i if66 -m nstest/membr1 -a IF8 -p 0
setenv if66 39774a80-c6b2-11c9-b723-08002b13ec4e,0001.0001
rpccp add_element nstest/profile6 -i if66 -m nstest/membr2 -a IF8 -p 0
rpccp add_element nstest/profile4 -d -m nstest/member1 -a IF4

# TEST 1: for rpc_ns_profile_elt_inq_begin where the RPC_DEFAULT_ENTRY_SYNTAX
#         is used for the profile_name_syntax, and member_name_syntax.
#
echo "Test 1:"
rpccp show_profile nstest/profile1 -i if11 -m nstest/entry1

#
# TEST 2: for rpc_ns_profile_elt_inq_begin where the inquiry_type is 
#         rpc_c_profile_match_by_if (just inquire by if_id)
#
echo "Test 2:"
rpccp show_profile nstest/profile2 -i if22 

#
# TEST 3: for rpc_ns_profile_elt_inq_begin where the inquiry_type is 
#         rpc_c_profile_elt_match_by_both (inquire by if_id and member name)
#
echo "Test 3:"
rpccp show_profile nstest/profile2 -i if22 -m nstest/mbr2

#
# TEST 4: for rpc_ns_profile_elt_inq_begin where the vers_option is
#         rpc_c_vers_all
#
echo "Test 4:"
rpccp show_profile nstest/profile6 -i if66 -v all

#
# TEST 5: for rpc_ns_profile_elt_inq_begin where the vers_option is
#         rpc_c_vers_major_only
#
echo "Test 5:"
rpccp show_profile nstest/profile3 -i if33 -v major_only

#
# TEST 6: for rpc_ns_profile_elt_remove where the RPC_DEFAULT_ENTRY_SYNTAX
#         is used for the profile_name_syntax, and the member_name_syntax
#
echo "Test 6:"
rpccp remove_element nstest/profile1 -i if11 -m nstest/entry1 
rpccp show_profile nstest/profile1 -i if11 -m nstest/entry1 

#
# TEST 7: for rpc_ns_profile_elt_remove where the if_id is NULL
#
echo "Test 7:"
rpccp remove_element nstest/profile4 -d -m nstest/member1        
rpccp show_profile nstest/profile4 -d        

#
# TEST 8: for rpc_ns_profile_elt_remove where the profile does not exist        
#         to do this, first add the profile, then delete the profile, then
#         try to remove a member from the already deleted profile.
#
echo "Test 8:"
rpccp remove_profile nstest/profile99         
rpccp remove_element nstest/profile99 -i if11 -m nstest/mbr0        

#
# TEST 9: for rpc_ns_profile_elt_remove where the profile element that is
#         being removed does not exist        
#
echo "Test 9:"
rpccp remove_element nstest/profile3 -i if33 -m nstest/member99 

#
# TEST 10: for rpc_ns_profile_delete where RPC_DEFAULT_ENTRY_SYNTAX is
#          used for profile_name_syntax 
#
echo "Test 10:"
rpccp remove_profile nstest/profile1         
rpccp show_profile nstest/profile1         

#
# TEST 11: for rpc_ns_profile_delete where the profile to be deleted does
#          not exist
#
echo "Test 11:"
rpccp remove_profile nstest/profile88         

# Cleanup- delete all profiles left after this test
echo "Cleanup- delete all profiles left after the tests"
rpccp remove_profile nstest/profile2  
rpccp remove_profile nstest/profile3 
rpccp remove_profile nstest/profile4 
rpccp remove_profile nstest/profile5 
rpccp remove_profile nstest/profile6 
