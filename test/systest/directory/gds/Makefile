# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/11  02:40:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:06:17  marty]
#
# 	Embedded copyright notice
# 	[1993/02/04  21:46:33  cjd]
#
# Revision 1.1.6.1  1995/12/11  21:57:31  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  19:03 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/3  1994/04/01  16:05 UTC  truitt
# 
# 	HP revision /main/HPDCE01/truitt_junk/2  1994/04/01  15:52 UTC  truitt
# 	Support for gds tet files
# 
# 	HP revision /main/HPDCE01/truitt_junk/1  1994/03/18  17:08 UTC  truitt
# 	Update to latest tet version of gds test
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  19:36  giza
# 	merge kk & hpdce01
# 	[1995/12/11  20:57:15  root]
# 
# 	trying to submit
# 	[1993/08/30  18:06:55  giza]
# 
# 	fixed IDIRs for TET
# 	[1993/08/27  21:16:43  giza]
# 
# Revision 1.1.1.6  1993/05/04  22:20:20  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.4.3  1993/11/03  21:49:02  mhickey
# 	Added gds_xds_str_001 to the subdirs list.
# 
# 	defect 9209.
# 	[1993/11/03  21:48:50  mhickey]
# 
# Revision 1.1.4.2  1993/09/21  14:24:48  pellis
# 	Uncommented SUBDIRS line so that dcegdshd will get built.
# 	[1993/09/21  14:16:46  pellis]
# 
# Revision 1.1.4.1  1993/09/20  22:51:31  pellis
# 	Moved dcegdshd to ./systest/directory/gds/dcegdshd/*.
# 	[1993/09/20  22:51:11  pellis]
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

tet_code_IDIR		= /tet/gds/
tet_scen_IDIR		= /tet/gds/
tetbuild.cfg_IDIR	= /tet/gds/
tetclean.cfg_IDIR	= /tet/gds/
tetexec.cfg_IDIR	= /tet/gds/

SUBDIRS = dcegdshd gds_xds_str_001

SCRIPTS = tet_code tet_scen tetbuild.cfg tetclean.cfg tetexec.cfg

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
