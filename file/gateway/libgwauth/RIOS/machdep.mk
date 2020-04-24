#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.2.2  1996/03/09  23:22:21  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:38:21  marty]
#
# Revision 1.1.2.1  1995/12/08  21:39:33  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/DCE1_1_WP/jrr_1.2_mothra/1  1995/11/13  14:40 UTC  dat
# 	Change VPATH from ${TARGET_OS} to ${TARGET_MACHINE} to correspond with directory name RIOS
# 
# 	HP revision /main/DCE1_1_WP/1  1995/06/26  20:25 UTC  maunsell_c
# 	osf dce1.2 gway
# 
# 	HP revision /main/HPDCE02/1  1995/06/22  20:14 UTC  maunsell_c
# 	mothra C&S fixes
# 
# 	HP revision /main/brezak_transarc/1  1995/06/01  22:03 UTC  brezak
# 	machdep makefile for RIOS
# 
# 	HP revision /main/brezak_dfsgw/1  1995/05/18  16:22 UTC  brezak
# 	new branch
# 	[1995/12/08  18:11:27  root]
# 
# $EndLog$

VPATH			= ${TARGET_MACHINE}

libgwauth.a_OFILES	+= auth_at_ki.o
