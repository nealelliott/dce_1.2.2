#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/11  02:18:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:55:28  marty]
#
# Revision 1.1.12.1  1995/12/11  19:29:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:15:44  root]
# 
# Revision 1.1.10.1  1993/12/28  22:11:13  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:54:34  pellis]
# 
# Revision 1.1.8.2  1993/06/24  20:42:29  hinman
# 	[hinman] - MAke XLC bug fix visible only to RIOS
# 	[1993/06/22  11:54:08  hinman]
# 
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:41:16  hinman]
# 
# Revision 1.1.4.2  1993/01/07  23:36:06  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  13:43:22  bbelch]
# 
# Revision 1.1  1992/01/19  03:29:23  devrcs
# 	Initial revision
# 
# $EndLog$
#

.if ${TARGET_MACHINE}=="RIOS"
IDL_CC=		# Because of XLC bug
.endif

IDLBASE = sparse
OTHER_CLIENT_FILES = client_aux.o util_xmit.o
OTHER_SERVER_FILES = util_xmit.o

.include "../maketest.mk"
