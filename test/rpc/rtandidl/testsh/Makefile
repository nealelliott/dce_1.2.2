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
# Revision 1.1.99.2  1996/03/11  02:28:01  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:00  marty]
#
# Revision 1.1.99.1  1995/12/11  20:13:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:25  root]
# 
# Revision 1.1.97.1  1993/09/27  18:18:05  tom
# 	Bug 8524 - Remove IDIR and EXPDIR, and let the install default to:
# 	 dcetest/dce1.0/rpc/rtandidl/testsh
# 	[1993/09/27  18:17:34  tom]
# 
# Revision 1.1.8.4  1993/06/16  22:39:49  sommerfeld
# 	Previous submit didn't work correctly
# 	[1993/06/16  22:31:57  sommerfeld]
# 
# Revision 1.1.8.3  1993/06/16  19:15:33  sommerfeld
# 	Comment out OPTFLAGS (this gets handled in common makefiles)
# 	[1993/06/16  17:57:56  sommerfeld]
# 
# Revision 1.1.8.2  1993/05/24  20:48:37  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:16:07  cjd]
# 
# Revision 1.1.6.2  1993/05/12  18:23:09  jd
# 	Iitial 486 port.
# 	[1993/05/12  18:21:59  jd]
# 
# Revision 1.1.2.5  1993/02/26  22:38:57  weisman
# 	very major cleanup
# 	[1993/02/26  22:15:50  weisman]
# 
# Revision 1.1.2.4  1993/01/13  19:51:11  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:18:26  bbelch]
# 
# Revision 1.1.2.3  1992/12/21  21:03:23  weisman
# 	Cloned Marty's logic from idl_compiler/Makefile for using
# 	OSC1.0.4 lex.
# 	[1992/12/21  19:11:38  weisman]
# 
# Revision 1.1.2.2  1992/11/02  15:37:33  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:39:32  root]
# 
# Revision 1.1  1992/01/19  03:41:07  devrcs
# 	Initial revision
# 
# $EndLog$
#
# 
# Copyright 1989 by Apollo Computer, Inc.
# Copyright 1990 by Locus Computing Corp.
# 
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Apollo Computer, Inc not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Apollo Computer, Inc. makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
# 
#

#OPTFLAGS       = -O 	
#DBGFLAGS      = -DYYDEBUG=1 -DLEXDEBUG=1 -DDEBUG
CFLAGS         = $(OPTFLAGS) ${DBGFLAGS} ${${TARGET_MACHINE}_CFLAGS}

YFLAGS         = -d ${${TARGET_MACHINE}_YFLAGS} -l

# Use the OSF 1.0.4 lex as needed.
 
.if defined (USE_OSC1_0_4_LEX)
LEX = env LEXER=${DEFTOOLBASE}/oscncform ${DEFTOOLBASE}/osc104lex
INCFLAGS= -I.
.endif

LIBS           =  -ly -ll

PROGRAMS       = testsh

ILIST		= ${PROGRAMS}

testsh_OFILES     = main.o    tshparse.o   tsh.o   utils.o

.include <${RULES_MK}>

tsh.o     : y.tab.h tsh.h
y.tab.h   : tshparse.y
main.o    : tsh.h
tshparse.o: tsh.h
utils.o   : tsh.h
