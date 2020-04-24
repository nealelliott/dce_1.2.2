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
# $Log: osf.depend.mk,v $
# Revision 1.1.6.2  1996/03/09  23:25:19  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:40:57  marty]
#
# Revision 1.1.6.1  1995/12/07  22:09:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:37  root]
# 
# Revision 1.1.4.5  1993/01/20  20:00:55  john
# 	Added a test to make sure I'm not running MD when I'm in a Makefile which
# 	has SUBDIR lines.
# 	[1993/01/20  20:00:31  john]
# 
# Revision 1.1.4.4  1992/12/31  16:47:14  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:51:11  zeliff]
# 
# Revision 1.1.4.3  1992/12/23  17:35:02  john
# 	Fixed the copyright marker to be OSF_FREE_COPYRIGHT.
# 	[1992/12/23  17:14:27  john]
# 
# Revision 1.1.4.2  1992/11/06  21:25:29  john
# 	Added restriction to run only on the THIRD and BASIC pass.
# 	[1992/10/20  01:26:51  john]
# 
# Revision 1.1.2.3  1992/05/11  21:05:50  mhickey
# 	Replaced -f with _RMFLAGS_ in common makefiles.
# 	defect 2142.
# 	[1992/05/11  21:04:45  mhickey]
# 
# Revision 1.1.2.2  1992/04/16  14:32:15  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:29:34  marty]
# 
# Revision 1.1  1992/01/19  15:31:09  devrcs
# 	Initial revision
# 
# $EndLog$
#
#

.if !defined(_OSF_DEPEND_MK_)
_OSF_DEPEND_MK_=

MFILETEST = ${MAKEFILE_PASS:D${MAKEFILE_PASS}:UNONE}
.if ${MFILETEST} == "THIRD" || ${MFILETEST} == "BASIC"

nodep_all:
	${RM} ${_RMFLAGS_} depend.mk .depend

.if !empty(.TARGETS:Mnodep_*.o)
${TARGETS:Mnodep_*.o}:
	${RM} ${_RMFLAGS_} ${.TARGET:S/^nodep_//} ${.TARGET:S/^nodep_//:.o=.d}
	echo "${.TARGET:S/^nodep_//}: ${.TARGET:S/^nodep_//}" \
		> ${.TARGET:S/^nodep_//:.o=.d}
.endif

.if !defined(SUBDIRS)
.EXIT:
	${MD} ${_MDFLAGS_} .
.endif

.endif
.endif
