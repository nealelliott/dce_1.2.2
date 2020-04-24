#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.6.2  1996/03/09  23:26:14  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:52  marty]
#
# Revision 1.1.6.1  1995/12/08  00:14:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:10  root]
# 
# Revision 1.1.4.4  1993/01/03  22:35:58  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:51:57  bbelch]
# 
# Revision 1.1.4.3  1992/12/23  19:38:25  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:52:40  zeliff]
# 
# Revision 1.1.4.2  1992/12/03  22:50:02  delgado
# 	Changes for new krpc helper interface
# 	[1992/12/03  22:49:08  delgado]
# 
# Revision 1.1.2.2  1992/06/26  06:09:24  jim
# 	Changes for AIX 3.2
# 	[1992/06/02  16:00:35  jim]
# 
# Revision 1.1  1992/01/19  03:15:31  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# rpc/kruntime/RIOS/machdep.mk for RIOS-specific RPC kernel rules
#

RIOS_OBJS_SYS=kproc_map.o socket.o krpc_helper_mach.o
sec_authn_krpc.o_CCTYPE         =traditional
krpc_helper.o_CCTYPE            =traditional
comsoc_sys.o_CCTYPE             =traditional
pthread_sys.o_CCTYPE            =traditional
uuidsys.o_CCTYPE                =traditional
kproc_map.o_CCTYPE              =traditional
socket.o_CCTYPE                 =traditional
krpc_helper_mach.o_CCTYPE       =traditional

