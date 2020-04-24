# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:24:00  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:12:30  marty]
#
# Revision 1.1.4.1  1995/12/08  17:46:43  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/14  14:07 UTC  root
# 	code cleanup- new compiler complains about redundant -Aa
# 	[1995/12/08  16:24:52  root]
# 
# Revision 1.1.2.3  1994/06/21  14:48:14  marrek
# 	June 1994 code submission.
# 	[1994/06/17  11:40:02  marrek]
# 
# Revision 1.1.2.2  1994/06/10  21:16:20  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:54:22  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  16:15:01  marrek
# 	Created for April Submission.
# 	[1994/04/26  12:50:05  marrek]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:00 $
#

LIBRARIES	= libtsi.a
PROGRAMS	= gdscmxl

ILIST		= gdscmxl 
IDIR		= /bin/
INCFLAGS        = -I../h
EXPDIR		= /usr/include/dce/
libtsi.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

.ifdef (${TARGET_MACHINE} == "RIOS")
cmxl.o_CCTYPE	= traditional
.endif

HP800_CFLAGS	+= -D_HPUX_SOURCE -D_XPG2 -DPCMX -DNDS_LNK_ACCESS
RIOS_CFLAGS	+= -D_BSD -D_POSIX_SOURCE -DPCMX -DAIX -DNDS_LNK_ACCESS
AT386_CFLAGS	+= -DPCMX -DNDS_LNK_ACCESS
SVR4_CFLAGS	+= -KPIC -D_STC_ -DPCMX -DNDS_LNK_ACCESS
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS}

gdscmxl_OFILES     = cmxl.o cmxlup.o prxd.o
libtsi.a_OFILES	=  \
		cmx.o cmx2k.o cmx3.o cmx_tlv.o cmxinet.o \
		cmxinup.o cmxliz.o cmxmap.o cmxopt.o cmxsna.o \
		cmxtrace.o cmxxad.o cmxxrd.o cmxxti.o cmxxtr.o \
		cmxxup.o emiface.o

.include <${RULES_MK}>

