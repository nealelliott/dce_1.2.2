#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# Trade Secret of Hitachi, Ltd.
# Do not disclose without written concession from Hitachi, Ltd.
# Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
#
# $Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/setdfakey/Makefile,v 1.1.2.4 1996/03/09 20:44:29 marty Exp $
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.4  1996/03/09  20:44:29  marty
# 	Add OSF copyright
# 	[1996/03/09  20:24:19  marty]
#
# Revision 1.1.2.3  1996/03/06  06:47:58  takano
# 	modify environment valiable for install_all
# 	[1996/03/06  06:47:30  takano]
# 
# Revision 1.1.2.2  1996/02/23  07:11:09  takano
# 	Modify TARGET_FLAGS
# 	[1996/02/23  07:10:45  takano]
# 
# Revision 1.1.2.1  1996/02/16  04:58:56  dce-sdc
# 	First Submit
# 	[1996/02/16  04:58:28  dce-sdc]
# 
# Revision 1.1.2.4  96/01/12  06:54:16  06:54:16  kishi (Y.Kishi)
# 	... ODE test ... and Change Copyright
# 	[1996/01/12  06:54:08  kishi]
# 
# Revision 1.1.2.3  1995/10/25  12:11:50  kishi
# 	Change Copyright.
# 	[1995/10/25  12:11:40  kishi]
# 
# Revision 1.1.2.2  1995/09/16  04:47:15  kishi
# 	Change IDIR. /bin/ -> /riosww/bin/
# 	[1995/09/16  04:43:56  kishi]
# 
# 	Change IDIR. Add IMODE.
# 	[1995/09/14  05:12:09  kishi]
# 
# Revision 1.1.2.1  1995/09/09  07:54:55  kishi
# 	Initial revision
# 	[1995/09/09  07:54:04  kishi]
# 
# $EndLog$
#

PROGRAMS	= setdfakey

LIBS 		= -ldce 

CFLAGS 		= -D${TARGET_MACHINE}

TARGET_FLAGS	= -D_ALL_SOURCE -Drios -D_IBMR2 -DAIX_PROD -D_THREAD_SAFE -qchars=signed

INCFLAGS 	= -I../h

OFILES		= agt_sdk.o

ILIST		= ${PROGRAMS}
IDIR		= /bin/
IMODE		= 4550
IOWNER		= root
IGROUP		= bin

.include<${RULES_MK}>
