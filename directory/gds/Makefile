#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.24.1  1996/10/03  14:36:07  arvind
# 	"Enhanced" NO_GDS..
# 	[1996/09/17  00:09 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.22.2  1996/03/09  23:12:30  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:41  marty]
# 
# Revision 1.1.22.1  1995/12/08  15:09:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:49:16  root]
# 
# Revision 1.1.20.10  1994/08/23  15:27:33  keutel
# 	OT 10919: remove gda
# 	[1994/08/23  14:15:33  keutel]
# 
# Revision 1.1.20.9  1994/07/06  15:06:16  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:10:32  marrek]
# 
# Revision 1.1.20.8  1994/06/30  19:37:04  ohara
# 	support for the NO_GDS option
# 	[1994/06/29  20:07:40  ohara]
# 
# Revision 1.1.20.7  1994/06/21  14:43:07  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:22:04  marrek]
# 
# Revision 1.1.20.6  1994/05/10  15:49:06  marrek
# 	Merged with changes from 1.1.20.5
# 	[1994/05/10  15:44:27  marrek]
# 
# 	Move nds from GDSLIB_SUBDIRS to src/Makefile.
# 
# 	April 1994 code submission.
# 	[1994/04/27  12:08:39  marrek]
# 
# Revision 1.1.20.3  1994/04/06  11:50:07  zeliff
# 	OT10299: build libxti when NO_GDS set
# 	[1994/04/06  11:49:54  zeliff]
# 
# Revision 1.1.20.2  1994/03/23  15:02:39  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:00:47  keutel]
# 
# Revision 1.1.20.1  1994/02/22  15:41:14  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  08:50:04  marrek]
# 
# Revision 1.1.18.2  1993/10/14  15:55:16  keutel
# 	October 1993 code drop
# 	[1993/10/14  15:53:22  keutel]
# 
# Revision 1.1.18.1  1993/10/13  17:28:43  keutel
# 	October 1993 code drop
# 	[1993/10/09  18:43:13  keutel]
# 
# Revision 1.1.16.2  1993/06/10  19:17:29  sommerfeld
# 	Add dua subdir even if NO_GDS
# 	[1993/06/08  17:42:02  sommerfeld]
# 
# Revision 1.1.8.5  1992/12/31  17:12:27  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:55:16  bbelch]
# 
# Revision 1.1.8.4  1992/12/16  15:33:08  sommerfe
# 	Fix stupid typo in previous fix.
# 	[1992/12/16  15:31:37  sommerfe]
# 
# Revision 1.1.8.3  1992/12/14  17:38:15  sommerfeld
# 	Turn off most subdirs in GDS if NO_GDS is set.
# 	[1992/12/08  20:27:36  sommerfeld]
# 
# Revision 1.1.8.2  1992/11/27  17:40:43  marrek
# 	November 1992 code drop
# 	[1992/11/23  12:39:40  marrek]
# 
# Revision 1.1.3.3  1992/06/01  20:23:01  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:34:32  zeliff]
# 
# Revision 1.1.3.2  1992/04/30  21:04:18  john
# 	Added SETUP_SUBDIRS.
# 	[1992/04/10  14:36:53  john]
# 
# Revision 1.1  1992/01/19  15:14:31  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# @SNI_COPYRIGHT@
#
# $RCSfile: Makefile,v $ $Revision: 1.1.24.1 $ $Date: 1996/10/03 14:36:07 $
#

.ifndef NO_GDS
GDS_SUBDIRS = cacheadm adm bin cache conf dsa ipc msg osiadr tsi rtros stub nds step gdscp sams
GDSLIB_SUBDIRS = rtros dua gds_sec ipc osiadr util cacheadm asn1 adm cache cisam dsa tsi stub gdscp xti
GDSINC_SUBDIRS = asn1 cisam h tsi rtros stub gdscp
.endif

SAMIDL_SUBDIRS	= sams
EXPINC_SUBDIRS	= sams ASN1 ${GDSINC_SUBDIRS}
EXPLIB_SUBDIRS	= ASN1  ${GDSLIB_SUBDIRS}
EXPBIN_SUBDIRS	= MAVROS MAVCOD
SETUP_SUBDIRS	= MAVROS MAVCOD
SUBDIRS		= ${GDS_SUBDIRS}

.ifndef NO_GDS
OBJECTS = .profile 
SCRIPTS	= .profile.sh

ILIST	= .profile
IDIR	= /share/var/adm/directory/gds/
.endif

.include <${RULES_MK}>
