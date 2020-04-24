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
# Revision 1.1.4.2  1996/03/09  23:13:36  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:54  marty]
#
# Revision 1.1.4.1  1995/12/08  15:41:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:05:44  root]
# 
# Revision 1.1.2.2  1994/07/06  15:53:43  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:40:28  marrek]
# 
# Revision 1.1.2.1  1994/06/21  14:39:34  marrek
# 	Created in June 94 code submission.
# 	[1994/06/10  16:04:14  marrek]
# 
# $EndLog$
#
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:36 $
#

LIBRARIES += libdsal.a

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

ILIST	+= libdsal.a
libdsal.a_IDIR	= /usr/lib/
libdsal.a_EXPDIR	= /usr/lib/
libdsal.a_IMODE	= 755
libdsal.a_OFILES = d26dsal.o

d26dsal.o_LIBS	= -bE:d26dsal.exp -bM:SRE -lc -lPW -T512 -H512
d26dsal.o_OFILES	= d26phon.o

d26dsal.o: ${d26dsal.o_OFILES} d26dsal.exp
	${_LD_} -o d26dsal.o ${LIBDIRS} ${d26dsal.o_OFILES} ${d26dsal.o_LIBS} 

libdsal.a: d26dsal.o
	${_AR_} -rv libdsal.a d26dsal.o

d26dsal.exp: d26dsal.dat
	rm -f $@
	cp $> $@

