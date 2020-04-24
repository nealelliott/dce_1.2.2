#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.426.1  1996/10/17  18:28:48  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:54:29  damon]
#
# Revision 1.1.421.3  1994/02/07  16:28:21  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:34  mckeen]
# 
# Revision 1.1.421.2  1994/02/04  20:46:32  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:25:04  devsrc]
# 
# Revision 1.1.418.1  1993/12/07  17:45:36  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:33:03  jaffe]
# 
# Revision 1.1.2.3  1993/01/11  18:32:22  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:13:19  htf]
# 
# Revision 1.1.2.2  1992/10/28  17:59:25  jaffe
# 	Transarc delta: fred-itl-move-hashing-to-utils 1.1
# 	  Selected comments:
# 	    Moved the name hashing stuff from the itl/file sub-dir to itl/utils
# 	    sub-dir so it is accessible to anyone writing ITL C code.
# 	    Added itl_utilsNameHash.o and itl_utilsNameHash.h.
# 	Transarc delta: jdp-add-itl-stuff 1.8
# 	  Selected comments:
# 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
# 	    libitlFset.a
# 	    Build libitlUtils.a
# 	    Update error message in itlu_CheckReturnCode() so that the offending command
# 	    is printed.
# 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
# 	    Changed interface and implementation of result checking routines.
# 	    Provide routine for converting a value back to a name, assuming that
# 	    the value--name mapping was constructed using itlu_ProcessValueMap().
# 	    Checkpointing in order to import another delta.
# 	    Fix misc. compilation problems and close inorder to open another delta.
# 	    Make this stuff compile on the pmax.
# 	[1992/10/28  12:54:44  jaffe]
# 
# $EndLog$

LIBRARIES		= libitlUtils.a
INCLUDES		= itl_utils.h itl_utilsNameHash.h

EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

libitlUtils.a_OFILES	= itl_utils.o itl_utilsNameHash.o
libitlUtils.a_EXPDIR	= /usr/lib/

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

