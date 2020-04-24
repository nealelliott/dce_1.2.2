#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.18.1  1996/10/04  20:06:59  arvind
# 	fix NO_GDS support.. gssapi depends on some xds macros.
# 	[1996/09/17  18:29 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
#
# 	More NO_GDS support.
# 	[1996/09/17  05:52 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.16.2  1996/03/09  23:11:22  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:46  marty]
# 
# Revision 1.1.16.1  1995/12/08  00:09:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:53:01  root]
# 
# Revision 1.1.13.3  1994/05/10  15:48:28  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:06:56  marrek]
# 
# Revision 1.1.13.2  1994/02/22  15:19:39  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  08:49:48  marrek]
# 
# Revision 1.1.13.1  1994/01/13  16:07:32  peckham
# 	Add SAMIDL_SUBDIRS for cds.
# 	[1994/01/12  00:11:46  peckham]
# 
# Revision 1.1.11.2  1993/10/14  15:56:06  keutel
# 	October 1993 code drop
# 	[1993/10/14  15:52:51  keutel]
# 
# Revision 1.1.11.1  1993/10/13  17:28:18  keutel
# 	October 1993 code drop (CR 9047)
# 	[1993/10/13  11:40:00  keutel]
# 
# Revision 1.1.9.2  1993/08/11  11:59:02  marrek
# 	July 1993 code drop.
# 	[1993/08/04  09:38:04  marrek]
# 
# Revision 1.1.5.4  1992/12/06  03:02:37  zeliff
# 	Embedding Copyrights
# 	[1992/12/06  03:00:35  zeliff]
# 
# Revision 1.1.5.3  1992/11/27  18:53:52  marrek
# 	November 1992 code drop
# 	[1992/11/23  15:44:17  marrek]
# 
# Revision 1.1.5.2  1992/09/29  18:42:50  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:43:39  treff]
# 
# Revision 1.1.2.5  1992/06/30  20:52:44  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:30:47  melman]
# 
# Revision 1.1.2.4  1992/06/15  17:28:33  melman
# 	Added xmh to EXPINC_SUBDIRS
# 	[1992/06/15  17:28:12  melman]
# 
# Revision 1.1.2.3  1992/04/30  20:55:18  john
# 	Added SETUP_SUBDIRS.
# 	[1992/04/10  14:27:51  john]
# 
# Revision 1.1.2.2  1992/02/07  20:35:59  marty
# 	Null delta, testing dce1.0.1b1 backing tree for rios
# 	[1992/02/07  20:32:33  marty]
# 
# Revision 1.1  1992/01/19  15:28:24  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.18.1 $ $Date: 1996/10/04 20:06:59 $
#

.if !defined(NO_GDS)
SETUP_SUBDIRS	= gds cds
SAMIDL_SUBDIRS	= gds cds
EXPINC_SUBDIRS	= gds cds xds xom xmh xoms
DATA_SUBDIRS	= gds                 xoms
EXPLIB_SUBDIRS	= gds cds xds xom     xoms
SUBDIRS		= gds cds xds xom xmh xoms
.else
SETUP_SUBDIRS	= gds cds
SAMIDL_SUBDIRS	= gds cds
EXPINC_SUBDIRS	= gds cds xds xom
EXPLIB_SUBDIRS	= gds cds
SUBDIRS		= cds
.endif

.include <${RULES_MK}>
