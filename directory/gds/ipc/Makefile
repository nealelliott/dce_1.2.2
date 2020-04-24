#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.102.2  1996/03/09  23:13:57  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:15  marty]
#
# Revision 1.1.102.1  1995/12/08  16:02:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:14:49  root]
# 
# Revision 1.1.100.4  1994/06/21  14:47:23  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:42:03  marrek]
# 
# Revision 1.1.100.3  1994/05/10  15:56:14  marrek
# 	Bug fixes and I18N for April 1994 submission.
# 	[1994/05/06  09:57:59  marrek]
# 
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:23:20  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  12:39:15  marrek]
# 
# Revision 1.1.100.2  1994/03/23  15:18:48  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:31:29  keutel]
# 
# Revision 1.1.100.1  1994/02/22  18:08:39  marrek
# 	February 1994 code drop
# 	[1994/02/16  11:18:28  marrek]
# 
# 	Changes for libdce.
# 	[1994/02/10  17:04:37  marrek]
# 
# 	Code drop for dce1.1.
# 	[1994/02/08  10:50:56  marrek]
# 
# Revision 1.1.98.1  1993/10/04  17:37:50  keutel
# 	use of libdua instead of libdce for gds processes
# 	[1993/10/04  17:35:29  keutel]
# 
# Revision 1.1.7.2  1993/08/11  11:59:56  marrek
# 	July 1993 code drop.
# 	[1993/07/30  14:24:09  marrek]
# 
# Revision 1.1.5.3  1992/12/31  20:44:00  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:31:35  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  17:31:13  marrek
# 	November 1992 code drop
# 	[1992/11/20  14:05:58  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:52:29  melman
# 	New GDS merged drop
# 	[1992/05/28  21:39:52  melman]
# 
# Revision 1.1  1992/01/19  15:06:03  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.102.2 $ $Date: 1996/03/09 23:13:57 $
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
##########################################################
#

libd23_OFILES	= ${libd23.a_OFILES}
libd23.a_OFILES	= \
		d23sattach.o   d23sdetach.o   d23event.o      d23util.o \
		d23waitin.o    d23result.o    d23invoke.o     d23waitrs.o \
		d23usrcred.o   d23cdetach.o   d23cattach.o    d23bind.o \
                d23unbind.o    d23retbuf.o    d23ipcinfo.o    d23svrinfo.o \
                d23heap.o      d23distcmd.o   d23regproc.o    d23spstat.o \
		d23abort.o     d23abandon.o

#
# The rest of the Makefile is private for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#

LIBRARIES	= libd23.a

PROGRAMS	= gdsipcinit  gdsipcstat  gdsdirinfo gdsipcchk
ILIST		= ${PROGRAMS}

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS	= ${libd23_OFILES}
IDIR		= /bin/

INCFLAGS	= -I../h

IDLFILES	= service.idl
IDLFLAGS	= -keep c_source

VPATH		= ../../../dce/utils/svc

gdsipcinit_OFILES = d23ipcinit.o

gdsipcstat_OFILES = d23ipcstat.o

gdsdirinfo_OFILES = d23dirinfo.o

gdsipcchk_OFILES  = d23ipcchk.o service_sstub.o d23svcserv.o

gdsipcchk_LIBS = ${LIBS}

gdsdirinfo_LIBS = ${LIBS}

gdsipcinit_LIBS = ${LIBS}

gdsipcstat_LIBS = ${LIBS}

d23svcserv.o:	service.h

.include <${RULES_MK}>

.endif
