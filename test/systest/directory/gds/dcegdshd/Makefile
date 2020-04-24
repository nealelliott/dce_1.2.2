# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/11  02:41:13  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:06:21  marty]
#
# 	Embedded copyright notice
# 	[1993/02/04  21:46:33  cjd]
#
# Revision 1.1.6.1  1995/12/11  21:57:38  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/30  15:57 UTC  dat
# 	remove dump.o object for view_obj which can't be found and does not seem to be necessary
# 
# 	HP revision /main/HPDCE02/1  1994/11/09  20:22 UTC  bissen
# 	merge from HPDCE01
# 
# 	HP revision /main/HPDCE01/1  1994/03/18  17:15 UTC  truitt
# 
# 	HP revision /main/truitt_junk/1  1994/03/18  17:08 UTC  truitt
# 	Update Makefile to support latest tet version of tests
# 	[1995/12/11  20:57:21  root]
# 
# Revision 1.1.4.2  1994/03/23  16:06:56  keutel
# 	xt_dump no longer exists
# 	[1994/03/21  18:14:30  keutel]
# 
# Revision 1.1.4.1  1994/02/24  21:23:04  pellis
# 	Changed dump.c to xt_dump.c.
# 	[1994/02/24  21:22:52  pellis]
# 
# Revision 1.1.2.6  1993/11/01  16:42:01  pellis
# 	Fixed defect 9301 - Removed gdsSetup, gdsdirinfo.awk, gdskill, gdsmod,
# 	setup.i.gds and worldSetup.gds from OTHERS, they have all been moved to
# 	tools by mhickey except for gdsmod and setup.i.gds which have been
# 	deleted.
# 	[1993/11/01  16:41:49  pellis]
# 
# Revision 1.1.2.5  1993/09/23  20:24:27  pellis
# 	Moved gdsSetup, gdsmod, worldSetup.gds, gdsdirinfo.awk, sTest.gds,
# 	gdskill and setup.i.gds from ./systest/tools.
# 	[1993/09/23  20:23:38  pellis]
# 
# Revision 1.1.2.4  1993/09/21  16:17:12  pellis
# 	Fixed problems with building on an AIX.
# 	[1993/09/21  16:16:47  pellis]
# 
# Revision 1.1.2.3  1993/09/21  14:29:55  pellis
# 	Fixed missing line continuation problem.
# 	[1993/09/21  14:29:41  pellis]
# 
# Revision 1.1.2.2  1993/09/21  14:24:49  pellis
# 	Moved to ./test/systest/directory/gds/dcegdshd/* and fixed problem
# 	with dump.c not finding ./directory/gds/h/d2dir.h.
# 	[1993/09/21  14:24:13  pellis]
# 
# Revision 1.1.2.1  1993/09/20  22:40:37  pellis
# 	Moved from ./systest/directory/gds/*.
# 	[1993/09/20  22:16:56  pellis]
# 
# Revision 1.1.2.3  1993/04/01  20:17:37  mhickey
# 	Removed all lines pertaining to building stuff other
# 	than view_obj, and removed everything from LIBS line except for
# 	lindce, since nothing else is needed.
# 
# 	Defect 7071.
# 	[1993/04/01  20:15:47  mhickey]
# 
# 	Removed temporary test program references.
# 	[1992/06/24  21:38:39  gmd]
# 
# 	Re-add dcecdnam_rd to PROGRAMS. Still a useful program for dcecdnam.
# 	[1992/06/24  21:03:47  gmd]
# 
# 	Removed dcecdnam_rd from PROGRAMS list. Not used and therefore not worth
# 	updating with pipe workaround for OT 4147.
# 	[1992/06/19  14:46:36  gmd]
# 
# 	Added dcecdnam_co program. It is a companion process for dcecdnam_pg
# 	and dcecdnam_cl that accepts commands via pipes, executes them via
# 	system() and returns status via pipes. Note that it is single-
# 	threaded and NOT linked with dce. In this way, we avoid the problem
# 	of system() using unwrapped fork() in a multi-threaded program. We
# 	ALSO avoid bumping against lack of atfork() support in rpc (OT 4326).
# 	[1992/06/18  18:44:46  gmd]
# 
# 	Fix INCFLAGS so that most compiles only -I to ../tools, and only 3 files
# 	-I to directory/{xds,xom}/h.
# 
# 	Defect 4050.
# 	[1992/06/09  21:28:29  mhickey]
# 
# 	Add -g compiler option
# 	[1992/04/22  22:03:24  rps]
# 
# 	Use tools libs
# 	[1992/04/15  15:43:30  rps]
# 
# 	Remove -no_epv from IDLFLAGS
# 	[1992/03/23  20:17:35  rps]
# 
# 	Shrink INCFLAGS to prevent rios compiler fault
# 	[1992/03/18  01:05:53  rps]
# 
# Revision 1.1.2.2  1993/03/31  21:56:45  mhickey
# 	Initial checkin for defect 7071.
# 	[1993/03/31  21:52:29  mhickey]
# 
# 	Initial ODE Makefile for Directory services.
# 	[1992/03/05  22:21:23  akers]
# 
# Revision 1.1.6.4  1993/02/05  15:18:28  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:37:47  cjd]
# 
# Revision 1.1.6.3  1993/01/22  15:19:20  pellis
# 	Made many changes to prepare for moving unsupported files to the
# 	./src/nosupport/test/systest directory.
# 	[1993/01/21  22:13:47  pellis]
# 
# Revision 1.1.6.2  1992/12/31  19:10:16  pellis
# 	Added "OTHERS = cds_abuse cds_obj_only dcegdsh1" and "ILIST =
# 	${PROGRAMS} ${OTHERS}" so these tests would be included in the
# 	dcetest install tree.
# 	[1992/12/31  18:25:20  pellis]
# 
# $EndLog$

VPATH           = ../../../../directory/${TARGET_MACHINE}\
		  :../../../../../test/directory/:../../../tools\
          	  :../../../../../test/directory/xds

PROGRAMS = view_obj 

view_obj_OFILES     =  view_obj.o shd_util.o

INCFLAGS        =  -I../../../tools/

shd_util.o_INCFLAGS = ${INCFLAGS} -I../../../../../directory/xom/h
view_obj.o_INCFLAGS = ${INCFLAGS} -I../../../../../directory/xom/h

Alter_object.sv_IDIR  = /tet/gds/ts/dcegdshd/
Create_object.sv_IDIR = /tet/gds/ts/dcegdshd/
Create_shadow.sv_IDIR = /tet/gds/ts/dcegdshd/
Delete_shadow.sv_IDIR = /tet/gds/ts/dcegdshd/
Remove_object.sv_IDIR = /tet/gds/ts/dcegdshd/
dcegdsh1_IDIR         = /tet/gds/ts/dcegdshd/
dcegdshd_IDIR         = /tet/gds/ts/dcegdshd/
gds.data_IDIR         = /tet/gds/ts/dcegdshd/
gds.functions_IDIR    = /tet/gds/ts/dcegdshd/
gdsSetup_IDIR         = /tet/gds/ts/dcegdshd/
gdsdirinfo.awk_IDIR   = /tet/gds/ts/dcegdshd/
gdskill_IDIR          = /tet/gds/ts/dcegdshd/
gdstest_tet.ksh_IDIR  = /tet/gds/ts/dcegdshd/
sTest.gds_IDIR        = /tet/gds/ts/dcegdshd/
worldSetup.gds_IDIR   = /tet/gds/ts/dcegdshd/
view_obj_IDIR         = /tet/gds/ts/dcegdshd/

CFLAGS	= -D_BSD -Dunix -DIDL_NO_PROTOTYPES ${${TARGET_MACHINE}_CFLAGS}
.if ${TARGET_OS} != "AIX"
CFLAGS += +e
.endif

LDFLAGS         = ${${TARGET_MACHINE}_LDFLAGS}

dcecdnam_co_LIBS = ${${TARGET_MACHINE}_LIBS}
 
LIBS            = -ldce ${${TARGET_MACHINE}_LIBS}

OTHERS		= Alter_object.sv Create_object.sv Create_shadow.sv \
		  Delete_shadow.sv Remove_object.sv dcegdsh1 dcegdshd \
		  gds.data gds.functions gdsSetup gdsdirinfo.awk \
		  gdskill gdstest_tet.ksh sTest.gds worldSetup.gds

ILIST		= ${PROGRAMS} ${OTHERS}

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
