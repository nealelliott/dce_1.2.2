#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.17.2  1996/03/09  23:14:54  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:12  marty]
#
# Revision 1.2.17.1  1995/12/08  15:35:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:51:10  root]
# 
# Revision 1.2.15.4  1994/03/23  15:48:22  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:34:38  keutel]
# 
# Revision 1.2.15.3  1994/02/24  16:45:15  marrek
# 	SITE switch for SNI Burlington.
# 	[1994/02/24  13:30:17  marrek]
# 
# Revision 1.2.15.2  1994/02/22  18:38:50  ohara
# 	Merged with changes from 1.2.15.1
# 	[1994/02/22  18:38:16  ohara]
# 
# 	use -Xa on SINIX
# 	[1994/02/18  16:00:06  ohara]
# 
# Revision 1.2.15.1  1994/02/22  14:58:59  marrek
# 	February 1994 code drop
# 	[1994/02/18  09:19:04  marrek]
# 
# 	Code drop for dce1.1.
# 	[1994/02/08  11:56:19  marrek]
# 
# Revision 1.2.12.2  1993/11/08  09:17:16  marrek
# 	Add -Xc for SVR4_CFLAGS and remove redundant .if TARGET_MACHINE for
# 	${TARGET_MACHINE}_CFLAGS.
# 	[1993/11/08  09:16:43  marrek]
# 
# Revision 1.2.12.1  1993/10/14  19:19:11  keutel
# 	October 1993 code drop
# 	[1993/10/14  19:16:25  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/11  09:29:30  keutel]
# 
# Revision 1.2.8.4  1993/08/23  13:52:28  treff
# 	Fix libload problem
# 	[1993/08/23  13:51:29  treff]
# 
# Revision 1.2.8.3  1993/08/20  21:35:58  treff
# 	DCT @ OSF
# 	Get rid of ANSI_CC = xlc_r, for more than
# 	one reason.
# 	[1993/08/20  21:31:50  treff]
# 
# Revision 1.2.8.2  1993/08/10  06:58:29  marrek
# 	Add flags in CFLAGS instead of redefine CFLAGS.
# 	[1993/08/06  07:42:46  marrek]
# 
# 	Put some lines out of LIBDCE_LIBRARY_MAKEFILE conditional.
# 	[1993/08/06  06:41:10  marrek]
# 
# 	Correct conditional.
# 	[1993/08/05  10:57:14  marrek]
# 
# 	Introduce GDS_NO_THREADSAFE.
# 	[1993/08/05  09:06:02  marrek]
# 
# 	changes for THREADSAFE: Jochen Keutel
# 	[1993/08/02  08:18:50  marrek]
# 
# 	July 1993 code drop.
# 	[1993/07/28  14:53:10  marrek]
# 
# Revision 1.2.5.6  1993/02/02  08:58:43  marrek
# 	*** empty log message ***
# 
# Revision 1.2.5.5  1993/02/02  08:12:55  marrek
# 	Use -Xc for SVR4_CFLAGS and remove it from CFLAGS.
# 	add xdsgds.h xdscds.h xdsmdup.h to ILIST and resp. _IDIR.
# 	[1993/02/01  13:40:34  marrek]
# 
# Revision 1.2.5.4  1992/12/30  23:28:50  bbelch
# 	Embedding copyright notice
# 	[1992/12/30  23:18:42  bbelch]
# 
# Revision 1.2.5.3  1992/11/30  08:05:56  marrek
# 	November 1992 code drop
# 	[1992/11/26  13:23:07  marrek]
# 
# Revision 1.2.5.2  1992/09/29  21:12:11  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:43:47  treff]
# 
# Revision 1.2.2.4  1992/06/30  21:00:14  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:41:24  melman]
# 
# Revision 1.2.2.3  1992/06/01  23:13:00  melman
# 	New GDS merged drop
# 	[1992/05/29  12:37:03  melman]
# 
# Revision 1.2.2.2  1992/05/11  17:50:48  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:21:40  mhickey]
# 
# Revision 1.2  1992/01/19  22:13:18  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.17.2 $ $Date: 1996/03/09 23:14:54 $
#

.if !defined(GDS_NO_LIBDCE)
RIOS_CFLAGS	+= -D_THREAD_SAFE 
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
.else
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
.endif # defined GDS_NO_LIBDCE

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
libXDS_OFILES	= \
	dsAbandon.o \
	dsAddEntry.o \
	dsBind.o \
	dsCompare.o \
	dsExport.o \
	dsInit.o \
	dsList.o \
	dsModEntry.o \
	dsModRdn.o \
	dsRead.o \
	dsRecvRes.o \
	dsRemEntry.o \
	dsSearch.o \
	dsShutdown.o \
	dsTrace.o \
	dsUnbind.o \
	dsUtils.o \
	dsVersion.o \
	  dsPAcl.o \
	  dsPCode.o \
	  dsPCreate.o \
	  dsPDelete.o \
	  dsPGet.o \
	  dsPGetBdcp.o \
	  dsPInst.o \
	  dsPMdup.o \
	  dsPPsap.o \
	  dsPPut.o \
	  dsPWkspace.o \
          dsXext.o

libXDS.a_OFILES = ${libXDS_OFILES}

dsPAcl.o_CFLAGS     = $(CFLAGS) -D_MDUP_PKG_
dsPCode.o_CFLAGS    = $(CFLAGS) -D_MDUP_PKG_
dsPCreate.o_CFLAGS  = $(CFLAGS) -D_MDUP_PKG_
dsPDelete.o_CFLAGS  = $(CFLAGS) -D_MDUP_PKG_
dsPGet.o_CFLAGS     = $(CFLAGS) -D_MDUP_PKG_
dsPGetBdcp.o_CFLAGS = $(CFLAGS) -D_MDUP_PKG_
dsPInst.o_CFLAGS    = $(CFLAGS) -D_MDUP_PKG_
dsPMdup.o_CFLAGS    = $(CFLAGS) -D_MDUP_PKG_
dsPPsap.o_CFLAGS    = $(CFLAGS) -D_MDUP_PKG_
dsPPut.o_CFLAGS     = $(CFLAGS) -D_MDUP_PKG_
dsPWkspace.o_CFLAGS = $(CFLAGS) -D_MDUP_PKG_

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =
LIBS    =

LIBRARIES	= libXDS.a
EXPLIB_TARGETS        = ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS        = ${libXDS_OFILES}

EXPDIR		= /usr/lib/
EXPINC_SUBDIRS	= h
SUBDIRS		= h
INCFLAGS	= -Ih -I../xom/h -I../gds/h

.include <${RULES_MK}>

.endif
