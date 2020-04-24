#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.106.2  1996/03/09  23:14:34  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:55  marty]
#
# Revision 1.1.106.1  1995/12/08  16:06:45  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1994/08/17  22:34 UTC  dtruong
# 	Merge from Hpdce02_01
# 	[1995/12/08  15:16:52  root]
# 
# Revision 1.1.104.5  1994/07/11  21:06:13  keutel
# 	OT 10732: gdsgennsap runs during installation
# 	[1994/07/11  21:05:13  keutel]
# 
# Revision 1.1.104.4  1994/05/10  15:57:29  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:43:37  marrek]
# 
# Revision 1.1.104.3  1994/02/23  16:55:46  keutel
# 	OT 9997: added use of inlib on OSF1
# 	[1994/02/23  16:41:11  keutel]
# 
# Revision 1.1.104.2  1994/02/22  16:10:53  marrek
# 	February 1994 code drop
# 	[1994/02/16  11:18:34  marrek]
# 
# 	Changes for libdce.
# 	[1994/02/10  17:04:41  marrek]
# 
# 	Code drop for dce1.1.
# 	[1994/02/08  10:59:59  marrek]
# 
# Revision 1.1.104.1  1994/01/13  09:18:30  marrek
# 	Add ${TARGET_MACHINE} in LOCALFILES path.
# 	[1994/01/12  12:06:17  marrek]
# 
# Revision 1.1.102.3  1993/10/20  14:02:43  keutel
# 	CR 9206
# 	[1993/10/20  12:55:22  keutel]
# 
# Revision 1.1.102.2  1993/10/14  17:52:22  keutel
# 	October 1993 code drop
# 	[1993/10/14  17:42:28  keutel]
# 
# Revision 1.1.102.1  1993/10/13  17:33:44  keutel
# 	October 1993 code drop
# 	[1993/10/09  18:51:15  keutel]
# 
# Revision 1.1.9.2  1993/08/11  12:31:10  marrek
# 	Remove handle_psap.
# 	[1993/08/03  09:35:49  marrek]
# 
# 	July 1993 code drop.
# 	[1993/07/30  14:29:10  marrek]
# 
# Revision 1.1.5.5  1993/01/06  20:51:32  zeliff
# 	Added -f as a flag to CP of datafiles
# 	[1993/01/06  20:48:48  zeliff]
# 
# Revision 1.1.5.4  1992/12/31  20:47:31  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:33:46  bbelch]
# 
# Revision 1.1.5.3  1992/12/04  18:09:37  marrek
# 	November 1992 code drop
# 	insert encodeoct.o
# 	[1992/12/04  12:08:57  marrek]
# 
# Revision 1.1.5.2  1992/11/27  18:10:33  marrek
# 	November 1992 code drop
# 	[1992/11/23  13:33:54  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:59:40  melman
# 	New GDS merged drop
# 	[1992/05/28  21:51:55  melman]
# 
# Revision 1.1  1992/01/19  15:14:18  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.106.2 $ $Date: 1996/03/09 23:14:34 $
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
##########################################################
#
libosiaddr.a_OFILES = \
		encodepsap.o encodensap.o decodepsap.o decodensap.o \
		osidat.o     cmposiaddr.o decodeoct.o  encodeoct.o  \
		ldformats.o  hdladmpsap.o convert.o

libosiaddr_OFILES   = ${libosiaddr.a_OFILES}

#
# The rest of the Makefile is private for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#

PROGRAMS	= gdsgennsap

LIBRARIES	= libosiaddr.a
ILIST		= ${PROGRAMS}

libosiaddr.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS	= ${libosiaddr_OFILES}

INCFLAGS	= -I../h
IDIR		= /bin/

CFLAGS	+= -DNODEBUG

gdsgennsap_OFILES = gennsap.o
gdsgennsap_LIBS = ${LIBS}

.include <${RULES_MK}>

.endif
