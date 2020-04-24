#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.96.2  1996/03/09  23:14:59  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:17  marty]
#
# Revision 1.2.96.1  1995/12/08  15:38:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:56:38  root]
# 
# Revision 1.2.94.4  1994/03/23  15:49:17  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:38:06  keutel]
# 
# Revision 1.2.94.3  1994/02/24  16:45:17  marrek
# 	SITE switch for SNI Burlington.
# 	[1994/02/24  13:30:20  marrek]
# 
# Revision 1.2.94.2  1994/02/22  18:38:51  ohara
# 	Merged with changes from 1.2.94.1
# 	[1994/02/22  18:38:20  ohara]
# 
# 	use -Xa on SINIX
# 	[1994/02/18  15:59:59  ohara]
# 
# Revision 1.2.94.1  1994/02/22  14:59:44  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  12:06:43  marrek]
# 
# Revision 1.2.91.1  1993/10/14  19:24:02  keutel
# 	October 1993 code drop
# 	[1993/10/14  19:22:57  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/11  10:14:23  keutel]
# 
# Revision 1.2.7.4  1993/08/23  13:57:22  treff
# 	DCT OSF
# 	Get rid of libXOM_r.a, at least temporarily
# 	[1993/08/23  13:56:39  treff]
# 
# Revision 1.2.7.3  1993/08/20  21:38:30  treff
# 	DCT @ OSF
# 	Get rid of ANSI_CC = xlc_r, for many reasons,
# 	but mainly, it doesn't build . . .
# 	[1993/08/20  21:33:33  treff]
# 
# Revision 1.2.7.2  1993/08/10  07:13:08  marrek
# 	Add flags in CFLAGS instead of redefine CFLAGS.
# 	[1993/08/06  07:48:11  marrek]
# 
# 	changes for THREADSAFE: Jochen Keutel
# 	[1993/08/02  08:24:39  marrek]
# 
# 	Add -DSNI for omAlloc because of signal.h error.
# 	[1993/07/30  08:47:51  marrek]
# 
# 	No -Xc for omAlloc on SVR4 because of signal.h error.
# 	[1993/07/30  07:35:21  marrek]
# 
# 	July 1993 code drop.
# 	[1993/07/28  15:58:21  marrek]
# 
# Revision 1.2.5.5  1993/02/02  09:00:45  marrek
# 	*** empty log message ***
# 
# Revision 1.2.5.4  1993/02/02  08:15:13  marrek
# 	Add (${TARGET_MACHINE}=="SVR4") for -Xc flag.
# 	[1993/02/01  14:17:58  marrek]
# 
# Revision 1.2.5.3  1992/12/30  21:16:05  bbelch
# 	Embedding copyright notice
# 	[1992/12/30  21:04:22  bbelch]
# 
# Revision 1.2.5.2  1992/11/30  08:31:21  marrek
# 	November 1992 code drop
# 	[1992/11/25  15:40:43  marrek]
# 
# Revision 1.2.2.3  1992/06/01  23:17:34  melman
# 	New GDS merged drop
# 	[1992/05/29  12:45:13  melman]
# 
# Revision 1.2.2.2  1992/05/11  17:51:00  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:21:45  mhickey]
# 
# Revision 1.2  1992/01/19  22:13:19  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.96.2 $ $Date: 1996/03/09 23:14:59 $
#

.if !defined(GDS_NO_LIBDCE)
RIOS_CFLAGS	+= -D_THREAD_SAFE 
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS} -DTHREADSAFE
.else
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS}
.endif

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

libXOM_OFILES	= omAlloc.o \
		omCode.o \
		omCopy.o \
		omCopyValu.o \
		omCreate.o \
		omDelete.o \
		omEntry.o \
		omGet.o \
		omInst.o \
		omPut.o \
		omRead.o \
		omRemove.o \
		omTrace.o \
		omVersion.o \
		omWkspace.o \
		omWrite.o \
                omXext.o

libXOM.a_OFILES = ${libXOM_OFILES}

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
# 
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS=
LIBS=

LIBRARIES	= libXOM.a
EXPLIB_TARGETS  = ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS	= ${libXOM_OFILES}
EXPDIR			= /usr/lib/
EXPINC_SUBDIRS		= h
SUBDIRS			= h

#INCFLAGS	= -I../xds/h -Ih -I../gds/h -I../xmh/h
INCFLAGS	= -Ih -I../xoms/h

.include <${RULES_MK}>

.endif
