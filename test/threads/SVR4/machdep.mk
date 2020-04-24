# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
# Burlington, MA, USA
# All Rights Reserved
# (c) Copyright 1990, 1991, 1992
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.7.2  1996/03/11  02:50:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:53  marty]
#
# Revision 1.1.7.1  1995/12/11  22:57:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:34:37  root]
# 
# Revision 1.1.5.2  1993/06/24  20:43:49  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:42:18  hinman]
# 
# Revision 1.1.3.3  1992/12/30  16:27:24  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  16:03:54  zeliff]
# 
# Revision 1.1.3.2  1992/09/29  20:37:33  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/17  20:31:43  sekhar]
# 
# $EndLog$

SVR4_LIBS	=	-lcma 
cuvb_wrp_001.o_CFLAGS   = -Dunix 
cuvb_wrp_002.o_CFLAGS   = -Dunix 
cuvb_wrp_003.o_CFLAGS   = -Dunix 
CUVB		+=	cuvb_nbi_011 cuvb_nbi_012
CUPB		=
