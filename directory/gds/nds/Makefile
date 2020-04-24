#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.177.2  1996/03/09  23:14:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:40  marty]
#
# Revision 1.1.177.1  1995/12/08  16:05:45  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/12/05  20:56 UTC  psn
# 	Fix to build on AIX.
# 
# 	HP revision /main/HPDCE02/4  1995/08/02  21:27 UTC  dtruong
# 	Replaced FSF with HP800 for INCFLAGS
# 
# 	[1995/08/02  21:25 UTC  dtruong  /main/HPDCE02/dtruong_mothra1.1d/1]
# 	      Replaced FSF with HP800 for INCFLAGS
# 	HP revision /main/HPDCE02/3  1994/08/17  22:15 UTC  dtruong
# 	Merge from Hpdce02_01
# 	[1995/12/08  15:16:14  root]
# 
# Revision 1.1.175.8  1994/07/21  14:01:51  marrek
# 	Add FSF to INCFLAGS.
# 	[1994/07/21  14:01:25  marrek]
# 
# Revision 1.1.175.7  1994/07/11  21:06:10  keutel
# 	OT 10732: ndscomp runs during installation
# 	[1994/07/11  21:03:08  keutel]
# 
# Revision 1.1.175.6  1994/05/10  15:57:08  marrek
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:23:42  marrek]
# 
# 	April 1994 submission.
# 	[1994/04/29  10:53:06  marrek]
# 
# Revision 1.1.175.5  1994/03/23  15:25:41  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:33:00  keutel]
# 
# Revision 1.1.175.4  1994/02/23  18:09:40  keutel
# 	OT 9995 : on RIOS libNDS links libcma
# 	[1994/02/23  18:09:28  keutel]
# 
# Revision 1.1.175.3  1994/02/23  16:55:42  keutel
# 	OT 9997: added use of inlib on OSF1
# 	[1994/02/23  16:41:08  keutel]
# 
# Revision 1.1.175.2  1994/02/22  17:41:25  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:59:51  marrek]
# 
# Revision 1.1.175.1  1994/01/13  09:18:27  marrek
# 	Add -ly -ll -lgen to libNDS.so_LIBS for SVR4.
# 	[1994/01/12  12:05:36  marrek]
# 
# Revision 1.1.172.6  1993/10/26  12:21:44  zeliff
# 	OT9253: export and install libNDS.so on at386
# 	[1993/10/26  12:14:37  zeliff]
# 
# Revision 1.1.172.5  1993/10/25  20:30:47  zeliff
# 	OT9253: make libNDS.so a shared library on OSF1
# 	[1993/10/25  20:30:28  zeliff]
# 
# Revision 1.1.172.4  1993/10/20  14:35:10  zeliff
# 	OT9203 - libNDS exported to wrong libraries on RIOS,HP
# 	[1993/10/20  14:19:33  zeliff]
# 
# Revision 1.1.172.3  1993/10/19  17:12:22  keutel
# 	CR 9126
# 	[1993/10/19  17:10:28  keutel]
# 
# Revision 1.1.172.2  1993/10/14  17:36:01  keutel
# 	October 1993 code drop
# 	[1993/10/14  17:34:37  keutel]
# 
# Revision 1.1.172.1  1993/10/13  17:33:24  keutel
# 	October 1993 code drop
# 	[1993/10/09  16:50:52  keutel]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.177.2 $ $Date: 1996/03/09 23:14:23 $
#

LOCALFILES	= NDSCONF.dat

PROGRAMS	= ndscomp

VPATH           = ${TARGET_MACHINE}
YFLAGS		= -dtv

IDIR	= /bin/

LIBFLAGS	= ${LIBDIRS}
HP800_INCFLAGS	= -IHP800
RIOS_INCFLAGS	= -IFSF
INCFLAGS	= -I../h ${${TARGET_MACHINE}_INCFLAGS}

ndsshmatt.o_CFLAGS = ${CFLAGS}
ndscompil.o_CFLAGS = -D DEBUG ${CFLAGS}
y.tab.o_CFLAGS = -DDEBUGYACC ${CFLAGS}
# the following statement should be enabled only if debugging of the lex-
# code is necessary
#lex.yy.o_CFLAGS = -DDEBUGLEX ${CFLAGS}
 
NDSCONF.dat_IDIR = /share/var/adm/directory/gds/adm/

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

ndsreg1.c:   ndsreg.c
	rm -f $@
	cp $> $@

.include <${RULES_MK}>

y.tab.c:	ndscomp.y
	yacc ${YFLAGS} ${ndscomp.y:P}

y.tab.o:	y.tab.c lex.yy.c

lex.yy.c:	ndscomp.l
	lex ${LXFLAGS} ${ndscomp.l:P}

ndscompil.o:	y.tab.c

ndsshr.exp: ndsshr.dat
	rm -f $@
	cp $> $@
