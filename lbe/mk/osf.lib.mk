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
# $Log: osf.lib.mk,v $
# Revision 1.2.18.1  1996/10/03  14:36:45  arvind
# 	Fix clearcase churn due to bogus _RMFLAGS_ setting.
# 	[1996/09/19  20:22 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
#
# 	put ranlib back in for hp-ux; delete archive before rebuilding for
# 	clearcase.
# 	[1996/09/16  19:55 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.2.15.2  1996/03/09  23:25:21  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:40:59  marty]
# 
# Revision 1.2.15.1  1995/12/07  22:10:09  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:42  root]
# 
# Revision 1.2.12.1  1994/05/20  22:03:28  sommerfeld
# 	For ClearCase, don't use archive(member) syntax..
# 	[1994/05/17  19:02:59  sommerfeld]
# 
# Revision 1.2.10.2  1993/06/10  19:18:43  sommerfeld
# 	Make call to RANLIB conditional on the TARGET_OS type.
# 	This way, on HPUX machines, we will stop getting the
# 	annoying 'ranlib: ar already did it for you" warning
# 	messages.
# 	[1993/04/22  17:39:43  truitt]
# 
# Revision 1.2.6.2  1992/12/31  16:47:28  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:51:27  zeliff]
# 
# Revision 1.2.2.3  1992/05/11  21:05:53  mhickey
# 	Replaced -f with _RMFLAGS_ in common makefiles.
# 	defect 2142.
# 	[1992/05/11  21:04:51  mhickey]
# 
# Revision 1.2.2.2  1992/04/16  14:32:29  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:29:48  marty]
# 
# Revision 1.2  1992/01/19  22:13:21  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#
#  OSF DCE Version 1.0 

.if !defined(_OSF_LIB_MK_)
_OSF_LIB_MK_=

#
#  Build rules
#
.if defined(LIBRARIES)

.if defined(ORDER_LIBRARIES)
COFF_LORDER=`lorder *.o | tsort`
AIXCOFF_LORDER=`lorder *.o | tsort`
A_OUT_LORDER=${_OFILES_}
MACHO_LORDER=${_OFILES_}
_RMFLAGS_=-rf
.endif

#_LIBRARY_OFILES_=${_OFILES_:@.OF.@${.TARGET}(${.OF.})@}
#${LIBRARIES}: $${_LIBRARY_OFILES_}

.if defined(ODE_UNDER_CLEARCASE)
${LIBRARIES}: $${_OFILES_}
	${RM} ${_RMFLAGS_} ${.TARGET}
	${_AR_} ${DEF_ARFLAGS} ${.TARGET} ${_OFILES_}
.else
${LIBRARIES}: $${.TARGET}($${_OFILES_})
	${_AR_} ${DEF_ARFLAGS} ${.TARGET} ${.OODATE}
.endif
.if defined(ORDER_LIBRARIES)
	${RM} ${_RMFLAGS_} tmp
	mkdir tmp
	cd tmp && { \
	    ${_AR_} x ../${.TARGET}; \
	    ${RM} ${_RMFLAGS_} __.SYMDEF __________ELELX; \
	    ${_AR_} cr ${.TARGET} ${${OBJECT_FORMAT}_LORDER}; \
	}
	${MV} -f tmp/${.TARGET} .
	${RM} ${_RMFLAGS_} tmp
.endif
	${_RANLIB_} ${.TARGET}
#	${RM} ${_RMFLAGS_} ${.OODATE}

.endif

.if defined(SHARED_LIBRARIES)

${SHARED_LIBRARIES}: $${_OFILES_}
	${_LD_} ${_SHLDFLAGS_} -o ${.TARGET}.X ${_OFILES_} ${_LIBS_}
	${MV} ${.TARGET}.X ${.TARGET}
.endif

.endif
