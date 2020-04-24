# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/08/09  11:56:27  arvind
# 	Add IMODE.
# 	[1996/07/17  18:45 UTC  psn  /main/psn_bugfix/2]
#
# 	Fix OT 13212.
# 	[1996/07/17  17:57 UTC  psn  /main/psn_bugfix/1]
#
# Revision 1.1.4.2  1996/03/09  23:10:45  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:28  marty]
# 
# Revision 1.1.4.1  1995/12/08  21:35:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:07:38  root]
# 
# Revision 1.1.2.1  1994/07/22  21:39:39  pwang
# 	Initial Implementation
# 	[1994/07/22  20:32:47  pwang]
# 
# $EndLog$

# Exported files
INCLUDES		=  ${IDLFILES} ${IDLDATAFILES}

# Export dir
EXPDIR			= /usr/include/dce/

# idl files and idl flags
IDLFILES		= dce_attr_base.idl dce_attr_sch.idl
IDLDATAFILES		= ${IDLFILES:S/.idl$/.h/g}

IDLFLAGS                = -cepv -keep c_source
IDLINCFLAGS		= -I.

# compiler flags
#CFLAGS	 		= ${${TARGET_MACHINE}_CFLAGS}
CFLAGS	 		+= ${STRICT_COMPILER_WARNINGS}
INCFLAGS		= -I.

# Files to install and their locations
ILIST   = ${IDLFILES} ${IDLDATAFILES}
IDIR    = /share/include/dce/
IMODE   = 444

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
