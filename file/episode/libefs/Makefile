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
# Revision 1.1.769.1  1996/10/02  17:24:16  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:31:55  damon]
#
# Revision 1.1.764.3  1994/06/09  14:00:28  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:43  annie]
# 
# Revision 1.1.764.2  1994/02/04  20:13:17  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:21  devsrc]
# 
# Revision 1.1.76.1  1993/12/07  17:18:56  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:58:02  jaffe]
# 
# Revision 1.1.4.3  1993/01/19  15:26:37  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:46:46  cjd]
# 
# Revision 1.1.4.2  1992/10/27  20:39:27  jaffe
# 	Transarc delta: comer-ot3013-dfs-zlc-file-handling-stubs 1.3
# 	  Selected comments:
# 	    Created a new package (library) to handle zero-linkcount files, and added
# 	    it to the dfs core extension under AIX.  There are calls to this package
# 	    from the xvolume package so these calls must be stubbed out for the
# 	    stand-alone episode extension.  The AIX libefs Makefile was, therefore,
# 	    changed to build and include ../../zlc/zlc_sepistubs.c in the stand-alone
# 	    extension.
# 	    For the PMAX, the "stand-alone" episode vmunix contains the dfs core and the
# 	    episode libraries.  For the PMAX, therefore, the normal zlc library is
# 	    included.
# 	    Fix compilation (link) problems with 1.52, specifically handle kzlc issues.
# 	    (oops, wrong Makefile)
# 	    Due to merge problems, repeat last change.
# 	    (Probably should have opened a merge-* delta instead of reopening this one.
# 	    Sorry.)
# 	[1992/10/26  22:41:53  jaffe]
# 
# Revision 1.1.2.4  1992/05/05  19:06:59  jdp
# 	delta jdp-epimerge-add-dynamic-configuration 1.1
# 	delta jdp-epimerge-add-libefs 1.2
# 	[1992/05/04  21:39:23  jdp]
# 
# Revision 1.1.2.3  1992/04/07  20:27:06  rec
# 	Change for test on machine and os subdirs since there
# 	are OSF1 and RIOS subdirs that must be built to built
# 	libefs for the respective platforms.  This is a hack
# 	but the correct answer is to move to the machine_os
# 	format throught the DCE source tree.
# 	[1992/04/07  19:58:11  rec]
# 
# Revision 1.1  1992/01/19  02:39:30  devrcs
# 	Initial revision
# 
# $EndLog$
#

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>

