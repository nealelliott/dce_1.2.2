# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.14.2  1996/03/11  01:43:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:47  marty]
#
# Revision 1.1.14.1  1995/12/08  22:08:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:43:20  root]
# 
# Revision 1.1.12.2  1994/06/10  20:18:48  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:58:09  devsrc]
# 
# Revision 1.1.12.1  1993/12/29  19:22:38  rsalz
# 	Move dce_cf_XXX from config to dce/utils/dcecf (OT CR 9663).
# 	[1993/12/29  18:37:28  rsalz]
# 
# $EndLog$
#
# Makefile for the dce_cf_XXX tests.
#

VPATH		= ../../config
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
IDIR		= /usr/include/dce/
EXPDIR		= /usr/include/dce/

PROGRAMS		= dce_cf_from_host dce_cf_test
dce_cf_from_host	= dce_cf_from_host.o
getcell			= dce_cf_test.o

LIBS		= -ldce

ILIST		= ${INCLUDES} 

.include <${RULES_MK}>

