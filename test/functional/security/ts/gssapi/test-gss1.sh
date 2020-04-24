#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test-gss1.sh,v $
# Revision 1.1.2.2  1996/03/09  20:47:37  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:27  marty]
#
# Revision 1.1.2.1  1995/12/11  19:54:08  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:29  root]
# 
# $EndLog$
#
# Assume that the tcc envrionment is run with the right 
# DCE permissions. 
#

MYDIR=$(dirname $0)		# Need to know where to find the other
				# components of this test.
cd ${MYDIR}
${MYDIR}/gss-test-setup.sh
export GSSAPI_TEST_ARGUMENTS="-p gss-pw-1 gss-prin-1 gss-prin-2 FILE:/krb5/gss-keytab"
${MYDIR}/test-gssapi
${MYDIR}/gss-test-cleanup.sh
