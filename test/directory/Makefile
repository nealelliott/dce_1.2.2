#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/11  01:43:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:49  marty]
#
# Revision 1.1.12.1  1995/12/08  22:08:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:43:23  root]
# 
# Revision 1.1.10.2  1994/03/23  15:50:41  keutel
# 	cds not built for SNI Munich
# 	[1994/03/21  13:40:16  keutel]
# 
# Revision 1.1.10.1  1994/02/24  15:03:39  marrek
# 	Change rcs string format.
# 	[1994/02/24  12:14:18  marrek]
# 
# Revision 1.1.8.3  1993/08/12  12:44:41  marrek
# 	July 1993 code drop.
# 	[1993/08/04  11:03:15  marrek]
# 
# Revision 1.1.8.2  1993/06/23  18:39:52  jd
# 	Conditionalize gds on NO_GDS.
# 	[1993/06/23  18:38:30  jd]
# 
# Revision 1.1.6.4  1993/02/22  20:43:28  treff
# 	DCT @ OSF
# 	Added "DATA_SUBDIRS = " to move datafiles
# 	into obj tree, where they belong
# 	[1993/02/22  20:33:04  treff]
# 
# Revision 1.1.6.3  1993/01/28  18:07:57  jd
# 	Part of fix for 6530. Cause the cds tests to get built.
# 	[1993/01/28  18:07:37  jd]
# 
# Revision 1.1.6.2  1993/01/05  19:59:45  htf
# 	Embedding copyright notice
# 	[1993/01/05  18:17:09  htf]
# 
# Revision 1.1.2.2  1992/06/11  21:51:06  melman
# 	New GDS test code drop.
# 	[1992/06/11  21:44:38  melman]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/11 01:43:40 $
#

.ifndef NO_GDS
BUILD_GDS = gds 
.endif

.if ${SITE}=="SNI-Munich"
EXPINC_SUBDIRS	= ${BUILD_GDS} xds
EXPLIB_SUBDIRS	= ${BUILD_GDS} xds
EXPBIN_SUBDIRS	= ${BUILD_GDS} xds
SUBDIRS		= ${BUILD_GDS} xds
DATA_SUBDIRS	= ${BUILD_GDS} xds
.else
EXPINC_SUBDIRS	= ${BUILD_GDS} xds cds
EXPLIB_SUBDIRS	= ${BUILD_GDS} xds cds
EXPBIN_SUBDIRS	= ${BUILD_GDS} xds cds
SUBDIRS		= ${BUILD_GDS} xds cds
DATA_SUBDIRS	= ${BUILD_GDS} xds cds
.endif

.include <${RULES_MK}>

