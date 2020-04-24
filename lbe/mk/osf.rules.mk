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
# $Log: osf.rules.mk,v $
# Revision 1.1.9.2  1996/03/09  23:25:27  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:04  marty]
#
# Revision 1.1.9.1  1995/12/07  22:10:48  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:55  root]
# 
# Revision 1.1.6.4  1992/12/31  16:48:02  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:08  zeliff]
# 
# Revision 1.1.6.3  1992/12/14  21:44:32  john
# 	Fixed the error caused by using TOSTAGE to check if in the install pass.
# 	[1992/12/14  21:43:19  john]
# 
# Revision 1.1.6.2  1992/12/10  17:21:39  marty
# 	No longer tries to build objects during the install pass.
# 	[1992/11/11  23:02:15  marty]
# 
# Revision 1.1.2.2  1992/04/16  14:33:07  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:30:32  marty]
# 
# Revision 1.1  1992/01/19  15:31:08  devrcs
# 	Initial revision
# 
# $EndLog$
#
#

.if !defined(_OSF_RULES_MK_)
_OSF_RULES_MK_=

.include <osf.std.mk>

#.if !defined(TOSTAGE)
.if empty(.TARGETS:Minstall_*)
.if defined(PROGRAMS)
.include <osf.prog.mk>
.endif
.if defined(LIBRARIES) || defined(SHARED_LIBRARIES)
.include <osf.lib.mk>
.endif
.if defined(BINARIES)
.include <osf.obj.mk>
.endif
.if defined(SCRIPTS)
.include <osf.script.mk>
.endif
.if defined(MANPAGES)
.include <osf.man.mk>
.endif
.if defined(DOCUMENTS)
.include <osf.doc.mk>
.endif
.if defined(DEPENDENCIES)
.include <osf.depend.mk>
.endif
.endif

.endif
