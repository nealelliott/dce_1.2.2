#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: gss-test-cleanup.sh,v $
# Revision 1.1.2.2  1996/03/11  02:07:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:59  marty]
#
# Revision 1.1.2.1  1995/12/11  19:54:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/01/27  14:58 UTC  mdf
# 	Functional test changes
# 
# 	HP revision /main/HPDCE02/mdf_MOTHRA_2/2  1995/01/27  14:24 UTC  mdf
# 	More changes to make output of tests look reasonable.
# 
# 	HP revision /main/HPDCE02/mdf_MOTHRA_2/1  1995/01/25  19:37 UTC  mdf
# 	Fixes.
# 
# 	HP revision /main/HPDCE02/1  1995/01/20  13:42 UTC  mdf
# 	Correct the location of GSSAPI functional tests
# 
# 	HP revision /main/mdf_MOTHRA_1/1  1995/01/19  18:39 UTC  mdf
# 	[1995/12/11  19:27:28  root]
# 
# Revision 1.1.2.2  1994/08/10  19:23:09  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:26  annie]
# 
# Revision 1.1.2.1  1994/07/20  14:55:19  wray
# 	Make tet-compliant
# 	[1994/07/20  14:54:33  wray]
# 
# $EndLog$
#
#

rgy_edit > /dev/null <<- EOF1
	domain principal
	delete gss-prin-1
	delete gss-prin-2
	quit
EOF1

#
/bin/rm -f /krb5/gss-keytab
