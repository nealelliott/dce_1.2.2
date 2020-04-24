#
# ID: $Id: 
#
# ORIGINS: Transarc Corp.
#
# (C) COPYRIGHT Transarc Corp. 1993
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

DATAFILES = README README.moving README.open2.itl                          \
	     close.dist.scr common1.env common2.env common3.env             \
	     config connection.dist.scr create.dist.scr                     \
	     create.stat.lookup.scr create.unlink.scr daemon.dist.scr       \
	     getwd.dist.scr hardlink.dist.scr merge.status.dist.scr         \
	     misc1.dist.scr misc2.dist.scr mkrm.dir.dist.scr                \
	     name.hash.dist.scr name.resolution.basic.scr                   \
	     name.resolution.scr read.write.dist.scr readlink.dist.scr      \
	     rename.dist.scr runall stat.cache.data.scr symlink.dist.scr    \
	     sys5lock.asyn.scr sys5lock.scr sys5lock.multi-contender.scr    \
	     truncate.dist.scr                                              \
             close.dist.scr.setup connection.dist.scr.setup                 \
             create.dist.scr.setup create.stat.lookup.scr.setup             \
             create.unlink.scr.setup daemon.dist.scr.setup                  \
             getwd.dist.scr.setup hardlink.dist.scr.setup                   \
             merge.status.dist.scr.setup misc1.dist.scr.setup               \
             misc2.dist.scr.setup mkrm.dir.dist.scr.setup                   \
             name.hash.dist.scr.setup name.resolution.basic.scr.setup       \
             name.resolution.scr.setup read.write.dist.scr.setup            \
             readlink.dist.scr.setup rename.dist.scr.setup                  \
             stat.cache.data.scr.setup symlink.dist.scr.setup               \
             sys5lock.asyn.scr.setup sys5lock.scr.setup                     \
             sys5lock.multi-contender.scr.setup truncate.dist.scr.setup     \
	     common_dist.config.itl movingIO.config.itl movingIO.itl        \
	     movingIO.setup.itl movingLocks.config.itl movingLocks.itl      \
	     movingLocks.setup.itl moving_common.config.itl open1.itl       \
	     open1.setup.itl open2.config.itl open2.itl open2.setup.itl

ILIST	= ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
