# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:11:11  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:37  marty]
#
# Revision 1.1.4.1  1995/12/08  21:37:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:08:52  root]
# 
# Revision 1.1.2.2  1994/08/10  19:21:26  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:15:44  annie]
# 
# 	expand copyright with OSF copyright text
# 
# 	expand copyright with OSF copyright text
# 
# Revision 1.1.2.1  1994/06/30  19:37:03  ohara
# 	create file, need RPC_UUID_KERNEL defined on SVR4
# 	[1994/06/27  19:49:03  ohara]
# 
# $EndLog$
.if defined(BLD_USR_OR_KERNEL) && (${BLD_USR_OR_KERNEL}=="kernel")
SVR4_CFLAGS     += -DRPC_UUID_KERNEL
.endif

