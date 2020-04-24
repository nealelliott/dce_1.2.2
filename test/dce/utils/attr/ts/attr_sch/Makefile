# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  01:43:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:45  marty]
#
# Revision 1.1.4.1  1995/12/08  22:08:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:43:16  root]
# 
# Revision 1.1.2.1  1994/08/11  18:35:30  pwang
# 	Initial Implementation
# 	[1994/08/11  18:27:36  pwang]
# 
# $EndLog$


PROGRAMS        = dce_attr_sch_fvt 

OFILES		= dce_attr_sch_fvt.o
HFILES		= dce_attr_sch_fvt.h

IDIR		= /test/tet/functional/dce/utils/attr/ts/attr_sch/
ILIST		= ${PROGRAMS}
IMODE		= 755

LIBS		= -ltettcm -ltetapi -ldce
CFLAGS		+= ${STRICT_COMPILER_WARNINGS}

.include <${RULES_MK}>

