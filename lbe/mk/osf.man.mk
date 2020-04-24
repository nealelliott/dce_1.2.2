#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
#
# HISTORY
# $Log: osf.man.mk,v $
# Revision 1.1.6.2  1996/03/09  23:25:22  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:00  marty]
#
# Revision 1.1.6.1  1995/12/07  22:10:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:44  root]
# 
# Revision 1.1.4.2  1992/12/31  16:47:33  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:51:31  zeliff]
# 
# Revision 1.1.2.2  1992/04/16  14:32:34  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:29:53  marty]
# 
# Revision 1.1  1992/01/19  15:31:00  devrcs
# 	Initial revision
# 
# $EndLog$
#
#

.if !defined(_OSF_MAN_MK_)
_OSF_MAN_MK_=

#
# default nroff program to run
#
NROFF?=nroff

#
# default flags to nroff
#
DEF_NROFFFLAGS?=-man -h

#
# all flags for nroff
#
_NROFFFLAGS_=${${.TARGET}_DEF_NROFFFLAGS:U${DEF_NROFFFLAGS}} ${${.TARGET}_NROFFENV:U${NROFFENV}} ${${.TARGET}_NROFFFLAGS:U${NROFFFLAGS}} ${${.TARGET}_NROFFARGS:U${NROFFARGS}}

#
#  Default single suffix compilation rules
#
.SUFFIXES: .0 .${MANSECTION}

#
#  Default double suffix compilation rules
#
.${MANSECTION}.0:
	${NROFF} ${_NROFFFLAGS_} ${.IMPSRC} > ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}

.endif
