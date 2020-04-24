#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/03  20:36:38  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:45:15  arvind]
#
# 	OSF DCE 1.2.2 Drop 4
#
# Revision /main/DCE_1.2.2/1  1996/09/26  23:32 UTC  sommerfeld
# 	Dummy makefile for kerberos public key test.
# 	[1996/09/16  19:52 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
# 
# $EndLog$

PROGRAMS=k5pktest k5pkgen

k5pkgen_OFILES=k5pkgen.o pkcommon.o
k5pktest_OFILES=k5pktest.o pkcommon.o 

BSAFE_ROOT=/vob/bsafe
BSAFE_INCDIR=${BSAFE_ROOT}/${target_machine}/bsafe/include
BSAFE_LIBDIR=${BSAFE_ROOT}/${target_machine}/bsafe/lib
LIBS=-lpsm -lbsafe2 -ldce
CFLAGS=-DOSF_DCE
INCFLAGS=-I../../../../security/krb5/kdc

k5pktest_LIBS = ../../../../security/krb5/kdc/kdc_pk_preauth.o \
	../../../../security/krb5/lib/krb5/pk_preauth.o  \
	${LIBS}

LIBFLAGS=-L${BSAFE_LIBDIR}


.include <${RULES_MK}>


