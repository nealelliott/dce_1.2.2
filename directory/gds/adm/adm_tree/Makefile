#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/09  23:12:42  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:52  marty]
#
# Revision 1.1.11.1  1995/12/08  15:14:13  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:50:56  root]
# 
# Revision 1.1.9.1  1994/02/22  15:57:29  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:21:33  marrek]
# 
# Revision 1.1.7.2  1993/08/10  09:21:51  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:18:41  marrek]
# 
# Revision 1.1.5.3  1992/12/31  17:47:33  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:01:29  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  14:16:15  marrek
# 	November 1992 code drop
# 	[1992/11/16  15:30:47  marrek]
# 
# Revision 1.1.2.2  1992/06/01  21:43:44  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  19:11:31  zeliff]
# 
# Revision 1.1  1992/01/19  14:58:23  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:12:42 $
#

LIBRARIES	= libtree.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I../../h

libtree.a_OFILES = \
                    d20tree.o d20save.o d20append.o d20copy.o d20delete.o \
	d21addap.o   d21apsub.o   d21hdldn.o   d21modap.o   d21oldobj.o \
	d21restor.o  d21svsub.o   d21cpsub.o   d21delst.o   d21chmas.o \
	d21chnam.o   d21checkp.o  \
	d21confl.o   d20chngall.o d20chgmas.o d20chgnam.o d21chdnl.o


.include <${RULES_MK}>

