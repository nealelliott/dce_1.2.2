#
# ID: $Id: 
#
# ORIGINS: Transarc Corp.
#
# (C) COPYRIGHT Transarc Corp. 1992
# All Rights Reserved
# Licensed Materials - Property of Transarc
#
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
#

#
# @OSF_COPYRIGHT
#
# HISTORY
# $Log$
# $EndLog$
#

DATAFILES = access.scr closefile.scr common1.env common2.env dirops.scr \
            linkcall.scr opendir.scr openfile.scr openvdir.scr \
            read.char.file.scr read.binary.file.scr rename.scr \
            symlink.scr runscripts README

ILIST	= ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
