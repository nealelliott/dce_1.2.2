#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.814.2  1996/03/09  23:12:39  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:49  marty]
#
# Revision 1.1.814.1  1995/12/08  15:11:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:49:44  root]
# 
# Revision 1.1.812.3  1994/06/21  14:43:12  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:22:17  marrek]
# 
# Revision 1.1.812.2  1994/03/23  15:02:53  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:01:59  keutel]
# 
# Revision 1.1.812.1  1994/02/22  15:56:11  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:13:43  marrek]
# 
# Revision 1.1.810.2  1993/10/14  16:46:09  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:42:17  keutel]
# 
# Revision 1.1.810.1  1993/10/13  17:29:17  keutel
# 	October 1993 code drop (CR 3100)
# 	[1993/10/13  15:18:03  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/09  19:00:27  keutel]
# 
# Revision 1.1.7.2  1993/08/10  08:36:40  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:05:06  marrek]
# 
# Revision 1.1.5.3  1992/12/31  17:44:55  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:58:26  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  13:39:14  marrek
# 	November 1992 code drop
# 	[1992/11/16  14:35:06  marrek]
# 
# Revision 1.1.2.2  1992/06/01  21:26:07  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  19:07:27  zeliff]
# 
# Revision 1.1  1992/01/19  14:59:42  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.814.2 $ $Date: 1996/03/09 23:12:39 $
#

LIBRARIES	= libaplv2.a
EXPDIR		= /usr/lib/
INCFLAGS	= -I../../h
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

d20calld.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20interf.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20fentry.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20fentryr.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20fattr.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20ffilt.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20dnget.o_CFLAGS = -DDIRV2 ${CFLAGS}

libaplv2.a_OFILES	=  \
        d20calld.o d20interf.o d20fentry.o d20fentryr.o                 \
	d20fattr.o d20ffilt.o d20sattr.o d20dnget.o           \
	d20specdir.o \
	d20init.o    d20fmodify.o d20getattr.o d20subset.o  d20iapl.o \
	d20setinfo.o d20sattr.o   d20smodify.o d20salias.o \
	d20gfattr.o	d20bind.o    d20unbind.o

.include <${RULES_MK}>
