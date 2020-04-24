#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.111.2  1996/03/09  23:12:46  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:55  marty]
#
# Revision 1.1.111.1  1995/12/08  15:21:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:52:56  root]
# 
# Revision 1.1.109.4  1994/06/21  14:43:53  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:25:06  marrek]
# 
# Revision 1.1.109.3  1994/05/10  15:50:14  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:14:33  marrek]
# 
# Revision 1.1.109.2  1994/03/23  15:07:00  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:13:49  keutel]
# 
# Revision 1.1.109.1  1994/02/22  17:35:48  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:37:54  marrek]
# 
# Revision 1.1.107.3  1993/10/20  14:02:42  keutel
# 	CR 9206
# 	[1993/10/20  12:55:22  keutel]
# 
# Revision 1.1.107.2  1993/10/14  16:46:40  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:45:27  keutel]
# 
# Revision 1.1.107.1  1993/10/13  17:29:57  keutel
# 	October 1993 code drop
# 	[1993/10/09  18:59:33  keutel]
# 
# Revision 1.1.9.3  1993/08/10  10:58:47  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:41:02  marrek]
# 
# Revision 1.1.9.2  1993/07/02  15:07:17  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:34:43  marrek]
# 
# Revision 1.1.5.3  1992/12/31  18:10:55  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:07:34  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  15:39:40  marrek
# 	November 1992 code drop
# 	[1992/11/16  16:45:48  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:01:28  melman
# 	New GDS merged drop
# 	[1992/05/28  20:45:34  melman]
# 
# Revision 1.1  1992/01/19  15:01:00  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.111.2 $ $Date: 1996/03/09 23:12:46 $
#

PROGRAMS	= gdssysadm gdsutil gdsdistcmd gdslanguage
ILIST		= ${PROGRAMS}
IDIR		= /bin/

# for d20frame.c 
VPATH = ../../adm/admin/${TARGET_MACHINE}
INCFLAGS	= -I../../h
d20frame.o_CFLAGS	= ${CFLAGS}

# VR added termcap library
gdssysadm_LIBS	= -lsysadm -ladm_msk -ldupd -lcurses -ltermcap ${LIBS}
gdssysadm_OFILES = d20frame.o 

LIBRARIES	= libsysadm.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

libsysadm.a_OFILES	= \
		       	  d20admmsk.o d20admin.o d20param.o d20m_init.o

gdsutil_OFILES	= d20util.o

gdsdistcmd_OFILES	= d20distcmd.o

gdslanguage_OFILES	= d20lngset.o

.include <${RULES_MK}>
