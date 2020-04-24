#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.14.2  1996/03/09  23:12:44  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:54  marty]
#
# Revision 1.2.14.1  1995/12/08  15:18:30  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:52:20  root]
# 
# Revision 1.2.12.5  1994/08/23  15:27:38  keutel
# 	OT 10919: gda_util ---> cdsutil
# 	[1994/08/23  14:17:50  keutel]
# 
# Revision 1.2.12.4  1994/06/21  14:43:46  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:24:52  marrek]
# 
# Revision 1.2.12.3  1994/05/10  15:49:56  marrek
# 	Bug fixes for April 1994 submission.
# 	[1994/05/04  14:26:48  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  12:13:31  marrek]
# 
# Revision 1.2.12.2  1994/03/23  15:06:40  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:12:13  keutel]
# 
# Revision 1.2.12.1  1994/02/22  17:35:12  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:33:30  marrek]
# 
# Revision 1.2.10.5  1993/10/20  14:02:40  keutel
# 	CR 9206
# 	[1993/10/20  12:55:21  keutel]
# 
# Revision 1.2.10.4  1993/10/14  16:46:39  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:45:26  keutel]
# 
# Revision 1.2.10.3  1993/10/13  17:29:56  keutel
# 	October 1993 code drop (CR 3100)
# 	[1993/10/13  11:58:23  keutel]
# 
# Revision 1.2.10.2  1993/10/05  16:48:07  zeliff
# 	Defect 8954: removed ${TARGET_MACHINE}_LIBS from gdsdaemon_LIBS and added -lslog
# 	[1993/10/05  16:23:48  zeliff]
# 
# Revision 1.2.10.1  1993/10/04  17:37:39  keutel
# 	use of libdua instead of libdce for gds processes
# 	[1993/10/04  17:33:04  keutel]
# 
# Revision 1.2.7.2  1993/08/10  10:32:23  marrek
# 	Remove double libraries in gdsdaemon_LIBS and use -lisam fro SVR4.
# 	[1993/07/30  09:17:17  marrek]
# 
# 	July 1993 code drop.
# 	[1993/07/30  08:35:27  marrek]
# 
# Revision 1.2.5.5  1992/12/31  18:09:46  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:05:48  bbelch]
# 
# Revision 1.2.5.4  1992/12/09  16:58:50  marrek
# 	November 1992 code drop
# 	remove duplicated -ld2aasn from gdsdaemon_LIBS
# 	[1992/12/09  16:57:50  marrek]
# 
# Revision 1.2.5.3  1992/11/27  14:54:48  marrek
# 	November 1992 code drop
# 	[1992/11/16  16:08:24  marrek]
# 
# Revision 1.2.5.2  1992/09/29  21:09:46  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:22:35  treff]
# 
# Revision 1.2.2.4  1992/06/30  20:54:34  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:33:30  melman]
# 
# Revision 1.2.2.3  1992/06/01  22:03:14  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  19:45:40  zeliff]
# 
# Revision 1.2.2.2  1992/01/22  22:33:16  jim
# 	add TARGET_MACHINE_LIBS so gds will build on AIX.
# 	[1992/01/22  22:18:48  jim]
# 
# Revision 1.2  1992/01/19  22:13:06  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.14.2 $ $Date: 1996/03/09 23:12:44 $
#

LIBRARIES	= libdupd.a
PROGRAMS	= gdsdaemon gdsexec gdscacheupd
ILIST		= ${PROGRAMS}

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
IDIR		= /bin/

INCFLAGS	= -I../../h

d20daemon.o_CFLAGS = -DDAEMON  ${CFLAGS}
d20updutil.o_CFLAGS = -DDAEMON -DDSA ${CFLAGS}

gdsdaemon_LIBS = \
			-ldirasn -ld2aasn -lDUA -lids \
			-ldsa_sec \
			-ld26dsa -ld26isam \
	                -ld26dae -lisam -ldsal ${LIBS}

gdscacheupd_LIBS = -lduac -lcdsutil ${LIBS}

libdupd.a_OFILES = \
	d20create.o  d20cupd.o   d20delutil.o d20display.o d20errfile.o \
	d20remove.o  d20update.o d20delupd.o  d20updconfig.o            \
	d21cache.o   d21dpsub.o  d21dutil.o   d21getjob.o  d21gnshd.o   \
	d21lock.o    d21mabind.o d21param.o   d21remove.o  d21time.o    \
	d21trace.o   d21update.o

gdsdaemon_OFILES  = d20daemon.o  d20updutil.o
gdsexec_OFILES  = d20exec.o

gdscacheupd_OFILES = d20cupcach.o d20atcmp.o

.include <${RULES_MK}>
