#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.411.1  1996/10/17  18:24:18  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:20  damon]
#
# Revision 1.1.406.3  1994/02/07  16:28:14  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:27  mckeen]
# 
# Revision 1.1.406.2  1994/02/04  20:42:58  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:28  devsrc]
# 
# Revision 1.1.404.1  1993/12/07  17:43:12  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:06:39  jaffe]
# 
# Revision 1.1.2.3  1993/01/11  18:29:28  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:06:58  htf]
# 
# Revision 1.1.2.2  1992/10/28  17:38:47  jaffe
# 	Transarc delta: jdp-add-itl-stuff 1.8
# 	  Selected comments:
# 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
# 	    libitlFset.a
# 	    Build libitlAggr.a
# 	    Update error message in itlu_CheckReturnCode() so that the offending command
# 	    is printed.
# 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
# 	    Changed interface and implementation of result checking routines.
# 	    Provide routine for converting a value back to a name, assuming that
# 	    the value--name mapping was constructed using itlu_ProcessValueMap().
# 	    Checkpointing in order to import another delta.
# 	    Fix misc. compilation problems and close inorder to open another delta.
# 	    Make this stuff compile on the pmax.
# 	[1992/10/27  19:56:32  jaffe]
# 
# $EndLog$

LIBRARIES		= libitlAggr.a
INCLUDES		= itl_aggr.h

EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

libitlAggr.a_OFILES	= itl_aggr.o
libitlAggr.a_EXPDIR	= /usr/lib/

CFLAGS			= ${USERCFLAGS} -DITL_FS_TEST

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

