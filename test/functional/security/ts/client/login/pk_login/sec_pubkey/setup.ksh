#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: setup.ksh,v $
# Revision 1.1.2.2  1996/10/03  18:46:02  arvind
# 	Add OSF copyright
# 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/2]
#
# Revision 1.1.2.1  1996/08/09  20:15:15  arvind
# 	Merge from DCE_1.2
# 	[1996/08/09  20:01:04  arvind]
# 
# $EndLog$
#


# Create pk_user account

#dcecp << !
#user create test_pubkey_pk_user -gro none -org none -mypwd "-dce-" -password "pk_pwd"
#!

dcecp << !
principal create test_pubkey_pk_user
group add none -member test_pubkey_pk_user
organization add none -member test_pubkey_pk_user
account create test_pubkey_pk_user -gro none -org none -mypwd "-dce-" -password "pk_pwd"
!

if [[ $? != 0 ]]
then
    exit $?
fi
