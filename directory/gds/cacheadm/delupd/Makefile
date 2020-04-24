#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/09  23:13:13  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:25  marty]
#
# Revision 1.1.11.1  1995/12/08  15:28:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:42  root]
# 
# Revision 1.1.9.2  1994/03/23  15:08:52  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:18:24  keutel]
# 
# Revision 1.1.9.1  1994/02/22  16:03:48  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:56:23  marrek]
# 
# Revision 1.1.7.2  1993/08/11  08:16:39  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:17:14  marrek]
# 
# Revision 1.1.5.5  1992/12/31  18:49:10  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:14:52  bbelch]
# 
# Revision 1.1.5.4  1992/11/27  17:18:58  marrek
# 	November 1992 code drop
# 	[1992/11/17  16:40:00  marrek]
# 
# Revision 1.1.5.3  1992/10/05  17:20:44  marrek
# 	Changes for bug 5205.
# 	[1992/10/05  17:20:21  marrek]
# 
# Revision 1.1.5.2  1992/09/29  21:10:02  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:23:16  treff]
# 
# Revision 1.1.2.2  1992/06/01  23:33:58  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  20:39:19  zeliff]
# 
# Revision 1.1  1992/01/19  15:14:26  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:13:13 $
#

LIBRARIES	= libdupdc.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
IDIR		= /bin/
GARBAGE		= d20delutilc.c d20displayc.c d21timec.c
# for picking up sources
VPATH		= ../../adm/delupd

INCFLAGS	= -I../../h

d20delutilc.o_CFLAGS = -DCACHE_ADMIN ${CFLAGS}
d20displayc.o_CFLAGS = -DCACHE_ADMIN ${CFLAGS}

d21timec.o_CFLAGS =  -DCACHE_ADMIN ${CFLAGS}

libdupdc.a_OFILES = \
	d20cupd.o   d20delutilc.o d20displayc.o \
	d21cache.o   d21lock.o    d21timec.o    \
	d21trace.o   


.include <${RULES_MK}>

d20delutilc.c:	d20delutil.c
	rm -f $@
	cp $> $@

d20displayc.c:	d20display.c
	rm -f $@
	cp $> $@

d21timec.c:	d21time.c
	rm -f $@
	cp $> $@
