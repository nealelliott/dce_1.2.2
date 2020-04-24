#
# @OSF_COPYRIGHT@
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/10/17  18:19:00  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:22  damon]
#
# $EndLog$
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved
#

SUBDIRS 	= fileset

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.include <${RULES_MK}>
