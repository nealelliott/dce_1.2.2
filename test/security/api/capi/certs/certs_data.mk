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
# $Log: certs_data.mk,v $
# Revision 1.1.2.1  1996/11/14  16:59:29  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:02:58  arvind]
#
# Revision /main/DCE_1.2.2/2  1996/11/04  22:38 UTC  arvind
# 	Cert-API drop 2 from DEC
# 	[1996/11/03  22:48 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
# 
# Revision /main/DCE_1.2.2/1  1996/10/18  00:49 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/17  01:16 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  






CERTS = 0_1_0_0.cert 0_2_0_0.cert 0_3_0_0.cert 0_4_0_0.cert 0_5_0_0.cert 0_6_0_0.cert \
        0_7_0_0.cert 1_0_0_0.cert 1_2_0_0.cert 1_3_0_0.cert 1_4_0_0.cert 1_5_0_0.cert \
        1_6_0_0.cert 1_7_0_0.cert 2_0_0_0.cert 2_1_0_0.cert 2_3_0_0.cert 2_4_0_0.cert \
        2_5_0_0.cert 2_6_0_0.cert 2_7_0_0.cert 3_0_0_0.cert 3_1_0_0.cert 3_2_0_0.cert \
        3_4_0_0.cert 3_5_0_0.cert 3_6_0_0.cert 3_7_0_0.cert 4_0_0_0.cert 4_1_0_0.cert \
        4_2_0_0.cert 4_3_0_0.cert 4_5_0_0.cert 4_6_0_0.cert 4_7_0_0.cert 5_0_0_0.cert \
        5_1_0_0.cert 5_2_0_0.cert 5_3_0_0.cert 5_4_0_0.cert 5_6_0_0.cert 5_7_0_0.cert \
        6_0_0_0.cert 6_1_0_0.cert 6_2_0_0.cert 6_3_0_0.cert 6_4_0_0.cert 6_5_0_0.cert \
        6_7_0_0.cert 7_0_0_0.cert 7_1_0_0.cert 7_2_0_0.cert 7_3_0_0.cert 7_4_0_0.cert \
        7_5_0_0.cert 7_6_0_0.cert

XCERTS =  8_9_0_0.cert 8_10_0_0.cert 8_11_0_0.cert 8_12_0_0.cert 8_13_0_0.cert 8_14_0_0.cert 8_15_0_0.cert \
          9_10_0_0.cert 9_11_0_0.cert  9_12_0_0.cert  9_13_0_0.cert  9_14_0_0.cert  9_15_0_0.cert 9_8_0_0.cert \
         10_11_0_0.cert  10_12_0_0.cert  10_13_0_0.cert  10_14_0_0.cert  10_15_0_0.cert 10_8_0_0.cert   10_9_0_0.cert  \
         11_12_0_0.cert  11_14_0_0.cert  11_10_0_0.cert  11_13_0_0.cert  11_15_0_0.cert 11_6_0_0.cert   11_8_0_0.cert  \
         12_11_0_0.cert  12_14_0_0.cert  12_9_0_0.cert 12_10_0_0.cert  12_13_0_0.cert  12_15_0_0.cert  12_8_0_0.cert \
         13_11_0_0.cert  13_14_0_0.cert  13_9_0_0.cert 13_10_0_0.cert  13_12_0_0.cert  13_15_0_0.cert  13_8_0_0.cert \
         14_11_0_0.cert  14_13_0_0.cert 14_9_0_0.cert 14_10_0_0.cert  14_12_0_0.cert  14_15_0_0.cert 14_8_0_0.cert \
         15_11_0_0.cert  15_13_0_0.cert  15_9_0_0.cert 15_10_0_0.cert  15_12_0_0.cert  15_14_0_0.cert  15_8_0_0.cert








CROSS_CERTS =   0_1_0_0.crcert 0_2_0_0.crcert 0_3_0_0.crcert 0_4_0_0.crcert \
                0_5_0_0.crcert 0_6_0_0.crcert 0_7_0_0.crcert 1_2_0_0.crcert \
                1_3_0_0.crcert 1_4_0_0.crcert 1_5_0_0.crcert 1_6_0_0.crcert \
                1_7_0_0.crcert 2_3_0_0.crcert 2_4_0_0.crcert 2_5_0_0.crcert \
                2_6_0_0.crcert 2_7_0_0.crcert 3_4_0_0.crcert 3_5_0_0.crcert \
                3_6_0_0.crcert 3_7_0_0.crcert 4_5_0_0.crcert 4_6_0_0.crcert \
                4_7_0_0.crcert 5_6_0_0.crcert 5_7_0_0.crcert 6_7_0_0.crcert 

XCROSS_CERTS = 8_10_0_0.crcert  8_11_0_0.crcert  8_12_0_0.crcert  8_13_0_0.crcert \
               8_14_0_0.crcert  8_15_0_0.crcert  8_9_0_0.crcert \
               9_10_0_0.crcert  9_11_0_0.crcert  9_12_0_0.crcert  9_13_0_0.crcert  \
               9_14_0_0.crcert 9_15_0_0.crcert \
               10_11_0_0.crcert  10_12_0_0.crcert  10_13_0_0.crcert  10_14_0_0.crcert  \
               10_15_0_0.crcert \
               11_12_0_0.crcert  11_13_0_0.crcert  11_14_0_0.crcert  11_15_0_0.crcert \
               12_13_0_0.crcert  12_14_0_0.crcert  12_15_0_0.crcert \
               13_14_0_0.crcert  13_15_0_0.crcert \
               14_15_0_0.crcert


CRLS =      1_0_0.crl

XCRLS =     9_0_8.crl

DATAFILES =  ${CERTS} ${CROSS_CERTS} ${CRLS} ${XCERTS} ${XCROSS_CERTS} ${XCRLS}
