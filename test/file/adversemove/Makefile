#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# (C) Copyright 1993 Transarc Corporation.
# All Rights Reserved.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/10/17  18:11:47  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:47:32  damon]
#
# $EndLog$
#
#

DATAFILES =	README adversemove.tex \
		adversemove adverse_move.setup adverse_move.config \
		adverse_move1.itl adverse_move1a.itl adverse_move1b.itl \
		adverse_move2.itl adverse_move2a.itl \
		adverse_move3.itl adverse_move3a.itl adverse_move3b.itl \
		adverse_move4.itl adverse_move4b.itl 
		

ILIST   = ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
