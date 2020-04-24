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
# Revision 1.1.9.2  1996/03/09  23:30:39  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:00  marty]
#
# Revision 1.1.9.1  1995/12/07  21:50:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:00:37  root]
# 
# Revision 1.1.7.1  1994/06/09  13:35:29  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:26:04  devsrc]
# 
# Revision 1.1.5.2  1992/12/15  22:23:18  alan
# 	Insert copyright notices
# 	[1992/12/07  16:02:50  alan]
# 
# Revision 1.1.3.3  1992/04/07  19:20:39  mckeen
# 	Merged upto mainline
# 	[1992/03/31  16:38:44  mckeen]
# 
# 	Fixed Runes to use .TARGET instead of .PREFIX
# 	[1992/03/13  15:48:10  mckeen]
# 
# Revision 1.1.3.2  1992/01/30  18:55:54  mhickey
# 	Made the following changes:
# 
# 	    - changed incorrect use of .PREFIX to .TARGET:.o=<appropriate_suffix>
# 	      when referring to temporary files - Fix for defect 2033.
# 
# 	    - made all the commands execute out of a single shell for
# 	      efficiency.
# 	[1992/01/30  18:55:26  mhickey]
# 
# Revision 1.1  1992/01/19  14:41:32  devrcs
# 	Initial revision
# 
# Revision 1.1.19.2  91/12/11  10:57:43  melman
# 	Merge to dce.72
# 	[91/12/11  09:58:30  melman]
# 
# Revision 1.1.3.2  1992/01/30  18:55:54  mhickey
# 	Made the following changes:
# 	 - changed incorrect use of .PREFIX to .TARGET:.o=<appropriate_suffix>
# 	   when referring to temporary files - Fix for defect 2033.
# 	 - made all the commands execute out of a single shell for
# 	   efficiency.
# 	[1992/01/30  18:55:26  mhickey]
# 
# Revision 1.1  1992/01/19  14:41:32  devrcs
# 	Initial revision
# 
# $EndLog$
#

CMASOBJ         = cma_assem.o

PMAX_INCLUDES	= cma_host.h

${CMASOBJ}: $${.TARGET:.o=.s}
	${CP} ${${.TARGET:.o=.s}:P} ${.TARGET:.o=.S}; \
	${_CC_} -E ${_CCFLAGS_} -D${TARGET_MACHINE} -DASSEMBLER -traditional ${.TARGET:.o=.S}> ${.TARGET:.o=.pp}; \
	sed '/^\#/d' ${.TARGET:.o=.pp} > ${.TARGET:.o=.s}; \
	${RM} ${_RMFLAGS_} ${.TARGET:.o=.S}; \
	${RM} ${_RMFLAGS_} ${.TARGET:.o=.pp}; \
	${_CC_} -c ${.TARGET:.o=.s}; \
	${RM} ${_RMFLAGS_} ${.TARGET:.o=.s} 
