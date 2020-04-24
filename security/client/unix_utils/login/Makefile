# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:27:27  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:56  marty]
#
# Revision 1.1.4.1  1995/12/13  16:24:14  root
# 	Submit
# 	[1995/12/11  15:14:03  root]
# 
# Revision 1.1.2.2  1992/12/29  13:05:22  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:13:52  zeliff]
# 
# Revision 1.1  1992/01/19  14:47:42  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=1 08/30/91 //littl/prgy/src/client/unix_tools/login
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# Copyright (c) 1988 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation, advertising
# materials, and other materials related to such redistribution and
# use acknowledge that the software was developed by the University
# of California, Berkeley.  The name of the University may not be
# used to endorse or promote products derived from this software
# without specific prior written permission.  THIS SOFTWARE IS PROVIDED
# ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND
# FITNESS FOR A PARTICULAR PURPOSE.
#
# @(#)Makefile	5.3 (Berkeley) 5/9/89
#

PROGRAMS= login

#EXTRA_INCLUDES=-I../../bky_src/../include -I../../bky_src/../sys
INCFLAGS= -I. -I.. -I/ncs/idl2 -I../bsd4.4_include
CFLAGS= -W0,-info,3 -DOSF_DCE_SECURITY

OBJS=   lib_login.o dummy_netlogin.o rtmp.o dummy.o 
OFILES= login.o ${OBJS}
#LIBS= -lsecutil -lkrb -ldes
LIBS=

ILIST= login
IOWNER= ${ROOT_OWNER}
IMODE= ${SETUID_MODE}
IDIR= /usr/bin/

.include <${RULES_MK}>
