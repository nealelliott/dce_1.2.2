#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: cleanup.ksh,v $
# Revision 1.1.2.2  1996/10/03  18:43:13  arvind
# 	Add OSF copyright
# 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/2]
#
# Revision 1.1.2.1  1996/08/09  20:12:28  arvind
# 	Merge from DCE_1.2
# 	[1996/08/09  19:57:55  arvind]
# 
# $EndLog$
#


# delete pk_user account

dcecp -c user delete test_rsec_pk_user

if [[ $? != 0 ]]
then
    exit $?
fi
