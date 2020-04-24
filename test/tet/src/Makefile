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
# Revision 1.1.6.2  1996/03/11  02:49:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:29  marty]
#
# Revision 1.1.6.1  1995/12/11  22:55:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:25:00  root]
# 
# Revision 1.1.4.1  1994/02/10  22:50:25  rousseau
# 	Added Tcl API.
# 	[1994/02/10  22:49:49  rousseau]
# 
# 	Created TCL API.
# 	[1993/12/17  21:02:32  rousseau]
# 
# Revision 1.1.2.2  1993/07/20  16:06:42  cmckeen
# 	Initial DCE version
# 	[1993/07/15  15:34:58  cmckeen]
# 
# $EndLog$
#

.if exists(dtet/Makefile)
DTET = dtet
.endif

EXPLIB_SUBDIRS	= posix_c ${DTET}
SUBDIRS		= posix_c ksh xpg3sh tcl ${DTET}
DATA_SUBDIRS	= ${SUBDIRS}

.include <${RULES_MK}>
