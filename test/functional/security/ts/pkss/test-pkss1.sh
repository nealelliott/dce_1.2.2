#!/bin/ksh
#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: test-pkss1.sh,v $
# Revision 1.1.2.1  1996/11/19  20:44:14  arvind
# 	Merge from DCE_1.2
# 	[1996/11/19  20:42:21  arvind]
#
# $EndLog$
#  
#
# JC - Aug 20, 1996
# Script to invoke the PKSS tests             
#
MYDIR=$(dirname $0)             # Need to know where to find the other
				# components of this test.
cd ${MYDIR}


rm -f dce_login_noexec
ln -s `which dce_login` dce_login_noexec
KRB5CCNAME=`dce_login_noexec cell_admin -dce-`
ps aux | grep pkssd
${MYDIR}/test_pkss1
kdestroy
KRB5CCNAME=
export KRB5CCNAME
#EOF
