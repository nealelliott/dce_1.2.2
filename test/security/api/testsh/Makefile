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
# Revision 1.1.9.2  1996/03/11  02:35:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:09  marty]
#
# Revision 1.1.9.1  1995/12/11  21:50:37  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/05  17:34 UTC  root
# 	add install of local_rgy.tet and security.fun.ksh
# 	[1995/12/11  20:53:16  root]
# 
# Revision 1.1.7.2  1993/05/24  20:48:49  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:16:16  cjd]
# 
# Revision 1.1.5.2  1993/05/12  18:20:06  jd
# 	Initial 486 port
# 	[1993/05/12  18:19:28  jd]
# 
# Revision 1.1.2.4  1992/12/31  17:34:55  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:59:19  htf]
# 
# Revision 1.1.2.3  1992/12/23  21:08:39  sekhar
# 	[OT 6543]  Copied# (and modified slightly) Marty's logic from
# 	           idl_compiler/Makefile to use OSC1.0.4 lex
# 	[1992/12/23  20:15:02  sekhar]
# 
# Revision 1.1.2.2  1992/11/02  15:38:15  root
# 	Add ILIST for install pass
# 	[1992/10/28  20:40:56  root]
# 
# Revision 1.1  1992/01/19  04:07:07  devrcs
# 	Initial revision
# 
# $EndLog$
#
# 
# DCE Security Services
# test/security/api/testsh Makefile
#

# Compiler flags for this directory
#DBGFLAGS      = -DYYDEBUG=1 -DLEXDEBUG=1 -DDEBUG
CFLAGS         = ${${TARGET_MACHINE}_CFLAGS}

YFLAGS         = -d ${${TARGET_MACHINE}_YFLAGS}

# Use the OSF 1.0.4 lex as needed.
 
.if defined (USE_OSC1_0_4_LEX)
LEX = env LEXER=${DEFTOOLBASE}/oscncform ${DEFTOOLBASE}/osc104lex
INCFLAGS= -I.
.endif

# Programs to build
PROGRAMS       = testsh

DATAFILES      = local_rgy.tet security.fun.ksh

ILIST		= ${PROGRAMS} ${DATAFILES}

testsh_OFILES  = main.o     \
		 tshparse.o \
		 tsh.o	

# Libraries to link the program with
LIBS           =  -ly -ll

.include <${RULES_MK}>

# Rules and dependencies
tsh.o     : y.tab.h tsh.h
y.tab.h   : tshparse.y
main.o    : tsh.h
tshparse.o: tsh.h
lex.yy.c  : tsh.l
	    ${LEX} ${_LFLAGS_} -t ${.ALLSRC} > ${.TARGET}





