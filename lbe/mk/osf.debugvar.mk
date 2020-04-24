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
# $Log: osf.debugvar.mk,v $
# Revision 1.1.6.2  1996/03/09  23:25:18  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:40:56  marty]
#
# Revision 1.1.6.1  1995/12/07  22:09:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:33  root]
# 
# Revision 1.1.4.2  1992/12/31  16:47:12  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:51:06  zeliff]
# 
# Revision 1.1.2.2  1992/04/16  14:32:11  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:29:29  marty]
# 
# Revision 1.1  1992/01/19  22:13:21  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
# 
# 
#  OSF DCE Version 1.0 
#

${.TARGETS:Mpr_*}:
	@echo "${.TARGET:S/^pr_//} = ${${.TARGET:S/^pr_//}}"
