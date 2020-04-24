#!/bin/ksh
#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: setup.ksh,v $
# Revision 1.1.2.2  1996/10/03  18:47:05  arvind
# 	Merge from DCE_1.2
# 	[1996/10/03  18:31:19  arvind]
#
# $EndLog$
#  
# Create pk_user account

#dcecp << !
#user create test_pvtkey_pk_user -gro none -org none -mypwd "-dce-" -password "pk_pwd"
#!

dcecp << !
principal create test_pvtkey_pk_user
group add none -member test_pvtkey_pk_user
organization add none -member test_pvtkey_pk_user
account create test_pvtkey_pk_user -gro none -org none -mypwd "-dce-" -password "pk_pwd"

!

if [[ $? != 0 ]]
then
    exit $?
fi
