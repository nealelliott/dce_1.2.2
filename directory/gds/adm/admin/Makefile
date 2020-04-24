#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.820.2  1996/03/09  23:12:43  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:53  marty]
#
# Revision 1.1.820.1  1995/12/08  15:15:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:51:36  root]
# 
# Revision 1.1.818.5  1994/08/23  15:27:35  keutel
# 	OT 10919: gda_util ---> cdsutil
# 	[1994/08/23  14:17:48  keutel]
# 
# Revision 1.1.818.4  1994/07/14  19:23:54  keutel
# 	added: gdsditadm depends on libs
# 	[1994/07/14  19:07:57  keutel]
# 
# Revision 1.1.818.3  1994/05/10  15:49:22  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:09:53  marrek]
# 
# Revision 1.1.818.2  1994/03/23  15:05:04  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:07:55  keutel]
# 
# Revision 1.1.818.1  1994/02/22  15:58:09  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:26:23  marrek]
# 
# Revision 1.1.816.2  1993/10/14  16:46:22  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:42:58  keutel]
# 
# Revision 1.1.816.1  1993/10/13  17:29:25  keutel
# 	October 1993 code drop (CR 3100)
# 	[1993/10/13  11:56:13  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/09  18:56:26  keutel]
# 
# Revision 1.1.7.3  1993/08/10  09:50:44  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:25:22  marrek]
# 
# Revision 1.1.7.2  1993/07/02  15:06:23  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:34:53  marrek]
# 
# Revision 1.1.5.4  1992/12/31  17:48:55  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:03:23  bbelch]
# 
# Revision 1.1.5.3  1992/12/15  15:22:03  marrek
# 	Add machine dependent Makefile
# 	[1992/12/15  12:24:19  marrek]
# 
# 	Insert -DSYS5ATT in cget.o_CFLAGS
# 	remove will happen during 1.1 code cleanup.
# 	[1992/12/10  14:50:07  marrek]
# 
# Revision 1.1.5.2  1992/11/27  13:07:07  marrek
# 	November 1992 code drop
# 	[1992/11/16  14:05:42  marrek]
# 
# Revision 1.1.2.3  1992/06/01  21:50:19  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  19:14:26  zeliff]
# 
# Revision 1.1.2.2  1992/04/27  21:36:27  melman
# 	Removed -DSYS5ATT from cget.o_CFLAGS.
# 	[1992/04/27  21:35:55  melman]
# 
# Revision 1.1  1992/01/19  15:00:46  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.820.2 $ $Date: 1996/03/09 23:12:43 $
#

# for picking up d20m_init.c and d20frame.c
VPATH		= ../usradmin:${TARGET_MACHINE}
INCFLAGS	= -I../../h

PROGRAMS = gdsditadm
ILIST	= gdsditadm
IDIR	= /bin/

gdsditadm: libadmv2e.a libadm_msk.a

gdsditadm_LIBS = -lcurses -ladmv2e -ladm_msk -ladmsch -ltree -ldupd -ldump \
	         -laplv2 -lcdsutil -ltermcap ${LIBS} 
.if ${TARGET_MACHINE} == "HP800"
d20frame.o_CFLAGS = -Aa ${CFLAGS}
.endif
gdsditadm_OFILES = d20frame.o

LIBRARIES	= libadmv2e.a libadm_msk.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
d20admin.o_CFLAGS = ${CFLAGS}
d20objname.o_CFLAGS = ${CFLAGS}
d20tnsx.o_CFLAGS = -DVERS2 -D_HETERO ${CFLAGS}
cget.o_CFLAGS	= -DM_SYSV ${CFLAGS}
d20trans.o_CFLAGS = -DNODEBUG ${CFLAGS}
d20msk21.o_CFLAGS = ${CFLAGS}

libadmv2e.a_OFILES = \
	d20acl.o d20admin.o d20admobj.o d20objname.o \
	d20getat.o d20modattr.o d20displst.o d20util.o \
	d20trans.o d20tnsx.o \
	d20logon.o d20gstruct.o d20gdnname.o d20hdlacl.o \
	d20getsh.o d20m_init.o d20schem.o  d20msk21.o d20msk22.o \
	d20spec_msk.o d20mhs_msk.o

libadm_msk.a_OFILES = \
	cget.o d20hdlmsk.o d20mskmsg.o

.include <${RULES_MK}>

.if exists (${TARGET_MACHINE}/Makefile)
.include <${TARGET_MACHINE}/Makefile>
.endif
