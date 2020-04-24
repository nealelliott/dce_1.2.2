#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/09  23:12:47  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:56  marty]
#
# Revision 1.1.11.1  1995/12/08  15:22:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:53:05  root]
# 
# Revision 1.1.9.3  1994/06/21  14:43:59  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:25:35  marrek]
# 
# Revision 1.1.9.2  1994/05/20  22:03:21  sommerfeld
# 	Clearcase cleanup (make dua.h, dua.c translate rules identical)
# 	[1994/05/17  19:27:40  sommerfeld]
# 
# Revision 1.1.9.1  1994/02/22  18:04:20  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:43:30  marrek]
# 
# Revision 1.1.7.2  1993/08/11  10:00:57  marrek
# 	Activate removal of dua.make
# 	[1993/08/04  11:10:10  marrek]
# 
# 	July 1993 code drop.
# 	[1993/07/29  12:28:17  marrek]
# 
# Revision 1.1.5.3  1992/12/31  18:11:21  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:08:12  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  16:25:38  marrek
# 	November 1992 code drop
# 	[1992/11/26  14:45:12  marrek]
# 
# Revision 1.1.2.3  1992/06/01  20:02:33  melman
# 	New GDS merged drop
# 	[1992/05/28  20:46:29  melman]
# 
# Revision 1.1.2.2  1992/04/07  19:07:17  mckeen
# 	Removed definition of MAVROS.  Is now defined in osf.dce.mk.
# 	[1992/03/13  15:39:26  mckeen]
# 
# Revision 1.1  1992/01/19  15:01:08  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:12:47 $
#

LIBRARIES	= libDUA.a
libDUA.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I.. -I../../h
GARBAGE		= dua.c dua.make dua.h gdsauth.c gdsauth.make gdsauth.h

dua_ids.o: dua.h gdsauth.h

dua_dua.o: dua.h gdsauth.h

dua_free.o: dua.h gdsauth.h

ids_free.o: dua.h gdsauth.h

std.o: dua.h gdsauth.h

dua.h: dua.mvr dua_mvr.h gdsauth.h
	${MAVROS} ${dua.mvr:P} \
		-m dua -u -d ${dua_mvr.h:P} -nolinenum
	${RM} ${_RMFLAGS_} dua.make

dua.c: dua.mvr dua_mvr.h gdsauth.h
	${MAVROS} ${dua.mvr:P} \
		-m dua -u -d ${dua_mvr.h:P} -nolinenum
	${RM} ${_RMFLAGS_} dua.make

gdsauth.h: gdsauth.mvr gdsauth_mvr.h
	${MAVROS} ${gdsauth.mvr:P} \
		-m gdsauth -u -d ${gdsauth_mvr.h:P}  -nolinenum

gdsauth.c: gdsauth.mvr gdsauth.h
	${MAVROS} ${gdsauth.mvr:P} \
		-m gdsauth -u -d ${gdsauth_mvr.h:P} -nolinenum

gdsauth.mvr: gdsauth.asn1
	${MAVCOD} -srce ${gdsauth.asn1:P}

gdsauth_mvr.h: gdsauth.asn1
	${MAVCOD} -srce ${gdsauth.asn1:P}

libDUA.a_OFILES	= gdsauth.o dua.o \
        		dua_ids.o dua_dua.o dua_free.o \
			ids_free.o \
			std.o

.include <${RULES_MK}>

