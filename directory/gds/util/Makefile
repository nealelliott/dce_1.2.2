#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.14.2  1996/03/09  23:14:52  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:10  marty]
#
# Revision 1.1.14.1  1995/12/08  16:15:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:21:08  root]
# 
# Revision 1.1.12.5  1994/09/19  08:39:42  marrek
# 	Fixes for OT12176 (wrong incore tables.)
# 	[1994/09/19  08:30:59  marrek]
# 
# Revision 1.1.12.4  1994/06/21  14:48:30  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:47:08  marrek]
# 
# Revision 1.1.12.3  1994/05/10  16:03:35  marrek
# 	Bug fixes.
# 	[1994/04/29  09:28:47  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  13:13:02  marrek]
# 
# Revision 1.1.12.2  1994/03/23  15:47:27  keutel
# 	d27signal.c added
# 	[1994/03/22  14:17:04  keutel]
# 
# Revision 1.1.12.1  1994/02/22  17:55:36  marrek
# 	Remove CFLAGS; -DTHREADSAFE is set globally.
# 	[1994/02/22  10:52:20  marrek]
# 
# 	February 1994 code drop
# 	[1994/02/17  16:17:03  marrek]
# 
# 	February 1994 code drop
# 	[1994/02/16  11:14:56  marrek]
# 
# 	February 1994 code drop
# 	[1994/02/15  14:13:56  marrek]
# 
# 	Changes for libdce.
# 	[1994/02/10  17:04:44  marrek]
# 
# 	Code drop for dce1.1.
# 	[1994/02/08  11:53:16  marrek]
# 
# Revision 1.1.8.2  1993/10/04  17:37:53  keutel
# 	use of libdua instead of libdce for gds processes
# 	[1993/10/04  17:36:01  keutel]
# 
# Revision 1.1.8.1  1993/09/13  20:40:37  damon
# 	Synched with ODE 2.1 based build
# 	[1993/09/13  20:40:20  damon]
# 
# Revision 1.1.8.2  1993/09/10  07:55:48  marrek
# 	Replace tabs by blanks before comment.
# 	[1993/09/10  07:38:36  marrek]
# 
# Revision 1.1.6.3  1993/08/27  08:30:30  marrek
# 	Insert GDS_NO_THREADSAFE part and remove empty
# 	d27util.o_CFLAGS and d27logb.o_CFLAGS.
# 	[1993/08/24  07:57:35  marrek]
# 
# Revision 1.1.6.2  1993/08/11  14:26:32  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:11:46  marrek]
# 
# Revision 1.1.4.2  1992/12/31  22:04:53  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:48:33  bbelch]
# 
# Revision 1.1.2.2  1992/06/01  23:10:41  melman
# 	New GDS merged drop
# 	[1992/05/29  12:34:03  melman]
# 
# Revision 1.1  1992/01/19  15:10:29  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.14.2 $ $Date: 1996/03/09 23:14:52 $
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
##########################################################
#
VPATH		= ../sams

libd27_OFILES	= ${libd27.a_OFILES}
libd27.a_OFILES	= \
		 d27svc.o d27util.o d27pfm.o d27sort.o d27close.o  \
		 d27copy.o d27prmsg.o d27obid.o d27syn2rep.o \
		 d27sntx.o d27cbl.o d27t61.o d27shid.o d27dme.o \
		 d27charvar.o d27charconv.o d27charinit.o d27signal.o \
		 dcegdssvc.o d27conv.o dcegdsmsg.o

#
# The rest of the Makefile is private for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#

LIBRARIES       = libd27.a

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS	= ${libd27_OFILES}
INCFLAGS	= -I../h 


.include <${RULES_MK}>

.endif
