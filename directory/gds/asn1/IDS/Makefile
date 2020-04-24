#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.11.2  1996/03/09  23:12:51  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:59  marty]
#
# Revision 1.2.11.1  1995/12/08  15:22:51  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:53:17  root]
# 
# Revision 1.2.9.2  1994/05/10  15:50:25  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:14:50  marrek]
# 
# Revision 1.2.9.1  1994/02/22  18:35:48  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:47:54  marrek]
# 
# Revision 1.2.7.2  1993/08/11  10:12:12  marrek
# 	July 1993 code drop.
# 	[1993/07/29  12:30:36  marrek]
# 
# Revision 1.2.5.3  1992/12/31  18:11:58  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:08:58  bbelch]
# 
# Revision 1.2.5.2  1992/11/27  16:34:22  marrek
# 	November 1992 code drop
# 	[1992/11/17  13:39:05  marrek]
# 
# Revision 1.2.2.3  1992/06/01  20:03:42  melman
# 	New GDS merged drop
# 	[1992/05/28  20:50:01  melman]
# 
# Revision 1.2.2.2  1992/04/07  19:07:24  mckeen
# 	Removed definition of MAVROS.  Is now defined in osf.dce.mk.
# 	[1992/03/13  15:39:32  mckeen]
# 
# Revision 1.2  1992/01/19  22:13:07  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.11.2 $ $Date: 1996/03/09 23:12:51 $
#

LIBRARIES	= libids.a

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I..
libids.a_OFILES	= ids.o credext.o

GARBAGE		= ids.c ids.h ids.make credext.c credext.make credext.h

.include <${RULES_MK}>

ids.h: ids.mvr ids_mvr.h
	${MAVROS} ${ids.mvr:P} \
		-m ids -u -d ${ids_mvr.h:P}  -nolinenum

ids.c: ids.mvr ids_mvr.h
	${MAVROS} ${ids.mvr:P} \
		-m ids -u -d ${ids_mvr.h:P}  -nolinenum

credext.h: credext.mvr credext_mvr.h
	${MAVROS} ${credext.mvr:P} \
		-m credext -u -d ${credext_mvr.h:P} -nolinenum

credext.c: credext.mvr credext.h
	${MAVROS} ${credext.mvr:P} \
		-m credext -u -d ${credext_mvr.h:P} -nolinenum

credext.mvr : credext.asn1
	${MAVCOD} -srce ${credext.asn1:P}

credext_mvr.h: credext.asn1
	${MAVCOD} -srce ${credext.asn1:P}
