#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.6.2  1996/03/09  23:26:10  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:48  marty]
#
# Revision 1.1.6.1  1995/12/08  00:14:19  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:55:50  root]
# 
# Revision 1.1.4.3  1993/01/03  22:35:12  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:50:44  bbelch]
# 
# Revision 1.1.4.2  1992/12/23  19:37:07  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:48:53  zeliff]
# 
# Revision 1.1.2.4  1992/05/26  21:41:10  garyf
# 	move compilation options to rpc.mk
# 	[1992/05/26  20:55:00  garyf]
# 
# Revision 1.1.2.3  1992/05/20  18:30:59  garyf
# 	cleanup compilation options
# 	fix history
# 	[1992/05/20  18:24:23  garyf]
# 
# Revision 1.1.2.2  1992/04/09  18:50:23  mckeen
# 	No longer set compiler values.  Only set OBJECT_FORMAT
# 	[1992/04/09  18:30:34  mckeen]
# 
# Revision 1.1  1992/01/19  22:13:31  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# 	 OSF DCE Version 1.0
# 	Revision 1.1.2.3  91/07/16  12:41:23  hermi
# 	Added -mregparm and -mnosb to CFLAGS.
# 	[91/07/16  12:39:54  hermi]
# 
# Revision 1.1.2.2  91/07/15  11:06:10  hermi
# 	Added MMAX_SFLAGS to move machine specific assembler
# 	directives out of the Makefile.
# 	Initial MMAX revision created by cribbing PMAX version.
# 	[91/07/15  11:04:49  hermi]
# 
# $EndLog$

MMAX_OBJS	= dce_exc_context.o

dce_exc_context.S: dce_exc_context.s
	${RM} -f $@
	${CP} ${dce_exc_context.s:P} $@

dce_exc_context.o: dce_exc_context.S
	${ANSI_CC} -c ${CFLAGS}  -DASSEMBLER ${${TARGET_MACHINE}_SFLAGS} ${INCDIRS} ${@:.o=.S}

# Operating-system-specific kernel RPC modules
OSF1_OBJS	= comsoc_sys.o ipnaf_sys.o pthread_sys.o uuidsys.o

