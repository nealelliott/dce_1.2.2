#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/09  23:13:15  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:28  marty]
#
# Revision 1.1.12.1  1995/12/08  15:28:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:43  root]
# 
# Revision 1.1.10.2  1994/06/21  14:44:47  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:29:04  marrek]
# 
# Revision 1.1.10.1  1994/02/22  16:03:50  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:56:40  marrek]
# 
# Revision 1.1.8.1  1993/10/28  13:33:02  keutel
# 	CR 9279
# 	[1993/10/28  13:24:28  keutel]
# 
# Revision 1.1.6.3  1993/08/11  12:52:17  marrek
# 	July 1993 code drop.
# 	[1993/07/30  13:38:13  marrek]
# 
# Revision 1.1.6.2  1993/07/02  15:08:14  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:35:16  marrek]
# 
# Revision 1.1.4.2  1992/12/31  18:49:18  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:14:58  bbelch]
# 
# Revision 1.1.2.2  1992/06/01  23:35:36  zeliff
# 	Added the RIOS_CFLAGS so that dev_t will be defined in the
# 	/usr/include/sys/types.h on a RIOS.
# 		RIOS_CFLAGS = -D_POSIX_SOURCE
# 	[1992/06/01  18:46:36  zeliff]
# 
# 	New version of file from GDS drop
# 	[1992/05/28  20:39:35  zeliff]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/09 23:13:15 $
#

LIBRARIES	= libisam.a
libisam.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
isam.h_EXPDIR	= /usr/include/dce/
RIOS_CFLAGS	+= -D_POSIX_SOURCE
CFLAGS		+= -DISAM_LOCK -DSINIX="" ${${TARGET_MACHINE}_CFLAGS}

libisam.a_OFILES = \
        isbuild.o isclustr.o isopen.o isread.o iswrite.o \
	isaudit.o istrans.o isbtree.o isnode.o isitem.o \
	iskey.o islock.o issupp.o isbuf.o isvalid.o isdef.o \
	isutil1.o isutil2.o isasmb.o

.include <${RULES_MK}>
