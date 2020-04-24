#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.24.2  1996/03/11  02:49:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:57  marty]
#
# Revision 1.1.24.1  1995/12/11  22:50:32  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/29  20:53 UTC  psn
# 	Remove the missing killrpoc2 from Makefile.
# 
# 	HP revision /main/HPDCE02/3  1994/10/12  21:01 UTC  bissen
# 	Merge Hpdce02_04 to HPDCE02 branch
# 	[1995/12/11  22:23:31  root]
# 
# Revision 1.1.21.2  1994/09/01  19:25:03  petero
# 	add gdsDump to files to be exported
# 	[1994/09/01  19:24:45  petero]
# 
# 	HP revision /main/HPDCE02/2  1994/08/29  15:25 UTC  bissen
# 	Remove referenences to old PROGRAMS
# 
# 	HP revision /main/HPDCE02/1  1994/07/05  18:41 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# Revision 1.1.21.1  1994/05/09  21:57:25  pellis
# 	Moved the ropen command to ./src/nosupport/test/systest/tools.
# 	[1994/05/09  21:54:16  pellis]
# 
# 	HP revision /main/HPDCE01/3  1994/04/11  15:21 UTC  truitt
# 
# 	HP revision /main/HPDCE01/truitt_junk/2  1994/04/11  15:13 UTC  truitt
# 	get rid of gds-specific files
# 
# 	HP revision /main/HPDCE01/truitt_junk/1  1994/04/04  13:46 UTC  truitt
# 	Update for latest version of tet files
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  15:32  bissen
# 	merge kk and hpdce01
# 
# Revision 1.1.13.3  1993/08/27  21:07:38  giza
# 	fixed IDIRs for TET
# 	[1993/08/27  21:07:18  giza]
# 
# Revision 1.1.13.2  1993/07/09  15:42:59  root
# 	Initial King Kong branch
# 	[1993/07/09  15:42:30  root]
# 
# Revision 1.1.11.2  1993/04/23  17:54:07  sekhar
# 	Merged Hp_dce1_0_1 changes
# 	[1993/04/23  16:56:01  sekhar]
# 
# 	Embedded copyright notice
# 	[1993/02/05  13:15:59  cjd]
# 
# Revision 1.1.1.9  1993/04/21  20:29:29  root
# 	    Loading drop DCE1_0_2B23
# 
# Revision 1.1.7.9  1993/04/03  05:11:47  mhickey
# 	Added gds systest stuff.
# 
# 	Defect 7071
# 	[1993/04/03  05:11:12  mhickey]
# 
# Revision 1.1.7.8  1993/02/05  15:27:43  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:48:49  cjd]
# 
# Revision 1.1.9.2  1993/02/05  13:15:59  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.7.7  1993/01/29  19:34:39  pellis
# 	Added xx_* to ILIST for tests not using systest.functions.
# 	[1993/01/29  19:34:13  pellis]
# 
# Revision 1.1.7.6  1993/01/27  16:14:58  pellis
# 	Fix for CR# 7029: ./src/test/systest/tools/Makefile doesn't know how
# 			  to make sec.functions & machinfo.
# 	[1993/01/27  16:14:18  pellis]
# 
# Revision 1.1.7.5  1993/01/22  15:32:31  pellis
# 	Made many changes to prepare for moving unsupported files to the
# 	./src/nosupport/test/systest directory.
# 	[1993/01/21  22:15:16  pellis]
# 
# Revision 1.1.3.5  1993/01/08  19:40:59  mgm
# 	Added ifdef for instrumented build.
# 	[1993/01/08  19:40:00  mgm]
# 
# Revision 1.1.7.4  1993/01/07  20:10:18  ramb
# 	Add 'systest.functions' to OTHERS list
# 	[1993/01/07  19:43:20  ramb]
# 
# Revision 1.1.3.4  1992/12/18  19:40:01  mgm
# 	Added killproc2 to scripts
# 	[1992/12/18  19:39:39  mgm]
# 
# Revision 1.1.3.3  1992/11/09  13:42:39  mgm
# 	Updated for HPUX
# 	[1992/11/09  13:41:53  mgm]
# 
# Revision 1.1.7.3  1992/11/04  18:18:16  root
# 	Comment out systest.functions until it is ready for submission
# 	[1992/11/04  18:17:27  root]
# 
# Revision 1.1.7.2  1992/11/02  15:38:51  root
# 	Added OTHERS listing files to be moved to install area and added ILIST
# 	required for the install pass.
# 	[1992/10/28  20:41:52  root]
# 
# Revision 1.1.2.11  1992/06/04  15:16:10  mhickey
# 	Added the line
# 
# 	   EXPLIB_TARGETS = ${LIBRARIES}
# 
# 	so that the libraries get built, but not exported,
# 	replaced the use of osf.genliblns.mk with the actual
# 	XX_LIBS lines for all of the programs, and
# 	reorganized a little bit for readability.
# 
# 	Defect 3990.
# 	[1992/06/03  19:11:15  mhickey]
# 
# Revision 1.1.2.10  1992/05/21  16:41:50  gmd
# 	Removed su.dce from list of executables to build
# 	[1992/05/15  20:44:17  gmd]
# 
# Revision 1.1.2.9  1992/04/30  22:34:04  rps
# 	Link only programs with library list
# 	Flush get_cell (use supported getcellname instead)
# 	[1992/04/30  22:31:36  rps]
# 
# Revision 1.1.2.8  1992/04/15  17:17:43  rps
# 	Fix merge
# 	[1992/04/15  17:16:31  rps]
# 
# Revision 1.1.2.7  1992/04/15  16:57:34  rps
# 	Fixed libs, move dependencies after RULES_MK
# 	Yank kinit for now (missing krb5 syms on rios)
# 	[1992/04/15  15:20:58  rps]
# 
# Revision 1.1.2.6  1992/04/09  15:55:47  eperkins
# 	Added the following changes as suggested by Rob Stanzel:
# 	Transposed ropen and ropend rules
# 	Added -g to CFLAGS
# 	Added util library rules
# 	Added EXPLIB_TARGETS rule so that tool libraies are now not exported
# 	[1992/04/09  15:54:54  eperkins]
# 
# Revision 1.1.2.5  1992/03/19  19:29:38  eperkins
# 	Added EXPLIB = ${LIBARIES}
# 	[1992/03/19  19:29:00  eperkins]
# 
# Revision 1.1.2.4  1992/03/17  21:40:13  eperkins
# 	Added INCLUDES defintion and idl file rules
# 	[1992/03/17  21:39:31  eperkins]
# 
# Revision 1.1.2.3  1992/03/16  22:06:20  eperkins
# 	Added rmq library and executable rules
# 	[1992/03/16  22:05:33  eperkins]
# 
# Revision 1.1.2.2  1992/02/13  21:12:06  eperkins
# 	Initial version
# 	[1992/02/13  21:11:39  eperkins]
# 
# $EndLog$

IDIR = /tet/tools/

chkproc_IDIR           = /tet/tools/
expmachinfo_IDIR       = /tet/tools/
gethost_IDIR           = /tet/tools/
getproc_IDIR           = /tet/tools/
grd_IDIR               = /tet/tools/
killproc_IDIR          = /tet/tools/
lookup_IDIR            = /tet/tools/
lrd_IDIR               = /tet/tools/
lrd_ut_IDIR            = /tet/tools/
lrdsnd_IDIR            = /tet/tools/
mkpath_IDIR            = /tet/tools/
msgqd_IDIR             = /tet/tools/
msgrcv_IDIR            = /tet/tools/
msgsnd_IDIR            = /tet/tools/
rcheck_IDIR            = /tet/tools/
rmqd_IDIR              = /tet/tools/
rmqsnd_IDIR            = /tet/tools/
ropen_IDIR             = /tet/tools/
ropend_IDIR            = /tet/tools/
rshsp_IDIR             = /tet/tools/
su.dce_IDIR            = /tet/tools/
xx_begin_IDIR          = /tet/tools/
xx_error_IDIR          = /tet/tools/
xx_examine_IDIR        = /tet/tools/
xx_fail_IDIR           = /tet/tools/
xx_log_IDIR            = /tet/tools/
xx_pass_IDIR           = /tet/tools/
xx_severe_IDIR         = /tet/tools/
xx_warning_IDIR        = /tet/tools/
rc.ropen_IDIR          = /tet/tools/
systest.functions_IDIR = /tet/tools/
xx_lib.h_IDIR          = /tet/tools/
xx_lib_r.c_IDIR        = /tet/tools/

ILIST		= ${PROGRAMS} ${OTHERS}

INCFLAGS = -I. -I../../../security/client/login 

RIOS_CFLAGS	= -qlanglvl=ansi -D_ALL_SOURCE 
PMAX_CFLAGS	= 

CFLAGS		= -g -DOSF_DCE -D_BSD -Dunix -D_NONSTD_TYPES -U__STR__ \
		  -DIDL_PROTOTYPES -DNIDL_PROTOTYPES \
		  -DSYSTYPE_${TARGET_MACHINE} \
		  -DDEBUG ${${TARGET_MACHINE}_CFLAGS} 

LIBFLAGS	= -L../tools

.ifdef INSTR
ALL_PROGLIBS = -lxx_ -lrmq -lmsgq -lBSD -ldce -lcmax
.else
ALL_PROGLIBS = -lxx_ -lrmq -lmsgq -lBSD -ldce
.endif

IDLFLAGS = -keep c_source

PROGRAMS = msgqd msgsnd msgrcv

.if ${TARGET_OS} == "HPUX"
BSD_DEF = -lBSD
.else
BSD_DEF = -lbsd
.endif

.if ${TARGET_MACHINE}=="SVR4"
SVR4_CFLAGS = -U_BSD
msgqd_LIBS = -lxx_ -lmsgq
msgsnd_LIBS = -lxx_ -lmsgq
msgrcv_LIBS = -lxx_ -lmsgq
.else
msgqd_LIBS = -ldce -lxx_ -lmsgq ${BSD_DEF}
msgsnd_LIBS = -ldce -lxx_ -lmsgq ${BSD_DEF}
msgrcv_LIBS = -ldce -lxx_ -lmsgq ${BSD_DEF}
.endif

LIBRARIES	        = libxx_.a libmsgq.a libutil.a
EXPLIB_TARGETS          = ${LIBRARIES}
libxx_.a_OFILES		= xx_lib_r.o 
libmsgq.a_OFILES	= msgq_ut.o msgrcv_bsd.o msgsnd_bsd.o 
libutil.a_OFILES        = util.o

OTHERS = expmachinfo mkpath rshsp chkproc getproc killproc \
	 systest.functions xx_begin \
	 xx_examine xx_lib.h xx_log xx_severe xx_error xx_fail \
	 xx_lib_r.c xx_pass xx_warning su.dce \
	 rcheck conf_util.sh dce.ps gdsDump

.include <${RULES_MK}>
