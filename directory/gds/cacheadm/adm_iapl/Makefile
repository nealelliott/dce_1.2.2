#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.813.2  1996/03/09  23:13:10  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:22  marty]
#
# Revision 1.1.813.1  1995/12/08  15:28:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:39  root]
# 
# Revision 1.1.811.4  1994/07/15  13:29:27  marrek
# 	Remove mess.o_CFLAGS for SVR4.
# 	[1994/07/15  12:09:47  marrek]
# 
# Revision 1.1.811.3  1994/06/22  11:59:32  zeliff
# 	ot11033: removed mess.o from object list
# 	[1994/06/22  11:58:59  zeliff]
# 
# Revision 1.1.811.2  1994/03/23  15:08:51  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:18:21  keutel]
# 
# Revision 1.1.811.1  1994/02/22  16:03:44  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:55:59  marrek]
# 
# Revision 1.1.809.3  1993/10/19  15:03:49  keutel
# 	CR 9189
# 	[1993/10/19  15:03:40  keutel]
# 
# Revision 1.1.809.2  1993/10/14  16:46:43  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:45:37  keutel]
# 
# Revision 1.1.809.1  1993/10/13  17:30:06  keutel
# 	October 1993 code drop
# 	[1993/10/09  19:01:33  keutel]
# 
# Revision 1.1.7.2  1993/08/11  07:53:30  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:16:50  marrek]
# 
# Revision 1.1.5.4  1992/12/31  18:42:28  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:14:40  bbelch]
# 
# Revision 1.1.5.3  1992/12/07  13:18:59  zeliff
# 	Removed duplicate OSF copyright markers
# 	[1992/12/06  00:42:53  zeliff]
# 
# Revision 1.1.5.2  1992/11/27  17:18:51  marrek
# 	November 1992 code drop
# 	[1992/11/17  16:39:44  marrek]
# 
# Revision 1.1.2.2  1992/06/01  23:33:34  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  20:38:56  zeliff]
# 
# Revision 1.1  1992/01/19  15:14:25  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.813.2 $ $Date: 1996/03/09 23:13:10 $
#

LIBRARIES	= libaplv2c.a 
EXPDIR		= /usr/lib/
INCFLAGS	= -I../../h
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
GARBAGE		= d20calldc.c

# for picking up sources
VPATH		= ../../adm/adm_iapl

d20calld.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20calldc.o_CFLAGS = -DCACHE_ADMIN -DDIRV2 ${CFLAGS}
d20interf.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20fentry.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20fentryr.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20fattr.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20ffilt.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20dnget.o_CFLAGS = -DDIRV2 ${CFLAGS}

libaplv2c.a_OFILES	=  \
        d20calldc.o d20fentry.o d20fentryr.o                 \
	d20fattr.o d20sattr.o d20dnget.o           \
	d20specdir.o d20bind.o    d20unbind.o	\
	d20getattr.o d20subset.o  d20setinfo.o \
	d20ffilt.o d20init.o  d20iapl.o

.include <${RULES_MK}>

d20calldc.c:	d20calld.c
	rm -f $@
	cp $> $@
