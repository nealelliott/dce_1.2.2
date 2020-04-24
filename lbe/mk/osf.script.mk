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
# $Log: osf.script.mk,v $
# Revision 1.1.6.2  1996/03/09  23:25:28  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:05  marty]
#
# Revision 1.1.6.1  1995/12/07  22:10:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:58  root]
# 
# Revision 1.1.4.3  1992/12/31  16:48:05  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:12  zeliff]
# 
# Revision 1.1.4.2  1992/09/16  19:41:15  marty
# 	CR# 4443, Fix rules so that they don't remove "#" when expanding the
# 	source warning.
# 	[1992/09/16  19:33:33  marty]
# 
# Revision 1.1.2.2  1992/04/16  14:33:12  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:30:39  marty]
# 
# Revision 1.1  1992/01/19  15:31:12  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
#

.if !defined(_OSF_SCRIPT_MK_)
_OSF_SCRIPT_MK_=

#
#  Definitions for rules using sed
#
_N_A_S_F_=THIS IS NOT A SOURCE FILE - DO NOT EDIT

#
#  Default single suffix compilation rules
#
.csh:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^\(#.*\)\@SOURCEWARNING\@;\1${_N_A_S_F_};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	 ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}

.sh:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^\(#.*\)\@SOURCEWARNING\@;\1${_N_A_S_F_};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	 ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}

.endif
