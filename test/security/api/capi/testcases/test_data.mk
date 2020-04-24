#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# All Rights Reserved
#
# @DEC_COPYRIGHT@
# Copyright (c) 1996
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved
#
# HISTORY
# $Log: test_data.mk,v $
# Revision 1.1.2.1  1996/11/14  17:11:43  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:19:14  arvind]
#
# Revision /main/DCE_1.2.2/2  1996/11/04  22:39 UTC  arvind
# 	Cert-API drop 2 from DEC
# 	[1996/11/03  22:53 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
# 
# Revision /main/DCE_1.2.2/1  1996/10/18  00:51 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:47 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  



TESTCASES = pc1 pc2 pc3 pc4 pc5 pc6 pc7 xc1 xc2 xc3 xc7
TEST_PRINCIPALS = principals
HELPERS = cdsconfig cdscreatedir cdsdeletedir cdsprincattr revokelist xdsconfig \
          xdscreatedir xdsdeletedir xdsprincattr

DATAFILES =  ${TESTCASES} ${TEST_PRINCIPALS} ${HELPERS}
           

