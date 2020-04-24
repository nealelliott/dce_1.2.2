# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.2  1996/03/09  23:14:53  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:11  marty]
#
# Revision 1.1.10.1  1995/12/08  16:17:30  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:21:56  root]
# 
# Revision 1.1.8.1  1994/02/22  19:22:10  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  11:56:01  marrek]
# 
# Revision 1.1.6.2  1993/08/11  12:49:00  marrek
# 	July 1993 code drop.
# 	[1993/08/04  09:15:53  marrek]
# 
# Revision 1.1.4.2  1992/12/31  22:06:27  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:49:34  bbelch]
# 
# Revision 1.1.2.2  1992/06/01  23:12:49  melman
# 	New GDS merged drop
# 	[1992/05/29  12:36:51  melman]
# 
# Revision 1.1  1992/01/19  15:14:21  devrcs
# 	Initial revision
# 
# $EndLog$
# 
# $RCSfile: Makefile,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:14:53 $
# 
#  This builds a dummy libxti.a.  If you are building on a machine which
#  has a resident libxti.a (with real XTI functionality that is to be used)
#  comment out the next line.
LIBRARIES	= libxti.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I../h

libxti.a_OFILES	= libxti.o

.include <${RULES_MK}>
