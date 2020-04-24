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
# $Log: test-pkss5.sh,v $
# Revision 1.1.2.1  1996/11/19  20:45:19  arvind
# 	Merge from DCE_1.2
# 	[1996/11/19  20:43:11  arvind]
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

${MYDIR}/test_pkss5

#EOF
