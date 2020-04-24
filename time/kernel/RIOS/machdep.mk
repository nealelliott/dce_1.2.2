#
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
# Revision 1.1.8.2  1996/03/09  23:31:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:35  marty]
#
# Revision 1.1.8.1  1995/12/08  18:10:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:24:45  root]
# 
# Revision 1.1.6.1  1994/05/31  21:29:14  cbrooks
# 	CR 10611 - DTS needs new ACL manager
# 	[1994/05/31  21:03:55  cbrooks]
# 
# Revision 1.1.4.2  1992/12/30  20:40:53  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:09:25  htf]
# 
# Revision 1.1.2.3  1992/04/07  19:20:53  mckeen
# 	Merged up to mainline.
# 	[1992/03/31  16:38:59  mckeen]
# 
# 	Removed :P directive from rm rules
# 	[1992/03/13  15:48:32  mckeen]
# 
# Revision 1.1.2.2  1992/02/25  19:10:55  jim
# 	Changes to build dtsloadobj here where it belongs.
# 	[1992/02/25  17:30:40  jim]
# 
# Revision 1.1  1992/01/19  15:33:45  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
#  ALL RIGHTS RESERVED
# 
#
# build kernel extension and others

PROGRAMS = load_kern_ext config_kern_ext unload_kern_ext \
	   query_kern_ext dtsloadobj

OBJECTS = dtskernext

OTHERS = linkke

ILIST = dtskernext load_kern_ext config_kern_ext unload_kern_ext \
	query_kern_ext dtsloadobj

dtsloadobj_LDFLAGS    = -edtsloadobj -bexport:${dtsload.exp:P} -bI:${dts.exp:P}

CCTYPE = traditional

dtskernext: kern_utctime.o

linkke: dtskernext kern_utctime.exp kern_utctime.imp
	${_LD_} -o dtskernext -D0 -H512 -T512 kern_utctime.o \
	        -bI:/lib/kernex.exp -bI:/lib/syscalls.exp \
	        -bI:${kern_utctime.exp:P} -bE:${kern_utctime.imp:P} \
                -e config_dts_kern


dtsloadobj: dtsload.exp dts.exp

clobber_dtskernext:
	-${RM} kern_utctime.o

clobber_kern_utctime.exp:
	-${RM} kern_utctime.exp

clobber_kern_utctime.imp:
	-${RM} kern_utctime.imp
