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
# Revision 1.1.6.2  1996/03/11  02:28:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:07  marty]
#
# Revision 1.1.6.1  1995/12/11  20:13:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:38  root]
# 
# Revision 1.1.4.2  1993/01/13  19:51:46  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:19:15  bbelch]
# 
# Revision 1.1.2.2  1992/05/01  13:48:00  rsalz
# 	Added as part of rpc6 drop.
# 	[1992/04/30  23:21:58  rsalz]
# 
# $EndLog$
#
#
# 
# 

HP_PA_PROGRAMS		= dev_helper
dev_helper_OFILES	= dev_helper.o
dev_helper_LIBS		= -lauthelper ${LIBS_DCE} ${${TARGET_MACHINE}_LIBS}

