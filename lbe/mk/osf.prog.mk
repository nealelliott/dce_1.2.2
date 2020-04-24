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
# $Log: osf.prog.mk,v $
# Revision 1.1.17.1  1996/08/09  12:01:53  arvind
# 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
# 	[1996/08/08  20:01:59  arvind]
#
# Revision 1.1.15.2  1996/03/09  23:25:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:03  marty]
# 
# Revision 1.1.15.1  1995/12/07  22:10:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:53  root]
# 
# Revision 1.1.13.1  1994/07/26  20:52:02  annie
# 	added VISTA - 11163
# 	[1994/07/26  20:51:31  annie]
# 
# Revision 1.1.4.5  1992/12/31  16:48:00  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:04  zeliff]
# 
# Revision 1.1.4.4  1992/12/21  18:33:14  john
# 	Replacec _CC_ with _CC_LD_.
# 	[1992/12/21  18:32:56  john]
# 
# Revision 1.1.4.3  1992/12/21  18:00:57  john
# 	Pulled out change.
# 	[1992/12/21  18:00:36  john]
# 
# Revision 1.1.4.2  1992/12/21  17:33:09  john
# 	Changed the link command from _CC_ to _CC_LD_ to allow a seperate linker and
# 	compiler.
# 	[1992/12/21  15:30:30  john]
# 
# Revision 1.1.2.2  1992/04/16  14:33:03  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:30:24  marty]
# 
# Revision 1.1  1992/01/19  15:31:07  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

.if !defined(_OSF_PROG_MK_)
_OSF_PROG_MK_=

#
#  Build rules
#
.if defined(PROGRAMS)

${PROGRAMS}: $${_OFILES_}

.if defined(VISTALIB)
	${_CC_LD_} ${_LDFLAGS_} -o ${.TARGET}.X ${_OFILES_} ${VISTA:D${VISTALIB}} ${_LIBS_}
.else
	${_CC_LD_} ${_LDFLAGS_} -o ${.TARGET}.X ${_OFILES_} ${_LIBS_}
.endif
	${MV} ${.TARGET}.X ${.TARGET}
.endif

.endif
