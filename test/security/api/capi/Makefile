#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/11/14  16:17:51  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:31:49  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:45 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:45 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  @DEC_COPYRIGHT@
#  Copyright (c) 1996
#  Digital Equipment Corporation, Maynard, Massachusetts, USA
#  All Rights Reserved
#  



LIBS = -lpkplcy -lpktrust -lpk -lpkcrypto -lpkxds -ldce -bloadmap:error
CC_LD = xlC

DATA_SUBDIRS = certs keys testcases

LIBFLAGS = -L../../../../security/pkc -L../../../../security/pkc/capi/trust \
            -L../../../../security/pkc/capi/plcy  -L../../../../security/pkc/capi/crypto \
            -L../../../../security/pkc/xds

INCFLAGS = -I. -I../../../../security/pkc/h -I../../../../security/h -I../../../../directory/cds/includes

test_registry_OFILES = test_registry.o trycase_helper.o
trycase_a_OFILES = trycase_a.o cds_help_a.o trycase_helper.o
PROGRAMS = trycase_a test_registry 

.include <${RULES_MK}>
