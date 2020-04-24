# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.16.2  1996/03/09  20:52:38  marty
# 	Add OSF copyright
# 	[1996/03/09  20:30:11  marty]
#
# Revision 1.1.16.1  1995/12/11  22:50:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/07/05  18:32 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/2  1994/04/01  18:51 UTC  truitt
# 
# 	HP revision /main/HPDCE01/truitt_junk/1  1994/04/01  18:42 UTC  truitt
# 	Cleanup file
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  20:57  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  22:23:23  root]
# 
# Revision 1.1.8.2  1993/07/09  15:25:24  root
# 	Initial King Kong branch
# 	[1993/07/09  15:24:58  root]
# 
# Revision 1.1.5.8  1993/04/23  12:44:38  truitt
# 	We are going to keep dcetmchg around, since we have the test
# 	working.  There is a problem with DTS, however, that will
# 	keep us from integrating this test into our suite at this time.
# 	[1993/04/23  12:44:20  truitt]
# 
# Revision 1.1.1.9  1993/04/21  20:29:24  root
# 	    Loading drop DCE1_0_2B23
# 
# Revision 1.1.5.7  1993/03/30  21:00:49  truitt
# 	Added support for new dcetmchg system test.
# 	[1993/03/30  21:00:15  truitt]
# 
# Revision 1.1.5.5  1993/04/06  14:16:15  pellis
# 	The testcase dcetmchg was moved to the nosupport directory so it
# 	needed to be deleted from OTHERS.
# 	[1993/04/06  14:13:32  pellis]
# 
# Revision 1.1.5.4  1993/02/05  15:27:26  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:48:32  cjd]
# 
# Revision 1.1.5.6  1993/03/17  19:10:13  jake
# 	Merged for Jim "I wear my butt as a hat" Truitt
# 	[1993/03/17  18:41:13  jake]
# 
# Revision 1.1.1.7  1993/03/11  22:15:24  osfrcs
# 	Loading drop DCE1_0_2B18
# 	Revision 1.1.3.10  1993/01/08  20:24:03  mgm
# 	Added ifdef for instrumented build
# 	[1993/01/08  20:23:42  mgm]
# 
# Revision 1.1.3.9  1992/11/25  20:15:58  mgm
# 	Added tet* to install
# 	[1992/11/25  20:11:35  mgm]
# 
# Revision 1.1.3.8  1992/11/23  17:36:11  mgm
# 	Removed -lm from LIBS for building with shared libraries
# 	[1992/11/23  12:33:26  mgm]
# 
# Revision 1.1.3.7  1992/11/20  21:02:40  mgm
# 	Added tet_scen to install
# 	[1992/11/20  21:02:19  mgm]
# 
# Revision 1.1.3.6  1992/11/17  13:56:14  mgm
# 	Added rpt.time and run.time to install.
# 	[1992/11/17  13:55:47  mgm]
# 
# Revision 1.1.3.5  1992/11/11  21:27:04  mgm
# 	Can now install to release area from ODE
# 	[1992/11/11  21:26:23  mgm]
# 
# Revision 1.1.3.4  1992/11/05  21:41:37  mgm
# 	changed the LIBS line to include the math library (couldn't
# 	find fabs symbol):
# 	LIBS            = -ldce -lxx_ ${${TARGET_MACHINE}_LIBS} -lm
# 	[1992/11/05  21:41:04  mgm]
# 
# Revision 1.1.3.3  1992/11/03  18:02:46  mgm
# 	Added changes for TET info. Moved -ldce to end of LIBS line.
# 	[1992/11/03  18:02:13  mgm]
# 
# Revision 1.1.3.2  1992/06/11  20:04:38  mcfarland
# 	changed files from test_latest
# 	[1992/06/09  13:11:03  mcfarland]
# 
# 	Use tools libs
# 	[1992/04/15  15:41:56  rps]
# 
# 	Initial version
# 	[1992/03/17  23:04:25  rps]
# 
# Revision 1.1.1.2  1992/05/28  15:18:07  root
# 	    Loading drop Dce1_0_1b15
# 
# $EndLog$

addtime_IDIR	= /tet/time/ts/
subtime_IDIR	= /tet/time/ts/
dcetm.ksh_IDIR	= /tet/time/ts/

dcetmsyn_IDIR	= /tet/time/ts/dcetmsyn/
dcetmudt_IDIR	= /tet/time/ts/dcetmudt/
dcetmchg_IDIR	= /tet/time/ts/dcetmchg/

tet_code_IDIR		= /tet/time/
tet_scen_IDIR		= /tet/time/
tetbuild.cfg_IDIR	= /tet/time/
tetclean.cfg_IDIR	= /tet/time/
tetexec.cfg_IDIR	= /tet/time/

ILIST		= ${PROGRAMS} ${SCRIPTS}
IMODE		= 777

SCRIPTS		= dcetm.ksh dcetmsyn dcetmudt dcetmchg \
		  tet_scen tet_code tetbuild.cfg tetclean.cfg tetexec.cfg

PROGRAMS	= addtime subtime

CFLAGS		= -g ${${TARGET_MACHINE}_CFLAGS}

LIBFLAGS	= -L../tools

# Need -lm for building with archived libraries
# (like the instrumented build)

.ifdef INSTR
LIBS          	= -lxx_ ${${TARGET_MACHINE}_LIBS} -ldce -lm
.else
LIBS          	= -lxx_ ${${TARGET_MACHINE}_LIBS} -ldce
.endif

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
