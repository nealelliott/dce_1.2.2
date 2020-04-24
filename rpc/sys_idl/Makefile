# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.16.1  1996/10/03  14:42:14  arvind
# 	Export idlfiles during SAMIDL pass, not FIRST pass.
# 	[1996/09/17  00:27 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.14.2  1996/03/09  23:26:29  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:04  marty]
# 
# Revision 1.1.14.1  1995/12/08  00:22:45  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/22  15:52 UTC  jrr
# 	Fix INCLUDES line.
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  22:25 UTC  jrr
# 	Merge mothra changes into 1.2.1 for XIDL drop 2.
# 
# 	HP revision /main/HPDCE02/5  1995/01/16  21:04 UTC  mk
# 	Merge in changes from mk_mothra_dced/1.
# 
# 	HP revision /main/HPDCE02/mk_mothra_dced/1  1995/01/16  21:02 UTC  mk
# 	Delete references to glb.idl, llb.idl, and sockbase.idl.
# 
# 	HP revision /main/HPDCE02/4  1994/11/22  21:49 UTC  tatsu_s
# 	Removed rrpc.idl.
# 
# 	HP revision /main/HPDCE02/3  1994/09/08  20:36 UTC  sommerfeld
# 	put idl files into INCLUDES so they get exported during the AUTOGEN pass.
# 
# 	HP revision /main/HPDCE02/2  1994/08/17  20:41 UTC  tatsu_s
# 	Merged from Hpdce02_01
# 	[1995/12/08  00:00:48  root]
# 
# Revision 1.1.10.10  1994/07/28  18:30:30  mori_m
# 	CR: 11294	Added nsattrid.h for export.
# 	[1994/07/28  18:16:42  mori_m]
# 
# Revision 1.1.10.9  1994/07/14  21:29:33  tom
# 	Remove NCS 1.5 files (glb.idl,llb.idl,rrpc.idl,sockbase.idl)
# 	[1994/07/14  21:09:13  tom]
# 
# Revision 1.1.10.8  1994/07/13  15:15:02  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:27  marty]
# 
# Revision 1.1.10.7  1994/05/20  22:03:37  sommerfeld
# 	clean up sams file generation.
# 	[1994/05/20  21:38:15  sommerfeld]
# 
# Revision 1.1.10.6  1994/05/19  21:15:09  hopkins
# 	Merge with DCE1_1.
# 	[1994/05/04  19:30:50  hopkins]
# 
# 	Serviceability:
# 	  Generate rpcsts.idl and dcerpc.cat
# 	  using sams ...
# 	[1994/05/03  20:26:41  hopkins]
# 
# Revision 1.1.10.5  1994/02/08  21:25:07  mori_m
# 	CR 9701:  Initial submission for RPC runtime I18N support
# 	[1994/02/08  20:57:15  mori_m]
# 
# Revision 1.1.10.4  1994/01/21  22:39:50  cbrooks
# 	Add DCE1.1 platform dependent headers
# 	[1994/01/21  20:27:34  cbrooks]
# 
# Revision 1.1.10.3  1994/01/03  19:35:07  rsalz
# 	Defuncted dce_error.h (again); moved to dce/utils/svc.
# 	[1994/01/03  15:35:44  rsalz]
# 
# Revision 1.1.10.2  1993/12/15  00:20:17  mario
# 	Added dce_error.h back to INLCUDES list
# 	[1993/12/15  00:20:02  mario]
# 
# Revision 1.1.10.1  1993/12/14  20:04:46  rsalz
# 	Move dce_error.h to SVC library.
# 	[1993/12/14  20:04:29  rsalz]
# 
# Revision 1.1.6.2  1993/07/07  20:12:01  ganni
# 	reduced stub idl sources
# 	[1993/07/07  19:40:16  ganni]
# 
# Revision 1.1.4.3  1993/01/03  23:56:06  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  20:14:24  bbelch]
# 
# Revision 1.1.4.2  1992/12/23  21:19:06  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  15:46:06  zeliff]
# 
# Revision 1.1.2.3  1992/05/01  16:05:55  rsalz
# 	Edited as part of rpc6 drop.
# 	[1992/05/01  00:19:54  rsalz]
# 
# Revision 1.1.2.2  1992/04/07  19:17:50  mckeen
# 	Added .ORDER line to support pmake
# 	[1992/03/13  15:41:20  mckeen]
# 
# Revision 1.1  1992/01/19  03:13:57  devrcs
# 	Initial revision
# 
# $EndLog$
#
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##
##  NAME:
##
##      makefile.rpcd (src/rpc/sys_idl/Makefile)
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for system IDL files.
##
##

# IDL files to process (and export)
IDL_INSTALL = conv.idl    convc.idl  ep.idl     iovector.idl \
	   lbase.idl   mgmt.idl   nbase.idl  ncastat.idl  rpc.idl      \
	   rpcbase.idl rpcpvt.idl rpcsts.idl rpctypes.idl \
	   twr.idl     uuid.idl   codesets.idl

IDL_NOINSTALL =
IDLFILES     = ${IDL_INSTALL} ${IDL_NOINSTALL}

# Include files to export; the ordering matters, so be careful.
INC_INSTALL = idlbase.h rpcexc.h idl_es.h stubbase.h codesets_stub.h \
	   idlddefs.h ndrtypes.h nsattrid.h\
	   ndr_rep.h marshall.h ndrold.idl ndrold.h ${IDLFILES:S/.idl$/.h/g}

CPLUSPLUS_INCLUDES = hashtab.hxx ifacemgr.hxx objref.hxx objtable.hxx \
                proxymgr.hxx trace.hxx

INC_NOINSTALL = ${SVC_INCLUDES}
INCLUDES      = ${INC_INSTALL} ${INC_NOINSTALL} ${CPLUSPLUS_INCLUDES}

SVC_INCLUDES  = dcerpcmsg.h dcerpcsvc.h dcerpcmac.h

OTHER_INSTALL   = dcerpc.cat
OTHER_NOINSTALL = ${SVC_CFILES}
OTHERS          = ${OTHER_INSTALL} ${OTHER_NOINSTALL}

SVC_CFILES = dcerpcmsg.c dcerpcsvc.c

# Files to export and their locations
#  (Shouldn't need to export dcerpc.cat, but it was
#  getting exported before ...)
EXPINC_TARGETS = ${INCLUDES:S/^/export_/g} dcerpc.cat

EXPSAMIDL_TARGETS = ${IDLFILES:S/^/export_/g} \

EXPDIR	= /usr/include/dce/

VPATH = ${TARGET_MACHINE}

# Files to install and their locations
IMODE	= 444
ILIST	= ${IDL_INSTALL} ${INC_INSTALL} ${OTHER_INSTALL} ${CPLUSPLUS_INCLUDES}
IDIR	= /share/include/dce/
dcerpc.cat_IDIR = /nls/msg/en_US.ASCII/

IDLFLAGS = -cepv 
ep.h_IDLFLAGS    = ${IDLFLAGS} -no_mepv
convc.h_IDLFLAGS = ${IDLFLAGS} -no_mepv
conv.h_IDLFLAGS  = ${IDLFLAGS} -no_mepv
mgmt.h_IDLFLAGS  = ${IDLFLAGS} -no_mepv
codesets.h_IDLFLAGS  = -no_mepv
SAMSFILES=rpc.sams
SAMSFLAGS=-i rpcsts
SAMS_TARGETS = ${SVC_INCLUDES} ${SVC_CFILES} rpcsts.idl dcerpc.cat

.include <${RULES_MK}>

.ORDER: ${IDLFILES:S/^/export_/g} ${INCLUDES}

rpcsts.idl: 	rpc.sams
		${SAMS} ${_SAMSFLAGS_} -oi ${rpc.sams:P}
