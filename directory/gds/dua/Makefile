#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.14.2  1996/03/09  23:13:42  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:01  marty]
#
# Revision 1.2.14.1  1995/12/08  15:52:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:09:18  root]
# 
# Revision 1.2.12.3  1994/06/21  14:46:33  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:38:28  marrek]
# 
# Revision 1.2.12.2  1994/02/23  17:56:02  zeliff
# 	OT9996:  add 'switch' to EXPLIB_SUBDIRS so RIOS libdce build
# 	process can find build rules for libd21sw.a
# 	[1994/02/23  17:51:37  zeliff]
# 
# Revision 1.2.12.1  1994/02/22  17:38:03  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:28:22  marrek]
# 
# Revision 1.2.10.3  1993/10/14  17:27:25  keutel
# 	October 1993 code drop
# 	[1993/10/14  17:25:10  keutel]
# 
# Revision 1.2.10.2  1993/10/13  17:31:53  keutel
# 	October 1993 code drop
# 	[1993/10/11  11:20:47  keutel]
# 
# Revision 1.2.10.1  1993/10/04  18:13:09  marrek
# 	two libraries are built now
# 	[1993/10/04  18:11:58  marrek]
# 
# Revision 1.2.8.2  1993/08/11  05:49:43  marrek
# 	Set OFLAGS also for libdce.
# 	[1993/08/06  09:04:50  marrek]
# 
# 	no switch if OSFCDS is not defined.
# 	[1993/08/05  08:26:06  marrek]
# 
# 	changes for OSFCDS and THREADSAFE: Jochen Keutel
# 	[1993/08/02  08:13:58  marrek]
# 
# 	Remove unused -DDNS_V3API -DTRACE and ofiles.rlog .
# 	[1993/07/30  11:14:34  marrek]
# 
# 	July 1993 code drop.
# 	[1993/07/30  10:48:36  marrek]
# 
# Revision 1.2.6.5  1993/03/17  20:21:47  treff
# 	DCT @ OSF
# 	Turn on CFLAGS=-DOSFCDS for d21bind.c, to get
# 	XDS switch to work without GDS running.
# 	[1993/03/17  20:21:18  treff]
# 
# Revision 1.2.6.4  1992/12/31  19:45:40  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:23:33  bbelch]
# 
# Revision 1.2.6.3  1992/11/27  21:04:22  marrek
# 	November 1992 code drop
# 	[1992/11/20  13:32:01  marrek]
# 
# Revision 1.2.6.2  1992/09/29  21:10:13  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:23:43  treff]
# 
# Revision 1.2.3.4  1992/06/30  20:57:31  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:37:47  melman]
# 
# Revision 1.2.3.3  1992/06/02  03:04:45  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  21:35:25  zeliff]
# 
# Revision 1.2.3.2  1992/05/11  17:50:21  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:21:22  mhickey]
# 
# Revision 1.2  1992/01/19  22:13:10  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.14.2 $ $Date: 1996/03/09 23:13:42 $
#

CFLAGS += -DOSFCDS

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
libdua_OFILES		=  ${ofiles.dua}

libdua.a_OFILES = ${libdua_OFILES}

ofiles.dua = \
        d21bind.o    d21binda.o   d21bindh.o   \
	d21_3_read.o d21readc.o   d21readh.o   \
	d21_4_list.o d21lsh.o     \
	d21_5_sea.o  d21sch.o     \
	d21_6_comp.o d21cph.o     \
	d21_7_add.o  d21adh.o     \
	d21_8_rem.o  d21remh.o    \
	d21_B_rr.o   d21rrh.o     \
	d21_C_ma.o   d21mah.o     \
        d21com.o     d21common.o  \
	d21_c20.o    d21_c70.o    d21_c80.o    \
	d21log.o     \
	d21ref.o     d21refh.o  \
	d21filter.o  \
	d21file.o    \
        d21spafm.o   d21spalm.o   d21spals.o   d21spatm.o  \
	d21mhsfm.o   d21mhslm.o   d21mhsls.o   d21mhstm.o  \
	d21sap.o     d21sec.o	  \
	d21ipcev.o   D00_dua_protect.o d21abandon.o d21signal.o

#
# The rest of the Makefile is private for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#

EXPLIB_SUBDIRS	= switch

LIBRARIES	= libdua.a
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS	= ${libdua_OFILES}

EXPDIR		= /usr/lib/
INCFLAGS	= -I../h

.include <${RULES_MK}>

.endif

