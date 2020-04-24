# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.8.2  1996/03/09  23:31:27  marty
# 	Update copyright years
# 	[1996/03/09  23:00:45  marty]
#
# Revision 1.1.8.1  1995/12/08  18:11:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:24:52  root]
# 
# Revision 1.1.6.2  1993/05/24  20:51:37  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:20:21  cjd]
# 
# Revision 1.1.4.2  1993/05/12  17:43:22  jd
# 	Initial 486 port
# 	[1993/05/12  17:42:06  jd]
# 
# $EndLog$
#
AT386_libutc_OFILES= utc_adjtime.o utc_gettime.o

${AT386_libutc_OFILES:.o=.S}: $${@:.S=.s}
	${RM} ${_RMFLAGS_} $@
	${CP} ${${@:.S=.s}:P} $@

${AT386_libutc_OFILES}: $${@:.o=.S}
	${_CC_} -traditional ${_CCFLAGS_} -D${TARGET_MACHINE} -D__ASSEMBLER__ -c ${@:.o=.S}

