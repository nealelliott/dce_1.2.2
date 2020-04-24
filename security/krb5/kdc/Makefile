# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.2  1996/10/15  20:50:30  arvind
# 	Change for international build
# 	[1996/10/07  19:26 UTC  arvind  /main/arvind_work/4]
#
# Revision 1.1.13.1  1996/10/03  14:58:08  arvind
# 	add pk_preauth.o to build.
# 	[1996/09/16  20:39 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
# 
# Revision 1.1.11.2  1996/03/09  23:29:42  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:03  marty]
# 
# Revision 1.1.11.1  1995/12/08  17:44:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:57:27  root]
# 
# Revision 1.1.8.2  1994/06/02  21:28:53  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:16:41  mdf]
# 
# 	hp_sec_to_osf_3 drop
# 
# Revision 1.1.5.3  1992/12/29  14:02:18  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:54:21  zeliff]
# 
# Revision 1.1.5.2  1992/09/29  21:19:14  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/11  21:33:23  sekhar]
# 
# Revision 1.1.2.2  1992/04/07  19:18:47  mckeen
# 	Changed compile_et to ${COMPILE_ET}.  COMPILE_ET is now defined in osf.dce.mk
# 	[1992/03/13  15:46:31  mckeen]
# 
# Revision 1.1  1992/01/19  14:54:30  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=5 11/25/91 //littl/prgy/krb5/kdc
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#
# DCE Security Services 
# krb5/kdc Makefile
#

# Compiler flags for this directory
CFLAGS	 = -Dmain=kdc_main -DOSF_DCE ${${TARGET_MACHINE}_CFLAGS} ${NOENCRYPTION}
INCFLAGS = -I../../h -I../../idl

# Library to build
LIBRARIES = 	libkrb5kdc.a
EXPLIB_TARGETS = libkrb5kdc.a


OFILES    =	dispatch.o    \
		do_as_req.o   \
		do_tgs_req.o  \
		extern.o      \
		kdc_preauth.o \
		kdc_pk_preauth.o \
		kdc_util.o    \
		kerberos_v4.o \
		main.o        \
		network.o     \
		policy.o      \
		replay.o 

# Files to install and their locations
OTHERS = dcekdc.cat
ILIST   = ${OTHERS}
dcekdc.cat_IDIR = /nls/msg/en_US.ASCII/

.include <${RULES_MK}>

kdc5_err.h: kdc5_err.et
	${COMPILE_ET} $>

kdc5_err.msf: kdc5_err.et
	${COMPILE_ET} $>

dcekdc.cat: kdc5_err.msf
	${GENCAT} $@ $>

main.o: kdc5_err.h
network.o: kdc5_err.h
