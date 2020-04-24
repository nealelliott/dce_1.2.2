#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.95.2  1996/03/09  23:13:08  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:20  marty]
#
# Revision 1.2.95.1  1995/12/08  15:26:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:09  root]
# 
# Revision 1.2.93.4  1994/06/21  14:44:45  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:29:00  marrek]
# 
# Revision 1.2.93.3  1994/05/10  15:50:48  marrek
# 	Bug fixes and I18N for April 1994 submission.
# 	[1994/05/06  09:58:01  marrek]
# 
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:23:40  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  12:16:12  marrek]
# 
# Revision 1.2.93.2  1994/03/23  15:08:47  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:18:08  keutel]
# 
# Revision 1.2.93.1  1994/02/22  17:52:10  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:50:02  marrek]
# 
# Revision 1.2.91.3  1993/10/14  16:46:42  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:45:36  keutel]
# 
# Revision 1.2.91.2  1993/10/13  17:30:04  keutel
# 	October 1993 code drop
# 	[1993/10/09  18:53:01  keutel]
# 
# Revision 1.2.91.1  1993/10/04  17:37:41  keutel
# 	use of libdua instead of libdce for gds processes
# 	[1993/10/04  17:34:15  keutel]
# 
# Revision 1.2.7.2  1993/08/11  07:12:51  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:10:04  marrek]
# 
# Revision 1.2.5.4  1992/12/31  18:39:31  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:12:15  bbelch]
# 
# Revision 1.2.5.3  1992/10/05  17:09:33  marrek
# 	Changes for bug 5205.
# 	[1992/10/05  17:09:07  marrek]
# 
# Revision 1.2.5.2  1992/09/29  21:09:53  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:23:00  treff]
# 
# Revision 1.2.2.4  1992/06/30  20:55:19  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:34:28  melman]
# 
# Revision 1.2.2.3  1992/06/01  20:12:21  melman
# 	New GDS merged drop
# 	[1992/05/28  21:05:03  melman]
# 
# Revision 1.2.2.2  1992/01/22  22:33:20  jim
# 	Added TARGET_MACHINE_LIBS so that we can include libslog for AIX.
# 	The symbols for libslog.a are not exported from libdce.a on AIX.
# 	[1992/01/22  22:24:47  jim]
# 
# Revision 1.2  1992/01/19  22:13:08  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.95.2 $ $Date: 1996/03/09 23:13:08 $
#

LIBRARIES	= libduac.a
PROGRAMS	= gdscache
ILIST		= gdscache

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
IDIR		= /bin/
INCFLAGS	= -I../h 
d22cache.o_CFLAGS	= -DNOT_CHECK_KEY ${CFLAGS}
d22errhdl.o_CFLAGS	= -DNORMERR ${CFLAGS}

libduac.a_OFILES = \
	d22comp.o    d22cachact.o d22evapar.o  d22hash.o   d22iinsobj.o \
	d22getoffs.o d22vrfydn.o  d22insobj.o  d22delobj.o d22mdelobj.o \
	d22srchobj.o d22cffkt.o   d22reorg.o   d22cicmp.o  d22checkp.o  \
	d22cread.o   d22crmove.o  d22ccomp.o   d22clist.o  d22cadd.o    \
	d22csrch.o   d22errhdl.o  d22attrmsg.o d22filter.o d22convdn.o  \
	d22term.o    d22distcmd.o d22cfilter.o 

gdscache_LIBS = -lduac ${LIBS}

gdscache_OFILES = d22cache.o

.include <${RULES_MK}>

