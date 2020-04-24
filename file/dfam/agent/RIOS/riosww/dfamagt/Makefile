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
# $Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/Makefile,v 1.1.2.5 1996/03/12 07:13:34 takano Exp $
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.5  1996/03/12  07:13:34  takano
# 	fix OT 13386.
# 	[1996/03/12  07:13:12  takano]
#
# Revision 1.1.2.4  1996/03/11  14:05:48  marty
# 	Update OSF copyright years.
# 	[1996/03/11  13:55:39  marty]
# 
# Revision 1.1.2.3  1996/03/06  06:14:56  takano
# 	modify environment valiable for install_all
# 	[1996/03/06  06:14:34  takano]
# 
# Revision 1.1.2.2  1996/02/23  07:06:34  takano
# 	Modify TAEGET_FLAGS
# 	[1996/02/23  07:05:31  takano]
# 
# Revision 1.1.2.1  1996/02/19  11:59:55  dce-sdc
# 	First Submit
# 	[1996/02/19  02:04:28  dce-sdc]
# 
# Revision 1.1.2.7  96/01/12  04:26:01  04:26:01  kishi (Y.Kishi)
# 	... ODE test ... and Change Copyright
# 	[1996/01/12  04:25:52  kishi]
# 
# Revision 1.1.2.6  1995/10/26  00:30:46  kishi
# 	Change Copyright.
# 	[1995/10/26  00:30:32  kishi]
# 
# Revision 1.1.2.5  1995/10/25  00:24:18  kishi
# 	Change IMODE.  4500 -> 0550.
# 	[1995/10/25  00:24:03  kishi]
# 
# Revision 1.1.2.4  1995/09/23  02:15:46  kishi
# 	Link libpthreads.a .
# 	[1995/09/23  02:15:09  kishi]
# 
# Revision 1.1.2.3  1995/09/19  06:35:27  kishi
# 	Change CFLAGS.
# 	[1995/09/19  06:35:14  kishi]
# 
# Revision 1.1.2.2  1995/09/16  04:46:54  kishi
# 	Change IDIR. /bin/ -> /riosww/bin/
# 	[1995/09/16  04:43:02  kishi]
# 
# 	Change IDIR and add IMODE.
# 	[1995/09/14  05:10:53  kishi]
# 
# Revision 1.1.2.1  1995/09/09  06:54:44  kishi
# 	Initial revision
# 	[1995/09/09  06:14:56  kishi]
# 
# $EndLog$
#

PROGRAMS	= dfaagt

CFLAGS		= -D_FRAGWRITE

TARGET_FLAGS	= -D_ALL_SOURCE -Drios -D_IBMR2 -DAIX_PROD -D_THREAD_SAFE -qchars=signed

INCFLAGS	= -I../h

LIBS		= -ldce

OFILES		= agt_acl.o agt_cmd.o agt_dsync.o agt_file.o agt_init.o \
		  agt_lib.o agt_login.o agt_main.o agt_msg.o agt_parm.o \
		  agt_sck.o agt_sem.o agt_sig.o agt_sigdce.o agt_stf.o \
		  agt_sysc.o agt_term.o agt_time.o agt_timer.o agt_trc.o \
		  agt_usrc.o agt_wchd.o errno.o

ILIST		= ${PROGRAMS}
IDIR		= /bin/
IMODE		= 0550
IOWNER		= root
IGROUP		= bin

.include<${RULES_MK}>
