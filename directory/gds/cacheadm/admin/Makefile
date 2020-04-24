#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.814.2  1996/03/09  23:13:12  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:24  marty]
#
# Revision 1.1.814.1  1995/12/08  15:28:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:41  root]
# 
# Revision 1.1.812.4  1994/08/23  15:27:39  keutel
# 	OT 10919: gda_util ---> cdsutil
# 	[1994/08/23  14:17:52  keutel]
# 
# Revision 1.1.812.3  1994/05/10  15:51:28  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:17:42  marrek]
# 
# Revision 1.1.812.2  1994/03/23  15:08:52  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:18:23  keutel]
# 
# Revision 1.1.812.1  1994/02/22  16:03:46  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:56:16  marrek]
# 
# Revision 1.1.810.2  1993/10/14  16:46:44  keutel
# 	October 1993 code drop
# 	[1993/10/14  16:45:39  keutel]
# 
# Revision 1.1.810.1  1993/10/13  17:30:08  keutel
# 	October 1993 code drop (CR 3100)
# 	[1993/10/13  12:06:27  keutel]
# 
# Revision 1.1.7.3  1993/08/11  08:15:51  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:17:06  marrek]
# 
# Revision 1.1.7.2  1993/07/02  15:08:05  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:35:05  marrek]
# 
# Revision 1.1.5.5  1992/12/31  18:49:08  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:14:48  bbelch]
# 
# Revision 1.1.5.4  1992/11/27  17:18:55  marrek
# 	November 1992 code drop
# 	[1992/11/17  16:39:52  marrek]
# 
# Revision 1.1.5.3  1992/10/05  17:17:31  marrek
# 	Changes for bug 5205.
# 	[1992/10/05  17:16:29  marrek]
# 
# Revision 1.1.5.2  1992/09/29  21:09:58  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:23:07  treff]
# 
# Revision 1.1.2.3  1992/06/01  23:33:49  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  20:39:12  zeliff]
# 
# Revision 1.1.2.2  1992/04/28  14:25:33  melman
# 	Removed -DSYS5ATT from cget.o_CFLAGS.
# 	[1992/04/28  14:18:01  melman]
# 
# Revision 1.1  1992/01/19  15:14:28  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.814.2 $ $Date: 1996/03/09 23:13:12 $
#

# for picking up sources
VPATH		= ../../adm/admin:../../adm/admin/${TARGET_MACHINE}:../../adm/usradmin
INCFLAGS	= -I../../h

PROGRAMS = gdscacheadm
ILIST = gdscacheadm
IDIR = /bin/

gdscacheadm_LIBS = -lcurses -ladmv2ec -ladmschc -ldupdc \
	           -laplv2c -lcdsutil -ltermcap ${LIBS}
.if ${TARGET_MACHINE} == "HP800"
d20frame.o_CFLAGS = -Aa ${CFLAGS}
.endif
gdscacheadm_OFILES = d20frame.o  

LIBRARIES	= libadmv2ec.a
EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
GARBAGE		= d20aclc.c d20adminc.c d20displstc.c d20logonc.c d20objnamec.c
d20aclc.o_CFLAGS = -DCACHE_ADMIN ${CFLAGS}
d20admin.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20adminc.o_CFLAGS = -DCACHE_ADMIN -DDIRV2 ${CFLAGS}
d20displstc.o_CFLAGS = -DCACHE_ADMIN ${CFLAGS}
d20logonc.o_CFLAGS = -DCACHE_ADMIN ${CFLAGS}
d20objname.o_CFLAGS = -DDIRV2 ${CFLAGS}
d20objnamec.o_CFLAGS = -DCACHE_ADMIN -DDIRV2 ${CFLAGS}
d20tnsx.o_CFLAGS = -DVERS2 -D_HETERO ${CFLAGS}
cget.o_CFLAGS = -DM_SYSV ${CFLAGS}
d20trans.o_CFLAGS = -DNODEBUG ${CFLAGS}
d20msk21.o_CFLAGS = -DDIRV2 ${CFLAGS}

libadmv2ec.a_OFILES = \
        d20aclc.o d20adminc.o d20objnamec.o d20admobj.o     \
	d20getat.o d20displstc.o d20util.o              \
	d20trans.o d20tnsx.o d20msk21.o d20mskmsg.o d20hdlmsk.o d20logonc.o  \
	cget.o d20gstruct.o d20gdnname.o d20hdlacl.o    \
	d20getsh.o d20m_init.o d20schem.o d20msk22.o d20spec_msk.o d20mhs_msk.o

.include <${RULES_MK}>

d20aclc.c:	d20acl.c
	-${RM} ${_RMFLAGS_} $@
	-${CP} $> $@

d20adminc.c:	d20admin.c
	-${RM} ${_RMFLAGS_} $@
	-${CP} $> $@

d20displstc.c:	d20displst.c
	-${RM} ${_RMFLAGS_} $@
	-${CP} $> $@

d20logonc.c:	d20logon.c
	-${RM} ${_RMFLAGS_} $@
	-${CP} $> $@

d20objnamec.c:	d20objname.c
	-${RM} ${_RMFLAGS_} $@
	-${CP} $> $@
