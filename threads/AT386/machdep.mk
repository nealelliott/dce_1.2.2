#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.10.2  1996/03/09  23:30:28  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:45  marty]
#
# Revision 1.1.10.1  1995/12/07  21:43:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:58:17  root]
# 
# Revision 1.1.8.2  1994/06/09  13:33:54  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:25:07  devsrc]
# 
# Revision 1.1.8.1  1994/04/01  20:13:33  jd
# 	First drop for code cleanup
# 	[1994/03/28  19:30:12  jd]
# 
# Revision 1.1.6.2  1993/05/24  20:49:32  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:16:49  cjd]
# 
# Revision 1.1.4.2  1993/05/12  13:57:59  jd
# 	Initial 486 port.
# 	[1993/05/12  13:57:02  jd]
# 
# $EndLog$
#

cma_assem.o :	cma_assem.s
	${CP} ${${.TARGET:.o=.s}:P} ${.TARGET:.o=.S};
	${_CC_} -E ${_CCFLAGS_} -D${TARGET_MACHINE} -D__LANGUAGE_ASSEMBLY__ -traditional ${.TARGET:.o=.S}> ${.TARGET:.o=.pp};
	sed '/^\#/d' ${.TARGET:.o=.pp} > ${.TARGET:.o=.s};
#	${RM} ${_RMFLAGS_} ${.TARGET:.o=.S};
#	${RM} ${_RMFLAGS_} ${.TARGET:.o=.pp};
	${_CC_} -c ${.TARGET:.o=.s};
#	${RM} ${_RMFLAGS_} ${.TARGET:.o=.s} 

