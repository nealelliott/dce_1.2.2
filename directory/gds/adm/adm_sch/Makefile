#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/09  23:12:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:50  marty]
#
# Revision 1.1.11.1  1995/12/08  15:12:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:50:16  root]
# 
# Revision 1.1.9.1  1994/02/22  15:56:45  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:16:58  marrek]
# 
# Revision 1.1.7.2  1993/08/10  08:56:38  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:12:35  marrek]
# 
# Revision 1.1.5.3  1992/12/31  17:46:29  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:59:50  bbelch]
# 
# Revision 1.1.5.2  1992/11/27  13:49:52  marrek
# 	November 1992 code drop
# 	[1992/11/16  14:58:21  marrek]
# 
# Revision 1.1.2.2  1992/06/01  21:37:46  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  19:09:20  zeliff]
# 
# Revision 1.1  1992/01/19  15:00:00  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:12:40 $
#

LIBRARIES	= libadmsch.a
EXPDIR	= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I../../h
libadmsch.a_OFILES = \
            d20read.o d20transf.o d20dpsrt.o d20dpoct.o d20dpat.o 	\
	    d20sadmsch.o d20ldsch.o d20update.o 	\
	    d20updsch.o \
	    d20admsch.o d20gattr.o d20gobj.o d20gsrule.o \
	    d20ldsh.o \
	    d20disp.o  d20hdlsrt.o d20hdloct.o d20hdlat.o d20store.o

.include <${RULES_MK}>

