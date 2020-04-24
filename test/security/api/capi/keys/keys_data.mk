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
# $Log: keys_data.mk,v $
# Revision 1.1.2.1  1996/11/14  17:06:52  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:14:31  arvind]
#
# Revision /main/DCE_1.2.2/2  1996/11/04  22:39 UTC  arvind
# 	Cert-API drop 2 from DEC
# 	[1996/11/03  22:52 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
# 
# Revision /main/DCE_1.2.2/1  1996/10/18  00:50 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:45 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  




PUBLIC_KEYS = \
0_0.pubkey \
1_0.pubkey \
2_0.pubkey \
3_0.pubkey \
4_0.pubkey \
5_0.pubkey \
6_0.pubkey \
7_0.pubkey \


PRIVATE_KEYS = \
0_0.privkey \
1_0.privkey \
2_0.privkey \
3_0.privkey \
4_0.privkey \
5_0.privkey \
6_0.privkey \
7_0.privkey \

XPUBLIC_KEYS = \
11_0.pubkey  13_0.pubkey  15_0.pubkey \
8_0.pubkey 10_0.pubkey  12_0.pubkey  14_0.pubkey  9_0.pubkey


XPRIVATE_KEYS = \
11_0.privkey  13_0.privkey  15_0.privkey  8_0.privkey \
10_0.privkey  12_0.privkey  14_0.privkey 9_0.privkey



DATAFILES =  ${PUBLIC_KEYS} ${PRIVATE_KEYS} ${XPUBLIC_KEYS} ${XPRIVATE_KEYS}
           
