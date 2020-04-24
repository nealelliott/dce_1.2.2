#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.109.2  1996/03/11  01:46:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:16:29  marty]
#
# Revision 1.2.109.1  1995/12/08  22:12:14  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:45:04  root]
# 
# Revision 1.2.107.9  1994/06/21  14:28:41  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:47:06  marrek]
# 
# Revision 1.2.107.8  1994/06/02  16:26:47  zeliff
# 	ot10763: add IDIR
# 	[1994/06/02  15:53:37  zeliff]
# 
# Revision 1.2.107.7  1994/05/10  16:39:41  marrek
# 	Remove SUBDIRS and DATA_SUBDIRS. The testcases are in the
# 	process of being moved to test/functional/directory/xds/ts .
# 	[1994/05/10  16:39:18  marrek]
# 
# Revision 1.2.107.6  1994/03/23  16:06:33  keutel
# 	March 1994 code drop
# 	[1994/03/21  18:14:45  keutel]
# 
# Revision 1.2.107.5  1994/02/25  15:02:32  keutel
# 	OT 10012: tests.xoms was not installed
# 	[1994/02/25  14:18:44  keutel]
# 
# Revision 1.2.107.4  1994/02/24  16:45:18  marrek
# 	SITE switch for SNI Burlington.
# 	[1994/02/24  13:30:22  marrek]
# 
# Revision 1.2.107.3  1994/02/22  18:38:56  ohara
# 	Merged with changes from 1.2.107.2
# 	[1994/02/22  18:38:30  ohara]
# 
# 	use -Xa on SINIX
# 	[1994/02/18  16:05:37  ohara]
# 
# Revision 1.2.107.2  1994/02/22  15:09:13  marrek
# 	Add -ld23.
# 	[1994/02/22  15:08:45  marrek]
# 
# 	Changes for libdce.
# 	[1994/02/10  17:05:02  marrek]
# 
# 	Code drop for dce1.1.
# 	[1994/02/07  13:28:02  marrek]
# 
# Revision 1.2.107.1  1994/01/13  09:18:40  marrek
# 	Add -DSNI to SVR4_CFLAGS.
# 	[1994/01/12  12:08:34  marrek]
# 
# Revision 1.2.8.4  1993/10/15  20:13:04  ohara
# 	Restoring file to 1.2.8.2 contents
# 	[1993/10/15  20:12:47  ohara]
# 
# Revision 1.2.8.2  1993/10/14  19:24:31  keutel
# 	October 1993 code drop
# 	[1993/10/14  19:23:17  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/11  12:57:30  keutel]
# 
# Revision 1.2.8.1  1993/09/16  08:13:45  keutel
# 	on non-SVR4 platforms only libdce is linked
# 	[1993/09/16  08:12:01  keutel]
# 
# Revision 1.2.6.2  1993/08/12  12:15:36  marrek
# 	Remove unused libraries in xt_test_LIBS for -ldce.
# 	[1993/08/06  14:00:14  marrek]
# 
# 	July 1993 code drop.
# 	[1993/08/04  08:43:13  marrek]
# 
# Revision 1.2.4.5  1993/02/22  20:42:39  treff
# 	DCT @ OSF
# 	Added "DATA_SUBDIRS = " to move datafiles
# 	into obj tree, where they belong
# 	[1993/02/22  20:42:19  treff]
# 
# Revision 1.2.4.4  1993/01/05  19:59:47  htf
# 	Embedding copyright notice
# 	[1993/01/05  18:17:13  htf]
# 
# Revision 1.2.4.3  1992/12/01  14:57:25  marrek
# 	November 1992 code drop
# 	[1992/12/01  13:16:50  marrek]
# 
# Revision 1.2.4.2  1992/11/02  15:33:56  root
# 	Added SUBDIRS and ILIST for install pass
# 	[1992/10/28  20:14:31  root]
# 
# Revision 1.2.2.4  1992/07/01  03:25:11  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  23:03:42  melman]
# 
# Revision 1.2.2.3  1992/06/11  21:52:40  melman
# 	New GDS test code drop.
# 	[1992/06/11  21:46:07  melman]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.109.2 $ $Date: 1996/03/11 01:46:10 $
#

.if ${SITE} == "SNI-Munich"
.BEGIN:
	rm -f xt_test.o # to get date and time info in executable
.endif

#LIBRARIES	= 
PROGRAMS	= xt_test
LIST		= xt_test
ILIST		= ${PROGRAMS}
IDIR		= /test/tet/functional/directory/xds/tools/
#SUBDIRS		= examples

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES/^/export_}

INCFLAGS	= -I../../../directory/xds/h -I../../../directory/xom/h -I../../../directory/gds/h 

.if !defined(GDS_NO_LIBDCE)
RIOS_CFLAGS	+= -D_THREAD_SAFE
xt_test_LIBS = -ldce

xt_test.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_file.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_run.o_CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_rValue.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_omFuncs.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_dsFuncs.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_tree.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 
xt_thread.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE -DTHREADSAFE 

xt_test_OFILES = xt_file.o xt_run.o xt_rValue.o xt_omFuncs.o \
xt_dsFuncs.o xt_tree.o xt_thread.o xt_test.o

xt_test.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_file.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h xt_parms.h
xt_run.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_rValue.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_omFuncs.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_dsFuncs.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_tree.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_thread.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h
xt_test.o: xt.h xt_thread.h xt_thread_struct.h xt_len.h

.else # GDS_NO_LIBDCE
xt_test_LIBS = -lXDS -lXOM -lxoms -ldua -lslog -ld27 -ld23 -losiaddr

xt_dsFuncs.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_file.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_nonthread.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_omFuncs.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_run.o_CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_rValue.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_test.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE
xt_tree.o_CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} -DTRACE

xt_test_OFILES = xt_file.o xt_run.o xt_rValue.o xt_omFuncs.o \
xt_dsFuncs.o xt_tree.o xt_nonthread.o xt_test.o


xt_dsFuncs.o: xt.h xt_len.h xt_nonthread.h
xt_file.o: xt.h xt_len.h xt_nonthread.h xt_parms.h
xt_omFuncs.o: xt.h xt_len.h xt_nonthread.h
xt_run.o: xt.h xt_len.h xt_nonthread.h
xt_rValue.o: xt.h xt_len.h xt_nonthread.h
xt_test.o: xt.h xt_len.h xt_nonthread.h
xt_tree.o: xt.h xt_len.h xt_nonthread.h

.endif # GDS_NO_LIBDCE

.include <${RULES_MK}>
.include <osf.debugvar.mk>
