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
# $Log: rpccp_entry.sh,v $
# Revision 1.1.4.2  1996/03/11  02:20:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:56:37  marty]
#
# Revision 1.1.4.1  1995/12/11  19:36:29  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:19:12  root]
# 
# Revision 1.1.2.2  1993/01/11  21:15:12  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  14:55:45  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# Note: must use cdscp to create the directory nstest before running this
#       script. 
set -x
RPC_DEFAULT_ENTRY_SYNTAX=${RPC_DEFAULT_ENTRY_SYNTAX:-3}
export RPC_DEFAULT_ENTRY_SYNTAX
. rpccp_setup.sh
rpccp export -i $if1 -b $b1 -o $obj1 -o $obj2 nstest/entry1 || { echo fail;exit 1; }
rpccp export -i $if1 -b $b2 nstest/entry1 || { echo fail;exit 1; }
rpccp export -i $if2 -b $b1 nstest/entry1 || { echo fail;exit 1; }
rpccp export -i $if1 -b $b1 nstest/entry2 || { echo fail;exit 1; }
rpccp export -i $if10 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if11 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if20 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if30 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if30 -b $b4 nstest/entry4 || { echo fail;exit 1; }
rpccp add_member -m nstest/entry1 nstest/group1 || { echo fail;exit 1; }
rpccp add_member -m nstest/entry2 nstest/group1 || { echo fail;exit 1; }
rpccp add_member -m nstest/entry1 nstest/group2 || { echo fail;exit 1; }
rpccp add_member -m nstest/group1 nstest/group || { echo fail;exit 1; }
rpccp add_member -m nstest/group2 nstest/group || { echo fail;exit 1; }
rpccp add_element -i $if1 -m nstest/entry1 -a IF1 -p 0 nstest/profile1  || { echo fail;exit 1; }
rpccp add_element -i $if1 -m nstest/entry2 -a IF1 -p 0 nstest/profile2 || { echo fail;exit 1; }
rpccp add_element -i $if11 -m nstest/entry3 -a IF11 -p 0 nstest/profile3 || { echo fail;exit 1; }
rpccp add_element -i $if30 -m nstest/entry4 -a IF30 -p 0 nstest/profile4 || { echo fail;exit 1; }
rpccp add_element -i $if1 -m nstest/profile1 -a IF1 -p 0 nstest/profile || { echo fail;exit 1; }
rpccp add_element -i $if1 -m nstest/profile2 -a IF1 -p 1 nstest/profile || { echo fail;exit 1; }
rpccp add_element -i $if11 -m nstest/profile3 -a IF11 -p 0 nstest/profile || { echo fail;exit 1; }
rpccp add_element -d -m nstest/profile4 -a IF30 -i $if11 -p 0 nstest/profile || { echo fail;exit 1; }
rpccp show_entry nstest/entry1 || { echo fail;exit 1; }
rpccp show_entry nstest/entry2 || { echo fail;exit 1; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp show_entry nstest/entry4 || { echo fail;exit 1; }
rpccp show_group nstest/group || { echo fail;exit 1; }
rpccp show_group -r nstest/group || { echo fail;exit 1; }
rpccp show_group -r 1 nstest/group || { echo fail;exit 1; }
rpccp show_group -m nstest/group1 nstest/group || { echo fail;exit 1; }
rpccp show_group -m nstest/entry1 nstest/group || { echo fail;exit 1; }
rpccp show_group nstest/group1 || { echo fail;exit 1; }
rpccp show_group nstest/group2 || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp show_profile -a IF1 nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if1 nstest/profile || { echo fail;exit 1; }
rpccp show_profile -m nstest/profile1 nstest/profile || { echo fail;exit 1; }
rpccp show_profile -d  nstest/profile || { echo fail;exit 1; }
rpccp show_profile -r nstest/profile || { echo fail;exit 1; }
rpccp show_profile -r 1 nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if11 -v all nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if11 -v exact nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if11 -v compatible nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if11 -v major_only nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if11 -v upto nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if1 -v all nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if1 -v exact nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if1 -v compatible nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if1 -v major_only nstest/profile || { echo fail;exit 1; }
rpccp show_profile -i $if1 -v upto nstest/profile || { echo fail;exit 1; }
rpccp show_profile nstest/profile1 || { echo fail;exit 1; }
rpccp show_profile nstest/profile2 || { echo fail;exit 1; }
rpccp show_profile nstest/profile3 || { echo fail;exit 1; }
rpccp show_server nstest/entry1 || { echo fail;exit 1; }
rpccp show_server  -i $if1 nstest/entry1 || { echo fail;exit 1; }
rpccp show_server -o $obj1 nstest/entry1 || { echo fail;exit 1; }
rpccp show_entry nstest/entry1 || { echo fail;exit 1; }
#rpccp unexport -o $obj1 nstest/entry1 || { echo fail;exit 1; }
rpccp show_entry nstest/entry1 || { echo fail;exit 1; }
rpccp unexport -i $if1 nstest/entry   && { echo fail;exit 1; }
rpccp show_entry nstest/entry1 || { echo fail;exit 1; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if1 -v exact nstest/entry3 && { echo fail;exit 1; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if1 -v upto nstest/entry3 && { echo fail;exit 1; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if1 -v major_only nstest/entry3 && { echo fail;exit 1; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if1 -v compatible nstest/entry3 || { echo fail; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if10 -v exact nstest/entry3 || { echo fail; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if11 -compatible nstest/entry3 || { echo fail; }
rpccp show_entry nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if20 -v all nstest/entry3 || { echo fail; }
rpccp show_entry nstest/entry3  || { echo fail;exit 1; }
rpccp export -i $if10 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if11 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if12 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if20 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if30 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp show_entry nstest/entry3  || { echo fail;exit 1; }
rpccp unexport -i $if10 -v major_only nstest/entry3 || { echo fail;exit 1; }
rpccp show_entry nstest/entry3  || { echo fail;exit 1; }
rpccp export -i $if10 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if11 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp export -i $if12 -b $b1 nstest/entry3 || { echo fail;exit 1; }
rpccp unexport -i $if11 -v upto nstest/entry3 || { echo fail;exit 1; }
rpccp show_entry nstest/entry3  || { echo fail;exit 1; }
rpccp show_entry nstest/entry1 || { echo fail;exit 1; }
rpccp remove_entry nstest/entry1 || { echo fail;exit 1; }
rpccp show_entry nstest/entry1 && { echo fail;exit 1; }
rpccp export -i $if1 -b $b1 -o $obj1 -o $obj2 nstest/entry1 || { echo fail;exit 1; } 
rpccp export -i $if1 -b $b2 nstest/entry1  || { echo fail;exit 1; }
rpccp export -i $if1 -b $b3 nstest/entry1  || { echo fail;exit 1; }
rpccp export -i $if1 -b $b4 nstest/entry1  || { echo fail;exit 1; }
rpccp export -i $if2 -b $b1 nstest/entry1  || { echo fail;exit 1; }
rpccp show_entry nstest/entry1 || { echo fail;exit 1; }
rpccp import -i $if1 nstest/entry1 || { echo fail;exit 1; }
rpccp import -e -i $if1 nstest/entry1 || { echo fail;exit 1; }
rpccp import -i $if2 -o $obj1 nstest/entry1 || { echo fail;exit 1; }
rpccp import -i $if1 -o $obj1 nstest/entry1 || { echo fail;exit 1; }
rpccp import -e -i $if1 -n 1 nstest/entry1 || { echo fail;exit 1; }
rpccp import -i $if1 -n 3 nstest/entry1 || { echo fail;exit 1; }
rpccp import -i $if1 -n nstest/entry1 || { echo fail;exit 1; }
rpccp show_group nstest/group1 || { echo fail;exit 1; }
rpccp remove_member -m nstest/entry1 nstest/group1 || { echo fail;exit 1; }
rpccp show_group nstest/group1 || { echo fail;exit 1; }
rpccp remove_group nstest/group1 || { echo fail;exit 1; }
rpccp show_group nstest/group1 && { echo fail;exit 1; }
rpccp show_group nstest/group || { echo fail;exit 1; }
rpccp remove_group nstest/group || { echo fail;exit 1; }
rpccp show_group nstest/group && { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_element -a IF1 nstest/profile || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_element -i $if11 -m nstest/profile3  nstest/profile || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_element -d nstest/profile  || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_profile nstest/profile || { echo fail;exit 1; }
rpccp show_profile nstest/profile && { echo fail;exit 1; }
rpccp add_element -i $if1 -m nstest/profile1 -a IF1 -p 0 nstest/profile || { echo fail;exit 1; }
rpccp add_element -i $if1 -m nstest/profile2 -a IF1 -p 1 nstest/profile || { echo fail;exit 1; }
rpccp add_element -i $if11 -m nstest/profile3 -a IF11 -p 0 nstest/profile || { echo fail;exit 1; }
rpccp add_element -d -m nstest/profile4 -a IF30 -i $if11 -p 0 nstest/profile || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_element -i $if11 -a IF1 nstest/profile || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_element -d -a IF1 nstest/profile  || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_element -d -a IF30 nstest/profile  || { echo fail;exit 1; }
rpccp show_profile nstest/profile || { echo fail;exit 1; }
rpccp remove_entry nstest/entry1 || { echo fail;exit 1; }
rpccp remove_entry nstest/entry2 || { echo fail;exit 1; }
rpccp remove_entry nstest/entry3 || { echo fail;exit 1; }
rpccp remove_entry nstest/entry4 || { echo fail;exit 1; }
rpccp remove_entry nstest/group1 || { echo fail;exit 1; }
rpccp remove_entry nstest/group2 || { echo fail;exit 1; }
rpccp remove_entry nstest/group || { echo fail;exit 1; }
rpccp remove_entry nstest/profile1 || { echo fail;exit 1; }
rpccp remove_entry nstest/profile2 || { echo fail;exit 1; }
rpccp remove_entry nstest/profile3 || { echo fail;exit 1; }
rpccp remove_entry nstest/profile4 || { echo fail;exit 1; }
rpccp remove_entry nstest/profile || { echo fail;exit 1; }
echo success
exit 0
