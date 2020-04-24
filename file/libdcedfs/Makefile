#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.64.1  1996/10/02  17:54:06  damon
# 	New DFS from Transarc
# 	[1996/10/01  18:58:39  damon]
#
# Revision 1.1.59.2  1994/06/09  14:12:58  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:26:16  annie]
# 
# Revision 1.1.59.1  1994/02/04  20:22:29  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:49  devsrc]
# 
# Revision 1.1.57.1  1993/12/07  17:27:54  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:47:36  jaffe]
# 
# Revision 1.1.4.3  1993/01/19  16:07:30  cjd
# 	embedded copyright notice
# 	[1993/01/19  15:10:57  cjd]
# 
# Revision 1.1.4.2  1992/08/31  20:06:43  jaffe
# 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
# 	  Selected comments:
# 	    The use of com_err in dfs is inconsistent and in some cases
# 	    incorrect.
# 	    1) there are many sources which include <com_err.h> but don't
# 	    use any definitions from it.
# 	    2) there are sources which use com_err, but don't include the
# 	    <com_err.h>
# 	    3) there are subdirectories which do not need libcom_err.a for
# 	    any reason that link with it.
# 	    This delta is created to clean up the usage of com_err in dfs.
# 	    Remove -lcom_err in the var LIBS
# 	[1992/08/30  02:57:48  jaffe]
# 
# Revision 1.1.2.3  1992/07/03  01:53:59  mason
# 	Transarc delta: jaffe-ot2785-dont-use-link-in-VPATH 1.1
# 	  Selected comments:
# 	    remove reference through link in the VPATH line to allow for multiple
# 	    backing trees.
# 	[1992/07/02  19:04:03  mason]
# 
# Revision 1.1.2.2  1992/07/02  21:52:29  mason
# 	Transarc delta: jaffe-ot2785-dont-use-link-in-VPATH 1.1
# 	  Selected comments:
# 	    remove reference through link in the VPATH line to allow for multiple
# 	    backing trees.
# 	[1992/07/02  19:04:03  mason]
# 
# Revision 1.1  1992/01/19  02:59:14  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# DCE 
# libdcedfs Makefile
#

VPATH	= ${TARGET_MACHINE}:../../../export/${target_machine}/usr/lib

# Files to export
EXPLIB_TARGETS  = export_libdcedfs.a


LIBS	= ${libvolc.a:P} ${libfldb.a:P} ${libftserver.a:P} ${libbos.a:P} \
	  ${libafs4int.a:P} ${libncompat.a:P} \
	  ${libnubik.a:P} ${libdauth.a:P} ${libdacl.a:P} ${libdacllfs.a:P} \
	  ${librep.a:P} ${libcmd.a:P} ${libafsutil.a:P} ${libtpq.a:P}\
	  ${libafssys.a:P} ${libosi.a:P}

	
ILIST	=	libdcedfs.a
IDIR	=	/usr/lib/

# Directory in which to place exported files
EXPDIR = /usr/lib/

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${TARGET_OS}/machdep.mk)
.include "${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
