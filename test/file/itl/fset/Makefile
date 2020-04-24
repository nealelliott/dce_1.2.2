#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.410.1  1996/10/17  18:26:04  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:53:07  damon]
#
# Revision 1.1.405.3  1994/02/07  16:28:18  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:31  mckeen]
# 
# Revision 1.1.405.2  1994/02/04  20:44:02  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:54  devsrc]
# 
# Revision 1.1.403.1  1993/12/07  17:43:50  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:13:35  jaffe]
# 
# Revision 1.1.2.3  1993/01/11  18:30:14  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:08:39  htf]
# 
# Revision 1.1.2.2  1992/10/28  17:48:44  jaffe
# 	Transarc delta: jdp-add-itl-stuff 1.8
# 	  Selected comments:
# 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
# 	    libitlFset.a
# 	    Build libitlFset.a
# 	    Update error message in itlu_CheckReturnCode() so that the offending command
# 	    is printed.
# 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
# 	    Changed interface and implementation of result checking routines.
# 	    Provide routine for converting a value back to a name, assuming that
# 	    the value--name mapping was constructed using itlu_ProcessValueMap().
# 	    Checkpointing in order to import another delta.
# 	    Fix misc. compilation problems and close inorder to open another delta.
# 	    Make this stuff compile on the pmax.
# 	[1992/10/27  22:29:30  jaffe]
# 
# $EndLog$

LIBRARIES		= libitlFset.a
INCLUDES		= itl_fset.h

EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

libitlFset.a_OFILES	= itl_fset.o
libitlFset.a_EXPDIR	= /usr/lib/

CFLAGS			= ${USERCFLAGS} -DITL_FS_TEST

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

