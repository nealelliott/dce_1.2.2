#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.101.2  1996/03/09  23:13:55  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:13  marty]
#
# Revision 1.1.101.1  1995/12/08  15:58:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:12:58  root]
# 
# Revision 1.1.99.3  1994/06/21  14:46:58  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:40:15  marrek]
# 
# Revision 1.1.99.2  1994/03/23  15:18:14  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:29:17  keutel]
# 
# Revision 1.1.99.1  1994/02/22  18:47:02  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:40:43  marrek]
# 
# Revision 1.1.97.3  1993/10/14  17:27:38  keutel
# 	October 1993 code drop
# 	[1993/10/14  17:26:09  keutel]
# 
# Revision 1.1.97.2  1993/10/13  17:32:20  keutel
# 	October 1993 code drop
# 	[1993/10/09  18:42:28  keutel]
# 
# Revision 1.1.97.1  1993/10/04  17:37:48  keutel
# 	use of libdua instead of libdce for gds processes
# 	[1993/10/04  17:35:14  keutel]
# 
# Revision 1.1.7.2  1993/08/11  14:54:45  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:59:02  marrek]
# 
# Revision 1.1.5.4  1992/12/31  20:37:43  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:28:04  bbelch]
# 
# Revision 1.1.5.3  1992/11/27  19:08:21  marrek
# 	November 1992 code drop
# 	[1992/11/20  09:31:07  marrek]
# 
# Revision 1.1.5.2  1992/09/29  21:11:30  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:26:09  treff]
# 
# Revision 1.1.2.3  1992/06/30  20:59:41  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:40:44  melman]
# 
# Revision 1.1.2.2  1992/06/01  19:41:14  zeliff
# 	New files from GDS drop
# 	[1992/05/27  19:39:05  zeliff]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.101.2 $ $Date: 1996/03/09 23:13:55 $
#

PROGRAMS	= genmsg

ILIST		= genmsg 
IDIR		= /bin/
INCFLAGS        = -I../h
CFLAGS		= -DSTOP_TIME

genmsg_OFILES     = genmsg.o createmsg.o checkmsg.o aban_cod.o

genmsg_LIBS	= -lids ${LIBS}

.include <${RULES_MK}>

