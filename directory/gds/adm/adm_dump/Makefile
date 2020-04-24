#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.2  1996/03/09  23:12:38  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:48  marty]
#
# Revision 1.1.10.1  1995/12/08  15:10:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:49:26  root]
# 
# Revision 1.1.8.2  1994/06/21  14:43:09  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:22:07  marrek]
# 
# Revision 1.1.8.1  1994/02/22  15:50:47  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:11:30  marrek]
# 
# Revision 1.1.6.2  1993/08/10  08:05:46  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:02:12  marrek]
# 
# Revision 1.1.4.4  1992/12/31  17:43:18  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:57:27  bbelch]
# 
# Revision 1.1.4.3  1992/11/27  13:34:40  marrek
# 	November 1992 code drop
# 	[1992/11/16  14:26:41  marrek]
# 
# Revision 1.1.4.2  1992/09/23  09:36:09  marrek
# 	Deleted blank in CFLAGS line.
# 	[1992/09/23  09:35:40  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:25:22  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:35:52  zeliff]
# 
# Revision 1.1  1992/01/19  14:58:48  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:12:38 $
#

LIBRARIES	= libdump.a 
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I../../h
CFLAGS		+= -Dyy ${${TARGET_MACHINE}_CFLAGS}
libdump.a_OFILES	= d20getdsa.o \
	                  d20restore.o \
                          d20hdlsub.o  d20gendn.o \
	                  d20getdns.o  d20rdwr.o    d20dpfile.o \
                          d20search.o d20gen.o   

.include <${RULES_MK}>
