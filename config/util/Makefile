# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.7.2  1996/03/09  23:10:31  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:21  marty]
#
# Revision 1.2.7.1  1995/12/07  21:40:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:57:10  root]
# 
# Revision 1.2.4.3  1992/12/18  21:28:36  tom
# 	Bug 6424 - Add getip program for dce_config.
# 	Also remove -lbsd for PMAX.
# 	[1992/12/18  21:26:14  tom]
# 
# Revision 1.2.4.2  1992/12/06  03:06:31  zeliff
# 	Embedding copyrights
# 	[1992/12/05  22:57:50  zeliff]
# 
# Revision 1.2.2.2  1992/05/13  23:12:24  sommerfeld
# 	Add HP_PA_LIBS
# 	[1992/05/12  18:26:41  sommerfeld]
# 
# Revision 1.2  1992/01/19  22:12:51  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#
# 
#
#
# Makefile for the configuration library 
#

PROGRAMS	= getcellname getip
ILIST		= ${PROGRAMS}
IDIR		= /bin/
EXPDIR		= /bin/

getcellname_OFILES	= getcellname.o
getip_OFILES		= getip.o

HP_PA_LIBS	= -L/usr/lib -lpthreads -lmach -lc_r -lc
LIBS		= -ldce ${${TARGET_MACHINE}_LIBS}
getip.o_LIBS	=
getip_LIBS	=

.include <${RULES_MK}>

