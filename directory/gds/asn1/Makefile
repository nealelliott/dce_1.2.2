# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.2  1996/03/09  23:12:54  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:04  marty]
#
# Revision 1.1.13.1  1995/12/08  15:23:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:53:21  root]
# 
# Revision 1.1.11.2  1994/06/21  14:44:05  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:25:59  marrek]
# 
# Revision 1.1.11.1  1994/02/22  18:36:26  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:39:26  marrek]
# 
# Revision 1.1.9.2  1993/08/11  10:15:16  marrek
# 	July 1993 code drop.
# 	[1993/07/29  12:31:18  marrek]
# 
# Revision 1.1.5.3  1992/12/31  18:12:10  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:09:11  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  15:53:26  marrek
# 	November 1992 code drop
# 	[1992/11/17  12:47:05  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:04:05  melman
# 	New GDS merged drop
# 	[1992/05/28  21:00:53  melman]
# 
# Revision 1.1  1992/01/19  15:01:59  devrcs
# 	Initial revision
# 
# $EndLog$
# 
# $RCSfile: Makefile,v $ $Revision: 1.1.13.2 $ $Date: 1996/03/09 23:12:54 $
# 

EXPLIB_SUBDIRS	= IDS DUA
# IDS has to be done before DUA, because DUA compiles dua.c, which includes
# ids.h, which is created by IDS
LIBRARIES	= libd24asn.a libdirasn.a libd2aasn.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I../h
GARBAGE		= d24aaddn.c d24abindn.c d24acompn.c d24aerrorn.c \
		  d24alistn.c d24amodn.c d24amodrdnn.c d24areadn.c \
		  d24aremn.c d24asearchn.c d24asn1n.c

d24aaddn.o_CFLAGS	= -DDSA ${CFLAGS}
d24abindn.o_CFLAGS	= -DDSA ${CFLAGS}
d24acompn.o_CFLAGS	= -DDSA ${CFLAGS}
d24aerrorn.o_CFLAGS	= -DDSA ${CFLAGS}
d24alistn.o_CFLAGS	= -DDSA ${CFLAGS}
d24amodn.o_CFLAGS	= -DDSA  ${CFLAGS}
d24amodrdnn.o_CFLAGS	= -DDSA ${CFLAGS}
d24areadn.o_CFLAGS	= -DDSA ${CFLAGS}
d24aremn.o_CFLAGS	= -DDSA ${CFLAGS}
d24asearchn.o_CFLAGS	= -DDSA ${CFLAGS}
d24asn1n.o_CFLAGS	= -DDSA  ${CFLAGS}

d25asn1.o_CFLAGS	= -DDSA ${CFLAGS}

LDFLAGS			= ${${TARGET_MACHINE}_LDFLAGS}

libd24asn.a_OFILES = \
	d24aadd.o     d24abind.o    d24acomp.o    d24aerror.o \
	d24alist.o    d24amod.o     d24amodrdn.o  d24aread.o    d24arem.o \
	d24asearch.o  d24asn1.o

libd2aasn.a_OFILES = \
	d2aattr.o     d2acommon.o   d2ads_dn.o    d2aref.o \
	d2amhs.o      d2amhs1.o  d2asap.o

libdirasn.a_OFILES = \
	d24aaddn.o     d24abindn.o    d24acompn.o    d24aerrorn.o \
	d24alistn.o    d24amodn.o     d24amodrdnn.o  d24areadn.o    d24aremn.o \
	d24asearchn.o  d24asn1n.o     d24acharg.o \
	d25aadd.o      d25abind.o     d25acharg.o   d25acomp.o    d25aerror.o \
	d25alist.o     d25amod.o      d25amodrdn.o  d25aread.o    d25arem.o \
	d25asearch.o   d25asn1.o      

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>


d24aaddn.c:	d24aadd.c
	rm -f $@
	cp $> $@

d24abindn.c:	d24abind.c
	rm -f $@
	cp $> $@

d24acompn.c:	d24acomp.c
	rm -f $@
	cp $> $@

d24aerrorn.c:	d24aerror.c
	rm -f $@
	cp $> $@

d24alistn.c:	d24alist.c
	rm -f $@
	cp $> $@

d24amodn.c:	d24amod.c
	rm -f $@
	cp $> $@

d24amodrdnn.c:	d24amodrdn.c
	rm -f $@
	cp $> $@

d24areadn.c:	d24aread.c
	rm -f $@
	cp $> $@

d24aremn.c:	d24arem.c
	rm -f $@
	cp $> $@

d24asearchn.c:	d24asearch.c
	rm -f $@
	cp $> $@

d24asn1n.c:	d24asn1.c
	rm -f $@
	cp $> $@

