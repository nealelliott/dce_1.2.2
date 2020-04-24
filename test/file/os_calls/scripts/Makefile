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
SUBDIRS= interface stress

DATA_SUBDIRS= interface stress

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

